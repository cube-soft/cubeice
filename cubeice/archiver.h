/* ------------------------------------------------------------------------- */
/*
 *  archiver.h
 *
 *  Copyright (c) 2010 CubeSoft Inc. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see < http://www.gnu.org/licenses/ >.
 *
 *  Last-modified: Tue 28 Dec 2010 20:42:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBE_ARCHIVER_H
#define CUBE_ARCHIVER_H

// 粒度の細かいタイマーを使用する
#ifndef CLX_USE_HIGH_PRECISION_TIMER
#define CLX_USE_HIGH_PRECISION_TIMER
#endif

#include <string>
#include <vector>
#include <tchar.h>
#include <shlwapi.h>
#include <clx/hexdump.h>
#include <clx/strip.h>
#include <clx/split.h>
#include <clx/timer.h>
#include <babel/babel.h>
#include "wpopen.h"
#include "user-setting.h"
#include "dialog.h"

#define CUBEICE_ENGINE _T("7z.exe")

namespace cubeice {
	/* --------------------------------------------------------------------- */
	//  archiver
	/* --------------------------------------------------------------------- */
	class archiver {
	public:
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef std::size_t size_type;
		typedef cubeice::user_setting setting_type;
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		explicit archiver(const setting_type& setting) :
			setting_(setting) {}
		
		/* ----------------------------------------------------------------- */
		//  compress
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void compress(InputIterator first, InputIterator last) {
			static const string_type keyword = _T("Compressing");
			
			bool pass = false;
			
			// オプションを読み飛ばす．
			if (first->compare(0, 3, _T("/c:")) != 0) return;
			string_type filetype(first->substr(3));
			for (;first != last && first->at(0) == _T('/'); ++first) {
				if (first->compare(0, 2, _T("/p")) == 0) pass = true;
			}
			
			// 保存先パスの決定
			string_type dest = this->compress_path(setting_.compression(), *first, filetype);
			if (dest.empty()) return;
			
			// パスワードの設定．
			if (pass && password_dialog() == IDCANCEL) return;
			
			// 一時ファイルのパスを決定
			string_type tmp = tmpfile(_T("cubeice"));
			if (tmp.empty()) return;
			
			// 上書きの確認
			if (setting_.compression().output_condition() != OUTPUT_RUNTIME) {
				int result = this->is_overwrite(dest, tmp, setting_.compression(), 0);
				result &= ~ID_TO_ALL;
				if (result != IDOK && result != IDYES) return;
			}
			
			// プログレスバーの設定
			cubeice::dialog::progressbar progress;
			size_type n = this->compress_size(first, last, 5.0);
			char msgbuf[32] = {};
			double step = (n > 0) ? (progress.maximum() - progress.minimum()) / static_cast<double>(n) : 0.0;
			if (n == 0) progress.marquee(true);
			
			// コマンドラインの作成
			std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
			cmdline += _T(" a");
			if (filetype == _T("exe")) cmdline += _T(" -sfx7z.sfx");
			else if (filetype == _T("gzip") || filetype == _T("bzip2")) cmdline += _T(" -ttar");
			else cmdline += _T(" -t") + filetype;
			if (pass) cmdline += _T(" -p") + cubeice::password();
			cmdline += _T(" -bd -scsWIN -y \"") + tmp + _T("\"");
			for (; first != last; ++first) cmdline += _T(" \"") + *first + _T("\"");
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) return;
			
			// メイン処理
			int interval = 1;
			int status = 0;
			string_type line;
			while ((status = proc.gets(line)) >= 0) {
				progress.refresh();
				if (progress.is_cancel()) {
					proc.close();
					break;
				}
				
				if (status == 2) break; // pipe closed
				else if (status == 0 || line.empty()) {
					Sleep(interval);
					if (interval < 100) interval *= 2;
					continue;
				}
				assert(status == 1);
				interval = 1;
				
				string_type::size_type pos = line.find(keyword);
				if (pos == string_type::npos || line.size() <= keyword.size()) continue;
				
				string_type filename = clx::strip_copy(line.substr(pos + keyword.size()));
				string_type message = dest + _T("\n") + filename;
				progress.text(message);
				if (n > 0) progress += step;
			}
			
			if (status == 2) {
				if (filetype == _T("gzip") || filetype == _T("bzip2")) this->compress_tar(tmp, dest, filetype, pass);
				else MoveFile(tmp.c_str(), dest.c_str());
				
				// フィルタリング
				if ((setting_.compression().details() & DETAIL_FILTER) && !setting_.filters().empty()) {
					this->compress_filter(dest, setting_.filters());
				}
				
				// フォルダを開く
				if (setting_.compression().details() & DETAIL_OPEN) {
					string_type root = dest.substr(0, dest.find_last_of(_T('\\')));
					if ((setting_.compression().details() & DETAIL_SKIP_DESKTOP) == 0 || !this->is_desktop(root)) {
						ShellExecute(NULL, _T("open"), root.c_str(), NULL, NULL, SW_SHOWNORMAL);
					}
				}
			}
			
			if (PathFileExists(tmp.c_str())) DeleteFile(tmp.c_str());
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  decompress
		 *
		 *  NOTE: 複数の圧縮ファイルが指定された場合は，1ファイルずつ
		 *  解凍処理を実行する．
		 */
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void decompress(InputIterator first, InputIterator last) {
			static const string_type keyword = _T("Extracting");
			babel::init_babel();
			
			// レジストリの設定を無視するコマンドかどうか．
			string_type force;
			if (first->compare(0, 3, _T("/x:")) == 0) force = first->substr(3);
			
			// オプションを読み飛ばす．
			while (first != last && first->at(0) == _T('/')) ++first;
			
			for (; first != last; ++first) {
				string_type src = *first;
				
				// 保存先パスの取得
				string_type root = this->decompress_path(setting_.decompression(), src, force);
				if (root.empty()) break;
				
				// パスワードの設定
				bool pass = this->is_password(src);
				if (pass && password_dialog() == IDCANCEL) break;
				
				// フォルダの作成
				if (!PathFileExists(root.c_str()) && CreateDirectory(root.c_str(), NULL) == 0) break;
				
				// 一時フォルダの作成
				string_type tmp = tmpdir(_T("cubeice"));
				if (tmp.empty()) break;
				
				// プログレスバーの初期化
				cubeice::dialog::progressbar progress;
				progress.text(root);
				
				// *.tar 系の処理
				if (this->is_tar(src)) src = this->decompress_tar(src, tmp, pass);
				
				// プログレスバーの進行度の設定
				size_type n = this->decompress_size(src, 5.0);
				double step = (n > 0) ? (progress.maximum() - progress.minimum()) / static_cast<double>(n) : 0.0;
				if (n == 0) progress.marquee(true);
				
				// コマンドラインの生成
				string_type cmdline = CUBEICE_ENGINE;
				cmdline += _T(" x -bd -scsWIN -y");
				if (pass) cmdline += _T(" -p") + cubeice::password();
				cmdline += _T(" -o\"") + tmp + _T("\"");
				cmdline += _T(" \"") + src + _T("\"");
				cube::popen proc;
				if (!proc.open(cmdline.c_str(), _T("r"))) return;
				
				// メイン処理
				int interval = 1;
				int status = 0;
				int to_all = 0; // 全ての項目に適用する処理
				string_type line;
				while ((status = proc.gets(line)) >= 0) {
					progress.refresh();
					if (progress.is_cancel()) {
						proc.close();
						break;
					}
					
					if (status == 2) break; // pipe closed
					else if (status == 0 || line.empty()) {
						Sleep(interval);
						if (interval < 100) interval *= 2;
						continue;
					}
					assert(status == 1);
					interval = 1;
					
					string_type::size_type pos = line.find(keyword);
					if (pos == string_type::npos || line.size() <= keyword.size()) continue;
					
					string_type filename = clx::strip_copy(line.substr(pos + keyword.size()));
					
					// 上書きの確認
					int result = this->is_overwrite(root + _T('\\') + filename, tmp + _T('\\') + filename, setting_.decompression(), to_all);
					if ((result & ID_TO_ALL)) {
						result &= ~ID_TO_ALL;
						to_all = result;
					}
					if (result == IDCANCEL) break;
					else if (result == IDNO) continue;
					
					// フィルタリング
					string_type message = root + _T("\n");
					if ((setting_.decompression().details() & DETAIL_FILTER) && this->is_filter(filename, setting_.filters())) {
						message += _T("Filtering: ");
					}
					else this->move(tmp + _T('\\') + filename, root + _T('\\') + filename);
					
					// TODO: ここの root + _T('\\') + filename の方の filename を文字コード変換する．
					message += filename;
					progress.text(message);
					if (n > 0) progress += step;
				}
				
				// フォルダを開く
				if (setting_.decompression().details() & DETAIL_OPEN) {
					if ((setting_.decompression().details() & DETAIL_SKIP_DESKTOP) == 0 || !this->is_desktop(root)) {
						ShellExecute(NULL, _T("open"), root.c_str(), NULL, NULL, SW_SHOWNORMAL);
					}
				}
			}
		}
		
	private: // compress_xxx
		/* ----------------------------------------------------------------- */
		//  compress_path
		/* ----------------------------------------------------------------- */
		string_type compress_path(const setting_type::archive_type& setting, const string_type& src, const string_type& filetype) {
			string_type dest =  this->rootdir(setting, src);
			if (dest.empty()) {
				const TCHAR filter[] = _T("All files(*.*)\0*.*\0\0");
				string_type init = src.substr(0, src.find_last_of(_T('.'))) + this->extension(filetype);
				dest = cubeice::dialog::savefile(filter, init.c_str());
			}
			else {
				string_type::size_type first = src.find_last_of(_T('\\'));
				if (first == string_type::npos) first = 0;
				else ++first;
				string_type filename = src.substr(first);
				dest += _T('\\') + filename.substr(0, filename.find_last_of(_T('.'))) + this->extension(filetype);
			}
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_size
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		size_type compress_size(InputIterator first, InputIterator last, double timeout) {
			size_type n = 0;
			clx::timer t;
			for (; first != last; ++first) {
				if (PathIsDirectory(first->c_str())) n += compress_size_folder(*first);
				else ++n;
				if (t.elapsed() > timeout) return 0;
			}
			return n;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_tar
		/* ----------------------------------------------------------------- */
		void compress_tar(const string_type& src, const string_type& dest, const string_type& filetype, bool pass) {
			static const string_type keyword = _T("Compressing");
			
			string_type tar;
			string_type ext = dest.substr(dest.find_last_of(_T('.')));
			tar = dest.substr(0, dest.find_last_of(_T('.')));
			if (ext != _T(".gz") && ext != _T(".bz2")) tar += _T(".tar");
			if (!MoveFile(src.c_str(), tar.c_str())) return;
			
			std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
			cmdline += _T(" a -t") + filetype;
			if (pass) cmdline += _T(" -p") + cubeice::password();
			cmdline += _T(" -bd -scsWIN -y \"") + dest + _T("\"");
			cmdline += _T(" \"") + tar + _T("\"");
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) return;
			
			int status = 0;
			string_type line;
			while ((status = proc.gets(line)) >= 0) {
				if (status == 2) break; // pipe closed
				else if (status == 0 || line.empty()) {
					Sleep(10);
					continue;
				}
				assert(status == 1);
				
				string_type::size_type pos = line.find(keyword);
				if (pos == string_type::npos || line.size() <= keyword.size()) continue;
			}
			
			DeleteFile(tar.c_str());
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_size_folder
		/* ----------------------------------------------------------------- */
		size_type compress_size_folder(const string_type& root) {
			string_type path = root + _T("\\*.*");
			WIN32_FIND_DATA wfd = {};
			HANDLE handle = FindFirstFile(path.c_str(), &wfd);
			if (handle == INVALID_HANDLE_VALUE) return 1;
			
			size_type n = 0;
			do {
				if (_tcscmp(wfd.cFileName, _T(".")) != 0 && _tcscmp(wfd.cFileName, _T("..")) != 0) {
					if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
						string_type subdir = root + _T('\\') + wfd.cFileName;
						n += this->compress_size_folder(subdir);
					}
					else ++n;
				}
			} while (FindNextFile(handle, &wfd));
			FindClose(handle);
			
			return n;
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_filter
		/* ----------------------------------------------------------------- */
		void compress_filter(const string_type& path, const std::set<string_type>& filters) {
			std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
			cmdline += _T(" d -r -bd -scsWIN -y \"") + path + _T("\"");
			for (std::set<string_type>::const_iterator pos = filters.begin(); pos != filters.end(); ++pos) {
				cmdline += _T(" \"") + *pos + _T("\"");
			}
			
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) return;
			int status = 0;
			string_type line;
			while ((status = proc.gets(line)) >= 0) {
				if (status == 2) break; // closed pipe.
				else if (status == 0 || line.empty()) {
					Sleep(10);
					continue;
				}
				assert(status == 1);
			}
		}
		
	private: // decompress_xxx
		/* ----------------------------------------------------------------- */
		//  decompress_path
		/* ----------------------------------------------------------------- */
		string_type decompress_path(const setting_type::archive_type& setting, const string_type& src, string_type force) {
			// 保存先パスの決定
			string_type root = this->rootdir(setting, src, force);
			if (root.empty()) root = cubeice::dialog::browsefolder(_T("解凍するフォルダを指定して下さい。"));
			if (root.empty()) return root;
			
			// フォルダの作成
			if (setting.details() & DETAIL_CREATE_FOLDER) {
				string_type::size_type pos = src.find_last_of(_T('\\'));
				string_type filename = (pos == string_type::npos) ? src : src.substr(++pos);
				root += _T('\\') + filename.substr(0, filename.find_last_of(_T('.')));
				pos = root.find_last_of(_T('.'));
				if (pos != string_type::npos && root.substr(pos) == _T(".tar")) root.erase(pos);
			}
			return root;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_size
		/* ----------------------------------------------------------------- */
		size_type decompress_size(const string_type& path, double timeout) {
			string_type cmdline = CUBEICE_ENGINE;
			cmdline += _T(" l ");
			cmdline += path;
			
			clx::timer t;
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) return 0;
			string_type buffer, src;
			int status = 0;
			while ((status = proc.gets(buffer)) >= 0) {
				if (status == 2) break; // pipe closed
				else if (status == 1 && !buffer.empty()) src = buffer;
				buffer.clear();
				if (t.elapsed() > timeout) return 0;
			}
			
			std::vector<string_type> v;
			clx::split(src, v);
			if (v.size() < 5) return 0;
			
			try {
				return clx::lexical_cast<size_type>(v.at(2)) + clx::lexical_cast<size_type>(v.at(4));
			}
			catch (std::exception&) {}
			
			return 0;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_tar
		/* ----------------------------------------------------------------- */
		string_type decompress_tar(const string_type& src, const string_type& root, bool pass) {
			static const string_type keyword = _T("Extracting");
			
			string_type cmdline = CUBEICE_ENGINE;
			cmdline += _T(" x -bd -scsWIN -y");
			if (pass) cmdline += _T(" -p") + cubeice::password();
			cmdline += _T(" -o\"") + root + _T("\"");
			cmdline += _T(" \"") + src + _T("\"");
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) return src;
			
			int status = 0;
			string_type filename;
			string_type line;
			while ((status = proc.gets(line)) >= 0) {
				if (status == 2) break; // pipe closed
				else if (status == 0 || line.empty()) {
					Sleep(10);
					continue;
				}
				assert(status == 1);
				
				string_type::size_type pos = line.find(keyword);
				if (pos == string_type::npos || line.size() <= keyword.size()) continue;
				filename = clx::strip_copy(line.substr(pos + keyword.size()));
			}
			
			return root + _T('\\') + filename;
		}
		
	private: // others
		const setting_type& setting_;
		
		/* ----------------------------------------------------------------- */
		//  non-copyable
		/* ----------------------------------------------------------------- */
		archiver(const archiver& cp);
		archiver& operator=(const archiver& cp);
		
		/* ----------------------------------------------------------------- */
		/*
		 *  extension
		 *
		 *  NOTE: gzip, bz2 の場合は *.tar で返す．
		 */
		/* ----------------------------------------------------------------- */
		string_type extension(const string_type& filetype) {
			string_type ext = _T(".");
			if (filetype == _T("gzip")) ext += _T("tar.gz");
			else if (filetype == _T("bzip2")) ext += _T("tar.bz2");
			else ext += filetype;
			return ext;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  tmpfile
		 *
		 *  ランダムな一時ファイルを生成してパスを返す．
		 */
		/* ----------------------------------------------------------------- */
		string_type tmpfile(const string_type& prefix) {
			char_type dir[1024] = {};
			if (GetTempPath(1024, dir) == 0) return string_type();
			char_type path[2048] = {};
			if (GetTempFileName(dir, prefix.c_str(), 0, path) == 0) return string_type();
			
			// 一時ファイルが生成されているので削除する．
			if (PathFileExists(path)) DeleteFile(path);
			return string_type(path);
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  tmpdir
		 *
		 *  ラインダムな一時ディレクトリを生成してパスを返す．
		 */
		/* ----------------------------------------------------------------- */
		string_type tmpdir(const string_type& prefix) {
			string_type path = tmpfile(prefix);
			if (path.empty()) return path;
			if (CreateDirectory(path.c_str(), NULL) == 0) return string_type();
			return path;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  rootdir
		 *
		 *  ユーザ設定の値を元に出力先ディレクトリを取得する．特定の
		 *  ディレクトリを指定がチェックされており，かつ出力先が
		 *  空白の場合はデスクトップのパスを返す．
		 *
		 *  force に指定される可能性のあるものは以下の通り．
		 *   - runtime: 実行時に指定
		 *   - desktop: デスクトップ
		 *   - source: ソースファイルと同じ場所
		 */
		/* ----------------------------------------------------------------- */
		string_type rootdir(const setting_type::archive_type& setting, const string_type& src, const string_type& force = _T("")) {
			// デスクトップのパスを取得しておく．
			LPITEMIDLIST item;
			SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &item);
			char_type buffer[CUBE_MAX_PATH] = {};
			SHGetPathFromIDList(item, buffer);
			string_type desktop = buffer;
			
			if (force == _T("runtime")) return string_type();
			if (force == _T("desktop")) return desktop;
			else if (force == _T("source") || setting.output_condition() == OUTPUT_SOURCE) {
				// TODO: 相対パスの場合どうするか．
				return src.substr(0, src.find_last_of(_T('\\')));
			}
			else if (setting.output_condition() == OUTPUT_SPECIFIC) {
				if (!setting.output_path().empty()) return setting.output_path();
				else return desktop;
			}
			return string_type();
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  move
		 *
		 *  ファイルを移動させる．ただし，ディレクトリの場合は移動させる
		 *  代わりに同名のディレクトリを移動先に作成する．
		 */
		/* ----------------------------------------------------------------- */
		void move(const string_type& src, const string_type& dest) {
			if (PathIsDirectory(src.c_str())) CreateDirectory(dest.c_str(), NULL);
			else {
				string_type branch(dest.substr(0, dest.find_last_of(_T('\\'))));
				if (!PathFileExists(branch.c_str())) CreateDirectory(branch.c_str(), NULL);
				MoveFile(src.c_str(), dest.c_str());
			}
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  is_filter
		 *
		 *  ディレクトリ単位で区切り，それぞれに対してフィルタリング文字列
		 *  に該当するかどうかをチェックする．
		 *  TODO: ワイルドカードへの対応を考える．
		 */
		/* ----------------------------------------------------------------- */
		bool is_filter(const string_type& src, const std::set<string_type>& filters) {
			std::vector<string_type> v;
			clx::split_if(src, v, clx::is_any_of(_T("\\")));
			for (std::vector<string_type>::const_iterator pos = v.begin(); pos != v.end(); ++pos) {
				if (filters.find(*pos) != filters.end()) return true;
			}
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  is_older
		 *
		 *  ファイルの更新日時を比較する．
		 +/
		/* ----------------------------------------------------------------- */
		bool is_older(const string_type& target, const string_type& compared) {
			HANDLE h = CreateFile(target.c_str(), 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (h == INVALID_HANDLE_VALUE) return false;
			FILETIME t1 = {};
			BOOL status = GetFileTime(h, NULL, NULL, &t1);
			CloseHandle(h);
			if (status == FALSE) return false;
			
			h = CreateFile(compared.c_str(), 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (h == INVALID_HANDLE_VALUE) return false;
			FILETIME t2 = {};
			status = GetFileTime(h, NULL, NULL, &t2);
			if (status == FALSE) return false;
			
			if (CompareFileTime(&t1, &t2) < 0) return true;
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  is_overwrite
		 *
		 *  ファイルの上書きを行うかどうかを確認する．作成先に該当ファイル
		 *  が存在しない場合は IDOK を返す．
		 */
		/* ----------------------------------------------------------------- */
		int is_overwrite(const string_type& target, const string_type& compared, const setting_type::archive_type& setting, int force) {
			if ((setting.details() & DETAIL_OVERWRITE) && PathFileExists(target.c_str())) {
				if ((setting.details() & DETAIL_IGNORE_NEWER) && !is_older(target, compared)) return IDYES;
				else if (force > 0) return force;
				else {
					cubeice::overwrite() = target + _T(" は既に存在します。\r\n上書きしますか？");
					return cubeice::overwrite_dialog();
				}
			}
			return IDOK;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  is_password
		 *
		 *  アーカイブファイルにパスワードが設定されているかどうかを
		 *  判定する．
		 */
		/* ----------------------------------------------------------------- */
		bool is_password(const string_type& path) {
			static const string_type keyword(_T("Enter password"));
			static const string_type pass(_T("Testing"));
			
			string_type cmdline = CUBEICE_ENGINE;
			cmdline += _T(" t \"") + path + _T("\"");
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) return false;
			
			int n = 0;
			int status = 0;
			string_type line;
			while ((status = proc.gets(line)) >= 0) {
				if (status == 2) break; // closed pipe
				else if (status == 0 || line.empty()) {
					Sleep(10);
					continue;
				}
				assert(status == 1);
				if (line.find(keyword) != string_type::npos) return true;
				else if (line.find(pass) != string_type::npos) {
					if (++n >= 10) break;
				}
			}
			
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  is_tar
		 *
		 *  *.tar 系の圧縮ファイルかどうかを判定する．現状チェックしている
		 *  拡張子は，*.tar.gz, *.tar.bz2, *.tgz, *.tbz の 4種類．
		 */
		/* ----------------------------------------------------------------- */
		bool is_tar(const string_type& path) {
			string_type::size_type pos = path.find_last_of(_T('.'));
			if (pos == string_type::npos) return false;
			string_type ext = path.substr(pos);
			if (ext == _T(".gz") || ext == _T(".bz2") || ext == _T(".tgz") || ext == _T(".tbz")) return true;
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  is_desktop
		 *
		 *  指定されたパスがデスクトップかどうかを判定する．
		 */
		/* ----------------------------------------------------------------- */
		bool is_desktop(const string_type& path) {
			IMalloc* p = NULL;
			SHGetMalloc(&p);
			
			LPITEMIDLIST item = NULL;
			if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &item))) {
				char_type buffer[CUBE_MAX_PATH] = {};
				SHGetPathFromIDList(item, buffer);
				p->Free(item);
				p->Release();
				return path == string_type(buffer);
			}
			
			// not fouond
			p->Release();
			return false;
		}
	};
}

#endif // CUBE_ARCHIVER_H
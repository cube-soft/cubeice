/* ------------------------------------------------------------------------- */
/*
 *  archiver.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
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
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <clx/strip.h>
#include <clx/split.h>
#include <clx/replace.h>
#include <clx/date_time.h>
#include <clx/timer.h>
#include <psdotnet/logger.h>
#include <psdotnet/appender.h>
#include <babel/babel.h>
#include "wpopen.h"
#include "io.h"
#include "format.h"
#include "error.h"
#include "user-setting.h"
#include "pathmatch.h"
#include "dialog.h"
#include "sendmail.h"
#include "compressor.h"

#define CUBEICE_ENGINE _T("cubeice-exec.exe")
#define CUBEICE_MAXCOLUMN 45
#define MAIL_SUBJECT ""
#define MAIL_BODY ""

namespace cubeice {
	/* --------------------------------------------------------------------- */
	//  archiver
	/* --------------------------------------------------------------------- */
	class archiver {
	public:
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef __int64 size_type;
		typedef cubeice::user_setting setting_type;
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		explicit archiver(const setting_type& setting) :
			setting_(setting), input_files_(), files_(), size_(0), progress_() {}
		
		/* ----------------------------------------------------------------- */
		//  destructor
		/* ----------------------------------------------------------------- */
		~archiver() {
			if(!decomp_tmp_dir_.empty()) cubeice::removedir(decomp_tmp_dir_.c_str());
		}
		
		/* ----------------------------------------------------------------- */
		//  compress
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void compress(InputIterator first, InputIterator last) {
			static const string_type keyword = _T("Compressing");
			static const string_type error = _T("ERROR:");
			
			LOG_TRACE(_T("function archiver::compress() start"));
			
			bool pass = false;
			bool mail = false;
			
			// オプションを読み飛ばす．
			if (first->compare(0, 3, _T("/c:")) != 0) return;
			string_type filetype(first->substr(3));
			for (; first != last && first->at(0) == _T('/'); ++first) {
				if (first->compare(0, 2, _T("/p")) == 0) {
					pass = true;
					LOG_TRACE(_T("set password"));
				}
				
				if (first->compare(0, 2, _T("/m")) == 0) {
					mail = true;
					LOG_TRACE(_T("set mail-mode"));
				}
			}
			
			// NOTE: ドラッグ&ドロップの際にファイルが指定されなかった
			// (ショートカットをダブルクリックした）と想定する．
			// TODO: それ以外の場合も想定したメッセージを表示する必要があるかどうか．
			if (first == last) {
				string_type err(_T("圧縮したいファイルやフォルダを「CubeICE 圧縮」ショートカットにドラッグ&ドロップして下さい。"));
				LOG_ERROR(_T("message = %s"), err.c_str());
				MessageBox(NULL, err.c_str(), _T("CubeICE 圧縮"), MB_OK | MB_ICONINFORMATION);
				return;
			}
			
			string_type ext;
			string_type dest;
			std::vector<string_type> options;
			bool update = false;
			bool optar = false;
			if (filetype == _T("detail")) {
				LOG_TRACE(_T("set detail-mode"));
				cubeice::runtime_setting runtime;
				ext = cubeice::compressor::extension(first, last, runtime.type(), optar);
				dest = first->substr(0, first->find_last_of(_T('.'))) + ext;
				runtime.path() = dest;
				if (cubeice::dialog::runtime_setting(progress_.handle(), runtime) == IDCANCEL) return;
				
				// ランタイム時の設定を反映する
				runtime.save();
				dest = runtime.path();
				LOG_INFO(_T("dest = %s"), dest.c_str());
				
				filetype = runtime.type();
				update = runtime.update();
				ext = cubeice::compressor::extension(first, last, filetype, optar);
				if (filetype == _T("tgz")) filetype = _T("gzip");
				else if (filetype == _T("tbz")) filetype = _T("bzip2");
				LOG_INFO(_T("filetype = %s, ext = %s"), filetype.c_str(), ext.c_str());
				
				if (runtime.type() != _T("gzip")) options.push_back(_T("-mx=") + boost::lexical_cast<string_type>(runtime.level()));
				if (runtime.type() == _T("zip")) options.push_back(_T("mm=") + runtime.method());
				else if (runtime.type() == _T("7z")) options.push_back(_T("m0=") + runtime.method());
				if (runtime.type() != _T("gzip")) options.push_back(_T("-mmt=") + boost::lexical_cast<string_type>(runtime.thread_size()));
				if (runtime.enable_password()) {
					pass = true;
					cubeice::password() = runtime.password();
					LOG_INFO(_T("password = %s"), cubeice::password().c_str());
					if (runtime.type() == _T("zip")) options.push_back(_T("-mem=") + runtime.encoding());
				}
			}
			else {
				// 保存先パスの決定
				ext = cubeice::compressor::extension(first, last, filetype, optar);
				LOG_INFO(_T("filetype = %s, ext = %s"), filetype.c_str(), ext.c_str());
				
				dest = this->compress_path(setting_.compression(), *first, ext);
				LOG_INFO(_T("dest = %s"), dest.c_str());
				
				if (dest.empty()) return;
				
				// パスワードの設定．
				if (pass && cubeice::dialog::password(progress_.handle(), COMPRESS_FLAG) == IDCANCEL) return;
				LOG_INFO(_T("password = %s"), cubeice::password().c_str());
			}
			
			// 一時ファイルのパスを決定
			string_type tmp = update ? dest : cubeice::tmpfile(_T("cubeice"), false);
			if (update) LOG_INFO(_T("update-mode, path = %s"), tmp);
			if (tmp.empty()) return;
			
			progress_.style(cubeice::dialog::progressbar::simple);
			progress_.show();
			progress_.title(_T("0% - ") + this->filename(dest) + _T(" を圧縮しています - CubeICE"));
			progress_.marquee(true);
			
			// プログレスバーの設定
			this->compress_prepare_filelist(first, last);
			boost::thread	thr( boost::bind( &cubeice::archiver::compress_filelist, this ) );
			HANDLE th = thr.native_handle();
			SetPriorityClass(th, ABOVE_NORMAL_PRIORITY_CLASS);
			if (progress_.is_marquee()) progress_.marquee(false);
			
			// コマンドラインの作成
			std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
			if( update ) cmdline += _T(" u");
			else cmdline += _T(" a");
			if (filetype == _T("exe")) cmdline += _T(" -sfx7z.sfx");
			else if (optar) cmdline += _T(" -ttar");
			else cmdline += _T(" -t") + filetype;
			if (pass) cmdline += _T(" -p\"") + cubeice::password() + _T("\"");
			cmdline += _T(" -scsWIN -y \"") + tmp + _T("\"");
			for (InputIterator pos = first; pos != last; ++pos) cmdline += _T(" \"") + *pos + _T("\"");
			for(std::vector<string_type>::const_iterator pos = options.begin(); pos != options.end(); ++pos) cmdline += _T(' ') + *pos;
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) {
				LOG_ERROR(_T("proccess open"));
				return;
			}
			LOG_TRACE(_T("cmdline-7z = %s"), cmdline.c_str());
			
			// NOTE: marquee スタイルから復帰する際，バーが 1% まで進まないと再描画されない．
			// そのため，一瞬だけ 1% に進めて強制的に再描画を行っている．
			progress_.position(100);
			progress_.refresh();
			progress_.position(progress_.minimum());
			progress_.refresh();
			progress_.text(dest);
			
			// メイン処理
			int status = 0;
			string_type line;
			string_type report;
			unsigned int index = 0;
			int percent = 0;
			progress_.start();
			while ((status = proc.gets(line)) >= 0) {
				if (!this->refresh(proc)) break;
				if (status == 2) break; // pipe closed
				else if (status == 0 || line.empty()) {
					Sleep(10);
					continue;
				}
				assert(status == 1);
				
				string_type::size_type pos = line.find(error);
				if (pos != string_type::npos) {
					report += clx::strip_copy(line.substr(pos));
					if (pos != 0) report += _T(" (") + clx::strip_copy(line.substr(0, pos)) + _T(")");
					report += _T("\r\n");
					continue;
				}
				
				pos = line.find(_T(":%")); // :%n% と言う形で進捗率が表示される
				if (pos != string_type::npos) {
					string_type s = line.substr(pos);
					clx::strip_if(s, clx::is_any_of(_T(":% ")));
					line.erase(pos);
					int prev = percent;
					percent = _ttoi(s.c_str()) * 100;
					progress_.position(percent);
					if (percent > prev) {
						string_type title = s + _T("% - ") + this->filename(dest) + _T(" を圧縮しています - CubeICE");
						progress_.title(title);
					}
				}
				
				pos = line.find(keyword);
				if (pos == string_type::npos || line.size() <= keyword.size()) {
					if (!line.empty()) {
						LOG_TRACE(_T("UnknownMessage = %s"), line.c_str());
					}
					continue;
				}
				
				string_type filename = clx::strip_copy(line.substr(pos + keyword.size()));
				LOG_INFO(_T("Filename = %s"), filename.c_str());
				
				if (filename.size() > CUBEICE_MAXCOLUMN) {
					string_type::size_type startpos = filename.size() - CUBEICE_MAXCOLUMN;
					filename = _T("...") + filename.substr(startpos);
				}
				progress_.text(filename);
				
				if (index < files_.size() - 1) ++index;
				progress_.numcount();
			}
			
			if (status < 0) {
				LOG_ERROR(_T("Status = %d"), status);
				report += error + _T(" Broken pipe.\r\n");
			}
			
			if ((setting_.compression().details() & DETAIL_REPORT) && !report.empty()) {
				cubeice::dialog::report(progress_.handle(), report);
			}
			
			if (status == 2) {
				if (optar) {
					string_type prev = tmp;
					tmp = cubeice::tmpfile(_T("cubeice"), false);
					tmp = tmp.substr(0, tmp.find_last_of(_T('\\')) + 1);
					tmp += PathFindFileName(dest.c_str());
					this->compress_tar(prev, tmp, filetype, pass);
					if (PathFileExists(prev.c_str())) DeleteFile(prev.c_str());
				}
				
				if (mail) {
#ifdef	UNICODE
					babel::init_babel();
					cube::utility::sendmail::SendMail( MAIL_SUBJECT, MAIL_BODY, babel::unicode_to_sjis( tmp ).c_str(), babel::unicode_to_sjis( dest.substr( dest.find_last_of( _T( '\\' ) ) + 1 ) ).c_str() );
#else	// UNICODE
					cube::utility::sendmail::SendMail( MAIL_SUBJECT, MAIL_BODY, tmp.c_str(), dest.substr( dest.find_last_of( _T( '\\' ) ) + 1 ).c_str() );
#endif	// UNICODE
					if ((setting_.compression().details() & DETAIL_MAIL_REMOVE)) {
						if (PathFileExists(tmp.c_str())) DeleteFile(tmp.c_str());
						return;
					}
				}
				
				if(!update) {
					LOG_INFO(_T("move, src = %s, dest = %s"), tmp.c_str(), dest.c_str());
					if (PathFileExists(dest.c_str())) DeleteFile(dest.c_str());
					MoveFileEx(tmp.c_str(), dest.c_str(), (MOVEFILE_COPY_ALLOWED));
				}
				
				// フィルタリング
				if ((setting_.compression().details() & DETAIL_FILTER) && !setting_.filters().empty()) {
					this->compress_filter(dest, setting_.filters());
				}
				
				// フォルダを開く
				if (setting_.compression().details() & DETAIL_OPEN) {
					string_type root = dest.substr(0, dest.find_last_of(_T('\\')));
					if ((setting_.compression().details() & DETAIL_SKIP_DESKTOP) == 0 || !this->is_desktop(root)) {
						string_type explorer = setting_.compression().explorer();
						if (explorer.empty() || !PathFileExists(explorer.c_str())) explorer = _T("explorer.exe");
						LOG_INFO(_T("Explorer = %s"), explorer.c_str());
						LOG_INFO(_T("OpenDir = %s"), root.c_str());
						ShellExecute(NULL, _T("open"), explorer.c_str(), ( _T("\"") + root + _T("\"") ).c_str(), NULL, SW_SHOWNORMAL);
					}
				}
			}
			
			if (!update && PathFileExists(tmp.c_str())) DeleteFile(tmp.c_str());

			LOG_TRACE(_T("function archiver::compress() end"));
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
			static const string_type error = _T("ERROR:");
			static const string_type password_error(_T("Wrong password?"));
			
			LOG_TRACE(_T("function archiver::decompress() start"));
			
			// レジストリの設定を無視するコマンドかどうか．
			string_type force;
			if (first->compare(0, 3, _T("/x:")) == 0) force = first->substr(3);
			if (!force.empty()) {
				LOG_TRACE(_T("ForceCmd = %s"), force.c_str());
			}
			
			// オプションを読み飛ばす．
			for (; first != last && first->at(0) == _T('/'); ++first) {
				if (first->compare(0, 6, _T("/drop:")) == 0) {
					this->drop_path_ = first->substr(6);
					LOG_TRACE(_T("DropPath = %s"), this->drop_path_.c_str());
				}
			}
			
			// NOTE: ドラッグ&ドロップの際にファイルが指定されなかった
			// (ショートカットをダブルクリックした）と想定する．
			// TODO: それ以外の場合も想定したメッセージを表示する必要があるかどうか．
			if (first == last) {
				string_type err(_T("解凍したい圧縮ファイルを「CubeICE 解凍」ショートカットにドラッグ&ドロップして下さい。"));
				LOG_ERROR(_T("Message = %s"), err.c_str());
				MessageBox(NULL, err.c_str(), _T("CubeICE 解凍"), MB_OK | MB_ICONINFORMATION);
				return;
			}
			
			for (; first != last; ++first) {
				string_type report; // エラーレポート
				LOG_INFO(_T("SrcFile = %s"), first->c_str());
				
				string_type src = *first;
				string_type original = src;
				string_type title_message = this->filename(original) + _T(" を解凍しています - CubeICE");
				string_type filetype;
				if (!this->decompress_filetype(src, filetype)) {
					string_type message = src + _T(" は未対応のファイル形式のため解凍できません。");
					LOG_ERROR(_T("Message = %s"), message.c_str());
					MessageBox(NULL, message.c_str(), _T("CubeICE 解凍エラー"), MB_OK | MB_ICONERROR);
					return;
				}
				
				// 保存先パスの取得
				string_type root = this->decompress_path(setting_.decompression(), src, force);
				clx::rstrip_if(root, clx::is_any_of(_T("\\")));
				if (root.empty()) break;
				LOG_INFO(_T("RootDir = %s"), root.c_str());
				
				progress_.show();
				progress_.title(_T("0% - ") + title_message);
				progress_.marquee(true);
				
				// 一時フォルダの作成
				string_type tmp = cubeice::tmpdir(root, _T("cubeice"));
				decomp_tmp_dir_ = tmp;
				if (tmp.empty()) break;
				
				// プログレスバーの進行度の設定
				string_type encoding;
				string_type folder = this->decompress_filelist(src, encoding);
				LOG_INFO(_T("TotalSize = %I64d"), this->size_);
				if (progress_.is_cancel()) break;
				if (this->size_ == 0) progress_.marquee(true);
				else if (progress_.is_marquee()) progress_.marquee(false);
				
				// *.tar 系の処理
				// TODO: 現在，拡張子が本来の種類と異なるファイルも対象にしているが，
				// それらの偽装（？）拡張子のファイルが *.tar かどうかをどう判断するか．
				if ((filetype == _T("gzip") || filetype == _T("bzip2")) && this->is_tar(src)) {
					src = this->decompress_tar(src, tmp, report);
					progress_.position(progress_.minimum());
					progress_.subposition(progress_.minimum());
					progress_.title(_T("0% - ") + title_message);
					folder = this->decompress_filelist(src, encoding);
					LOG_INFO(_T("TotalSize = %I64d (After decompressing *.tar.*)"), this->size_);
					if (src.empty() || !PathFileExists(src.c_str())) {
						LOG_ERROR(_T("Filename = %s"), src.c_str());
						break;
					}
					if (this->size_ == 0) progress_.marquee(true);
					else if (progress_.is_marquee()) progress_.marquee(false);
				}
				
				// フォルダの作成
				// 単一ファイルかどうかは files_ の数（ファイル数），単一フォルダかどうかは
				// 変数 folder に文字列が設定されているかどうかで判断する．
				if (decompress_is_create_folder(setting_, this->files_.size() == 1, !folder.empty())) {
					root = this->decompress_dirname(setting_.decompression(), root, src);
					clx::rstrip_if(root, clx::is_any_of(_T("\\")));
					LOG_INFO(_T("RootDir = %s"), root.c_str());
				}
				
				// コマンドラインの生成
				string_type cmdline = decompress_cmdline(src, tmp, encoding, false);
				LOG_TRACE(_T("CmdLine-7z = %s"), cmdline.c_str());
				
				cube::popen proc;
				if (!proc.open(cmdline.c_str(), _T("r"))) {
					LOG_ERROR(_T("cube::popen::open()"));
					return;
				}
				
				// メイン処理
				unsigned int index = 0;
				int status = 0;
				int to_all = 0; // はい/いいえ/リネーム
				string_type line;
				double calcpos = 0.0; // 計算上のプログレスバーの位置
				int percent = 0;
				
				// NOTE: marquee スタイルから復帰する際，バーが 1% まで進まないと再描画されない．
				// そのため，一瞬だけ 1% に進めて強制的に再描画を行っている．
				if (this->size_ > 0) {
					progress_.position(100);
					progress_.subposition(100);
					progress_.refresh();
					progress_.position(progress_.minimum());
					progress_.subposition(progress_.minimum());
					progress_.refresh();
				}
				
				progress_.start();
				while ((status = proc.gets(line)) >= 0) {
					if (!progress_.is_marquee() && progress_.subposition() > progress_.maximum() - 1.0) {
						progress_.subposition(progress_.minimum());
					}
					if (!this->refresh(proc)) break;
					
					string_type message = (root.size() > CUBEICE_MAXCOLUMN) ? _T("...") + root.substr(root.size() - CUBEICE_MAXCOLUMN) : root;
					message += _T("\r\n");
					message += (files_[index].name.size() > CUBEICE_MAXCOLUMN) ?
						_T("...") + files_[index].name.substr(files_[index].name.size() - CUBEICE_MAXCOLUMN) :
						files_[index].name;
					progress_.text(message);
					
					if (status == 2) break; // pipe closed
					else if (status == 0 || line.empty()) {
						if (!this->refresh(proc)) break;
						
						// プログレスバーの更新
						if (this->size_ > 0) {
							size_type fsize = filesize(tmp + _T("\\") + files_[index].name);
							if (!this->refresh(proc)) break;
							
							double tmppos = (this->size_ > 0 && fsize > 0) ?
								calcpos + (progress_.maximum() - progress_.minimum()) / (this->size_ / (double)fsize) :
								calcpos;
							if (tmppos > progress_.maximum()) tmppos = progress_.maximum();
							progress_.position(tmppos);
							if (!this->refresh(proc)) break;
							
							double subpos = (files_[index].size > 0 && fsize > 0) ?
								(progress_.maximum() - progress_.minimum()) / (files_[index].size / (double)fsize) :
								0.0;
							if (subpos > progress_.maximum()) subpos = progress_.maximum();
							progress_.subposition(subpos);
							if (!this->refresh(proc)) break;
							
							int prev = percent;
							percent = (tmppos > 1.0) ? static_cast<int>(tmppos / 100.0) : 0;
							if (percent > prev) {
								string_type title = boost::lexical_cast<string_type>(percent) + _T("% - ") + title_message;
								progress_.title(title);
							}
							if (!this->refresh(proc)) break;
						}
						Sleep(10);
						continue;
					}
					assert(status == 1);
					LOG_TRACE(_T("Message = %s"), line.c_str());
					
					string_type::size_type pos = line.find(error);
					if (pos != string_type::npos && line.find(password_error) == string_type::npos) {
						string_type err = clx::strip_copy(line.substr(pos));
						if (pos != 0) err += _T(" (") + clx::strip_copy(line.substr(0, pos)) + _T(")");
						LOG_ERROR(err.c_str());
						report += err + _T("\r\n");
						continue;
					}
					
					// パスワード処理
					if (this->decompress_is_password(proc, line, tmp)) {
						LOG_TRACE(_T("Password operations start"));
						proc.close();
						
						string_type remove_file = root + _T("\\") + files_[std::max(static_cast<int>(index - 1), 0)].name;
						if (PathFileExists(remove_file.c_str()) != FALSE && this->filesize(remove_file) == 0) {
							DeleteFile(remove_file.c_str());
						}
						if (cubeice::dialog::password(progress_.handle(), DECOMPRESS_FLAG) == IDCANCEL) {
							// キャンセルを押されたときの暫定処理
							string_type		deldir = root;
							if ((setting_.decompression().details() & DETAIL_CREATE_FOLDER) &&
								(setting_.decompression().details() & DETAIL_SINGLE_FOLDER) &&
								!folder.empty()) {
									deldir += _T("\\") + folder;
							}
							if (PathFileExists(deldir.c_str()))
								RemoveDirectory(deldir.c_str());
							return;
						}
						LOG_INFO(_T("Password = %s"), cubeice::password().c_str());
						
						cmdline = decompress_cmdline(src, tmp, encoding, true);
						LOG_TRACE(_T("CmdLine-7z = %s"), cmdline.c_str());
						
						if (!proc.open(cmdline.c_str(), _T("r"))) {
							LOG_ERROR(_T("cube::popen::open()"));
							break;
						}
						
						index = 0;
						status = 0;
						to_all = 0;
						calcpos = 0.0;
						line.erase();
						if (this->size_ > 0) {
							progress_.position(progress_.minimum());
							progress_.subposition(progress_.minimum());
							progress_.title(_T("0% - ") + title_message);
						}
						continue;
					}
					
					pos = line.find(keyword);
					if (pos == string_type::npos || line.size() <= keyword.size()) {
						continue;
					}
					string_type filename = clx::strip_copy(line.substr(pos + keyword.size()));
					pos = filename.find(_T('<'));
					if(pos != string_type::npos) filename = filename.substr(pos + 1);
					pos = filename.rfind(_T('>'));
					if(pos != string_type::npos) filename = filename.substr(0, pos);
					LOG_DEBUG(_T("Filename = %s"), filename.c_str());
					
					// プログレスバーの更新
					if (this->size_ > 0) {
						if (this->size_ > 0 && this->files_[index].size > 0) {
							calcpos += (progress_.maximum() - progress_.minimum()) / (this->size_ / (double)this->files_[index].size);
						}
						if (calcpos > progress_.maximum()) calcpos = progress_.maximum();
						progress_.position(calcpos);
						progress_.subposition(progress_.maximum());
						
						// タイトルの更新
						int prev = percent;
						percent = (calcpos > 1.0) ? static_cast<int>(calcpos / 100.0) : 0;
						if (percent > prev) {
							string_type title = boost::lexical_cast<string_type>(percent) + _T("% - ") + title_message;
							progress_.title(title);
						}
					}
					
					// 上書きの確認
					int result = this->is_overwrite(root + _T('\\') + filename, tmp, filename, setting_.decompression(), to_all);
					if ((result & ID_TO_ALL)) {
						result &= ~ID_TO_ALL;
						to_all = result;
					}
					if (result == IDCANCEL) break;
					else if (result == IDNO) continue;
					
					// フィルタリング
					if ((setting_.decompression().details() & DETAIL_FILTER) && this->is_filter(filename, setting_.filters())) {
						LOG_INFO(_T("Filtering = %s"), filename.c_str());
					}
					else {
						SetLastError(NO_ERROR);
						bool status = this->move(tmp + _T('\\') + filename, root + _T('\\') + filename, result == IDRENAME);
						if (!status) {
							// TODO:
							// - move を実行する前に MAX_PATH のチェック
							// - false だった場合に GetLastError() に対応するメッセージを出力（改行を抜く）
							DWORD error_code = GetLastError();
							LOG_ERROR(_T("Filename = %s, ErrCode = %d (%s)"), filename.c_str(), error_code, error_message(error_code).c_str());
							if (error_code != NO_ERROR) report += error + _T(" ") + error_message(error_code);
							else report += error + _T(" 作業領域からのファイル移動に失敗しました。");
							report += _T(" (") + keyword + _T(' ') + filename + _T(")\r\n");
						}
					}
					
					if (index < files_.size() - 1) ++index;
					
					// 進行状況の更新
					progress_.numcount();
				}
				
				if (status < 0) {
					LOG_ERROR(_T("Status = %d"), status);
					report += error + _T(" Broken pipe.");
				}
				
				if ((setting_.decompression().details() & DETAIL_REPORT) && !report.empty()) {
					cubeice::dialog::report(progress_.handle(), report);
				}
				
				// フォルダを開く
				if (setting_.decompression().details() & DETAIL_OPEN) {
					if ((setting_.decompression().details() & DETAIL_CREATE_FOLDER) &&
						(setting_.decompression().details() & DETAIL_SINGLE_FOLDER) &&
						!folder.empty()) {
						root += _T("\\") + folder;
					}
					
					bool skip_flag = (setting_.decompression().details() & DETAIL_SKIP_DESKTOP) != 0;
					if ((!skip_flag || !this->is_desktop(root)) && PathFileExists(root.c_str())) {
						string_type explorer = setting_.decompression().explorer();
						if (explorer.empty() || !PathFileExists(explorer.c_str())) explorer = _T("explorer.exe");
						LOG_INFO(_T("Explorer = %s"), explorer.c_str());
						LOG_INFO(_T("OpenDir = %s"), root.c_str());
						ShellExecute(NULL, _T("open"), explorer.c_str(), ( _T("\"") + root + _T("\"") ).c_str(), NULL, SW_SHOWNORMAL);
					}
				}
				
				cubeice::removedir(tmp.c_str());
				decomp_tmp_dir_.clear();
				
				if ((setting_.decompression().details() & DETAIL_REMOVE_SRC) && report.empty() && !progress_.is_cancel()) {
					DeleteFile(original.c_str());
				}

				LOG_TRACE(_T("function archiver::decompress() end"));
			}
		}
		
	private:
		struct fileinfo {
		public:
			string_type    name;
			size_type      size;
			clx::date_time time;
			bool           directory;
			
			fileinfo() : name(), size(0), time(), directory(false) {}
		};
		
		const setting_type& setting_;
		std::vector<string_type> input_files_;
		std::vector<fileinfo> files_;
		bool done_get_filelist_;
		size_type size_; // トータルサイズ
		cubeice::dialog::progressbar progress_;
		string_type decomp_tmp_dir_;
		string_type drop_path_;
		
	private: // compress_xxx
		/* ----------------------------------------------------------------- */
		//  compress_path
		/* ----------------------------------------------------------------- */
		string_type compress_path(const setting_type::archive_type& setting, const string_type& src, const string_type& ext) {
			const TCHAR filter[] = _T("All files(*.*)\0*.*\0\0");
			string_type path = src;
			if (!PathIsDirectory(src.c_str())) {
				path = src.substr(0, src.find_last_of(_T('.')));
				string_type::size_type pos = path.find_last_of(_T('.'));
				if (pos != string_type::npos && path.substr(pos) == _T(".tar")) path = path.substr(0, pos);
			}
			path += ext;
			
			string_type dest =  this->rootdir(setting, src);
			if (dest.empty()) dest = cubeice::dialog::savefile(NULL, filter, path.c_str());
			else {
				string_type::size_type first = path.find_last_of(_T('\\'));
				if (first == string_type::npos) first = 0;
				else ++first;
				string_type filename = path.substr(first);
				dest += _T('\\') + filename;
				if (PathFileExists(dest.c_str())) dest = cubeice::dialog::savefile(NULL, filter, path.c_str());
			}
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_prepare_filelist
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void compress_prepare_filelist(InputIterator first, InputIterator last) {
			for (; first != last; ++first) {
				input_files_.push_back(*first);
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_filelist
		/* ----------------------------------------------------------------- */
		void compress_filelist() {
			this->size_ = 0;
			this->files_.clear();
			
			for (std::vector<string_type>::const_iterator first = input_files_.begin(); first != input_files_.end(); ++first) {
				progress_.refresh();
				if (progress_.is_cancel()) return;
				while (progress_.is_suspend() && !progress_.is_cancel()) {
					progress_.refresh();
					Sleep(10);
				}
				
				bool directory = (PathIsDirectory(first->c_str()) != FALSE);
				
				// NOTE: 現状，総ファイル数しか必要としていないので高速化のためにファイル情報の取得を省く．
#ifdef CUBEICE_ORIGINAL
				fileinfo elem = this->createinfo(*first);
#else
				fileinfo elem;
				elem.name = *first;
				elem.size = 0;
				elem.directory = directory;
#endif
				this->files_.push_back(elem);
				this->size_ += elem.size;
				progress_.denomcount();
				if (directory) compress_filelist_folder(*first);
			}
			progress_.denomcount(0);
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_filelist_folder
		/* ----------------------------------------------------------------- */
		void compress_filelist_folder(const string_type& root) {
			string_type path = root + _T("\\*.*");
			WIN32_FIND_DATA wfd = {};
			HANDLE handle = FindFirstFile(path.c_str(), &wfd);
			if (handle == INVALID_HANDLE_VALUE) return;
			
			int filenum = 0;
			do {
				progress_.refresh();
				if (progress_.is_cancel()) return;
				while (progress_.is_suspend() && !progress_.is_cancel()) {
					progress_.refresh();
					Sleep(10);
				}
				
				string_type filename(wfd.cFileName);
				if (filename != _T(".") && filename != _T("..")) {
					string_type s = root + _T('\\') + filename;
					bool directory = ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
					
					// NOTE: 現状，総ファイル数しか必要としていないので高速化のためにファイル情報の取得を省く．
#ifdef CUBEICE_ORIGINAL
					fileinfo elem = this->createinfo(s);
#else
					fileinfo elem;
					elem.name = s;
					elem.size = 0;
					elem.directory = directory;
#endif
					this->files_.push_back(elem);
					++filenum;
					progress_.denomcount();
					this->size_ += elem.size;
					if (directory) this->compress_filelist_folder(s);
				}
			} while (FindNextFile(handle, &wfd));
			FindClose(handle);
			//progress_.denomcount(filenum);
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_tar
		/* ----------------------------------------------------------------- */
		void compress_tar(const string_type& src, const string_type& dest, const string_type& filetype, bool pass) {
			static const string_type keyword = _T("Compressing");
			
			LOG_TRACE(_T("function archiver::compress_tar() start"));
			LOG_TRACE(_T("src = %s"), src.c_str());
			LOG_TRACE(_T("dest = %s"), dest.c_str());
			LOG_TRACE(_T("filetype = %s"), filetype.c_str());
			LOG_TRACE(_T("pass = %d"), (pass ? 1 : 0));
			
			string_type tar;
			string_type::size_type pos_ext = dest.find_last_of(_T('.'));
			string_type ext = (pos_ext != string_type::npos) ? dest.substr(pos_ext) : _T("");
			tar = dest.substr(0, dest.find_last_of(_T('.')));
			if (ext != _T(".gz") && ext != _T(".bz2")) tar += _T(".tar");
			if (!MoveFileEx(src.c_str(), tar.c_str(), MOVEFILE_COPY_ALLOWED)) return;
			
			std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
			cmdline += _T(" a -t") + filetype;
			if (pass) cmdline += _T(" -p") + cubeice::password();
			cmdline += _T(" -scsWIN -y \"") + dest + _T("\"");
			cmdline += _T(" \"") + tar + _T("\"");
			LOG_DEBUG(_T("CmdLine-7z = %s"), cmdline.c_str());
			
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) {
				LOG_ERROR(_T("cube::popen::open()"));
				return;
			}
			
			progress_.title(_T("0% - ") + this->filename(dest) + _T(" を圧縮しています - CubeICE"));
			progress_.position(progress_.minimum());
			
			int status = 0;
			string_type line;
			int percent = 0;
			while ((status = proc.gets(line)) >= 0) {
				if (!this->refresh(proc)) break;
				if (status == 2) break; // pipe closed
				else if (status == 0 || line.empty()) {
					Sleep(10);
					continue;
				}
				assert(status == 1);
				
				string_type::size_type pos = line.find(_T(":%")); // :%n% と言う形で進捗率が表示される
				if (pos != string_type::npos) {
					string_type s = line.substr(pos);
					clx::strip_if(s, clx::is_any_of(_T(":% ")));
					line.erase(pos);
					int prev = percent;
					percent = _ttoi(s.c_str()) * 100;
					progress_.position(percent);
					if (percent > prev) {
						string_type title = s + _T("% - ") + this->filename(dest) + _T(" を圧縮しています - CubeICE");
						progress_.title(title);
					}
				}
				
				pos = line.find(keyword);
				if (pos == string_type::npos || line.size() <= keyword.size()) continue;
				string_type filename = clx::strip_copy(line.substr(pos + keyword.size()));
				LOG_TRACE(_T("filename = %s"), filename.c_str());
				
				if (filename.size() > CUBEICE_MAXCOLUMN) {
					string_type::size_type startpos = filename.size() - CUBEICE_MAXCOLUMN;
					filename = _T("...") + filename.substr(startpos);
				}
				progress_.text(filename);
			}
			
			DeleteFile(tar.c_str());
			LOG_TRACE(_T("function archiver::compress_tar() end"));
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_filter
		/* ----------------------------------------------------------------- */
		void compress_filter(const string_type& path, const std::set<string_type>& filters) {
			LOG_TRACE(_T("function archiver::compress_filter() start"));
			LOG_TRACE(_T("path = %s"), path.c_str());
			LOG_TRACE(_T("filters (size) = %d"), filters.size());
			
			std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
			cmdline += _T(" d -r -bd -scsWIN -y \"") + path + _T("\"");
			for (std::set<string_type>::const_iterator pos = filters.begin(); pos != filters.end(); ++pos) {
				cmdline += _T(" \"") + *pos + _T("\"");
			}
			LOG_DEBUG(_T("CmdLine-7z = %s"), cmdline.c_str());
			
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) {
				LOG_ERROR(_T("cube::popen::open()"));
				return;
			}
			
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
			
			LOG_TRACE(_T("function archiver::compress_filter() end"));
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_getinfo
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		fileinfo compress_getinfo(InputIterator first, InputIterator last, const string_type& filename) {
			while (first != last) {
				string_type path = first->substr(0, first->find_last_of(_T('\\')));
				path += _T('\\') + filename;
				//std::map<string_type, fileinfo>::const_iterator pos = this->filelist_.find(path);
				//if (pos != this->filelist_.end()) return pos->second;
				if (PathFileExists(path.c_str())) return this->createinfo(path);
				++first;
			}
			return fileinfo();
		}
		
	private: // decompress_xxx
		/* ----------------------------------------------------------------- */
		//  decompress_cmdline
		/* ----------------------------------------------------------------- */
		string_type decompress_cmdline(const string_type& src, const string_type& tmp, const string_type& encoding, bool is_password) {
			string_type cmdline = CUBEICE_ENGINE;
			cmdline += _T(" x -bd -y");
			if (!encoding.empty()) cmdline += _T(" -scs") + encoding;
			if (is_password) cmdline += _T(" -p\"") + cubeice::password() + _T("\"");
			cmdline += _T(" -o\"") + tmp + _T("\"");
			cmdline += _T(" \"") + src + _T("\"");
			return cmdline;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_path
		/* ----------------------------------------------------------------- */
		string_type decompress_path(const setting_type::archive_type& setting, const string_type& src, string_type force) {
			// 保存先パスの決定
			string_type root = this->rootdir(setting, src, force);
			if (root.empty()) {
				string_type init = src.substr(0, src.find_last_of(_T('\\')));
				root = cubeice::dialog::browsefolder(NULL, init.c_str(), _T("解凍するフォルダを指定して下さい。"));
			}
			return root;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_dirname
		/* ----------------------------------------------------------------- */
		string_type decompress_dirname(const setting_type::archive_type& setting, const string_type& root, const string_type& src) {
			string_type dest = root;
			
			// フォルダの作成
			string_type::size_type pos = src.find_last_of(_T('\\'));
			string_type filename = (pos == string_type::npos) ? src : src.substr(++pos);
			dest += _T('\\') + filename.substr(0, filename.find_last_of(_T('.')));
			pos = dest.find_last_of(_T('.'));
			if (pos != string_type::npos && dest.substr(pos) == _T(".tar")) dest.erase(pos);
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_filetype
		/* ----------------------------------------------------------------- */
		bool decompress_filetype(const string_type& path, string_type& filetype) {
			LOG_TRACE(_T("function archiver::decompress_filetype() start"));
			LOG_TRACE(_T("path = %s"), path.c_str());
			LOG_TRACE(_T("filetype = %s"), filetype.c_str());
			
			if (PathIsDirectory(path.c_str())) return false;
			
			string_type cmdline = CUBEICE_ENGINE;
			cmdline += _T(" l ");
			cmdline += _T("\"") + path + _T("\"");
			LOG_DEBUG(_T("CmdLine-7z = %s"), cmdline.c_str());
			
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) {
				LOG_ERROR(_T("cube::popen::open()"));
				return false;
			}
			
			string_type buffer;
			int status = 0;
			while ((status = proc.gets(buffer)) >= 0) {
				if (status == 2) break; // pipe closed
				else if (status == 0 || buffer.empty()) continue;
				
				clx::escape_separator<TCHAR> sep(_T(" \t"), _T("\""), _T(""));
				clx::basic_tokenizer<clx::escape_separator<TCHAR>, std::basic_string<TCHAR> > v(buffer, sep);
				
				buffer.clear();
				if (v.size() >= 3 && v.at(0) == _T("Type") && v.at(1) == _T("=")) {
					// 対象外のファイルタイプを記述する．
					if (v.at(2) == _T("Compound")) continue;
					
					filetype = v.at(2);
					proc.close();
					return true;
				}
			}
			
			// NOTE: パスワードが設定されている場合ファイルタイプが取得できない事がある．
			// 拡張子が *.zip, *.7z, *.rar の場合はスルーする．
			string_type::size_type pos = path.find_last_of(_T('.'));
			if (pos != string_type::npos) {
				if (path.substr(pos) == _T(".zip") ||
					path.substr(pos) == _T(".7z") ||
					path.substr(pos) == _T(".rar")) {
					LOG_DEBUG(_T("get filetype from extension (for password)"));
					filetype = path.substr(pos);
					LOG_DEBUG(_T("filetype = %s"), filetype.c_str());
					return true;
				}
			}
			
			LOG_DEBUG(_T("filetype = %s"), filetype.c_str());
			LOG_TRACE(_T("function archiver::decompress_filetype() end"));
			
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  decompress_filelist
		 *
		 *  返り値は root となるフォルダが単一フォルダの場合，その
		 *  フォルダ名．
		 */
		/* ----------------------------------------------------------------- */
		string_type decompress_filelist(const string_type& path, string_type& encoding) {
			static string_type keyword_encoding = _T("Encoding:");
			
			LOG_TRACE(_T("function archiver::decompress_filelist() start"));
			LOG_TRACE(_T("path = %s"), path.c_str());
			LOG_TRACE(_T("encoding = %s"), encoding.c_str());
			
			string_type cmdline = CUBEICE_ENGINE;
			cmdline += _T(" l ");
			if (!encoding.empty()) cmdline += _T("-scs") + encoding + _T(" ");
			cmdline += _T("\"") + path + _T("\"");
			LOG_DEBUG(_T("CmdLine-7z = %s"), cmdline.c_str());
			
			this->size_ = 0;
			this->files_.clear();
			string_type dest;
			
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) {
				LOG_ERROR(_T("cube::popen::open()"));
				return dest;
			}
			
			string_type buffer, src;
			int status = 0;
			bool single = true;
			while ((status = proc.gets(buffer)) >= 0) {
				progress_.refresh();
				if (progress_.is_cancel()) {
					proc.close();
					break;
				}
				if (progress_.is_suspend()) {
					proc.suspend();
					while (progress_.is_suspend() && !progress_.is_cancel()) {
						progress_.refresh();
						Sleep(5);
					}
					proc.resume();
					continue;
				}
				
				if (status == 2) break; // pipe closed
				else if (status == 1 && !buffer.empty()) {
					LOG_TRACE(_T("Message = %s"), buffer.c_str());
					src = buffer;
				}
				
				string_type::size_type pos = buffer.find(keyword_encoding);
				if (pos != string_type::npos && buffer.size() > keyword_encoding.size()) {
					encoding = clx::strip_copy(buffer.substr(pos + keyword_encoding.size()));
				}
				
				clx::escape_separator<TCHAR> sep(_T(" \t"), _T("\""), _T(""));
				clx::basic_tokenizer<clx::escape_separator<TCHAR>, std::basic_string<TCHAR> > v(buffer, sep);
				
				if (v.empty() || v.at(0) != _T("<>")) {
					buffer.clear();
					continue;
				}
				
				if (v.size() == 6) {
					// ファイルリストの更新
					fileinfo elem;
					elem.name = v.at(5);

					elem.size = v.at(3) != _T("-") ? _ttoi64(v.at(3).c_str()) : 0;
					if (v.at(1) != _T("-")) elem.time.from_string(v.at(1), string_type(_T("%Y-%m-d %H:%M:%S")));
					
					elem.directory = (v.at(2).find(_T('D')) != string_type::npos);
					files_.push_back(elem);
					progress_.denomcount();
					this->size_ += elem.size;
					
					LOG_TRACE(_T("fileinfo = %s, (%I64d Byte)"), elem.name.c_str(), elem.size);

					// 単一フォルダかどうかのチェック
					if (single) {
						string_type::size_type pos = v.at(5).find_first_of(_T('\\'));
						string_type folder = (pos != string_type::npos) ? v.at(5).substr(0, pos) : v.at(5);
						if (pos != string_type::npos || elem.directory) {
							if (dest.empty()) dest = folder;
							else if (dest != folder) single = false;
						}
						else single = false;
					}
				}
				buffer.clear();
			}
			if (!single) dest.erase();
			progress_.denomcount(0);
			
			LOG_DEBUG(_T("RootDir = %s"), dest.c_str());
			LOG_DEBUG(_T("Encoding = %s"), encoding.c_str());
			LOG_TRACE(_T("function archiver::decompress_filelist() end"));
			
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_tar
		/* ----------------------------------------------------------------- */
		string_type decompress_tar(const string_type& src, const string_type& root, string_type& report) {
			static const string_type keyword = _T("Extracting");
			static const string_type error = _T("ERROR:");
			
			LOG_TRACE(_T("function archiver::decompress_tar() start"));
			LOG_TRACE(_T("src = %s"), src.c_str());
			LOG_TRACE(_T("root = %s"), root.c_str());
			
			string_type cmdline = CUBEICE_ENGINE;
			cmdline += _T(" x -bd -scsWIN -y");
			cmdline += _T(" -o\"") + root + _T("\"");
			cmdline += _T(" \"") + src + _T("\"");
			LOG_TRACE(_T("CmdLine-7z = %s"), cmdline.c_str());
			
			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) {
				LOG_ERROR(_T("cube::popen::open()"));
				return src;
			}
			
			string_type message = progress_.text();
			if (!this->files_.empty()) {
				progress_.text(this->filename(this->files_[0].name) + _T(" を抽出しています..."));
			}
			
			// NOTE: marquee スタイルから復帰する際，バーが 1% まで進まないと再描画されない．
			// そのため，一瞬だけ 1% に進めて強制的に再描画を行っている．
			string_type title_message = this->filename(src) + _T(" を解凍しています - CubeICE");
			if (this->size_ > 0) {
				progress_.position(100);
				progress_.subposition(100);
				progress_.refresh();
				progress_.position(progress_.minimum());
				progress_.subposition(progress_.minimum());
				progress_.refresh();
			}
			progress_.title(_T("0% - ") + title_message);
			
			int status = 0;
			int percent = 0;
			string_type filename;
			string_type line;
			while ((status = proc.gets(line)) >= 0) {
				if (!this->refresh(proc)) break;
				if (status == 2) break; // pipe closed
				else if (status == 0 || line.empty()) {
					if (!this->refresh(proc)) break;
					
					// プログレスバーの更新
					if (this->size_ > 0 && !this->files_.empty()) {
						size_type fsize = filesize(root + _T("\\") + this->files_[0].name);
						if (!this->refresh(proc)) break;
						
						double tmppos = (this->size_ > 0 && fsize > 0) ? (progress_.maximum() - progress_.minimum()) / (this->size_ / (double)fsize) : 0;
						if (tmppos > progress_.maximum()) tmppos = progress_.maximum();
						progress_.position(tmppos);
						progress_.subposition(tmppos);
						if (!this->refresh(proc)) break;
						
						int prev = percent;
						percent = (tmppos > 1.0) ? static_cast<int>(tmppos / 100.0) : 0;
						if (percent > prev) {
							string_type title = boost::lexical_cast<string_type>(percent) + _T("% - ") + title_message;
							progress_.title(title);
						}
						if (!this->refresh(proc)) break;
					}
					Sleep(10);
					continue;
				}
				assert(status == 1);
				
				string_type::size_type pos = line.find(error);
				if (pos != string_type::npos) {
					report += clx::strip_copy(line.substr(pos));
					if (pos != 0) report += _T(" (") + clx::strip_copy(line.substr(0, pos)) + _T(")");
					report += _T("\r\n");
					continue;
				}
				
				pos = line.find(keyword);
				if (pos == string_type::npos || line.size() <= keyword.size()) continue;
				filename = clx::strip_copy(line.substr(pos + keyword.size()));
				pos = filename.find(_T('<'));
				if(pos != string_type::npos)
					filename = filename.substr(pos + 1);
				pos = filename.rfind(_T('>'));
				if(pos != string_type::npos)
					filename = filename.substr(0, pos);
			}
			
			progress_.text(message);
			
			LOG_DEBUG(_T("filename = %s\\%s"), root.c_str(), filename.c_str());
			LOG_TRACE(_T("function archiver::decompress_tar() end"));
			
			return root + _T('\\') + filename;
		}

		/* ----------------------------------------------------------------- */
		//  decompress_is_password
		/* ----------------------------------------------------------------- */
		bool decompress_is_password(cube::popen& proc, const string_type& message, const string_type& tmpdir) {
			typedef TCHAR char_type;
			typedef std::basic_string<TCHAR> string_type;
			
			static const string_type keyword = _T("Extracting");
			static const string_type password_required(_T("Enter password"));
			static const string_type password_error(_T("Wrong password?"));
			
			if (message.find(password_required) != string_type::npos ||
				message.find(password_error) != string_type::npos) return true;
			

			string_type::size_type pos = message.find(keyword);
			if (pos == string_type::npos || message.size() <= keyword.size()) return false;

			string_type filename = clx::strip_copy(message.substr(pos + keyword.size()));
			pos = filename.find(_T('<'));
			if (pos != string_type::npos) filename = filename.substr(pos + 1);
			pos = filename.rfind(_T('>'));
			if (pos != string_type::npos) filename = filename.substr(0, pos);

			string_type	nextline;
			if (this->createinfo(tmpdir + _T("\\") + filename).size == 0) {
				int st = 0;
				bool f = false;
				while((st = proc.peek(nextline)) == 0) {
					if (!this->refresh(proc)) {
						f = true;
						break;
					}
					Sleep(10);
				}
				if(f) return false;
				if(st < 0 || st == 2) return false;
			}
			if (nextline.empty()) return false;

			if (nextline.find(password_required) != string_type::npos ||
				nextline.find(password_error) != string_type::npos) return true;

			return false;
		}

		/* ----------------------------------------------------------------- */
		//  decompress_is_create_folder
		/* ----------------------------------------------------------------- */
		bool decompress_is_create_folder(const user_setting& setting, bool single_file, bool single_folder) {
			if ((setting.decompression().details() & DETAIL_CREATE_FOLDER) == 0) return false;
			if ((setting.decompression().details() & DETAIL_SINGLE_FILE) != 0 && single_file) return false;
			if ((setting.decompression().details() & DETAIL_SINGLE_FOLDER) != 0 && single_folder) return false;
			
			return true;
		}
		
	private: // others
		/* ----------------------------------------------------------------- */
		//  non-copyable
		/* ----------------------------------------------------------------- */
		archiver(const archiver& cp);
		archiver& operator=(const archiver& cp);
		
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
		 *   - mydocuments: マイドキュメント
		 */
		/* ----------------------------------------------------------------- */
		string_type rootdir(const setting_type::archive_type& setting, const string_type& src, const string_type& force = _T("")) {
			// デスクトップのパスを取得しておく．
			LPITEMIDLIST item;
			SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &item);
			char_type buffer[CUBE_MAX_PATH] = {};
			SHGetPathFromIDList(item, buffer);
			string_type desktop = buffer;
			// マイドキュメントのパスを取得しておく
			SHGetSpecialFolderLocation(NULL, CSIDL_MYDOCUMENTS, &item);
			SHGetPathFromIDList(item, buffer);
			string_type mydocuments = buffer;
			
			if (force == _T("runtime")) return string_type();
			if (force == _T("desktop")) return desktop;
			if (force == _T("mydocuments")) return mydocuments;
			else if (force == _T("source") || setting.output_condition() == OUTPUT_SOURCE) {
				if (!this->drop_path_.empty()) return this->drop_path_;

				string_type folder;
				ZeroMemory(buffer, CUBE_MAX_PATH);
				DWORD status = GetFullPathName(src.c_str(), CUBE_MAX_PATH, buffer, NULL);
				if (status == 0) folder = src;
				else folder = buffer;
				return folder.substr(0, folder.find_last_of(_T('\\')));
			}
			else if (setting.output_condition() == OUTPUT_SPECIFIC) {
				if (!setting.output_path().empty()) return setting.output_path();
				else return desktop;
			}
			return string_type();
		}
		
		/* ----------------------------------------------------------------- */
		//  createinfo
		/* ----------------------------------------------------------------- */
		fileinfo createinfo(const string_type& path) {
			fileinfo dest;
			
			dest.name = path;
			dest.directory = (PathIsDirectory(path.c_str()) != FALSE);
			HANDLE h = CreateFile(path.c_str(), 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (h != INVALID_HANDLE_VALUE) {
				FILETIME t = {};
				if (GetFileTime(h, NULL, NULL, &t)) {
					SYSTEMTIME sys = {};
					FileTimeToSystemTime(&t , &sys);
					dest.time.assign(static_cast<int>(sys.wYear), static_cast<int>(sys.wMonth), static_cast<int>(sys.wDay),
						static_cast<int>(sys.wHour), static_cast<int>(sys.wMinute), static_cast<int>(sys.wSecond));
				}
				dest.size = GetFileSize(h, NULL);
			}
			CloseHandle(h);
			
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		//  SHFileMove
		/* ----------------------------------------------------------------- */
		bool SHFileMove(const string_type& src, const string_type& dest) {
			// NOTE: src/dest に指定する文字列は "\0" が 2連続で終わっていないとならない
			std::vector<TCHAR> src_buffer(src.begin(), src.end());
			src_buffer.push_back(0);
			src_buffer.push_back(0);
			
			std::vector<TCHAR> dest_buffer(dest.begin(), dest.end());
			dest_buffer.push_back(0);
			dest_buffer.push_back(0);
			
			SHFILEOPSTRUCT op = {};
			op.hwnd = this->progress_.handle();
			op.wFunc = FO_MOVE;
			op.pFrom = reinterpret_cast<const TCHAR*>(&src_buffer.at(0));
			op.pTo = reinterpret_cast<const TCHAR*>(&dest_buffer.at(0));
			op.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI;
			if (SHFileOperation(&op)) return false;
			return true;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  move
		 *
		 *  ファイルを移動させる．ただし，ディレクトリの場合は移動させる
		 *  代わりに同名のディレクトリを移動先に作成する．
		 */
		/* ----------------------------------------------------------------- */
		bool move(const string_type& src, const string_type& dest, bool rename) {
			LOG_TRACE(_T("function archiver::move() start"));
			LOG_TRACE(_T("src = %s"), src.c_str());
			LOG_TRACE(_T("dest = %s"), dest.c_str());
			LOG_TRACE(_T("rename = %d"), (rename ? 1 : 0));
			
			bool status = false;
			if (PathIsDirectory(src.c_str())) status = cubeice::createdir(dest);
			else {
				// dest からディレクトリ部分を抽出 Path.GetDirectoryName(dest);
				// TODO: この部分がおかしい場合があるので調査．
				string_type branch(dest.substr(0, dest.find_last_of(_T('\\'))));
				LOG_TRACE(_T("DirectoryName = %s"), branch.c_str());
				
				status = cubeice::createdir(branch);
				// TODO: リネーム処理を追加
				if (rename && PathFileExists(dest.c_str())) {
					TCHAR drivename[8], dirname[4096], basename[1024], extname[32];
					_wsplitpath_s(dest.c_str(), drivename, sizeof(drivename), dirname, sizeof(dirname), basename, sizeof(basename), extname, sizeof(extname));
					TCHAR renamed[8192] = {};
					// renamed を.....(N).拡張子に変更する Nは数字。一応 N< MAX_RENAME_DIGITとしておく
					for (int i = 2; i <= 2147483647; i++) {
						swprintf_s(renamed, sizeof(renamed), _T("%s%s%s(%d)%s"), drivename, dirname, basename, i, extname);
						if (!PathFileExists(renamed)) break;
					}
					status &= SHFileMove(src, renamed);
					LOG_TRACE(_T("SHFileMove(%s, %s), status = %d: %s"), src.c_str(), renamed, GetLastError(), error_message().c_str());
					
				} else {
					status &= SHFileMove(src, dest);
					LOG_TRACE(_T("SHFileMove(%s, %s), status = %d: %s"), src.c_str(), dest.c_str(), GetLastError(), error_message().c_str());
				}
			}
			
			LOG_TRACE(_T("function archiver::move() end"));
			
			return status;
		}
		
		/* ----------------------------------------------------------------- */
		//  is_filter
		/* ----------------------------------------------------------------- */
		bool is_filter(const string_type& src, const std::set<string_type>& filters) {
			for (std::set<string_type>::const_iterator pos = filters.begin(); pos != filters.end(); ++pos) {
				if (cubeice::pathmatch(src, *pos)) return true;
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
			if ((setting.details() & DETAIL_OVERWRITE) && PathFileExists(target.c_str()) && !PathIsDirectory(target.c_str())) {
				if ((setting.details() & DETAIL_IGNORE_NEWER) && !is_older(target, compared)) return IDYES;
				else if (force > 0) return force;
				else {
					string_type message = target + _T(" は既に存在します。上書きしますか？");
					return MessageBox(NULL, message.c_str(), _T("上書きの確認"), MB_YESNO | MB_ICONWARNING);
				}
			}
			return IDOK;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  is_overwrite
		 *
		 *  ファイルの上書きを行うかどうかを確認する．
		 *  ファイルの上書きダイアログにファイル情報を出力するバージョン．
		 */
		/* ----------------------------------------------------------------- */
		int is_overwrite(const string_type& target, const string_type& root, const string_type& filename, const setting_type::archive_type& setting, int force) {
			string_type compared = root + _T("\\") + filename;
			if ((setting.details() & DETAIL_OVERWRITE) && PathFileExists(target.c_str()) && !PathIsDirectory(target.c_str())) {
				if ((setting.details() & DETAIL_IGNORE_NEWER) && !is_older(target, compared)) return IDYES;
				else if (force > 0) return force;
				else {
					string_type message = _T("この場所には同じ名前のファイルが既に存在します。上書きしますか？\r\n");
					
					// 現在のファイル
					fileinfo elem = this->createinfo(target);
					message += _T("\r\n");
					message += _T("現在のファイル\r\n");
					message += target + _T("\r\n");
					message += _T("サイズ: ") + cubeice::punct(elem.size) + _T(" バイト\r\n");
					message += _T("更新日時: ") + elem.time.to_string(_T("%Y-%m-%d %H:%M:%S\r\n"));
					
					// 新しいファイル
					//elem = this->filelist_[filename];
					elem = this->createinfo(compared);
					message += _T("\r\n");
					message += _T("新しいファイル\r\n");
					message += filename + _T("\r\n");
					message += _T("サイズ: ") + cubeice::punct(elem.size) + _T(" バイト\r\n");
					message += _T("更新日時: ") + elem.time.to_string(_T("%Y-%m-%d %H:%M:%S"));
					
					return cubeice::dialog::overwrite(progress_.handle(), message);
				}
			}
			return IDOK;
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
			if (ext == _T(".tgz") || ext == _T(".tbz")) return true;

			if (pos==0) return false;
			pos = path.find_last_of(_T('.'), pos-1);
			if (pos == string_type::npos) return false;
			ext = path.substr(pos);
			if (ext == _T(".tar.gz") || ext == _T(".tar.bz2")) return true;
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
		
		/* ----------------------------------------------------------------- */
		//  filesize
		/* ----------------------------------------------------------------- */
		size_type filesize(const string_type& path) {
			HANDLE handle = CreateFile(path.c_str(), 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD dwSizeHigh = 0;
			DWORD dwSizeLow = GetFileSize(handle, &dwSizeHigh);
			CloseHandle(handle);
			return ( dwSizeLow == -1 ) ? 0 : ( (static_cast<size_type>(dwSizeHigh) << sizeof(DWORD) * CHAR_BIT) | dwSizeLow );
		}

		/* ----------------------------------------------------------------- */
		//  filename
		/* ----------------------------------------------------------------- */
		string_type filename(const string_type& path) {
			string_type::size_type pos = path.find_last_of(_T('\\'));
			return (pos == string_type::npos) ? path : path.substr(pos + 1);
		}

		/* ----------------------------------------------------------------- */
		//  refresh
		/* ----------------------------------------------------------------- */
		bool refresh(cube::popen& proc) {
			progress_.refresh();
			if (progress_.is_cancel()) {
				proc.close();
				return false;
			}
			
			if (progress_.is_suspend()) {
				proc.suspend();
				while (progress_.is_suspend() && !progress_.is_cancel()) {
					progress_.refresh();
					Sleep(10);
				}
				proc.resume();
			}
			return true;
		}
	};
}

#endif // CUBE_ARCHIVER_H
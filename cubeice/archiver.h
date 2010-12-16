#ifndef CUBE_ARCHIVER_H
#define CUBE_ARCHIVER_H

#include <string>
#include <vector>
#include <tchar.h>
#include <clx/split.h>
#include "wpopen.h"
#include "user-setting.h"

#define CUBEICE_ENGINE _T("7z.exe")

namespace cubeice {
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
			const TCHAR filter[] = _T("All files(*.*)\0*.*\0\0");
			std::basic_string<TCHAR> dest = cubeice::dialog::savefile(filter);
			std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
			cmdline += _T(" a -tzip -bd -scsWIN -y \"") + dest + _T("\"");
			while (++first != last) cmdline += _T(" \"") + *first + _T("\"");
			this->execute(cmdline, 0);
		}

		/* ----------------------------------------------------------------- */
		//  decompress
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void decompress(InputIterator first, InputIterator last) {
			while (++first != last) {
				std::basic_string<TCHAR>::size_type pos = first->find_last_of('\\');
				std::basic_string<TCHAR> filename = (pos == std::basic_string<TCHAR>::npos) ? *first : first->substr(pos);
				std::basic_string<TCHAR> dest = getdir(setting_.decompression(), *first);
				if (dest.empty()) dest = cubeice::dialog::browsefolder(_T("解凍するフォルダを指定して下さい。"));
				if (dest.empty()) break;
				if (setting_.decompression().details() & DETAIL_CREATE_FOLDER) {
					dest += _T("\\") + filename.substr(0, filename.find_last_of(_T('.')));
				}
				std::basic_string<TCHAR> cmdline = CUBEICE_ENGINE;
				cmdline += _T(" x -bd -scsWIN -y -o\"") + dest + _T("\"");
				cmdline += _T(" \"") + *first + _T("\"");
				this->execute(cmdline, 0);
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		/*
		 *  execute
		 *
		 *  MEMO: CreateProcess に渡すコマンドライン文字列は変更される
		 *  可能性があるため，いったん配列 (vector) にコピーして渡している．
		 */
		/* ----------------------------------------------------------------- */
		void execute(const std::basic_string<TCHAR>& cmdline, size_type n) {
			PROCESS_INFORMATION pi = {};
			
			cubeice::dialog::progressbar progress;
			if (n == 0) progress.marquee(true);

			cube::popen proc;
			if (!proc.open(cmdline.c_str(), _T("r"))) return;
			
			// 1 ファイル辺りのプログレスバーの進行量
			size_type step = (n > 0) ? (progress.maximum() - progress.minimum()) / n : 1;
			if (step == 0) step = 1;

			int status = 0;
			string_type buffer;
			while ((status = proc.gets(buffer)) >= 0) {
				if (status == 2) break; // pipe closed
				else if (status == 1 && !buffer.empty()) {
					progress.text(buffer);
					if (n > 0) progress += step;
				}
				buffer.clear();

				progress.refresh();
				if (progress.is_cancel()) {
					proc.close();
					break;
				}
				Sleep(10);
			}
		}

	private:
		const setting_type& setting_;
		
		/* ----------------------------------------------------------------- */
		//  non-copyable
		/* ----------------------------------------------------------------- */
		archiver(const archiver& cp);
		archiver& operator=(const archiver& cp);
		
		/* ----------------------------------------------------------------- */
		/*
		 *  getdir
		 *
		 *  ユーザ設定の値を元に出力先ディレクトリを取得する．特定の
		 *  ディレクトリを指定がチェックされており，かつ出力先が
		 *  空白の場合はデスクトップのパスを返す．
		 */
		/* ----------------------------------------------------------------- */
		static string_type getdir(const setting_type::archive_type& setting, const string_type& src) {
			string_type dest;
			if (setting.output_condition() == OUTPUT_SPECIFIC) {
				if (!setting.output_path().empty()) return setting.output_path();
				else { // デスクトップのパスを取得
					LPITEMIDLIST item;
					SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &item);
					
					char_type buffer[CUBE_MAX_PATH] = {};
					SHGetPathFromIDList(item, buffer);
					return string_type(buffer);
				}
			}
			else if (setting.output_condition() == OUTPUT_SOURCE) {
				// TODO: 相対パスの場合どうするか．
				return src.substr(0, src.find_last_of(_T('\\')));
			}
			
			return string_type();
		}
	};
}

#endif // CUBE_ARCHIVER_H
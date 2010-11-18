#ifndef CUBE_ARCHIVER_H
#define CUBE_ARCHIVER_H

#include <string>
#include <vector>
#include <tchar.h>

#define CUBE_ICE_ENGINE _T("7z")

namespace cube {
	class archiver {
	public:
		/* ----------------------------------------------------------------- */
		//  compress
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void compress(InputIterator first, InputIterator last) {
			const TCHAR filter[] = _T("All files(*.*)\0*.*\0\0");
			std::basic_string<TCHAR> dest = cube::win32api::save_dialog(filter);
			std::basic_string<TCHAR> cmdline = CUBE_ICE_ENGINE;
			cmdline += _T(" a -tzip -y \"") + dest + _T("\"");
			while (++first != last) cmdline += _T(" \"") + *first + _T("\"");
			this->execute(cmdline);
		}

		/* ----------------------------------------------------------------- */
		//  decompress
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void decompress(InputIterator first, InputIterator last) {
			while (++first != last) {
				std::basic_string<TCHAR>::size_type pos = first->find_last_of('\\');
				std::basic_string<TCHAR> filename = (pos == std::basic_string<TCHAR>::npos) ? *first : first->substr(pos);
				std::basic_string<TCHAR> dest = cube::win32api::folder_dialog(_T("解凍するフォルダーを指定して下さい。"));
				dest += _T("\\") + filename.substr(0, filename.find_last_of(_T('.')));
				std::basic_string<TCHAR> cmdline = CUBE_ICE_ENGINE;
				cmdline += _T(" x -y -o\"") + dest + _T("\"");
				cmdline += _T(" \"") + *first + _T("\"");
				this->execute(cmdline);
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		/*
		 *  execute
		 *
		 *  MEMO: CreateProcess に渡すコマンドライン文字列は変更される可能性が
		 *  あるため，いったん配列 (vector) にコピーして渡している．
		 */
		/* ----------------------------------------------------------------- */
		void execute(const std::basic_string<TCHAR>& cmdline) {
			STARTUPINFO si = {};
			PROCESS_INFORMATION pi = {};
			GetStartupInfo(&si);
			
			// \0 で終わる const でない char 配列
			std::vector<TCHAR> tmp(cmdline.begin(), cmdline.end());
			tmp.push_back(0);
			
			MessageBox(NULL, cmdline.c_str(), _T("cmdline"), MB_OK);
	
			CreateProcess(
				CUBE_ICE_ENGINE,    // 実行可能モジュール
				&tmp.at(0),    // コマンドライン
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				NULL,               // カレントディレクトリ
				&si, &pi
			);
		}
	};
}

#endif // CUBE_ARCHIVER_H
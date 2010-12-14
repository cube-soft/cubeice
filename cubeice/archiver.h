#ifndef CUBE_ARCHIVER_H
#define CUBE_ARCHIVER_H

#include <string>
#include <vector>
#include <tchar.h>
#include <clx/split.h>
#include "wpopen.h"

#define CUBE_ICE_ENGINE _T("7z.exe")

namespace cube {
	class archiver {
	public:
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef std::size_t size_type;

		archiver() {}

		/* ----------------------------------------------------------------- */
		//  compress
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		void compress(InputIterator first, InputIterator last) {
			const TCHAR filter[] = _T("All files(*.*)\0*.*\0\0");
			std::basic_string<TCHAR> dest = cube::dialog::savefile(filter);
			std::basic_string<TCHAR> cmdline = CUBE_ICE_ENGINE;
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
				std::basic_string<TCHAR> dest = cube::dialog::browsefolder(_T("解凍するフォルダを指定して下さい。"));
				dest += _T("\\") + filename.substr(0, filename.find_last_of(_T('.')));
				std::basic_string<TCHAR> cmdline = CUBE_ICE_ENGINE;
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
			
			cube::dialog::progressbar progress;
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
		/* ----------------------------------------------------------------- */
		//  non-copyable
		/* ----------------------------------------------------------------- */
		archiver(const archiver& cp);
		archiver& operator=(const archiver& cp);
	};
}

#endif // CUBE_ARCHIVER_H
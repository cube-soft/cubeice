#ifndef CUBE_ARCHIVER_H
#define CUBE_ARCHIVER_H

#include <string>
#include <vector>
#include <tchar.h>
#include <clx/split.h>

#define CUBE_ICE_ENGINE _T("7z.exe")

namespace cube {
	class archiver {
	public:
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
				std::basic_string<TCHAR> dest = cube::dialog::browsefolder(_T("解凍するフォルダを指定して下さい。"));
				dest += _T("\\") + filename.substr(0, filename.find_last_of(_T('.')));
				std::basic_string<TCHAR> cmdline = CUBE_ICE_ENGINE;
				cmdline += _T(" x -bd -scsWIN -y -o\"") + dest + _T("\"");
				cmdline += _T(" \"") + *first + _T("\"");
				this->execute(cmdline);
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
		void execute(const std::basic_string<TCHAR>& cmdline) {
			PROCESS_INFORMATION pi = {};
			
#if 0
			// MEMO: こういった処理を行いたいが，popen を使用すると DOS 窓が開いてしまう．
			// TODO: popen 相当の関数を CreateProcess と CreatePipe を用いて作成する．
			// see also: http://support.microsoft.com/kb/190351/ja
			cube::dialog::progressbar progress(app_);

			FILE* fd = _tpopen(cmdline.c_str(), _T("r"));
			TCHAR buffer[2048] = {};
			while (_fgetts(buffer, 2048, fd)) {
				progress.text(buffer);
				progress += 10; // TODO: (1 / 総ファイル数) だけ進める．
				Sleep(50);
			}
#else
			// MEMO: 現在は，いったんファイルに吐き出して結果を列挙している．
			// TODO: 作成したプロセスの標準出力を標準入力にリダイレクトし，
			// その内容を一行ずつ表示する．
			SECURITY_ATTRIBUTES sa;
			sa.nLength= sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			HANDLE handle = CreateFile( _T("tmp.txt"), GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
			
			STARTUPINFO si = {};
			//GetStartupInfo(&si);
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdOutput = handle;
			
			// \0 で終わる const でない char 配列
			std::vector<TCHAR> tmp(cmdline.begin(), cmdline.end());
			tmp.push_back(0);
			
			//MessageBox(NULL, cmdline.c_str(), _T("cmdline"), MB_OK);
	
			CreateProcess(
				NULL, // 実行可能モジュール
				&tmp.at(0), // コマンドライン
				NULL,
				NULL,
				TRUE,
				CREATE_NO_WINDOW | DETACHED_PROCESS, // 作成のフラグ
				NULL,
				NULL, // カレントディレクトリ
				&si, &pi
			);
			
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			CloseHandle(handle);
			
			// 1行ずつ表示するサンプル．
			handle = CreateFile( _T("tmp.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
			std::basic_string<TCHAR> src;
			TCHAR buffer[2048] = {};
			DWORD n = 0;
			while (ReadFile(handle, buffer, 2048, &n, NULL) && n > 0) src += buffer;
			std::vector<std::basic_string<TCHAR> > v;
			clx::split_if(src, v, clx::is_any_of(_T("\r\n")));
			
			// progressbar: プログレスバーを表示するためのクラス．
			// ++演算子で1進む．text() に引数を指定すると，指定された内容が表示される．
			cube::dialog::progressbar progress;
			for (std::vector<std::basic_string<TCHAR> >::iterator pos = v.begin(); pos != v.end(); ++pos) {
				progress.text(*pos);
				progress += 5; // TODO: (1 / 総ファイル数) だけ進める．
				Sleep(10);
			}
			CloseHandle(handle);
			DeleteFile( _T("tmp.txt"));
#endif
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
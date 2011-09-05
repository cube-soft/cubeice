#ifndef CUBEICE_ENCODING_H
#define CUBEICE_ENCODING_H

#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <babel/babel.h>
#define	CLX_USE_WCHAR
#include <clx/split.h>
#include <clx/predicate.h>

#pragma comment(lib, "shlwapi.lib")
#pragma warning(disable:4996)

// Resouces
#define IDC_ICON_PICTUREBOX         1000
#define IDC_ENCODING_LISTBOX        1001
#define IDC_PREVIEW_TEXTBOX         1002

namespace cubeice {
	/* --------------------------------------------------------------------- */
	//  EncodingDialog
	/* --------------------------------------------------------------------- */
	class EncodingDialog {
	public:
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef std::size_t size_type;
		
		enum {
			ASCII           = babel::base_encoding::ansi,
			JIS             = babel::base_encoding::jis,
			ShiftJIS        = babel::base_encoding::sjis,
			EUCJP           = babel::base_encoding::euc,
			UTF8            = babel::base_encoding::utf8,
			UTF16           = babel::base_encoding::utf16,
			Unknown         = -1,
			Ignore          = -2
		};
	
	public:
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		EncodingDialog(const string_type& resource_name, const char* src, int encoding = Unknown) :
			handle_(NULL), resource_(resource_name), src_(src), encoding_(encoding), kinds_() {
			babel::init_babel();
			if (encoding_ == Unknown || encoding_ == Ignore) {
				int tmp = this->Guess(src_);
				if (tmp != Unknown) encoding_ = tmp;
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  denstructor
		/* ----------------------------------------------------------------- */
		virtual ~EncodingDialog() {}
		
		/* ----------------------------------------------------------------- */
		//  ShowDialog
		/* ----------------------------------------------------------------- */
		int ShowDialog(HWND owner = NULL) {
			return this->RunDialog(owner) ? IDOK : IDCANCEL;
		}
		
		/* ----------------------------------------------------------------- */
		//  Encoding
		/* ----------------------------------------------------------------- */
		int& Encoding() { return encoding_; }
		const int& Encoding() const { return encoding_; }
		
		/* ----------------------------------------------------------------- */
		//  Handle
		/* ----------------------------------------------------------------- */
		HWND& Handle() { return handle_; }
		const HWND& Handle() const { return handle_; }
		
		/* ----------------------------------------------------------------- */
		//  Convert
		/* ----------------------------------------------------------------- */
		std::basic_string<wchar_t> Convert() {
			return this->Convert(src_, encoding_);
		}
		
		/* ----------------------------------------------------------------- */
		//  Test
		/* ----------------------------------------------------------------- */
		bool Test() {
			std::basic_string<wchar_t> converted = this->Convert();
			std::vector<std::basic_string<wchar_t> > parts;
			clx::split_if(converted, parts, clx::is_any_of(L"\\/"));
			std::basic_string<wchar_t> s;
			clx::join(parts, s, L"");

			wchar_t dir[1024] = {};
			if (::GetTempPath(1024, dir) == 0) return false;
			std::basic_string<wchar_t> path = dir + s;
			HANDLE test = CreateFile(path.c_str(),
				GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE, NULL);

			// TODO: 暫定．もうちょっとまともに判定する．
			//if (test == INVALID_HANDLE_VALUE) return false;
			if (test == INVALID_HANDLE_VALUE &&
				path.find(L"CON") == std::basic_string<wchar_t>::npos &&
				path.find(L"PRN") == std::basic_string<wchar_t>::npos &&
				path.find(L"LPT") == std::basic_string<wchar_t>::npos &&
				path.find(L"COM") == std::basic_string<wchar_t>::npos &&
				path.find(L"NUL") == std::basic_string<wchar_t>::npos &&
				path.find(L"AUX") == std::basic_string<wchar_t>::npos) return false;

			::CloseHandle(test);
			return true;
		}
		
	protected:
		/* ----------------------------------------------------------------- */
		//  RunDialog
		/* ----------------------------------------------------------------- */
		virtual bool RunDialog(HWND owner) {
			INT_PTR result = ::DialogBoxParam(::GetModuleHandle(NULL), resource_.c_str(), owner, StaticWndProc, reinterpret_cast<LPARAM>(this));
			return result == IDOK;
		}

		/* ----------------------------------------------------------------- */
		//  WndProc
		/* ----------------------------------------------------------------- */
		virtual bool WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
			return this->DefaultWndProc(uMsg, wParam, lParam);
		}
		
		/* ----------------------------------------------------------------- */
		//  OnCreate
		/* ----------------------------------------------------------------- */
		virtual bool OnCreate() {
			// 警告メッセージ用のアイコンを表示
			HWND handle = ::GetDlgItem(this->Handle(), IDC_ICON_PICTUREBOX);
			assert(handle != NULL);
			HICON warn = ::LoadIcon(NULL, IDI_WARNING);
			::SendMessage(handle, STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(warn));
			
			// リストボックスの初期化
			handle = ::GetDlgItem(this->Handle(), IDC_ENCODING_LISTBOX);
			assert(handle != NULL);
			kinds_.clear();
			::SendMessage(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Shift-JIS")));
			kinds_.push_back(ShiftJIS);
			::SendMessage(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("EUC-JP")));
			kinds_.push_back(EUCJP);
			::SendMessage(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("JIS")));
			kinds_.push_back(JIS);
			::SendMessage(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Unicode (UTF-8)")));
			kinds_.push_back(UTF8);
			::SendMessage(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Unicode (UTF-16)")));
			kinds_.push_back(UTF16);
			::SendMessage(handle, LB_SETCURSEL, this->EncodingToIndex(encoding_), 0);
			
			// テキストボックスの初期化
			handle = ::GetDlgItem(this->Handle(), IDC_PREVIEW_TEXTBOX);
			assert(handle != NULL);
			std::basic_string<wchar_t> converted = this->Convert();
			::SendMessage(handle, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(converted.c_str()));
			
			return true;
		}
		
		/* ----------------------------------------------------------------- */
		//  OnCommand
		/* ----------------------------------------------------------------- */
		virtual bool OnCommand(WPARAM wParam, LPARAM lParam) {
			switch (LOWORD(wParam)) {
			case IDC_ENCODING_LISTBOX:
				if (HIWORD(wParam) == LBN_SELCHANGE) {
					return this->EncodingListBox_SelectedIndexChanged(wParam, lParam);
				}
				break;
			case IDC_PREVIEW_TEXTBOX:
			default:
				return this->DefaultCommandProc(wParam, lParam);
			}
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		//  DefaultCommandProc
		/* ----------------------------------------------------------------- */
		bool DefaultCommandProc(WPARAM wParam, LPARAM lParam) {
			switch (LOWORD(wParam)) {
			case IDOK:
			{
				HWND handle = GetDlgItem(this->Handle(), IDC_ENCODING_LISTBOX);
				assert(handle != NULL);
				LRESULT index = ::SendMessage(handle, LB_GETCURSEL, 0, 0);
				encoding_ = this->IndexToEncoding(static_cast<int>(index));
				EndDialog(this->Handle(), IDOK);
				break;
			}
			case IDCANCEL:
				EndDialog(this->Handle(), IDCANCEL);
				return true;
			default:
				break;
			}
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		//  DefaultWndProc
		/* ----------------------------------------------------------------- */
		bool DefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg) {
				case WM_INITDIALOG:     return this->OnCreate();
				case WM_COMMAND:        return this->OnCommand(wParam, lParam);
				default: break;
			}
			return false;
		}
		
	private: // event handlers
		/* ----------------------------------------------------------------- */
		//  DefaultWndProc
		/* ----------------------------------------------------------------- */
		bool EncodingListBox_SelectedIndexChanged(WPARAM wParam, LPARAM lParam) {
			// 変更されたエンコーディングの取得
			HWND handle = GetDlgItem(this->Handle(), IDC_ENCODING_LISTBOX);
			assert(handle != NULL);
			LRESULT index = ::SendMessage(handle, LB_GETCURSEL, 0, 0);
			int encoding = this->IndexToEncoding(static_cast<int>(index));
			
			// 変更されたエンコーディングで実際に変換してみる
			handle = GetDlgItem(this->Handle(), IDC_PREVIEW_TEXTBOX);
			assert(handle != NULL);
			std::basic_string<wchar_t> converted = this->Convert(src_, encoding);
			::SendMessage(handle, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(converted.c_str()));
			
			return true;
		}
		
	private:
		/* ----------------------------------------------------------------- */
		//  IndexToEncoding
		/* ----------------------------------------------------------------- */
		int IndexToEncoding(int index) {
			if (index < 0 || static_cast<size_type>(index) >= kinds_.size()) return Unknown;
			return kinds_.at(index);
		}
		
		/* ----------------------------------------------------------------- */
		//  EncodingToIndex
		/* ----------------------------------------------------------------- */
		int EncodingToIndex(int encoding) {
			for (size_type i = 0; i < kinds_.size(); ++i) {
				if (encoding == kinds_.at(i)) return static_cast<int>(i);
			}
			return -1;
		}
		
		/* ----------------------------------------------------------------- */
		//  Guess
		/* ----------------------------------------------------------------- */
		int Guess(const std::basic_string<char>& src) {
			babel::analyze_result result = babel::analyze_base_encoding(src);
			switch (result.get_strict_result()) {
			case ASCII:
			case ShiftJIS:
				return ShiftJIS;
			case EUCJP:
			case JIS:
			case UTF8:
			case UTF16:
				return result.get_strict_result();
			default:
				break;
			}
			return Unknown;
		}
		
		/* ----------------------------------------------------------------- */
		//  Convert
		/* ----------------------------------------------------------------- */
		std::basic_string<wchar_t> Convert(const std::basic_string<char>& src, int encoding) {
			switch (encoding) {
			case ShiftJIS:
				return babel::sjis_to_unicode(src);
			case EUCJP:
				return babel::euc_to_unicode(src);
			case JIS:
				return babel::jis_to_unicode(src);
			case UTF8:
			{
				std::basic_string<char> tmp = src;
				this->ConvertForUTF8Mac(tmp);
				return babel::utf8_to_unicode(tmp);
			}
			case UTF16:
			default:
				break;
			}
			return this->Widen(src);
		}
		
		/* ----------------------------------------------------------------- */
		//  Widen
		/* ----------------------------------------------------------------- */
		std::basic_string<wchar_t> Widen(const std::basic_string<char>& src) {
			wchar_t buffer[2048] = {};
			int result = ::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, buffer, sizeof(buffer) / sizeof(wchar_t));
			if (result == 0) std::basic_string<wchar_t>();
			return std::basic_string<wchar_t>(buffer);
		}
		
		/* ----------------------------------------------------------------- */
		//  ConvertForUTF8Mac
		/* ----------------------------------------------------------------- */
		void ConvertForUTF8Mac(std::basic_string<char>& src) {
			static const char *utf_8_mac_mapping[][2] = {
				{ "\xE3\x81\x8B\xE3\x82\x99", "\xE3\x81\x8C" },		// が
				{ "\xE3\x81\x8D\xE3\x82\x99", "\xE3\x81\x8E" },		// ぎ
				{ "\xE3\x81\x8F\xE3\x82\x99", "\xE3\x81\x90" },		// ぐ
				{ "\xE3\x81\x91\xE3\x82\x99", "\xE3\x81\x92" },		// げ
				{ "\xE3\x81\x93\xE3\x82\x99", "\xE3\x81\x94" },		// ご
				{ "\xE3\x81\x95\xE3\x82\x99", "\xE3\x81\x96" },		// ざ
				{ "\xE3\x81\x97\xE3\x82\x99", "\xE3\x81\x98" },		// じ
				{ "\xE3\x81\x99\xE3\x82\x99", "\xE3\x81\x9A" },		// ず
				{ "\xE3\x81\x9B\xE3\x82\x99", "\xE3\x81\x9C" },		// ぜ
				{ "\xE3\x81\x9D\xE3\x82\x99", "\xE3\x81\x9E" },		// ぞ
				{ "\xE3\x81\x9F\xE3\x82\x99", "\xE3\x81\xA0" },		// だ
				{ "\xE3\x81\xA1\xE3\x82\x99", "\xE3\x81\xA2" },		// ぢ
				{ "\xE3\x81\xA4\xE3\x82\x99", "\xE3\x81\xA5" },		// づ
				{ "\xE3\x81\xA6\xE3\x82\x99", "\xE3\x81\xA7" },		// で
				{ "\xE3\x81\xA8\xE3\x82\x99", "\xE3\x81\xA9" },		// ど
				{ "\xE3\x81\xAF\xE3\x82\x99", "\xE3\x81\xB0" },		// ば
				{ "\xE3\x81\xB2\xE3\x82\x99", "\xE3\x81\xB3" },		// び
				{ "\xE3\x81\xB5\xE3\x82\x99", "\xE3\x81\xB6" },		// ぶ
				{ "\xE3\x81\xB8\xE3\x82\x99", "\xE3\x81\xB9" },		// べ
				{ "\xE3\x81\xBB\xE3\x82\x99", "\xE3\x81\xBC" },		// ぼ
				{ "\xE3\x81\xAF\xE3\x82\x9A", "\xE3\x81\xB1" },		// ぱ
				{ "\xE3\x81\xB2\xE3\x82\x9A", "\xE3\x81\xB4" },		// ぴ
				{ "\xE3\x81\xB5\xE3\x82\x9A", "\xE3\x81\xB7" },		// ぷ
				{ "\xE3\x81\xB8\xE3\x82\x9A", "\xE3\x81\xBA" },		// ぺ
				{ "\xE3\x81\xBB\xE3\x82\x9A", "\xE3\x81\xBD" },		// ぽ
				{ "\xE3\x82\xAB\xE3\x82\x99", "\xE3\x82\xAC" },		// ガ
				{ "\xE3\x82\xAD\xE3\x82\x99", "\xE3\x82\xAE" },		// ギ
				{ "\xE3\x82\xAF\xE3\x82\x99", "\xE3\x82\xB0" },		// グ
				{ "\xE3\x82\xB1\xE3\x82\x99", "\xE3\x82\xB2" },		// ゲ
				{ "\xE3\x82\xB3\xE3\x82\x99", "\xE3\x82\xB4" },		// ゴ
				{ "\xE3\x82\xB5\xE3\x82\x99", "\xE3\x82\xB6" },		// ザ
				{ "\xE3\x82\xB7\xE3\x82\x99", "\xE3\x82\xB8" },		// ジ
				{ "\xE3\x82\xB9\xE3\x82\x99", "\xE3\x82\xBA" },		// ズ
				{ "\xE3\x82\xBB\xE3\x82\x99", "\xE3\x82\xBC" },		// ゼ
				{ "\xE3\x82\xBD\xE3\x82\x99", "\xE3\x82\xBE" },		// ゾ
				{ "\xE3\x82\xBF\xE3\x82\x99", "\xE3\x83\x80" },		// ダ
				{ "\xE3\x83\x81\xE3\x82\x99", "\xE3\x83\x82" },		// ヂ
				{ "\xE3\x83\x84\xE3\x82\x99", "\xE3\x83\x85" },		// ヅ
				{ "\xE3\x83\x86\xE3\x82\x99", "\xE3\x83\x87" },		// デ
				{ "\xE3\x83\x88\xE3\x82\x99", "\xE3\x83\x89" },		// ド
				{ "\xE3\x83\x8F\xE3\x82\x99", "\xE3\x83\x90" },		// バ
				{ "\xE3\x83\x92\xE3\x82\x99", "\xE3\x83\x93" },		// ビ
				{ "\xE3\x83\x95\xE3\x82\x99", "\xE3\x83\x96" },		// ブ
				{ "\xE3\x83\x98\xE3\x82\x99", "\xE3\x83\x99" },		// ベ
				{ "\xE3\x83\x9B\xE3\x82\x99", "\xE3\x83\x9C" },		// ボ
				{ "\xE3\x83\x8F\xE3\x82\x9A", "\xE3\x83\x91" },		// パ
				{ "\xE3\x83\x92\xE3\x82\x9A", "\xE3\x83\x94" },		// ピ
				{ "\xE3\x83\x95\xE3\x82\x9A", "\xE3\x83\x97" },		// プ
				{ "\xE3\x83\x98\xE3\x82\x9A", "\xE3\x83\x9A" },		// ペ
				{ "\xE3\x83\x9B\xE3\x82\x9A", "\xE3\x83\x9D" },		// ポ
			};
			
			for(unsigned int i = 0 ; i < sizeof(utf_8_mac_mapping) / sizeof(utf_8_mac_mapping[0]); ++i) {
				this->ReplaceAll(src, utf_8_mac_mapping[i][0], utf_8_mac_mapping[i][1]);
			}
		}

		/* --------------------------------------------------------------------- */
		//  ReplaceAll
		/* --------------------------------------------------------------------- */
		void ReplaceAll(std::basic_string<char>& s, const std::basic_string<char>& sch, const std::basic_string<char>& rep) {
			if (s.empty()) return;
			
			std::basic_string<char>::size_type pos = 0;
			while ((pos = s.find(sch, pos)) != std::basic_string<char>::npos) {
				s.replace(pos, sch.length(), rep);
				pos += rep.length();
			}
		}
	private: // static member functions
		/* ----------------------------------------------------------------- */
		//  StaticWndProc
		/* ----------------------------------------------------------------- */
		static INT_PTR CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg) {
			case WM_INITDIALOG:
			{
				EncodingDialog* instance = reinterpret_cast<EncodingDialog*>(lParam);
				instance->Handle() = hWnd;
				::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG>(instance));
				break;
			}
			default:
				break;
			}
			
			LONG_PTR ptr = ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (ptr != 0) {
				EncodingDialog* instance = reinterpret_cast<EncodingDialog*>(ptr);
				return instance->WndProc(uMsg, wParam, lParam) ? TRUE : FALSE;
			}
			return FALSE;
		}
		
	protected:
		HWND handle_;
		string_type resource_;
		std::basic_string<char> src_;
		int encoding_;
		std::vector<int> kinds_;
	};
	
	/* --------------------------------------------------------------------- */
	//  GetEncoding
	/* --------------------------------------------------------------------- */
	inline int GetEncoding() {
		TCHAR buffer[32] = {};
		DWORD result = ::GetEnvironmentVariable(_T("Encoding"), buffer, sizeof(buffer) / sizeof(TCHAR));
		if (result == 0) return EncodingDialog::Unknown;
		return _ttoi(buffer);
	}
	
	/* --------------------------------------------------------------------- */
	//  SetEncoding
	/* --------------------------------------------------------------------- */
	inline void SetEncoding(int encoding) {
		TCHAR buffer[32] = {};
		::_itot_s(encoding , buffer, sizeof(buffer) / sizeof(TCHAR), 10);
		::SetEnvironmentVariable(_T("Encoding"), buffer);
	}
	
	/* --------------------------------------------------------------------- */
	//  SetEncoding
	/* --------------------------------------------------------------------- */
	inline void SetEncoding(const std::basic_string<TCHAR>& name) {
		int encoding = EncodingDialog::Unknown;
		if (name == _T("SJIS")) encoding = EncodingDialog::ShiftJIS;
		else if (name == _T("EUC")) encoding = EncodingDialog::EUCJP;
		else if (name == _T("JIS")) encoding = EncodingDialog::JIS;
		else if (name == _T("UTF8") || name == _T("UTF-8")) encoding = EncodingDialog::UTF8;
		else if (name == _T("UTF16") || name == _T("UTF-16")) encoding = EncodingDialog::UTF16;
		else if (name == _T("IGNORE")) encoding = EncodingDialog::Ignore;
		
		if (encoding != EncodingDialog::Unknown) SetEncoding(encoding);
	}
	
	/* --------------------------------------------------------------------- */
	//  EncodingToStringA
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> EncodingToStringA(int encoding) {
		switch (encoding) {
			case EncodingDialog::ShiftJIS: return "SJIS";
			case EncodingDialog::EUCJP:    return "EUC";
			case EncodingDialog::JIS:      return "JIS";
			case EncodingDialog::UTF8:     return "UTF8";
			case EncodingDialog::UTF16:    return "UTF16";
			default: break;
		}
		return std::basic_string<char>();
	}

	/* --------------------------------------------------------------------- */
	//  GetNormalizationFilesPath
	/* --------------------------------------------------------------------- */
	inline std::basic_string<TCHAR> GetNormalizationFilesPath() {
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		
		char_type path[2048] = {};
		DWORD result = ::GetEnvironmentVariable(_T("Normalization"), path, sizeof(path) / sizeof(char_type));
		if (result != 0) return string_type(path);
		
		// ノーマライズされた結果を記憶するためのファイルのパスを設定する．
		char_type dir[1024] = {};
		if (GetTempPath(1024, dir) == 0) return string_type();
		ZeroMemory(path, sizeof(path));
		if (GetTempFileName(dir,_T("cubeice"), 0, path) == 0) return string_type();
		
		// 一時ファイルが生成されているので削除する．
		if (PathFileExists(path)) DeleteFile(path);
		::SetEnvironmentVariable(_T("Normalization"), path);
		return string_type(path);
	}

	/* --------------------------------------------------------------------- */
	//  GetNormalizationFiles
	/* --------------------------------------------------------------------- */
	inline std::vector<std::basic_string<TCHAR> > GetNormalizationFiles() {
		std::vector<std::basic_string<TCHAR> > dest;
		FILE* fin = _wfopen(GetNormalizationFilesPath().c_str(), _T("rb"));
		if (fin == NULL) return dest;

		TCHAR buffer[2048] = {};
		while (fgetws(buffer, sizeof(buffer) / sizeof(TCHAR), fin)) {
			dest.push_back(std::basic_string<TCHAR>(buffer));
		}
		fclose(fin);

		return dest;
	}

	/* --------------------------------------------------------------------- */
	//  PutNormalizationLog
	/* --------------------------------------------------------------------- */
	inline void PutNormalizationLog(const std::basic_string<wchar_t>& src, const std::basic_string<wchar_t>& dest) {
		HANDLE handle = ::CreateFile(GetNormalizationFilesPath().c_str(),
			GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (handle != INVALID_HANDLE_VALUE) {
			::SetFilePointer(handle, 0, NULL, FILE_END);
			std::basic_string<wchar_t> message = src + L" -> " + dest + L" (リネームしました)\r\n";
			DWORD written = 0;
			::WriteFile(handle, message.c_str(), message.size() * sizeof(wchar_t), &written, NULL);
			::CloseHandle(handle);
		}
	}

	/* --------------------------------------------------------------------- */
	/*
	 *  Normalize
	 *
	 *  文字エンコードの指定が不正，Windows で許されていない名前の設定等，
	 *  ファイル名として不適切である名前が指定された場合に，
	 *  「CubeICEによってリネームされたファイルYYYYMMDDHHMMSS(N).ext」
	 *  と言うファイル名にリネームする．
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<wchar_t> Normalize(const std::basic_string<wchar_t>& src) {
		typedef wchar_t char_type;
		typedef std::basic_string<char_type> string_type;
		typedef std::map<string_type, string_type> filemap_type;
		
		static const string_type prefix_ = _T("CubeICEによってリネームされたファイル");
		static filemap_type v_;
		static SYSTEMTIME now_;
		static bool initialized_ = false;

		if (!initialized_) {
			v_.clear();
			::GetLocalTime(&now_);
			initialized_ = true;
		}
		
		std::vector<std::basic_string<wchar_t> > parts;
		clx::split_if(src, parts, clx::is_any_of(L"\\/"));

		static const wchar_t	*reserved_name[] = {
			L"CON",
			L"PRN",
			L"AUX",
			L"NUL",

			L"COM0",
			L"COM1",
			L"COM2",
			L"COM3",
			L"COM4",
			L"COM5",
			L"COM6",
			L"COM7",
			L"COM8",
			L"COM9",

			L"LPT0",
			L"LPT1",
			L"LPT2",
			L"LPT3",
			L"LPT4",
			L"LPT5",
			L"LPT6",
			L"LPT7",
			L"LPT8",
			L"LPT9"
		};
		
		bool escaped = false;
		for(int i = 0 ; i < parts.size() ; ++i) {
			std::basic_string<wchar_t>	&s = parts[i];
			while(!s.empty() && (s[s.size()-1] == L'.' || s[s.size()-1] == L' '))
				s.pop_back();
			if(!s.empty()) {
				for(int j = 0 ; j < sizeof(reserved_name) / sizeof(reserved_name[0]) ; ++j) {
					const int	len = wcslen(reserved_name[j]);
					if(s.substr(0, len) == reserved_name[j] && (s.size() == len || s[len] == L'.')) {
						s = L'_' + s;
						escaped = true;
						break;
					}
				}
			}
		}
		std::basic_string<wchar_t> s;
		std::basic_string<wchar_t> checkname;
		clx::join(parts, s, L"\\");
		clx::join(parts, checkname, L"");

		wchar_t dir[1024] = {};
		if (::GetTempPath(1024, dir) == 0) return s;
		string_type path = dir + checkname;
		HANDLE test = CreateFile(path.c_str(),
			GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE, NULL);
		
		if (test != INVALID_HANDLE_VALUE) {
			DWORD type = GetFileType(test);
			::CloseHandle(test);
			if (type == FILE_TYPE_DISK) {
				if (escaped) PutNormalizationLog(src, s);
				return s;
			}
		}

		// ファイル名をリネームする
		filemap_type::iterator pos = v_.find(s);
		if (pos != v_.end()) return pos->second;
		
		char_type buffer[2048] = {};
		if (v_.empty()) {
			_sntprintf(buffer, sizeof(buffer) / sizeof(char_type),
				_T("%s%d%02d%02d%02d%02d%02d"),
				prefix_.c_str(), now_.wYear, now_.wMonth, now_.wDay, now_.wHour, now_.wMinute, now_.wSecond
			);
		}
		else {
			_sntprintf(buffer, sizeof(buffer) / sizeof(char_type),
				_T("%s%d%02d%02d%02d%02d%02d(%d)"),
				prefix_.c_str(), now_.wYear, now_.wMonth, now_.wDay, now_.wHour, now_.wMinute, now_.wSecond, v_.size()
			);
		}

		string_type::size_type offset = s.find_last_of(_T('.'));
		string_type ext = (offset != string_type::npos) ? s.substr(offset) : _T("");
		string_type result = buffer + ext;
		v_.insert(std::make_pair(s, result));
		
		PutNormalizationLog(src, result);
		return result;
	}
}

#endif // CUBEICE_ENCODING_H

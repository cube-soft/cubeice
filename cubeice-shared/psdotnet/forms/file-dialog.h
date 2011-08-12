// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  psdotnet/forms/file-dialog.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef PSDOTNET_FORMS_FILE_DIALOG_H
#define PSDOTNET_FORMS_FILE_DIALOG_H

#include <psdotnet/config.h>
#include <algorithm>
#include <string>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <boost/foreach.hpp>
#include <boost/utility.hpp>
#include <psdotnet/forms/dialog-result.h>

namespace PsdotNet {
	namespace Forms {
		/* ----------------------------------------------------------------- */
		/*
		 *  FileDialog
		 *
		 *  OPENFILENAME 構造体を使用して表示するダイアログの基底クラス．
		 *  このクラスを直接使用する事はできない．代わりに FileDialog の
		 *  派生クラスである OpenFileDialog クラス，または SaveFileDialog
		 *  クラスを使用する事．
		 */
		/* ----------------------------------------------------------------- */
		class FileDialog {
		public:
			typedef OPENFILENAME data_type;
			typedef DWORD size_type;
			typedef TCHAR char_type;
			typedef std::basic_string<char_type> string_type;
			typedef std::pair<string_type, string_type> string_pair;
			typedef std::vector<string_type> string_array;
			typedef std::vector<string_pair> string_pair_array;
			
		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			FileDialog() : data_(), paths_(), initdir_(), title_(), filters_() {
				data_.Flags |= OFN_EXPLORER;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  ShowDialog
			 *
			 *  ダイアログボックスを表示する．OK ボタンが押された場合には
			 *  DialogResult::OK, それ以外のボタンが押された場合には
			 *  DialogResult::Cancel が返る．
			 */
			/* ------------------------------------------------------------- */
			virtual DialogResult ShowDialog(HWND owner) = 0;
			
			/* ------------------------------------------------------------- */
			/*
			 *  FileName
			 *
			 *  ダイアログボックスで選択されたファイル名を取得する．
			 *  FileName() メンバ関数は，実際には，ファイル名を含むパスが
			 *  返される (System.Windows.Forms.FileDialog.FileName)．
			 */
			/* ------------------------------------------------------------- */
			//const string_type& FileName() const { return paths_.at(0); }
			string_type FileName() const { return !paths_.empty() ? paths_.at(0) : string_type(); }
			
			/* ------------------------------------------------------------- */
			/*
			 *  FileNames
			 *
			 *  ダイアログボックスで選択されたすべてのファイルの名前を
			 *  取得する．
			 */
			/* ------------------------------------------------------------- */
			const string_array& FileNames() const { return paths_; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  DefaultExt
			 *
			 *  ファイルの種類ですべてのファイル (*.*) が指定され，かつ
			 *  拡張子なしのファイル名が入力された場合に自動的に付加する
			 *  拡張子を取得する．
			 */
			/* ------------------------------------------------------------- */
			const string_type& DefaultExt() const { return ext_; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  DefaultExt
			 *
			 *  ファイルの種類ですべてのファイル (*.*) が指定され，かつ
			 *  拡張子なしのファイル名が入力された場合に自動的に付加する
			 *  拡張子を設定する．設定する拡張子は，". (ドット)" なしで
			 *  指定する．
			 */
			/* ------------------------------------------------------------- */
			void DefaultExt(const string_type& ext) {
				ext_ = ext;
				data_.lpstrDefExt = ext_.c_str();
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  AddExtension
			 *
			 *  ユーザが拡張子なしのファイル名を入力した際に，自動的に
			 *  拡張子を付加するかどうかの真偽値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool AddExtension() const { return data_.lpstrDefExt != NULL; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  AddExtension
			 *
			 *  ユーザが拡張子なしのファイル名を入力した際に，自動的に
			 *  拡張子を付加するかどうかの真偽値を設定する．
			 */
			/* ------------------------------------------------------------- */
			void AddExtension(bool enable) {
				if (enable) data_.lpstrDefExt = ext_.c_str();
				else data_.lpstrDefExt = NULL;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  InitialDirectory
			 *
			 *  ダイアログボックスの初期ディレクトリを取得する．
			 */
			/* ------------------------------------------------------------- */
			const string_type& InitialDirectory() const { return initdir_; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  InitialDirectory
			 *
			 *  ダイアログボックスの初期ディレクトリを設定する．同時に
			 *  NativeData().lpstrInitialDir の値も更新される．
			 */
			/* ------------------------------------------------------------- */
			void InitialDirectory(const string_type& path) {
				initdir_ = path;
				data_.lpstrInitialDir = initdir_.c_str();
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Title
			 *
			 *  ダイアログボックスのタイトルバーに表示される文字列を取得
			 *  する．
			 */
			/* ------------------------------------------------------------- */
			const string_type Title() const { return title_; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  Title
			 *
			 *  ダイアログボックスのタイトルバーに表示される文字列を設定
			 *  する．同時に NativeData().lpstrTitle の値も更新される．
			 */
			/* ------------------------------------------------------------- */
			void Title(const string_type& str) {
				title_ = str;
				data_.lpstrTitle = title_.c_str();
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Filters
			 *
			 *  ファイル種類コンボボックスに表示させるフィルターの一覧を
			 *  取得または設定する．各要素は (説明，拡張子) と言う文字列
			 *  ペアになっている．
			 *
			 *  NOTE: System.Windows.Forms.FileDialog では，
			 *  "description1|ext1|description2|ext2|..."
			 *  のように "|" をセパレータとした文字列で取得・設定する
			 *  ようになっているが，PsdotNet では，std::pair<string, string>
			 *  の配列 (vector) で扱うようにする．
			 */
			/* ------------------------------------------------------------- */
			const string_pair_array& Filters() const { return filters_; }
			string_pair_array& Filters() { return filters_; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  AddFilter
			 *
			 *  ファイル種類コンボボックスに表示させるフィルターを追加する．
			 *  filters().push_back(description, extension) と等しい．
			 */
			/* ------------------------------------------------------------- */
			void AddFilter(const string_type& description, const string_type& extension) {
				filters_.push_back(string_pair(description, extension));
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  FilterIndex
			 *
			 *  ファイル種類コンボボックスで，表示させるフィルターの
			 *  インデックスを取得する．
			 */
			/* ------------------------------------------------------------- */
			const size_type& FilterIndex() const { return data_.nFilterIndex; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  FilterIndex
			 *
			 *  ファイル種類コンボボックスで，表示させるフィルターの
			 *  インデックスを設定する．
			 */
			/* ------------------------------------------------------------- */
			void FilterIndex(const size_type& n) { data_.nFilterIndex = n; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  CheckFileExists
			 *
			 *  ユーザーが存在しないファイルを選択しようとしたとき，
			 *  警告ダイアログを表示して選択不可能にするかどうかの真偽値を
			 *  取得する．
			 *
			 *  NOTE: System.Windows.Forms.FileDialog は CheckFileExists()
			 *  の他に，CheckPathExists() と言うプロパティも定義されているが，
			 *  OFN_FILEMUSTEXIST は OFN_PATHMUSTEXIST フラグも含むため
			 *  CheckPathExists プロパティに相当するメンバ関数は用意しない
			 *  事とする．
			 */
			/* ------------------------------------------------------------- */
			bool CheckFileExists() const { return (data_.Flags & OFN_FILEMUSTEXIST) != 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  CheckFileExists
			 *
			 *  ユーザーが存在しないファイルを選択しようとしたとき，
			 *  警告ダイアログを表示して選択不可能にするかどうかの真偽値を
			 *  設定する．
			 */
			/* ------------------------------------------------------------- */
			void CheckFileExists(bool enable) {
				if (enable) data_.Flags |= OFN_FILEMUSTEXIST;
				else data_.Flags &= ~OFN_FILEMUSTEXIST;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  DereferenceLinks
			 *
			 *  ダイアログボックスが，ショートカットで参照されたファイルの
			 *  場所を返すかどうか，またはショートカットの場所 (.lnk) を
			 *  返すかどうかを指定する値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool DereferenceLinks() const { return (data_.Flags & OFN_NODEREFERENCELINKS) == 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  DereferenceLinks
			 *
			 *  ダイアログボックスが，ショートカットで参照されたファイルの
			 *  場所を返すかどうか，またはショートカットの場所 (.lnk) を
			 *  返すかどうかを指定する値を設定する．
			 */
			/* ------------------------------------------------------------- */
			void DereferenceLinks(bool enable) {
				if (enable) data_.Flags &= ~OFN_NODEREFERENCELINKS;
				else data_.Flags |= OFN_NODEREFERENCELINKS;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  RestoreDirectory
			 *
			 *  ダイアログボックスを閉じる前に，現在のディレクトリを
			 *  復元するかどうかを示す値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool RestoreDirectory() const { return (data_.Flags & OFN_NOCHANGEDIR) != 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  RestoreDirectory
			 *
			 *  ダイアログボックスを閉じる前に，現在のディレクトリを
			 *  復元するかどうかを示す値を取得する．
			 */
			/* ------------------------------------------------------------- */
			void RestoreDirectory(bool enable) {
				if (enable) data_.Flags |= OFN_NOCHANGEDIR;
				else data_.Flags &= ~OFN_NOCHANGEDIR;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  ValidateNames
			 *
			 *  ダイアログボックスが有効な Win32 ファイル名だけを受け入れる
			 *  かどうかを示す値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool ValidateNames() const { return (data_.Flags & OFN_NOVALIDATE) == 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  ValidateNames
			 *
			 *  ダイアログボックスが有効な Win32 ファイル名だけを受け入れる
			 *  かどうかを示す値を取得する．
			 */
			/* ------------------------------------------------------------- */
			void ValidateNames(bool enable) {
				if (enable) data_.Flags &= ~OFN_NOVALIDATE;
				else data_.Flags |= OFN_NOVALIDATE;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  ShowHelp
			 *
			 *  ダイアログボックスに [ヘルプ] ボタンを表示するかどうかを
			 *  示す値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool ShowHelp() const { return (data_.Flags & OFN_SHOWHELP) != 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  ShowHelp
			 *
			 *  ダイアログボックスに [ヘルプ] ボタンを表示するかどうかを
			 *  示す値を設定する．
			 *  NOTE: OFN_SHOWHELP フラグを有効にした場合，親ウィンドウが
			 *  指定されていなければならない．
			 */
			/* ------------------------------------------------------------- */
			void ShowHelp(bool enable) {
				if (enable) data_.Flags |= OFN_SHOWHELP;
				else data_.Flags &= ~OFN_SHOWHELP;
			}
			
			/* ------------------------------------------------------------- */
			//  NativeData
			/* ------------------------------------------------------------- */
			const data_type& NativeData() const { return data_; }
			
			/* ------------------------------------------------------------- */
			//  NativeData
			/* ------------------------------------------------------------- */
			data_type& NativeData() { return data_; }
			
		protected:
			/* ------------------------------------------------------------- */
			//  destructor
			/* ------------------------------------------------------------- */
			~FileDialog() {}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Split
			 *
			 *  セパレータをヌル文字として連結されている配列 src を分割して
			 *  文字列の配列 dest に格納する．
			 */
			/* ------------------------------------------------------------- */
			template <class Container>
			bool Split(const Container& src, string_array& dest) {
				dest.clear();
				typename Container::const_iterator first = src.begin();
				while (first != src.end() && *first != 0) {
					typename Container::const_iterator last = std::find(first, src.end(), 0);
					string_type item;
					for (; first != last; ++first) item += *first;
					if (!item.empty()) dest.push_back(item);
					if (first != src.end()) ++first;
				}
				return !dest.empty();
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Split
			 *
			 *  セパレータをヌル文字として連結されている配列 src を分割して
			 *  文字列の配列 dest に格納する．
			 */
			/* ------------------------------------------------------------- */
			template <class T, std::size_t N>
			bool Split(const T (&src)[N], string_array& dest) {
				dest.clear();
				const T* first = src;
				while (first != src + N && *first != 0) {
					const T* last = std::find(first, src + N, 0);
					string_type item;
					for (; first != last; ++first) item += *first;
					if (!item.empty()) dest.push_back(item);
					if (first != src + N) ++first;
				}
				return !dest.empty();
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Join
			 *
			 *  文字列の配列 src をセパレータをヌル文字として連結して
			 *  dest に格納する．src が空の場合は false を返す．
			 */
			/* ------------------------------------------------------------- */
			template <class Container>
			bool Join(const string_array& src, Container& dest) {
				dest.clear();
				BOOST_FOREACH(string_type item, src) {
					dest.insert(dest.end(), item.begin(), item.end());
					dest.push_back(0);
				}
				if (dest.empty()) return false;
				
				dest.push_back(0);
				return true;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Join
			 *
			 *  文字列ペアの配列を文字列の配列に展開して join メンバ関数
			 *  を実行する．
			 */
			/* ------------------------------------------------------------- */
			template <class Container>
			bool Join(const string_pair_array& src, Container& dest) {
				string_array tmp;
				BOOST_FOREACH(string_pair item, src) {
					tmp.push_back(item.first);
					tmp.push_back(item.second);
				}
				return this->Join(tmp, dest);
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  ClearInvalidData
			 *
			 *  show() メンバ関数を終了すると無効になるメンバ変数の値を
			 *  クリアする．
			 */
			/* ------------------------------------------------------------- */
			void ClearInvalidData() {
				data_.lpstrFilter = NULL;
				data_.lpstrFile = NULL;
				data_.nMaxFile = 0;
				data_.lpstrFileTitle = NULL;
				data_.nMaxFileTitle = 0;
			}
			
		protected:
			data_type data_;
			string_array paths_;
			string_type ext_;
			string_type initdir_;
			string_type title_;
			string_pair_array filters_;
		};
		
		/* ----------------------------------------------------------------- */
		/*
		 *  OpenFileDialog
		 *
		 *  「ファイルを開く」ダイアログボックスを表示するクラス．
		 *  .NetFramework のSystem.Windows.Forms.OpenFileDialog を参考に
		 *  各メンバ関数を定義している．
		 *  http://msdn.microsoft.com/ja-jp/library/y1kh29w3.aspx
		 */
		/* ----------------------------------------------------------------- */
		class OpenFileDialog : public FileDialog, boost::noncopyable {
		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			OpenFileDialog() : FileDialog() {
				this->CheckFileExists(true);
			}
			
			/* ------------------------------------------------------------- */
			//  destructor
			/* ------------------------------------------------------------- */
			~OpenFileDialog() {}
			
			/* ------------------------------------------------------------- */
			//  ShowDialog
			/* ------------------------------------------------------------- */
			virtual DialogResult ShowDialog(HWND owner = NULL) {
				char_type path[PSDOTNET_MAX_PATH] = {};
				char_type filename[PSDOTNET_MAX_PATH] = {};
				
				std::vector<char_type> filter;
				char_type* pf = this->Join(filters_, filter) ? reinterpret_cast<char_type*>(&filter[0]) : NULL;
				
				data_.lStructSize     = sizeof(data_);
				data_.hwndOwner       = owner;
				data_.lpstrFilter     = pf;
				data_.lpstrFile       = path;
				data_.nMaxFile        = sizeof(path) / sizeof(char_type);
				data_.lpstrFileTitle  = filename;
				data_.nMaxFileTitle   = sizeof(filename) / sizeof(char_type);
				
				BOOL result = ::GetOpenFileName(&data_);
				
				if (result) {
					paths_.clear();
					if (this->Multiselect()) {
						string_array tmp;
						if (this->Split(path, tmp)) {
							paths_.clear();
							if (tmp.size() > 1) {
								string_type folder = tmp.at(0);
								for (size_type i = 1; i < tmp.size(); ++i) paths_.push_back(folder + _T('\\') + tmp.at(i));
							}
							else if (tmp.size() == 1) paths_.push_back(tmp.at(0));
						}
					}
					else paths_.push_back(path);
					
					// NOTE: System.Windows.Forms.FileDialog が filename_ に
					// 当たるプロパティを持っていないため，この値は捨てる．
					// filename_ = filename;
					
					this->ClearInvalidData();
				}
				
				if (result != FALSE) return DialogResult::OK;
				return DialogResult::Cancel;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Multiselect
			 *
			 *  ダイアログボックスで複数のファイルを選択できるかどうかを
			 *  示す値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool Multiselect() const { return (this->data_.Flags & OFN_ALLOWMULTISELECT) != 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  Multiselect
			 *
			 *  ダイアログボックスで複数のファイルを選択できるかどうかを
			 *  示す値を設定する．
			 */
			/* ------------------------------------------------------------- */
			void Multiselect(bool enable) {
				if (enable) this->data_.Flags |= OFN_ALLOWMULTISELECT;
				else this->data_.Flags &= ~OFN_ALLOWMULTISELECT;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  ShowReadOnly
			 *
			 *  ダイアログボックスに読み取り専用チェックボックスが表示され
			 *  ているかどうかを示す値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool ShowReadOnly() const { return (this->data_.Flags & OFN_HIDEREADONLY) == 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  ShowReadOnly
			 *
			 *  ダイアログボックスに読み取り専用チェックボックスが表示され
			 *  ているかどうかを示す値を設定する．
			 */
			/* ------------------------------------------------------------- */
			void ShowReadOnly(bool enable) {
				if (enable) this->data_.Flags &= ~OFN_HIDEREADONLY;
				else this->data_.Flags |= OFN_HIDEREADONLY;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  ReadOnlyChecked
			 *
			 *  読み取り専用チェックボックスがオンかオフかを示す値を取得
			 *  する．
			 */
			/* ------------------------------------------------------------- */
			bool ReadOnlyChecked() const { return (this->data_.Flags & OFN_READONLY) != 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  ReadOnlyChecked
			 *
			 *  読み取り専用チェックボックスがオンかオフかを示す値を設定
			 *  する．
			 */
			/* ------------------------------------------------------------- */
			void ReadOnlyChecked(bool enable) {
				if (enable) this->data_.Flags |= OFN_READONLY;
				else this->data_.Flags &= ~OFN_READONLY;
			}
		};
		
		/* ----------------------------------------------------------------- */
		/*
		 *  SaveFileDialog
		 *
		 *  「ファイルを保存」ダイアログボックスを表示するクラス．
		 *  .NetFramework のSystem.Windows.Forms.SaveFileDialog を参考に
		 *  各メンバ関数を定義している．
		 *  http://msdn.microsoft.com/ja-jp/library/9tah40xd.aspx
		 */
		/* ----------------------------------------------------------------- */
		class SaveFileDialog : public FileDialog, boost::noncopyable {
		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			SaveFileDialog() : FileDialog() {
				this->CheckFileExists(false);
				this->CreatePrompt(false);
				this->OverwritePrompt(true);
			}
			
			/* ------------------------------------------------------------- */
			//  destructor
			/* ------------------------------------------------------------- */
			~SaveFileDialog() {}
			
			/* ------------------------------------------------------------- */
			//  ShowDialog
			/* ------------------------------------------------------------- */
			virtual DialogResult ShowDialog(HWND owner = NULL) {
				char_type path[PSDOTNET_MAX_PATH] = {};
				char_type filename[PSDOTNET_MAX_PATH] = {};
				
				std::vector<char_type> filter;
				if (filters_.empty()) this->AddFilter(_T("すべてのファイル (*.*)"), _T("*.*"));
				char_type* pf = this->Join(filters_, filter) ? reinterpret_cast<char_type*>(&filter[0]) : NULL;
				
				data_.lStructSize     = sizeof(data_);
				data_.hwndOwner       = owner;
				data_.lpstrFilter     = pf;
				data_.lpstrFile       = path;
				data_.nMaxFile        = sizeof(path) / sizeof(char_type);
				data_.lpstrFileTitle  = filename;
				data_.nMaxFileTitle   = sizeof(filename) / sizeof(char_type);
				
				BOOL result = ::GetSaveFileName(&data_);
				
				if (result) {
					paths_.clear();
					paths_.push_back(path);
					
					// NOTE: System.Windows.Forms.FileDialog が filename_ に
					// 当たるプロパティを持っていないため，この値は捨てる．
					// filename_ = filename;
					
					this->ClearInvalidData();
				}
				
				if (result != FALSE) return DialogResult::OK;
				return DialogResult::Cancel;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  OverwritePrompt
			 *
			 *  既に存在するファイルを選択しようとしたとき，上書きの
			 *  確認ダイアログを表示するかどうかの真偽値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool OverwritePrompt() const { return (data_.Flags & OFN_OVERWRITEPROMPT) != 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  OverwritePrompt
			 *
			 *  既に存在するファイルを選択しようとしたとき，上書きの
			 *  確認ダイアログを表示するかどうかの真偽値を設定する．
			 */
			/* ------------------------------------------------------------- */
			void OverwritePrompt(bool enable) {
				if (enable) data_.Flags |= OFN_OVERWRITEPROMPT;
				else data_.Flags &= ~OFN_OVERWRITEPROMPT;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  CreatePrompt
			 *
			 *  現在存在しないファイルを指定しようとしたとき，作成の確認
			 *  ダイアログを表示するかどうかの真偽値を取得する．
			 */
			/* ------------------------------------------------------------- */
			bool CreatePrompt() const { return (data_.Flags & OFN_CREATEPROMPT) != 0; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  CreatePrompt
			 *
			 *  現在存在しないファイルを指定しようとしたとき，作成の確認
			 *  ダイアログを表示するかどうかの真偽値を設定する．
			 */
			/* ------------------------------------------------------------- */
			void CreatePrompt(bool enable) {
				if (enable) data_.Flags |= OFN_CREATEPROMPT;
				else data_.Flags &= ~OFN_CREATEPROMPT;
			}
		};
	}
}

#endif // PSDOTNET_FORMS_FILE_DIALOG_H

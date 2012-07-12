// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/utility.h
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
#ifndef CUBEICE_USER_SETTING_UTILITY_H
#define CUBEICE_USER_SETTING_UTILITY_H

#include <cubeice/config.h>
#include <cubeice/context.h>
#include <map>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// CompressionLevel
	///
	/// <summary>
	/// 圧縮レベルを定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	namespace CompressionLevel {
		enum {
			None                = 0, // 無圧縮
			Fastest             = 1, // 最速
			Fast                = 3, // 高速
			Normal              = 5, // 標準
			Maximum             = 7, // 最高
			Ultra               = 9  // 超圧縮
		};
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// OutputKind
	///
	/// <summary>
	/// 出力先の決定方法を定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	namespace OutputKind {
		enum {
			Specific            = 0x00, // 予め決められたフォルダを出力先とします。
			SameAsSource        = 0x01, // 入力元と同じフォルダを出力先とします。
			Runtime             = 0x02  // 実行時に指定します。
		};
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// OverwriteKind
	///
	/// <summary>
	/// 同名のファイルが存在した場合の上書き方法を定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	namespace OverwriteKind {
		enum {
			Force               = 0x00, // 強制的に上書きします。
			Confirm             = 0x01, // 上書きの確認を行います。
			ConfirmOlder        = 0x02, // 更新日時が新しい場合は強制的に上書きします。
			Add                 = 0x03  // 追加します。
		};
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// CreateFolderKind
	///
	/// <summary>
	/// 解凍時にフォルダを作成するかどうかを定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	namespace CreateFolderKind {
		enum {
			Skip                = 0x00, // フォルダを作成しません。
			Create              = 0x01, // フォルダを作成します。
			SkipSingleFolder    = 0x02, // 単一フォルダの場合、フォルダを作成しません。
			SkipSingleFile      = 0x03  // 単一ファイル、または単一フォルダの場合、フォルダを作成しません
		};
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// OpenKind
	///
	/// <summary>
	/// 圧縮・解凍処理の終了後、出力先フォルダを開くかどうかを定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	namespace OpenKind {
		enum {
			Skip                = 0x00, // 開かずに終了します。
			Open                = 0x01, // フォルダを開きます。
			SkipDesktop         = 0x02  // 出力先がデスクトップの場合、開かずに終了します。
		};
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// EncodingKind
	///
	/// <summary>
	/// 文字エンコーディングを変換する際の、元の文字エンコーディングの指定
	/// 方法を定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	namespace EncodingKind {
		enum {
			Auto                = 0x00,
			Shift_JIS           = 0x01,
			EUC_JP              = 0x02,
			UTF8                = 0x03,
			Unicode             = 0x04
		};
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// ContextKind
	///
	/// <summary>
	/// コンテキストメニューに表示する項目を定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	namespace ContextKind {
		enum {
			Compression                 = 0x00000001, // 圧縮
			Decompression               = 0x00000002, // 解凍
			Setting                     = 0x00000004, // 設定
			Mail                        = 0x00000008, // 圧縮してメール送信
			DecompressionHere           = 0x00000010, // ここに解凍
			DecompressionDesktop        = 0x00000020, // デスクトップに解凍
			DecompressionRuntime        = 0x00000040, // 解凍先を指定
			DecompressionMyDocuments    = 0x00000080, // マイドキュメントに解凍
			CompressionZip              = 0x00000100, // zip
			CompressionZipPassword      = 0x00000200, // zip (パスワード)
			Compression7z               = 0x00000400, // 7z
			CompressionBzip2            = 0x00000800, // bzip2
			CompressionGzip             = 0x00001000, // gzip
			CompressionDetail           = 0x00002000, // 詳細設定
			CompressionSelfExecutable   = 0x00004000, // exe
			MailZip                     = 0x00010000, // zip
			MailZipPassword             = 0x00020000, // zip (パスワード) 
			Mail7z                      = 0x00040000, // 7z
			MailBzip2                   = 0x00080000, // bzip2
			MailGzip                    = 0x00100000, // gzip
			MailDetail                  = 0x00200000, // 詳細設定
			MailSelfExecutable          = 0x00400000, // exe
		};
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// GetCompressionContexts
	///
	/// <summary>
	/// 圧縮に関するコンテキストメニューに表示する項目を取得します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	inline void GetCompressionContexts(std::map<int, Context>& dest, const string_type& install) {
		string_type program = install + _T("\\") + CUBEICE_NAME;
		dest[ContextKind::CompressionZip] = Context(_T("zip"), program, _T("/c:zip"), _T(""));
		dest[ContextKind::CompressionZipPassword] = Context(_T("zip (パスワード)"), program, _T("/c:zip /p"), _T(""));
		dest[ContextKind::Compression7z] = Context(_T("7z"), program, _T("/c:7z"), _T(""));
		dest[ContextKind::CompressionBzip2] = Context(_T("bzip2"), program, _T("/c:bzip2"), _T(""));
		dest[ContextKind::CompressionGzip] = Context(_T("gzip"), program, _T("/c:gzip"), _T(""));
		dest[ContextKind::CompressionSelfExecutable] = Context(_T("exe"), program, _T("/c:exe"), _T(""));
		dest[ContextKind::CompressionDetail] = Context(_T("詳細設定"), program, _T("/c:detail"), _T(""));
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// GetDecompressionContexts
	///
	/// <summary>
	/// 解凍に関するコンテキストメニューに表示する項目を取得します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	inline void GetDecompressionContexts(std::map<int, Context>& dest, const string_type& install) {
		string_type program = install + _T("\\") + CUBEICE_NAME;
		dest[ContextKind::DecompressionHere] = Context(_T("ここに解凍"), program, _T("/x /o:source"), _T(""));
		dest[ContextKind::DecompressionDesktop] = Context(_T("デスクトップに解凍"), program, _T("/x /o:desktop"), _T(""));
		dest[ContextKind::DecompressionRuntime] = Context(_T("場所を指定して解凍"), program, _T("/x /o:runtime"), _T(""));
		dest[ContextKind::DecompressionMyDocuments] = Context(_T("マイドキュメントに解凍"), program, _T("/x /o:mydocuments"), _T(""));
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// GetMailContexts
	///
	/// <summary>
	/// 圧縮してメール送信に関するコンテキストメニューに表示する項目を
	/// 取得します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	inline void GetMailContexts(std::map<int, Context>& dest, const string_type& install) {
		string_type program = install + _T("\\") + CUBEICE_NAME;
		dest[ContextKind::MailZip] = Context(_T("zip"), program, _T("/c:zip /m"), _T(""));
		dest[ContextKind::MailZipPassword] = Context(_T("zip (パスワード)"), program, _T("/c:zip /p /m"), _T(""));
		dest[ContextKind::Mail7z] = Context(_T("7z"), program, _T("/c:7z /m"), _T(""));
		dest[ContextKind::MailBzip2] = Context(_T("bzip2"), program, _T("/c:bzip2 /m"), _T(""));
		dest[ContextKind::MailGzip] = Context(_T("gzip"), program, _T("/c:gzip /m"), _T(""));
		dest[ContextKind::MailSelfExecutable] = Context(_T("exe"), program, _T("/c:exe /m"), _T(""));
		dest[ContextKind::MailDetail] = Context(_T("詳細設定"), program, _T("/c:detail /m"), _T(""));
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// GetContexts
	///
	/// <summary>
	/// ビルトイン型のコンテキストメニューを展開します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	inline void ConvertContext(int src, ContextContainer& dest, const string_type& install) {
		string_type program = install + _T("\\") + CUBEICE_NAME;
		
		if ((src & ContextKind::Compression) != 0) {
			Context cx(_T("圧縮"), program + _T(",1"));
			std::map<int, Context> v;
			GetCompressionContexts(v, install);
			for (std::map<int, Context>::iterator pos = v.begin(); pos != v.end(); ++pos) {
				if ((src & pos->first) != 0) cx.Children().push_back(pos->second);
			}
			if (!cx.Children().empty()) dest.push_back(cx);
		}
		
		if ((src & ContextKind::Decompression) != 0) {
			Context cx(_T("解凍"), program + _T(",2"));
			std::map<int, Context> v;
			GetDecompressionContexts(v, install);
			for (std::map<int, Context>::iterator pos = v.begin(); pos != v.end(); ++pos) {
				if ((src & pos->first) != 0) cx.Children().push_back(pos->second);
			}
			if (!cx.Children().empty()) dest.push_back(cx);
		}
		
		if ((src & ContextKind::Mail) != 0) {
			Context cx(_T("圧縮してメール送信"), program + _T(",1"));
			std::map<int, Context> v;
			GetMailContexts(v, install);
			for (std::map<int, Context>::iterator pos = v.begin(); pos != v.end(); ++pos) {
				if ((src & pos->first) != 0) cx.Children().push_back(pos->second);
			}
			if (!cx.Children().empty()) dest.push_back(cx);
		}
		
		if ((src & ContextKind::Setting) != 0) {
			dest.push_back(Context(_T("設定"), install + _T("\\") + CUBEICE_SETTING_NAME, _T(""), program + _T(",0")));
		}
	}
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_UTILITY_H

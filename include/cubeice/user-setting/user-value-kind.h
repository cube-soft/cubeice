/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/config.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_USER_SETTING_CONFIG_H
#define CUBEICE_USER_SETTING_CONFIG_H

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	/*!
	 *  OutputKind
	 *
	 *  出力先の決定方法を定義します。
	 */
	/* --------------------------------------------------------------------- */
	namespace OutputKind {
		enum {
			Runtime             = 0x00, // 実行時に指定します。
			Specific            = 0x01, // 予め決められたフォルダを出力先とします。
			SameAsSource        = 0x02  // 入力元と同じフォルダを出力先とします。
		};
	}
	
	/* --------------------------------------------------------------------- */
	/*!
	 *  OverwriteKind
	 *
	 *  同名のファイルが存在した場合の上書き方法を定義します。
	 */
	/* --------------------------------------------------------------------- */
	namespace OverwriteKind {
		enum {
			Force               = 0x00, // 強制的に上書きします。
			Confirm             = 0x01, // 上書きの確認を行います。
			ConfirmOlder        = 0x02  // 更新日時が新しい場合は強制的に上書きします。
		};
	}
	
	/* --------------------------------------------------------------------- */
	/*!
	 *  CreateFolderKind
	 *
	 *  解凍時にフォルダを作成するかどうかを定義します。
	 */
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
	/*!
	 *  OpenKind
	 *
	 *  圧縮・解凍処理の終了後、出力先フォルダを開くかどうかを定義します。
	 */
	/* --------------------------------------------------------------------- */
	namespace OpenKind {
		enum {
			Skip                = 0x00, // 開かずに終了します。
			Open                = 0x01, // フォルダを開きます。
			SkipDesktop         = 0x02  // 出力先がデスクトップの場合、開かずに終了します。
		};
	}
	
	/* --------------------------------------------------------------------- */
	/*!
	 *  EncodingKind
	 *
	 *  文字エンコーディングを変換する際の、元の文字エンコーディングの指定
	 *  方法を定義します。
	 */
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
	/*!
	 *  ContextKind
	 *
	 *  コンテキストメニューに表示する項目を定義します。
	 */
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
			CompressionSevenZip         = 0x00000400, // 7z
			CompressionBzip2            = 0x00000800, // bzip2
			CompressionGzip             = 0x00001000, // gzip
			CompressionDetail           = 0x00002000, // 詳細設定
			CompressionSelfExecutable   = 0x00004000, // exe
			MailZip                     = 0x00010000, // zip
			MailZipPassword             = 0x00020000, // zip (パスワード) 
			MailSevenZip                = 0x00040000, // 7z
			MailBzip2                   = 0x00080000, // bzip2
			MailGzip                    = 0x00100000, // gzip
			MailDetail                  = 0x00200000, // 詳細設定
			MailSelfExecutable          = 0x00400000  // exe
		};
	}
	
	/* --------------------------------------------------------------------- */
	/*!
	 *  IconKind
	 *
	 *  使用できるアイコンの種類を定義します。
	 */
	/* --------------------------------------------------------------------- */
	namespace IconKind {
		enum {
			None                = 0x00,              // アイコンを表示しない
			Logo                = IDB_LOGO,          // CubeICE のロゴ用アイコン
			Compression         = IDB_COMPRESSION,   // 圧縮用アイコン
			Decompression       = IDB_DECOMPRESSION, // 解凍用アイコン
			File                = IDB_FILE,          // CubeICE の圧縮ファイル用アイコン
			Folder              = IDB_FOLDER         // フォルダアイコン
		};
	}
	
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_USER_VALUE_KIND_H

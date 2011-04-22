/* ------------------------------------------------------------------------- */
/*
 *  resource.h
 *
 *  Copyright (c) 2010 CubeSoft.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - No names of its contributors may be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Last-modified: Wed 17 Nov 2010 17:39:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_RESOURCE_H
#define CUBEICE_RESOURCE_H

#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

/* ------------------------------------------------------------------------- */
//  「一般」タブ
/* ------------------------------------------------------------------------- */
#define IDC_EXT_GROUP               2100
#define IDC_ZIP_CHECKBOX            2101 // ZIP_FLAG
#define IDC_LZH_CHECKBOX            2102 // LZH_FLAG
#define IDC_RAR_CHECKBOX            2103 // RAR_FLAG
#define IDC_TAR_CHECKBOX            2104 // TAR_FLAG
#define IDC_GZ_CHECKBOX             2105 // GZ_FLAG
#define IDC_SEVENZIP_CHECKBOX       2106 // SEVENZIP_FLAG
#define IDC_ARJ_CHCKBOX             2107 // ARJ_FLAG
#define IDC_BZ2_CHECKBOX            2108 // BZ2_FLAG
#define IDC_CAB_CHECKBOX            2109 // CAB_FLAG
#define IDC_CHM_CHECKBOX            2110 // CHM_FLAG
#define IDC_CPIO_CHECKBOX           2111 // CPIO_FLAG
#define IDC_DEB_CHECKBOX            2112 // DEB_FLAG
#define IDC_DMG_CHECKBOX            2113 // DMG_FLAG
#define IDC_ISO_CHECKBOX            2114 // ISO_FLAG
#define IDC_RPM_CHECKBOX            2115 // RPM_FLAG
#define IDC_TBZ_CHECKBOX            2116 // TBZ_FLAG
#define IDC_TGZ_CHECKBOX            2117 // TGZ_FLAG
#define IDC_WIM_CHECKBOX            2118 // WIM_FLAG
#define IDC_XAR_CHECKBOX            2119 // XAR_FLAG
#define IDC_XZ_CHECKBOX             2120 // XZ_FLAG

#define IDC_CONTEXT_GROUP           2200 // 追加するコンテキストメニュー
#define IDC_CT_COMPRESS_CHECKBOX    2201 // COMPRESS_FLAG
#define IDC_CT_DECOMPRESS_CHECKBOX  2202 // DECOMPRESS_FLAG
#define IDC_CT_MAIL_CHECKBOX        2203 // MAIL_FLAG

// 圧縮メニューのサブメニュー
#define IDC_COMP_ZIP_CHECKBOX       2204 // COMP_ZIP_FLAG
#define IDC_COMP_ZIP_PASS_CHECKBOX  2205 // COMP_ZIP_PASS_FLAG
#define IDC_COMP_SEVENZIP_CHECKBOX  2206 // COMP_SEVENZIP_FLAG
#define IDC_COMP_BZIP2_CHECKBOX     2207 // COMP_BZIP2_FLAG
#define IDC_COMP_GZIP_CHECKBOX      2208 // COMP_GZIP_FLAG
#define IDC_COMP_DETAIL_CHECKBOX    2209 // COMP_DETAIL_FLAG

// 解凍メニューのサブメニュー
#define IDC_DECOMP_HERE_CHECKBOX    2210 // DECOMP_HERE_FLAG
#define IDC_DECOMP_DESKTOP_CHECKBOX 2211 // DECOMP_DESKTOP_FLAG
#define IDC_DECOMP_RUNTIME_CHECKBOX 2212 // DECOMP_RUNTIME_FLAG
#define IDC_DECOMP_MYDOCUMENTS_CHECKBOX 2219 // DECOMP_MYDOCUMENTS_FLAG

// 圧縮してメール送信のサブメニュー
#define IDC_MAIL_ZIP_CHECKBOX       2213 // COMP_ZIP_FLAG
#define IDC_MAIL_ZIP_PASS_CHECKBOX  2214 // COMP_ZIP_PASS_FLAG
#define IDC_MAIL_SEVENZIP_CHECKBOX  2215 // COMP_SEVENZIP_FLAG
#define IDC_MAIL_BZIP2_CHECKBOX     2216 // COMP_BZIP2_FLAG
#define IDC_MAIL_GZIP_CHECKBOX      2217 // COMP_GZIP_FLAG
#define IDC_MAIL_DETAIL_CHECKBOX    2218 // COMP_DETAIL_FLAG

#define IDC_SHORTCUT_GROUP          2300 // デスクトップに作成するショートカット
#define IDC_SC_COMPRESS_CHECKBOX    2301 // COMPRESS_FLAG
#define IDC_SC_DECOMPRESS_CHECKBOX  2302 // DECOMPRESS_FLAG
#define IDC_SC_SETTING_CHECKBOX     2303 // SETTING_FLAG
#define IDC_SC_TYPE_COMBOBOX        2304 // 圧縮の種類

#ifdef nouse
#define IDC_ADD_ZIP_CHECKBOX        2204 // ZIP_FLAG
#define IDC_ADD_SEVENZIP_CHECKBOX   2205 // SEVENZIP_FLAG
#define IDC_ADD_BZ2_CHECKBOX        2206 // BZ2_FLAG
#define IDC_ADD_GZ_CHECKBOX         2207 // GZ_FLAG
#define IDC_ADD_EXE_CHECKBOX        2208 // EXE_FLAG
#endif

/* ------------------------------------------------------------------------- */
//  「圧縮/解凍」タブ
/* ------------------------------------------------------------------------- */
#define IDC_DEST_GROUP              3000
#define IDC_SPECIFIC_RADIO          3001 // 指定したフォルダ
#define IDC_SOURCE_RADIO            3002 // 元のファイルと同じフォルダ
#define IDC_RUNTIME_RADIO           3003 // 実行時に指定する
#define IDC_DEST_TEXTBOX            3004
#define IDC_DEST_BUTTON             3005

#define IDC_DETAIL_GROUP            3100
#define IDC_OVERWRITE_CHECKBOX      3101 // 上書きの確認を行う
#define IDC_NEWER_CHECKBOX          3102 // 更新日時が新しい場合は確認しない
#define IDC_CREATE_FOLDER_CHECKBOX  3103 // フォルダを自動生成する
#define IDC_SINGLE_FOLDER_CHECKBOX  3109 // 単一フォルダの場合は生成しない
#define IDC_CHARCODE_CHECKBOX       3104 // ファイル名の文字コードを変換する
#define IDC_FILTER_CHECKBOX         3105 // ファイルのフィルタんリングを行う
#define IDC_POSTOPEN_CHECKBOX       3106 // 保存先フォルダを開
#define IDC_SKIP_DESKTOP_CHECKBOX   3107 // デスクトップの場合は開かない
#define IDC_MAIL_REMOVE_CHECKBOX    3110 // メール添付の際にファイルを残さない
#define IDC_REPORT_CHECKBOX         3108 // エラーレポートを表示する
#define IDC_TOOLTIP_CHECKBOX        3111 // ツールチップにファイル一覧を表示する
#define IDC_MAX_FILELIST_TEXTBOX    3112 // ツールチップの表示する最大件数
#define IDC_FILELIST_UPDOWN         3113 // 同スピンコントロール
#define IDC_REMOVE_SRC_CHECKBOX     3115 // 元ファイルを削除する

/* ------------------------------------------------------------------------- */
//  「フィルタリング」タブ
/* ------------------------------------------------------------------------- */
#define IDC_FILTER_TEXTBOX          4001
#define IDC_FILTER_LABEL1           4002
#define IDC_FILTER_LABEL2           4003
#define IDC_FILTER_LABEL3           4004

/* ------------------------------------------------------------------------- */
//  「バージョン情報」タブ
/* ------------------------------------------------------------------------- */
#define IDC_LOGO_PICTUREBOX         5000
#define IDC_NAME_LABEL              5001
#define IDC_VERSION_LABEL           5002
#define IDC_COMPANY_LABEL           5003
#define IDC_URI_LABEL               5004
#define IDC_UPDATE_CHECKBOX         5005

#endif // CUBEICE_RESOURCE_H

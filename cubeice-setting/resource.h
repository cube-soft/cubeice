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

/* ------------------------------------------------------------------------- */
//  「一般」タブ
/* ------------------------------------------------------------------------- */
#define IDC_EXT_GROUP				2100
#define IDC_ZIP_CHECKBOX			2101
#define ZIP_FLAG					0x00000001
#define IDC_LZH_CHECKBOX			2102
#define LZH_FLAG					0x00000002
#define IDC_RAR_CHECKBOX			2103
#define RAR_FLAG					0x00000004
#define IDC_TAR_CHECKBOX			2104
#define TAR_FLAG					0x00000008
#define IDC_GZ_CHECKBOX				2105
#define GZ_FLAG						0x00000010
#define IDC_SEVENZIP_CHECKBOX		2106
#define SEVENZIP_FLAG				0x00000020
#define IDC_ARJ_CHCKBOX				2107
#define ARJ_FLAG					0x00000040
#define IDC_BZ2_CHECKBOX			2108
#define BZ2_FLAG					0x00000080
#define IDC_CAB_CHECKBOX			2109
#define CAB_FLAG					0x00000100
#define IDC_CHM_CHECKBOX			2110
#define CHM_FLAG					0x00000200
#define IDC_CPIO_CHECKBOX			2111
#define CPIO_FLAG					0x00000400
#define IDC_DEB_CHECKBOX			2112
#define DEB_FLAG					0x00000800
#define IDC_DMG_CHECKBOX			2113
#define DMG_FLAG					0x00001000
#define IDC_ISO_CHECKBOX			2114
#define ISO_FLAG					0x00002000
#define IDC_RPM_CHECKBOX			2115
#define RPM_FLAG					0x00004000
#define IDC_TBZ_CHECKBOX			2116
#define TBZ_FLAG					0x00008000
#define IDC_TGZ_CHECKBOX			2117
#define TGZ_FLAG					0x00010000
#define IDC_WIM_CHECKBOX			2118
#define WIM_FLAG					0x00020000
#define IDC_XAR_CHECKBOX			2119
#define XAR_FLAG					0x00040000
#define IDC_XZ_CHECKBOX				2120
#define XZ_FLAG						0x00080000

#define IDC_CONTEXT_GROUP			2200
#define IDC_ARCHIVE_CHECKBOX		2201
#define ARCHIVE_FLAG				0x0001
#define IDC_EXPAND_CHECKBOX			2202
#define EXPAND_FLAG					0x0002
#define IDC_SETTING_CHECKBOX		2203
#define SETTING_FLAG				0x0004

#define IDC_ADD_ZIP_CHECKBOX		2204 // ZIP_FLAG
#define IDC_ADD_SEVENZIP_CHECKBOX	2205 // SEVENZIP_FLAG
#define IDC_ADD_BZ2_CHECKBOX		2206 // BZ2_FLAG
#define IDC_ADD_GZ_CHECKBOX			2207 // GZ_FLAG

/* ------------------------------------------------------------------------- */
//  「圧縮/解凍」タブ
/* ------------------------------------------------------------------------- */
#define IDC_DEST_GROUP				3000
#define IDC_SPECIFIC_CHECKBOX		3001 // 指定したフォルダ
#define IDC_SOURCE_CHECKBOX			3002 // 元のファイルと同じフォルダ
#define IDC_RUNTIME_CHECKBOX		3003 // 実行時に指定する
#define IDC_DEST_TEXTBOX			3004
#define IDC_DEST_BUTTON				3005

#define IDC_DETAIL_GROUP			3100
#define IDC_OVERWRITE_CHECKBOX		3101 // 上書きの確認を行う
#define IDC_NEWER_CHECKBOX			3102 // 更新日時が新しい場合は確認しない
#define IDC_MAKE_FOLDER_CHECKBOX	3103 // フォルダを自動生成する
#define IDC_CHARCODE_CHECKBOX		3104 // ファイル名の文字コードを変換する
#define IDC_FILTER_CHECKBOX			3105 // ファイルのフィルタんリングを行う
#define IDC_POSTOPEN_CHECKBOX		3106 // 保存先フォルダを開く
#define IDC_ZIP_DETAIL_COMBOBOX		3107

/* ------------------------------------------------------------------------- */
//  「フィルタリング」タブ
/* ------------------------------------------------------------------------- */
#define IDC_FILTER_LABEL			4000
#define IDC_FILTER_TEXTBOX			4001

/* ------------------------------------------------------------------------- */
//  「バージョン情報」タブ
/* ------------------------------------------------------------------------- */
#define IDC_LOGO_PICTUREBOX			5000
#define IDC_NAME_LABEL				5001
#define IDC_VERSION_LABEL			5002
#define IDC_COMPANY_LABEL			5003
#define IDC_URI_LABEL				5004

#endif // CUBEICE_RESOURCE_H

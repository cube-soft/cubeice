// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/resource.h
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
#ifndef CUBEICE_SETTING_RESOURCE_H
#define CUBEICE_SETTING_RESOURCE_H

#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

/* ------------------------------------------------------------------------- */
//  関連付けグループ
/* ------------------------------------------------------------------------- */
#define IDC_ASSOCIATION             2100
#define IDC_ASSOC_ZIP               2101
#define IDC_ASSOC_LZH               2102
#define IDC_ASSOC_RAR               2103
#define IDC_ASSOC_TAR               2104
#define IDC_ASSOC_GZ                2105
#define IDC_ASSOC_7Z                2106
#define IDC_ASSOC_ARJ               2107
#define IDC_ASSOC_BZ2               2108
#define IDC_ASSOC_CAB               2109
#define IDC_ASSOC_CHM               2110
#define IDC_ASSOC_CPIO              2111
#define IDC_ASSOC_DEB               2112
#define IDC_ASSOC_DMG               2113
#define IDC_ASSOC_ISO               2114
#define IDC_ASSOC_RPM               2115
#define IDC_ASSOC_TBZ               2116
#define IDC_ASSOC_TGZ               2117
#define IDC_ASSOC_WIM               2118
#define IDC_ASSOC_XAR               2119
#define IDC_ASSOC_XZ                2120
#define IDC_ASSOC_JAR               2121
#define IDC_ASSOC_CHECKALL          2198 // すべて選択
#define IDC_ASSOC_RESETALL          2199 // すべて解除

/* ------------------------------------------------------------------------- */
//  コンテキストメニューグループ
/* ------------------------------------------------------------------------- */
#define IDC_CONTEXT                 2200
#define IDC_CX_COMPRESSION          2201 // 圧縮
#define IDC_CX_DECOMPRESSION        2202 // 解凍
#define IDC_CX_MAIL                 2203 // 圧縮してメール送信
#define IDC_CX_ZIP                  2204 // zip
#define IDC_CX_ZIP_P                2205 // zip (パスワード)
#define IDC_CX_7Z                   2206 // 7z
#define IDC_CX_BZIP2                2207 // bzip2
#define IDC_CX_GZIP                 2208 // gzip
#define IDC_CX_EXE                  2220 // exe
#define IDC_CX_DETAIL               2209 // 詳細設定
#define IDC_CX_HERE                 2210 // ここに解凍
#define IDC_CX_DESKTOP              2211 // デスクトップに解凍
#define IDC_CX_RUNTIME              2212 // 場所を指定して解凍
#define IDC_CX_MYDOCUMENTS          2219 // マイドキュメントに解凍
#define IDC_CX_ZIP_M                2213 // zip (メール)
#define IDC_CX_ZIP_P_M              2214 // zip (パスワード、メール)
#define IDC_CX_7Z_M                 2215 // 7z (メール)
#define IDC_CX_BZIP2_M              2216 // bzip2 (メール)
#define IDC_CX_GZIP_M               2217 // gzip (メール)
#define IDC_CX_EXE_M                2221 // exe (メール)
#define IDC_CX_DETAIL_M             2218 // 詳細設定 (メール)
#define IDC_CX_CUSTOMIZE            2250 // カスタマイズ
#define IDC_CX_RESET                2251 // リセット

/* ------------------------------------------------------------------------- */
//  コンテキストメニューグループ
/* ------------------------------------------------------------------------- */
#define IDC_SHORTCUT                2300
#define IDC_SC_COMPRESSION          2301 // COMPRESS_FLAG
#define IDC_SC_DECOMPRESSION        2302 // DECOMPRESS_FLAG
#define IDC_SC_SETTING              2303 // SETTING_FLAG
#define IDC_DETAIL_DEFAULTTYPE      2304 // TODO: 「圧縮の種類」は圧縮タブに移動させる

/* ------------------------------------------------------------------------- */
//  出力先グループ
/* ------------------------------------------------------------------------- */
#define IDC_OUTPUT                  3000
#define IDC_OUTPUT_SPECIFIC         3001 // 指定したフォルダ
#define IDC_OUTPUT_SOURCE           3002 // 元のファイルと同じフォルダ
#define IDC_OUTPUT_RUNTIME          3003 // 実行時に指定する
#define IDC_OUTPUT_PATH             3004
#define IDC_OUTPUT_PATH_BUTTON      3005

/* ------------------------------------------------------------------------- */
//  詳細グループ
/* ------------------------------------------------------------------------- */
#define IDC_DETAIL                  3100
#define IDC_DETAIL_CONFIRM          3101 // 上書きの確認を行う
#define IDC_DETAIL_CONFIRMOLDER     3102 // 更新日時が新しい場合は確認しない
#define IDC_DETAIL_CREATEFOLDER     3103 // フォルダを自動生成する
#define IDC_DETAIL_SKIPSINGLEFOLDER 3109 // 単一フォルダの場合は生成しない
#define IDC_DETAIL_SKIPSINGLEFILE   3118 // 単一ファイルの場合は生成しない
#define IDC_DETAIL_ENCODE           3104 // ファイル名の文字コードを変換する
#define IDC_DETAIL_FILTERING        3105 // ファイルのフィルタンリングを行う
#define IDC_DETAIL_OPEN             3106 // 保存先フォルダを開く
#define IDC_DETAIL_SKIPDESKTOP      3107 // デスクトップの場合は開かない
#define IDC_DETAIL_DELETEONMAIL     3110 // メール添付の際にファイルを残さない
#define IDC_DETAIL_TOOLTIP          3111 // ツールチップにファイル一覧を表示する
#define IDC_DETAIL_TOOLTIPCOUNT_T   3112
#define IDC_DETAIL_TOOLTIPCOUNT     3113 // ツールチップの表示する最大件数
#define IDC_DETAIL_DELETEONEXTRACT  3115 // 元ファイルを削除する

/* ------------------------------------------------------------------------- */
//  「フィルタリング」タブ
/* ------------------------------------------------------------------------- */
#define IDC_FILTERING               4001

/* ------------------------------------------------------------------------- */
//  「バージョン情報」タブ
/* ------------------------------------------------------------------------- */
#define IDC_VERSION_LOGO            5000
#define IDC_VERSION_NAME            5001
#define IDC_VERSION_VERSION         5002
#define IDC_VERSION_COMPANY         5003
#define IDC_VERSION_URL             5004
#define IDC_VERSION_UPDATE          5005
#define IDC_VERSION_ERRORREPORT     5006
#define IDC_VERSION_DEBUG           5007

/* ------------------------------------------------------------------------- */
//  コンテキストメニューのカスタマイズ用ダイアログ
/* ------------------------------------------------------------------------- */
#define IDC_CURRENT_LABEL           6001
#define IDC_ADD_LABEL               6002
#define IDC_CURRENT_TREEVIEW        6003
#define IDC_ADD_TREEVIEW            6004
#define IDC_ADD_BUTTON              6005
#define IDC_DELETE_BUTTON           6006
#define IDC_UP_BUTTON               6007
#define IDC_DOWN_BUTTON             6008
#define IDC_FOLDER_BUTTON           6009
#define IDC_RENAME_BUTTON           6010
#define IDC_RESET_BUTTON            6011

#define IDM_UP_MENU                 6107
#define IDM_DOWN_MENU               6108
#define IDM_FOLDER_MENU             6109
#define IDM_RENAME_MENU             6110
#define IDM_RESET_MENU              6111
#define IDM_DELETE_MENU             6106
#define IDM_ADD_MENU                6105

#endif // CUBEICE_SETTING_RESOURCE_H

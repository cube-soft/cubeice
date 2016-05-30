// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/fileinfo.h
 *
 *  Copyright (c) 2010 CubeSoft, Inc.
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
#ifndef CUBEICE_FILEINFO_H
#define CUBEICE_FILEINFO_H

#include <cubeice/config.h>
#include <psdotnet/date-time.h>
#include <psdotnet/io/filesystem-info.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// FileInfo
	///
	/// <summary>
	/// 圧縮ファイル中のファイル、またはディレクトリの情報を表すための
	/// クラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class FileInfo {
	public:
		typedef CubeICE::char_type char_type;
		typedef CubeICE::string_type string_type;
		typedef __int64 size_type;
		typedef PsdotNet::DateTime DateTime;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		FileInfo() :
			name_(), attr_(0), size_(0), write_() {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~FileInfo() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Name
		///
		/// <summary>
		/// ファイル名を取得、または設定します。圧縮ファイル中のファイルは、
		/// ディレクトリに格納されている等の関係で構造化されている場合が
		/// あります。その場合は、ルートディレクトリからの相対パスを表します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& Name() const { return name_; }
		void Name(const string_type& name) { name_ = name; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Attributes
		///
		/// <summary>
		/// 現在のファイルの FileAttributes を取得または設定します。
		/// NOTE: 現時点で CubeICE が使用している属性は Directory のみです。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Attributes(const size_type& attr) { attr_ = attr; }
		size_type& Attributes() { return attr_; }
		const size_type& Attributes() const { return attr_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Attributes
		///
		/// <summary>
		/// 現在のファイルのファイルサイズを取得、または設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const size_type& Size() const { return size_; }
		void Size(size_type size) { size_ = size; }
		
		/* ----------------------------------------------------------------- */
		///
		/// LastWriteTime
		///
		/// <summary>
		/// 現在のファイルまたはディレクトリに最後に書き込みが行われた
		/// 時刻を取得、または設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const DateTime& LastWriteTime() const { return write_; }
		void LastWriteTime(const DateTime& write) { write_ = write; }
		
	private:
		string_type name_;
		size_type attr_;
		size_type size_;
		DateTime write_;
	}; // class FileInfo
} // namespace CubeICE

#endif // CUBEICE_FILEINFO_H

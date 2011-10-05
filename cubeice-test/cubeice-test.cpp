/* ------------------------------------------------------------------------- */
/*
 *  cubeice-test.cpp
 *
 *  Copyright (c) 2010 - 2011 CubeSoft Inc.
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
#include "test-io.h"
#include "test-format.h"
#include "test-compressor.h"

int test_main(int argc, char* argv[]) {
	test_directory();
	test_tmpfile();
	test_tmpdir();
	test_punct();
	test_compressor_extension();
	
	return 0;
}

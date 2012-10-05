// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-test.cpp
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
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "cubeice-test.h"

/* ------------------------------------------------------------------------- */
/// CubeICETestConfigure
/* ------------------------------------------------------------------------- */
class CubeICETestConfigure {
public:
	/* --------------------------------------------------------------------- */
	/// constructor
	/* --------------------------------------------------------------------- */
	CubeICETestConfigure() {
		TestMessageAppender mapp;
		PsdotNet::Logger::Configure(mapp, PsdotNet::LogLevel::Trace);
	}

	/* --------------------------------------------------------------------- */
	/// destructor
	/* --------------------------------------------------------------------- */
	~CubeICETestConfigure() {}
};

BOOST_GLOBAL_FIXTURE(CubeICETestConfigure);
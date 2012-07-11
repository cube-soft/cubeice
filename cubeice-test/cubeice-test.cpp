// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-test.cpp
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
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
// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  user-setting-saver.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_TEST_USER_SETTING_SAVER_H
#define CUBEICE_TEST_USER_SETTING_SAVER_H

#include <cubeice/config.h>
#include <cubeice/user-setting.h>

/* ------------------------------------------------------------------------- */
//  UserSettingSaver
/* ------------------------------------------------------------------------- */
class UserSettingSaver {
public:
	UserSettingSaver() : setting_() { setting_.Load(); }
	~UserSettingSaver() { setting_.Save(); }

private:
	CubeICE::UserSetting setting_;
};

#endif // CUBEICE_TEST_USER_SETTING_SAVER_H
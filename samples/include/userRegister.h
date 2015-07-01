///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: userRegister.h
//
// Description:
//
// Created: 2015年05月28日 星期四 09时30分53秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __USERREGISTER_H__
#define __USERREGISTER_H__

#include "httpClient.h"

class UserRegister :public HttpClient{
	public:
		UserRegister();
		~UserRegister();

	public:
		virtual int user_register(const std::string &token, const std::string &strUrl, std::string &strResponse);

};

#endif

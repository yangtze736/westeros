///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: validate.h
//
// Description:
//
// Created: 2015年06月25日 星期四 15时52分49秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __VALIDATE_H__
#define __VALIDATE_H__

#include "httpClient.h"

class Validate :public HttpClient{
	public:
		Validate();
		~Validate();

	public:
		virtual int validate(const std::string &strUrl, const std::string &postField, std::string &strResponse);

};

#endif

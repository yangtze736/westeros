///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: makeDirs.h
//
// Description:
//
// Created: 2015年05月05日 星期二 09时47分42秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __MAKEDIRS_H__
#define __MAKEDIRS_H__

#include "httpClient.h"

class MakeDirs :public HttpClient{
	public:
		MakeDirs();
		~MakeDirs();

	public:
		virtual int make_directory(const std::string &token, const std::string &strUrl, std::string &strResponse);


};

#endif

///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: ../include/createContainer.h
//
// Description:
//
// Created: 2015年06月25日 星期四 17时53分51秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __CREATECONTAINER_H__
#define __CREATECONTAINER_H__

#include "httpClient.h"

class CreateContainer :public HttpClient{
	public:
		CreateContainer();
		~CreateContainer();

	public:
		virtual int create_container(const std::string &token, const std::string &strUrl, std::string &strResponse);

};

#endif

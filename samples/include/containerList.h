///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: containerList.h
//
// Description:
//
// Created: 2015年06月25日 星期四 17时03分21秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __CONTAINERLIST_H__
#define __CONTAINERLIST_H__

#include "httpClient.h"

class ContainerList :public HttpClient{
	public:
		ContainerList();
		~ContainerList();

	public:
		virtual int container_list(const std::string &token, const std::string &strUrl, std::string &strResponse);

};

#endif

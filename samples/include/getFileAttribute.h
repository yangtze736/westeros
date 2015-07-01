///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: getFileAttribute.h
//
// Description:
//
// Created: 2015年05月05日 星期二 11时43分42秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __GETFILEATTRIBUTE_H__
#define __GETFILEATTRIBUTE_H__

#include "httpClient.h"

class GetFileAttribute :public HttpClient
{
	public:
		GetFileAttribute();
		~GetFileAttribute();

	public:
		virtual int get_file_attribute(const std::string &token, const std::string &strUrl, std::string &strResponse);
};

#endif

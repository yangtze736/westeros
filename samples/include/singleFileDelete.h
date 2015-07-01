///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: singleFileDelete.h
//
// Description:
//
// Created: 2015年05月05日 星期二 10时19分33秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __SINGLEFILEDELETE_H__
#define __SINGLEFILEDELETE_H__

#include "httpClient.h"

class SingleFileDelete :public HttpClient{
	public:
		SingleFileDelete();
		~SingleFileDelete();

	public:
		virtual int single_file_delete(const std::string &token, const std::string &strUrl, std::string &strResponse);

};

#endif

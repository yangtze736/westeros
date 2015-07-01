///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: fileOpen.h
//
// Description:
//
// Created: 2015年06月02日 星期二 16时47分08秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __FILEOPEN_H__
#define __FILEOPEN_H__

#include "httpClient.h"

class FileOpen :public HttpClient{
	public:
		FileOpen();
		~FileOpen();

	public:
		virtual int file_open(const std::string &token, const std::string &strUrl, std::string &strResponse);

};

#endif

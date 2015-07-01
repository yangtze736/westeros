///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: fileCopy.h
//
// Description:
//
// Created: 2015年05月05日 星期二 13时23分12秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __FILECOPY_H__
#define __FILECOPY_H__

#include "httpClient.h"

class FileCopy :public HttpClient{
	public:
		FileCopy();
		~FileCopy();

	public:
		virtual int file_copy(const std::string &strUrl, const std::string &strFilename, std::string &strResponse);

};

#endif

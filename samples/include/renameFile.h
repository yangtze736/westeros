///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: renameFile.h
//
// Description:
//
// Created: 2015年05月05日 星期二 11时21分38秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __RENAMEFILE_H__
#define __RENAMEFILE_H__

#include "httpClient.h"

class RenameFile :public HttpClient{
	public:
		RenameFile();
		~RenameFile();

	public:
		virtual int rename_file(const std::string &token, const std::string &strUrl, std::string &strResponse);

};

#endif

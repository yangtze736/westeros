///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: fileMoveTo.h
//
// Description:
//
// Created: 2015年05月05日 星期二 13时14分30秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __FILEMOVETO_H__
#define __FILEMOVETO_H__

#include "httpClient.h"

class FileMoveTo :public HttpClient{
	public:
		FileMoveTo();
		~FileMoveTo();
	
	public:
		virtual int file_move_to(const std::string &strUrl,const std::string &strFilename,std::string &strResponse);

};

#endif

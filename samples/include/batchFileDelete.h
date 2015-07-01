///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: batchFileDelete.h
//
// Description:
//
// Created: 2015年05月05日 星期二 11时12分36秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __BATCHFILEDELETE_H__
#define __BATCHFILEDELETE_H__

#include "httpClient.h"

class BatchFileDelete :public HttpClient{
	public:
		BatchFileDelete();
		~BatchFileDelete();

	public:
		virtual int batch_file_delete(const std::string &token, const std::string &postField, const std::string &strUrl, std::string &strResponse);

};

#endif

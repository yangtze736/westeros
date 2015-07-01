///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: storageList.h
//
// Description:
//
// Created: 2015年05月15日 星期五 16时31分06秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __STORAGELIST_H__
#define __STORAGELIST_H__

#include "httpClient.h"

class StorageList :public HttpClient{
	public:
		StorageList();
		~StorageList();

	public:
		virtual int storage_list(const std::string &token, const std::string &strUrl, std::string &strResponse);

};

#endif

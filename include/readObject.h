///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: readObject.h
//
// Description:
//
// Created: 2015年06月26日 星期五 13时51分14秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __READOBJECT_H__
#define __READOBJECT_H__

#include "CppSQLite3.h"
#include "httpClient.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>

#define STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES         1024*1024*1024
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     1

class ReadObject :public HttpClient{

	struct myprogress {
		double lastruntime;
		CURL *curl;
		CppSQLite3DB *db;
		pthread_spinlock_t lock;
		std::string uuid;
	};

	public:
		ReadObject();
		~ReadObject();
	
	protected:
		static int xferinfo(void *p,
				curl_off_t dltotal, curl_off_t dlnow,
				curl_off_t ultotal, curl_off_t ulnow);
		static int older_progress(void *p,
				double dltotal, double dlnow,
				double ultotal, double ulnow);

		void initDB(void);

	public:
		virtual int read_object(const std::string &strFilename, const std::string &uuid, const std::string &user, const std::string &token, const std::string &strUrl, std::string &strResponse);

	private:
		CppSQLite3DB *m_db;
		pthread_spinlock_t m_lock;

};

#endif

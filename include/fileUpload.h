///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: fileUpload.h
//
// Description:
//
// Created: 2015年05月04日 星期一 09时37分58秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __FILEUPLOAD_H__
#define __FILEUPLOAD_H__

#include "CppSQLite3.h"
#include "httpClient.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>

#define STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES         1024*1024*1024
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     1

class FileUpload :public HttpClient{

	struct myprogress {
		double lastruntime;
		CURL *curl;
		CppSQLite3DB *db;
		pthread_spinlock_t lock;
		std::string uuid;
	};

	public:
		FileUpload();
		~FileUpload();
	
	protected:
		static int xferinfo(void *p,
				curl_off_t dltotal, curl_off_t dlnow,
				curl_off_t ultotal, curl_off_t ulnow);
		static int older_progress(void *p,
				double dltotal, double dlnow,
				double ultotal, double ulnow);

		void initDB(void);
		bool handle_clean(const std::string &strFilename, int flag);
		int  handle_mode(const std::string &strFilename, const std::string &token, int flag, struct stat &file_info, FILE *fp);

	public:
		virtual int file_upload(const std::string &strFilename, const std::string &uuid, const std::string &token, const std::string &strUrl, std::string &strResponse, int flag);

	private:
		//SqliteWrap *m_sqliteWrap;
		CppSQLite3DB *m_db;
		pthread_spinlock_t m_lock;
};

#endif

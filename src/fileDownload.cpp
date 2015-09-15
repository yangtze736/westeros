///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: fileDownload.cpp
//
// Description:
//
// Created: 2015年05月04日 星期一 13时56分25秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "middleware.h"
#include "static.h"
#include "fileDownload.h"
#include "mfcFile.h"
#include "compress.h"
#include "trap.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>



FileDownload::FileDownload()
{
	//pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
	m_lock = MiddleWare::getInstance()->m_lock;
	initDB();
}

FileDownload::~FileDownload()
{
	//pthread_spin_destroy(&m_lock);
}

void FileDownload::initDB(void)
{
	//m_db = new CppSQLite3DB;
	m_db = MiddleWare::getInstance()->m_db;
	//m_db->open(DEFAULT_DB_FILE);
	if(!m_db->tableExists("download"))
	{
		pthread_spin_lock(&m_lock);
		try{
			m_db->execDML("create table download(id char(64) primary key, user char(32), file char(128), total int, now int);");
		}
		catch(CppSQLite3Exception &e){
			fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		}
		pthread_spin_unlock(&m_lock);
	}
}

int FileDownload::xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	struct myprogress *myp = (struct myprogress *)p;
	double curtime = 0;

	curl_easy_getinfo(myp->curl, CURLINFO_TOTAL_TIME, &curtime);

	if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL)
	{
		myp->lastruntime = curtime;
		char buf[256] = {0};
		sprintf(buf, "update download set now = %ld, total= %ld where id = '%s';", (long int)dlnow,(long int)dltotal,myp->uuid.c_str());

		pthread_spin_lock(&myp->lock);
		try{
			myp->db->execDML(buf);
		}
		catch(CppSQLite3Exception &e){
			fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		}
		pthread_spin_unlock(&myp->lock);

		PR("%s", buf);
		PR("UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
				"  DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
				"\r\n",
				ulnow, ultotal, dlnow, dltotal);
	}

	// don't limit file size.
	if(dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
		return 0;

	return 0;
}

/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
int FileDownload::older_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow)
{
	return xferinfo(p, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal, (curl_off_t)ulnow);
}

int FileDownload::handle_mode(const std::string &strFilename, const std::string &token, int flag)
{
	// flag == 1    NORMAL        flag == 2    ENCRYPT
	// flag == 3    COMPRESS      flag == 4    ENCRYPT_COMPRESS
	if(flag == 1)
	{
		PR("fileDownload mode = NORMAL.");
		return 0;
	}
	else if(flag == 2)
	{
		PR("fileDownload mode = ENCRYPT.");
		std::string decryptFilename = MfcFile::createTmpFile(strFilename);
		if(!decrypt_filename(token, strFilename, decryptFilename))
		{
			fprintf(stderr, "decrypt src file failed.\n");
			return -1;
		}
		MfcFile::delFile(strFilename);
		MfcFile::reName(decryptFilename, strFilename);
	}
	else if(flag == 3)
	{
		PR("fileDownload mode = COMPRESS.");
		if(MfcFile::getFileSize(strFilename) < 300*1024*1024){
			UncompressFile(strFilename.c_str());
			std::string strUncompressFile = strFilename +".uncomp";
			MfcFile::moveFile(strUncompressFile, strFilename);
		}
		else{
			//TODO
		}
	}
	else if(flag == 4)
	{
		PR("fileDownload begin, mode = ENCRYPT_COMPRESS.");
		if(MfcFile::getFileSize(strFilename) < 300*1024*1024){
			UncompressFile(strFilename.c_str());
			std::string strUncompressFile = strFilename +".uncomp";
			MfcFile::moveFile(strUncompressFile, strFilename);
			std::string decryptFilename = MfcFile::createTmpFile(strFilename);
			if(!decrypt_filename(token, strFilename, decryptFilename))
			{
				fprintf(stderr, "decrypt src file failed.\n");
				return -1;
			}
			MfcFile::delFile(strFilename);
			MfcFile::reName(decryptFilename, strFilename);
		}
		else{
			//TODO
		}
	}
	else
	{
		fprintf(stderr, "fileUpload begin, check mode error!\n");
		return -1;
	}
	return 0;
}

int FileDownload::file_download(const std::string &strFilename, const std::string &uuid, const std::string &user, const std::string &token, const std::string &strUrl, std::string &strResponse, int flag)
{
	// insert data
	char buf[1024] = {0};
	sprintf(buf, "insert into download values ('%s', '%s', '%s', 0, 0);",uuid.c_str(),user.c_str(),strFilename.c_str());
	pthread_spin_lock(&m_lock);
	try{
		m_db->execDML(buf);
	}
	catch(CppSQLite3Exception &e){
		fprintf(stderr, "ErrorNo: %d , %s.\n", e.errorCode(), e.errorMessage());
	}
	pthread_spin_unlock(&m_lock);

	CURLcode res;  
	CURL* curl = curl_easy_init();  
	if(NULL == curl)  {  
		return CURLE_FAILED_INIT;  
	}

	struct myprogress prog;
	prog.lastruntime = 0;prog.curl = curl;prog.db = m_db;
	prog.lock = m_lock;prog.uuid = uuid;

	if(m_bDebug){  
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
	}

	struct curl_slist *slist = 0;
	char tokenBuf[128] = {0};
	sprintf(tokenBuf, "X-Auth-Token:%s", token.c_str());
	slist = curl_slist_append(slist,tokenBuf);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	//
	FILE *fp;
	if((fp = fopen(strFilename.c_str(),"wb")) == NULL){
		fprintf(stderr, "file open error, file:[%s]\n",strFilename.c_str());
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);  
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);  
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 300);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	// start of progress
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);
#if LIBCURL_VERSION_NUM >= 0x072000
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
#endif
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	// end of progress
	PR("***************************");
	res = curl_easy_perform(curl);
	PR("***************************");

	// clean up
	fclose(fp);
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);  

	chmod(strFilename.c_str(), 0666);

	// judge mode, NORMAL|ENCRYPT|COMPRESS|ENCRYPT_COMPRESS
	if(handle_mode(strFilename, token, flag) < 0){
		strResponse = "{\"status\":15003,\"errMsg\":\"handle fileDownload mode error\"}";
		return -1;
	}

	// capture filedownload result.
	if(res == CURLE_OK){
		char buff[256] = {0};
		long int total = (long int)MfcFile::getFileSize(strFilename);
		long int now = total;
		sprintf(buf, "update download set now = %ld, total= %ld where id = '%s';", now, total, uuid.c_str());
		pthread_spin_lock(&m_lock);
		try{
			m_db->execDML(buf);
		}catch(CppSQLite3Exception &e){
			fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		}
		pthread_spin_unlock(&m_lock);
	}

	return res;  
}

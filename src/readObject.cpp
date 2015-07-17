///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: readObject.cpp
//
// Description:
//
// Created: 2015年06月26日 星期五 13时49分12秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "static.h"
#include "readObject.h"
#include "mfcFile.h"
#include <stdlib.h>

ReadObject::ReadObject()
{
	pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
	initDB();
}

ReadObject::~ReadObject()
{
	pthread_spin_destroy(&m_lock);
}

void ReadObject::initDB(void)
{
	m_db = new CppSQLite3DB;
	m_db->open("./test.db");
	if(!m_db->tableExists("download"))
	{
		pthread_spin_lock(&m_lock);
		try{
			m_db->execDML("create table download(id char(64) primary key, total int, now int);");
		}
		catch(CppSQLite3Exception &e){
			fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		}
		pthread_spin_unlock(&m_lock);
	}
}

int ReadObject::xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
  struct myprogress *myp = (struct myprogress *)p;
  double curtime = 0;

  curl_easy_getinfo(myp->curl, CURLINFO_TOTAL_TIME, &curtime);

  if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL)
  {
    myp->lastruntime = curtime;
	char buf[128] = {0};
	sprintf(buf, "update download set now = %ld, total= %ld where id = '%s';", dlnow,dltotal,myp->uuid.c_str());

	pthread_spin_lock(&myp->lock);
	try{
		myp->db->execDML(buf);
	}
	catch(CppSQLite3Exception &e){
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
	}
	pthread_spin_unlock(&myp->lock);

	fprintf(stdout, "%s", buf);
	fprintf(stdout, "UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
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
int ReadObject::older_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow)
{
  return xferinfo(p, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal, (curl_off_t)ulnow);
}

int ReadObject::read_object(const std::string &strFilename, const std::string &uuid, const std::string &token, const std::string &strUrl, std::string &strResponse)
{
	// insert data
	char buf[128] = {0};
	sprintf(buf, "insert into download values ('%s', 0, 0);",uuid.c_str());
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
	if(NULL == curl)  
	{  
		return CURLE_FAILED_INIT;  
	}

	struct myprogress prog;
	prog.lastruntime = 0;
	prog.curl = curl;
	prog.db = m_db;
	prog.lock = m_lock;
	prog.uuid = uuid;

	if(m_bDebug)  
	{  
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
	if((fp = fopen(strFilename.c_str(),"wb")) == NULL)
	{
		printf("file open error, file:[%s]\n",strFilename.c_str());
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
	printf("***************************\n");
	res = curl_easy_perform(curl);
	printf("***************************\n");
	// clean up
	fclose(fp);
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);  


	// decrypt file
	std::string decryptFilename = MfcFile::createTmpFile(strFilename);
	if(!decrypt_filename(token, strFilename, decryptFilename))
	{
		printf("decrypt src file failed.\n");
		return -1;
	}
	//
	MfcFile::delFile(strFilename);
	MfcFile::reName(decryptFilename, strFilename);


	return res;  
}

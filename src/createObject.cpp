///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: createObject.cpp
//
// Description:
//
// Created: 2015年06月26日 星期五 11时27分30秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "middleware.h"
#include "static.h"
#include "createObject.h"
#include "mfcFile.h"
#include "trap.h"
#include <sys/stat.h>
#include <stdlib.h>

CreateObject::CreateObject()
{
	m_lock = MiddleWare::getInstance()->m_lock;
	initDB();
}

CreateObject::~CreateObject()
{
}

void CreateObject::initDB(void)
{
	m_db = MiddleWare::getInstance()->m_db;
	if(!m_db->tableExists("upload"))
	{
		pthread_spin_lock(&m_lock);
		try{
			m_db->execDML("create table upload(id char(64) primary key, user char(32), file char(128), total int, now int);");
		}
		catch(CppSQLite3Exception &e){
			fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		}
		pthread_spin_unlock(&m_lock);
	}
}

int CreateObject::xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
  struct myprogress *myp = (struct myprogress *)p;
  double curtime = 0;

  curl_easy_getinfo(myp->curl, CURLINFO_TOTAL_TIME, &curtime);

  if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL)
  {
    myp->lastruntime = curtime;
	char buf[256] = {0};
	sprintf(buf, "update upload set now = %ld, total= %ld where id = '%s';", (long int)ulnow,(long int)ultotal,myp->uuid.c_str());

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
int CreateObject::older_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow)
{
  return xferinfo(p, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal, (curl_off_t)ulnow);
}

int CreateObject::create_object(const std::string &strFilename, const std::string &uuid, const std::string &user, const std::string &token, const std::string &strUrl, std::string &strResponse)
{
	// insert data
	char buf[256] = {0};
	sprintf(buf, "insert into upload values ('%s', '%s', '%s', 0, 0);",uuid.c_str(),user.c_str(),strFilename.c_str());
	pthread_spin_lock(&m_lock);
	try{
		m_db->execDML(buf);
	}
	catch(CppSQLite3Exception &e){
		fprintf(stderr, "ErrorNo: %d , %s.\n", e.errorCode(), e.errorMessage());
	}
	pthread_spin_unlock(&m_lock);

	CURLcode res;
	CURL *curl = curl_easy_init();
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
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	struct curl_slist *slist = 0;
	char tokenBuf[128] = {0};
	sprintf(tokenBuf, "X-Auth-Token:%s", token.c_str());
	slist = curl_slist_append(slist,tokenBuf);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	// create linux file start with dot
	std::string encryptFilename = MfcFile::createTmpFile(strFilename);
	if(!encrypt_filename(token, strFilename, encryptFilename))
	{
		fprintf(stderr, "encrypt src file failed.\n");
		return -1;
	}

	struct stat file_info;
	FILE *fp;
	if((fp = fopen(encryptFilename.c_str(),"rb")) == NULL)
	{
		fprintf(stderr, "file open error. file:[%s]\n", strFilename.c_str());
		return -1;
	}
	if(fstat(fileno(fp), &file_info) != 0)
	{
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadData);
	curl_easy_setopt(curl, CURLOPT_READDATA, fp);
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
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

	// capture fileupload result.
	if(res == CURLE_OK){
		char buff[256] = {0};
		int total = (int)file_info.st_size;
		int now = total;
		sprintf(buf, "update upload set now = %ld, total= %ld where id = '%s';", (long int)now, (long int)total, uuid.c_str());
		pthread_spin_lock(&m_lock);
		try{
			m_db->execDML(buf);
		}catch(CppSQLite3Exception &e){
			fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		}
		pthread_spin_unlock(&m_lock);
	}

	// remove tmp file
	fclose(fp);
	MfcFile::delFile(encryptFilename);

	// clean up
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);

	return res;
}

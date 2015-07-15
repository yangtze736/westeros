///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: static.h
//
// Description:
//
// Created: 2015年05月04日 星期一 09时12分14秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#ifndef __STATIC_H__
#define __STATIC_H__

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>
#include <string>

// libcurl use it to read the data from file
static size_t ReadData(void* buff, size_t size, size_t nmemb, void* userp)
{
	return fread(buff, size, nmemb, (FILE *)userp);
}
#if 0
static size_t ReadCompData(void* buff, size_t size, size_t nmemb, void* userp)
{
	char *buf = (char*)malloc(sizeof(char) * size * nmemb);

	size_t ret = fread(buf, size, nmemb, (FILE *)userp);
	if(ret == 0 && ferror(fp)){
		perror("fread");
		exit(1);
	}
	std::string data;
	data.append(buf, size * nmemb);

	free(buf);
	//TODO
}
#endif
// libcurl use it to write the received data into file
static size_t WriteData(void* buff, size_t size, size_t nmemb, void* userp)
{
	return fwrite(buff, size, nmemb, (FILE *)userp);
}

// libcurl use it to write the received data
static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		return -1;
	}

	char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if(itype == CURLINFO_TEXT)
	{
		//printf("[TEXT]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_IN)
	{
		printf("[HEADER_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_OUT)
	{
		printf("[HEADER_OUT]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_IN)
	{
		printf("[DATA_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_OUT)
	{
		printf("[DATA_OUT]%s\n", pData);
	}
	return 0;
}

#endif

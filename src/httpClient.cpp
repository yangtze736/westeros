///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: httpClient.cpp
//
// Description:
//
// Created: 2015年04月30日 星期四 09时34分37秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "static.h"
#include "httpClient.h"
#include "trap.h"
#include <fcntl.h>
#include <unistd.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <strings.h>

HttpClient::HttpClient():m_bDebug(false)
{
}

HttpClient::~HttpClient()
{
}

void HttpClient::setDebug(bool bDebug)
{
	m_bDebug = bDebug;
}

int HttpClient::put(const std::string &token, const std::string &strUrl, std::string &strResponse, std::string dest, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	struct curl_slist *slist = 0;
	char tokenBuf[128] = {0};
	sprintf(tokenBuf, "X-Auth-Token:%s", token.c_str());
	slist = curl_slist_append(slist,tokenBuf);
	if(!dest.empty()){
		char destBuf[512] = {0};
		sprintf(destBuf, "Destination:%s",dest.c_str());
		slist = curl_slist_append(slist,destBuf);
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	//curl_easy_setopt(curl, CURLOPT_PUT, 1L);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}
	res = curl_easy_perform(curl);
	// clean up
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);

	// keep response length, fix bug(buffer overflow detected)
	char resp[512] = {0};
	snprintf(resp, 511, "%s", strResponse.c_str());
	strResponse.clear();
	strResponse = std::string(resp);

	return res;
}

int HttpClient::postss(const std::string &token, const std::string &meta, const std::string &strUrl, std::string &strResponse, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	struct curl_slist *slist = 0;
	char tokenBuf[128] = {0};
	sprintf(tokenBuf, "X-Auth-Token:%s", token.c_str());
	slist = curl_slist_append(slist,tokenBuf);
	if(!meta.empty()){
		char metaBuf[128] = {0};
		sprintf(metaBuf, "X-Account-Meta-Quota-Bytes:%s", meta.c_str());
		slist = curl_slist_append(slist,metaBuf);
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(curl);
	// clean up
	curl_easy_cleanup(curl);

	return res;
}

int HttpClient::_posts(const std::string &token, const std::string &strUrl, const std::string &postField, std::string &strResponse, std::string &flag, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	struct curl_slist *slist = 0;
	char tokenBuf[128] = {0};
	sprintf(tokenBuf, "X-Auth-Token:%s", token.c_str());
	slist = curl_slist_append(slist,tokenBuf);
	if(0 == strcasecmp(flag.c_str(), "true")){
		slist = curl_slist_append(slist,"X-Static-Large-Object: True");
		PR("X-Static-Large-Object: True");
	}
	else{
		slist = curl_slist_append(slist,"X-Static-Large-Object: false");
		PR("X-Static-Large-Object: false");
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postField.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(curl);
	// clean up
	curl_easy_cleanup(curl);

	return res;
}

int HttpClient::posts(const std::string &token, const std::string &strUrl, const std::string &postField, std::string &strResponse, bool flag, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	struct curl_slist *slist = 0;
	char tokenBuf[128] = {0};
	sprintf(tokenBuf, "X-Auth-Token:%s", token.c_str());
	slist = curl_slist_append(slist,tokenBuf);
	if(flag){
		slist = curl_slist_append(slist,"X-Static-Large-Object: True");
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postField.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(curl);
	// clean up
	curl_easy_cleanup(curl);

	return res;
}

int HttpClient::post(const std::string &strUrl, const std::string &postField, std::string &strResponse, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postField.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(curl);
	// clean up
	curl_easy_cleanup(curl);

	return res;
}

int HttpClient::get(const std::string &token, const std::string &strUrl, std::string &strResponse, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

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

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(curl);
	// clean up
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);

	return res;
}

int HttpClient::del(const std::string &token, const std::string &strUrl, std::string &strResponse, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

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

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(curl);
	// clean up
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);

	return res;
}

int HttpClient::copy(const std::string &token, const std::string &dest, const std::string &strUrl, std::string &strResponse, const char *pCaPath)
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	struct curl_slist *slist = 0;
	char tokenBuf[128] = {0};
	sprintf(tokenBuf, "X-Auth-Token:%s", token.c_str());
	slist = curl_slist_append(slist,tokenBuf);
	if(!dest.empty()){
		char destBuf[512] = {0};
		sprintf(destBuf, "Destination:%s",dest.c_str());
		slist = curl_slist_append(slist,destBuf);
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "COPY");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	if(NULL == pCaPath){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(curl);
	// clean up
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);

	return res;
}

#if 0
bool HttpClient::encrypt_filename(const std::string &token, const std::string &srcStr, const std::string &dstStr)
{
	/****** To Get User Access Token ******/
	//std::string pResult = m_strResult;
	char tokenJson[256] = {0};                                                                                   
	sprintf(tokenJson, "{\"access_token\":\"%s\"}", token.c_str());
	std::string pResult(tokenJson);
	PR("Access_token : [%s]", pResult.c_str());


	CSecrypto* sc = NULL;
	try{
		sc = new CSecrypto();
	}catch(...){
		fprintf(stderr, "catch except when new CSecrypto\n");
		return false;
	}
	// add openssl conf
	CSecrypto::load_padlock();

	/******  Encrypt The File ******/
	int srcFile, dstFile;
	int bytes_read,bytes_write;

	unsigned char buffer[BLOCK_SIZE] = {0};
	unsigned char buffers[BLOCK_SIZE] = {0};

	if((srcFile = open(srcStr.c_str(), O_RDONLY)) == -1)
	{
		fprintf(stderr, "Open %s Error:%s\n", srcStr.c_str(), strerror(errno));
		return false;
	}

	if((dstFile = open(dstStr.c_str(), O_RDONLY|O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)) == -1)
	{
		fprintf(stderr, "Open %s Error:%s\n", dstStr.c_str(), strerror(errno));
		return false;
	}


	while(bytes_read = read(srcFile, buffer, BLOCK_SIZE))
	{   
		if((bytes_read == -1) && (errno != EINTR))
		{
			break;
		}
		else if(bytes_read > 0)
		{
			int padding = 0;
			if (bytes_read % BLOCK_SIZE > 0)
			{
				padding =  BLOCK_SIZE - bytes_read % BLOCK_SIZE;
			}
			bytes_read += padding;
			while (padding > 0)
			{
				buffer[BLOCK_SIZE-padding] = '\001';
				padding--;
			}

			sc->sAES_encrypt((const unsigned char*)buffer, buffers, pResult);
			bytes_write = write(dstFile, buffers, bytes_read);

			if(bytes_write == -1) 
				break;
		}
	}   

	close(srcFile);
	close(dstFile);

	sc->secrypto_destroy();
	delete sc, sc = NULL;

	return true;
}

bool HttpClient::decrypt_filename(const std::string &token, const std::string &srcStr, const std::string &dstStr)
{
	/****** To Get User Access Token ******/
	//std::string pResult = m_strResult;
	char tokenJson[256] = {0};                                                                                   
	sprintf(tokenJson, "{\"access_token\":\"%s\"}", token.c_str());
	std::string pResult(tokenJson);
	PR("Access_token : [%s]", pResult.c_str());

	CSecrypto* sc = NULL;
	try{
		sc = new CSecrypto();
	}catch(...){
		fprintf(stderr, "catch except when new CSecrypto\n");
		return false;
	}
	// add openssl conf
	CSecrypto::load_padlock();

	/******  Decrypt The File ******/
	int srcFile, dstFile;
	int bytes_read, bytes_write;

	unsigned char buffer[BLOCK_SIZE] = {0};
	unsigned char buffers[BLOCK_SIZE] = {0};

	if((srcFile = open(srcStr.c_str(), O_RDONLY, S_IRUSR|S_IWUSR)) == -1)
	{
		fprintf(stderr, "Open %s Error:%s\n", srcStr.c_str(), strerror(errno));
		exit(1);
	}

	if((dstFile = open(dstStr.c_str(), O_RDONLY|O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)) == -1)
	{
		fprintf(stderr, "Open %s Error:%s\n", dstStr.c_str(), strerror(errno));
		return false;
	}


	unsigned long offset = lseek(srcFile, 0, SEEK_END);
	lseek(srcFile, 0, SEEK_SET);

	unsigned long blocks = offset / BLOCK_SIZE;

	while(bytes_read = read(srcFile, buffer, BLOCK_SIZE))
	{   		
		if((bytes_read == -1) && (errno != EINTR))
		{
			break;
		} 
		else if(bytes_read > 0)
		{
			sc->sAES_decrypt((const unsigned char*)buffer, buffers, pResult);
			unsigned char* ptr = &buffers[BLOCK_SIZE-1];

			if(blocks == 1){
				while(true)
				{
					if(*ptr != '\001' || bytes_read == 0)
					{
						break;
					}
					ptr--;
					bytes_read--;
				}
			}            

			bytes_write = write(dstFile, buffers, bytes_read);

			if(bytes_write == -1)
				break;
		}
		blocks--;
	}

	close(srcFile);
	close(dstFile);

	sc->secrypto_destroy();
	delete sc, sc = NULL;

	return true;
}
#endif

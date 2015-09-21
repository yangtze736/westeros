///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: httpClient.h
//
// Description:
//
// Created: 2015年04月30日 星期四 09时36分03秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <string>

class HttpClient{

	enum{
		BLOCK_SIZE = 1024
	};

	public:
	HttpClient();
	virtual ~HttpClient();

	void setDebug(bool bDebug);
	//bool encrypt_filename(const std::string &token, const std::string &srcFileStr, const std::string &dstFileStr);
	//bool decrypt_filename(const std::string &token, const std::string &srcFileStr, const std::string &dstFileStr);

	public:
	int put(const std::string &token, const std::string &strUrl, std::string &strResponse, std::string dest="", const char *pCaPath=NULL);
	int post(const std::string &strUrl, const std::string &postField, std::string &strResponse, const char *pCaPath=NULL);
	int posts(const std::string &token, const std::string &strUrl, const std::string &postField, std::string &strResponse, bool flag=false, const char *pCaPath=NULL);
	int _posts(const std::string &token, const std::string &strUrl, const std::string &postField, std::string &strResponse, std::string &flag, const char *pCaPath=NULL);
	int postss(const std::string &token, const std::string &meta, const std::string &strUrl, std::string &strResponse, const char *pCaPath=NULL);
	int get(const std::string &token, const std::string &strUrl, std::string &strResponse, const char *pCaPath=NULL);
	int del(const std::string &token, const std::string &strUrl, std::string &strResponse, const char *pCaPath=NULL);
	int copy(const std::string &token, const std::string &dest, const std::string &strUrl, std::string &strResponse, const char *pCaPath=NULL);

	public:
	virtual int create_object(const std::string &strFilename, const std::string &uuid, const std::string &user, const std::string &token, const std::string &strUrl, std::string &strResponse){};
	virtual int read_object(const std::string &strFilename, const std::string &uuid, const std::string &user, const std::string &token, const std::string &strUrl, std::string &strResponse){};
	virtual int file_upload(const std::string &strFilename, const std::string &user, const std::string &uuid, const std::string &token, const std::string &strUrl, std::string &strResponse, int flag){};
	virtual int file_download(const std::string &strFilename, const std::string &uuid, const std::string &user, const std::string &token, const std::string &strUrl, std::string &strResponse, int flag){};

	public:
	bool			m_bDebug;
	std::string		m_strResult;

};

#endif

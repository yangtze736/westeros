///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: file_delete.cpp
//
// Description:
//
// Created: 2015年05月07日 星期四 16时48分10秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include <curl/curl.h>
#include <curl/easy.h>
#include <string>

int main()
{
	CURLcode res;
	CURL *curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	
	struct curl_slist *slist = 0;
	slist = curl_slist_append(slist,"X-Auth-Token: ");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	std::string strUrl = "http://10.3.3.236:443/v1/AUTH_glfs/user/f1";
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

	// 
	res = curl_easy_perform(curl);

	// clean up
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);

	return res;
}

///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: download.cpp
//
// Description:
//
// Created: 2015年05月07日 星期四 19时25分15秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#if 0
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
#endif

static size_t OnWriteData(void *buffer, size_t size, size_t nmemb, void *user_p)
{
	FILE *fp = (FILE *)user_p;
	return fwrite(buffer, size, nmemb, fp);
}


int main()
{
	FILE *fp;
	if((fp = fopen("download.txt","wb")) == NULL)
	{
		printf("file open error.\n");
		return -1;
	}

    CURLcode res;
    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

	curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

	//register the callback function to curl for write data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  

	//set a fd to parameter CURLOPT_WRITEDATA will pass to eh writer function for handle the data
	//std::string strResponse;
    //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);  
	
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    res = curl_easy_perform(curl);


	//printf("--- [%s] ---\n",strResponse.c_str());
    curl_easy_cleanup(curl);

    return res;

}

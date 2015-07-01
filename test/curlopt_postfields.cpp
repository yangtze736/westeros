///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: curlopt_postfields.cpp
//
// Description:
//
// Created: 2015年05月06日 星期三 09时08分53秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include <curl/curl.h>
#include <curl/easy.h>

int main()
{

	CURL *curl = curl_easy_init();
	if(curl) {
		const char *data = "data to send";

		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");

		/* size of the POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 12L);

		/* pass in a pointer to the data - libcurl will not copy */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

		curl_easy_perform(curl);
	}

	/* always cleanup */
	curl_easy_cleanup(curl);

	return 0;
}

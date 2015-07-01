//////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: middleware.cpp
//
// Description:
//
// Created: 2015年04月30日 星期四 09时02分47秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "middleware.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

long long get_time()
{
	timeval tp;
	gettimeofday(&tp, 0);

	return (long)(tp.tv_sec * 1000) + (long)(tp.tv_usec / 1000);
}

int main()
{
	long long t1 = get_time();

	MiddleWare *middleware = new MiddleWare();

	std::string version, info;
	middleware->getVersionInfo(version, info);
	printf(" MiddleWare Version : %s\n MiddleWare Info : %s\n", version.c_str(), info.c_str());

	std::string upStr, downStr;
	middleware->getUploadTask(upStr);
	middleware->getDownloadTask(downStr);
	printf("upload : %s\n", upStr.c_str()); 
	printf("down ：%s\n", downStr.c_str());
	//exit(0);

	std::string method = "containerList";
	std::string jsonStr;
	std::string responseStr;
	middleware->data_pipeline(method, jsonStr, responseStr);
		
#if 0
	std::string method = "fileUpload";
	std::string jsonStr = "{\"uuid\":\"12358000\",\"X-Auth-Token\":[{\"access_token\":\"GXgybmes7m1fTd5KTCHrCkcblnJ1TccoV50o9DAz\"}],\"tenant\":\"glfs\",\"source\": [{\"file\": \"/home/bran/upload/file_100M.bin\"}], \"destination\": [{\"path\": \"/normal\", \"ftype\": \"f\"}]}";
	std::string responseStr = "?op=CREATE&overwrite=true&type=NORMAL";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif

#if 0
	std::string method2 = "fileDownLoad";
	std::string jsonStr2 = "{\"uuid\":\"12358001\",\"X-Auth-Token\":[{\"access_token\":\"GXgybmes7m1fTd5KTCHrCkcblnJ1TccoV50o9DAz\"}],\"tenant\":\"glfs\",\"source\": [{\"file\": \"/home/bran/download\"}], \"destination\": [{\"path\": \"/normal/file_100M.bin\", \"ftype\": \"f\"}]}";
	std::string responseStr22 = "?op=OPEN&type=NORMAL";
	middleware->data_pipeline(method2, jsonStr2, responseStr22);
#endif

	//delete
	delete middleware, middleware = NULL;

	// total cost
	long long t2 = get_time();
	printf(" Total cost : %lld ms.\n", t2-t1);

	return 0;
}

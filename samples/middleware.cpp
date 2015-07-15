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

#if 0
	std::string upStr, downStr;
	middleware->getUploadTask(upStr);
	middleware->getDownloadTask(downStr);
	printf("upload : %s\n", upStr.c_str()); 
	printf("down ：%s\n", downStr.c_str());
	//exit(0);
#endif

#if 1
	std::string method = "networkRegister";
	std::string jsonStr = "{\"username\":\"messi\",\"password\":\"123456\",\"password_confirmation\":\"123456\",\"email\":\"messi@126.com\",\"name\":\"zhang\",\"uuid\":\"20150713111\"}";
	std::string responseStr;
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif
#if 0
	std::string method = "fileUpload";
	//std::string jsonStr = "{\"uuid\":\"12358002221\",\"X-Auth-Token\":\"xTexuD7XN9oKJPzivigHfXB0kA6VmeVJhVUlkwDo\",\"Source\":\"/home/bran/upload/file_100M.bin\",\"Destination\":\"/normal/file_100M.bin\"}";
	std::string jsonStr = "{\"uuid\":\"12358002221\",\"X-Auth-Token\":\"xTexuD7XN9oKJPzivigHfXB0kA6VmeVJhVUlkwDo\",\"Source\":\"/home/bran/upload/swift.pdf\",\"Destination\":\"/normal/swift.pdf\"}";
	std::string responseStr = "?op=CREATE&overwrite=true&type=NORMAL&storetype=USER";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif

#if 0
	std::string method2 = "fileDownLoad";
	std::string jsonStr2 = "{\"uuid\":\"12358001\",\"X-Auth-Token\":[{\"access_token\":\"GXgybmes7m1fTd5KTCHrCkcblnJ1TccoV50o9DAz\"}],\"tenant\":\"glfs\",\"source\": [{\"file\": \"/home/bran/download\"}], \"destination\": [{\"path\": \"/normal/file_100M.bin\", \"ftype\": \"f\"}]}";
	std::string responseStr22 = "?op=OPEN&type=NORMAL";
	middleware->data_pipeline(method2, jsonStr2, responseStr22);
#endif

#if 0
	std::string method = "batchDeleteFileDir";
	std::string jsonStr = "{\"uuid\":\"122334556\",\"X-Auth-Token\":\"0a0VjsEqHNln6WmWQy0Jvksf3gcKr5IcAXfNy1H2\",\"list\":[{\"path\":\"/normal/myDir3\",\"ftype\":\"d\"},{\"path\":\"/normal/myDir2/log\",\"ftype\":\"f\"}]}";
	std::string responseStr = "?op=DELETE&type=NORMAL";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif
#if 0
	std::string method = "moveFileDir";
	std::string jsonStr = "{\"uuid\":\"12358002221\",\"X-Auth-Token\":\"0a0VjsEqHNln6WmWQy0Jvksf3gcKr5IcAXfNy1H2\",\"Source\":\"/home/bran/upload/swift.pdf\",\"Destination\":\"/normal/swift.pdf\"}";
	std::string responseStr = "?op=MOVE&ftype=f&type=NORMAL";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif
#if 0
	std::string method = "mergeFile";
	std::string jsonStr = "{\"X-Auth-Token\":\"xTexuD7XN9oKJPzivigHfXB0kA6VmeVJhVUlkwDo\",\"uuid\":\"1233455\",\"Destination\":\"/normal/seg_test\",\"Merge-Info\":[{\"path\":\"/segments/seg_test/2\",\"etag\":\"qwerty\",\"size_bytes\":1234},{\"path\":\"/segments/seg_test/1\",\"etag\":\"asdfg\",\"size_bytes\":2345}]}";
	std::string responseStr = "?multipart-manifest=put";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif

	//delete
	delete middleware, middleware = NULL;

	// total cost
	long long t2 = get_time();
	printf(" Total cost : %lld ms.\n", t2-t1);

	return 0;
}

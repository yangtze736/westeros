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
#include "trap.h"

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

/*
 * how to get access token
 * curl -k -X POST -d '{"password":"123456","email":"zhang@163.com"}' https://10.3.3.236:443/oauth/access_token
 * curl -k -X POST  https://10.3.3.236:443/oauth/token_refresh -H "X-Auth-Token:DqOs4479hs6Q2ez9EY1D3axT9diRiS3SJxaSx9lb" 
 */

long long get_time()
{
	timeval tp;
	gettimeofday(&tp, 0);

	return (long)(tp.tv_sec * 1000) + (long)(tp.tv_usec / 1000);
}

int main()
{
	//long long t1 = get_time();

	MiddleWare *middleware = new MiddleWare;

	std::string version, info;
	middleware->getVersionInfo(version, info);
	PR("MiddleWare Version : %s", version.c_str());
	PR("MiddleWare Info : %s", info.c_str());
	//exit(0);

#if 0
	std::string upStr, downStr, statusStr;
	std::string uuid = "12358002221";

	middleware->getUploadTask(upStr);
	middleware->getDownloadTask(downStr);
	middleware->checkTaskStatus(uuid, statusStr);
	PR("upload     : %s", upStr.c_str()); 
	PR("download   : %s", downStr.c_str());
	PR("uuid       : %s", uuid.c_str());
	PR("status     : %s", statusStr.c_str());
	//exit(0);
#endif

#if 0
	std::string method = "networkRegister";
	std::string jsonStr = "{\"username\":\"messi\",\"password\":\"123456\",\"password_confirmation\":\"123456\",\"email\":\"messi@126.com\",\"name\":\"zhang\",\"uuid\":\"20150713111\"}";
	std::string responseStr;
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif
#if 0
	std::string method = "fileUpload";
	//std::string jsonStr = "{\"uuid\":\"12358002221\",\"X-Auth-Token\":\"X16U1SFEW5slPM0oS7ISMUgJjpRtucA2b143hPKk\",\"Source\":\"/home/bran/upload/file_500M\",\"Destination\":\"/normal/file_500M\"}";
	std::string jsonStr = "{\"uuid\":\"12358002221\",\"X-Auth-Token\":\"Zc707lH42mowSKXEGE1IhcFmgvH9klu5rAqBrtRz\",\"Source\":\"/home/bran/upload/swift.pdf\",\"Destination\":\"/normal/swift.pdf\",\"time\":\"20150730T10:12:23\"}";
	std::string responseStr = "?op=CREATE&overwrite=true&type=NORMAL&mode=COMPRESS&storetype=USER";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif

#if 0
	std::string method2 = "readFile";
	std::string jsonStr2 = "{\"uuid\":\"12358001\",\"X-Auth-Token\":\"Zc707lH42mowSKXEGE1IhcFmgvH9klu5rAqBrtRz\",\"Source\":\"/home/bran/download/swift.pdf\",\"Destination\":\"/normal/swift.pdf\"}";
	std::string responseStr22 = "?op=OPEN&type=NORMAL&mode=COMPRESS";
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
	std::string jsonStr = "{\"uuid\":\"12358002221\",\"X-Auth-Token\":\"186oLbOtvEBv46rFAkIRhWDjGndrNkBGgIxKFAh4\",\"Source\":\"/home/bran/upload/swift.pdf\",\"Destination\":\"/normal/swift.pdf\"}";
	std::string responseStr = "?op=MOVE&ftype=f&type=NORMAL";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif
#if 1
	std::string method = "moveRecycle";
	std::string  jsonStr = "{\"X-Auth-Token\":\"wSnZdB7spdM2mDQd2JOZnjslrhIltVchEERkFbCd\",\"list\":[{\"ftype\":\"f\",\"path\":\"/new_container2/ufw.log\",\"uuid\":\"yCHNmAYC-8ts2kbYk8A\"],\"uuid\":\"6f0b34c6b947-47fc-8ce0-922d88a77b8f\"}";
	std::string responseStr = "?op=MOVERECYCLE&type=NORMAL";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif
#if 0
	std::string method = "mergeFile";
	//std::string jsonStr = "{\"X-Auth-Token\":\"UC4tgmodkQjdPeut7HnU02pujyQf9jxpb5cH9E1E\",\"uuid\":\"1233455\",\"Destination\":\"/normal/seg_test\",\"Merge-Info\":[{\"path\":\"/segments/seg_test/2\",\"etag\":\"qwerty\",\"size_bytes\":1234},{\"path\":\"/segments/seg_test/1\",\"etag\":\"asdfg\",\"size_bytes\":2345}]}";
	std::string  jsonStr = "{\"Destination\":\"/normal/seg_test\",\"Merge-Info\":[{\"etag\":\"35c5fd0756433be860343909403ea08f\",\"path\":\"/segments/1\",\"size_bytes\":20},{\"etag\":\"35c5fd0756433be860343909403ea08f\",\"path\":\"/segments/2\",\"size_bytes\":20}],\"X-Auth-Token\":\"BTXogEGQrpPT2P7pFUxmJ4wWjIQikqxxjvFSrxr9\",\"X-Static-Large-Object\":\"false\",\"method\":\"mergeFile\",\"overwrite\":\"true\",\"uuid\":\"3dd62ded-e6af-4155-8b18-edd4dab503e8\"}";
	std::string responseStr = "?multipart-manifest=put&overwrite=true";
	middleware->data_pipeline(method, jsonStr, responseStr);
#endif

	//delete
	delete middleware, middleware = NULL;

	// total cost
	//long long t2 = get_time();
	//printf(" Total cost : %lld ms.\n", t2-t1);

	return 0;
}

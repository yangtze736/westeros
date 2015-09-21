//////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: interface.cpp
//
// Description:
//
// Created: 2015年04月29日 星期三 15时33分22秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "middleware.h"
#include <string.h>

#ifdef CloudPy

extern "C" {
	MiddleWare obj;

	bool getVersionInfo(char *version, char *info){
		std::string version_str;
		std::string info_str;

		getVersionInfo(version_str, info_str);
		strcpy(version, version_str.c_str());
		strcpy(info, info_str.c_str());
	}


	bool data_pipeline(char *method, char *json, char *response){
		std::string method_str(method);
		std::string json_str(json);
		std::string response_str(response);

		obj.data_pipeline(method_str, json_str, response_str);
		strcpy(response, response_str.c_str());
	}

	bool getUploadTask(char *user, char *query){
		std::string userStr(user);
		std::string queryStr;

		obj.getUploadTask(userStr, queryStr);
		strcpy(query, queryStr.c_str());
	}

	bool getDownloadTask(char *user, char *query){
		std::string userStr(user);
		std::string queryStr;

		obj.getDownloadTask(userStr, queryStr);
		strcpy(query, queryStr.c_str());
	}

	bool cleanByUser(char *user){
		std::string userStr(user);

		obj.cleanByUser(userStr);
	}

	bool cleanAllByUser(char *user){
		std::string userStr(user);

		obj.cleanAllByUser(userStr);
	}

	bool cleanByUserKey(char *user, char *file){
		std::string userStr(user);
		std::string fileStr(file);

		obj.cleanByUserKey(userStr, fileStr);
	}

	bool checkTaskStatus(char *uuid, char *response){
		std::string uuid_str(uuid);
		std::string response_str;

		obj.checkTaskStatus(uuid_str, response_str);
		strcpy(response, response_str.c_str());
	}
}
#endif

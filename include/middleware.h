///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: middleware.h
//
// Description:
//
// Created: 2015年04月29日 星期三 16时36分59秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __MIDDLEWARE_H__
#define __MIDDLEWARE_H__

#include "CppSQLite3.h"
#include <string>

// Parser parameter from URL
typedef struct ParameterStructTag
{
	ParameterStructTag()
	{
		op.clear();
		offset.clear();
		length.clear();
		destination.clear();
		ftype.clear();
		recursive.clear();
		overwrite.clear();
		permission.clear();
		type.clear();
		metadata.clear();
		version.clear();
		recent.clear();
		mode.clear();
		storetype.clear();
		multipart_manifest.clear();
	};
	std::string    op;
	std::string    offset;
	std::string    length;
	std::string    destination;
	std::string    ftype;
	std::string    recursive;
	std::string    overwrite;
	std::string    permission;
	std::string    type;
	std::string    metadata;
	std::string    version;
	std::string    recent;
	std::string    mode;
	std::string    storetype;
	std::string    multipart_manifest;
}ParameterStruct;

class MiddleWare{
	public:
		MiddleWare();
		~MiddleWare();
	
	public:
		bool getUploadTask(std::string &queryStr);
		//bool getUploadTaskByKey(std::string &queryStr);
		bool getDownloadTask(std::string &queryStr);
		//bool getDownloadTaskByKey(std::string &queryStr);
		bool checkTaskStatus(const std::string &uuid, std::string &response);

		bool getVersionInfo(std::string &version, std::string &info);
		bool data_pipeline(const std::string &method, const std::string &strJson, std::string &strResponse);

	private:
		bool getIpPort(std::string &strIpPort);
		bool judgeReturnCode(int ret);
		int  judgeFlag(ParameterStruct &paraStruct);
		bool parseFromStr(const std::string &str, ParameterStruct &paraStruct);
		bool parseFromStr(const std::string &str, const char *key, std::string &value);
		bool coverParameter(const std::string &para, const std::string &value, ParameterStruct &paraStruct);
		bool getTenant(const std::string &token, std::string &tenant, std::string &strResponse);
		bool test(void);
		bool recordTask2DB(const std::string &uuid, const std::string &response);
	
	private:
		std::string m_version;
		std::string m_buildTime;

		CppSQLite3DB *m_db;
		pthread_spinlock_t m_lock;
};

#endif //__MIDDLEWARE_H__

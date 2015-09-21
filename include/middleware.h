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
#include "versionInfo.h"
#include <string>

#define DEFAULT_CONF_FILE "./server.cnf"
#define DEFAULT_DB_FILE "./test.db"

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

class ConfApplication{
	public:
		ConfApplication(char *cnf){
			setIpPort(cnf);
		}
		~ConfApplication(){}
		bool setIpPort(char *cnf);
		std::string getIpPort();
	private:
		std::string m_strIpPort;
};

class Boot{
	public:
		Boot();
		virtual ~Boot();

		static Boot *getInstance(void) {
			return m_instance;
		}
	public:
		static Boot *m_instance;
		static ConfApplication *g_conf;

		CppSQLite3DB *m_db;
		pthread_spinlock_t m_lock;
};

class MiddleWare : public Boot{
	public:
		MiddleWare();
		~MiddleWare();
	
	public:
		bool getUploadTask(const std::string &userName, std::string &queryStr);
		bool getDownloadTask(const std::string &userName, std::string &queryStr);
		bool checkTaskStatus(const std::string &uuid, std::string &response);
		bool cleanByUser(const std::string &userName);
		bool cleanAllByUser(const std::string &userName);
		bool cleanByUserKey(const std::string &userName, const std::string &fileName);

		bool data_pipeline(const std::string &method, const std::string &strJson, std::string &strResponse);

	private:
		bool judgeReturnCode(int ret, std::string &errMsg);
		int  judgeFlag(ParameterStruct &paraStruct);
		bool parseFromStr(const std::string &str, ParameterStruct &paraStruct);
		bool parseFromStr(const std::string &str, const char *key, std::string &value);
		bool coverParameter(const std::string &para, const std::string &value, ParameterStruct &paraStruct);
		bool getTenant(const std::string &token, std::string &tenant, std::string &strResponse);
		bool recordTask2DB(const std::string &uuid, const std::string &response);

		std::string keepRespLength(const std::string &responseStr);
	
	private:
		std::string m_strIpPort;
};

// Public Interface
inline
bool getVersionInfo(std::string &version, std::string &info)
{
	version = MIDDLEWARE_VERSION_STRING;

	char buff[64] = {0};
	sprintf(buff, "%s %s", __TIME__, __DATE__);
	info = buff;

	return true;
}
#endif //__MIDDLEWARE_H__

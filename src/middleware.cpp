///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: middleware.cpp
//
// Description:
//
// Created: 2015年04月29日 星期三 16时40分12秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "configure.h"
#include "httpClient.h"
#include "parser.h"
#include "middleware.h"
#include "baseFunc.h"
#include "mfcFile.h"
#include "generateUrl.h"
#include "secrypto.h"
#include "createObject.h"
#include "readObject.h"
#include "fileUpload.h"
#include "fileDownload.h"
#include "trap.h"

#include <string.h>
#include <list>


//////////////////////////////////////////////////////
////        Conf Application

bool ConfApplication::setIpPort(char *conf)
{
	PR("ConfApplication::setIpPort");
	ConfigSet *cfg = new ConfigSet;
	int result = 0;
	if(cfg->LoadFromFile(conf, &result) < 0)
	{
		PR("get ip port from conf failed.");
		exit(0);
	}
	char *serverIp = (char*)cfg->GetValue("server", "ip", NULL);
	int port = cfg->GetIntVal("server", "port", 0);
	char ch[128] = {0};
	sprintf(ch, "%s:%d", serverIp, port);
	m_strIpPort = ch;
	
	delete cfg, cfg = NULL;
	return true;
}

std::string ConfApplication::getIpPort()
{
	return m_strIpPort;
}


//////////////////////////////////////////////////////
////        Boot Application

Boot *Boot::m_instance = NULL;
ConfApplication *Boot::g_conf = NULL;

Boot::Boot()
{
	PR("Boot::Boot");

	if(m_instance == NULL){
		m_instance = this;
	}else{
		PR("Boot can be instantiated for one and only one time!");
		exit(0);
	}
	g_conf = new ConfApplication(DEFAULT_CONF_FILE);
	m_db = new CppSQLite3DB;
	pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);

	try{
		m_db->open(DEFAULT_DB_FILE);
	}catch(CppSQLite3Exception &e){
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());;
	}
#ifdef CloudSDK
	if(!m_db->tableExists("status"))
	{
		try{
			m_db->execDML("create table status(id char(64) primary key, info char(512));");
		}
		catch(CppSQLite3Exception &e){
			fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		}
	}
#endif
}

Boot::~Boot()
{
	PR("Boot::~Boot");

	m_instance = NULL;
	m_db->close();
	delete m_db, m_db = NULL;
	pthread_spin_destroy(&m_lock);	
}


//////////////////////////////////////////////////////
////        main program

MiddleWare::MiddleWare()
	:m_strIpPort(g_conf->getIpPort())
{
	PR("MiddleWare::MiddleWare");

	PR("MiddleWare::getInstance() address: %p", MiddleWare::getInstance());
	PR("MiddleWare::getInstance()->m_db address: %p", MiddleWare::getInstance()->m_db);
	PR("MiddleWare::getInstance()->m_lock address: %p", MiddleWare::getInstance()->m_lock);
}

MiddleWare::~MiddleWare()
{
	PR("MiddleWare::~MiddleWare");
}


bool MiddleWare::data_pipeline(const std::string &method, const std::string &strJson, std::string &strResponse)
{
	std::string strIpPort = m_strIpPort;

	// parser parameter from strResponse string.
	ParameterStruct paraStruct;
	parseFromStr(strResponse, paraStruct);
	std::string mergeFileResp = strResponse;
	strResponse.clear();
	// end of parser

	// record task status when finish
	std::string uuid;

	if( "validate" == method )
	{
		PR("method is validate.");
		std::string email, passwd;

		Parser parser;
		parser.parseValidate(email, passwd, uuid, strJson);

		std::string strUrl = GenerateUrl::genValidate(strIpPort);
		PR("strUrl = [%s]", strUrl.c_str());

		std::string postField;
		postField.append("{\"password\":\"");postField.append(passwd);
		postField.append("\",\"email\":\"");postField.append(email);
		postField.append("\"}");
		
		HttpClient httpClient;
		httpClient.post(urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "containerList" == method )
	{
		PR("method is containerList.");
		std::string token, tenant;

		Parser parser;
		parser.parseContainerList(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genContainerList(strIpPort,version,tenant);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "createContainer" == method )
	{
		PR("method is createContainer.");
		std::string token, tenant, container;

		Parser parser;
		parser.parseCreateContainer(token, uuid, container, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateContainer(strIpPort,version,tenant,container);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "deleteContainer" == method )
	{
		PR("method is deleteContainer.");
		std::string token, tenant, container;
		
		Parser parser;
		parser.parseDeleteContainer(token, uuid, container, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDeleteContainer(strIpPort,version,tenant,container);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.del(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "listContainerObjects" == method )
	{
		PR("method is listContainerObjects.");
		std::string token, tenant, container;
		
		Parser parser;
		parser.parseListContainerObjects(token, uuid, container, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genListContainerObjects(strIpPort,version,tenant,container,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "createObject" == method )
	{
		PR("method is createObject.");
		std::string token, user, tenant, localObject, container, objectName;
		
		Parser parser;
		parser.parseCreateObject(token, user, uuid, localObject, container, objectName, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateObject(strIpPort,version,tenant,container,objectName);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient *httpClient = new CreateObject;
		httpClient->create_object(localObject,uuid,user,token,urlEncode(strUrl),strResponse);
		delete httpClient, httpClient = NULL;
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "deleteObject" == method )
	{
		PR("method is deleteObject.");
		std::string token, tenant, container, objectName;
		
		Parser parser;
		parser.parseDeleteObject(token, uuid, container, objectName, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDeleteObject(strIpPort,version,tenant,container,objectName);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.del(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "copyObject" == method )
	{
		PR("method is copyObject.");
		std::string token, tenant, container, objectName, dest;
		
		Parser parser;
		parser.parseCopy(token, uuid, container, objectName, dest, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCopyObject(strIpPort,version,tenant,container,objectName);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.copy(token, urlEncode(dest), urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "readObject" == method )
	{
		PR("method is readObject.");
		std::string token, user, tenant, localObject, container, objectName;
		
		Parser parser;
		parser.parseReadObject(token, user, uuid, localObject, container, objectName, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genReadObject(strIpPort,version,tenant,container,objectName);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient *httpClient = new ReadObject;
		httpClient->read_object(localObject,uuid,user,token,urlEncode(strUrl),strResponse);
		delete httpClient, httpClient = NULL;
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "getQuotaInfo" == method )
	{
		PR("method is getQuotaInfo.");
		std::string token, tenant;
		
		Parser parser;
		parser.parseGetQuotaInfo(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetQuotaInfo(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "fileUpload" == method )
	{
		PR("method is fileUpload.");
		std::string token, tenant, src, dst, time, user;
		
		Parser parser;
		parser.parseFileUpload(token, uuid, src, dst, time, user, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string meta = "{\"size\":\"" + toStr(MfcFile::getFileSize(src)) \
							+ "\",\"time\":\"" + time + "\"}";
		//Parser::genMetaData(meta, toStr(MfcFile::getFileSize(src)), time);
		coverParameter("metadata", meta, paraStruct);
		std::string strUrl = GenerateUrl::genFileUpload(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient *httpClient = new FileUpload;
		int flag = judgeFlag(paraStruct);
		httpClient->file_upload(src, user, uuid, token, urlEncode(strUrl), strResponse, flag);
		delete httpClient, httpClient = NULL;
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "mergeFile" == method )
	{
		PR("method is mergeFile.");
		std::string token, tenant, dest, flag, postField;
		
		Parser parser;
		parser.parseMergeFile(token, uuid, dest, flag, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		//std::string strUrl = GenerateUrl::genMergeFile(strIpPort,version,tenant,dest,paraStruct);
		std::string strUrl = "https://" + strIpPort + version + "AUTH_" + \
							  tenant + dest + mergeFileResp;
		PR("strUrl = [%s]", strUrl.c_str());
		PR("postField:[%s]", postField.c_str());

		HttpClient httpClient;
		httpClient._posts(token, urlEncode(strUrl), postField, strResponse, flag);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "createDir" == method )
	{
		PR("method is createDir.");
		std::string token, tenant, dst;
		
		Parser parser;
		parser.parseCreateDir(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateDir(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "deleteFileDir" == method )
	{
		PR("method is deleteFileDir.");
		std::string token, tenant, dst;
		
		Parser parser;
		parser.parseDeleteFileDir(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDeleteFileDir(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.del(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "batchDeleteFileDir" == method )
	{
		PR("method is batchDeleteFileDir.");
		std::string token, tenant, postField;
		
		Parser parser;
		parser.parseBatchDeleteFileDir(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genBatchDeleteFileDir(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());
		PR("postField:[%s]", postField.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "moveFileDir" == method )
	{
		PR("method is moveFileDir.");
		std::string token, tenant, src, dst;
		
		Parser parser;
		parser.parseMoveFileDir(token, uuid, src, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genMoveFileDir(strIpPort,version,tenant,src,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse, urlEncode(dst));
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "batchMoveFileDir" == method )
	{
		PR("method is batchMoveFileDir.");
		std::string token, tenant, postField;
		
		Parser parser;
		parser.parseBatchMoveFileDir(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genBatchMoveFileDir(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());
		PR("postField:[%s]", postField.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "copyFileDir" == method )
	{
		PR("method is copyFileDir.");
		std::string token, tenant, src, dst;
		
		Parser parser;
		parser.parseCopyFileDir(token, uuid, src, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCopyFileDir(strIpPort,version,tenant,src,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse, urlEncode(dst));
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "batchCopyFileDir" == method )
	{
		PR("method is batchCopyFileDir.");
		std::string token, tenant, postField;
		
		Parser parser;
		parser.parseBatchCopyFileDir(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genBatchCopyFileDir(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());
		PR("postField:[%s]", postField.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "readFile" == method )
	{
		PR("method is readFile.");
		std::string token, tenant, src, dst, user;
		
		Parser parser;
		parser.parseReadFile(token, uuid, src, dst, user, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genReadFile(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient *httpClient = new FileDownload;
		int flag = judgeFlag(paraStruct);
		httpClient->file_download(src, uuid, user, token, urlEncode(strUrl), strResponse, flag);
		delete httpClient, httpClient = NULL;
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "getFileHistory" == method )
	{
		PR("method is getFileHistory.");
		std::string token, tenant, dst;
		
		Parser parser;
		parser.parseGetFileHistory(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetFileHistory(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "getOperateHistory" == method )
	{
		PR("method is getOperateHistory.");
		std::string token, tenant;
		
		Parser parser;
		parser.parseGetOperateHistory(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetOperateHistory(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "deleteOperateHistory" == method )
	{
		PR("method is deleteOperateHistory.");
		std::string token, tenant;
		
		Parser parser;
		parser.parseDeleteOperateHistory(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDelOperHistory(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "createSymbolicLink" == method )
	{
		PR("method is createSymbolicLink.");
		std::string token, tenant, dst;
		
		Parser parser;
		parser.parseCreateSymbolicLink(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateSymbolicLink(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "renameFileDir" == method )
	{
		PR("method is renameFileDir.");
		std::string token, tenant, dst;
		
		Parser parser;
		parser.parseRenameFileDir(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genRenameFileDir(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "getFileAttribute" == method )
	{
		PR("method is getFileAttribute.");
		std::string token, tenant, dst;

		Parser parser;
		parser.parseGetFileAttribute(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetFileAttribute(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "getRecycleList" == method )
	{
		PR("method is getRecycleList.");
		std::string token, tenant;

		Parser parser;
		parser.parseGetRecycleList(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetRecycleList(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "moveRecycle" == method )
	{
		PR("method is moveRecycle.");
		std::string token, tenant, postField;

		Parser parser;
		parser.parseMoveRecycle(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genMoveRecycle(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());
		PR("postField:[%s]", postField.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "cleanRecycle" == method )
	{
		PR("method is cleanRecycle.");
		std::string token, tenant;

		Parser parser;
		parser.parseCleanRecycle(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCleanRecycle(strIpPort,version,tenant,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), "", strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "setPermission" == method )
	{
		PR("method is setPermission.");
		std::string token, tenant, dst;

		Parser parser;
		parser.parseSetPermission(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genSetPermission(strIpPort,version,tenant,dst,paraStruct);	
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "getFileList" == method )
	{
		PR("method is getFileList.");
		std::string token, tenant, dst;

		Parser parser;
		parser.parseGetFileList(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetFileList(strIpPort,version,tenant,dst,paraStruct);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "createStorageQuota" == method )
	{
		PR("method is createStorageQuota.");
		std::string token, tenant, metaQuota;

		Parser parser;
		parser.parseCreateStorageQuota(token,uuid,metaQuota,strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateStorageQuota(strIpPort,version,tenant);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.postss(token, metaQuota, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "userRegister" == method )
	{
		PR("method is userRegister.");
		std::string token, tenant;

		Parser parser;
		parser.parseUserRegister(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genUserRegister(strIpPort,version,tenant);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "refreshToken" == method )
	{
		PR("method is refreshToken.");
		std::string token, passwd, email;

		Parser parser;
		parser.parseRefreshToken(token, passwd, email, uuid, strJson);

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genRefreshToken(strIpPort,version);
		PR("strUrl = [%s]", strUrl.c_str());

		std::string postField;
		postField.append("{\"password\":\"");postField.append(passwd);
		postField.append("\",\"email\":\"");postField.append(email);
		postField.append("\"}");

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "getCloudServVersion" == method )
	{
		PR("method is getCloudServVersion.");
		std::string token, tenant;

		Parser parser;
		parser.parseGetCloudServVersion(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			recordTask2DB(uuid, strResponse);
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetCloudServVersion(strIpPort,version,tenant);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "verifyToken" == method )
	{
		PR("method is verifyToken.");
		std::string token;

		Parser parser;
		parser.parseVerifyToken(token, uuid, strJson);

		std::string strUrl = GenerateUrl::genVerifyToken(strIpPort);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), "", strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "networkRegister" == method )
	{
		PR("method is networkRegister.");
		std::string postField;

		Parser parser;
		parser.parseNetworkRegister(postField, uuid, strJson);

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genNetworkRegister(strIpPort,version);
		PR("strUrl = [%s]", strUrl.c_str());
		PR("postField:[%s]", postField.c_str());

		HttpClient httpClient;
		httpClient.post(urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "netCloudRegister" == method )
	{
		PR("method is netCloudRegister.");
		std::string postField;

		Parser parser;
		parser.parseNetCloudRegister(postField, uuid, strJson);

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genNetCloudRegister(strIpPort,version);
		PR("strUrl = [%s]", strUrl.c_str());
		PR("postField:[%s]", postField.c_str());

		HttpClient httpClient;
		httpClient.post(urlEncode(strUrl), postField, strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else if( "validateNetworkAccout" == method )
	{
		PR("method is validateNetworkAccout.");
		std::string token;

		Parser parser;
		parser.parseValidateNetworkAccout(token, uuid, strJson);

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genValNetAccout(strIpPort,version);
		PR("strUrl = [%s]", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), "", strResponse);
		PR("Response:\n%s",strResponse.c_str());
	}
	else
	{
		fprintf(stderr, "can not match method:[%s] !\n",method.c_str());
		return false;
	}
	// record task status now
	recordTask2DB(uuid, strResponse);

	return true;
}

bool MiddleWare::getUploadTask(const std::string &userName, std::string &queryStr)
{
	if(!m_db->tableExists("upload"))
	{
		fprintf(stderr, "table: upload isn't exist\n");
		return false;
	}
	queryStr.clear();
	char buf[128] = {0};
	sprintf(buf, "select * from upload where user = '%s';",userName.c_str());

	CppSQLite3Query q;
	pthread_spin_lock(&m_lock);
	try{
		q = m_db->execQuery(buf);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	std::map<std::string,std::string> taskMap;
	while(!q.eof())
	{
		std::string status = q.fieldValue(3) + std::string("|") + q.fieldValue(4);
		taskMap.insert(std::map<std::string,std::string>::value_type(q.fieldValue(2),status));
		q.nextRow();
	}
	Parser::genTaskStr(queryStr, taskMap);

	return true;
}

bool MiddleWare::getDownloadTask(const std::string &userName, std::string &queryStr)
{
	if(!m_db->tableExists("download"))
	{
		fprintf(stderr, "table: download isn't exist\n");
		return false;
	}
	queryStr.clear();
	char buf[128] = {0};
	sprintf(buf, "select * from download where user = '%s';",userName.c_str());

	CppSQLite3Query q;
	pthread_spin_lock(&m_lock);
	try{
		q = m_db->execQuery(buf);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	std::map<std::string,std::string> taskMap;
	while(!q.eof())
	{
		std::string status = q.fieldValue(3) + std::string("|") + q.fieldValue(4);
		taskMap.insert(std::map<std::string,std::string>::value_type(q.fieldValue(2),status));
		q.nextRow();
	}
	Parser::genTaskStr(queryStr, taskMap);
	
	return true;
}

bool MiddleWare::cleanByUserKey(const std::string &userName, const std::string &fileName)
{
	char buf1[512] = {0};
	sprintf(buf1, "delete from upload where user = '%s' and file = '%s';", userName.c_str(), fileName.c_str());
	char buf2[512] = {0};
	sprintf(buf2, "delete from download where user = '%s' and file = '%s';", userName.c_str(), fileName.c_str());

	// < 1 >
	if(!m_db->tableExists("upload"))
	{
		fprintf(stderr, "table: upload isn't exist\n");
		return false;
	}
	CppSQLite3Query p;
	pthread_spin_lock(&m_lock);
	try{
		p = m_db->execQuery(buf1);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	// < 2 >
	if(!m_db->tableExists("download"))
	{
		fprintf(stderr, "table: download isn't exist\n");
		return false;
	}
	CppSQLite3Query q;
	pthread_spin_lock(&m_lock);
	try{
		q = m_db->execQuery(buf2);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	return true;
}

bool MiddleWare::cleanByUser(const std::string &userName)
{
	char buf1[128] = {0};
	sprintf(buf1, "delete from upload where user = '%s' and total=now;", userName.c_str());
	char buf2[128] = {0};
	sprintf(buf2, "delete from download where user = '%s' and total=now;", userName.c_str());

	// < 1 >
	if(!m_db->tableExists("upload"))
	{
		fprintf(stderr, "table: upload isn't exist\n");
		return false;
	}
	CppSQLite3Query p;
	pthread_spin_lock(&m_lock);
	try{
		p = m_db->execQuery(buf1);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	// < 2 >
	if(!m_db->tableExists("download"))
	{
		fprintf(stderr, "table: download isn't exist\n");
		return false;
	}
	CppSQLite3Query q;
	pthread_spin_lock(&m_lock);
	try{
		q = m_db->execQuery(buf2);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	return true;
}

bool MiddleWare::cleanAllByUser(const std::string &userName)
{
	char buf1[128] = {0};
	sprintf(buf1, "delete from upload where user = '%s';", userName.c_str());
	char buf2[128] = {0};
	sprintf(buf2, "delete from download where user = '%s';", userName.c_str());

	// < 1 >
	if(!m_db->tableExists("upload"))
	{
		fprintf(stderr, "table: upload isn't exist\n");
		return false;
	}
	CppSQLite3Query p;
	pthread_spin_lock(&m_lock);
	try{
		p = m_db->execQuery(buf1);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	// < 2 >
	if(!m_db->tableExists("download"))
	{
		fprintf(stderr, "table: download isn't exist\n");
		return false;
	}
	CppSQLite3Query q;
	pthread_spin_lock(&m_lock);
	try{
		q = m_db->execQuery(buf2);
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	return true;
}

bool MiddleWare::recordTask2DB(const std::string &uuid, const std::string &response)
{
#ifdef CloudSDK
	if(!m_db->tableExists("status"))
	{
		fprintf(stderr, "table: status isn't exist\n");
		return false;
	}

	// keep response length, fix bug(stack smashing detected)
	std::string resp = keepRespLength(response);

	char buf[64+512+100] = {0};
	sprintf(buf, "insert into status values ('%s', '%s');", uuid.c_str(), resp.c_str());
	pthread_spin_lock(&m_lock);
	try{
		m_db->execDML(buf);
	}
	catch(CppSQLite3Exception &e){
		fprintf(stderr, "Throw exception when recordTask2DB, errorNo: %d , %s.\n", \
				e.errorCode(), e.errorMessage());
	}
	pthread_spin_unlock(&m_lock);
#endif
	return true;
}

bool MiddleWare::checkTaskStatus(const std::string &uuid, std::string &queryStr)
{
#ifdef CloudSDK
	if(!m_db->tableExists("status"))
	{
		fprintf(stderr, "table: status isn't exist\n");
		return false;
	}
	char buf[128] = {0};
	sprintf(buf, "select * from status where id = '%s';", uuid.c_str());
	queryStr.clear();

	CppSQLite3Query q;
	pthread_spin_lock(&m_lock);
	try{
		q = m_db->execQuery(buf);
	}
	catch(CppSQLite3Exception &e){
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	}
	pthread_spin_unlock(&m_lock);

	std::map<std::string,std::string> taskMap;
	while(!q.eof())
	{
		taskMap.insert(std::map<std::string,std::string>::value_type(q.fieldValue(0),q.fieldValue(1)));
		q.nextRow();
	}
	Parser::genTaskStr(queryStr, taskMap);
#endif
	return true;
}

bool MiddleWare::coverParameter(const std::string &para, const std::string &value, ParameterStruct &paraStruct)
{
	if(!strcasecmp("OP", para.c_str()))
	{
		paraStruct.op = value;
	}
	else if(!strcasecmp("OFFSET", para.c_str()))
	{
		paraStruct.offset = value;
	}
	else if(!strcasecmp("LENGTH", para.c_str()))
	{
		paraStruct.length = value;
	}
	else if(!strcasecmp("DESTINATION", para.c_str()))
	{
		paraStruct.destination = value;
	}
	else if(!strcasecmp("FTYPE", para.c_str()))
	{
		paraStruct.ftype = value;
	}
	else if(!strcasecmp("RECURSIVE", para.c_str()))
	{
		paraStruct.recursive = value;
	}
	else if(!strcasecmp("OVERWRITE", para.c_str()))
	{
		paraStruct.overwrite = value;
	}
	else if(!strcasecmp("PERMISSION", para.c_str()))
	{
		paraStruct.permission = value;
	}
	else if(!strcasecmp("TYPE", para.c_str()))
	{
		paraStruct.type = value;
	}
	else if(!strcasecmp("METADATA", para.c_str()))
	{
		paraStruct.metadata = value;
	}
	else if(!strcasecmp("VERSION", para.c_str()))
	{
		paraStruct.version = value;
	}
	else if(!strcasecmp("RECENT", para.c_str()))
	{
		paraStruct.recent = value;
	}
	else if(!strcasecmp("MODE", para.c_str()))
	{
		paraStruct.mode = value;
	}
	else if(!strcasecmp("STORETYPE", para.c_str()))
	{
		paraStruct.storetype = value;
	}
	else if(!strcasecmp("MULTIPART-MANIFEST", para.c_str()))
	{
		paraStruct.multipart_manifest = value;
	}
	else
	{
		return false;
	}

	return true;
}

bool MiddleWare::parseFromStr(const std::string &str, ParameterStruct &paraStruct)
{
	// When add parameter, parse here!
	parseFromStr(str, "op", paraStruct.op);
	parseFromStr(str, "offset", paraStruct.offset);
	parseFromStr(str, "length", paraStruct.length);
	parseFromStr(str, "destination", paraStruct.destination);
	parseFromStr(str, "ftype", paraStruct.ftype);
	parseFromStr(str, "recursive", paraStruct.recursive);
	parseFromStr(str, "overwrite", paraStruct.overwrite);
	parseFromStr(str, "permission", paraStruct.permission);
	parseFromStr(str, "type", paraStruct.type);
	parseFromStr(str, "metadata", paraStruct.metadata);
	parseFromStr(str, "version", paraStruct.version);
	parseFromStr(str, "recent", paraStruct.recent);
	parseFromStr(str, "mode", paraStruct.mode);
	parseFromStr(str, "storetype", paraStruct.storetype);
	parseFromStr(str, "multipart-manifest", paraStruct.multipart_manifest);
	
	return true;
}

bool MiddleWare::parseFromStr(const std::string &str, const char *key, std::string &value)
{
	value.clear();
	const char split = '&';
	const char assign = '=';

	std::string str0(1,'?'), str1(1,split), str2(key), str3(1,assign);
	// for case: &key=value
	std::string target1 = str1 + str2 + str3;
	// for case: ?key=value
	std::string target2 = str0 + str2 + str3;

	string::size_type pos1 = str.find(target1);
	if(pos1 != string::npos)
	{
		size_t offset = target1.length();

		string::size_type pos2 = str.find(split,pos1+offset);
		if(pos2 != string::npos)
		{
			value = str.substr(pos1+offset,pos2-pos1-offset);
		}
		else
		{
			value = str.substr(pos1+offset,str.length()-pos1-offset);
		}
	}
	else
	{
		string::size_type pos3 = str.find(target2);
		if(pos3 != string::npos)
		{
			size_t offset = target2.length();

			string::size_type pos4 = str.find(split,pos3+offset);
			if(pos4 != string::npos)
			{
				value = str.substr(pos3+offset,pos4-pos3-offset);
			}
			else
			{
				value = str.substr(pos3+offset,str.length()-pos3-offset);
			}
		}
	}

	return true;
}

bool MiddleWare::getTenant(const std::string &token, std::string &tenant, std::string &strResponse)
{
	std::string output;

	Parser *parser = new Parser;
	CSecrypto *sc = NULL;
	try{
		sc = new CSecrypto;
	}catch(...){
		strResponse = "{\"status\":\"" + toStr(15004) + "\",\"errMsg\":\"" \
					   + std::string("catch except when new CSecrypto") + "\"}";
		return false;
	}

	char tokenJson[256] = {0};
	sprintf(tokenJson, "{\"access_token\":\"%s\"}", token.c_str());
	string tokenString(tokenJson);

	// fix bug, if do not catch except, crash.
	int ret = 0;
	try{
		ret = sc->convert_ukey2_cloudtoken(tokenString, output);
	}catch(...){
		strResponse = "{\"status\":\"" + toStr(15004) + "\",\"errMsg\":\"" \
					   + std::string("catch except at convert_ukey2_cloudtoken") + "\"}";
		return false;
	}
	std::string errMsg;
	if(!judgeReturnCode(ret, errMsg))
	{
		strResponse = "{\"status\":\"" + toStr(ret) + "\",\"errMsg\":\"" \
					   + errMsg + "\"}";
		delete sc, sc = NULL;
		return false;
	}
	tenant = parser->parserKey(output, "tenant_id");

	delete sc, sc = NULL;
	delete parser, parser = NULL;

	return true;
}

bool MiddleWare::judgeReturnCode(int ret, std::string &errMsg)
{
	switch(ret)
	{
		case Secrypto_NoError :
			break;
		case Error_CommunicationKey :
			errMsg = "get communication key fail.";
			return false;
			break;
		case Error_PublicKey :
			errMsg = "get public key fail.";
			return false;
			break;
		case Error_Expire :
			errMsg = "the access token provided is expired.";
			return false;
			break;
		case Error_NoJson :
			errMsg = "no json object could be decoded.";
			return false;
			break;
		case Error_WrongRequest :
			errMsg = "wrong request.";
			return false;
			break;
		case Error_Invalid :
			errMsg = "invalid access token.";
			return false;
			break;
		default :
			errMsg = "unknown error number.";
			return false;
			break;
	}
	return true;
}

int MiddleWare::judgeFlag(ParameterStruct &paraStruct)
{
	std::string modeStr = paraStruct.mode;;
	
	if(strcasecmp(modeStr.c_str(),"NORMAL") == 0)
	{
		return 1;
	}
	else if(strcasecmp(modeStr.c_str(),"ENCRYPT") == 0)
	{
		return 2;
	}
	else if(strcasecmp(modeStr.c_str(),"COMPRESS") == 0)
	{
		return 3;
	}
	else if(strcasecmp(modeStr.c_str(),"ENCRYPT_COMPRESS") == 0)
	{
		return 4;
	}
	else
	{
		return -1; //error
	}
}

std::string MiddleWare::keepRespLength(const std::string &responseStr)
{
	std::string ret;

	if(responseStr.length() >= 512){
		PR("respStr's length is bigger than 512, cut off.");

		std::string status, msg;
		Parser parser;
		parser.judgeRespJson(responseStr, status, msg);
		ret = "{\"status\":\"" + toStr(status) + "\",\"Msg\":\"" \
			   + msg + "\"}";
	}
	else{
		ret = responseStr;
	}
	
	return ret;
}


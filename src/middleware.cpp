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
#include "versionInfo.h"
#include "baseFunc.h"
#include "mfcFile.h"
#include "generateUrl.h"
#include "secrypto.h"
#include "createObject.h"
#include "readObject.h"
#include "fileUpload.h"
#include "fileDownload.h"

#include <string.h>
#include <list>

MiddleWare::MiddleWare()
{
	m_version = middleware_version_info;
	char tmpBuildTime[64] = {0};
	sprintf(tmpBuildTime, "%s %s", __TIME__, __DATE__);
	m_buildTime = tmpBuildTime;

	m_db = new CppSQLite3DB;
	m_db->open("./test.db");
}

MiddleWare::~MiddleWare()
{
}

bool MiddleWare::data_pipeline(const std::string &method, const std::string &strJson, std::string &strResponse)
{
	std::string strIpPort;
	if(!getIpPort(strIpPort))
	{
		printf("get ip port from conf failed.\n");
		return false;
	}

#if 0
	// test encode && decode	
	std::string str1 = "https://1.2.3.4:443/v1/AUTH_messi126com/normal/文 件3?op=CREATE&overwrite=true";
	printf("str1 %s \n",str1.c_str());std::string str2 = urlEncode(str1);
	printf("str2 %s \n",str2.c_str());std::string str3 = urlDecode(str2);
	printf("str3 %s \n",str3.c_str());exit(0);
#endif

	//parser parameter from strResponse string.
	ParameterStruct paraStruct;
	parseFromStr(strResponse, paraStruct);
	strResponse.clear();
	// end of parser

	if( 0 == strcmp(method.c_str(), "validate"))
	{
		printf("method is validate.\n");
		std::string email, passwd, uuid;

		Parser parser;
		parser.parseValidate(email, passwd, uuid, strJson);

		std::string strUrl = GenerateUrl::genValidate(strIpPort);
		printf("strUrl = [%s]\n", strUrl.c_str());

		std::string postField;
		postField.append("{\"password\":\"");postField.append(passwd);
		postField.append("\",\"email\":\"");postField.append(email);
		postField.append("\"}");
		
		HttpClient httpClient;
		httpClient.post(urlEncode(strUrl), postField, strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "containerList"))
	{
		printf("method is containerList.\n");
		std::string token, tenant, uuid;

		Parser parser;
		parser.parseContainerList(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genContainerList(strIpPort,version,tenant);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "createContainer"))
	{
		printf("method is createContainer.\n");
		std::string token, tenant, container, uuid;

		Parser parser;
		parser.parseCreateContainer(token, uuid, container, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateContainer(strIpPort,version,tenant,container);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "deleteContainer"))
	{
		printf("method is deleteContainer.\n");
		std::string token, tenant, container, uuid;
		
		Parser parser;
		parser.parseDeleteContainer(token, uuid, container, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDeleteContainer(strIpPort,version,tenant,container);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.del(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "listContainerObjects"))
	{
		printf("method is listContainerObjects.\n");
		std::string token, tenant, container, uuid;
		
		Parser parser;
		parser.parseListContainerObjects(token, uuid, container, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genListContainerObjects(strIpPort,version,tenant,container,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "createObject"))
	{
		printf("method is createObject.\n");
		std::string token, tenant, localObject, container, objectName, uuid;
		
		Parser parser;
		parser.parseCreateObject(token, uuid, localObject, container, objectName, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateObject(strIpPort,version,tenant,container,objectName);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient *httpClient = new CreateObject;
		httpClient->create_object(localObject,uuid,token,urlEncode(strUrl),strResponse);
		delete httpClient, httpClient = NULL;
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "deleteObject"))
	{
		printf("method is deleteObject.\n");
		std::string token, tenant, container, uuid, objectName;
		
		Parser parser;
		parser.parseDeleteObject(token, uuid, container, objectName, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDeleteObject(strIpPort,version,tenant,container,objectName);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.del(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "copyObject"))
	{
		printf("method is copyObject.\n");
		std::string token, tenant, container, uuid, objectName, dest;
		
		Parser parser;
		parser.parseCopy(token, uuid, container, objectName, dest, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCopyObject(strIpPort,version,tenant,container,objectName);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.copy(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "readObject"))
	{
		printf("method is readObject.\n");
		std::string token, tenant, localObject, container, objectName, uuid;
		
		Parser parser;
		parser.parseReadObject(token, uuid, localObject, container, objectName, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genReadObject(strIpPort,version,tenant,container,objectName);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient *httpClient = new ReadObject;
		httpClient->read_object(localObject,uuid,token,urlEncode(strUrl),strResponse);
		delete httpClient, httpClient = NULL;
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "getQuotaInfo"))
	{
		printf("method is getQuotaInfo.\n");
		std::string token, tenant, uuid;
		
		Parser parser;
		parser.parseGetQuotaInfo(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetQuotaInfo(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "fileUpload"))
	{
		printf("method is fileUpload.\n");
		std::string token, tenant, src, dst, uuid;
		
		Parser parser;
		parser.parseFileUpload(token, uuid, src, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genFileUpload(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient *httpClient = new FileUpload;
		httpClient->file_upload(src, uuid, token, urlEncode(strUrl), strResponse);
		delete httpClient, httpClient = NULL;
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "mergeFile"))
	{
		printf("method is mergeFile.\n");
		std::string token, tenant, uuid, dest, postField;
		
		Parser parser;
		parser.parseMergeFile(token, uuid, dest, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genMergeFile(strIpPort,version,tenant,dest,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse, true);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "createDir"))
	{
		printf("method is createDir.\n");
		std::string token, tenant, dst, uuid;
		
		Parser parser;
		parser.parseCreateDir(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateDir(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "deleteFileDir"))
	{
		printf("method is deleteFileDir.\n");
		std::string token, tenant, dst, uuid;
		
		Parser parser;
		parser.parseDeleteFileDir(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDeleteFileDir(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.del(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "batchDeleteFileDir"))
	{
		printf("method is batchDeleteFileDir.\n");
		std::string token, tenant, uuid, postField;
		
		Parser parser;
		parser.parseBatchDeleteFileDir(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genBatchDeleteFileDir(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "moveFileDir"))
	{
		printf("method is moveFileDir.\n");
		std::string token, tenant, src, dst, uuid;
		
		Parser parser;
		parser.parseMoveFileDir(token, uuid, src, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genMoveFileDir(strIpPort,version,tenant,src,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse, dst);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "batchMoveFileDir"))
	{
		printf("method is batchMoveFileDir.\n");
		std::string token, tenant, uuid, postField;
		
		Parser parser;
		parser.parseBatchMoveFileDir(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genBatchMoveFileDir(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "copyFileDir"))
	{
		printf("method is copyFileDir.\n");
		std::string token, tenant, src, dst, uuid;
		
		Parser parser;
		parser.parseCopyFileDir(token, uuid, src, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCopyFileDir(strIpPort,version,tenant,src,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse, dst);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "batchCopyFileDir"))
	{
		printf("method is batchCopyFileDir.\n");
		std::string token, tenant, postField, uuid;
		
		Parser parser;
		parser.parseBatchCopyFileDir(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genBatchCopyFileDir(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "readFile"))
	{
		printf("method is readFile.\n");
		std::string token, tenant, src, dst, uuid;
		
		Parser parser;
		parser.parseReadFile(token, uuid, src, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genReadFile(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient *httpClient = new FileDownload;
		httpClient->file_download(src, uuid, token, urlEncode(strUrl), strResponse);
		delete httpClient, httpClient = NULL;
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "getFileHistory"))
	{
		printf("method is getFileHistory.\n");
		std::string token, tenant, dst, uuid;
		
		Parser parser;
		parser.parseGetFileHistory(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetFileHistory(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "getOperateHistory"))
	{
		printf("method is getOperateHistory.\n");
		std::string token, tenant, uuid;
		
		Parser parser;
		parser.parseGetOperateHistory(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetOperateHistory(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "deleteOperateHistory"))
	{
		printf("method is deleteOperateHistory.\n");
		std::string token, tenant, uuid;
		
		Parser parser;
		parser.parseDeleteOperateHistory(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genDelOperHistory(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "createSymbolicLink"))
	{
		printf("method is createSymbolicLink.\n");
		std::string token, tenant, dst, uuid;
		
		Parser parser;
		parser.parseCreateSymbolicLink(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateSymbolicLink(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "renameFileDir"))
	{
		printf("method is renameFileDir.\n");
		std::string token, tenant, dst, uuid;
		
		Parser parser;
		parser.parseRenameFileDir(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genRenameFileDir(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "getFileAttribute"))
	{
		printf("method is getFileAttribute.\n");
		std::string token, tenant, dst, uuid;

		Parser parser;
		parser.parseGetFileAttribute(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetFileAttribute(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "getRecycleList"))
	{
		printf("method is getRecycleList.\n");
		std::string token, tenant, uuid;

		Parser parser;
		parser.parseGetRecycleList(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetRecycleList(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "moveRecycle"))
	{
		printf("method is moveRecycle.\n");
		std::string token, tenant, postField, uuid;

		Parser parser;
		parser.parseMoveRecycle(token, uuid, postField, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genMoveRecycle(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "cleanRecycle"))
	{
		printf("method is cleanRecycle.\n");
		std::string token, tenant, uuid;

		Parser parser;
		parser.parseCleanRecycle(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCleanRecycle(strIpPort,version,tenant,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), "", strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "setPermission"))
	{
		printf("method is setPermission.\n");
		std::string token, tenant, uuid, dst;

		Parser parser;
		parser.parseSetPermission(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}
		
		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genSetPermission(strIpPort,version,tenant,dst,paraStruct);	
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "getFileList"))
	{
		printf("method is getFileList.\n");
		std::string token, tenant, uuid, dst;

		Parser parser;
		parser.parseGetFileList(token, uuid, dst, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetFileList(strIpPort,version,tenant,dst,paraStruct);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "createStorageQuota"))
	{
		printf("method is createStorageQuota.\n");
		std::string token, tenant, uuid, metaQuota;

		Parser parser;
		parser.parseCreateStorageQuota(token,uuid,metaQuota,strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genCreateStorageQuota(strIpPort,version,tenant);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.postss(token, metaQuota, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "userRegister"))
	{
		printf("method is userRegister.\n");
		std::string token, tenant, uuid;

		Parser parser;
		parser.parseUserRegister(token, uuid, strJson);
		if(!getTenant(token, tenant, strResponse))
		{
			return false;
		}

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genUserRegister(strIpPort,version,tenant);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.put(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "refreshToken"))
	{
		printf("method is refreshToken.\n");
		std::string token, tenant, passwd, email, uuid;

		Parser parser;
		parser.parseRefreshToken(token, passwd, email, uuid, strJson);

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genRefreshToken(strIpPort,version);
		printf("strUrl = [%s]\n", strUrl.c_str());

		std::string postField;
		postField.append("{\"password\":\"");postField.append(passwd);
		postField.append("\",\"email\":\"");postField.append(email);
		postField.append("\"}");

		HttpClient httpClient;
		httpClient.posts(token, urlEncode(strUrl), postField, strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else if(0 == strcmp(method.c_str(), "getCloudServVersion"))
	{
		printf("method is getCloudServVersion.\n");
		std::string token, tenant, uuid;

		Parser parser;
		parser.parseGetCloudServVersion(token, uuid, strJson);

		std::string version("/v1/");
		std::string strUrl = GenerateUrl::genGetCloudServVersion(strIpPort,version,tenant);
		printf("strUrl = [%s]\n", strUrl.c_str());

		HttpClient httpClient;
		httpClient.get(token, urlEncode(strUrl), strResponse);
		printf("Response:\n%s\n",strResponse.c_str());
	}
	else
	{
		printf("can not match method:[%s] !\n",method.c_str());
		return false;
	}

	return true;
}

//bool MiddleWare::getUploadTask(std::map<std::string, std::string> &taskMap)
bool MiddleWare::getUploadTask(std::string &queryStr)
{
	if(!m_db->tableExists("upload"))
	{
		fprintf(stdout, "table: upload isn't exist\n");
		return false;
	}
	queryStr.clear();
	queryStr.append("{");

	CppSQLite3Query q;
	try{
		q = m_db->execQuery("select * from upload;");
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	} 
	while(!q.eof())
	{
		//taskMap.insert(std::map<std::string,std::string>::value_type(q.fieldValue(0),q.fieldValue(1)));
		queryStr.append("\"");
		queryStr.append(q.fieldValue(0));
		queryStr.append("\":\"");
		queryStr.append(q.fieldValue(1));
		queryStr.append("|");
		queryStr.append(q.fieldValue(2));
		queryStr.append("\",");
		q.nextRow();
	}
	queryStr.append("}");

	//fprintf(stdout, "query %d lines from table upload.", taskMap.size());
	return true;
}

//bool MiddleWare::getDownloadTask(std::map<std::string, std::string> &taskMap)
bool MiddleWare::getDownloadTask(std::string &queryStr)
{
	if(!m_db->tableExists("download"))
	{
		fprintf(stdout, "table: download isn't exist\n");
		return false;
	}
	queryStr.clear();
	queryStr.append("{");

	CppSQLite3Query q;
	try{
		q = m_db->execQuery("select * from download;");
	}
	catch(CppSQLite3Exception &e){                                          
		fprintf(stderr, "%d , %s.\n", e.errorCode(), e.errorMessage());
		return false;
	} 
	while(!q.eof())
	{
		//taskMap.insert(std::map<std::string,std::string>::value_type(q.fieldValue(0),q.fieldValue(1)));
		queryStr.append("\"");
		queryStr.append(q.fieldValue(0));
		queryStr.append("\":\"");
		queryStr.append(q.fieldValue(1));
		queryStr.append("|");
		queryStr.append(q.fieldValue(2));
		queryStr.append("\",");
		q.nextRow();
	}
	queryStr.append("}");

	//fprintf(stdout, "query %d lines from table upload.", taskMap.size());
	return true;
}

bool MiddleWare::getVersionInfo(std::string &version, std::string &info)
{
	version = m_version;
	info = m_buildTime;

	return true;
}

bool MiddleWare::getIpPort(std::string &strIpPort)
{
	printf("\n<*>\t parse configure\n");

	ConfigSet *cfg = new ConfigSet;
	int result = 0;
	if(cfg->LoadFromFile("./server.conf", &result) < 0)
	{
		//printf("failed to read conf file.\n");
		return false;
	}
	char *serverIp = (char*)cfg->GetValue("server", "ip", NULL);
	int port = cfg->GetIntVal("server", "port", 0);
	char ch[128] = {0};
	sprintf(ch, "%s:%d", serverIp, port);
	strIpPort = ch;
	
	delete cfg, cfg = NULL;
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
	CSecrypto *sc = new CSecrypto;
	sc->secrypto_init();

	char tokenJson[256] = {0};
	sprintf(tokenJson, "{\"access_token\":\"%s\"}", token.c_str());
	string tokenString(tokenJson);

	int ret = sc->convert_ukey2_cloudtoken(tokenString, output);
	if(!judgeReturnCode(ret))
	{
		strResponse = "{\"errorNum\":\"" + toStr(ret) + "\"}";
		delete sc, sc = NULL;
		return false;
	}
	tenant = parser->parserKey(output, "tenant_id");

	delete sc, sc = NULL;
	delete parser, parser = NULL;

	return true;
}

bool MiddleWare::judgeReturnCode(int ret)
{
	switch(ret)
	{
		case Secrypto_NoError :
			break;
		case Error_CommunicationKey :
			printf("get communication key fail.\n");
			return false;
			break;
		case Error_PublicKey :
			printf("get public key fail.\n");
			return false;
			break;
		case Error_Expire :
			printf("the access token provided is expired.\n");
			return false;
			break;
		case Error_NoJson :
			printf("no json object could be decoded.\n");
			return false;
			break;
		case Error_WrongRequest :
			printf("wrong request.\n");
			return false;
			break;
		case Error_Invalid :
			printf("invalid access token.\n");
			return false;
			break;
		default :
			printf("noknown error number.\n");
			return false;
			break;
	}
	return true;
}

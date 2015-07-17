/////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: parser.cpp
//
// Description:
//
// Created: 2015年04月29日 星期三 17时30分09秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "parser.h"
#include "json/json.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

// Static Interface
bool Parser::genTaskStr(std::string &queryStr, std::map<std::string,std::string> &taskMap)
{
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	std::map<std::string,std::string>::iterator it = taskMap.begin();
	for(; it != taskMap.end(); it++)
	{
		item["uuid"] = it->first;
		item["status"] = it->second;
		arrayObj.append(item);
	}

	root["list"] = arrayObj;
	queryStr = root.toStyledString();

	return true;
}

// Protected Interface
bool Parser::generateMergeFile(const std::list<FileAttrStruct> &tmpList, std::string &postField)
{
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	std::list<FileAttrStruct>::const_iterator it = tmpList.begin();
	for(; it != tmpList.end(); it++)
	{
		item["path"] = it->path;
		item["etag"] = it->etag;
		item["size_types"] = it->sizebytes;
		arrayObj.append(item);
	}

	//root["list"] = arrayObj;
	//postField = root.toStyledString();
	postField = arrayObj.toStyledString();

	return true;
}

bool Parser::genBatchDelFileDir(const std::list<FileAttrStruct> &tmpList, std::string &postField)
{
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	std::list<FileAttrStruct>::const_iterator it = tmpList.begin();
	for(; it != tmpList.end(); it++)
	{
		item["path"] = it->path;
		item["ftype"] = it->ftype;
		arrayObj.append(item);
	}

	root["list"] = arrayObj;
	postField = root.toStyledString();

	return true;
}

bool Parser::generateMoveRecycle(const std::list<FileAttrStruct> &tmpList, std::string &postField)
{
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	std::list<FileAttrStruct>::const_iterator it = tmpList.begin();
	for(; it != tmpList.end(); it++)
	{
		item["uuid"] = it->uuid;
		item["path"] = it->path;
		item["ftype"] = it->ftype;
		arrayObj.append(item);
	}

	root["list"] = arrayObj;
	postField = root.toStyledString();

	return true;
}

bool Parser::genBatchMoveFileDir(const std::list<MoveFileDirStruct> &tmpList, std::string &postField)
{
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	std::list<MoveFileDirStruct>::const_iterator it = tmpList.begin();
	for(; it != tmpList.end(); it++)
	{
		item["from"] = it->from;
		item["to"] = it->to;
		item["ftype"] = it->ftype;
		arrayObj.append(item);
	}

	root["list"] = arrayObj;
	postField = root.toStyledString();

	return true;
}

bool Parser::genBatchCopyFileDir(const std::list<MoveFileDirStruct> &tmpList, std::string &postField)
{
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	std::list<MoveFileDirStruct>::const_iterator it = tmpList.begin();
	for(; it != tmpList.end(); it++)
	{
		item["from"] = it->from;
		item["to"] = it->to;
		item["ftype"] = it->ftype;
		arrayObj.append(item);
	}

	root["list"] = arrayObj;
	postField = root.toStyledString();

	return true;
}

bool Parser::generate(const std::string &username, const std::string &passwd, const std::string &confirm, const std::string &email, const std::string &name, std::string &postField)
{
	Json::Value root;

	root["username"] = username;
	root["password_confirmation"] = confirm;
	root["password"] = passwd;
	root["email"] = email;
	root["name"] = name;
	
	postField = root.toStyledString();

	return true;
}

// Public Interface
std::string Parser::parserKey(const std::string &strJson, const std::string &key)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	std::string val = value[key.c_str()].asString();
	return val;
}

bool Parser::parseValidate(std::string &email, std::string &passwd, std::string uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	email = value["email"].asString();
	passwd = value["password"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseContainerList(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseCreateContainer(std::string &token, std::string &uuid, std::string &container, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	container = value["Container-Name"].asString();

	return true;
}

bool Parser::parseDeleteContainer(std::string &token, std::string &uuid, std::string &container, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	container = value["Container-Name"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseListContainerObjects(std::string &token, std::string &uuid, std::string &container, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	container = value["Container-Name"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseCreateObject(std::string &token, std::string &uuid, std::string &local, std::string &container, std::string &obj, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	local = value["Local-Object"].asString();
	uuid = value["uuid"].asString();
	container = value["Container-Name"].asString();
	obj = value["Object-Name"].asString();

	return true;
}

bool Parser::parseDeleteObject(std::string &token, std::string &uuid, std::string &container, std::string &obj, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	container = value["Container-Name"].asString();
	uuid = value["uuid"].asString();
	obj = value["Object-Name"].asString();

	return true;
}

bool Parser::parseCopy(std::string &token, std::string &uuid, std::string &containerName, std::string &objectName, std::string &destination, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	containerName = value["Container-Name"].asString();
	destination = value["Destination"].asString();
	objectName = value["Object-Name"].asString();

	return true;
}

bool Parser::parseReadObject(std::string &token, std::string &uuid, std::string &local, std::string &container, std::string &obj, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	local = value["Local-Object"].asString();
	uuid = value["uuid"].asString();
	container = value["Container-Name"].asString();
	obj = value["Object-Name"].asString();

	return true;
}

bool Parser::parseGetQuotaInfo(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseFileUpload(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	src = value["Source"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseMergeFile(std::string &token, std::string &uuid, std::string &dest, std::string &postField, const std::string &strJson)
{
	std::list<FileAttrStruct> tmpList;

	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	dest = value["Destination"].asString();

	const Json::Value arrayObj = value["Merge-Info"];
	for(int i=0; i<arrayObj.size(); i++)
	{
		FileAttrStruct tmpStruct;
		tmpStruct.path = arrayObj[i]["path"].asString();
		tmpStruct.etag = arrayObj[i]["etag"].asString();
		tmpStruct.sizebytes = arrayObj[i]["size_bytes"].asInt();

		tmpList.push_back(tmpStruct);
	}

	generateMergeFile(tmpList, postField);

	return true;
}

bool Parser::parseCreateDir(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseDeleteFileDir(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseBatchDeleteFileDir(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson)
{
	std::list<FileAttrStruct> tmpList;

	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	const Json::Value arrayObj = value["list"];
	for(int i=0; i<arrayObj.size(); i++)
	{
		FileAttrStruct tmpStruct;
		tmpStruct.path = arrayObj[i]["path"].asString();
		tmpStruct.ftype = arrayObj[i]["ftype"].asString();

		tmpList.push_back(tmpStruct);
	}

	genBatchDelFileDir(tmpList, postField);

	return true;
}

bool Parser::parseMoveFileDir(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	src = value["Source"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseBatchMoveFileDir(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson)
{
	std::list<MoveFileDirStruct> tmpList;

	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	const Json::Value arrayObj = value["list"];
	for(int i=0; i<arrayObj.size(); i++)
	{
		MoveFileDirStruct tmpStruct;
		tmpStruct.from = arrayObj[i]["from"].asString();
		tmpStruct.to = arrayObj[i]["to"].asString();
		tmpStruct.ftype = arrayObj[i]["ftype"].asString();

		tmpList.push_back(tmpStruct);
	}

	genBatchMoveFileDir(tmpList, postField);

	return true;
}

bool Parser::parseCopyFileDir(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	src = value["Source"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseBatchCopyFileDir(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson)
{
	std::list<MoveFileDirStruct> tmpList;

	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	const Json::Value arrayObj = value["list"];
	for(int i=0; i<arrayObj.size(); i++)
	{
		MoveFileDirStruct tmpStruct;
		tmpStruct.from = arrayObj[i]["from"].asString();
		tmpStruct.to = arrayObj[i]["to"].asString();
		tmpStruct.ftype = arrayObj[i]["ftype"].asString();

		tmpList.push_back(tmpStruct);
	}

	genBatchCopyFileDir(tmpList, postField);

	return true;
}

bool Parser::parseReadFile(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	src = value["Source"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseGetFileHistory(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseGetOperateHistory(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseDeleteOperateHistory(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseCreateSymbolicLink(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	dst = value["Source"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseRenameFileDir(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	dst = value["Source"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseGetFileAttribute(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	dst = value["Destination"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseGetRecycleList(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseMoveRecycle(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson)
{
	std::list<FileAttrStruct> tmpList;

	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	const Json::Value arrayObj = value["list"];
	for(int i=0; i<arrayObj.size(); i++)
	{
		FileAttrStruct tmpStruct;

		tmpStruct.uuid = arrayObj[i]["uuid"].asString();
		tmpStruct.path = arrayObj[i]["path"].asString();
		tmpStruct.ftype = arrayObj[i]["ftype"].asString();

		tmpList.push_back(tmpStruct);
	}

	generateMoveRecycle(tmpList, postField);

	return true;
}

bool Parser::parseCleanRecycle(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseSetPermission(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseGetFileList(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	dst = value["Destination"].asString();

	return true;
}

bool Parser::parseCreateStorageQuota(std::string &token, std::string &uuid, std::string &meta, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();
	meta = value["X-Account-Meta-Quota-Bytes"].asString();

	return true;
}

bool Parser::parseUserRegister(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseRefreshToken(std::string &token, std::string &passwd, std::string &email, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	passwd = value["password"].asString();
	email = value["email"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseGetCloudServVersion(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseVerifyToken(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}

bool Parser::parseNetworkRegister(std::string &postField, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	uuid = value["uuid"].asString();

	std::string username, confirm, passwd, email, name;
	username = value["username"].asString();
	passwd = value["password"].asString();
	confirm = value["password_confirmation"].asString();
	email = value["email"].asString();
	name = value["name"].asString();
	generate(username,passwd,confirm,email,name,postField);

	return true;
}

bool Parser::parseNetCloudRegister(std::string &postField, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	uuid = value["uuid"].asString();

	std::string username, confirm, passwd, email, name;
	username = value["username"].asString();
	passwd = value["password"].asString();
	confirm = value["password_confirmation"].asString();
	email = value["email"].asString();
	name = value["name"].asString();
	generate(username,passwd,confirm,email,name,postField);

	return true;
}

bool Parser::parseValidateNetworkAccout(std::string &token, std::string &uuid, const std::string &strJson)
{
	Json::Reader reader;
	Json::Value value;

	reader.parse(strJson, value);
	token = value["X-Auth-Token"].asString();
	uuid = value["uuid"].asString();

	return true;
}


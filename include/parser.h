///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: parser.h
//
// Description:
//
// Created: 2015年04月29日 星期三 16时30分42秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <list>
#include <map>

typedef struct MoveFileDirStructTag
{
	MoveFileDirStructTag()
	{
		from.clear();
		to.clear();
		ftype.clear();
	};
	std::string from;
	std::string to;
	std::string ftype;
}MoveFileDirStruct;

typedef struct FileAttrStructTag
{
	FileAttrStructTag()
	{
		path.clear();
		etag.clear();
		sizebytes = 0;
		ftype.clear();
		uuid.clear();
	};
	std::string path;
	std::string etag;
	int sizebytes;
	std::string ftype;
	std::string uuid;
}FileAttrStruct;

class Parser{
	public:
		Parser();
		~Parser();
	
	public:
		std::string parserKey(const std::string &strJson, const std::string &key);
		//
		bool parseValidate(std::string &email, std::string &passwd, std::string uuid, const std::string &strJson);

		bool parseContainerList(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseCreateContainer(std::string &token, std::string &uuid, std::string &container, const std::string &strJson);

		bool parseDeleteContainer(std::string &token, std::string &uuid, std::string &container, const std::string &strJson);

		bool parseListContainerObjects(std::string &token, std::string &uuid, std::string &container, const std::string &strJson);

		bool parseCreateObject(std::string &token, std::string &uuid, std::string &local, std::string &container, std::string &obj, const std::string &strJson);

		bool parseDeleteObject(std::string &token, std::string &uuid, std::string &container, std::string &obj, const std::string &strJson);

		bool parseCopy(std::string &token, std::string &uuid, std::string &containerName, std::string &objectName, std::string &destination, const std::string &strJson);

		bool parseReadObject(std::string &token, std::string &uuid, std::string &local, std::string &container, std::string &obj, const std::string &strJson);

		bool parseGetQuotaInfo(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseFileUpload(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson);

		bool parseMergeFile(std::string &token, std::string &uuid, std::string &dest, std::string &postField, const std::string &strJson);

		bool parseCreateDir(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseDeleteFileDir(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseBatchDeleteFileDir(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson);

		bool parseMoveFileDir(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson);

		bool parseBatchMoveFileDir(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson);

		bool parseCopyFileDir(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson);

		bool parseBatchCopyFileDir(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson);

		bool parseReadFile(std::string &token, std::string &uuid, std::string &src, std::string &dst, const std::string &strJson);

		bool parseGetFileHistory(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseGetOperateHistory(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseDeleteOperateHistory(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseCreateSymbolicLink(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseRenameFileDir(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseGetFileAttribute(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseGetRecycleList(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseMoveRecycle(std::string &token, std::string &uuid, std::string &postField, const std::string &strJson);

		bool parseCleanRecycle(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseSetPermission(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseGetFileList(std::string &token, std::string &uuid, std::string &dst, const std::string &strJson);

		bool parseCreateStorageQuota(std::string &token, std::string &uuid, std::string &meta, const std::string &strJson);

		bool parseUserRegister(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseRefreshToken(std::string &token, std::string &passwd, std::string &email, std::string &uuid, const std::string &strJson);

		bool parseGetCloudServVersion(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseVerifyToken(std::string &token, std::string &uuid, const std::string &strJson);

		bool parseNetworkRegister(std::string &postField, std::string &uuid, const std::string &strJson);

		bool parseNetCloudRegister(std::string &postField, std::string &uuid, const std::string &strJson);

		bool parseValidateNetworkAccout(std::string &token, std::string &uuid, const std::string &strJson);

	protected:
		bool generateMergeFile(const std::list<FileAttrStruct> &tmpList, std::string &postField);

		bool genBatchDelFileDir(const std::list<FileAttrStruct> &tmpList, std::string &postField);

		bool genBatchMoveFileDir(const std::list<MoveFileDirStruct> &tmpList, std::string &postField);

		bool genBatchCopyFileDir(const std::list<MoveFileDirStruct> &tmpList, std::string &postField);

		bool generateMoveRecycle(const std::list<FileAttrStruct> &tmpList, std::string &postField);

		bool generate(const std::string &username, const std::string &passwd, const std::string &confirm, const std::string &email, const std::string &name, std::string &postField);

};

#endif //__PARSER_H__

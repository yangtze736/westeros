///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: generateUrl.h
//
// Description:
//
// Created: 2015年06月11日 星期四 10时49分02秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __GENERATEURL_H__
#define __GENERATEURL_H__

#include "middleware.h"
#include <string>

class GenerateUrl{
	public:
		GenerateUrl();
		~GenerateUrl();

	public:
		static std::string genValidate(const std::string &strIpPort);

		static std::string genContainerList(const std::string &strIpPort, const std::string &version, const std::string &tenant);

		static std::string genCreateContainer(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container);

		static std::string genDeleteContainer(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container);

		static std::string genListContainerObjects(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, ParameterStruct &paraStruct);

		static std::string genCreateObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName);

		static std::string genDeleteObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName);

		static std::string genCopyObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName);

		static std::string genReadObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName);

		static std::string genGetQuotaInfo(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genFileUpload(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genMergeFile(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genCreateDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genDeleteFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genBatchDeleteFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genMoveFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &src, ParameterStruct &paraStruct);

		static std::string genBatchMoveFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genCopyFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &src, ParameterStruct &paraStruct);

		static std::string genBatchCopyFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genReadFile(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genGetFileHistory(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genGetOperateHistory(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genDelOperHistory(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genCreateSymbolicLink(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genRenameFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genGetFileAttribute(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genGetRecycleList(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genMoveRecycle(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genCleanRecycle(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct);

		static std::string genSetPermission(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genGetFileList(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct);

		static std::string genCreateStorageQuota(const std::string &strIpPort, const std::string &version, const std::string &tenant);

		static std::string genUserRegister(const std::string &strIpPort, const std::string &version, const std::string &tenant);

		static std::string genRefreshToken(const std::string &strIpPort, const std::string &version);

		static std::string genGetCloudServVersion(const std::string &strIpPort, const std::string &version, const std::string &tenant);

		static std::string genVerifyToken(const std::string &strIpPort);

		static std::string genNetworkRegister(const std::string &strIpPort, const std::string &version);

		static std::string genNetCloudRegister(const std::string &strIpPort, const std::string &version);

		static std::string genValNetAccout(const std::string &strIpPort, const std::string &version);

	private:
		static bool generateUrlEnd(std::string &strUrl, ParameterStruct &paraStruct);

};

#endif

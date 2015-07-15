///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: ../src/generateUrl.cpp
//
// Description:
//
// Created: 2015年06月11日 星期四 10时52分57秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "generateUrl.h"

GenerateUrl::GenerateUrl()
{
}

GenerateUrl::~GenerateUrl()
{
}

bool GenerateUrl::generateUrlEnd(std::string &strUrl, ParameterStruct &paraStruct)
{
	// When parameter change, modify here
	//strUrl.append(paraStruct.op.empty() ? "?op=CREATE" : ("?op=" + paraStruct.op));
	strUrl.append(paraStruct.op.empty() ? "?" : ("?op=" + paraStruct.op));
	strUrl.append(paraStruct.offset.empty() ? "" : ("&offset=" + paraStruct.offset));
	strUrl.append(paraStruct.length.empty() ? "" : ("&length=" + paraStruct.length));
	strUrl.append(paraStruct.destination.empty() ? "" : ("&destination=" + paraStruct.destination));
	strUrl.append(paraStruct.ftype.empty() ? "" : ("&ftype=" + paraStruct.ftype));
	strUrl.append(paraStruct.recursive.empty() ? "" : ("&recursive=" + paraStruct.recursive));
	strUrl.append(paraStruct.overwrite.empty() ? "" : ("&overwrite=" + paraStruct.overwrite));
	strUrl.append(paraStruct.permission.empty() ? "" : ("&permission=" + paraStruct.permission));
	strUrl.append(paraStruct.type.empty() ? "" : ("&type=" + paraStruct.type));
	strUrl.append(paraStruct.metadata.empty() ? "" : ("&metadata=" + paraStruct.metadata));
	strUrl.append(paraStruct.version.empty() ? "" : ("&version=" + paraStruct.version));
	strUrl.append(paraStruct.recent.empty() ? "" : ("&recent=" + paraStruct.recent));
	strUrl.append(paraStruct.mode.empty() ? "" : ("&mode=" + paraStruct.mode));
	strUrl.append(paraStruct.storetype.empty() ? "" : ("&storetype=" + paraStruct.storetype));
	//strUrl.append(paraStruct.multipart_manifest.empty() ? "" : ("&multipart-manifest" + paraStruct.multipart_manifest));
	strUrl.append(paraStruct.multipart_manifest.empty() ? "" : ("multipart-manifest=" + paraStruct.multipart_manifest));

	return true;
}

std::string GenerateUrl::genValidate(const std::string &strIpPort)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append("/oauth/access_token");

	return strUrl;
}

std::string GenerateUrl::genContainerList(const std::string &strIpPort, const std::string &version, const std::string &tenant)
{
	std::string strUrl;
	
	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);

	return strUrl;
}

std::string GenerateUrl::genCreateContainer(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container)
{
	std::string strUrl;
	
	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/");
	strUrl.append(container);

	return strUrl;
}

std::string GenerateUrl::genDeleteContainer(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container)
{
	std::string strUrl;
	
	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/");
	strUrl.append(container);

	return strUrl;
}

std::string GenerateUrl::genListContainerObjects(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/");
	strUrl.append(container);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genCreateObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/");
	strUrl.append(container);
	strUrl.append("/");
	strUrl.append(objectName);

	return strUrl;
}

std::string GenerateUrl::genDeleteObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/");
	strUrl.append(container);
	strUrl.append("/");
	strUrl.append(objectName);

	return strUrl;
}

std::string GenerateUrl::genCopyObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/");
	strUrl.append(container);
	strUrl.append("/");
	strUrl.append(objectName);

	return strUrl;
}

std::string GenerateUrl::genReadObject(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &container, const std::string &objectName)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/");
	strUrl.append(container);
	strUrl.append("/");
	strUrl.append(objectName);

	return strUrl;
}

std::string GenerateUrl::genGetQuotaInfo(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/quota");

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genFileUpload(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genMergeFile(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genCreateDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genDeleteFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genBatchDeleteFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/batch");

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genMoveFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &src, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(src);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genBatchMoveFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/batch");

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genCopyFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &src, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(src);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genBatchCopyFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/batch");

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genReadFile(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genGetFileHistory(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genGetOperateHistory(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genDelOperHistory(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genCreateSymbolicLink(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genRenameFileDir(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genGetFileAttribute(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genGetRecycleList(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/recycle/user");

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genMoveRecycle(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/batch");

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genCleanRecycle(const std::string &strIpPort, const std::string &version, const std::string &tenant, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/clearrecycle");

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genSetPermission(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genGetFileList(const std::string &strIpPort, const std::string &version, const std::string &tenant, const std::string &dst, ParameterStruct &paraStruct)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append(dst);

	generateUrlEnd(strUrl, paraStruct);

	return strUrl;
}

std::string GenerateUrl::genCreateStorageQuota(const std::string &strIpPort, const std::string &version, const std::string &tenant)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/quota?op=createstorage");

	return strUrl;
}

std::string GenerateUrl::genUserRegister(const std::string &strIpPort, const std::string &version, const std::string &tenant)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/register");

	return strUrl;
}

std::string GenerateUrl::genRefreshToken(const std::string &strIpPort, const std::string &version)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	//strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("/oauth/token_refresh");

	return strUrl;
}

std::string GenerateUrl::genGetCloudServVersion(const std::string &strIpPort, const std::string &version, const std::string &tenant)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("AUTH_");
	strUrl.append(tenant.empty() ? "" : tenant);
	strUrl.append("/cloud_server_version");

	return strUrl;
}

std::string GenerateUrl::genVerifyToken(const std::string &strIpPort)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append("/oauth/verify_token");

	return strUrl;
}

std::string GenerateUrl::genNetworkRegister(const std::string &strIpPort, const std::string &version)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("network_register");

	return strUrl;
}

std::string GenerateUrl::genNetCloudRegister(const std::string &strIpPort, const std::string &version)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("user_register");

	return strUrl;
}

std::string GenerateUrl::genValNetAccout(const std::string &strIpPort, const std::string &version)
{
	std::string strUrl;

	strUrl.append(1 ? "https://" : "http://");
	strUrl.append(strIpPort.empty() ? "" : strIpPort);
	strUrl.append(version.empty() ? "/v1/" : version);
	strUrl.append("token_validation");

	return strUrl;
}

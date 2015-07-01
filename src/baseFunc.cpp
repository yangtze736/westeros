///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: baseFunc.cpp
//
// Description:
//
// Created: 2015年06月09日 星期二 16时10分02秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "baseFunc.h"
#include "urlhandle.h"
#include <string.h>

std::string itoa(short i)
{
	char buf[256] = {0};
	sprintf(buf, "%d", i);

	return std::string(buf);
}

std::string itoa(long i)
{
	char buf[256] = {0};
	sprintf(buf, "%ld", i);

	return std::string(buf);
}

std::string itoa(long long i)
{
	char buf[256] = {0};
	sprintf(buf, "%lld", i);

	return std::string(buf);
}

std::string itoa(unsigned i)
{
	char buf[256] ={0};
	sprintf(buf, "%u", i);

	return std::string(buf);
}

// long to string (with format)
// return value: string
std::string itoa(
		long  in_number,
		long  in_len,
		short in_fill,
		short in_alignment,
		char  in_fillChar
		)
{
	char buf[256] = {0};
	sprintf(buf, "%ld", in_number);
	std::string ret(buf);
	char format[256];

	if(in_fill)
		memset(format, in_fillChar, sizeof(format));
	else
		memset(format, ' ', sizeof(format));

	long i = ret.length();
	if(in_alignment == 1)
	{
		if(i < in_len)
			ret.insert(0, format, in_len-1);
		else if(i > in_len)
			ret.erase(0, i-in_len);
	}
	else
	{
		if(i < in_len)
			ret.insert(i, format, in_len-i);
		else if(i > in_len)
			ret.erase(in_len, i-in_len);
	}
	return ret;
}

std::string urlEncode(const std::string &strUrl)
{
	char buff[256] = {0};
	URL::UrlEncode(buff, strUrl.c_str(), sizeof(buff));

	return std::string(buff);
}

std::string urlDecode(const std::string &strUrl)
{
	char buff[256] = {0};
	URL::UrlDecode(buff, strUrl.c_str(), sizeof(buff));

	return std::string(buff);
}

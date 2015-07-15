///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: baseFunc.h
//
// Description:
//
// Created: 2015年06月09日 星期二 16时10分52秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __BASEFUNC_H__
#define __BASEFUNC_H__

#include <string>
#include <sstream>

template<class DataType>
inline std::string toStr(DataType data)
{
	if(data == DataType(-1))
		return "-1";

	std::ostringstream ostr;
	ostr << data;
	return ostr.str();
}

template<>
inline std::string toStr<bool>(bool data)
{
	if(data)
		return "1";
	else
		return "0";
}

template<>
inline std::string toStr<char *>(char *data)
{
	return data;
}

template<>
inline std::string toStr<std::string>(std::string data)
{
	return data;
}

std::string itoa(short i);
std::string itoa(long i);
std::string itoa(long long i);
std::string itoa(unsigned i);

std::string itoa(
		long    in_number,
		long    in_len,
		short   in_fill=0,
		short   in_alignment=0,
		char    in_fillChar=' '
		);

std::string urlEncode(const std::string &strUrl);

std::string urlDecode(const std::string &strUrl);

#endif

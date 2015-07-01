///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: ../include/sqliteWrap.h
//
// Description:
//
// Created: 2015年06月16日 星期二 10时24分23秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __SQLITEWRAP_H__
#define __SQLITEWRAP_H__

#include <sqlite3.h>
#include <string>

class SqliteWrap{
	public:
		SqliteWrap();
		~SqliteWrap();

	public:
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	public:
		bool createTable(const std::string &table);
		bool insert(const std::string &table, const std::string &sequence, int ulTotal, int ulNow);
		bool select(const std::string &table);
		bool update(const std::string &table, const std::string &key, int ulNow);

	private:
		int     m_rcOpen;

		sqlite3 *m_db;
		char    *m_zErrMsg;

		std::string m_sql;
};

#endif

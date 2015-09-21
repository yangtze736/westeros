///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: trap.h
//
// Description:
//
// Created: 2015年07月23日 星期四 09时38分09秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#ifndef __TRAP_H__
#define __TRAP_H__

#include <stdio.h>
#include <pthread.h>

// TODO
#ifdef RELEASE
#define PR_LOG(fmt, args...) \
	do{ \
	}while(0)
#define PR(fmt, args...) PR_LOG(fmt, ##args)
#endif

#ifdef DEBUG
#define PR_LOG(fmt, args...) \
	do{ \
		Logger::PrintLog(__FILE__, __LINE__, __func__, fmt, ##args); \
	}while(0)
#define PR(fmt, args...) PR_LOG(fmt, ##args)
#endif

class Logger
{
	protected:
		Logger();
		~Logger();

		int do_printlog(const char *filename, int linenum, const char *funcname, const char *message);

	public:
		static int PrintLog(const char *filename, int linenum, const char *funcname, const char *fmt, ...);

	protected:
		static Logger m_logger;
		pthread_spinlock_t m_lock;

};

#endif // __TRAP_H__

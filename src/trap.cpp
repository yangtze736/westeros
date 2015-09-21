///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: trap.cpp
//
// Description:
//
// Created: 2015年07月23日 星期四 10时23分32秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "trap.h"
#include <stdarg.h>
#include <sys/time.h>

static inline void g_m_localtime_r(time_t *tsec, struct tm *mytm)
{
	static struct tm s_lasttm = {0};
	time_t s_tsec = 0;

	if(*tsec - s_tsec < 60 - s_lasttm.tm_sec)
	{
		mytm->tm_sec = s_lasttm.tm_sec + (*tsec) - s_tsec;
	}
	else
	{
		s_tsec = *tsec;
		localtime_r(&s_tsec, &s_lasttm);
		mytm->tm_sec = s_lasttm.tm_sec;
	}

	mytm->tm_year = s_lasttm.tm_year;
	mytm->tm_mon = s_lasttm.tm_mon;
	mytm->tm_mday = s_lasttm.tm_mday;
	mytm->tm_hour = s_lasttm.tm_hour;
	mytm->tm_min = s_lasttm.tm_min;

	return;
}

Logger Logger::m_logger;

int Logger::PrintLog(const char *filename, int linenum, const char *funcname, const char *fmt, ...)
{
	char tmpbuf[1024];
	int count = 0;

	va_list ap;
	va_start(ap, fmt);
	count = vsnprintf(tmpbuf, sizeof(tmpbuf), fmt, ap);
	va_end(ap);

	if(count <= 0)
		return count;

	return m_logger.do_printlog(filename, linenum, funcname, tmpbuf);
}

Logger::Logger()
{
	//pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
}

Logger::~Logger()
{
	//pthread_spin_destroy(&m_lock);
}

int Logger::do_printlog(const char *filename, int linenum, const char *funcname, const char *message)
{
	const char *my_filename = (filename == NULL) ? "(null)" : filename;
	const char *my_funcname = (funcname == NULL) ? "(null)" : funcname;

	struct timeval tv;
	time_t tsec;
	struct tm mytm;
	gettimeofday(&tv, NULL);
	tsec = tv.tv_sec;
	//pthread_spin_lock(&m_lock);
	g_m_localtime_r(&tsec, &mytm);
	//pthread_spin_unlock(&m_lock);

	// need bigger buf for more info
	char buf[10*1024];
	int len = snprintf(buf, sizeof(buf), "[%04d%02d%02d %02d:%02d:%02d] [%s:%d:%s] [%s] %s\n",
			mytm.tm_year + 1900, mytm.tm_mon + 1, mytm.tm_mday, mytm.tm_hour, mytm.tm_min, mytm.tm_sec,
			my_filename, linenum, my_funcname, "DEBUG", message );

	if( len < 0 ) 
		return len;

	// print to STDOUT
	fprintf(stdout, "%s", buf);

	return 0;
}

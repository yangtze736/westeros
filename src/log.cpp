///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: log.cpp
//
// Description:
//
// Created: 2015年05月20日 星期三 10时41分43秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "log.h"
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <syslog.h>
#include <linux/unistd.h>
//#include <execinfo.h>


static inline void g_my_localtime_r(time_t *tsec, struct tm *mytm)
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


/////////////////////////////////
MfcLogger MfcLogger::ms_logger;

// static func
int MfcLogger::Init(int loglevel, int logoutput)
{
	return ms_logger.do_init(loglevel, logoutput);
}

int MfcLogger::SetLevel(int loglevel)
{
	return ms_logger.do_setlevel(loglevel);
}

int MfcLogger::SetFile(const char *fname)
{
	return ms_logger.do_setfile(fname);
}

int MfcLogger::SetSyslog(int logfacility)
{
	return ms_logger.do_setsyslog(logfacility);
}

int MfcLogger::SetUdp(const char *svraddr, int port)
{
	return ms_logger.do_setudp(svraddr, port);
}

int MfcLogger::SetProg(const char *progname)
{
	return ms_logger.do_setprog(progname);
}

int MfcLogger::PrintLog(int loglevel, const char *filename, int linenum, const char *funcname, const char *fmt, ...)
{
	if((loglevel < LOG_LEVEL_CRIT) || (loglevel > LOG_LEVEL_TRACE))
		return 0;

	char tmpbuf[LOG_MAX_MSG_SIZE];
	int count = 0;

	va_list ap;
	va_start(ap, fmt);
	count = vsnprintf(tmpbuf, sizeof(tmpbuf), fmt, ap);
	va_end(ap);

	if(count <= 0)
		return count;

	return ms_logger.do_printlog_internal(loglevel, filename, linenum, funcname, tmpbuf);
}

/////////////////////////////////
// MfcLogger implement
MfcLogger::MfcLogger()
{
	m_loglevel = LOG_LEVEL_NOTICE;		// default level
	m_logoutput = LOG_OUTPUT_SYSLOG;	// default output
	m_file_fp = NULL;
	m_syslog_facility = -1;
	m_udp_fd = -1;
	m_udp_facility = -1;
	m_udp_severity = LOG_LEVEL_NOTICE;

	m_my_ip_addr = 0;
	memset(m_hostname, 0, sizeof(m_hostname));
	memset(m_progname, 0, sizeof(m_progname));
	pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
}

MfcLogger::~MfcLogger()
{
	if(m_file_fp != NULL)
		::fclose(m_file_fp), m_file_fp = NULL;

	if(m_udp_fd >= 0)
		::close(m_udp_fd), m_udp_fd = -1;

	pthread_spin_destroy(&m_lock);
}

int MfcLogger::do_init(int loglevel, int logoutput)
{
	if(loglevel >= LOG_LEVEL_CRIT && loglevel <= LOG_LEVEL_TRACE)
		m_loglevel = loglevel;
	m_logoutput = logoutput;

	return 0;
}

int MfcLogger::do_setlevel(int loglevel)
{
	if(loglevel >= LOG_LEVEL_CRIT && loglevel <= LOG_LEVEL_TRACE)
		m_loglevel = loglevel;

	return 0;
}

int MfcLogger::do_setfile(const char *fname)
{
	m_logoutput &= ~LOG_OUTPUT_FILE;
	if(m_file_fp != NULL)
		::fclose(m_file_fp), m_file_fp = NULL;
	
	if(fname == NULL || fname[0] == '\0')
		return -1;

	FILE *fp = NULL;
	if((fp = do_fopen(fname)) == NULL)
		return -1;

	m_file_fp = fp;
	m_logoutput |= LOG_OUTPUT_FILE;

	return 0;
}

FILE *MfcLogger::do_fopen(const char *fname) 
{
	FILE *fp = NULL;
	fp = ::fopen(fname, "a");
	if(fp == NULL)
	{
		char name[1024] = {0};
		do_reg_path(name,fname);

		char *p = NULL;
		p = strrchr(name, '/');
		if(p != NULL)
		{
			*p = '\0';
			if(do_mkdir(name) == -1)
				return NULL;
			*p = '/';
		}

		fp = ::fopen(name, "a");
	}

	return fp;
}

int MfcLogger::do_reg_path(char *name, const char *fname)
{
	if(fname[0] == '/' || (fname[0] == '.' && fname[1] == '/'))
	{
		snprintf(name, 1024, "%s", fname);
	}
	else
	{
		snprintf(name, 1024, "./%s", fname);
	}

	if(name[strlen(name)-1] == '/')
	{
		strcat(name, "UnnamedMiddlewareBase.log");
	}

	int index = 0;
	int len = strlen(name);
	for(int i = 0; i < len; i++)
	{
		name[index++] = name[i];
		if(name[i] == '/')
		{
			while(name[i+1] == '/') i++;
		}
	}
	name[index] = '\0';

	return 0;
}

int MfcLogger::do_mkdir(char *name)
{
	struct stat fstat;
	memset(&fstat, 0, sizeof(fstat));
	if(stat(name, &fstat) == 0 && S_ISDIR(fstat.st_mode))
		return 0;

	char *p = NULL;
	p = strrchr(name, '/');

	if(strchr(name, '/') != p)
	{
		*p = '\0';
		if(do_mkdir(name) == -1) return -1;
		*p = '/';
	}

	return mkdir(name, 0755);
}

int MfcLogger::do_setsyslog(int logfacility)
{
	m_logoutput &= ~LOG_OUTPUT_SYSLOG;
	if(logfacility < 0 || logfacility > 7)
		return -1;

	m_syslog_facility = logfacility;
	m_logoutput |= LOG_OUTPUT_SYSLOG;

	return 0;
}

int MfcLogger::do_setudp(const char *svraddr, int port)
{
	m_logoutput &= ~LOG_OUTPUT_UDP;
	if(m_udp_fd >= 0)
		::close(m_udp_fd), m_udp_fd = -1;

	if(svraddr == NULL && svraddr[0] == '\0' && port <= 0)
		return -1;

	int sockfd;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	// FIXME : domain name, ont only ip addr
	addr.sin_addr.s_addr = ::inet_addr(svraddr);
	addr.sin_port = htons((short)port);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(::connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		::close(sockfd);
		sockfd = -1;
		return -1;
	}

	struct sockaddr_in myaddr;
	socklen_t mylen = sizeof(myaddr);
	if(getsockname(sockfd, (struct sockaddr *)&myaddr, &mylen) >= 0)
	{
		m_my_ip_addr = myaddr.sin_addr.s_addr; // network byte order
	}

	m_udp_fd = sockfd;
	m_logoutput |= LOG_OUTPUT_UDP;

	if(gethostname(m_hostname, sizeof(m_hostname)) < 0)
	{
		memmove(m_hostname, inet_ntoa(myaddr.sin_addr), sizeof(m_hostname));
	}

	return 0;
}

int MfcLogger::do_setprog(const char *progname)
{
	memmove(m_progname, progname, sizeof(m_progname));
	return 0;
}

// internal implement func
int MfcLogger::do_printlog_internal(int loglevel, const char *filename, int linenum, const char *funcname, const char *message)
{
	static const char *s_level_str[] = {"null", "CRIT", \
		"ERR", "WARN", "NOTICE", "INFO", "DEBUG", "TRACE"};

	const char *my_filename = (filename == NULL) ? "(null)" : filename;
	const char *my_funcname = (funcname == NULL) ? "(null)" : funcname;

	struct timeval tv;
	time_t tsec;
	struct tm mytm;
	gettimeofday(&tv, NULL);
	tsec = tv.tv_sec;
	pthread_spin_lock(&m_lock);
	g_my_localtime_r(&tsec, &mytm);
	pthread_spin_unlock(&m_lock);

	if((m_logoutput & LOG_OUTPUT_UDP) && m_udp_fd >= 0)
	{
		int len = 0;
		char buf[1024];

		len = snprintf(buf, sizeof(buf), "<%d>", loglevel);
		len += strftime(buf+len,sizeof(buf)-len, "%h %e %T", &mytm);
		len += snprintf(buf+len, sizeof(buf)-len, "%s %s: [%2d.%d06d] [%s:%d:%s] [%s] %s\n",
				m_hostname, m_progname, mytm.tm_sec, (int)tv.tv_usec, my_filename, linenum, my_funcname, s_level_str[loglevel], message);

		if(len < 0)
			return len;

		// send len must <= 1024, RFC3164
		if(len > 1024)
			len = 1024;
		
		send(m_udp_fd, buf, len, 0);
	}
	
	// need bigger buf for more info
	char buf[10*1024];
	int len = snprintf(buf, sizeof(buf), "[%04d%02d%02d %02d:%02d:%02d.%06d] [%s:%d:%s] [%s] %s\n",
			mytm.tm_year + 1900, mytm.tm_mon + 1, mytm.tm_mday, mytm.tm_hour, mytm.tm_min, mytm.tm_sec,
			(int)tv.tv_usec, my_filename, linenum, my_funcname, s_level_str[loglevel], message );

	if( len < 0 ) 
		return len; 


	// print to STDERR
	if( m_logoutput & LOG_OUTPUT_STDERR ) {
		fprintf( stderr, "%s", buf );
	}

	// print to FILE
	if( (m_logoutput & LOG_OUTPUT_FILE) && m_file_fp != NULL ) {
		fprintf( m_file_fp, "%s", buf );
		fflush( m_file_fp );
	}

	// print to SYSLOG
	if(m_logoutput & LOG_OUTPUT_SYSLOG)
	{
		// syslog have no LOG_TRACE
		static const int s_syslog_level[] = { 0, LOG_CRIT, LOG_ERR, LOG_WARNING,
			LOG_NOTICE, LOG_INFO, LOG_DEBUG, LOG_DEBUG};
		static const int s_syslog_facility[] = {LOG_LOCAL0, LOG_LOCAL1, LOG_LOCAL2, LOG_LOCAL3,
		LOG_LOCAL4, LOG_LOCAL5, LOG_LOCAL6, LOG_LOCAL7};

		syslog(s_syslog_facility[m_syslog_facility] | s_syslog_level[loglevel], "%s", buf);
	}

	return 0;
}

/////////////////////////////
// END_MFC_NAMESPACE

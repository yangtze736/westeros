///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: log.h
//
// Description:
//
// Created: 2015年05月20日 星期三 08时59分58秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#ifndef __MIDDLEWARELOG_H__
#define __MIDDLEWARELOG_H__

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

// user interface
#define LOG_LEVEL_CRIT		1
#define LOG_LEVEL_ERR		2
#define LOG_LEVEL_WARN		3
#define LOG_LEVEL_NOTICE	4
#define LOG_LEVEL_INFO		5
#define LOG_LEVEL_DEBUG		6
#define LOG_LEVEL_TRACE		7


#define LOG_OUTPUT_NONE		0x00
#define LOG_OUTPUT_SYSLOG	0x01
#define LOG_OUTPUT_STDERR	0x02
#define LOG_OUTPUT_FILE		0x04
#define LOG_OUTPUT_UDP		0x08

// can fix UDP packet
#define LOG_MAX_MSG_SIZE	1400
#define LOG_DEFAILT_PORT	9997


// init & change setting func

#define MFC_LOG_INIT		MfcLogger::Init
#define MFC_LOG_SETSTDERR	MfcLogger::SetStderr
#define MFC_LOG_SETFILE		MfcLogger::SetFile
#define MFC_LOG_SETSYSLOG	MfcLogger::SetSyslog
#define MFC_LOG_SETUDP		MfcLogger::SetUdp
#define MFC_LOG_SETPROG		MfcLogger::SetProg

// printf log function
#define PR_MFCLOG(loglevel, fmt, args...) \
	do{ \
		MfcLogger::PrintLog(loglevel, __FILE__, __LINE__, __func__, fmt, ##args); \
	}while(0)

#define PR_CRIT(fmt, args...)	PR_MFCLOG(LOG_LEVEL_CRIT, fmt, ##args)
#define PR_ERR(fmt, args...)	PR_MFCLOG(LOG_LEVEL_ERR, fmt, ##args)
#define PR_WARN(fmt, args...)	PR_MFCLOG(LOG_LEVEL_WARN, fmt, ##args)
#define PR_NOTICE(fmt, args...)	PR_MFCLOG(LOG_LEVEL_NOTICE, fmt, ##args)
#define PR_INFO(fmt, args...)	PR_MFCLOG(LOG_LEVEL_INFO, fmt, ##args)
#define PR_DEBUG(fmt, args...)	PR_MFCLOG(LOG_LEVEL_DEBUG, fmt, ##args)
#define PR_TRACE(fmt, args...)	PR_MFCLOG(LOG_LEVEL_TRACE, fmt, ##args)


// BEGIN_MFC_NAMESPACE
///////////////////////////
class MfcLogger
{
	protected:
		MfcLogger();
		~MfcLogger();

		int do_init(int loglevel, int logoutput);
		int do_setlevel(int loglevel);
		int do_setfile(const char *fname);
		int do_setsyslog(int logfacility);
		int do_setudp(const char *svraddr, int port);
		int do_setprog(const char *progname);

		int do_mkdir(char *fname);
		int do_reg_path(char *name, const char *fname);
		FILE *do_fopen(const char *fname);

	protected:
		int do_printlog_internal(int loglevel, const char *filename, int linenum, const char *funcname, const char *message);


	public:
		static int Init(int loglevel, int logoutput);
		static int SetLevel(int loglevel);
		static int SetFile(const char *fname);
		static int SetSyslog(int logfacility);
		static int SetUdp(const char *svraddr, int port);
		static int SetProg(const char *progname);

		static int PrintLog(int loglevel, const char *filename, int linenum, const char *funcname, const char *fmt, ...);


	protected:
		static MfcLogger ms_logger;

		int m_loglevel;
		int m_logoutput;
		int m_syslog_facility;
		int m_udp_fd;
		int m_udp_facility;
		int m_udp_severity;
		int m_my_ip_addr; //network byte order
		char m_hostname[15];
		char m_progname[15];
		FILE *m_file_fp;
		pthread_spinlock_t m_lock;
};
#endif

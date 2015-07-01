#ifndef __LOG4CLOUD_H__
#define __LOG4CLOUD_H__
#include <iostream>  
#include <sstream>
#include <stdlib.h>  
#include <string>
#include <cstring>
#include <openssl/aes.h>
#include "utils.h"
#include <map>
#include <mutex>

using namespace std;

class CLog4cloud
{
private:
    typedef enum {
        INFO = 0,
        DEBUG = 1,
        WARN = 2,
        ERROR = 3,
        FATAL = 4,

        LIMIT = 200,
    } TLogType;

    typedef struct {
        string project;
        string module;
        std::map<TLogType,string> logtype;
    }TLogElement;

    TLogElement m_element;
    CUtils* m_utils;
    std::mutex _mutex;
    string m_url;
    string m_mac;
    string m_ip;
    string m_hostname;
    bool m_bDebug;

public:
    CLog4cloud(void):m_bDebug(true),m_utils(NULL){log4cloud_init();}
    ~CLog4cloud(void){log4cloud_destroy();}

    void log4cloud_init(void);
    void log4cloud_destroy(void);
    void enableLogger(void);
    void disableLogger(void);
    void loggerGetHost(void);

    void set(string projectname,string modulename);
    bool info(string message,string meta = "null");
    bool debug(string message,string meta = "null");
    bool warn(string message,string meta = "null");
    bool error(string message,string meta = "null");
    bool fatal(string message,string meta = "null");
};

#endif //__LOG4CLOUD_H__

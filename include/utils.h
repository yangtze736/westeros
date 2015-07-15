#ifndef __UTILS_H__
#define __UTILS_H__
#include <iostream>  
#include <sstream>
#include <stdlib.h>  
#include <openssl/rsa.h>
#include <openssl/aes.h> 
#include <openssl/err.h>
#include <openssl/pem.h>
#include <string>
#include <cstring>
#include <cassert>
#include "json/json.h"

using namespace std;
class CUtils
{
public:
    CUtils(void){}

    void utils_init();
    string get_base64_Encode(const unsigned char* Data,int DataByte);
    string get_base64_Decode(const char* Data,int DataByte,int& OutByte);

    bool EncodeRSAKeyFile( const std::string& strPemFilePath, const std::string& strData, std::string& strOut);
    bool DecodeRSAKeyFile( const std::string& strPemFilePath, const std::string& strData, std::string& strOut);

    bool EncodeAES( const std::string& password, const std::string& data, std::string& strOut);
    bool DecodeAES( const std::string& strPassword, const std::string& strData, std::string& strOut);

    bool get_json(string &jsonstr, Json::Value &out);
    void utils_destroy();
};

#endif //__UTILS_H__


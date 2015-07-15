#ifndef __SECRYPTO_H__
#define __SECRYPTO_H__
#include <iostream>  
#include <sstream>
#include <stdlib.h>  
#include <string>
#include <cstring>
#include <openssl/aes.h>
#include "clientsocket.h"
#include "socketexception.h"
#include "utils.h"
#include <map>
#include <mutex>
#include <openssl/conf.h>
#include <openssl/engine.h>

#define EVP_BLOCK_SIZE 1024

using namespace std;

//define error no
#define Secrypto_NoError        0 // return correct
#define Error_CommunicationKey -1 // get communication key fail
#define Error_PublicKey 	   -2 // get public key fail
#define Error_Expire           -3 // The access token provided is expired
#define Error_NoJson           -4 // No JSON object could be decoded
#define Error_WrongRequest     -5 // wrong request
#define Error_Invalid          -6 // invalid access token

typedef enum{
	AESType_128 = 128,
	AESType_192 = 192,
	AESType_256 = 256,	
}TAesType;

class CSecrypto
{

private:
    typedef enum {
        ENCRYPTO = 0,
        DECRYPTO = 1,

        LIMIT = 200,
    } TCryptoType;
	
    typedef struct {
        /*cache 200 keys in maximum, bypass to Client Agent once per limit*/
        unsigned long limit;
        std::map<string, string> cached_enkeys;
        std::map<string, string> cached_dekeys;
		
    } TTinycache;
	
    TTinycache m_cache;
    CUtils* m_utils;
    ClientSocket* m_client_socket;
    std::mutex _mutex;
    TAesType m_aestype;

public:
    CSecrypto(void):m_client_socket(NULL),m_utils(NULL){secrypto_init();}
    ~CSecrypto(void){secrypto_destroy();}

    void secrypto_init(void);
    void secrypto_destroy(void);
	
    void getversionInfo(std::string &version, std::string &info);
    int convert_ukey2_cloudtoken(std::string& input, std::string& output);
    int sAES_decrypt(const unsigned char *in, unsigned char *out, std::string& access_token, TAesType aestype = AESType_128);
    int sAES_encrypt(const unsigned char *in, unsigned char *out, std::string& access_token, TAesType aestype = AESType_128);
 
    int sAES_set_encrypt_key(string& token, string &keyobj);
    int sAES_set_decrypt_key(string& token, string &keyobj);

	static void load_padlock(void){
		OpenSSL_add_all_ciphers();
		ENGINE_load_builtin_engines();
		ENGINE_register_all_ciphers();
		OPENSSL_config(NULL);
	}

protected:
    void m_get_user_key(std::string& input, Json::Value& JsonStr);
    bool m_extract_utoken(string& utoken, string& access_token);
    int m_guarantee_key(string& utoken, string &key, CSecrypto::TCryptoType type);
    bool m_add_key2_cache(string& utoken, string &cryptokey, CSecrypto::TCryptoType type);
    void m_get_cached_key(string& utoken, string& key, TCryptoType type);
    void m_clear_cache(void);
	//add for support padlock
	bool m_evp_encrypt(const unsigned char *in, int inl, unsigned char *out, unsigned char *key);
	bool m_evp_decrypt(const unsigned char *in, int inl, unsigned char *out, unsigned char *key);

};

#endif //__SECRYPTO_H__

///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName:   urlhandle.cpp
//
// Description:
//
// Created:    2015-06-30
// Revision:   Revision: 1.0
// Compiler:   g++
//
///////////////////////////////////////////////////////////

#include "string.h"
#include "urlhandle.h"
#include "stdio.h"
#include "openssl/md5.h"
#include <iconv.h>
#include <iostream>

namespace URL
{
	#define XX 127
	//Table for base64 encoding
	static char base64_map[64] = {
		'A','B','C','D', 'E','F','G','H', 'I','J','K','L', 'M','N','O','P',
		'Q','R','S','T', 'U','V','W','X', 'Y','Z','a','b', 'c','d','e','f',
		'g','h','i','j', 'k','l','m','n', 'o','p','q','r', 's','t','u','v',
		'w','x','y','z', '0','1','2','3', '4','5','6','7', '8','9','+','/',
	};

	//Table for base64 decoding
	static char base64_index[256] = {
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,62, XX,XX,XX,63,
		52,53,54,55, 56,57,58,59, 60,61,XX,XX, XX,XX,XX,XX,
		XX, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
		15,16,17,18, 19,20,21,22, 23,24,25,XX, XX,XX,XX,XX,
		XX,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
		41,42,43,44, 45,46,47,48, 49,50,51,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
	};

	// Table for hex to char
	static char url_hex_map[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};

	// Table for url encode
	// ! $ & ' * + / , - .  0-9 : ; = ? @ A-Z _ a-z
	static char url_url_table[256] = {
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 0, 1, 1,  0, 1, 0, 0,  1, 1, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1, 0, 1, 0,
		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1,  1, 1, 1, 0,
		1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	};

	static char url_hex_index[256] = {
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		 0, 1, 2, 3,  4, 5, 6, 7,  8, 9,XX,XX, XX,XX,XX,XX,
		XX,10,11,12, 13,14,15,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,10,11,12, 13,14,15,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
		XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
	};

	bool md5Encrpt(string &md5OutPut, const string &originUrl)
	{
		unsigned char MD5Digest[MD5_DIGEST_LENGTH];
		string MD5Input(originUrl);

		md5OutPut.clear();

		MD5((unsigned char *)MD5Input.data(), MD5Input.length(), MD5Digest);
		for(unsigned int i = 0; i < MD5_DIGEST_LENGTH; i++)
		{
			char tmpNum[32] = "";
			sprintf(tmpNum, "%02x", MD5Digest[i]);
			md5OutPut.append(tmpNum);
		}

		return true;
	}

	// function about encoding
	void string_utf8_to_gbk(string &outUrl, const string &inUrl)
	{
		if(inUrl.length() <= 0)
		{
			return;
		}
		char* buf = new char[3*inUrl.length()];
		memset(buf, 0, 3*inUrl.length());
		utf8_to_gbk(buf, inUrl.c_str(), 3*inUrl.length(), inUrl.length());
		outUrl = buf;
		delete[] buf;

		return;
	}

	int utf8_to_gbk(char *outUrl, const char *inUrl, int max_len, int len)
	{
		size_t byte = len;
		size_t max_size = max_len - 1;

		char *out_ptr = outUrl;
		char *in_ptr = ( char *)inUrl;
		iconv_t cd = iconv_open("GBK", "UTF-8");
		if( cd == (iconv_t)(-1) ) {
			return -1;
		}
		int ret = iconv(cd, &in_ptr, &byte, &out_ptr, &max_size );
		iconv_close( cd );
		if( ret == -1 ) {
			//do something
		}
		outUrl[ out_ptr - outUrl ] = '\0';
		return ( out_ptr - outUrl );
	}

	void string_gbk_to_utf8(string &outUrl, const string &inUrl)
	{
		if(inUrl.length() <= 0)
		{
			return;
		}
		char* buf = new char[3*inUrl.length()];
		memset(buf, 0, 3*inUrl.length());
		gbk_to_utf8(buf, inUrl.c_str(), 3*inUrl.length(), inUrl.length());
		outUrl = buf;
		delete[] buf;

		return;
	}

	int gbk_to_utf8(char *outUrl, const char *inUrl, int max_len, int len) 
	{
		size_t byte = len, max_size = max_len - 1;
		char *out_ptr = outUrl;
		char *in_ptr = ( char *)inUrl;
		iconv_t cd = iconv_open("UTF-8", "GBK");
		if( cd == (iconv_t)(-1) ) {
			return -1;
		}
		int ret = iconv(cd, &in_ptr, &byte, &out_ptr, &max_size );
		iconv_close( cd );
		if( ret == -1 ) {
			// do something
		}
		outUrl[ out_ptr - outUrl ] = '\0';
		return ( out_ptr - outUrl );
	}

	int UrlDecode( char *outUrl, const char *inUrl, int max_len )
	{
		int pos = 0, len = strlen(inUrl);
		for(int i = 0; i < len && pos < max_len; ++i)
		{
			if(inUrl[i] != '%')
			{
				outUrl[pos++] = inUrl[i];
			}
			else if(i+3 <= len)
			{
				outUrl[pos++] = CharToHex(inUrl[i+1], inUrl[i+2]);
				i += 2;
			}
			else
			{
				break;
			}
		}

		if(pos == max_len)
		{
			pos--;
		}
		outUrl[pos] = '\0';

		return pos;
	}

	char CharToHex(char up, char low)
	{
		return ( char_to_hex(up) & 0x0f ) << 4 | ( char_to_hex(low) & 0x0f );
	}

	char char_to_hex(char ch)
	{
		return url_hex_index[(unsigned char)ch];
	}

	int UrlEncode( char *outUrl, const char *inUrl, int max_len )
	{
		int pos = 0, len = strlen(inUrl);
		for(int i = 0; i < len && pos < max_len; ++i)
		{
			if(url_url_table[(unsigned char)inUrl[i]] == '\0')
			{
				outUrl[pos++] = inUrl[i];
			}
			else if(pos + 3 <= max_len)
			{
				outUrl[pos++] = '%';
				outUrl[pos++] = url_hex_map[(inUrl[i] & 0xf0) >> 4];
				outUrl[pos++] = url_hex_map[inUrl[i] & 0x0f];
			}
			else
			{
				break;
			}
		}

		if(pos == max_len)
		{
			pos--;
		}
		outUrl[pos] = '\0';

		return pos;
	}

	int base64_decode( unsigned char *out, const unsigned char *in, int len, const char *decode_index )
	{
#if 0
		unsigned char buf[4];
		int index = 0, ret = 0;
		while( index < len ) {
			int remain = ( len - index ) > 4 ? 4 : ( len - index );
			*( unsigned int *)buf = 0;
			int count = 0;
			for( int i = 0; i < 4; ++i ) {
				*( unsigned int *)buf <<= 6;
				if( i < remain && decode_index[ in[i+index] ] != 64 && decode_index[ in[i+index] ] != XX ) {
					*( unsigned int *)buf |= decode_index[ in[ i+index ] ];
					++count;
				}
			}
			for( int i = 0; i < count-1; ++i )
				out[ ret + i ] = buf [ 2-i ];
			index += remain;
			ret += (count - 1);
		}	
		return ret;
#else
		int index = 0, ret = 0;
		while( index < len ) {
			unsigned char buf[4] = {0};
			int remain = ( len - index ) > 4 ? 4 : ( len - index );
			memcpy( buf, in+index, remain );
			if( decode_index[ buf[0] ] == XX )
				break;
			out[ret+0] = decode_index[ buf[0] ] << 2;
			if( decode_index[ buf[1] ] == XX ) {
				ret = ret + 1;
				break;
			}
			out[ret+0] = ( decode_index[ buf[1] ] >> 4 ) | out[ret+0];
			out[ret+1] = decode_index[ buf[1] ] << 4;
			if( decode_index[ buf[2] ] == XX ) {
				ret = ret + 2;
				break;
			}
			out[ret+1] = ( decode_index[ buf[2] ] >> 2 ) | out[ret+1];
			out[ret+2] = decode_index[ buf[2] ] << 6;
			if( decode_index[ buf[3] ] == XX ) {
				ret = ret + 3;	
				break;
			}
			out[ret+2] = decode_index[ buf[3] ] | out[ret+2];
			ret += 3;
			index += 4;
		}
		return ret;
#endif
	}

	int base64_encode( unsigned char *out, const unsigned char *in, int len, const char *encode_map )
	{
#if 0
		unsigned char buf[4];
		int index = 0, ret = 0;
		while( index < len ) {
			int remain = len - index > 3 ? 3 : ( len - index );		
			*(unsigned int *)buf = 0;
			for( int i = 0; i < remain; ++i )
				buf[2-i] = in[ index + i];
			for( int i = 0; i < 4; ++i ) {
				out[ret+3-i] = encode_map[ *(unsigned int *)buf & 0x0000003f ];
				*(unsigned int *)buf >>= 6;
			}
			for( int i = remain+1; i < 4; ++i )
				out[ret+i] = '='; 
			ret += 4;
			index += remain;
		}
		out[ret] = '\0';
		return ret;
#else
		int index = 0, ret = 0;
		while( index < len ) {
			unsigned char buf[3] = {0};
			int remain = len - index > 3 ? 3 : ( len - index );
			memcpy( buf, in + index, remain ); 
			out[ret+0] = encode_map[ buf[0] >> 2 ];
			out[ret+1] = encode_map[ ( buf[0] << 4 ) & 0x30 | ( buf[1] >> 4 ) ];
			out[ret+2] = encode_map[ ( buf[1] << 2 ) & 0x3C | ( buf[2] >> 6 ) ];
			out[ret+3] = encode_map[ buf[2] & 0x3F ];
			for( int i = remain + 1; i < 4; i++ )
				out[ret+i] = '=';
			ret += 4;
			index += remain;
		}
		out[ret] = '\0';
		return ret;
#endif
	}

	int Base64Encode( unsigned char *out, const unsigned char *in, int len )
	{
		return base64_encode( out, in, len, base64_map );
	}

	int Base64Decode( unsigned char *out, const unsigned char *in, int len )
	{
		return base64_decode( out, in, len, base64_index );
	}

}

#ifndef __NET_TOOL_H__
#define __NET_TOOL_H__
#include <string>

using namespace std;
class CnetTool
{
  private:
  	bool m_bDebug;

  public:
  	CnetTool(void);
	~CnetTool(void);

	void net_init();
	void net_destroy();
	void SetDebug(bool bDebug); 

	/** 
		* @brief HTTP GET Request
		* @param strUrl Url address,ex:http://www.baidu.com 
		* @param strResponse used to store the received data
		* @return status of GET Method
	*/	
	int http_get(const std::string & strUrl, std::string & strResponse);

	/** 
              * @brief HTTP POST Request
              * @param strUrl URL address, ex:http://www.baidu.com 
              * @param strPost  the input parameter like para1=val1?2=val2&бн 
              * @param strResponse output
              * @return status of POST
       */ 
	int http_post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);
	int http_upload(const std::string & strUrl, const std::string & strPost, std::string & strResponse);
	void http_put();
	void http_delete();
	void http_copy();


	/** 
               * @brief HTTPS GET Request
               * @param strUrl  url address, ex:https://www.alipay.com 
               * @param strResponse store the received data
               * @param pCaPath  input parameter,the path of CA. if input NULL,then not validate the certificate of server side
               * @return status of GET
        */ 
	int https_get(const std::string & strUrl, std::string & strResponse, const char * pCaPath);

	/** 
              * @brief HTTPS POST Request
              * @param strUrl url address,ex:https://www.alipay.com 
              * @param strPost strPost  the input parameter like para1=val1?2=val2&бн 
              * @param strResponse output
              * @param pCaPath input parameter,the path of CA. if input NULL,then not validate the certificate of server side
              * @return status of POST
       */
	int https_post(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath);
	void https_put();
	void https_delete();
	void https_copy();
};

#endif //__NET_TOOL_H__

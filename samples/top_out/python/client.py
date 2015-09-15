#coding:utf-8
#author:changyanjie
#created:2015-6-2

import os
import json
import re
import platform
import sys
import time,signal
import cPickle as pickle
import logging
from ctypes import *
from decorators import *
reload(sys)
sys.setdefaultencoding("utf-8")

class Client(object):
    '''
    与数据服务器通信的类
    '''
    def __init__(self):
        self.access_token = None
        self.cdll = CDLL('/usr/local/middleware/lib/libmiddleware.so')
        
    def exec_cdll(self,method,params,op_params='',buffer_size=1048576,**kwargs):
        ''''''
        params_json = json.dumps(params)
        str_response = create_string_buffer(buffer_size)
        str_response.value = op_params.encode('utf8')
        self.cdll.data_pipeline(method,params_json,str_response)
        rsp = ''
        try:
            if str_response.value:
                rsp = eval(str_response.value)
                if isinstance(rsp,dict):
                    if rsp.has_key('status') and rsp['status'] == '14003' and not kwargs.has_key('retry'):
                        self.access_token = self.refresh_token(params['X-Auth-Token'])
                        params['X-Auth-Token'] = self.access_token
                        kwargs['retry'] = True
                        return self.exec_cdll(method,params,op_params,buffer_size,kwargs=kwargs)
                    else: pass
        except:
            
            rsp = {
                'error':'middleware error',
            }
            logging.error('exec cdll has error :\n%s' % str_response.value)


        return rsp
    
    def download_file(self,access_token='',cloud_path='',local_path='',mode='NORMAL',uuid=None,userName=None,**kwargs):
        '''下载文件'''
        access_token = access_token or self.access_token

        op_params = '?op=OPEN&mode=%s'%(mode)
        for k,v in kwargs.items():
            op_params = op_params + '&%s=%s' % (k,v)

        params = {
            'X-Auth-Token':access_token,
            'Source':local_path,
            'Destination':cloud_path,
            'uuid':uuid,
            'userName':userName
        }

        rsp = self.exec_cdll('readFile',params,op_params,buffer_size=512)

        return rsp
    
    @async
    def open_file(self,access_token='',cloud_path='',local_path='',**kwargs):
        
        access_token = access_token or self.access_token

        tmp_path = local_path or '/tmp/' + cloud_path.split('/')[-1]
        self.download_file(access_token=access_token,cloud_path=cloud_path,local_path=tmp_path,kwargs=kwargs)
        system_type = platform.system()
        
        
        pattern = re.compile(r" ") # 对特殊文件名进行转义
        sub_tmppath = pattern.sub(r'\ ',tmp_path)

        if system_type == 'Linux':
            os.system('xdg-open %s' % sub_tmppath)
        elif system_type == 'Windows':
            os.system('%s' % sub_tmppath)
        else:
            os.system('gedit %s' %sub_tmppath)


   

if __name__ == '__main__':

    from pprint import pprint
    client = Client()
	
    #打开文件
    client.open_file(access_token="xxxx",cloud_path='/normal/APP功能列表.xls',local_path='/tmp/APP功能列表.xls')
    client.open_file(access_token="xxx",cloud_path='/normal/APP功能列表.xls',local_path='/tmp/APP功能列表.xls')
    client.open_file(access_token="xxx",cloud_path='/normal/APP功能列表.xls',local_path='/tmp/APP功能列表.xls')
    #client.open_file(access_token="u5AP5GJ25KDFCSNfoLfDs89XQ6L2rWQC9rwxyoT8",cloud_path='/normal/APP功能列表.xls',local_path='/tmp/APP功能列表.xls')

    




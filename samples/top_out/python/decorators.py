#coding=utf8
from threading import Thread
from functools import wraps
import time
import signal
from multiprocessing.pool import ThreadPool
from multiprocessing import TimeoutError

def async(func):
    ''''''
    @wraps(func)
    def async_func(*args,**kwds):
        pool = ThreadPool(processes=1)
        func_hl = pool.apply_async(func,args=args,kwds=kwds)
        try:
            thred_res = func_hl.get(timeout=0.5)
        except TimeoutError:
            thred_res = {'msg':'running'}
        return thred_res
    return async_func

        

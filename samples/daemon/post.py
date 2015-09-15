#!/usr/bin/env python
# Filename: post.py

import sys
def send(host,port,request):
	import socket
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect((host,port))
	s.sendall("%s"%(request))
	while 1:
		buf = s.recv(1024*8)
		if buf:
			break
	sys.stdout.write(buf)
	sys.stdout.write("\r\n")

#parse argv
if __name__== "__main__":
    n = len(sys.argv)
    if n == 2:
	   send("127.0.0.1",50276,open(sys.argv[1]).read())
    elif n != 4:
	  print "Usage: ./post.py IP PORT request.json"
	  sys.exit()
    else:
	  send(sys.argv[1],int(sys.argv[2]),open(sys.argv[3]).read())


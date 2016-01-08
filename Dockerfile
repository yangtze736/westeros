FROM ubuntu:14.04

#install software
RUN apt-get install -y gcc
RUN apt-get install -y make
RUN apt-get install -y git
RUN apt-get install -y curl-devel
RUN apt-get install -y openssl-devel

#get remote source
RUN ["/bin/bash", "-c", "cd ~ && /usr/bin/git clone https://github.com/yangtze736/Westeros"]

#set env variables
RUN echo 'export LD_LIBRARY_PATH=$PATH:/usr/local/middleware/lib' >> ~/.bashrc

#build middleware
WORKDIR ~/middleware
RUN make && make install


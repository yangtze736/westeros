FROM ubuntu:14.04

#install software
RUN apt-get update
RUN apt-get install -yq gcc
RUN apt-get install -yq make
RUN apt-get install -yq git
RUN apt-get install -yq libcurl4-openssl-dev
RUN apt-get install -yq libssl-dev

#get remote source
RUN ["/bin/bash", "-c", "cd ~ && /usr/bin/git clone https://github.com/yangtze736/Westeros"]

#set env variables
RUN echo 'export LD_LIBRARY_PATH=$PATH:/usr/local/middleware/lib' >> ~/.bashrc

#build middleware
RUN ["/bin/bash", "-c", "cd ~/Westeros && make && make install" ]

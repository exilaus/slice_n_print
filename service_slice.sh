#!/bin/sh

case $1 in
start) 
cd /home/pi/slice_n_print
python -m CGIHTTPServer 7001 &
echo $! > ./server.pid
;;
stop) # code to stop the service
sudo kill `cat ./server.pid`
esac

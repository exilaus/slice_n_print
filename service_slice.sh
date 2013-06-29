#!/bin/sh
#sudo cp /usr/bin/python2.7 /usr/bin/pythonRoot
#sudo chmod u+s /usr/bin/pythonRoot

case $1 in
<<<<<<< HEAD
start)
=======
start) 
>>>>>>> 139f3396af0a6425c7417e1625cdde9a6323cd8d
cd /home/pi/slice_n_print
sudo python -m CGIHTTPServer 7001 &
echo $! > ./server.pid
;;
stop) # code to stop the service
sudo kill `cat ./server.pid`
esac

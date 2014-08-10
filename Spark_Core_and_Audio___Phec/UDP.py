#!/usr/bin/python
# Beehive monitor
# 
# 
## 

import socket               # Import socket module
import time
import struct

host = "192.168.1.112"      # use name of Spark server
port = 5208                 # Reserve a port for your service.
i=0
micData = [0]*128
optData = [0]*128

while True:
    s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)   #Create a socket object
    print('+')
    s.settimeout(3)
    while s: 
        try:
            s.connect((host, port))      # connect to server if awake
            s.sendall(b'da\0 ')          # da = send audio data
            print ('sent send data message')
        except socket.error:
            print('unable to connect')
            break
        r='not read anything'
        try:                         #check for data
            r = s.recv(1024)
            if r == 0:          # if r is 0 then the sender has closed for good
                print('socket disconnected')
            #read audio data packed in a buffer
            micData=struct.unpack('128i',r)
            print(micData)

            s.sendall(b'do\0 ')          # da = send optical data
            print ('sent send data message')
        except socket.error:
            print('unable to connect')
            break
        r='not read anything'
        try:                         #check for data
            r = s.recv(1024)
            if r == 0:          # if r is 0 then the sender has closed for good
                print('socket disconnected')
            #read optical data packed in a buffer
            optData=struct.unpack('128i',r)
            print(optData)

            s.sendall(b'dt\0 ')          # da = send optical data
            print ('sent send data message')
        except socket.error:
            print('unable to connect')
            break
        r='not read anything'
        try:                         #check for data
            r = s.recv(1024)
            if r == 0:          # if r is 0 then the sender has closed for good
                print('socket disconnected')
            #read string with temperature data
            try:
                text = r.decode("utf-8")
            except:
                text = "Can't decode"
            print(text)


            #s.sendall(b's1\0 ')          # s = sleep n is time in 10s of min  (UDPstub does 1s not 10s of min
            #print ('sent go to sleep message')
        except socket.timeout:
            print('.')
        time.sleep(10)
    s.close()

print ("Finished...")
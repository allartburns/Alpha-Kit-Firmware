#!/usr/bin/python
# Beehive monitor
# host 192.168.113/105
# socket 5208
#save 16 spectra to average noise
#make time format excel compatible
#save raw and fft data in separate daily files v1.31
#save sample averages
#save backups as csv
#filter out spikes in raw data 
## 

import socket               # Import socket module
import time
import struct
import numpy
import sys
from math import log10
import shutil

host = "192.168.1.105"      # use name of server uFL (BlackuFL)
port = 5208                 # Reserve a port for your service.
i=0
datalen = 256

print('v1.3BlackuFL')

p1 = '/media/IDEdrv/BeeMonitor/'
p2 = '/media/HD-EU2pi/BeeMonitor/'
def copyFile(name):
    d = time.strftime("_%d_%m_%y")+ '.txt'
    c = time.strftime("_%d_%m_%y")+ '.csv'
    try:
        shutil.copyfile(p1+name+d, p2+name+c)
    except:
        return -1
    return 0


while True:
    s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)   #Create a socket object
    sys.stdout.write('+')  # print a row of ++ while waiting for the Spark to wake up
    sys.stdout.flush()
    s.settimeout(3)
    w = numpy.hamming(datalen)
    while s: 
        try:
            s.connect((host, port))      # connect to server if awake
        except socket.error:
            #print('unable to connect')
            break
        r='not read anything'
        try:                         #check for data
# try asking for 16 sets of audio data
######################################
            with open("beeDataAudRaw"+time.strftime("_%d_%m_%y")+".txt","a") as raw, \
                 open("beeDataAudFFT"+time.strftime("_%d_%m_%y")+".txt","a") as frq, \
                 open("beeDataAudLinFFT"+time.strftime("_%d_%m_%y")+".txt","a") as lin:
                micAvg = 0.0
                micCount = 0 #move above with open ??
                for repeat in range(0,16):
                    dataOK = False
                    while not dataOK:
                        s.sendall(b'da\0 ')          # do = send optical data
                        r = s.recv(2048)
                        if r == 0:          # if r is 0 then the sender has closed for good
                            print('socket disconnected')
                        #read audio data packed in a buffer
                        micData1=struct.unpack('256h',r)
                        micData = numpy.array([x-2048 for x in micData1])
                        stats = (micData.max() - micData.min())/micData.std()
                        #check data received OK
                        if len(micData) != datalen:
                            print('bad audio data')
                            dataOK = False
                        elif stats > 7:
                            print('spiky data '+ str(stats))
                            dataOK = False
                        else:
                            dataOK = True
                            print('good data '+ str(stats))                            
                    for x in micData:
                        raw.write('%d, ' % x)
                    raw.write(str(len(micData)) + '\n')
                    print(time.ctime()+' saved audio data')
                    print(str(repeat)+'----------------------')
                    freq = numpy.abs(numpy.fft.rfft(micData*w,256))
                    for x in freq:
                        lin.write('%.4f, ' % x)
                    lin.write(time.strftime("%d/%m/%y %H:%M:%S")+' '+str(len(freq))+'\n')
                    dB = numpy.log10(freq)
                    for x in dB:
                        frq.write('%.4f, ' % x)
                    frq.write(time.strftime("%d/%m/%y %H:%M:%S")+' '+str(len(freq))+'\n')
                    micCount +=1
                    if micCount == 1:
                        micAvg = freq
                    else:
                        micAvg += freq
            micAvg /= micCount        
            with open('beeDataAudAvg.txt','a') as avg, \
                 open('beeDataAudLinAvg.txt','a') as lin:
                for x in micAvg:
                    lin.write('%.4f, ' % x)
                    if x>0:
                        avg.write('%.4f, ' % log10(x))
                    else:
                        avg.write('-1000.0')
                avg.write(time.strftime("%d/%m/%y %H:%M:%S")+'\n')
                lin.write(time.strftime("%d/%m/%y %H:%M:%S")+'\n')
                print('audio average saved')
# try asking for 16 sets of optical data
########################################
            optAvg = 0
            optCount = 0
            with open("beeDataOptRaw"+time.strftime("_%d_%m_%y")+".txt","a") as raw, \
                 open("beeDataOptFFT"+time.strftime("_%d_%m_%y")+".txt","a") as frq, \
                 open("beeDataOptLinFFT"+time.strftime("_%d_%m_%y")+".txt","a") as lin:
                for repeat in range(0,16):
                    dataOK = False
                    while not dataOK:
                        s.sendall(b'do\0 ')          # do = send optical data
                        r = s.recv(2048)
                        if r == 0:          # if r is 0 then the sender has closed for good
                            print('socket disconnected')
                        #read optical data packed in a buffer
                        optData1=struct.unpack('256h',r)
                        optData = numpy.array([x-2047 for x in optData1])
                        stats = (optData.max() - optData.min())/optData.std()
                        if len(optData) != datalen:
                            print('bad optical data')
                            dataOK = False
                        elif stats > 7:
                            print('spiky data '+ str(stats))
                            dataOK = False
                        else:
                            dataOK = True
                            print('good data '+ str(stats))                            

                    for x in optData:
                        raw.write('%d, ' % x)
                    raw.write(str(len(optData)) + '\n')
                    print(time.ctime()+ ' saved optical data')
                    print(str(repeat)+'----------------------')
                    freq = numpy.abs(numpy.fft.rfft(optData*w,256))
                    for x in freq:
                        lin.write('%.4f, ' % x)
                    lin.write(time.strftime("%d/%m/%y %H:%M:%S")+' '+str(len(freq))+'\n')
                    dB = numpy.log10(freq)
                    for x in dB:
                        frq.write('%.4f, ' % x)
                    frq.write(time.strftime("%d/%m/%y %H:%M:%S")+' '+str(len(freq))+'\n')
                    optCount += 1
                    if optCount == 3:  #ignore first two readings
                        optAvg = freq
                    elif optCount > 3:
                        optAvg += freq
            optAvg /= (optCount - 2)
            with open('beeDataOptAvg.txt','a') as avg, \
                 open('beeDataOptLinAvg.txt','a') as linAv:
                for x in optAvg:
                    linAv.write('%.4f, ' % x)
                    if x>0:
                        avg.write('%.4f, ' % log10(x))
                    else:
                        avg.write('-1000.0')
                avg.write(time.strftime("%d/%m/%y %H:%M:%S")+'\n')
                linAv.write(time.strftime("%d/%m/%y %H:%M:%S")+'\n')
                print('opt average saved '+str(optCount))




# ask for thermal data
######################

            s.sendall(b'dt\0 ')          # dt = send thermal data
        except socket.error:
            #print('unable to connect')
            break
        r='not read anything'
        try:                         #check for data
            r = s.recv(2048)
            if r == 0:          # if r is 0 then the sender has closed for good
                print('socket disconnected')
            #read string with temperature data
            try:
                text = r.decode("utf-8",'ignore')
            except:
                text = "Can't decode"
            vals = []
            T = [0.0,0.0,0.0,0.0,0.0,0.0]
            vals = text.split(',')
            RSSI = vals[1]
            for i in range(0,6):
                T[i] = float(vals[i+2].strip('\0'))
            with open("beeDataTherm.txt","a") as myfile:
                for Tm in T:
                    myfile.write('%.1f, ' % Tm)
                print(RSSI,T, len(T))
                myfile.write(time.strftime("%d/%m/%y %H:%M:%S")+'\n')
                #myfile.write(time.ctime()+'\n')

# send to sleep
###############

            s.sendall(b's6\0 ')          # s = sleep n is time in 10s of min
        except socket.timeout:
            print('.',end='')

# make backup copies of data files
##################################

        try:
            shutil.copyfile(p1+'beeDataTherm.txt', p2+'beeDataTherm.csv')
            shutil.copyfile(p1+'beeDataAudAvg.txt', p2+'beeDataAudAvg.csv')
            shutil.copyfile(p1+'beeDataOptAvg.txt', p2+'beeDataOptAvg.csv')
            shutil.copyfile(p1+'beeDataAudLinAvg.txt', p2+'beeDataAudLinAvg.csv')
            shutil.copyfile(p1+'beeDataOptLinAvg.txt', p2+'beeDataOptLinAvg.csv')
            copyFile('beeDataOptRaw')
            copyFile('beeDataOptFFT')
            copyFile('beeDataOptLinFFT')
            copyFile('beeDataAudRaw')
            copyFile('beeDataAudFFT')
            copyFile('beeDataAudLinFFT')
        except:
            print('cant write to HD-EU2')
    time.sleep(10)
    s.close()

print ("Finished...")
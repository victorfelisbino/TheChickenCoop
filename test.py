#!/usr/bin/python
import serial
import syslog
import time

#The following line is for serial over GPIO
port = '/dev/cu.usbmodem1421'


ard = serial.Serial(port,9600,timeout=1)

i = 1

while i:
    # Serial read section
	msg = ard.readline()
	if len(msg) > 0:
		print (msg)
		if msg.strip(" ") == "35 81 06 6D":
			#print "OK"
			ard.write("OK")
			print "Hello David Kelly"
		else:
			ard.write("FAIL")
			print "Authorization denied " 
	msg = ""
	#print i
	i = i + 1
#else:
    #print "Exiting"
#exit()


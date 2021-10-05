#!/bin/python3
#needs pip install pyqrcode
import socket
import pyqrcode

img = 0

def showImage():
	hostname = socket.gethostname()
	local_ip = socket.gethostbyname(hostname)
	img = pyqrcode.create(local_ip)
	print("Your IP:"+local_ip)
	print(img.terminal(quiet_zone=1))
	return local_ip



#!/bin/python3
#needs pip install qrcode
#needs pip install opencv-python
import socket
import qrcode
import cv2
import _thread

img = 0

def showImage():
	try:
		cv2.imshow("QR Code", img)
		cv2.waitKey()
	except ValueError:
		print("ERROR")
	finally:
		cv2.destroyAllWindows()


hostname = socket.gethostname()
local_ip = socket.gethostbyname(hostname)

img = qrcode.make(local_ip)
img.save("ip.png")
img = cv2.imread("ip.png")

_thread.start_new_thread(showImage, ()) #Workaround to stop terminal freeze
input()
exit()



import XBee_Threaded
from time import sleep

xbee = XBee_Threaded.XBee("/dev/ttyUSB2")

xbee_receive = xbee.Receive()
if xbee_receive:
	content = xbee_receive[14:-1].decode('ascii')
	print "Msg: "+content
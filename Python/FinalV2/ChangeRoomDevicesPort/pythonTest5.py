from time import sleep

import XBee_Threaded

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

data = "2E 0D 01 02 1D 07 E0 15 30 01 04 01 00 01 01"

xbee.Send(bytearray.fromhex(data), "00 13 A2 00 40 D9 3C 9A")

while True:
	msg = xbee.Receive()
	# if msg:
	# 	if msg[14]==8:
	# 		xbee.shutdown();
	# 		break
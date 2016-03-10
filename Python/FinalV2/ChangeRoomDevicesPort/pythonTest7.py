from time import sleep

import XBee_Threaded

xbee = XBee_Threaded.XBee("/dev/ttyUSB2")

data = "2E 08 06 01 01 04 01 00 01 00"

xbee.Send(bytearray.fromhex(data), "00 13 a2 00 40 69 8c 8e")

while True:
	msg = xbee.Receive()
	if msg:
		if msg[14]==8:
			xbee.Send(bytearray.fromhex("2E 01 0E"), "00 13 a2 00 40 69 8c 8e")
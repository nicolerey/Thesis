from time import sleep

import XBee_Threaded

xbee = XBee_Threaded.XBee("/dev/ttyUSB2")

data = "2E 07 09 09 04 14 12 13 15"

xbee.Send(bytearray.fromhex(data), "00 13 a2 00 40 69 8c 8e")

msg = xbee.Receive()
if msg:
	xbee.shutdown()
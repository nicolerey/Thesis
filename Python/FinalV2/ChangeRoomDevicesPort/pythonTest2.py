from time import sleep

import XBee_Threaded

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

data = "2E 01 06"

xbee.Send(bytearray.fromhex(data), "00 13 A2 00 40 D9 3C 9A")

msg = xbee.Receive()
if msg:
	xbee.shutdown()

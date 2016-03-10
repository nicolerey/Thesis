from time import sleep

import XBee_Threaded

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

data = "2E 07 06 01 04 01 00 01 00"

xbee.Send(bytearray.fromhex(data), "00 13 A2 00 40 D9 3C 9A")

msg = xbee.Receive()
if msg:
	xbee.shutdown()
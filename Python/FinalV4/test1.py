from time import sleep
import XBee_Threaded

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

data = "2E 01 03"

xbee.Send(bytearray.fromhex(data), "00 13 A2 00 40 A9 D6 12")

while True:
	msg = xbee.Receive()
	if msg:
		xbee.shutdown()
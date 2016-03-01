from time import sleep

import XBee_Threaded

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

data = "2E 0B 06 01 01 00 03 6E 69 63 6F 6C 65"

xbee.Send(bytearray.fromhex(data), "00 13 A2 00 40 D9 3C 9A")

msg = xbee.Receive()
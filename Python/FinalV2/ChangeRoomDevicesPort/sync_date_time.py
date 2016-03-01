from time import sleep
from general_functions import ConvertIntToHex

import XBee_Threaded
import datetime

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

while True:
	msg = xbee.Receive()
	if msg:
		if msg[14]==2:
			now = datetime.datetime.now()

			hour = ConvertIntToHex(now.strftime("%H"))
			minute = ConvertIntToHex(now.strftime("%M"))
			second = ConvertIntToHex(now.strftime("%S"))
			day = ConvertIntToHex(now.strftime("%d"))
			month = ConvertIntToHex(now.strftime("%m"))
			year = int(now.strftime("%Y"))

			date_time_data = "03 {} {} {} {} {} {} {}".format(
				hour,
				minute,
				second,
				day,
				month,
				ConvertIntToHex((year >> 8) & 0xFF),
				ConvertIntToHex(year & 0xFF)
				)

			addr = ""
			for byte_data in msg[3:11]:
				addr = "{} {}".format(addr, ConvertIntToHex(byte_data))

			date_time_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(date_time_data))), date_time_data)
			xbee.Send(bytearray.fromhex(date_time_data), addr)
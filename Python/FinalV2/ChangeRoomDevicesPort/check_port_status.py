from time import sleep
from SQL_class import SQLClass
from general_functions import ConvertIntToHex

import XBee_Threaded
import sys

sql = SQLClass()

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

def CheckPortStatus(rooms_id):
	sql.GetWhereQuery("rooms", "rooms_id={}".format(rooms_id))
	sql.SelectColumn("rooms_address")
	rooms_result = sql.FetchOne()

	checking_port_status_data = "2E 01 07"

	xbee.Send(bytearray.fromhex(checking_port_status_data), rooms_result[0])

	msg = xbee.Receive()
	if msg:
		if msg[14]==8:
			sql.UpdateQuery("rooms", [["rooms_status", msg[15]]], "rooms_id={}".format(rooms_id))
			sql.Commit()

			sql.GetWhereQuery("room_devices", "rooms_id={}".format(rooms_id))
			sql.SelectColumn("room_devices_id")
			room_devices_result = sql.FetchAll()

			x = 0
			for device in room_devices_result:
				sql.UpdateQuery("room_devices", [["room_devices_status", msg[16+x]]], "room_devices_id={}".format(device[0]))
				sql.Commit()
				x += 1

	xbee.shutdown()

CheckPortStatus(sys.argv[1])
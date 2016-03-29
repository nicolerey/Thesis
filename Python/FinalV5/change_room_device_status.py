from SQL_class import SQLClass
from general_functions import ConvertIntToHex

import time
import XBee_Threaded
import sys
import datetime

def ChangeRoomDeviceStatus(sql, xbee, rooms_id):
	sql.GetWhereQuery("rooms", "rooms_id={}".format(rooms_id))
	sql.SelectColumn("rooms_address, rooms_status")
	rooms_result = sql.FetchOne()

	sql.GetWhereQuery("room_devices", "rooms_id={}".format(rooms_id))
	sql.SelectColumn("room_devices_status, room_devices_id")
	room_devices_result = sql.FetchAll()

	sql.GetWhereQuery("room_schedules", "rooms_id={} and room_schedules_type=1".format(rooms_id))
	sql.SelectColumn("room_schedules_id, room_schedules_date, room_schedules_end_time")
	room_schedules_result = sql.FetchOne()

	if room_schedules_result:
		year = int(room_schedules_result[1].strftime("%Y"))

		room_devices_status_data = "01 {} {} {} {} {} {} {} {}".format(
			ConvertIntToHex(room_schedules_result[1].strftime("%m")),
			ConvertIntToHex(room_schedules_result[1].strftime("%d")),
			ConvertIntToHex((year >> 8) & 0xFF),
			ConvertIntToHex(year & 0xFF),
			ConvertIntToHex(int(room_schedules_result[2]/60)),
			ConvertIntToHex(room_schedules_result[2]%60),
			ConvertIntToHex(rooms_result[1]),
			ConvertIntToHex(len(room_devices_result))
			)
		
		for room_device in room_devices_result:
			sql.GetWhereQuery("room_device_schedules", "room_devices_id={} and room_schedules_id={}".format(room_device[1], room_schedules_result[0]))
			result = sql.FetchOne()

			if result:
				value = "01"
			else:
				value = "00"

			room_devices_status_data = "{} {}".format(room_devices_status_data, value)

		room_devices_status_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(room_devices_status_data))), room_devices_status_data)

		xbee.Send(bytearray.fromhex(room_devices_status_data), rooms_result[0])

#ChangeRoomDeviceStatus(sys.argv[1])
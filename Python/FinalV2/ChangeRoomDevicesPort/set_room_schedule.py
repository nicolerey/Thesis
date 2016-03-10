from time import sleep
from general_functions import ConvertIntToHex
from SQL_class import SQLClass

import XBee_Threaded
import datetime
import sys

sql = SQLClass()

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

def SetRoomSchedule(rooms_id):
	sql.GetWhereQuery("rooms", "rooms_id={}".format(rooms_id))
	sql.SelectColumn("rooms_address")
	rooms_result = sql.FetchOne()

	sql.GetWhereQuery("room_devices", "rooms_id={}".format(rooms_id))
	sql.SelectColumn("room_devices_id")
	room_devices_result = sql.FetchAll()

	days = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"]

	room_schedules_data = [[0 for x in range(180)]for x in range(7)]
	for day in days:
		sql.GetWhereQuery("room_schedules", "rooms_id={} and room_schedules_day like '%{}%' and room_schedules_type=0".format(rooms_id, day))
		sql.SelectColumn("room_schedules_id, room_schedules_start_time, room_schedules_end_time")
		room_schedules_result = sql.FetchAll()

		room_schedules_data = ""
		room_device_status = "01"
		schedule_index = 1
		for room_schedule in room_schedules_result:
			# room_schedules_data = "{} {} {} {} {}".format(
			# 	room_schedules_data,
			# 	ConvertIntToHex((room_schedule[1] >> 8) & 0xFF),
			# 	ConvertIntToHex(room_schedule[1] & 0xFF),
			# 	ConvertIntToHex((room_schedule[2] >> 8) & 0xFF),
			# 	ConvertIntToHex(room_schedule[2] & 0xFF))

			for room_device in room_devices_result:
				sql.GetWhereQuery("room_device_schedules", "room_devices_id={} and room_schedules_id={}".format(room_device[0], room_schedule[0]))
				sql.SelectColumn("room_device_schedules_id")
				room_device_schedules_result = sql.FetchOne()

				if room_device_schedules_result:
					room_device_status = "{} 01".format(room_device_status)
				else:
					room_device_status = "{} 00".format(room_device_status)

			room_schedules_data = "{} {}".format(room_device_status, room_schedules_data.lstrip(" "))

			print room_schedules_data

			room_schedules_data = "06 {} {} {}".format(
				ConvertIntToHex(days.index(day)),
				ConvertIntToHex(schedule_index),
				room_schedules_data
				)

			print room_schedules_data

			room_schedules_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(room_schedules_data))), room_schedules_data)

			print room_schedules_data

			# while True:
			# 	xbee.Send(bytearray.fromhex(room_schedules_data), rooms_result[0])

			# 	msg = xbee.Receive()
			# 	if msg and msg[14]==15:
			# 		break;
					
			schedule_index += 1

	xbee.shutdown()

SetRoomSchedule(sys.argv[1])
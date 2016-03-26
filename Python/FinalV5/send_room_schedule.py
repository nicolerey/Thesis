from time import sleep
from general_functions import ConvertIntToHex
from SQL_class import SQLClass

import XBee_Threaded
import datetime
import schedule
import time

def SendRoomSchedules(sql, xbee, rooms_id, schedule_id):
	if not schedule_id:
		sql.GetWhereQuery("rooms", "rooms_id={}".format(rooms_id))
		sql.SelectColumn("rooms_address")
		rooms_result = sql.FetchOne()

		sql.GetWhereQuery("room_devices", "rooms_id={}".format(rooms_id))
		sql.SelectColumn("room_devices_id")
		room_devices_result = sql.FetchAll()

		sql.GetWhereQuery("room_schedules", "rooms_id={} and room_schedules_date is NULL and room_schedules_type=0".format(rooms_id))
		sql.SelectColumn("room_schedules_day, room_schedules_start_time, room_schedules_end_time, room_schedules_id")
		room_schedules_result = sql.FetchAll()

		frame_type_flag = 0;
		for schedule in room_schedules_result:
			day_abr = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"]

			day_abr_array = schedule[0].split(',')
			day_values = ""
			for day in day_abr_array:
				day_values = "{} {}".format(day_values, ConvertIntToHex(day_abr.index(day)+1))

			device_number = 2
			device_values = "01"
			for device in room_devices_result:
				sql.GetWhereQuery("room_device_schedules", "room_devices_id={} and room_schedules_id={}".format(device[0], schedule[3]))
				room_device_schedules_result = sql.FetchOne()

				if(room_device_schedules_result):
					device_values = "{} {}".format(device_values, ConvertIntToHex(device_number))

				device_number += 1

			if not frame_type_flag:
				schedule_data = "0A"
				frame_type_flag = 1
			else:
				schedule_data = "06"

			schedule_data = "{} {} {} {} {} {} {} {} {}".format(schedule_data, ConvertIntToHex(len(day_abr_array)), day_values.lstrip(" "), ConvertIntToHex(len(bytearray.fromhex(device_values))), device_values, ConvertIntToHex(((schedule[1] >> 8) & 0xFF)), ConvertIntToHex((schedule[1] & 0xFF)), ConvertIntToHex(((schedule[2] >> 8) & 0xFF)), ConvertIntToHex((schedule[2] & 0xFF)))

			schedule_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(schedule_data))), schedule_data)

			xbee.Send(bytearray.fromhex(schedule_data), rooms_result[0])
			time.sleep(4)
	else:
		sql.GetWhereQuery("rooms", "rooms_id={}".format(rooms_id))
		sql.SelectColumn("rooms_address")
		rooms_result = sql.FetchOne()

		sql.GetWhereQuery("room_devices", "rooms_id={}".format(rooms_id))
		sql.SelectColumn("room_devices_id")
		room_devices_result = sql.FetchAll()

		sql.GetWhereQuery("room_schedules", "rooms_id={} and room_schedules_id={}".format(rooms_id, schedule_id))
		sql.SelectColumn("room_schedules_day, room_schedules_start_time, room_schedules_end_time, room_schedules_id")
		room_schedules_result = sql.FetchOne()

		day_abr = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"]

		day_abr_array = room_schedules_result[0].split(',')
		day_values = ""
		for day in day_abr_array:
			day_values = "{} {}".format(day_values, ConvertIntToHex(day_abr.index(day)+1))

		device_number = 2
		device_values = "01"
		for device in room_devices_result:
			sql.GetWhereQuery("room_device_schedules", "room_devices_id={} and room_schedules_id={}".format(device[0], room_schedules_result[3]))
			room_device_schedules_result = sql.FetchOne()

			if(room_device_schedules_result):
				device_values = "{} {}".format(device_values, ConvertIntToHex(device_number))

			device_number += 1

		schedule_data = "06 {} {} {} {} {} {} {} {}".format(ConvertIntToHex(len(day_abr_array)), day_values.lstrip(" "), ConvertIntToHex(len(bytearray.fromhex(device_values))), device_values, ConvertIntToHex(((room_schedules_result[1] >> 8) & 0xFF)), ConvertIntToHex((room_schedules_result[1] & 0xFF)), ConvertIntToHex(((room_schedules_result[2] >> 8) & 0xFF)), ConvertIntToHex((room_schedules_result[2] & 0xFF)))

		schedule_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(schedule_data))), schedule_data)

		xbee.Send(bytearray.fromhex(schedule_data), rooms_result[0])

#SendRoomSchedules(7)
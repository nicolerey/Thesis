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

		for schedule in room_schedules_result:
			day_abr = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"]
			for day in schedule[0].split(','):
				day_index = day_abr.index(day)+1

				schedule_data = "0A {} 01 {} {} {} {}".format(ConvertIntToHex(day_index), ConvertIntToHex(((schedule[1] >> 8) & 0xFF)), ConvertIntToHex((schedule[1] & 0xFF)), ConvertIntToHex(((schedule[2] >> 8) & 0xFF)), ConvertIntToHex((schedule[2] & 0xFF)))

				schedule_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(schedule_data))), schedule_data)

				xbee.Send(bytearray.fromhex(schedule_data), rooms_result[0])

				device_number = 2
				for device in room_devices_result:
					sql.GetWhereQuery("room_device_schedules", "room_devices_id={} and room_schedules_id={}".format(device[0], schedule[3]))
					room_device_schedules_result = sql.FetchOne()

					if room_device_schedules_result:
						schedule_data = "06 {} {} {} {} {} {}".format(ConvertIntToHex(day_index), ConvertIntToHex(device_number), ConvertIntToHex(((schedule[1] >> 8) & 0xFF)), ConvertIntToHex((schedule[1] & 0xFF)), ConvertIntToHex(((schedule[2] >> 8) & 0xFF)), ConvertIntToHex((schedule[2] & 0xFF)))

						schedule_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(schedule_data))), schedule_data)

						xbee.Send(bytearray.fromhex(schedule_data), rooms_result[0])
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
		for day in room_schedules_result[0].split(','):
			day_index = day_abr.index(day)+1

			schedule_data = "06 {} 01 {} {} {} {}".format(ConvertIntToHex(day_index), ConvertIntToHex(((room_schedules_result[1] >> 8) & 0xFF)), ConvertIntToHex((room_schedules_result[1] & 0xFF)), ConvertIntToHex(((room_schedules_result[2] >> 8) & 0xFF)), ConvertIntToHex((room_schedules_result[2] & 0xFF)))

			schedule_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(schedule_data))), schedule_data)

			xbee.Send(bytearray.fromhex(schedule_data), rooms_result[0])

			device_number = 2
			for device in room_devices_result:
				sql.GetWhereQuery("room_device_schedules", "room_devices_id={} and room_schedules_id={}".format(device[0], room_schedules_result[3]))
				room_device_schedules_result = sql.FetchOne()

				if room_device_schedules_result:
					schedule_data = "06 {} {} {} {} {} {}".format(ConvertIntToHex(day_index), ConvertIntToHex(device_number), ConvertIntToHex(((room_schedules_result[1] >> 8) & 0xFF)), ConvertIntToHex((room_schedules_result[1] & 0xFF)), ConvertIntToHex(((room_schedules_result[2] >> 8) & 0xFF)), ConvertIntToHex((room_schedules_result[2] & 0xFF)))

					schedule_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(schedule_data))), schedule_data)

					xbee.Send(bytearray.fromhex(schedule_data), rooms_result[0])

#SendRoomSchedules(7)
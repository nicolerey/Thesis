from time import sleep
from general_functions import ConvertIntToHex
from SQL_class import SQLClass

import XBee_Threaded
import datetime
import schedule

sql = SQLClass()

xbee = XBee_Threaded.XBee("/dev/ttyUSB2")

def StartCheckRoomSchedules():
	schedule.every(1).minutes.do(CheckRoomSchedules)
	CheckRoomSchedules()

	return schedule.CancelJob

def CheckRoomSchedules():
	now = datetime.datetime.now()
	weekday_now = now.strftime("%a")
	time_in_minute_now = (int(now.strftime("%H")) * 60) + int(now.strftime("%M"))
	date_now = now.strftime("%Y-%m-%d")

	sql.GetWhereQuery("room_schedules", "(room_schedules_day like '%{}%' or room_schedules_date='{}') and (room_schedules_start_time={} or room_schedules_end_time={})".format(weekday_now, date_now, time_in_minute_now, time_in_minute_now))
	sql.SelectColumn("rooms_id, room_schedules_id, room_schedules_start_time, room_schedules_end_time")
	room_schedules_result = sql.FetchAll()

	if room_schedules_result:
		for schedule in room_schedules_result:
			sql.GetWhereQuery("rooms", "rooms_id={}".format(schedule[0]))
			sql.SelectColumn("rooms_address")
			rooms_result = sql.FetchOne()

			sql.GetWhereQuery("room_devices", "rooms_id={}".format(schedule[0]))
			sql.SelectColumn("room_devices_id")
			room_devices_result = sql.FetchAll()

			device_schedule_status = "06"
			if schedule[2]==time_in_minute_now:
				device_schedule_status = "{} 01 01 {}".format(device_schedule_status, ConvertIntToHex(len(room_devices_result)))
				for device in room_devices_result:
					sql.GetWhereQuery("room_device_schedules", "room_schedules_id={} and room_devices_id={}".format(schedule[1], device[0]))
					room_device_schedules_result = sql.FetchOne()

					if room_device_schedules_result:
						device_schedule_status = "{} 01".format(device_schedule_status)
					else:
						device_schedule_status = "{} 00".format(device_schedule_status)
			elif schedule[3]==time_in_minute_now:
				device_schedule_status = "{} 00 00 {}".format(device_schedule_status, ConvertIntToHex(len(room_devices_result)))
				for x in range(len(room_devices_result)):
					device_schedule_status = "{} 00".format(device_schedule_status)

			device_schedule_status = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(device_schedule_status))), device_schedule_status)

			print device_schedule_status

			xbee.Send(bytearray.fromhex(device_schedule_status), rooms_result[0])

		xbee.shutdown()

nw = datetime.datetime.now()
schedule.every(60-int(nw.strftime("%S"))).seconds.do(StartCheckRoomSchedules)

while True:
	schedule.run_pending()

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
		elif msg[14]==8:
			addr = ""
			for byte_data in msg[3:11]:
				addr = "{} {}".format(addr, ConvertIntToHex(byte_data).upper())

			sql.GetWhereQuery("rooms", "rooms_address='{}'".format(addr.lstrip(" ")))
			sql.SelectColumn("rooms_id")
			rooms_result = sql.FetchOne()

			if rooms_result:
				sql.UpdateQuery("rooms", [["rooms_status", msg[15]]], "rooms_id={}".format(rooms_result[0]))
				sql.Commit()

				sql.GetWhereQuery("room_devices", "rooms_id={}".format(rooms_result[0]))
				sql.SelectColumn("room_devices_id")
				room_devices_result = sql.FetchAll()

				if room_devices_result:
					x = 0;
					for device in room_devices_result:
						sql.UpdateQuery("room_devices", [["room_devices_status", msg[16+x]]], "room_devices_id={}".format(device[0]))
						sql.Commit()
						x += 1

				sql.UpdateQuery("check_rooms", [["check_rooms_status", 1]], 0)
				sql.Commit()

				sql.InsertQuery("check_devices", "{}, 1".format(rooms_result[0]))
				sql.Commit()

			data = "2E 01 0E"
			xbee.Send(bytearray.fromhex(data), addr)
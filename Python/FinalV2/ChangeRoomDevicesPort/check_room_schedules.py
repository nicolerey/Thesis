from time import sleep
from general_functions import ConvertIntToHex
from SQL_class import SQLClass

import XBee_Threaded
import datetime
import schedule
import time

sql = SQLClass()

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

def StartCheckRoomSchedules():	
	schedule.every(1).minutes.do(CheckRoomSchedules)

	return schedule.CancelJob

def CheckRoomSchedules():
	now = datetime.datetime.now()
	weekday_now = now.strftime("%a")
	time_in_minute_now = (int(now.strftime("%H")) * 60) + int(now.strftime("%M"))
	date_now = now.strftime("%Y-%m-%d")

	print date_now
	print time_in_minute_now
	print weekday_now

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

			xbee.Send(bytearray.fromhex(device_schedule_status), rooms_result[0])

		xbee.shutdown()

nw = datetime.datetime.now()
schedule.every(60-int(nw.strftime("%S"))).seconds.do(StartCheckRoomSchedules)

while True:
	schedule.run_pending()
	time.sleep(1)	
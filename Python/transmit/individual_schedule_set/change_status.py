#!/usr/bin/python

import datetime

from SQL_functions import SQLMain
from send_status import SendStatus
from file_functions import FileMain

def ChangeStatusToOn():
	FileMain("Changing status to ON")

	date_time = datetime.datetime.now()
	day_now = date_time.strftime("%a")
	date_now = date_time.strftime("%Y-%m-%d")
	hour_now = date_time.strftime("%H")
	minute_now = date_time.strftime("%M")
	time_now = (hour_now * 60) + minute_now

	sql_args = ["rooms_id", "room_schedules", "room_schedules_day like '%{}%' or room_schedules_date={} and room_schedules_start_time={}".format(day_now, date_now, time_now)]

	room_schedules_result = SQLMain(sql_args, 1)

	sql_args = ["rooms a inner join room_schedules b on a.rooms_id=b.rooms_id inner join room_device_schedules c on b.room_schedules_id=c.room_schedules_id inner join room_devices d on d.room_devices_id=c.room_devices_id", "a.rooms_status=1, d.room_devices_status=1", "room_schedules_day like '%{}%' or room_schedules_date={} and room_schedules_start_time={}".format(day_now, date_now, time_now)]

	SQLMain(sql_args, 4)

	for row in room_schedules_result:
		SendStatus(row[0])

	return

def ChangeStatusToOff():
	FileMain("Change status to OFF")

	date_time = datetime.datetime.now()
	day_now = date_time.strftime("%a")
	date_now = date_time.strftime("%Y-%m-%d")
	hour_now = date_time.strftime("%H")
	minute_now = date_time.strftime("%M")
	time_now = (hour_now * 60) + minute_now

	sql_args = ["rooms_id", "room_schedules", "room_schedules_day like '%{}%' or room_schedules_date={} and room_schedules_end_time={}".format(day_now, date_now, time_now)]

	room_schedules_result = SQLMain(sql_args, 1)

	sql_args = ["rooms a inner join room_schedules b on a.rooms_id=b.rooms_id inner join room_device_schedules c on b.room_schedules_id=c.room_schedules_id inner join room_devices d on d.room_devices_id=c.room_devices_id", "a.rooms_status=0, d.room_devices_status=0", "room_schedules_day like '%{}%' or room_schedules_date={} and room_schedules_end_time={}".format(day_now, date_now, time_now)]

	SQLMain(sql_args, 4)

	for row in room_schedules_result:
		SendStatus(row[0])

	return
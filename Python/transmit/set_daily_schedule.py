#!/usr/bin/python

import schedule
import datetime
import os
import time

from SQL_functions import SQLMain
from change_status import ChangeStatusToOn
from change_status import ChangeStatusToOff
from file_functions import FileMain

def SetDailySchedule():
	FileMain("Setting daily schedule")

	date = datetime.datetime.now()
	date = date.strftime("%Y-%m-%d")

	print date

	sql_args = ["room_schedules_start_time, room_schedules_end_time", "room_schedules", "room_schedules_date='{}'".format(date)]

	room_schedules_result = SQLMain(sql_args, 1)

	if room_schedules_result != 0:
		for row in room_schedules_result:
			hour_start_time = row[0]/60
			minute_start_time = row[0]%60
			start_time = "{}:{}".format(str(hour_start_time), str(minute_start_time))

			hour_end_time = row[1]/60
			minute_end_time = row[1]%60
			end_time = "{}:{}".format(str(hour_end_time), str(minute_end_time))

			schedule.every().day.at(time).do(DailyChangeOfStatusOn)

			schedule.every().day.at(time).do(DailyChangeOfStatusOff)

	return

def DailyChangeOfStatusOn():
	ChangeStatusToOn()

	return schedule.CancelJob

def DailyChangeOfStatusOff():
	ChangeStatusToOff()

	return schedule.CancelJob

SetDailySchedule()
schedule.every().day.at("00:00").do(SetDailySchedule)

while True:
	schedule.run_pending()

	if os.path.isfile('daily_stop_file'):
		os.remove('daily_stop_file')
		break

	time.sleep(1)

FileMain("Daily scheduling stopped")
#!/usr/bin/python

import schedule
import os
import time

from change_status import ChangeStatusToOn
from change_status import ChangeStatusToOff
from SQL_functions import SQLMain
from file_functions import FileMain

def SetTuesdaySchedule():
	FileMain("Setting tuesday schedule")

	sql_args = ["room_schedules_start_time, room_schedules_end_time", "room_schedules", "room_schedules_day like '%{}%'".format("Tue")]

	room_schedules_result = SQLMain(sql_args, 1)

	if room_schedules_result != 0:
		for row in room_schedules_result:
			hour_start_time = row[0]/60
			minute_start_time = row[0]%60
			start_time = "{}:{}".format(str(hour_start_time), str(minute_start_time))

			hour_end_time = row[1]/60
			minute_end_time = row[1]%60
			end_time = "{}:{}".format(str(hour_end_time), str(minute_end_time))

			schedule.every().tuesday.at(start_time).do(ChangeStatusToOn)

			schedule.every().tuesday.at(end_time).do(ChangeStatusToOff)

	return

SetTuesdaySchedule()

while True:
	schedule.run_pending()

	if os.path.isfile('tuesday_stop_file'):
		os.remove('tuesday_stop_file')
		break

	time.sleep(1)

FileMain("Tuesday scheduling stopped")
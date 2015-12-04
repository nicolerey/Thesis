#!/usr/bin/python

import serial
import schedule
import time
import datetime

from SQL_functions import SQLMain

present_datetime = datetime.datetime.now()
day_today = present_datetime.strftime("%a")
date_today = present_datetime.strftime("%Y-%m-%d")

sql_args = ["rooms_id, room_schedules_time", "room_schedules", "room_schedules_day like '%{}%' or room_schedules_date={} order by room_schedules_time DESC".format(day_today, date_today)]

room_schedules_result = SQLMain(sql_args, 1)

schedule_array = list()
for row in room_schedules_result:
	schedule_array.append(list(row))

print room_schedules_result[0]
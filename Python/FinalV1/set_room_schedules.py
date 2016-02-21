#!/usr/bin/python
from __future__ import division

import datetime
import math
import schedule

from general_functions import *

from create_frame import CreateFrame

receiver = []
sql = []
frame_id = 0
room_schedule_frame = []
frame_sending_attempts = 0
acknowledgement_status = 0

def SetRoomSchedules(rcvr_class, sql_class):
	global receiver
	global sql
	global frame_id
	global room_schedule_frame
	global frame_sending_attempts
	global acknowledgement_status

	receiver = rcvr_class
	sql = sql_class

	now = datetime.datetime.now()
	day = now.strftime("%a")
	date = now.strftime("%Y-%m-%d")

	sql.GetQuery("rooms")
	sql.SelectColumn("rooms_id, rooms_port, rooms_address")
	rooms_result = sql.FetchAll()

	for row in rooms_result:
		sql.GetWhereQuery("room_schedules", "rooms_id={} and room_schedules_day like '%{}%' or room_schedules_date='{}'".format(row[0], day, date))
		sql.SelectColumn("room_schedules_start_time, room_schedules_end_time")
		room_schedules_result = sql.FetchAll()

		room_schedule_array = [0 for x in range(180)]

		for schedule in room_schedules_result:
			schedule_start_time = schedule[0]
			schedule_end_time = schedule[1]

			for num in range(schedule_start_time, schedule_end_time+1):
				room_schedule_array[int(num/8)] ^= (-1 ^ room_schedule_array[int(num/8)]) & (1 << num%8)

		sql.GetWhereQuery("room_devices", "rooms_id={}".format(row[0]))
		sql.SelectColumn("room_devices_id, room_devices_port")
		room_devices_result = sql.FetchAll()

		room_device_schedule_array = [[0 for x in range(180)] for x in range(len(room_devices_result))]
		room_device_port_array = [0 for x in range(len(room_devices_result))]

		device_count = 0
		for device in room_devices_result:
			room_device_port_array[device_count] = int(str(device[1]).rstrip("L"))

			sql.SQL_query = []
			sql.SelectColumn("room_schedules.room_schedules_start_time, room_schedules.room_schedules_end_time")
			sql.FromTable("room_device_schedules")
			join_table = [["room_schedules", "room_schedules.room_schedules_id=room_device_schedules.room_schedules_id"]]
			sql.JoinTable(join_table)
			sql.WhereCondition("room_device_schedules.room_devices_id={} and room_schedules.room_schedules_day like '%{}%' or room_schedules.room_schedules_date='{}'".format(device[0], day, date))
			room_device_schedule_result = sql.FetchAll()

			for schedule in room_device_schedule_result:
				schedule_start_time = schedule[0]
				schedule_end_time = schedule[1]

				for num in range(schedule_start_time, schedule_end_time+1):
					room_device_schedule_array[device_count][int(num/8)] ^= (-1 ^ room_device_schedule_array[device_count][int(num/8)]) & (1 << num%8)

			device_count += 1

		room_schedule_data = []
		room_schedule_data.extend(room_schedule_array)
		for device_schedule in room_device_schedule_array:
			room_schedule_data.extend(device_schedule)

		# print room_schedule_data
		# print "\n"

		frame_id = GetFrameID(sql, row[0])

		room_addresses_data = GetRoomAddresses(sql, row[2])

		frame_count = int(math.ceil(len(room_schedule_data) / 70))
		for frame_number in range(frame_count):
			room_schedule_data_chunk = ""

			start_index = (frame_number*70)
			end_index = (frame_number*70)+69

			if end_index > len(room_schedule_data):
				end_index = len(room_schedule_data)

			for val in room_schedule_data[start_index:end_index]:
				room_schedule_data_chunk = "{} {}".format(room_schedule_data_chunk, val)

			# print room_schedule_data_chunk
			# print "\n"

			frame_data = "{} {} {} {} {}".format(room_addresses_data, frame_data, str(row[0]), str(frame_count-1), str(frame_number), room_schedule_data_chunk)

			# print frame_data
			# print "\n"

			room_schedule_frame = CreateFrame(frame_data, 9)

			print PrintFrameByteArray(room_schedule_frame)

			frame_sending_attempts = 1
			schedule.every(1).seconds.do(AcknowledgementTimer)

			acknowledgement_status = 0
			SendFrame()

def SendFrame(send_type=0):
	global acknowledgement_status

	#serial.write(room_schedule_frame)

	while True:
		if not send_type:
			schedule.run_pending()

		frame = receiver.ReceiveFrame()

		if frame:
			frame_data = ConvertDatasToInt(frame[7:-1])

			if frame_data[0]==6 and frame_data[frame_data[1]+3]==frame_id:
				acknowledgement_status = 1

				sql.UpdateQuery("frames", [["frames_status", 1]], "frames_id={}".format(frame_id))

				break

def AcknowledgementTimer():
	global acknowledgement_status

	if acknowledgement_status == 0:
		if frame_sending_attempts > 2:
			print "Stop expecting acknowledgement"
			# make log here
			schedule.CancelJob
		else:
			if frame_sending_attempts == 2:
				RouteRoomModules(receiver, sql)

			SendFrame(1)

			frame_sending_attempts += 1
	else:
		schedule.CancelJob
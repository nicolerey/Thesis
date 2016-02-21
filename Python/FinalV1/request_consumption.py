#!/usr/bin/python
import serial
import schedule
import datetime

from receive_frame import ReceiveClass

from SQL_class import SQLClass

from general_functions import *

from create_frame import CreateFrame

from route_room_modules import RouteRoomModules

sql = SQLClass()
frame_id = 0
rooms_id = 0
acknowledgement_status = 0
frame_sending_attempts = 0
consumption_frame = []

def RequestConsumption():
	global frame_id
	global rooms_id
	global frame_sending_attempts
	global room_consumption_frame

	now = datetime.datetime.now()
	time_now = (int(now.strftime("%H")) * 60) + int(now.strftime("%M"))
	day = now.strftime("%a")
	date = now.strftime("%Y-%m-%d")

	sql.SelectColumn("distinct rooms.rooms_address, rooms.rooms_id")
	sql.FromTable("room_schedules")
	sql.JoinTable([["rooms", "rooms.rooms_id=room_schedules.rooms_id"]])
	sql.WhereCondition("(room_schedules.room_schedules_day like '%{}%' or room_schedules.room_schedules_date='{}') and room_schedules.room_schedules_end_time={}".format(day, date, time_now))
	room_schedules_result = sql.FetchAll()

	for schedule in room_schedules_result:
		rooms_id = schedule[2]
		frame_id = GetFrameID(sql)
		room_addresses_data = GetRoomAddresses(sql, schedule[0])
		frame_data = "{} {} 0".format(room_addresses_data, frame_id)

		consumption_frame = CreateFrame(frame_data, 4)

		frame_sending_attempts = 1
		schedule.every(1).seconds.do(AcknowledgementTimer)

		SendFrame()

def SendFrame(send_type=0):
	global acknowledgement_status

	#serial.write(consumption_frame)

	while True:
		if not send_type:
			schedule.run_pending()

		#frame = receiver.ReceiveFrame()

		if frame:
			frame_data = ConvertDatasToInt(frame[7:-1])

			if frame_data[0]==6 and frame_data[frame_data[1]+3]==frame_id:
				now = datetime.datetime.now()
				date_time = now.strftime("%Y-%m-%d %H:%M:%S")

				acknowledgement_status = 1

				sql.UpdateQuery("frames", [["frames_status", 1]], "frames_id={}".format(frame_id))

				sql.InsertQuery("room_consumptions", "{}, {}, {}".format(rooms_id, frame_data[frame_data[1]+5], date_time))
				sql.Commit()

				break

def AcknowledgementTimer():
	global frame_sending_attempts

	if acknowledgement_status == 0:
		if frame_sending_attempts > 2:
			print "Stop expecting acknowledgement"
			schedule.CancelJob
		else:
			if frame_sending_attempts == 2:
				RouteRoomModules(receiver, sql)

			SendFrame(1)

			frame_sending_attempts += 1
	else:
		schedule.CancelJob

RequestConsumption()
#!/usr/bin/python
import sys
import schedule
import serial

#from receive_frame import ReceiveClass

from SQL_class import SQLClass

from general_functions import *

from create_frame import CreateFrame

frame_id = 0
rooms_id = 0
#receiver = ReceiveClass()
sql = SQLClass()
acknowledgement_status = 0
frame_sending_attempts = 0
room_status_frame = []

def ChangeStatus(roomID, status=1):
	global frame_id
	global rooms_id
	global frame_sending_attempts
	global room_status_frame

	rooms_id = roomID

	sql.GetWhereQuery("rooms", "rooms_id={}".format(roomID))
	sql.SelectColumn("rooms_port, rooms_address")
	rooms_result = sql.FetchOne()

	sql.GetWhereQuery("room_schedules", "room_schedules_type=1 and rooms_id={}".format(roomID))
	sql.SelectColumn("room_schedules_duration")
	room_schedules_result = sql.FetchOne()

	sql.GetWhereQuery("room_devices", "rooms_id={}".format(roomID))
	sql.SelectColumn("room_devices_port, room_devices_status")
	room_devices_result = sql.FetchAll()

	room_status_data = "{} {}".format(rooms_result[0], status)
	for device in room_devices_result:
		room_status_data = "{} {} {}".format(room_status_data, device[0], device[1])

	room_addresses_data = GetRoomAddresses(sql, rooms_result[1])
	frame_id = GetFrameID(sql)

	frame_data = "{} {} {}".format(room_addresses_data, frame_id, room_status_data)

	room_status_frame = CreateFrame(frame_data, 1)

	frame_sending_attempts = 1
	
	schedule.every(1).seconds.do(AcknowledgementTimer)

	SendFrame()

	if status == 1:
		schedule.every(room_schedules_result[0]).minutes.do(TurnOffRoom)
		while True:
			schedule.run_pending()

def TurnOffRoom():
	ChangeStatus(rooms_id, 0)

def SendFrame(send_type=0):
	global acknowledgement_status

	#serial.write(room_status_frame)

	while True:		
		#print PrintFrameByteArray(room_status_frame)
		if not send_type:
			schedule.run_pending()

		#frame = receiver.ReceiveFrame()

		if frame:
			frame_data = ConvertDatasToInt(frame[7:-1])

			if frame_data[0]==5 and frame_data[frame_data[1]+3]==frame_id:
				acknowledgement_status = 1

				sql.UpdateQuery("frames", [["frames_status", 1]], "frames_id={}".format(frame_id))
				sql.Commit()

				break

def AcknowledgementTimer():
	global frame_sending_attempts

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

ChangeStatus(sys.argv[1])
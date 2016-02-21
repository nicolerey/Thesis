#!/usr/bin/python
import sys
import serial
import schedule

from receive_frame import ReceiveClass

from SQL_class import SQLClass

from general_functions import *

from create_frame import CreateFrame

frame_id = 0
sql = SQLClass()
#receiver = ReceiveClass()
frame_sending_attempts = 0
acknowledgement_status = 0
room_status_frame = []

def CheckRoomStatus(roomsID):
	global frame_sending_attempts
	global room_status_frame

	sql.GetWhereQuery("rooms", "rooms_id={}".format(roomsID))
	sql.SelectColumn("rooms_address")
	rooms_result = sql.FetchOne()

	room_addresses_data = GetRoomAddresses(sql, rooms_result[0])
	frame_id = GetFrameID(sql)

	frame_data = "{} {} 0".format(room_addresses_data, frame_id)

	room_status_frame = CreateFrame(frame_data, 10)
	print PrintFrameByteArray(room_status_frame)

	frame_sending_attempts = 1
	schedule.every(1).seconds.do(AcknowledgementTimer)

	SendFrame()

def SendFrame(send_type=0):
	global acknowledgement_status

	#serial.write(room_status_frame)

	while True:
		if not send_type:
			schedule.run_pending()

		#frame = receiver.ReceiveFrame()

		if frame:
			frame_data = ConvertDatasToInt(frame[7:-1])

			if frame_data[0]==10 and frame_data[frame_data[1]+3]== frame_id:
				acknowledgement_status = 1

				sql.UpdateQuery("frames", [["frames_status", 1]], "frames_id={}".format(frame_id))
				sql.Commit()

				sql.UpdateQuery("rooms", [["rooms_status", frame_data[frame_data[1]+5]]], "rooms_id={}".format(frame_data[frame_data[1]+4]))
				sql.Commit()

				sql.GetWhereQuery("room_devices", "rooms_id={}".format(frame_data[frame_data[1]+4]))
				sql.SelectColumn("room_devices_id")
				room_devices_result = sql.FetchAll()

				device_status_index = 6
				for device in room_devices_result:
					sql.UpdateQuery("room_devices", [["room_devices_status", frame_data[frame_data[1]+device_status_index]]], "room_devices_id={}".format(device[0]))
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

CheckRoomStatus(sys.argv[1])
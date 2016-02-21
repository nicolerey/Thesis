#!/usr/bin/python
import sys
import serial
import schedule

from receive_frame import ReceiveClass

from SQL_class import SQLClass

from general_functions import *

from create_frame import CreateFrame

from route_room_modules import RouteRoomModules

frame_id = 0
#receiver = ReceiveClass()
sql = SQLClass()
acknowledgement_status = 0
frame_sending_attempts = 0
port_frame = []

def ChangePort(roomID):
	global frame_id
	global frame_sending_attempts
	global port_frame

	sql.GetWhereQuery("rooms", "rooms_id={}".format(roomID))
	sql.SelectColumn("rooms_port, rooms_address")
	rooms_result = sql.FetchOne()

	sql.GetWhereQuery("room_devices", "rooms_id={}".format(roomID))
	sql.SelectColumn("room_devices_port")
	room_devices_result = sql.FetchAll()

	port_data = "{} {}".format(rooms_result[0], len(room_devices_result))
	for device in room_devices_result:
		port_data = "{} {}".format(port_data, device[0])

	room_addresses_data = GetRoomAddresses(sql, rooms_result[1])
	frame_id = GetFrameID(sql)

	frame_data = "{} {} {}".format(room_addresses_data, frame_id, port_data)

	port_frame = CreateFrame(frame_data, 12)

	frame_sending_attempts = 1
	schedule.every(1).seconds.do(AcknowledgementTimer)

	SendFrame()

def SendFrame(send_type=0):
	global acknowledgement_status

	#serial.write(port_frame)

	while True:
		if not send_type:
			schedule.run_pending()

		#frame = receiver.ReceiveFrame()

		if frame:
			frame_data = ConvertDatasToInt(frame[7:-1])

			if frame_data[0]==6 and frame_data[frame_data[1]+3]==frame_id:
				acknowledgement_status = 1

				sql.UpdateQuery("frames", [["frames_status",1]], "frames_id={}".format(frame_id))
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

ChangePort(sys.argv[1])
#!/usr/bin/python
from __future__ import division

import datetime
import serial
import schedule

from general_functions import *

from create_frame import CreateFrame

from route_room_modules import RouteRoomModules

date_time_syncing_frame = []
receiver = []
sql = []
frame_id = 0
frame_sending_attempts = 0
acknowledgement_status = 0

def SyncRoomDateTime(rcvr_class, sql_class):
	global receiver
	global sql
	global date_time_syncing_frame
	global frame_id
	global frame_sending_attempts
	global acknowledgement_status

	receiver = rcvr_class
	sql = sql_class

	acknowledgement_status = 0
	frame_sending_attempts = 0

	sql.GetQuery("rooms")
	sql.SelectColumn("rooms_id, rooms_address")
	rooms_result = sql.FetchAll()

	for row in rooms_result:

		# get date and time
		now = datetime.datetime.now()

		hour = now.strftime("%H")
		minute = now.strftime("%M")
		second = now.strftime("%S")
		day = now.strftime("%d")
		month = now.strftime("%m")
		year = now.strftime("%Y")

		date_time_data = "{} {} {} {} {} {} {}".format(hour, minute, second, day, month, (int(year) >> 8) & 0xFF, int(year) & 0xFF)

		# get room addresses
		room_addresses_data = GetRoomAddresses(sql, row[1])
		# insert error check here

		# get frame id
		frame_id = GetFrameID(sql, row[0])

		# format frame data
		frame_data = "{} {} {}".format(room_addresses_data, frame_id, str(row[0]), date_time_data)

		date_time_syncing_frame = CreateFrame(frame_data, 3)

		#print PrintFrameByteArray(date_time_syncing_frame)

		frame_sending_attempts = 1
		scedule.every(1).seconds.do(AcknowledgementTimer)

		acknowledgement_status = 0
		SendFrame()

def SendFrame(send_type=0):
	global acknowledgement_status

	#serial.write(date_time_syncing_frame)

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
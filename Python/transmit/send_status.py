#!/usr/bin/python

import serial
import sys

from SQL_functions import SQLMain
from data_formatting_functions import FormatMain
from data_formatting_functions import PrintFrameByteArray
from file_functions import FileMain

def SendStatus(rooms_id):
	FileMain("Sending status data to modules")

	sql_args = ["rooms_address, rooms_port, rooms_status", "rooms", "rooms_id={}".format(rooms_id)]

	# query with select-fetchone argument
	# make query result into list
	rooms_results = list(SQLMain(sql_args, 0))

	if rooms_results:
		# format sql arguments for room_devices table query
		sql_args = ["room_devices_port, room_devices_status", "room_devices", "rooms_id={}".format(rooms_id)]

		# query with select-fetchall argument
		roomDevices_results = SQLMain(sql_args, 1)

	if rooms_results != None:
		if rooms_results != 0:
			# write to file  if room table query return is not empty
			if roomDevices_results:
				for row in roomDevices_results:
					rooms_results.extend(row)

	# format DB results to be sent to modules
	formatted_frame = FormatMain(rooms_results, 1)

	# format and print formatted frame in terminal
	frame = PrintFrameByteArray(formatted_frame)

	FileMain("Frame: {}".format(frame))

	# send frame to serial
	#serial.write(formatted_frame)

# check whether function call from change_status_on_schedule or php file
if len(sys.argv) == 2:
	SendStatus(sys.argv[1])
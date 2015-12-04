#!/usr/bin/python

# import system and serial library functions
import serial
import sys

# import specific functions from other python files
from SQL_functions import SQLMain
from data_formatting_functions import FormatMain
from data_formatting_functions import PrintFrameByteArray

# format sql arguments for room table query
sql_args = ["rooms_address, rooms_port, rooms_status", "rooms", "rooms_id=%d" % int(sys.argv[1])]

# query with select-fetchone argument
# make query result into list
rooms_results = list(SQLMain(sql_args, 0))

if rooms_results:
	# format sql arguments for room_devices table query
	sql_args = ["room_devices_port, room_devices_status", "room_devices", "rooms_id = %d" % int(sys.argv[1])]

	# query with select-fetchall argument
	roomDevices_results = SQLMain(sql_args, 1)

# open log file for appending
file = open('log.txt', 'a')

if rooms_results != None:
	if rooms_results == 0:
		# write to file if room table query return is empty
		file.write("Empty result set in DB query\n")
	else:
		# write to file  if room table query return is not empty
		file.write("Retrieved data from DB: %s\n" % str(rooms_results))
		if roomDevices_results:
			for row in roomDevices_results:
				rooms_results.extend(row)
				file.write("\t%s\n" % str(row))

# close file
file.close()

print rooms_results

# format DB results to be sent to modules
formatted_frame = FormatMain(rooms_results)

# format and print formatted frame in terminal
PrintFrameByteArray(formatted_frame)

# send frame to serial
#serial.write(formatted_frame)
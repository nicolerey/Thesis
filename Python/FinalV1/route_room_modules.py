#!/usr/bin/python
import serial

from create_frame import CreateFrame

from general_functions import PrintFrameByteArray

def RouteRoomModules(receiver, sql):
	# empty ARP frame
	ARP_data_frame = "00 00 00 00 00 00 00 00"

	ARP_frame = CreateFrame(ARP_data_frame, 5)

	print PrintFrameByteArray(ARP_frame)

	#serial.write(ARP_frame)

	while True:
		frame = receiver.ReceiveFrame()

		if frame[7] == 8:
			sql.DeleteQuery("rooms_routing_table", "rooms_routing_table_id>0")
			sql.Commit()

			for addr in frame[10:15]:
				sql.InsertQuery("rooms_routing_table", addr)
				sql.Commit()
				# add query checking here

			break;
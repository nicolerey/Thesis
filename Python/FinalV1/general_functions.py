#!/usr/bin/python

def GetRoomAddresses(sql, rooms_address):
	sql.GetWhereQuery("rooms_routing_table", "rooms_address={}".format(rooms_address))
	sql.SelectColumn("rooms_routing_table_id")
	routing_table_result = sql.FetchOne()

	if routing_table_result:
		sql.GetWhereQuery("rooms_routing_table", "rooms_routing_table_id<={}".format(routing_table_result[0]))
		sql.SelectColumn("rooms_address")
		routing_table_result = sql.FetchAll()

		room_addresses_data = "{} 1".format(str(len(routing_table_result)))
		for addr in routing_table_result:
			room_addresses_data = "{} {}".format(room_addresses_data, addr[0])

		return room_addresses_data
	else:
		return False

def GetFrameID(sql):
	sql.GetLastID("frames", "frames_id")

	if sql.FetchOne()[0] > 254:
		sql.DeleteQuery("frames", "frames_status=1")
		sql.Commit()

	sql.InsertQuery("frames", "0")
	sql.Commit()

	sql.GetLastID("frames", "frames_id")

	return str(sql.FetchOne()[0])

def PrintFrameByteArray(frame):
	return " ".join("{:02x}".format(byte_val) for byte_val in frame)

def ConvertDatasToInt(data):
	frame_data = []
	for dat in data:
		frame_data.append(int(dat))

	return frame_data
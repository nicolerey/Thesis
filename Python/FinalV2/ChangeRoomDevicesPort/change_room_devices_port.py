from time import sleep
from SQL_class import SQLClass
from general_functions import ConvertIntToHex

import XBee_Threaded
import sys

sql = SQLClass()

# xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

def ChangeRoomDevicesPort(rooms_id):
	sql.GetWhereQuery("rooms", "rooms_id={}".format(rooms_id))
	sql.SelectColumn("rooms_port, rooms_address")
	rooms_result = sql.FetchOne()

	sql.GetWhereQuery("room_devices", "rooms_id={}".format(rooms_id))
	sql.SelectColumn("room_devices_port")
	room_devices_result = sql.FetchAll()

	port_data = "09 {} {}".format(ConvertIntToHex(rooms_result[0]), ConvertIntToHex(len(room_devices_result)))
	for device_port in room_devices_result:
		port_data = "{} {}".format(port_data, ConvertIntToHex(device_port[0]))

	port_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(port_data))), port_data)

	while True:
		xbee.Send(bytearray.fromhex(port_data), rooms_result[1])
		time.sleep(1);

		msg = xbee.Receive();
		if msg[14]==11:
			xbee.shutdown()
			break

ChangeRoomDevicesPort(sys.argv[1])
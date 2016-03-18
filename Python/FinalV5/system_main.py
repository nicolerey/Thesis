import thread
import time
import XBee_Threaded
import datetime
import schedule
import struct

from change_room_device_port import ChangeRoomDevicePort
from change_room_device_status import ChangeRoomDeviceStatus
from send_room_schedule import SendRoomSchedules

from SQL_class import SQLClass
from general_functions import ConvertIntToHex

#prev_address = ""

def ReceiveXBeeData():
	xbee = XBee_Threaded.XBee("/dev/ttyUSB0")
	#global prev_address

	msg = xbee.Receive()
	if msg:
		if msg[14]==2:
			addr = ""
			for byte_data in msg[3:11]:
				addr = "{} {}".format(addr, ConvertIntToHex(byte_data))

			#if addr!=prev_address:
				#prev_address = addr

			now = datetime.datetime.now()

			hour = ConvertIntToHex(now.strftime("%H"))
			minute = ConvertIntToHex(now.strftime("%M"))
			second = ConvertIntToHex(now.strftime("%S"))
			day = ConvertIntToHex(now.strftime("%d"))
			month = ConvertIntToHex(now.strftime("%m"))
			year = int(now.strftime("%Y"))

			date_time_data = "03 {} {} {} {} {} {} {}".format(
				hour,
				minute,
				second,
				day,
				month,
				ConvertIntToHex((year >> 8) & 0xFF),
				ConvertIntToHex(year & 0xFF)
				)			

			date_time_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(date_time_data))), date_time_data)

			xbee.Send(bytearray.fromhex(date_time_data), addr)
		elif msg[14]==7:
			sql = SQLClass()

			sql.UpdateQuery("rooms", [["rooms_status", msg[16]]], "rooms_id={}".format(msg[15]))
			sql.Commit()

			sql.GetWhereQuery("room_devices", "rooms_id={}".format(msg[15]))
			sql.SelectColumn("room_devices_id")
			room_devices_result = sql.FetchAll()

			x = 0
			for device in room_devices_result:
				sql.UpdateQuery("room_devices", [["rooms_status", msg[17+x]]], "room_devices_id={}".format(device[0]))
				sql.Commit()

				x += 1
		elif msg[14]==5:
			curr_data = struct.unpack('d', msg[17:(17+msg[16])])

			sql.InsertQuery("room_consumptions", "{}, {}, CURRENT_TIMESTAMP".format(msg[15], curr_data))
			sql.Commit()

	xbee.shutdown()

def PerformTriggerFunctions():
	sql = SQLClass()

	sql.GetQuery("triggers")
	triggers_result = sql.FetchAll()

	for trigger in triggers_result:
		if trigger[2]==1:
			ChangeRoomDevicePort(trigger[1])
		elif trigger[2]==2:
			sql.GetWhereQuery("room_schedules", "room_schedules_id={}".format(trigger[1]))
			sql.SelectColumn("rooms_id")
			room_schedules_result = sql.FetchOne()

			SendRoomSchedules(trigger[1], room_schedules_result[0])
		elif trigger[2]==3:
			SendRoomSchedules(trigger[1])
		elif trigger[2]==4:
			break;
		elif trigger[2]==5:
			ChangeRoomDeviceStatus(trigger[1])

		sql.DeleteQuery("triggers", "triggers_id={}".format(trigger[0]))
		sql.Commit()
	sql.CloseConnection()

while True:
	print "nicole"
	ReceiveXBeeData()
	time.sleep(0.2)
	#PerformTriggerFunctions()
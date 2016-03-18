import schedule
import time
import datetime
import XBee_Threaded
import thread
import struct
import array

from change_room_device_port import ChangeRoomDevicePort
from change_room_device_status import ChangeRoomDeviceStatus
from send_room_schedule import SendRoomSchedules

from SQL_class import SQLClass
from general_functions import ConvertIntToHex

xbee = XBee_Threaded.XBee("/dev/ttyUSB0")

def ReceiveXBeeData(val):
	msg = xbee.Receive()
	if msg:
		if msg[14]==2:
			addr = ""
			for byte_data in msg[3:11]:
				addr = "{} {}".format(addr, ConvertIntToHex(byte_data))

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

			sql.CloseConnection()
		elif msg[14]==5:
			sql = SQLClass()

			curr_data = float((array.array('B', msg[17:-1]).tostring()).strip("\x00"))

			sql.InsertQuery("room_consumptions", "{}, {}, CURRENT_TIMESTAMP".format(msg[15], curr_data))
			sql.Commit()

			print sql.ReturnQueryStatement()

			sql.CloseConnection()

def PerformTriggerFunctions(val):
	sql = SQLClass()

	sql.GetQuery("triggers")
	triggers_result = sql.FetchAll()

	for trigger in triggers_result:
		if trigger[2]==1:
			ChangeRoomDevicePort(sql, xbee, trigger[1])
		elif trigger[2]==2:
			sql.GetWhereQuery("room_schedules", "room_schedules_id={}".format(trigger[1]))
			sql.SelectColumn("rooms_id")
			room_schedules_result = sql.FetchOne()

			SendRoomSchedules(sql, xbee, room_schedules_result[0], trigger[1])
		elif trigger[2]==3:
			SendRoomSchedules(sql, xbee, trigger[1], False)
		elif trigger[2]==4:
			break;
		elif trigger[2]==5:
			ChangeRoomDeviceStatus(sql, xbee, trigger[1])

		sql.DeleteQuery("triggers", "triggers_id={}".format(trigger[0]))
		sql.Commit()
	sql.CloseConnection()

def ExecuteOperation():
	thread.start_new_thread(ReceiveXBeeData, (0, ))
	thread.start_new_thread(PerformTriggerFunctions, (0, ))

schedule.every(1).seconds.do(ExecuteOperation)

while 1:
	schedule.run_pending()
	time.sleep(1)
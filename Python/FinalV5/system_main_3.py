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

			status_update_flag = 0

			sql.UpdateQuery("rooms", [["rooms_status", msg[16]]], "rooms_id={}".format(msg[15]))
			sql.Commit()

			if msg[16]:
				sql.InsertQuery("status_logs", "{}, 1, CURRENT_TIMESTAMP".format(msg[15]))
				sql.Commit()

				status_update_flag = 1

			sql.GetWhereQuery("room_devices", "rooms_id={}".format(msg[15]))
			sql.SelectColumn("room_devices_id")
			room_devices_result = sql.FetchAll()

			if msg[17]:
				x = 0
				for device in room_devices_result:
					sql.UpdateQuery("room_devices", [["rooms_status", msg[17+x]]], "room_devices_id={}".format(device[0]))
					sql.Commit()

					if not status_update_flag:
						if msg[17+x]:
							sql.InsertQuery("status_logs", "{}, 1, CURRENT_TIMESTAMP".format(msg[15]))
							sql.Commit()

							status_update_flag = 1

					x += 1

			if not status_update_flag:
				sql.InsertQuery("status_logs", "{}, 0, CURRENT_TIMESTAMP".format(msg[15]))
				sql.Commit()

			sql.UpdateQuery("check_rooms", [["check_rooms_status", 1]], "check_rooms_id>0")
			sql.Commit()

			sql.UpdateQuery("check_devices", [["check_devices_status", 1]], "rooms_id={}".format(msg[15]))
			sql.Commit()

			sql.CloseConnection()
		elif msg[14]==5:
			sql = SQLClass()

			curr_data = float((array.array('B', msg[17:-1]).tostring()).strip("\x00"))/3600

			sql.InsertQuery("room_consumptions", "{}, {}, CURRENT_TIMESTAMP".format(msg[15], curr_data))
			sql.Commit()

			sql.CloseConnection()

def PerformTriggerFunctions(val):
	sql = SQLClass()

	sql.GetQuery("triggers")
	triggers_result = sql.FetchAll()

	for trigger in triggers_result:
		trigger_type = trigger[2]
		trigger_table_id = trigger[1]

		sql.DeleteQuery("triggers", "triggers_id={}".format(trigger[0]))
		sql.Commit()
		
		if trigger_type==1:
			ChangeRoomDevicePort(sql, xbee, trigger_table_id)
		elif trigger_type==2:
			sql.GetWhereQuery("room_schedules", "room_schedules_id={}".format(trigger_table_id))
			sql.SelectColumn("rooms_id")
			room_schedules_result = sql.FetchOne()

			SendRoomSchedules(sql, xbee, room_schedules_result[0], trigger_table_id)
		elif trigger_type==3:
			SendRoomSchedules(sql, xbee, trigger_table_id, False)
		elif trigger_type==4:
			break;
		elif trigger_type==5:
			ChangeRoomDeviceStatus(sql, xbee, trigger_table_id)
		elif trigger_type==6:
			sql.GetWhereQuery("rooms", "rooms_id={}".format(trigger_table_id))
			sql.SelectColumn("rooms_address")
			rooms_result = sql.FetchOne()

			cancel_manual = "2E 01 10"
			xbee.Send(bytearray.fromhex(cancel_manual), rooms_result[0])
		elif trigger_type==7 or trigger_type==8:
			sql.GetWhereQuery("room_schedules", "room_schedules_id={} and room_schedules_type=0".format(trigger_table_id))
			sql.SelectColumn("rooms_id")
			room_schedules_result = sql.FetchOne()

			sql.GetWhereQuery("rooms", "rooms_id={}".format(room_schedules_result[0]))
			sql.SelectColumn("rooms_address")
			rooms_result = sql.FetchOne()

			if trigger_type==7:
				sql.GetWhereQuery("room_devices", "rooms_id={}".format(room_schedules_result[0]))
				sql.SelectColumn("room_devices_id")
				room_devices_result = sql.FetchAll()

				if rooms_result:
					schedule_date_data = "11 01 {}".format(ConvertIntToHex(len(room_devices_result)))
					for device in room_devices_result:
						sql.GetWhereQuery("room_device_schedules", "room_devices_id={} and room_schedules_id={}".format(device[0], trigger_table_id))
						sql.SelectColumn("room_device_schedules_id")
						room_device_schedules_result = sql.FetchOne()

						device_status = "00"
						if room_device_schedules_result:
							device_status = "01"

						schedule_date_data = "{} {}".format(schedule_date_data, device_status)

					schedule_date_data = "2E {} {}".format(ConvertIntToHex(len(bytearray.fromhex(schedule_date_data))), schedule_date_data)
					xbee.Send(bytearray.fromhex(schedule_date_data), rooms_result[0])
			elif trigger_type==8:
				schedule_date_data = "2E 01 12"
				xbee.Send(bytearray.fromhex(schedule_date_data), rooms_result[0])

	sql.CloseConnection()

def ExecuteOperation():
	thread.start_new_thread(ReceiveXBeeData, (0, ))
	thread.start_new_thread(PerformTriggerFunctions, (0, ))

def CheckDateSchedule():
	now = datetime.datetime.now()

	date_now = now.strftime("%Y-%m-%d")
	time_now = (int(now.strftime("%H")) * 60) + int(now.strftime("%M"))

	sql = SQLClass()

	sql.GetWhereQuery("room_schedules", "room_schedules_date='{}' and room_schedules_type=0 and (room_schedules_start_time={} or room_schedules_end_time={})".format(date_now, time_now, time_now))
	sql.SelectColumn("room_schedules_id, room_schedules_start_time, room_schedules_end_time")
	room_schedules_result = sql.FetchAll()

	print date_now
	print time_now
	print room_schedules_result

	trigger_type = ""
	for schedule in room_schedules_result:
		if time_now==schedule[1]:
			trigger_type = "7"
		elif time_now==schedule[2]:
			trigger_type = "8"

		sql.InsertQuery("triggers", "{}, {}".format(schedule[0], trigger_type))
		sql.Commit()

	sql.CloseConnection()

schedule.every(1).seconds.do(ExecuteOperation)
schedule.every(1).minutes.do(CheckDateSchedule)

CheckDateSchedule()

while 1:
	schedule.run_pending()
	time.sleep(1)
#!/usr/bin/python

import serial
import time

from collections import deque
from SQL_functions import SQLMain

RX_buffer = bytearray()
RX_msg = deque()

def ReceiveData():
	remaining_byte = serial.inWaiting()
	while remaining:
		byte_chunk = serial.read(remaining_byte)
		remaining_byte -= len(byte_chunk)
		RX_buffer.extend(byte_chunk)

	message_array = RX_buffer.split(byte(b'\x7E'))
	for message in message_array[:-1]:
		RX_msg.append(ValidateMessage(message))

	if ValidateMessage(message_array[-1]):
		RX_buffer = bytearray()
	else:
		RX_buffer = message_array[-1]

	return RX_msg.popleft()

	# if frame[7] == 0x04:
	# 	ChangeAcknowledgementStatus(frame)
	# else:
	# 	SaveRoomConsumption(frame)

def ChangeAcknowledgementStatus(frame):
	sql_args = ["rooms_id", "rooms", "rooms_address={}".format(frame[9])]
	rooms_result = SQLMain(sql_args, 0)

	sql_args = ["acknowledgement_status", "0, {}, {}".format(rooms_result[0], frame[10])]
	SQLMain(sql_args, 3)

def SaveRoomConsumption(frame):
	sql_args = ["rooms_id", "rooms", "rooms_address={}".format(frame[9])]
	rooms_result = SQLMain(sql_args, 0)

	sql_args = ["room_consumptions", "0, {}, {}".format(rooms_result[0], frame[10])]

def ValidateMessage(message):
	if (len(message) - msg.count(bytes(b'0x7D'))) < 9:
		return False

	frame = UnescapeFrameFunctions(message)

	LSB = frame[1]
	if LSB > (len(frame[2:]) - 1):
		return False

	if (sum(frame[2:3+LSB]) & 0xFF) != 0xFF:
		return False

	return frame

def UnescapeFrameFunctions(message):
	if message[-1] == 0x7D:
		return None

	unescape_frame = bytearray()
	skip = False
	for i in range(len(message)):
		if skip:
			skip = False
			continue

		if message[i] = 0x7D:
			unescape_frame.append(message[i+1] ^ 0x20)
			skip = True
		else:
			unescape_frame.append(message[i])

	return unescape_frame
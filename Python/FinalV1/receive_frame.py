#!/usr/bin/python
import serial
from collections import deque

class ReceiveClass():
	RxBuff = bytearray()
	RxMessages = deque()

	def __init__(self, serialport, baudrate=9600):
		self.serial = serial.Serial(port=serialport, baudrate=baudrate)

	def ReceiveFrame(self):
		remaining_in_buff = self.serial.inWaiting()
		while remaining_in_buff:
			data_chunk = self.serial.read(remaining_in_buff)
			remaining_in_buff -= len(data_chunk)
			self.RxBuff.extend(data_chunk)

		messages = self.RxBuff.split(bytes(b'\x7E'))
		for msg in messages[:-1]:
			self.ValidateFrame(msg)

		self.RxBuff = (bytearray() if self.ValidateFrame(messages[-1]) else messages[-1])

		if self.RxMessages:
			return self.RxMessages.popleft()
		else:
			return None

	def ValidateFrame(self, message):
		if(len(message) - message.count(bytes(b'0x7D'))) < 9:
			return False

		frame = self.UnescapeFrame(message)

		LSB = frame[1]

		if LSB > (len(frame[2:]) - 1):
			return False

		if (sum(frame[2:3+LSB]) & 0xFF) != 0xFF:
			return False

		self.RxMessages.append(frame)
		return True

	def UnescapeFrame(self, message):
		if message[-1] == 0x7D:
			return None

		out = bytearray()
		skip = False
		for i in range(len(message)):
			if skip:
				skip = False
				continue

			if message[i] == 0x7D:
				out.append(message[i+1] ^ 0x20)
				skip = True
			else:
				out.append(message[i])

		return out
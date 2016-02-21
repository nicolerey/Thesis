#!/usr/bin/python

def CreateFrame(data, frame_type):
	if frame_type != 5:
		frame_data = ConvertDataToHex(data, frame_type)
	else:
		frame_data = "{} {}".format(str(frame_type).zfill(2), data)

	frame_headers = AddFrameHeaders(frame_data)

	frame = bytearray.fromhex("{} {}".format(frame_headers, frame_data))
	frame.append(0xFF - (sum(frame[3:]) & 0xFF))

	frame = EscapeFrameCharacters(frame)

	return frame

def ConvertDataToHex(data, frame_type):
	data = data.split(" ")
	frame_data = ""
	for val in data:
		frame_data += " {}".format(hex(int(str(val)))[2:].zfill(2).rstrip("L"))

	return "{}{}".format(hex(frame_type)[2:].zfill(2), frame_data)

def AddFrameHeaders(frame_data):
	frame_id = 0x00
	options = 0x01

	frame_headers = "7E 00 {:02X} 01 {:02X} {} {} {:02X}".format(
		len(frame_data.split(" ")) + 5,
		frame_id,
		(frame_data.split(" "))[3],
		(frame_data.split(" "))[4],
		options
	)

	return frame_headers

def EscapeFrameCharacters(frame):
	escaped_frame = bytearray()
	reserved_byte = bytearray(b"\x7E\x7D\x11\x13")

	escaped_frame.append(frame[0])

	for byte_val in frame[1:]:
		if byte_val in reserved_byte:
			escaped_frame.append(0x7D)
			escaped_frame.append(byte_val ^ 0x20)
		else:
			escaped_frame.append(byte_val)

	return escaped_frame
#!/usr/bin/python

def FormatMain(VAL_data):
	result = CombineSQLData(VAL_data)

	result = CombineHeadersAndData(bytearray.fromhex(result))

	result = EscapeFrameCharacters(result)

	return result

def CombineSQLData(VAL_data):

	combined_val = ''
	for row in VAL_data:
		hex_val = hex(row)[2:].zfill(2).rstrip("L")

		if len(hex_val) == 1:
			hex_val = "0"+hex_val

		combined_val = combined_val + hex_val + " "

	return combined_val.strip()

def CombineHeadersAndData(VAL_data, addr=0xFFFF, options=0x01, frameid=0x00):
	frame_header = '7E 00 {:02X} 01 {:02X} {:02X} {:02X} {:02X}'.format(
			len(VAL_data) + 5,
			frameid,
			(addr & 0xFF00) >> 8,
			addr & 0xFF,
			options
		)

	frame = bytearray.fromhex(frame_header)

	frame.extend(VAL_data)

	frame.append(0xFF - (sum(frame[3:]) & 0xFF))

	return frame

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

def PrintFrameByteArray(frame):
	print " ".join("{:02x}".format(byte_val) for byte_val in frame)
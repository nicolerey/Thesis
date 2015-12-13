#!/usr/bin/python

import datetime

def FileMain(message, continuity_flag=0, filename="log", file_mode="a"):
	now = datetime.datetime.now()
	date_time = now.strftime("%Y-%m-%d %H:%M:%S")

	file = open(filename, file_mode)

	if continuity_flag:
		file.write("\t{}".format(message))
	else:
		file.write("\n{}> {}".format(date_time, message))
	file.write("\n")

	file.close()
#!/usr/bin/python

import datetime

from data_formatting_functions import FormatMain
from data_formatting_functions import PrintFrameByteArray
from file_functions import FileMain

FileMain("Syncing time and date of modules")

# get datetime now
now = datetime.datetime.now()
# get hour (01-24) value from now variable
hour = now.strftime("%H")
# get minute value from now variable
minute = now.strftime("%M")
# get second value from now variable
second = now.strftime("%S")
# get day of month (01-31) value in now variable
day = now.strftime("%d")
# get month (01-12) value in now variable
month = now.strftime("%e")
# get year value with century ex. 2015
year = now.strftime("%Y")

# format data values
data = "{:02X} {:02X} {:02X} {:02X} {:02X} {:02X}".format(hour, minute, second, day, month, year)

# format headers and data values and combine them
formatted_frame = FormatMain(data, 2)

PrintFrameByteArray(formatted_frame)

# send formatted frame
#serial.write(formatted_frame)
#!/usr/bin/python
import serial
import datetime

from SQL_class import SQLClass

from route_room_modules import RouteRoomModules

from sync_room_date_time import SyncRoomDateTime

from receive_frame import ReceiveClass


# Create Receive object
receiver = ReceiveClass()
# Create SQL object
sql = SQLClass()

# Routing Modules
RouteRoomModules(receiver, sql)

# Syncing Modules' Date & Time
SyncRoomDateTime(receiver, sql)
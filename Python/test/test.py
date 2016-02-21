#!/usr/bin/python

from SQL_class import SQLClass

sql = SQLClass()
sql.InsertQuery("room_consumptions", "1, 255, NULL")
print sql.ReturnQueryStatement()
sql.Commit()
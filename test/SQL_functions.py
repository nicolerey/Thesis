#!/usr/bin/python

import MySQLdb

def SQLMain(SQL_argument, SQL_type, SQL_server="localhost", SQL_username="root", SQL_password="root", SQL_database="ThesisDB"):
	results = None

	db = ConnectToDB(SQL_server, SQL_username, SQL_password, SQL_database)
	cursor = db.cursor()
	
	if SQL_type < 3:
		results = SelectDataFromDB(SQL_argument, cursor, SQL_type)

		if not results:
			results = 0
	else:
		InsertUpdateDeleteToDB(SQL_argument, cursor, db, SQL_type)

	db.close()

	return results

def ConnectToDB(SQL_server, SQL_username, SQL_password, SQL_database):
	db = MySQLdb.connect(SQL_server, SQL_username, SQL_password, SQL_database)
	return db

# 0 - Select: fetchone()
# 1 - Select: fetchall()
# 2 - Select: rowcount
def SelectDataFromDB(SQL_argument, cursor, SQL_type):
	sql = "select "+SQL_argument[0]+" from "+SQL_argument[1]

	if len(SQL_argument) == 3 :
		sql = sql+" where "+SQL_argument[2]

	try:
		cursor.execute(sql)

		if SQL_type == 0:
			results = cursor.fetchone()
		elif SQL_type == 1:
			results = cursor.fetchall()
		elif SQL_type == 2:
			results = cursor.rowcount;
		else:
			results = "Invalid SQL Query type"
	except:
		results = "Failed to retrieve data from DB"

	return results

# 3 - Insert
# 4 - Update
# 5 - Delete
def InsertUpdateDeleteToDB(SQL_argument, cursor, db, SQL_type):
	if SQL_type == 3:
		sql = "insert into "+SQL_argument[0]+" values("+SQL_argument[1]+")"
		sql_query_type = "Insert"
	elif SQL_type == 4:
		sql = "update "+SQL_argument[0]+" set "+SQL_argument[1]+" where "+SQL_argument[2]
		sql_query_type = "Update"
	elif SQL_type == 5:
		sql = "delete from "+SQL_argument[0]+" where "+SQL_argument[1]
		sql_query_type = "Delete"

	if sql:
		try:
			cursor.execute(sql)

			db.commit()

			rtn = sql_query_type+" to DB a success"
		except:
			db.rollback()
			rtn = sql_query_type+" to DB failed"

		return rtn
	else:
		return "Empty query parameter"
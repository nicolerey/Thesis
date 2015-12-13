#!/usr/bin/python

# import MySQLdb Library
import MySQLdb

from file_functions import FileMain

# main function in python file
def SQLMain(SQL_argument, SQL_type, SQL_server="localhost", SQL_username="root", SQL_password="root", SQL_database="ThesisDB"):
	results = None

	FileMain("Creating database connection")

	# create and open connection to database using parameter values
	db = ConnectToDB(SQL_server, SQL_username, SQL_password, SQL_database)
	cursor = db.cursor()
	
	# identify what type of query needs to be done
	# if SQL_type < 3, do SELECT query
	if SQL_type < 3:
		# pass arguments to select function

		FileMain("Perform SELECT query in database")

		results = SelectDataFromDB(SQL_argument, cursor, SQL_type)

		# if query result is empty return 0
		if not results:
			results = 0
	else:
		FileMain("Perform INSERT/UPDATE/DELETE query in database")

		# pass arguments to insert, update, and delete functions
		InsertUpdateDeleteToDB(SQL_argument, cursor, db, SQL_type)

	FileMain("Close database connection")

	# close database connection
	db.close()

	FileMain("Query result: {}".format(str(results)))

	# return query results
	return results

# function to create and open connection to database
def ConnectToDB(SQL_server, SQL_username, SQL_password, SQL_database):
	# make connection to database
	db = MySQLdb.connect(SQL_server, SQL_username, SQL_password, SQL_database)

	# return connection to main function
	return db

# 0 - SELECT: fetchone()
# 1 - SELECT: fetchall()
# 2 - SELECT: rowcount
# function for select queries
def SelectDataFromDB(SQL_argument, cursor, SQL_type):
	# formulate select query statement
	sql = "select "+SQL_argument[0]+" from "+SQL_argument[1]

	# check if a where clause in query statement needs to be added
	if len(SQL_argument) == 3 :
		# add where clause to statement
		sql = sql+" where "+SQL_argument[2]

	print sql

	try:
		# execute query
		cursor.execute(sql)

		# identify what type of select needs to be done
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

	# return query results to main function
	return results

# 3 - INSERT
# 4 - UPDATE
# 5 - DELETE
# function for insert, update, and delete queries
def InsertUpdateDeleteToDB(SQL_argument, cursor, db, SQL_type):
	# identify whether to perform insert, update, or delete query
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
			# execute query
			cursor.execute(sql)

			# commit query changes to DB
			db.commit()

			rtn = sql_query_type+" to DB a success"
		except:
			# revert changes to DB
			db.rollback()

			rtn = sql_query_type+" to DB failed"

		return rtn
	else:
		return "Empty query parameter"
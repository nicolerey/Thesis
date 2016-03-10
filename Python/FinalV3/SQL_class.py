#!/usr/bin/python
import MySQLdb

class SQLClass:
	def __init__(self, server="localhost", username="root", password="root", database = "ThesisDB"):
		self.SQL_server = server
		self.SQL_username = username
		self.SQL_password = password
		self.SQL_database = database

		self.SQL_connection = MySQLdb.connect(self.SQL_server, self.SQL_username, self.SQL_password, self.SQL_database)
		self.SQL_cursor = self.SQL_connection.cursor()

		self.SQL_query = []

	def GetLastID(self, table, column_name):
		self.SQL_query = "select max({}) from {}".format(column_name, table)

	def GetQuery(self, table, limit=0, offset=0):
		self.SQL_query = "select * from {}".format(table)
		if limit:
			self.SQL_query += " limit {}".format(limit)
		if offset:
			self.SQL_query += " offset {}".format(offset)

	def GetWhereQuery(self, table, condition, limit=0, offset=0):
		self.SQL_query = "select * from {} where {}".format(table, condition)
		if limit != 0:
			self.SQL_query += " limit {}".format(limit)
		if offset != 0:
			self.SQL_query += " offset {}".format(offset)

	def SelectColumn(self, select):
		if not self.SQL_query:
			self.SQL_query = "select {}".format(select)
		else:
			self.SQL_query = self.SQL_query.replace("*", select)

	def FromTable(self, table):
		self.SQL_query += " from {}".format(table)

	def JoinTable(self, join_condition):
		for table in join_condition:
			self.SQL_query += " join {} on {}".format(table[0], table[1])

	def WhereCondition(self, condition):
		self.SQL_query += " where {}".format(condition)

	def InsertQuery(self, table, values):
		self.SQL_query = "insert into {} values(0, {})".format(table, values)

	def UpdateQuery(self, table, values, condition):
		self.SQL_query = "update {} set".format(table)
		for val in values:
			self.SQL_query += " {}={},".format(val[0], val[1])
		self.SQL_query = self.SQL_query.rstrip(",")
		if condition:
			self.SQL_query += " where {}".format(condition)

	def DeleteQuery(self, table, condition):
		self.SQL_query = "delete from {} where {}".format(table, condition)

	def FetchOne(self):
		self.SQL_cursor.execute(self.SQL_query)

		return self.SQL_cursor.fetchone()

	def FetchAll(self):
		self.SQL_cursor.execute(self.SQL_query)

		return self.SQL_cursor.fetchall()

	def RowCount(self):
		self.SQL_cursor.execute(self.SQL_query)

		return self.SQL_cursor.rowcount()

	def Commit(self):
		rtn = True

		try:
			self.SQL_cursor.execute(self.SQL_query)

			self.SQL_connection.commit()
		except:
			self.SQL_connection.rollback()

			rtn = False

		return rtn

	def ReturnQueryStatement(self):
		return self.SQL_query

	def CloseConnection(self):
		self.SQL_connection.close()
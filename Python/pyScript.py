import MySQLdb

db = MySQLdb.connect("localhost", "root", "root", "ThesisDB")
cursor = db.cursor()

sql = "select * from floors"

try:
	cursor.execute(sql)

	results = cursor.rowcount()

	print results
	#x = 1
	#for r in results:
		#print r[x]
		#x += 1
except:
	print "Error: unable to fetch data"

db.close()
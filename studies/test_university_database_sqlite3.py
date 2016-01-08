import sqlite3
con = sqlite3.connect('university.db')
cur = con.cursor()
cur.execute('select * from Student limit 100')
row = cur.fetchone()
while row:
	#print (row[0],row[1],row[2],row[3],row[4])
	for m in row:
		print(m)
	row = cur.fetchone()
cur.close()
con.close()
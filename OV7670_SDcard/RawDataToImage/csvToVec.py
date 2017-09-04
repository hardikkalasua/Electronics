import csv
import sys
import binascii
csv.field_size_limit(500 * 1024 * 1024)

columnvector = []
with open('data.csv', 'rb') as csvfile:
	csvreader = csv.reader(csvfile,delimiter=' ', quotechar='|')
	for row in csvreader:
		columnvector.append(row)


headers =['42','4D','36','84','03','00','00','00','00','00','36','00','00','00','28','00','00','00',
'40','01','00','00','F0','00','00','00','01','00','18','00','00','00','00','00','00','84','03','00','C5','00',
'00','00','C5','00','00','00','00','00','00','00','00','00','00','00']

hexArray=[]
for i in range(0,76800):
	data = columnvector[0][i]
	hexArray.extend([data,data,data])



with open('test.bmp', 'wb') as f:
    f.write(binascii.unhexlify(''.join(headers)))
    f.write(binascii.unhexlify(''.join(hexArray)))


        






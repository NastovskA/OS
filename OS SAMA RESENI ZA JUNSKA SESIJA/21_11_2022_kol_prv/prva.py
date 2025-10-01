#!/usr/bin/python3
import sys

if len(sys.argv) < 4:
	print("Nemate dovolno argumenti na komandna linija")

zbor = sys.argv[1]
zname = sys.argv[2]
datoteka = open(sys.argv[3], "r")
counter = 0
brojac = 0	
level = 1

for dat in datoteka:
	zbor_dat = dat.split(" ")
	for i in range(len(zbor_dat)):
		if zname == "l":
			if zbor == zbor_dat[i]:
				print("Zborot go ima vo linija: ", level)
				level +=1
				break
		elif zname == "w":
			if zbor != zbor_dat[i]:
				brojac += 1
			else:
				print("Brojot na zborovi pred nego e: ", brojac) 
				
		elif zname == "f":
			if zbor == zbor_dat[i]:
				counter += 1
if zname == "f":
	print ("Kolku pati go ima zborot: ", counter)
datoteka.close()
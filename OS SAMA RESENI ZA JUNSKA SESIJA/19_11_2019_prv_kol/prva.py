#!/usr/bin/python3
import sys


datoteka1 = open(sys.argv[1], "r")
datoteka2 = open(sys.argv[2], "r")


for red1 in datoteka1:
	brojac = 0
	komanda = red1.strip()
	datoteka2.seek(0)
	argumenti = []
	rezultat = ""

	for red2 in datoteka2:
		zbor = red2.split()
		if komanda == zbor[0]:
			brojac += 1 
			if brojac == 1:
				rezultat = red2.strip()
				for i in range(1, len(zbor)):
					if zbor[i].startswith("-"):
						argumenti.append(zbor[i])
					else:
						break
			
	if brojac > 1:
		print (f"{rezultat}-e {' '.join(argumenti)}")
	else:
		print (rezultat)
	
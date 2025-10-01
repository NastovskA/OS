#!/usr/bin/python3
import sys

datoteka1 = open(sys.argv[1], "r")
datoteka2 = open(sys.argv[2], "r")
level=1

broj_redovi = len(datoteka1.readlines())
datoteka1.seek(0)
rezultati = []

while level<=broj_redovi:
	dat1 = datoteka1.readline()
	datoteka2.seek(0)
	znamence = 0
	for dat2 in datoteka2:
		podeli = dat2.split("#")
		broj = int(podeli[0])
		komentar = podeli[1]
		if level == broj:
			rezultati.append(dat1.strip() + "#" + komentar + '\n')
			print(dat1.strip() + "#" + komentar)
			znamence = 1
			break
	if  not znamence:
		rezultati.append(dat1 + '\n')
		print(dat1) 
	level += 1
	
datoteka1.close()
datoteka2.close()

d = open(sys.argv[1], "w")
d.writelines(rezultati)
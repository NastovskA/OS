#!/usr/bin/python3
import sys


broj_datoteki = len(sys.argv) - 1

if broj_datoteki < 2:
	print("Mora da se povika so najmalku 2 argumenti")
	sys.exit(-1)


for dat in sys.argv[1:]:
	dat_open = open(dat, "r")
	
	for zb in dat_open:
		lista = []
		zbor = zb.split(" ")
		lista.extend(zbor)
	dat_open.close()

	i=0
	j=len(lista) - 1
		
	while i<j:
		if lista[i] == lista[j]:
			i += 1
			j -= 1
		else:
			lista[i], lista[j] = lista[j], lista[i]
			i += 1
			j -= 1
	dat_open = open(dat, "w")
	dat_open.write(" ".join(lista))
	dat_open.close()	
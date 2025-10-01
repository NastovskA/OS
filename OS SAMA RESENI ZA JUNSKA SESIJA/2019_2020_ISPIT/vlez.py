#!/usr/bin/python3
import sys

datoteka = open(sys.argv[1], "r")
nova = open (sys.argv[1] + "_out", "w")

for red in datoteka:
	lista_zborovi = []
	lista_dati = []
	zbor = red.split()

	for i in zbor:
		if len(i.split("-")) == 3:
			delovi = i.split("-")
			if all(d.isdigit() for d in delovi):
				lista_dati.append(i)
		else:
			lista_zborovi.append(i)
	if lista_dati:
		for datum in lista_dati:
			nova.write(datum + " " + " ".join(lista_zborovi) + "\n")
	else:
		nova.write(red)
datoteka.close()
nova.close()
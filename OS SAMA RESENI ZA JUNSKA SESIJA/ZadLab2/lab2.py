#!/usr/bin/python3
import sys

datoteka = open(sys.argv[1], "r")
znamence = sys.argv[2]

lista = []
brojac = 0
for red in datoteka:
	
	delce = red.split()
	datum = delce[5]
	lista.append(datum)
datoteka.close()
	
lista_pominati_datumi = []

if znamence == "-d":
	for datum in set(lista):
		print(datum)
elif znamence == "-f":
	i=0

	while 	i<len(lista):
		brojac = 1
		j=i+1
		while j<len(lista):
			if lista[i] == lista[j]:
				brojac += 1
			j += 1
		if lista[i] not in lista_pominati_datumi:
			lista_pominati_datumi.append(lista[i])
			print("Datumot e:", lista[i])
			print("Brojot na negovi pojavuvanja pri kreiranje:", brojac)
		i += 1			

#!/usr/bin/python3
import sys

broj_argumenti = len(sys.argv) - 1

if broj_argumenti == 1:
	datoteka = open(sys.argv[1], "r")
	for dat in datoteka:
		print(dat)
	datoteka.close()

if broj_argumenti == 2:
	suma = 0
	datoteka = open(sys.argv[1], "r")
	argument = sys.argv[2]
	broj1 = int(input("Vnesi prv broj: "))
	broj2 = int(input("Vnesi vtor broj: "))
	broj3 = int(input("Vnesi tret broj: "))
	
	for d in datoteka:
		zbor_vo_red = d.split(" ")
		red_broj = int(zbor_vo_red[0])
		jadenje = zbor_vo_red[1]
		arg_dat = zbor_vo_red[2]
		cena = int(zbor_vo_red[3])
		if argument == arg_dat and (red_broj == broj1 or red_broj == broj2 or red_broj == broj3):
			print(jadenje)
			suma += cena
	print("Vkupno: ", suma)

	datoteka.close()
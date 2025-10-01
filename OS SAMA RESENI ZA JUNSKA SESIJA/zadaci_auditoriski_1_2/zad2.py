#!/usr/bin/python3
import sys

dat1 = sys.argv[1]
dat2 = sys.argv[2]
dat3 = sys.argv[3]

dat1_otvorena = open(dat1, "r")
dat2_otvorena = open(dat2, "r")
dat3_otvorena = open(dat3, "w")

lista1 = []
lista2 = []

for d1 in dat1_otvorena:
	lista1.append(d1)
dat1_otvorena.close()
	
for d2 in dat2_otvorena:
	lista2.append(d2)
dat2_otvorena.close()

i=0
j=0
while i<len(lista1) or j<len(lista2):

	if i < len(lista1):
		dat3_otvorena.write(lista1[i])
		print(lista1[i], end = "")
		i += 1
	if j < len(lista2):
		dat3_otvorena.write(lista2[j])
		print(lista2[j], end = "")
		j += 1

dat3_otvorena.close()

	
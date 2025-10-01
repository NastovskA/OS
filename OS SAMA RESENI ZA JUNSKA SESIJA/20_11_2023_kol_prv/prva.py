#!/usr/bin/python3
import sys

broj_argumenti = len(sys.argv) - 1

if broj_argumenti == 2:
	datoteka_A = open(sys.argv[1], "r")
	zname_A = sys.argv[2]
	
	for datA in datoteka_A:
		zborovi = datA.split(" ")
		for i in range(len(zborovi)):
			if "." in zborovi[i] and zborovi[i].count(".") == 3:
				adresa = zborovi[i].split(".")
				if len(adresa) == 4 and adresa[0].isdigit() and adresa[1].isdigit() and adresa[2].isdigit() and adresa[3].isdigit():
					
					b1 = int(adresa[0])
					b2 = int(adresa[1])
					b3 = int(adresa[2])
					b4 = int(adresa[3])

					if zname_A == "a":

						while b1 <= 255:
							while b2 <= 255:
								while b3 <= 255:
									while b4 <= 255:
										print(f"{b1}.{b2}.{b3}.{b4}")
										b4 += 1
									b4 = 0
									b3 += 1
								b3 = 0
								b2 += 1
							b2 = 0
							b1 += 1

	datoteka_A.close()

elif broj_argumenti == 3:
	datoteka_O = open(sys.argv[1], "r")
	zname_O = sys.argv[2]
	izlezna_O = open(sys.argv[3], "w")
	lista = []

	for datO in datoteka_O:
		zborovi = datO.split(" ")
		b1 = b2 = b3 = b4 = None
		m1 = m2 = m3 = m4 = None

		for i in range(len(zborovi)):
			if "." in zborovi[i] and zborovi[i].count(".") == 3:
				adresa = zborovi[i].split(".")
				if len(adresa) == 4 and all(a.isdigit() for a in adresa):
					b1 = int(adresa[0])
					b2 = int(adresa[1])
					b3 = int(adresa[2])
					b4 = int(adresa[3])

			elif ":" in zborovi[i] and zborovi[i].count(":") == 3:
				maska = zborovi[i].split(":")
				if len(maska) == 4 and all(m.isdigit() for m in maska):
					m1 = int(maska[0])
					m2 = int(maska[1])
					m3 = int(maska[2])
					m4 = int(maska[3])

		if zname_O.lower() == "o" and None not in [b1, b2, b3, b4, m1, m2, m3, m4]:
			i1, i2, i3, i4 = b1, b2, b3, b4
			while True:
				lista.append(f"{i1}.{i2}.{i3}.{i4}\n")
				if [i1, i2, i3, i4] == [m1, m2, m3, m4]:
					break
				i4 += 1
				if i4 > 255:
					i4 = 0
					i3 += 1
					if i3 > 255:
						i3 = 0
						i2 += 1
						if i2 > 255:
							i2 = 0
							i1 += 1
							if i1 > 255:
								break

	izlezna_O.writelines(lista)
	izlezna_O.close()
	datoteka_O.close()

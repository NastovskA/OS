#!/usr/bin/python3
import sys

datoteka = open(sys.argv[1], "r")

lista_zborovi = []

for iterator in datoteka:
	zbor = iterator.split()
	lista_zborovi.extend(zbor)
datoteka.close()

i=0

lista_suma_BTC = []
lista_suma_Doge = []
lista_suma_EOS = []
lista_suma_ETH = []
lista_suma_XPM = []

while i<len(lista_zborovi):
	if lista_zborovi[i] == "BTC": 
		lista_suma_BTC.append(lista_zborovi[i+1])
	elif lista_zborovi[i] == "Doge":
		if lista_zborovi[i+1][0] == "$": 
			lista_suma_Doge.append(lista_zborovi[i+1])
	elif lista_zborovi[i] == "EOS": 
		if lista_zborovi[i+1][0] == "$" :
			vrednost = lista_zborovi[i+1].replace(",", "") 
			vrednost = vrednost.rstrip(".")
			lista_suma_EOS.append(vrednost)
	elif lista_zborovi[i] == "ETH": 
		lista_suma_ETH.append(lista_zborovi[i+1])
	elif lista_zborovi[i] == "XPM": 
		lista_suma_XPM.append(lista_zborovi[i+1])
	i += 1

btc=0
broj1_eth = broj2_eth = broj3_eth = broj4_eth = "N/A"
najmala_btc = 100000
najgolema_btc = 0 
razlika_btc = 0
while btc<len(lista_suma_BTC)-1:
		razlika_btc = float(lista_suma_BTC[btc][1:]) - float(lista_suma_BTC[btc+1][1:])

		if razlika_btc <= 0 and abs(razlika_btc) > najgolema_btc:
			najgolema_btc = abs(razlika_btc)
			broj1_btc = lista_suma_BTC[btc]
			broj2_btc = lista_suma_BTC[btc+1]
		if razlika_btc > 0 and razlika_btc < najmala_btc:
			najmala_btc = razlika_btc
			broj3_btc = lista_suma_BTC[btc]
			broj4_btc = lista_suma_BTC[btc+1]
		btc += 1
print("BTC najgolemo pokacuvanje", najgolema_btc, "so vrednosti ",broj1_btc, broj2_btc, "najgolemo namaluvanje ", najmala_btc, "so vrednosti", broj3_btc, broj4_btc)


eth=0
najmala_eth = 100000
najgolema_eth = 0 
razlika_eth = 0
broj1_eth = broj2_eth = broj3_eth = broj4_eth = "N/A"

while eth<len(lista_suma_ETH)-1:
		razlika_eth = float(lista_suma_ETH[eth][1:]) - float(lista_suma_ETH[eth+1][1:])

		if razlika_eth < 0 and abs(razlika_eth) > najgolema_eth:
			najgolema_eth = abs(razlika_eth)
			broj1_eth = lista_suma_ETH[eth]
			broj2_eth = lista_suma_ETH[eth+1]
		if razlika_eth > 0 and razlika_eth < najmala_eth:
			najmala_eth = razlika_eth
			broj3_eth = lista_suma_ETH[eth]
			broj4_eth = lista_suma_ETH[eth+1]
				
		eth += 1

if len(lista_suma_ETH) >= 2:
    print("ETH najgolemo pokacuvanje", najgolema_eth, "so vrednosti", broj1_eth, broj2_eth,
          "najgolemo namaluvanje", najmala_eth, "so vrednosti", broj3_eth, broj4_eth)
else:
    print("Nema dovolno vrednosti za ETH")




doge=0
najmala_doge = 100000
najgolema_doge = 0 
razlika_doge = 0
broj1_doge = broj2_doge = broj3_doge = broj4_doge = "N/A"

while doge<len(lista_suma_Doge)-1:
		razlika_doge = float(lista_suma_Doge[doge][1:]) - float(lista_suma_Doge[doge+1][1:])

		if razlika_doge < 0 and abs(razlika_doge) > najgolema_doge:
			najgolema_doge = abs(razlika_doge)
			broj1_doge = lista_suma_Doge[doge]
			broj2_doge = lista_suma_Doge[doge+1]
		if razlika_doge > 0 and razlika_doge < najmala_doge:
			najmala_doge = razlika_doge
			broj3_doge = lista_suma_Doge[doge]
			broj4_doge = lista_suma_Doge[doge+1]
				
		doge += 1

if len(lista_suma_Doge) >= 2:
    print("Doge najgolemo pokacuvanje", najgolema_doge, "so vrednosti", broj1_doge, broj2_doge,
          "najgolemo namaluvanje", najmala_doge, "so vrednosti", broj3_doge, broj4_doge)
else:
    print("Nema dovolno vrednosti za Doge")



eos = 0
najmala_eos = 100000
najgolema_eos = 0 
razlika_eos = 0
broj1_eos = broj2_eos = broj3_eos = broj4_eos = "N/A"

while eos < len(lista_suma_EOS) - 1:
    razlika_eos = float(lista_suma_EOS[eos][1:]) - float(lista_suma_EOS[eos+1][1:])
    if razlika_eos < 0 and abs(razlika_eos) > najgolema_eos:
        najgolema_eos = abs(razlika_eos)
        broj1_eos = lista_suma_EOS[eos]
        broj2_eos = lista_suma_EOS[eos+1]
    if razlika_eos > 0 and razlika_eos < najmala_eos:
        najmala_eos = razlika_eos
        broj3_eos = lista_suma_EOS[eos]
        broj4_eos = lista_suma_EOS[eos+1]
    eos += 1

if len(lista_suma_EOS) >= 2:
    print("EOS najgolemo pokacuvanje", najgolema_eos, "so vrednosti", broj1_eos, broj2_eos,
          "najgolemo namaluvanje", najmala_eos, "so vrednosti", broj3_eos, broj4_eos)
else:
    print("Nema dovolno vrednosti za EOS")

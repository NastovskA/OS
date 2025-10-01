#!/usr/bin/python3
import sys


datoteka = open(sys.argv[1], "r")

recnik_PREZIME = {}   # telefon → prezime
recnik_IME = {}   # telefon → ime
recnik_TELL = {}  # prezime → telefon
recnik_TEL = {}  # ime → telefon

for dat in datoteka:
    red = dat.strip().split()
        reden_broj = red[0]
        telefon = red[1]
        ime = red[2]
        prezime = red[3]
        
        recnik_PREZIME[telefon] = prezime
        recnik_IME[telefon] = ime
        recnik_TELL[prezime] = telefon
        recnik_IMEE[ime] = telefon

datoteka.close()

while True:
    print("1 - baraj po tel broj, a se pecati ime i prezime")
    print("2 - baraj po ime i prezime, a se pecati tel. broj")
    print("3 - se pecati celiot imenik")
    print("Druго - Izlez")
    
    broj = input("Vnesi reden broj 1,2 ili 3: ")
    
    if broj == "1":
        tel_broj = input("Vnesi tel broj: ")
        if tel_broj in recnik_IME and tel_broj in recnik_PREZIME:
            print("Ime: ", recnik_IME[tel_broj])
            print("Prezime: ", recnik_PREZIME[tel_broj])
        else:
            print("Ne postoi kontakt so takov broj!")

    elif broj == "2":
        i = input("Vnesi ime: ")
        p = input("Vnesi prezime: ")

        if i in recnik_TEL and p in recnik_TELL:
            broj_i = recnik_PREZIME[i]
            broj_p = recnik_TELL[p]
            if broj_i == broj_p:
                print("Telefonski broj e:", broj_i)
            else:
                print("Imeto i prezimeto ne se poklopuvaat.")
        else:
            print("Nema takov covek!")

    elif broj == "3":
        datoteka = open(sys.argv[1], "r")
        for linija in datoteka:
            print(linija.strip())
        datoteka.close()

    else:
        print("Zavrsuvanje...")
        break

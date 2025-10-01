# За секоја емаил адреса што ќе се пронајде во влезната датотека, се креира ваква наредба, каде на
# местото од kime@firma.com се сместува конкретната најдена емаил адреса. Во втората датотека, чие
# што име се добива како влез на скриптата, е сместен именик со познати емаил адреси, така што во секој
# ред е сместена посебна емаил адреса, заедно со името и презимето на корисникот на емаил адресата
# (одделени со празно место). Скриптата треба да ја провери секоја пронајдена емаил адреса од првата
# датотека дали ја има во именикот креиран од втората датотека. Доколку пронајдената емаил адреса ја
# има во втората датотека, тогаш Ime и Prezime се заменуваат со името и презимето пронајдени во
# именикот за таа емаил адреса. Инаку, се игнорираат од наредбата . Параметарот “ poraka “ е текстуална
# порака чиј што текст е сместен во датотека со име „poraka.txt“. Доколку емаил адресата ја има воporaka.txt“. Доколку емаил адресата ја има во
# именикот, тогаш пред текстот од пораката се додава следниот текст: „poraka.txt“. Доколку емаил адресата ја има воPocituvan Ime Prezime,“, каде име
# и презиме се земаат истите од претходно. Доколку емаил адресата ја нема во именикот, тогаш “poraka“
# останува непроменета. Секоја ваква формирана наредба се печати на екран, притоа не се печати повеќе
# пати ако се пронајде истата емаил адреса повеќе пати.

#!/usr/bin/python3
import sys
import os
import re

broj_argumenti = len(sys.argv) - 1
if(broj_argumenti < 2):
    print("Nema dovolno argumenti vneseno od komandna linija!")
    sys.exit(1)

datoteka_za_obrabotka = open(sys.argv[1], "r")
datoteka_imenik = open(sys.argv[2], "r")
lista_email = []

izgled_email = r"[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+" #kako lici eden email

for red_O in datoteka_za_obrabotka:
    email_vo_red = re.findall(izgled_email, red_O) #vraka lista so site sofpaganja od redot
    for email in email_vo_red:
        if email not in lista_email:
            lista_email.append(email)

datoteka_za_obrabotka.close()

imenik = {} # email -> ime, prezime

for red_I in datoteka_imenik:
    zbor = red_I.split(" ")
    if len(zbor) >= 3:
        eposta = zbor[0]
        ime = zbor[1]
        prezime = "".join(zbor[2:]) #vo slucaj ako imam poveke preziminja zemi ja listata od poveke preziminja
        imenik[eposta] = (ime, prezime) #kluc = eposta, i od klucot se vadat ime prezime
datoteka_imenik.close()

#PROVERKA DALI POSTOI FAJLOT
if not os.path.exists("poraka.txt"):
    print("Datotekata 'poraka.txt' ne postoi.")
    sys.exit(1)


f = open("poraka.txt", "r")
tekst_poraka = f.read()
f.close()

for email in lista_email:
    if email in imenik[email]:
        ime,prezime = imenik[email]
        pocetok_poraka = f"Pocituvani {ime} {prezime},"
        cel_tekst = f"{pocetok_poraka}\n{tekst_poraka}"
        print(f'mail -s "{cel_tekst}" {email} -u {ime} {prezime}')
    else:
        print(f'mail -s "{tekst_poraka}" {email}')




    
#  Да се напише PYTHON скрипта која како аргументи добива имиња на две
# датотеки. Во првата
# датотека е сместен текст кој треба да се обработи, додека пак, во 
# втората датотека е сместен именик со
# контакти. Скриптата треба да ја измине првата датотека и да ги 
# најде сите е-маил адреси (формат:
# kime@firma.com) и да креира команда спремна за испраќање на маил
# со конкретна порака. Командата се
# формира во следниов формат:
# mail -s “poraka“ kime@firma.com -u Ime Prezime
# За секоја емаил адреса што ќе се пронајде во влезната датотека,
# се креира ваква наредба, каде на
# местото од kime@firma.com се сместува конкретната најдена емаил
# адреса. Во втората датотека, чие
# што име се добива како влез на скриптата, е сместен именик со
# познати емаил адреси, така што во секој
# ред е сместена посебна емаил адреса, заедно со името и презимето 
# на корисникот на емаил адресата
# (одделени со празно место). Скриптата треба да ја провери секоја 
# пронајдена емаил адреса од првата
# датотека дали ја има во именикот креиран од втората датотека. 
# Доколку пронајдената емаил адреса ја
# има во втората датотека, тогаш Ime и Prezime се заменуваат со
# името и презимето пронајдени во
# именикот за таа емаил адреса. Инаку, се игнорираат од наредбата . 
# Параметарот “ poraka “ е текстуална
# порака чиј што текст е сместен во датотека со име „poraka.txt“. 
# Доколку емаил адресата ја има воporaka.txt“. Доколку емаил адресата 
# ја има во
# именикот, тогаш пред текстот од пораката се додава следниот текст: 
# „poraka.txt“. Доколку емаил адресата ја има воPocituvan Ime Prezime,
# “, каде име
# и презиме се земаат истите од претходно. Доколку емаил адресата 
# ја нема во именикот, тогаш “poraka“
# останува непроменета. Секоја ваква формирана наредба се печати
# на екран, притоа не се печати повеќе
# пати ако се пронајде истата емаил адреса повеќе пати.
# Да се напише Shell скрипта која што ќе провери дали во тековниот 
# директориум се наоѓаат
# датотеките „poraka.txt“. Доколку емаил адресата ја има воporaka.txt“
# и „poraka.txt“. Доколку емаил адресата ја има воkontakti.vcf“. 
# Доколку ги има, тогаш ги пребарува сите текстуални датотеки
# во тековниот директориум (со екстензија .txt, игнорирајќи ја
#  датотеката „poraka.txt“. Доколку емаил адресата ја има воporaka.txt“
#  ) и секоја една
# датотека ја праќа на влез на повик на PERL скриптата заедно со 
# „poraka.txt“. Доколку емаил адресата ја има воkontakti.vcf“. 
# Секоја наредба што ќе се
# врати, SHELL скриптата треба да ја изврши.

#!/usr/bin/python3
import sys

if len(sys.argv) < 3:  #skripta, datoteka 1, datoteka 2
    print("Gresno vnesuvanje!")
    quit()

dat1 = open(sys.argv[1], "r")

postojatEmails =[]

for line in dat1:
    zbor = line.split(" ")
    for z in zbor:
        if "@" in z and "." in z.split("@")[1]:
            if z not in postojatEmails: # za da nema duplikati
                postojatEmails.append(z.strip())
dat1.close()


dat2 = open(sys.argv[2], "r")
recnik = {}
for linija in dat2:
    element = linija.strip().split(" ")
    if len(element) >= 3:
        mail = element[0]
        ime_prezime = " ".join(element[1:])
        recnik[mail] = ime_prezime
dat2.close()

dat3 = open("poraka.txt", "r")
tekst_poraka = dat3.read().strip()
dat3.close()

for email in postojatEmails:
    if email in recnik:
        ime_prezime = recnik[mail]
        krajna_poraka = f"Pocituvan {ime_prezime}, \n{tekst_poraka}"
        print(f'mail -s "{krajna_poraka}" {email} -u "{ime_prezime}"')
    else:
        print(f'mail -s "{krajna_poraka}" {email} -u Ime Prezime')

# Да се напише Shell скрипта која што ќе провери дали во тековниот директориум се наоѓаат
# датотеките „poraka.txt“ "konttakti.vcf".  Доколку ги има, тогаш ги пребарува сите текстуални датотеки 
# во тековниот директориум (со екстензија .txt, игнорирајќи ја датотеката „poraka.txt“.) и секоја една 
# датотека ја праќа на влез на повик на PYTHON скриптата заедно со „kontakti.vfc“.Секоја наредба што ќе се 
# врати, SHELL скриптата треба да ја изврши


# #!/bin/sh

# tekoven_direktorium = "."
# datoteka_poraka = $(find $tekoven_direktorium -type f "poraka.txt")
# datoteka_kontakti = $(find $tekoven_direktorium -type f "kontakti.vcf")

# if[-n "$datoteka_poraka"] && [-n "$datoteka_kontakti"]; then
#     find . -type f -name "*.txt" | -name "poraka.txt" | while read datoteka; do
#     izlez = $(cat "$datoteka" | python3 16_9_2019_ispit.py "datoteka_kontakti")

#     if [-n "$izlez"]; then
#         eval "$izlez" #gi izvrsuva naredbite, echo samo pecati kako tekst ne izvrsuva
#     fi
# done
# fi
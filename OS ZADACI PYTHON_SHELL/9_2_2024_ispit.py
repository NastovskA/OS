# Да се напише Python скрипта со име „prva.py3“ која како аргументи
# добива две влезни датотеки. Првата
# датотека е код напишан во Python, додека пак втората датотека е 
# документација која треба да се вметне
# во кодот. Документацијата е организирана така што секоја линија
# од датотеката најпрво има број, а
# потоа текст. Скриптата треба да ги спои двете датотеки, така што
# втората датотека (документацијата)
# треба да ја додаде во програмскиот код (првата датотека).
# Додавањето се прави така што бројката на
# почеток на секоја линија во документацијата кажува на која линија
# во кодот треба да биде истата
# вметната, а потоа, на крајот на линијата во кодот, се додава 
# линијата од документацијата, започнувајќи
# со #. Новоформираниот код се печати на екран. 


#!/usr/bin/python3
import sys

if len(sys.argv) != 3:  #skripta, datoteka 1, datoteka 2
    print("Gresno vnesuvanje!")
    quit()

dat2 = open(sys.argv[2], "r")
pomosnaLista = []
for linija in dat2:
    element = linija.split(" ", 1) #go delam tekstot na 2 dela, 1 znaci deka ke odi do prvoto prazno mesto samo so ova imam element[0]=brojot i element [1]=celiot tekst

    if len(element) == 2: #znam deka tocno na 2 dela mi e 
        broj_linija = int(element[0])
        tekst_dokument = element[1].strip() # go spojuvam celiot tekst bez praznini
    pomosnaLista.append((broj_linija, tekst_dokument)) # isto kako da ja povikam 2 pati
    # pomosnaLista.append(broj_linija)
    # pomosnaLista.append(tekst_dokument)
dat2.close()


dat1 = open(sys.argv[1], "r")
site_linii = dat1.readlines() #site linii se citaat
dat1.close()

nov_kod = []


for i,line in enumerate(site_linii, 1): #od kade gi zimam liniite i od koj indeks ke zapocne vo slucajov 1
    linija = line.strip() #go trgam null terminatorot 
    nov_kod.append(linija) #gi stavam vo noviot kod site linii

    for pom in pomosnaLista:
        if pom[0] == i: #i e iterator vo site_listi a pom iterira vo pom_lista samo na elementite [0] pozicija
            nov_kod[-1] += " #" + pom[1] #so -1 sme na tekovniot


for line in nov_kod:
    print(line)


# Да се напише SHELL скрипта која треба да ги најде сите датотеки (во тековниот директориум и
# поддиректориумите) кои имаат наставка (екстензија) „.py3“. Потоа, скриптата треба за секоја
# датотека со наставка „.py“ да најде датотека со исто име како и датотеката само со екстензија
# „.со“. (Пример, ако најде датотека со име prvadat.py3, пребарува дали има датотека со име
# prvadat.co (во тековниот директориум и поддиректориумите). Скриптата, за секој пар датотеки
# што ќе ги најде датотека со наставка „.py“ и датотека со исто име само со наставка „.co“, ја
# повикува Python скриптата од првата задача и како аргументи им ги праќа ове две датотеки.

# #!/bin/sh

# tekoven_direktorium='.'

# datotekiPY = $(find "$tekoven_direktorium" -type f -name "*.py3")

# for py_dat in $datotekiPY; do
#     ime_py = $(basename "$py_dat" .py3) #zima samo ime bez ekstenzija

# datotekaSO=$(find $tekoven_direktorium -type f -name "$ime_py.so")
    

# if [-n "$datotekaSO" ]; then
#     python3 9_2_2024_ispit.py "$py_dat" "$datotekaSO"
# fi

# done
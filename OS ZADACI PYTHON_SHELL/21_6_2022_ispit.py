# Да се напише PYTHON скрипта која како аргументи од командна линија 
# добива 2 датотеки (имиња на
# датотеки). Доколку има помалку од 2 датотеки, тогаш треба да се 
# испише порака за грешка и да заврши
# скриптата. Првата датотека содржи парови зборови, по два зборови
# во секој ред посебно, втората
# датотека е датотека со случаен текст. Паровите зборови на првата
# датотека се граничници т.е. првиот
# збор е почетната граница а вториот збор е крајот. Скриптата треба
# да ги најде и отпечати сите зборови
# од втората датотека кои се наоѓаат од првиот граничен збор до
# вториот и да ги отпечати на екран. Ова
# треба да се направи за сите парови гранични зборови од првата 
# датотека. Доколку некој од граничните
# зборови го нема во втората датотека или првиот не е пред вториот,
# во тој случај не се печати за тој пар
# гранични зборови ништо.
# Пример:
# Dat1.txt:
# prv vtor
# most koj
# tri kraj
# Dat2.txt
# Sekoj prv den sedime na most pokraj reka, I sekoj vtor od nas e 
# koj od koj od elitata. Na kraj site se vratija na
# rabota
# На екран:
# prv den sedime na most pokraj reka, I sekoj vtor
# most pokraj reka, I sekoj vtor od nas e ko

#!/usr/bin/python3

import sys

if len(sys.argv) < 3:  #skripta, datoteka 1, datoteka 2
    print("Gresno vnesuvanje!")
    quit()

dat1 = open(sys.argv[1], "r")
parovi = []
for line in dat1:
    element = line.split(" ")
    if len(element) == 2:
        start = element[0]
        end = element[1]
        parovi.append((start, end))
dat1.close()

dat2 = open(sys.argv[2], "r")

for linija in dat2:
    zborovi = linija.strip().split(" ")
    for start,end in parovi:
        if start in zborovi and end in zborovi:
            start_indeks = zborovi.index(start) #go naogam indeksot
            end_indeks = zborovi.index(end) #i ovde indeks vraka
            if start_indeks < end_indeks:
                print(" ".join(zborovi[start_indeks:end_indeks +1])) #+1 e za dodavanje na posledniot zbor bidejki ne se vklucuva vo granicite

dat2.close()

# Да се напише Shell скрипта која како аргументи од командна линија добива имиња на два директориуми,
# притоа доколку се повика без аргументи, скриптата испишува порака за грешка, додека пак, доколку се
# повика со еден аргумент, вториот се смета дека е тековниот директориум. Скриптата треба да ја земе првата
# текстуална датотека од двата директориуми (со екстензија .тхт) и да ја повика PYTHON скриптата, со тоа што
# како прв аргумент ќе се прати датотеката од првиот директориум, додека како втор аргумент датотеката од
# вториот директориум. Доколку двата директориуми се исти, тогаш вториот директориум се заменува со
# тековниот директориум. На крај да се отпечати резултатот од извршување на PYTHON скриптата.

# #!/bin/sh

# if [ $# -lt 1 ]; then     #     $# broj na argumenti podneseni pri povik,     -lt = low then 1 
#     echo "Vnesi tocen broj na argumenti!"
#     exit 1

# elif [$# -lt 2]; then
#     echo " vtoriot direktorium e tekovniot"
#     direktorium2 = "."   #ili treba direktorium2= $(".")

# elif [$# -eq 2]; then
#     direktorium1=$1
#     direktorium2=$2
# fi

# if ["direktorium1" = "direktorium2"]; then
#     direktorium2 = "."
# fi

# prva_datoteka = $(find $direktorium1 -type f -neame "*.txt" | head -n 1) #vaka se zima prvata linija sto ke bide prvata tekstualna datoteka
# vtora_datoteka = $(find $direktorium2 -type f - name "*.txt" | head -n 1)

# if[-z "$prva_datoteka"] || [-z "$vtora_datoteka"]; then
#     echo "Ne se pronajdeni vrednosti! "
# fi

# python3 21_6_2022_ispit.py "$prva_datoteka" "$vtora_daatoteka"
# echo "Rezultat od skriptata: "

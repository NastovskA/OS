# Да се напише PYTHON скрипта која како аргументи од командна линија
# добива имиња на датотеки.
# Скриптата треба да ги измине датотеките и да провери колку различни
# валути на пари ќе најде. Знаците
# на валутите кои што се пребаруваат се: „$“ „€“ „£“ „kn“ „kr“ „Rp“
# „CHF“. Секоја валута започнува со
# знакот на валутата, по кое што следи сумата на истата (одделена со 
# празно место). Доколку после
# валутата нема бројка, тогаш валутата не се зема во предвид. Сите 
# валути што ќе бидат пронајдени во
# датотеката се собираат и се добива вкупна сума по валута. За секоја
# валута што била пронајдена во
# датотеката, му се дозволува на корисникот да внесе колку денари е 
# истата (не се внесува за оние валути
# кои што не биле пронајдени во тековната датотека). На крајот од 
# изминувањето на датотеката да се
# отпечати вкупната сума (во денари) што била пронајдена во 
# датотеката. Скриптата, на крај треба да
# отпечати кои валути биле пронајдени во сите датотеки.

#!/usr/bin/python3
import sys

if len(sys.argv) < 2:  #skripta, datoteka 1,
    print("Gresno vnesuvanje!")
    quit()

dat1 = open(sys.argv[1], "r")

valuti = {"$": 0, "€": 0, "£": 0, "kn": 0, "kr": 0, "Rp": 0, "CHF": 0}

for linija in dat1:
    zbor = linija.split(" ")
    for i in range(len(zbor) -1):
        if zbor[i] in valuti and zbor[i+1].isdigit():
            valuti[zbor[i]] += zbor[i+1]
dat1.close()

vkupno = 0

for val, iznos in valuti.items():
    if iznos > 0:
        kurs = input(f"Vnesete vrednost na {val} vo denari")
        vkupno += iznos * kurs

print("Pronajdeni valuti: ")
for val, iznos in valuti.items():
    if iznos > 0:
        print(f"{val}: {iznos}")

print(f"Vkupno vo denari: {vkupno}")



# Да се напише SHELL скрипта која ги листа сите датотеки од тековниот директориум. Потоа, имињата
# на оние датотеки кои што имаат помалку од 1000 зборови ги праќа како аргументи од командна линија
# на повик од PYTHON скриптата (сите имиња ги праќа во еден повик). За оние датотеки кои што имаат
# повеќе од 1000 зборови, креира посебен повик и нивните имиња ги праќа како еден аргумент од
# командна линија. 

# #!/bin/sh

# tekoven_direktorium ="."

# datoteka = $(find $tekoven_direktorium -type f) # gi baram site datoteki od tekoven direktorium

# pomali_od_1000 = ""
# pogolemi_od_1000 = ""

# for dat in $datoteka;do
#     broj_zborovi = $(wc -w < "$dat")

#     if["$broj_zborovi" -lt 1000]; then
#         pomali_od_1000="$pomali_od_1000 $dat"
#     else
#         pogolemi_od_1000="$pogolemi_od_1000 $dat"
#     fi
# done

# if [ -n "$pomali_od_1000"]; then
#     python3 10_4_2019_prv_kol.py "$pomali_od_1000"
# fi


# if [ -n "$pogolemi_od_1000"]; then
#     python3 10_4_2019_prv_kol.py "$pogolemi_od_1000"
# fi


# Да се напише PYTHON скрипта која како аргументи од командна линија добива имиња на датотеки.
# Скриптата треба да проверува две по две датотеки (секоја датотека со секоја останата датотека) и да
# споредува колку исти Хаштагови има (збор составен со букви и бројки кој започнува со специјалниот
# знак #). Истите Хаштагови и вкупниот број на Хаштагови се запишува во излезна датотека со име
# креирано од спојување на имињата на двете датотеки кои што се споредуваат. На крај, на екран се
# печати кој е најчесто користениот Хаштаг во сите датотеки.

#!/usr/bin/python3
import sys

if len(sys.argv)<3:
    print("Gresno vnesuvanje! ")
    quit()

datoteki = sys.argv[1:] #site datoteki ke gi zemam vo ova
global_hastag = {} #za broenje na site hash

for i in range(len(datoteki)):
    for j in range(i+1, len(datoteki)):

        dat1 = datoteki[i]
        dat2 = datoteki[j]

        hash_tag1 = []
        hash_tag2 = []

        prva = open(dat1, " r ")
        for linija in prva:
            zborovi = linija.strip().split(" ") #lista od site zborovi
            for zbor in zborovi:
                if zbor.startswith("#"):
                    hash_tag1.append(zbor)
        prva.close()

        vtora = open(dat2, "r")
        for linija in vtora:
            zborovi = linija.strip().split(" ")
            for zbor in zborovi:
                if zbor.startswith("#")
                hash_tag2.append(zbor)
        vtora.close()

        hash_zaednicki = []
        for tag in hash_tag1:
            if tag in hash_tag2 and tag not in hash_zaednicki: #dali go ima vo dr datoteka i dali ima duplikat
                hash_zaednicki.append(tag)
        
        hash_brojac = {}
        for htag in hash_zaednicki:
            if htag in hash_brojac:
                hash_brojac[htag] += 1
            else:
                hash_brojac[htag] = 1

        ime_dat1 = dat1.split("/")[-1] #izvlekuvanje bez pat
        ime_dat2 = dat2.split("/")[-1] #izvlekuvanje bez pat

        samo_ime1 = ime_dat1.split(".")[0]
        samo_ime2 = ime_dat2.split(".")[0]

        ime_izlezna_datoteka = f"{samo_ime1}_{samo_ime2}.txt"
        izlezna_dat = open(ime_izlezna_datoteka, "w")
        izlezna_dat.write(f"Zaednicki hash tags pomegu {dat1} i {dat2:}")
        for htags, brojac in hash_brojac.items():
            izlezna_dat.write(f"{htags}: {brojac}")
        izlezna_dat.close()

if global_hastag:
    najcesto_koristen_hash_tag = max(global_hastag, key=global_hastag.get)
    print(f"Najcesto koristen tag: {najcesto_koristen_hash_tag} so {global_hastag[najcesto_koristen_hash_tag]} pati")
else:
    print("Nema zaednicki!")






# import sys

# if len(sys.argv) < 3:  #skripta, datoteka 1, dat2 //minimum
#     print("Gresno vnesuvanje!")
#     quit()


# dat1 = open(sys.argv[1], "r")
# dat2 = open(sys.argv[2], "r")

# prvaDatoteka = []
# for linija in dat1:
#     zborovi = linija.strip().split(" ")
#     for zbor in zborovi:
#         if zbor.startswith("#"):
#             prvaDatoteka.append(zbor)
# dat1.close()


# vtoraDatoteka = []
# for linija in dat2:
#     zborovi = linija.strip().split(" ")
#     for zbor in zborovi:
#         if zbor.startswith("#"):
#             prvaDatoteka.append(zbor)
# dat2.close()

# zaednicki_hash = []

# for hash in prvaDatoteka:
#     if hash in vtoraDatoteka and hash not in zaednicki_hash:
#         zaednicki_hash.append(hash)
# vkupno_hash = len(zaednicki_hash)

# nova_datoteka = sys.argv[1].split('.')[0] + "_" + sys.argv[2].split('.')[0] + "_output.txt"

# datoteka = open(nova_datoteka, "w")
# datoteka.write(f"Zaednicki hash: {zaednicki_hash}")
# datoteka.write(f"Vkupno hash: {vkupno_hash}")
# datoteka.close()

# site_hash = prvaDatoteka + vtoraDatoteka
# najcesto_koristen = []
# max_brojac=0

# for hash in site_hash:
#     brojac=site_hash.count(hash)
#     if brojac > max_brojac:
#         max_brojac=brojac
#         najcesto_koristen=hash
        
# print(f"Najcesto koriste: {najcesto_koristen}")

# Да се напише SHELL скрипта која ги листа сите датотеки од тековниот директориум. Потоа, зема две по
# две датотеки од листата датотеки и ги праќа како влезни аргументи на повикот на pYthon скриптата од
# задача 1. Резултатот од секој повик на PYTHON Скриптата се запишува во датотека креирана од спој на
# имињата на двете датотеки кои се праќаат на PYTHON Скриптата, притоа се запишува во продолжение
# (append). 

# #!/bin/sh
# if [ $# -lt 1 ]; then       #lista
#     echo "Vnesi argument!"
#     exit 1
# fi

# datoteki=$(ls -p | grep -v /)  #site datoteki samo vo tekoven

# for dat1 in $datoteki; do
#     for dat2 in $datoteki; do

#     if("$dat1" != "$dat2"); then
#     ime_izlezna = "${dat1%.*}_${dat2%.*}.txt"
#       python3 6_4_2017_prv_kol.py "$dat1" "$dat2" >> "$ime_izlezna"
#     fi
# done
# done



# #!/bin/bash

# files=( *) #nizata files ja polnam so site datoteki i direktoriumi od tekoven folder

# # Проверка дали има барем 2 датотеки
# if [ "${#files[@]}" -lt 2 ]; then
#     echo "Nema dovolno datoteki vo tekovniot direktorium!"
#     exit 1
# fi

# prev_file=""

# for file in "${files[@]}"; do
#     if [ -n "$prev_file" ]; then
#         output_file="output_${prev_file%.*}_${file%.*}.txt" #%.* значи „отстрани го последниот . и сè што следи по него“
#         python3 zad6.py "$prev_file" "$file" >> "$output_file"
#     fi
#     prev_file="$file"
# done


# tekoven_direktorium ="."

# datoteki = $(find "$tekoven_direktorium" -type f) #site datoteki

# datoteki_vo_lista=($datoteki)

# dolzina_na_lista = ${datoteki_vo_lista[@]}

# for ((i=0; i<$dolzina_na_lista; i+=2)); do
#     datoteka1="${datoteki_vo_lista[$i]}"
#     datoteka2="${datoteki_vo_lista[$i+1]}"

#     nova_datoteka="${datoteka1}_${datoteka2}"
#     python3 6_4_2017_prv_kol.py "$datoteka1" "$datoteka2" >> "$nova_datoteka"

# done




# Да се напише Shell скрипта која ќе ја користи претходно дефинираната Python скрипта. Скриптата
# shell како аргументи добива влезна датотека и непознат број на излезни датотеки. Скриптата
# треба да ја подели влезната датотека на еднаков број на линии во излезните датотеки. Доколку
# влезната датотека има K линии и има N излезни датотеки, тогаш секоја од излезните датотеки
# ќе добие K/N дел од влезната датотека. 

# #!/bin/bash

# if [ $# -lt 2 ]; then
#     echo "Nedovolen broj na argumenti"
#     exit 1
# fi

# input_file=""
# output_files=()

# for file in "$@"; do
#     if [ -n "$input_file" ]; then
#         output_files+=("$file")
#     else
#         input_file="$file"
#     fi
# done

# vk_linii=$(wc -l < "$input_file")

# br_izlez=$(($# - 1))
# linii=$((vk_linii / br_izlez))


# start=1
# for file in "${output_files[@]}"; do
#     end=$((start + linii - 1))
    
#     sed -n "${start},${end}p" "$input_file" > "$file"

#     start=$((end + 1))
# done
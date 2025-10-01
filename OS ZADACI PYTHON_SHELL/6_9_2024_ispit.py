# . Да се напише Python скрипта со име „prva.py3“ која како аргументи добива име на влезна датотека и
# опционален втор аргумент. Во влезната датотека има мени, така што 
# во секој ред одделно има по едно
# јадење од менито во формат reden_broj ime_na_jadenje kategorija_na_
# jadenje cena. Секој дел е одделен
# помеѓу себе со празно место. Доколку корисникот не внесе втор 
# аргумент, тогаш се печати целото мени,
# но доколку внесе аргумент, тогаш ги печати само оние јадења чија
# категорија е иста со вториот
# аргумент (ги филтрира по категорија внесена како втор аргумент од 
#           командна линија). Потоа скриптата
# дозволува да се внесе од тастатура три јадења од менито преку 
# внесување на нивниот реден број. На
# крај скриптата треба да ги отпечати имињата на одбраните јадења од 
# корисникот (само оние кои ги има
# на менито што е претходно отпечатено) и вкупната сума за наплата 
# (вкупната цена).
# Пример:
# ./python3 meni.py3 dorucek
# На екран:
# 1 omlet dorucek 150
# 7 poshirani dorucek 120
# 20 mekici dorucek 70
# 25 angliski dorucek 350
# Од тастатура:
# 1 10 25
# На екран:
# Omlet
# Angliski
# Vkupno: 500 


#!/usr/bin/python3

import sys

if len(sys.argv) <2 or len(sys.argv) > 3:  #skripta, datoteka , vtor argument
    print("Gresno vnesuvanje!")
    quit()

dat = open(sys.argv[1], "r") #ja otvaram datotekata i citam
pomLista = []

for line in dat:
    element = line.split(" ")
    pomLista.append(element)
dat.close()

if len(sys.argv) == 3:
    kategorija = sys.argv[2]
else:
    kategorija = None

for i in pomLista:
    if kategorija is None or i[2] == kategorija:
        print(" ".join(i)) #se pecati cel red

odbrani_jadenja = []
vkupno=0
print("Vnesi 3 jadenja: ")

for _ in range(3): # _ SE KORISTI ZA KOGA SAKAME NESTO DA POFTORIME 3 PATI
    broj = int(input()) #VNESI REDEN BROJ PREKU TASTATURA

    if broj>0:
        najden = False
        for jadenje in pomLista:
         if broj == int(jadenje[0]):
            jadenje = pomLista[broj-1] #INDEKSI GLEDAME ZATOA E -1
            odbrani_jadenja.append(jadenje[1])
            vkupno += int(jadenje[3]) #na indeks 3 se naoga cenata
            najden=True
            break
         if not najden:
             print("Nevazecki")
    else:
        print("nevazecki broj")

for jadenje in odbrani_jadenja:
    print(jadenje)
print(f"vkupno: {vkupno}")

# Да се напише SHELL скрипта која како аргумент добива даден збор. Shell скриптата треба да ги
# најде сите текстуални датотеки во кои се содржи тој збор (во тековниот директориум и
# поддиректориумите). Секоја датотека која ќе се најде, се повикува одделно Python скриптата од
# првата задача, каде како прв аргумент се праќа името на датотеката, додека пак како втор
# аргумент се праќа зборот (што се добива како аргумент на Shell скриптата). Резултатот од
# извршувањето на секоја Python скрипта се печати на екран.

# #!/bin/sh

# if [ $# -lt 1 ]; then       #ima 1 zbor kako argument
#     echo "Vnesi argument!"
#     exit 1
# fi

# tekoven_direktorium="."
# zbor="$1"

# datoteki = $(find "$tekoven_direktorium" -type f -name "*.txt" )

# if [ -z "$datoteki" ]; then
#     echo "Нема датотеки што го содржат зборот \"$zbor\"."
#     exit 1
# fi

# for datoteka in $datoteki; do
    
#     if grep -q "$zbor" "$datoteka"; then
        
#         echo "Pronajdena datoteka: $datoteka"
#             python3 6_9_2024_ispit.py "$datoteka" "$zbor"
#         fi
# done



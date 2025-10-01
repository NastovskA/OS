# Да се напише PERL скрипта со име „prva.plx“ 
# која како аргументи добива најпрво збор кој ќе го
# пребарува, опција (знаменце) и име на датотека 
# во која ќе се прави пребарувањето. Скриптата треба да
# го пребара зборот (што се праќа како прв аргумент) 
# во зависност од опцијата (што се праќа како втор
# аргумент) во датотеката (чие име се праќа како трет аргумент). 
# Опцијата може да биде „l“, „w“ или „f“,
# притоа опцијата „l“ значи дека треба да се вратат броевите од 
# линиите во кои е пронајден зборот,
# „w“ како резултат враќа колку зборови биле пронајдени пред зборот 
# што се бара, и опцијата „f“ враќа
# колку пати е пронајден зборот во датотеката. Доколку зборот го нема
# во датотеката, опцијата „l“ враќа -
# 1, додека пак опциите „w“ и „f“ враќаат соодветно вредност 0. Зборовите
# се одделени еден од друг со
# празно место. Доколку добие невалидна опција, скриптата прекинува со 
# извршување.

#!/usr/bin/python3

import sys

if len(sys.argv) != 4:  # ime na py skripta (prva.plx), zbor, zname, datoteka
    quit()

zname = sys.argv[2]
if zname != 'l' and zname != 'w' and zname != 'f':
    quit()

Tocenzbor = sys.argv[1]
brojacL =0 #broevi od liniite vo koj go ima zborot
brojacW = 0 # brojac za zborovi pred zborot sto se bara
brojacF =0 # kolku bati e pronajden zborot vo datotekata

dat = open(sys.argv[3], "r")
for i in dat:  # dali treba da pisuva dat 
    brojacL = brojacL + 1
    zborovi = i.split(" ")
    for zborce in zborovi:

        if zborce == Tocenzbor:
            brojacF = brojacF + 1
            if zname=='w':
                print(brojacW)
            elif zname == 'l':
                print(brojacL)
        else:
            brojacW = brojacW + 1
if zname == 'f':
    print(brojacF)

dat.close()


# . Да се напише SHELL скрипта која како аргумент добива име на PYTHON 
# скрипта, збор за
# пребарување и листа од имиња на датотеки. Доколку не добие како прв 
# аргумент име на PYTHON
# скрипта (аргумент што завршува на .plx) тогаш зема како име на 
# скрипта „prva.plx“. Shell
# скриптата треба да провери дали има PYTHON скрипта со такво име 
# во тековниот директориум
# или поддиректориумите и доколку нема, завршува Shell скриптата со
# извршување. Доколку
# постои PYTHON скрипта со такво име (тековниот директориум или 
# поддиректориумите), тогаш
# Shell скриптата треба со помош на PYTHON скриптата, да најде и
# отпечати на екран, колку пати ќе
# го пронајде зборот (што се праќа како втор аргумент на Shell) за 
# сите датотеки што се праќаат
# како аргументи (од трет аргумент па натаму). На крај скриптата треба
# да отпечати колку
# вкупно бил пронајден зборот во сите датотеки.


#!/bin/sh

if [ $# -lt 3 ]; then
    echo "Koristenje: $0 [skripta.py] [zbor] [datoteki......]"
    exit 1 

skripta = "$1"
zbor = "$2"

if[[ "$skripta" != *.py ]]; then
skripta="prva.plx"
fi

pat_do_skripta = $(find . -type f -name "$skripta" 2>/dev/null)
if [ -z "$Ne e najdena skriptata"]
    exit 1
fi

vkupno =0

for dat in "$@:3"; do
if [! -f "$dat" ]; then
    echo "Ne postoi datotekata!"
    continue
fi

rezultat = $(21_11_2022_KOLOKVIUM_PRV_PY "$pat_do_skripta" "f" "$dat")
echo "Vo datotekata $dat zborot \"$zbor\" e pronajden $rezultat pati."

vkupno=$((vkupno + rezultat))
done

echo " Vkupno pronajdeni $vkupno pojavi na zborot \"$zbor\" vo site datoteki"


# SHELL SKRIPTATA

#!/bin/bash

# if [[ $# -lt 2 ]]; then
#     echo "Nevaliden broj na argumenti"
#     exit 1
# fi


# dali=$(echo $1 | grep .py3)
# dat=""
# zbor=""

# if [ -z "$dali" ] 
# then
#     dat="prva.py3"
#     zbor=$1
# else
#     dat=$1
#     zbor=$2
# fi

# for d in "${@:3}"; do
    
#     broj=$(python3 $dat $zbor f $d)
    
#     echo "$d $broj"

#     vkupno=$((vkupno + broj))
# done

# echo "Vkupno: $vkupno"



# #!/bin/sh
# # Проверка дали има доволно аргументи
# if [ $# -lt 3 ]; then
#     echo "Koristenje: $0 [skripta.py] [zbor] [datoteki......]"
#     exit 1 
# fi

# # Земање на првите два аргументи
# skripta="$1"
# zbor="$2"

# # Отстранување на првите два аргументи за да останат само датотеките
# shift 2  

# # Ако скриптата не е .py, постави ја на "prva.plx"
# if [[ "$skripta" != *.py ]]; then
#     skripta="prva.plx"
# fi

# # Наоѓање на патот до скриптата
# pat_do_skripta=$(find . -type f -name "$skripta" 2>/dev/null)

# if [ -z "$pat_do_skripta" ]; then
#     echo "Ne e najdena skriptata!"
#     exit 1
# fi

# vkupno=0

# # Обработка на датотеките (останатите аргументи)
# for dat in "$@"; do
#     if [ ! -f "$dat" ]; then
#         echo "Ne postoi datotekata: $dat"
#         continue
#     fi

#     # Извршување на Python скриптата и зачувување на резултатот
#     rezultat=$(python3 "$pat_do_skripta" "$zbor" "$dat")
    
#     echo "Vo datotekata $dat zborot \"$zbor\" e pronajden $rezultat pati."

#     # Сумирање на вкупниот број појави
#     vkupno=$((vkupno + rezultat))
# done

# echo "Vkupno pronajdeni $vkupno pojavi na zborot \"$zbor\" vo site datoteki."

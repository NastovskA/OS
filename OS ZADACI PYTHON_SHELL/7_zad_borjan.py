# Да се напише Python скрипта која како аргументи од командна линија добива листа од кориснички
# имиња. Секое корисничко име започнува со специјалниот знак „@“ а потоа следи листа од букви и
# бројки. Невалидните имиња од командна линија да се игнорираат. Скриптата дозволува да бидат
# внесени непознат број на имиња на датотеки од тастатура (прекинува кога ќе се внесе зборот „kraj“).
# Скриптата прави статистика за секоја една датотека, притоа статистиката вклучува секое едно
# корисничко име, во која линија од датотеката се појавува. Скриптата на екран ги печати (во секој ред
# одделно) во кој ред било пронајдено корисничкото име (бројот на редот) и кои други кориснички имиња
# биле пронајдени во истиот ред.


#!/usr/bin/python3

import sys

if len(sys.argv) < 2: #skripta, lista
    print("Nemate dovolno argumenti!")
    quit()

validni_korisnicki = []
for element in sys.argv[1:]: #imam lista koja ne znae kolku argumenti ima
    if element.startswith('@'):
        validni_korisnicki.append(element)

if len(validni_korisnicki) == 0:
    print("Nema validni korisnicki iminja!")
    quit()

statistika = {}

while True:
    ime_datoteka = input("Vnesi ime na datoteka - napisi kraj za da zavrsis: ").strip()
    if ime_datoteka == 'kraj':
        break

    dat = open(ime_datoteka, "r")
    Brojlinija = 0
    for linija in dat:
        Brojlinija += 1
        pronajdeni = []

        for ime in validni_korisnicki:
            if ime in linija:
                pronajdeni.append(ime)

        if len(pronajdeni) > 0: # ako imame barem edno korisnicko ime vo red od datotekata
            if Brojlinija not in statistika: #dali tekovniot red postoi vo recnikot
                statistika[Brojlinija] = [] #koga ne postoi go dodavame i mu davame prazen spisok koj ke gi cuva site korisnicki iminja koi se pojavile vo redot
                statistika[Brojlinija] += pronajdeni #go dodavam spisokot pronajdeni vo toj red
    dat.close()
for linija in sorted(statistika):
    print("Linija", linija, ": ", " ".join(statistika[linija]))

# statistika = {
#     3: ['@john', '@alice'],
#     5: ['@bob', '@john']
# }


# Да се напише SHELL скрипта која како аргумент добива листа од имиња на датотеки. 
# SHELL скриптата  треба да ја повика повика Python скриптата од првата задача онолку пати колку
# што има датотеки од командна линија. 
# При секој повик, на Python скриптата (како аргументи) ја испраќа содржината на соодветната датотека. 
# Резултатот од повикот го запишува во истата датотека само во продолжение (append).

# #!/bin/sh

# if [ $# -lt 1 ]; then     
#     echo "Vnesi argument!"
#     exit 1
# fi

# for datoteka in "$@"; do
#     if [-f "$datoteka" ]; then
#         sodrzina_od_datotekata = $(cat "$datoteka")
#         python3 7_zad_borjan.py "$sodrzina_od_datotekata" >> "$datoteka"
#         echo "Dodaden rezultat vo $datoteka"
#     else
#         echo "NEma datoteka"
#     fi
# done
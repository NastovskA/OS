# . Да се напише PYTHON скрипта за работа со датотеки. Скриптата, како аргумент од командна линија добива
# име на една влезна датотека. Скриптата треба да ги најде сите 
# телефонски броеви во датотеката и
# сопствениците на тие телефонски броеви. Секој телефонски збор е 
# составен од 9 цифри, притоа
# започнува со 07. Скриптата треба да ги најде сите различни 
# телефонски броеви и на крај да ги отпечати.
# Зборот пред секој телефонски број означува кој е корисникот
# на кој припаѓа тој телефонски број.
# Скриптата треба да го отпечати на екран името на
# сопственикот на телефонскиот број и самиот
# телефонски број. Доколку во датотеката истиот 
# телефонски број се појавува со различни корисници,
# тогаш се печати само последниот корисник што ќе биде
# пронајден во датотеката за тој телефонски број.
# Секој збор од друг збор во датотеката е одделен со празно место.


#!/usr/bin/python3

import sys

if len(sys.argv) < 2:  #skripta, datoteka 1
    print("Gresno vnesuvanje!")
    quit()

dat = open(sys.argv[1], "r")
telefon = {}
for linija in dat:

    zbor = linija.split(" ")
    if len(zbor) == 2:
        ime = zbor[0] 
        broj = zbor[1]

        if len(broj) == 9 and broj.isdigit() and broj.startswith("07"):
            telefon[broj] = ime
dat.close()

for broj, sopstvenik in telefon.items():
    print(f"Sopstvenik: {sopstvenik}, Telefon: {broj}")


# Потоа, да се напише Shell скрипта која како аргумент од командна линија добива еден телефонски број
# (низа од 9 цифри која започнува со 07). Скриптата треба да ја повика PYTHON скриптата од претходно со
# секоја една датотека што ќе ја најде во тековниот директориум со екстензија „.рр“. Скриптата треба да
# провери за секоја една датотека дали воопшто го има телефонскиот број што го добива како аргумент и
# доколку го има, да го отпечати името на корисникот на тој телефонски број.

# #!/bin/sh

# if [$# -lt 1 ]; then     #     $# broj na argumenti  telefonski broj
#     echo "Vnesi barem 1 telefonski broj!"
#     exit 1
# fi

# telefonski_broj = $1   #vaka se zima argument od komandna linija

# dolzina_telefonski_broj = $(echo $telefonski_broj | wc -c) 
# if ["$dolzina_telefonski_broj" -ne '10'] || ["${telefonski_broj:0:2}" != "07" ]; then
#     echo "Telefonskiot broj treba da zapocne so 07 i da ima 9 cifri."
#     exit 1
# fi

# for datoteka in *.pp; do # gi proveruva site datoteki vo tekoven direktorium
#     if[ -f "$datoteka" ]; then #dali postoi kako 
#         python3 24_06_2019_ispit.py "datoteka" "$telefon_broj"
#     fi

# done
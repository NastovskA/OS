# Да се напише PYTHON скрипта која како аргумент добива име на влезна 
# датотека. Скриптата треба да
# креира излезна датотека со исто име како и влезната, само со наставка 
# “_out”. Скриптата треба да ја
# измине влезната датотека линија по линија, и во излезната датотека да 
# ја запише линијата, така што
# доколку во линијата текст се наоѓа датум, датумот да дојде прв во 
# линијата, па потоа, без менување на
# редоследот, да дојде и останатиот текст од линијата. Форматот на датумот
# е “DD-MM-YYYY”, притоа,
# доколку има повеќе од еден датум, се запишуваат во излезната датотека 
# онолку линии колку што има
# различни датуми во влезната линија, притоа текстот што следи е без датуми.
# Доколку скриптата се
# повика со втор аргумент и доколку вториот аргумент е валиден датум, тогаш
# дополнително и на екран
# се печатат оние линии каде што го има тој датум. Зборовите во датотеката
# се одделени со празно место. 

#!/usr/bin/python3

import sys
if len(sys.argv) != 2:
    print("Pogresen broj na argumenti")
    quit()

dat = open(sys.argv[1], "r") # si ja otvaram datotekata za citanje
izlez = open(sys.argv[1] + "_out", "a") #imeto + _out ,  so "a" otvarame datoteka za dopisuvanje
# ako taa postoi novite podatoci ke bidat dodadeni na kraj

for linija in dat:
    zbor = linija.split(" ") # vraka lista so zborovi

datum = []
ostanato = []

for zborce in zbor:
    delovi = zborce.strip().split("-")  #strip za brisenje prazni delovi, split"-" gi deli zborovite so crticka pr: 01-01-2021   => ['01', '01', '2021']
    if len(delovi) == 3:
        if delovi[0].isdigit() and delovi[1].isdigit() and delovi[2].isdigit():
            den = int(delovi[0])
            mesec = int(delovi[1])
            godina = int(delovi[2])

            if(1<= den <=31) and (1<= mesec <= 12) and (1000<= godina <= 9999):
                datum.append(zborce.strip())
            else:
                ostanato.append(zborce.strip())
        else:
            ostanato.append(zborce.strip())

    else: 
        ostanato.append(zborce.strip())

    if len(datum) == 0: # ako nemame nieden datum vo listata
        izlez.write(linija)
    else:
        tekst = " ".join(ostanato) #site zborovi vo edna linija

        for d in datum: #gi zima datum po datum od linija
            nova_linija = f"{d} {tekst}\n" #format na zapis datum + ostanato
            izlez.write(nova_linija)

            
izlez.close()

dat.close()

# Да се напише SHELL скрипта која како аргумент добива листа од имиња од 
# датотеки. Скриптата, за
# секоја датотека (име од датотека) да провери дали ја има како датотека 
# во тековниот директориум, или
# под-директориуми. Доколку ја има, проверува дали датотеката 
# (во нејзината содржина) има барем еден
# валиден датум (DD-MM-YYYY), и доколку нема, ја игнорира. Доколку
# има валиден датум, тогаш ја
# повикува PYTHON скриптата и како прв аргумент го праќа името на датотеката,
# и т.н. со сите датотеки што
# се пратени како аргументи од командна линија. За секоја датотека што ќе 
# биде пратена
# на PYTHON скриптата, се печати на екран нејзиното име и колку различни 
# датуми се пронајдени.

# #!/bin/sh

# if [ $# -lt 1 ]; then     #     $# broj na argumenti podneseni pri povik,     -lt = low then 1 
#     echo "Vnesi barem 1 ime na datoteka!"
#     exit 1
# fi

# for dat in "$@"; do  # gi zima site argumenti ke gi napisam vo komandna
# jaIma=$(find . -name "$dat") #bara datoteki vo tekovniot direktorium i site site negovi poddirektoriumi isti ime kako dat
# # . oznaka za tekoven direktorium
# if [ -n "$jaIma" ]; then # -n proveruva dali promenlivata ne e prazna
#    valid=$(cat $jaIma | grep [1-31]\-[1-12]\-[0-9][0-9][0-9][0-9])
#    # cat = ja cita sodrzinata na datotekata $jaIma
#    if [-n "$valid"]; then # proveruva dali promenlivata ne e prazna
#    count=$(echo "$valid" | wc -l) #broime kolku elementi ima

# echo "Datotekata $dat sodrzi $count razlicni datumi."

# python3 2019_2020-prv_kol.py "$jaIma"
#         fi
#     else
#         echo "Datotekata $dat ne postoi"
#     fi
# done
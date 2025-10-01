# Да се напише PYTHON скрипта која како аргументи од командна линија добива
# имиња на две датотеки
# датотеки. Во првата датотека, во секоја линија одделно, се запишани
# наредби кои треба да ги
# препознава скриптата (секоја наредба е еден збор – низа од букви).
# Скриптата треба да ги најде сите
# наредби (дефинирани во првата датотека) во втората датотека. Секоја 
# наредба започнува со името на
# наредбата, после која, одделени со празно место, се наведени аргументите
# на таа наредба. За аргументи
# на дадена команда се сметаат сите зборови после командата (одделени со 
# празно место) кои започнуваат
# со цртичка (-). Низата аргументи завршува кога ќе се најде првиот збор 
# различен од цртичка, притоа
# може да се случи некои аргументи да се простираат и во повеќе линии од
# датотеката. За сите најдени
# наредби (со нивните аргументи) ги печати на екран. Доколку некоја наредба
# се појавува повеќе пати во
# датотеката, на екран се печати еднаш само, притоа аргументите се додаваат
# на првото појавување на
# наредбата, притоа започнуваат со „-е“ аргумент. 

#!/usr/bin/python3

import sys

if len(sys.argv) != 3:
    print("Gresno vnesuvanje!")
    quit()

dat1 = open(sys.argv[1], "r")
dat2 = open(sys.argv[2], "r")
Datoteka1_komandi = []

for linija in dat1:
    element = linija.strip() # ke gi spojam site novi linii vo edna niza
    Datoteka1_komandi.append(element)   
dat1.close()

najdeni_komandi = []

for linija in dat2:
    element=linija.split(" ")
    if len(element) == 0: #ako listata mi e prazna nemam elementi 
        continue
    #[stop, a, i, o u]
    Datoteka2_komanda = element[0] #prviot element vo nizata da bide komanta
    if Datoteka2_komanda in Datoteka1_komandi:
        najdena = False
        for i in najdeni_komandi:
            if i[0] == Datoteka2_komanda:
                najdena=True
                for el in element[1:]: #iterira niz celata linija od datoteka2
                 if el.startswith("-"):
                    i[1].append("-e" + el[1:])
                 else:
                    break
                break
            
        if not najdena:
            argumenti = []
            for el in element[1:]: #trgnuvame od 1 bidejki 0 ni e komandata
                if el.startswith("-"):
                    argumenti.append("-e" + el[1:])
                else:
                    break
            najdeni_komandi.append([Datoteka2_komanda, argumenti])
        
dat2.close()

#Ispis na rezultatite
for Datoteka2_komanda, argumenti in najdeni_komandi:
    print(Datoteka2_komanda, " ".join(argumenti))



# Да се напише SHELL скрипта која како аргумент од командна линија добива
# име на влезна датотека.
# Доколку не се прати аргумент на командна линија, тогаш се пребарува 
# датотека со име „naredbi.txt“ во
# тековниот директориум и неговите поддиректориуми и доколку се најде, 
# тогаш се копира таа датотека
# во тековниот директориум (доколку истата не е во тековниот директориум).
# Потоа, скриптата пребарува
# во тековниот директориум и неговите поддиректориуми датотека со име 
# „аctions.txt“. Доколку најде, ја
# повикува PYTHON скриптата од првата задача, каде како прв аргумент се праќа
# датотеката „naredbi.txt“, а
# како втор аргумент се праќа датотеката „actions.txt“, притоа втората 
# датотека се праќа со апсолутната
# патека каде ќе биде пронајдена. Секоја една наредба што ќе се врати од 
# извршувањето на PYTHON
# скриптата се извршува од страна на SHELL скриптата. Доколку наредбата 
# врати некаков текст од
# извршувањето, тогаш се печати името на наредбата и грешка во продолжение.
# Доколку не врати текст,
# тогаш се печати името на наредбата и текстот „ОК“ во продолжение.


# #!/bin/sh

# if [ $# -lt 1 ]; then
#     dat = $(find . -name "naredbi.txt" | head -1) then #tekoven dir i pod direktorium
# if [-n "$dat" ]; then
#     if[ "$(dirname "$dat")" != "."]; then #ako ne e vo tekoven direktorium
#         cp "$dat" .
#     fi
# else:
#     echo "Fajlot naredbi ne e pronajden!"
#     exit 1
# else:
#     dat="$1" #ako skriptata e povikana so argument zemi go argumentot
# fi

# action = $(find . -name "actions.txt")
# if [-z "$action" ]; then
#     echo "Ne e pronajden fajlot"
#     exit 1
# fi

# python3 script.py "$dat" "$(realpath "action")" | while read -r line; do  #script.py e imeto na py skriptata
# #ja zima sekoja linija od py skriptata i ja proaga niz while kade sto se cuva vo line
# output = $(eval "line" 2>$1) # prenasocuvanje na greska od 2 vo prviot argument
# if[ -n "$output"]; then
#     echo "$line: Greska: $output"
# else:
#     echo "$line: OK"
# fi
# done


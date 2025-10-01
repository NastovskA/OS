# Да се напише Shell скрипта која што ќе провери дали во тековниот директориум се наоѓаат
# датотеките „poraka.txt“. Доколку емаил адресата ја има воporaka.txt“ и „poraka.txt“. Доколку емаил адресата ја има воkontakti.vcf“. Доколку ги има, тогаш ги пребарува сите текстуални датотеки
# во тековниот директориум (со екстензија .txt, игнорирајќи ја датотеката „poraka.txt“. Доколку емаил адресата ја има воporaka.txt“) и секоја една
# датотека ја праќа на влез на повик на PERL скриптата заедно со „poraka.txt“. Доколку емаил адресата ја има воkontakti.vcf“. Секоја наредба што ќе се
# врати, SHELL скриптата треба да ја изврши.

#!/bin/bash


if [[ ! -f "poraka.txt" || ! -f "kontakti.vcf"]];then #dali ne postoi edna od ovie, koe bilo od ovie da e tocno ke se ispecati porakata
    echo "Datotekite ne postoat vo tekoven direktorium"
    exit 1
fi

for fajl in *.txt;do
    if[["$fajl" != "poraka.txt"]]; then

        komanda = (python3 prva.py "$fajl" "kontakti.vcf")

        for cmd in $komanda; do
            '$cmd' #so '' se IZVRSUVA
        done
    fi
done



# : Да се напише Shell скрипта која како аргумент од командна линија добива име на директориум (доколку не добие тогаш
# го зема тековниот директориум). Shell скриптата треба да ја изврши наредбата ls -al <ime_na_direktorium> и резултатот да го запише
# во датотека. Потоа, да ја повика Python скриптата од задача 1 на која ќе и ја прати датотеката како аргумент од командна линија.
# Python скриптата како резултат враќа низа од наредби кои Shell Скриптата треба да ги изврши. Доколку наредбата е наредба за
# компајлирање и доколку компајлирањето не врати празен стринг, тогаш следната наредба (следната линија) за извршување на
# компајлираната програма не се извршува. 


#!bin/bash
if [$# -lt 1];then
    direktorium = "." 
else
    direktorium = "$1"
fi

ime_datoteka = "izlezna.txt" #si ja kreiraj jas datotekata
ls -al "$direkorium" > "$ime_datoteka"

komandi = $(python3 prva.py "$ime_datoteka") #tuka se vraka lista od komandna linija


for kom in $komandi;do


# flag = false #Za dali ke treba da preskokneme sledna naredba

# while read -r linija;do  #vaka se cita sekoja linija od IZLEZ na skripta
#     if ["$flag" = true]; then
#         flag=false
#         continue
#     fi

#     if [["linija" == gcc*]];then #ako linijata zapocnuva so gcc ova e komanda sto treba da ja kompajlirame
#         echo "Kompajliram: $linija"
#         output=$(eval "$linija" 2>$1) #izvrsuvame so eval, se izvrsuva sodrzinata na linija

#         if [-n "$output"]; then #ako output ne e prazen, se pojavila greska vo kompajliranje
#             echo "Greska"
#             echo "$output"
#             flag = true
#         fi
#     else 
#         echo "Izvrsuvanje $linija"
#         eval "$linija"
#     fi

# done < izlezna.txt

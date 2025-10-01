# 1. Да се напише Python скрипта со име „prva.py3“ 
# која како аргумент добива влезна датотека и знаменце.
# Знаменцето може да биде „а“. Може да има и опционално
# дополнително знаменце „о“, после кое следи
# име на излезна датотека. Доколку нема дополнително
# знаменце „о“ и излезна датотека, се печати на
# екран, инаку, се печати во излезната датотека. 
# Скриптата треба, да ја измине влезната датотека и да ги
# пронајде во секој ред, IP адресите, притоа доколку најде во некој 
# ред IP адреса, можно е во истиот ред
# да има и маска за IP адреса. Форматот на IP адресите е х.х.х.х, каде 
# х е позитивен цел број во опсег од 0
# до 255, додека пак маската е х:х:х:х. Скриптата треба, ако најде само
# IP адреса во редот, да ја отпечати,
# но доколку најде маска во редот, да ги отпечати сите можни IP адреси
# во опсегот од IP адресата до
# маската, секоја во нов ред. Доколку има знаменце „а“, тогаш ги игнорира 
# маските, и ги печати сите IP
# адреси од таа IP адреса што ќе ја најде до 255.255.255.255 (да се 
# земе како маска за сите IP адреси да е
# оваа). 

#!/usr/bin/python3
import sys

if len(sys.argv) < 3:
    print("Gresno vnesuvanje!")  #skripta, vlezna datoteka, zname
    quit()

Vlezna_file_name = sys.argv[1]

if '-a' in sys.argv:
    izlezen_file_name = None


if '-o' in sys.argv:
    output_index = sys.argv.index('-o')
    if output_index +1 < len(sys.argv):
        izlezen_file_name=sys.argv[output_index + 1]


vlezna = open(Vlezna_file_name, "r")
izlezna = None
if izlezen_file_name:
    izlezna = open(izlezen_file_name, "w")

for line in vlezna:
    zborovi = line.strip().split(" ")

    if len(zborovi) == 2: # deka imam ip adresa i maska
        ip,maska = zborovi
        ip_delovi = ip.split('.')
        maska_delovi = maska.split('.')

        ip_delovi_int=[]
        maska_delovi_int =[]

        for dell in ip_delovi:
            ip_delovi_int.append(int(dell))
        for dell in maska_delovi:
            maska_delovi_int.append(int(dell))

        
    
    #for zbor in zborovi:
        # if '.' in zbor:
        #     delovi=zbor.split('.')
        #     if len(delovi) == 4:
        #         ip_broevi=[]
        #         validna = True
        #         for del_ip in delovi:
        #             if all(del_ip.isdigit()):
        #                 broj=int(del_ip)
        #             if 0<= broj <= 255:
        #                 ip_broevi.append(broj)
        #             else:
        #                 validna = False
        #                 break
        #         if validna:
        #             ip = f"{ip_broevi[0]}.{ip_broevi[1]}.{ip_broevi[2]}.{ip_broevi[3]}"

        #             if izlezna:
        #                 izlezna.write(ip + '\n')
        #             else:
        #                 print(ip)
vlezna.close()
if izlezna:
    izlezna.close()


#     2. Да се напише SHELL скрипта која треба да ги најде сите датотеки (во тековниот директориум и
# поддиректориумите) кои имаат наставка (екстензија) „.ip“. Потоа, користејќи ја Python
# скриптата од првата задача, да се генерираат сите можни IP адреси од секоја пронајдена
# датотека одделно. Откако ќе се изгенерираат IP адресите, за секоја IP адреса да се провери дали
# има пристап до истата, и анализата да се испише на екран (да се испише секоја IP адреса и до
# истата ОК или NOT во продолжение). Проверката дали има пристап или не до дадена IP адреса
# да се прави со наредбата „ping -c 1 $IP > /dev/null 2>&1; “

# #!/bin/sh

# tekoven_direktorium='.'

# datoteki = $(find "$tekoven_direktorium" -type f -name "*.ip")


# for dat in datoteki; do
#     echo "Obrabotuvam datoteki:  $dat"

#     ip_adr = $(python3 20_11_2023_prv_kolokvium.py "$dat")

# for ip in $ip_adr; do
#     if ping -c 1 "$ip" > /dev/null 2>&1; then
# echo "$ip OK"
# else
# echo "$ip NOT"
# fi
# done
# done


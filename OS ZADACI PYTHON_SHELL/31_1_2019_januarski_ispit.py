#  Да се напише PYTHON скрипта која како аргумент 
#  добива име на датотека во која се запишани
# контакти од даден именик. Во посебен ред од 
# датотеката се наоѓа секој еден од контактите, притоа во
# редот најпрво е редниот број на контактот, потоа одделен со 
# празно место е телефонскиот број, а потоа
# исто така одделен со празно место се сместени името и презимето.
#  Контактите треба да се сместат во
# Хеш структура, каде што клуч ќе е телефонскиот број, а како вредност ќе 
# се чува името и презимето на
# контактот. Потоа, скриптата треба да дозволи да се внесуваат непознат 
# број на команди од тастатура.
# Доколку од тастатура се внесе 1, тогаш низ именикот се пребарува 
# по телефонски број, што значи дека
# после тоа се внесува телефонскиот број кој се пребарува а на екран се 
# печати името и презимето на тој
# телефонски број (или дека контактот не постои). Доколку се внесе 2, 
# тогаш се пребарува по име и
# презиме и на екран се печати телефонскиот број за дадено име и презиме
#  внесени од тастатура. Доколку
# се внесе 3, се печати целиот именик со броевите придружени со име и 
# презиме. Доколку се внесе
# вредност различна од 1, 2 или 3, тогаш се прекинува со командите и 
# скриптата завршува. 

#!/usr/bin/python3

import sys

if len(sys.argv) != 2:
print("Nemate dovolno argumenti!")
quit()

dat = open(sys.argv[1], "r")

tel_broevi = {} # ako treba kluc da sozdadam

#      tel_broevi = {"071234567": "Petar Petrov"}
#          print(tel_broevi["071234567"])  # Дава: Petar Petrov


for linija in dat:

    element = linija.split(" ")
    if len(linija) >= 3: #moze da ima 2 preziminja
        telefon = element[1]

    if len(telefon) == 9 and telefon.isdigit() and telefon.startswith("07"):
     ime_prezime = " ".join(element[2:]) # so join gi spojuvam praznite mesta 
        tel_broevi[telefon] = ime_prezime # vaka se postavuva kluc

# lista = ["Ana", "Marija", "Anova"]
# rezultat = " ".join(lista)  # Спојување со празно место
# print(rezultat)  # "Ana Marija Anova"
# bez join ke dobijam samo ANA

dat.close()

while True:

print("1 - prebaruvaj po tel broj")
print("2 - prebaruvaj po ime i prezime")
print("3 - preku cel imenik ")

komanda = input("Vnesi komanda 1, 2 ili 3: ")

if komanda == "1":

telefon = input("Vnesi broj: ")

if telefon in tel_broevi:
print("Ime i prezime: {tel_broevi[telefon]}")
else:
print("Tel broj ne postoi!")

elif komanda == "2":

ime_prezime = input("Vnesi ime i prezime: ")
najdeno = False
for telefon, ime in tel_broevi.items():
if(ime == ime_prezime):
print("Telefonskiot broj e: " {telefon})
najdeno = True
break

if not najdeno:
print("Telefonski broj ne postoi")

elif komanda == "3":

if tel_broevi:
for telefon, ime in tel_broevi.items():
print("Tel broj: {telefon}, Ime i prezime: {ime}")
else:
print("Nevalidna opcija")
break



# Да се напише Shell скрипта која што ги наоѓа сите именици во 
# тековниот директориум т.е. ги
# бара сите датотеки кои што завршуваат на екстензија .vcf. Доколку 
# има повеќе именици, тогаш на крајот
# во првиот именик од листата именици се додава содржината на другите 
# именици и се повикува PYTHON
# скриптата со името на првиот именик. 

# #!/bin/sh

# imenici = $(ls | grep '\.vcf$')  #naoganje na site datoteki vo tekoven direktorium

# #  ls = site datoteki i papki vo tekoven direktorium
# #  grep '\ '  = filter za samo tie raboti
# #  $( )   = so ova se zacuvuva rezultatot vo promenliva

# kolku = $(echo "$imenici" | wc -l) 

# #  echo " "   = ja ispisuva vrednosta na imenicite 
# #  wc  -l    = se brojat kolku zborovi ili linii ima na vlez (-l) wc - word count

# if [$kolku -gt 1];  # kolku > 1
# # gt - greater than  
# then
# prv = $(echo "$imenici" | head -n 1)

# #head -n 1  =  go zima prviot red od vlez
# # mora da ja zacuvame za da gi staime site drugi vo nea

# for imenik in $(echo "$imenici" | tail -n +2); do

# # tail -n +2 ja ignorira prvata datoteka, i zapocnuva od vtora
# cat "$imenik" >> "$prv" #ovaa operacija se izvrsuva za site datoteki osven za prvata

# # cat " " = ja cita sodrzinata na datotekata i ja ispisuva vo vlez
# # >>   " " = dodadi na kraj vo prv datoteka
# done

# fi   # go zavrsuva uslovot
# python3 31_1_2019_janurarki_ispit.py "$prv"  #povik za skriptata
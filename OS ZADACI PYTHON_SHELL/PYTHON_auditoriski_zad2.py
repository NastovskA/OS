# Задача 2:
# Да се напише Python скрипта која од командна линија добива три
# датотеки. Скриптата треба да ги испреплети двете датотеки и
# излезот да го запише во трета датотека. Најпрво ќе се смести
# првата линија од првата датотека, па првата линија од втората и
# т.н. Дополнително, паралелно да се изврши печатење на екран
# излезот што се запишува во третата датотека.

#!/usr/bin/python

import sys

dat1 = open(sys.argv[1],"r") #ime na prva datoteka
dat2 = open(sys.argv[2],"r") #ime na vtora datoteka
dat3 = open(sys.argv[3], "w") # tretata e samo za zapisuvanje
dat1=0 # brojac za linii
dat2=0 #brojac za linii
pole1 = [] #listi za liniite
pole2 = [] #lista za linijata
 
for i in dat1: #niz eden red
    d1= d1+ 1  #brojac se zgolemuva
    pole1.append(i)  #kako prv element na nizata ke bide prviot red od dat1
for i in dat2:
    d2= d2+ 1
    pole2.append(i)

if d1 > d2:  # gi sporeduva brojacite
    min = pole2
    max = pole1

else:
    min = pole1
    max = pole2

j=0 # za pristap do elementite od max 
for i in min: #niz site redovi od pomalata, ovde se zgolemuva za minimalnata
    print(i) #prviot red od minimalnata
    print.(max[j]) # prviot red od maksimalnata
    dat3.write(i) #zapisi gi dvata reda vo dat3
    dat3.write(max[j])
    j= j+1 # ovde ke se zgolemi za maksimalnata niza

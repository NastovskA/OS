# Да се напише Python скрипта која како аргумент добива име на датотека во која се запишани 
# ценовник на производи. Во посебен ред од датотеката се наоѓа секој еден од производите, притоа во
# редот најпрво е баркодот на производот, потоа одделен со празно место е името на производот, а потоа
# исто така одделен со празно место е сместена цената на производот. Производите треба да се сместат во
# Хеш структура, каде што клуч ќе е баркодот, а како вредност ќе се чува името на производот. Треба да
# се креира и посебна Хеш структура каде како клуч ќе се земе повторно баркодот на производот, а како
# вредност ќе се чува цената на производот. Потоа, скриптата треба да дозволи да се внесуваат непознат
# број на команди од тастатура. Доколку од тастатура се внесе 1, тогаш низ ценовникот се пребарува по
# име на производ, што значи дека после тоа се внесува името на производот кој се пребарува а на екран
# се печати цената на производот (или дека производот не постои). Доколку се внесе 2, се листаат
# производите чија што цена им е помала од таа внесена од тастатура. Доколку се внесе 3, тогаш се
# печатат сите производи (секој производ во посебен ред) со името и цената на производот. Доколку се
# внесе вредност различна од 1, 2 или 3, тогаш се прекинува со командите и скриптата завршува. 

# Да се напише Shell скрипта која што ги наоѓа сите ценовници во тековниот директориум т.е. ги
# бара сите датотеки кои што завршуваат на екстензија .vcf. Доколку има повеќе ценовници, тогаш на
# крајот во првиот ценовник од листата ценовници се додава содржината на другите ценовници и се
# повикува python скриптата со името на првиот ценовник. 


#!/usr/bin/python3
import sys

if len(sys.argv) < 2:  #skripta, datoteka
    print("Gresno vnesuvanje!")
    quit()

barkodIME = {}
barkodCena = {}
dat = open(sys.argv[1], "r")
for linija in dat:
    elementi = linija.split(" ")
    if len(elementi) >=3:
        barkod = elementi[0]
    ime = " ".join(elementi[1: -1]) #od prviot do posledniot ako ima poveke iminja jadenjeto ke gi zeme pvoeketo
    cena = float(elementi[2:]) 
    barkodIME[barkod] = ime
    if cena.isdigit():
        barkodCena[barkod] = cena
dat.close()

while True:
    
    print("1 - prebaruvaj po ime na proizvod, treba da ovozmozime i vnes na ime= se pecati cena")
    print("2 - prebaruvaj po cena, vnesuvas i cena i barash po cenata pomali od taa")
    print("3 - pecati se ")
    
    komanda = input("Vnesi komanda 1, 2 ili 3: ")
    if komanda == 1:
        vleznoIme = input("Vnesi ime na hrana: ").strip()
        najden = False
        for barkod, ime in barkodIME.items():
                if ime.lower() == vleznoIme.lower():
                   print(f"{ime} so cena {barkodCena[barkod]}")
                   najden = True
        if not najden:
            print("Nema proizvod so toa ime!")
                
    elif komanda == 2:
        vleznaCena = float(input("Vnesi cena: ").strip())
        najden = False
        for barkod, cena in barkodCena.items():
            if cena <= vleznaCena:
                    print(f"Imeto na proizvodot e {barkodIME[barkod]} so cena {cena}")
                    najden = True
        if not najden:
                    print("Nema proizvod so cena poniska od ponudenata!")
   
    elif komanda == 3:
            for barkod, ime in barkodIME.items():
                 print(f"Proizvod {ime}, Cena: {barkodCena[barkod]}")
                 
    else:  
        print("Nevalidna vnesena opcija!!!")
        break 


# Да се напише Shell скрипта која што ги наоѓа сите ценовници во тековниот директориум т.е. ги
# бара сите датотеки кои што завршуваат на екстензија .vcf. Доколку има повеќе ценовници, тогаш на
# крајот во првиот ценовник од листата ценовници се додава содржината на другите ценовници и се
# повикува python скриптата со името на првиот ценовник. 

# #!/bin/sh
# tekoven_direktorium="."
# datoteki = $(find "$tekoven_direktorium" -type f -name "*.vcf")

# if [$(echo "$datoteki" | wc -l) -gt 1]; then
#      prv_cenovnik = $(echo "$datoteki" | head -n 1) #prvata linija od datotekata

#     ostanati = $(echo "$datoteki" | tail -n +2) #ja pecati listata na datoteki, od 2 red do kraj

#     for cenovnik in $ostanati; do
#      cat "$cenovnik" >> "$prv_cenovnik"
#     done

#     python3 2_zad_borjan.py "$prv_cenovnik"
# else
#     echo "Nema cenovnici!"
# fi
# done




# #!/bin/bash

# cenovnici=$(ls *.vcf)

# if [ -z "$cenovnici" ]; then
#     echo "Ne postoi nitu edna datoteka"
#     exit 1
# fi

# prv_cenovnik=$(echo "$cenovnici" | head -n1)

# broj_datoteki=$(echo "$cenovnici" | wc -l)

# if [ $broj_datoteki -gt 1 ]; then
    
#     for dat in $cenovnici; do
#         if [ "$dat" != "$prv_cenovnik" ]; then
#             cat "$datoteka" >> "$prv_cenovnik"
#         fi
#     done
# fi

# python3 zad2.py "$prv_cenovnik"
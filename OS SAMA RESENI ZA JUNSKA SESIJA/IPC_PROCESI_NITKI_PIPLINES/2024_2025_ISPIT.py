#ISPITNAA 2025
#!/usr/bin/python3
import sys

if len(sys.argv != 2){
    print("Nema vneseno datoteka!");
    sys.exit(1);
}

datoteka = open(sys.argv[1], "r")
# next(datoteka) - RIPAM SAMO PRVA LINIJA!
for red in datoteka:
    if red =="" or red.startswith("total"):
        continue
    
    zbor = red.split(" ")
    if len(zbor) == 9: #moze da se podeli tocno na 9 delovi
        permisii = zbor[0]
        ime_so_ekstenzija = zbor[8]
        ime_na_fajl = ime_so_ekstenzija.split(".")
        if len(ime_na_fajl == 2):
            samo_ime = ime_na_fajl[0]
            nastavka = ime_na_fajl[1]

            if nastavka == 'c': #napisi naredba za kompajliranje
                print(f"gcc {ime_so_ekstenzija} -o {samo_ime}")
                print(f"./{samo_ime}")
            elif nastavka == 'sh':

                if permisii[3] != 'x' and permisii[6] != 'x' and permisii[9] != 'x':
                    print(f"chmod +x {ime_so_ekstenzija}")
                print(f"./{ime_so_ekstenzija}")
datoteka.close()

#list(zborovi) //go zavuvuva po bukva
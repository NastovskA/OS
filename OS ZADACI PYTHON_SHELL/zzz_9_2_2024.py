
#!/usr/bin/python3
import sys

if len(sys.argv) != 3: 
    print("Gresno vnesuvanje!")
    quit()

dat1 = open(sys.argv[1], "r") #kod napisan
dat2 = open(sys.argv[2], "r") #dokument
niza=[]
for linija in dat2:

    element = linija.split(" ", 1)
    if len(element) == 2:
     broj = element[0]
     tekst = element[1].strip()
    niza.append((broj,tekst))

dat2.close()

sekoja_linija_vo_niza = dat1.readline()
dat1.close()







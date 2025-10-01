; Да се напише Python скрипта која ќе ги испечати првите линии од
; низа датотеки чии имиња се наведени на командна линија. Да
; се промени делимитерот за крај на ред, така што секоја линија
; да завршува со точка.

#!/usr/bin/python

import sys  ;za pristap na argumenti od komandna linija

print("Bez delimiter za tocka")
for dat in sys.argv[1:]:  ;lista na argumenti od komandna linija
;dat e sekoe ime na datotekata vo nizata
;sys.argv[0] ime na samata datoteka
;sys.argv e lista na argumenti sto se vnesuva pri start na programa

d = open(dat, "r")
print(d.readline())  ; citanje samo na prvata linija
d.close() ;zatvarame

print("So delimiter imame tocka")
for dat in sys.argv[1:]:
d = open(dat, "r")
s = ("".join(d.read())).split('.') ;so split go deli tekstot na delovi so .
;so d.read ja cita celata sodrzina na datotekata 
print(s[0]) ; nie so split ke gi podelime 1 recenica ke bide do 1 tocka i taa ke se postavi na 1 pozicija vo nizata
; vtorata recenica ke bide 2 element vo nizata
d.close()

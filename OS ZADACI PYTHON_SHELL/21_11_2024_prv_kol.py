# Задача 1: Потребно е да се напише Python скрипта за работа со крипто-валути. Скриптата добива на влез текстуална
# датотека во која, покрај другиот текст, се содржат вредности на крипто-валути. Скриптата треба да ја измине
# датотеката и за секоја одделна крипто-вредност да отпечати кое ќе биде најголемото зголемување од претходната
# вредност а кое ќе биде најголемото намалување од предходната вредност. На крај да се отпечатат, за секоја криптовалута одделно, најголемото зголемување на валутата и помеѓу кои вредности е истото, најголемото намалување на
# валутата и помеѓу кои вредности е истото. Крипто-валути кои се земаат во предвид се: BTC, Doge, EOS, ETH и XPM. Во
# текстот најпрво се наоѓа кратенката за крипто-валутата а потоа вредноста во долари. Сите зборови се одделени помеѓу
# себе со празно место. Доколку некоја крипто-валута ја нема во датотеката или се појавува еднаш, не се печати ништо
# за таа крипто-валута


#!/usr/bin/python3
import sys

if len(sys.argv) < 2:  #skripta, datoteka 1, dat2
    print("Gresno vnesuvanje!")
    quit()

dat = open(sys.argv[1], "r")

kripto_valuti = {"BTC" : [], "Doge" :[], "EOS" : [], "ETH" : [], "XPM" : []} #NA OVOJ NACIN KORISTAM RECNIK SO VREDNOSTI I SEKOJA VREDNOST IMA SVOJA LISTA

for linija in dat:
    zbor = linija.strip().split(" ")
    for i in range(len(zbor)): #odi do kraj na nizaata
        if zbor[i] in kripto_valuti:
            vrednost = zbor[i+1].replace("$", "") #zimam vrednost od sledniot zbor i go cistam od $ za da dobijam samo broj
            if vrednost.isdigit(): #se odiguram za vrednosta
                kripto_valuti[zbor[i]].append(float(vrednost))
dat.close()
print(kripto_valuti) #vaka ke go ispecatam cel so site vrednosti

for kv, vrednosti in kripto_valuti.items(): #so kv mi se kripto valutite zemeni, a so vrednosti iteriram niz listite od vrednosti
   #items vraka parovi na klucevi
   #ako nemam items ke gi dobijam samo nivnite iminja bez cenite
    if len(vrednosti) < 2:
        print(f"{kv} Nema dovolno vrednosti")
        continue

    max_pokacuvanje = 0
    max_namaluvanje = 0
    vrednost_pokacuvanje = ()
    vrednost_namaluvanje = ()

    for i in range(len(vrednosti) -1): # mora da ima -1 bidejki zimam vrednosti so i+1 sto znaci deka ke otide vo null ako ne odzemam
        razlika = vrednosti[i+1] - vrednosti[i]

        if razlika > max_pokacuvanje:
            max_pokacuvanje=razlika
            vrednost_pokacuvanje =(vrednosti[i], vrednosti[i+1])
        
        if razlika < max_namaluvanje:
            max_namaluvanje = razlika
            vrednost_namaluvanje = (vrednosti[i], vrednosti[i+1])
    
    print(f"{kv} najgolemo pokacuvanje {max_pokacuvanje} so vrednosti {vrednost_pokacuvanje}, najgolemo namaluvanje {max_namaluvanje} so vrednosti {vrednost_namaluvanje}")


# Задача 2: Да се напише Shell скрипта која треба да ги најде сите датотеки во тековниот директориум (и под-директориуми) чие што
# име завршува на екстензија „.crypto“. Потоа, за сите најдени датотеки, скриптата треба да провери во кои датотеки има барем еден
# од следниве крипто-валути како збор: BTC, Doge, EOS, ETH и XPM. Оние датотеки што го исполнуваат условот, нивната содржина се
# спојува во датотека со име „all.crypto“ (оваа датотека не постои, треба да се креира во тековниот директориум) а потоа се повикува
# Python скриптата од претходната задача, каде како аргумент и се испраќа датотеката „all.crypto“. Shell скриптата на екран печати
# колку крипто-вредности има пронајдено Python скриптата од претходната задача. 

# #!/bin/sh

# brojac_kripto_valuti = 0
# nova_datoteka = "all.crypto"
# >"nova_datoteka" # dokolku datotekata postoi da izbriseme, ako ne ja kreirame

# find . -type f -name "*.crypto" | while read datoteka; do #sekoj element od find sto zavrsuva na .crypto ide vo datoteka za da se obraboti
#     if grep -q -E "BTC|Doge|EOS|ETH|XPM" "$datoteka"; then
#     cat "$datoteka" >> "$nova_datoteka"

#     brojac_kripto_valuti=$((brojac_kripto_valuti+1)) #ako e pronajdena edna ke se zgolemi
#     fi
# done
# python3 21_11_2023_prv_kol.py "$nova_datoteka"
# echo "Pronajdeni se $brojac_kripto_valuti kripto valuti."

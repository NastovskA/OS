#!/bin/bash

fajlovi_od_sekade = $(find . -type f file -name "*.crypto") #ova se fajlovi od site direktoriumi i pod direktoriumi


touch ./all.crypto #sigurno ja kreira vo tekoven direktorium, bez./ ke bide samo kreirana

>./all.crypto

for fajl in $fajlovi_od_sekade
do
	if grep -E -q "BTC|Doge|EOS|ETH|XPM" "fajl" #proveruva dali postoi vrednosta 
	then
		cat "$fajl" >> ./all.crypto #zemi go tekstot i dodaj go
	fi

if [! -s ./all.crypto] #proverka dali fajlot ima sodrzina
then
	echo "Datotekata e prazna"
	exit 1
fi

broj_valuti = $(python3 kod_Zad1.py "$all.crypto")
echo "Pronajdeni se $broj_valuti vo python skriptata!"
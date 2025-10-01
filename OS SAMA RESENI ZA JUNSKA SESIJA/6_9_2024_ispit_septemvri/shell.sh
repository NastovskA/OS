#!/bin/bash

#$0 - imeto na skriptata
#$1 - prv argument


zbor = "$1"

fajlovi_od_sekade = $(find . -type f)

for fajl in $fajlovi_od_sekade; do
	if grep -q "$zbor" "$fajl"; then 
		echo "Fajl: $fajl "
		python3 prva.py "$fajl" "$zbor"
	fi

done
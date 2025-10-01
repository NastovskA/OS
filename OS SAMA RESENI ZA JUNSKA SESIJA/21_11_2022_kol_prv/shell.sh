#!/bin/bash

#   $# - broj na argumenti
#   $@ - sto tocno se tie arguumenit



if [$# -lt 3];do
	echo "Nema dovolno argumenti"
	exit 1
fi


ime_py = "$1"
zbor = "$2"

for datoteki in "${@:3}"; do


if ["$ime_py" != *.py]; then
	ime_py = "prva.py"
fi

najdena = $(find . -type f -name "$ime_py")
	if [-z "$najdena" ]; then
		echo "Nema takva skripta"
	fi

vkupno = 0

for fajl in "${@:3}"; do
	if [-f "$fajl"]; then #PROVERKA DALI FALJOT POSTOI
		rezultat = $(python3 "$najdena" "$fajl" "$zbor")
		echo "$fajl: $rezultat povai"
		(vkupno += rezultat)
	else 
		echo "Fajlot ne postoi"
	fi
done

echo "Vkupno pojavi na zborot '$zbor': $vkupno"


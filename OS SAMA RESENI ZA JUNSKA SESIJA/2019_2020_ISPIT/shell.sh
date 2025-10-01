#!/bin/bash

for i in "${@:1}"; do
	najdeno = $(find . -type f -name "$i" | head -n 1)
	
	if [-z "$najdeno"]; then
		echo "Fajlot ne postoi! "
		continue
	fi
	
	datumi = $(grep -q '\b(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-[0-9]{4}\b' "$najdeno")
	
	if [-z "$datumi"];then
		echo "Nema validen datum"
		continue
	fi

	razlicni=$(echo "$datumi" |sort |uniq)
	rezultat=$(python3 prva.py "$i" "$razlicni")
	echo "$rezultat"
done
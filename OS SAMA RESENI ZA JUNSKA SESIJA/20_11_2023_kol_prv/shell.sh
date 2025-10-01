#!/bin/bash

fajlovi_od_sekade = $(find . -type f -name "*.ip")


for file in $fajlovi_od_sekade; do
	
	generiraj_ip = $(python3 prva.py "$file")
	
	for ip in $generiraj_ip; do
		ping -c 1 "$ip" > /dev/null/ 2>$1
		if [$? -eq 0] #GO OZNACUVA REZULTATOT OD POSLEDNATA KOMANDA
		then
			echo "$ip -> OK"
		else
			echo "$ip ->NOT OK"
		fi
	done
	
	echo ""
done
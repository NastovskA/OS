#!/bin/bash

fajlovi_od_sekade_py = $(find . -type f -name "*.py")

fajlovi_od_sekade_co = $(find . -type f -name "*.co")

for fajl_py in $fajlovi_od_sekade_py; do
	ime_py = $(basename "$fajl_py" .py)
	
	for fajl_co in $fajlovi_od_sekade_co; do
		ime_co = $(basename "fajl_co" .co)
		
		if [ "ime_py" = "$ime_co"]; then
			povik = $(python3 prva.py "$fajl_py" "$fajl_co")
		fi
	done
done
	
#!/bin/bash

imenici = $(find . -type f -name "*.vcf")
prv=""
brojac = 0

for i in $imenici; do
	(brojac++)
	if [$brojac -gt 1]; then
		cat $i >> $prv
	else
		prv = $i

python3 prva.py "$prv"
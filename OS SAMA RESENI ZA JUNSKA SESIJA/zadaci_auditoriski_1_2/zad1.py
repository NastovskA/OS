#!/usr/bin/python3
import sys

print("Kako sto si e tekstot:")

for dat in sys.argv[1:]:
	d=open(dat,"r")
	print(d.readline())
	d.close()

print("Dodavam tocka na kraj na sekoja prvata recenica:")

for dat in sys.argv[1:]:
	d=open(dat, "r")
	recenica = d.read().split("\n")
	print(recenica[0].strip() + ".")
	d.close()


######print(d.readline().strip() + ".") 
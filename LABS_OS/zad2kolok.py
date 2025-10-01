import sys

if len (sys.argv) < 2:
    print ("Nedovolen br na argumenti")
    sys.exit()

dat = sys.argv[1]

if len(sys.argv) > 2 :
    if len(sys.argv[2].split("-")) == 3:
        flag = 1

d = open(dat, "r")
pole = []

for l in d.readlines():
    l.trim()
    zborovi = l.split(" ")

line = ""
p = []

for z in zborovi:
    if len(z.split("-")) == 3:
        p.append(z)
    else:
        line = line + " " + z
    if len(p) == 0:
        print (line)
    for z in p:
        print (z + " "+ line)
    if flag == 1:
        if sys.argv[2] in p:
            pole.append(line)



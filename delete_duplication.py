#function: delete the duplication row in txt!!!!!
#coding:utf-8
import shutil
readDir = "/home/uisee/linshu/fjy_second/imu-2.txt"
writeDir = "/home/uisee/linshu/fjy_second/imu-2-new.txt"
#txtDir = "/home/fuxueping/Desktop/１"
lines_seen = set()
outfile=open(writeDir,"w")
f = open(readDir,"r")
for line in f:
 if line not in lines_seen:
    outfile.write(line)
    lines_seen.add(line)
outfile.close()
print "success"


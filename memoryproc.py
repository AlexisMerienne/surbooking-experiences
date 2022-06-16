
import ast
import time
import sys


FILE = "/proc/1761670/status"
FILE_OUTPUT = "proc_out.txt"

TIME = 10
FINESSE = 1


def readfile(f):
    with open(f) as f:
        data = f.read()

    return data

def findmeminfo(data):
    
    
    for line in data.split("\n"):
        if 'VmRSS' in line:
            vmrss = [i for i in line.split() if i.isdigit()]
    
    return vmrss[0]




if __name__ == "__main__":



    if len(sys.argv)>1:
        TIME = float(sys.argv[1])
    if len(sys.argv)>2:
        FINESSE = float(sys.argv[2])

    start_time = time.clock()
    n=FINESSE

    procinfo = readfile(FILE)
    vmrss = findmeminfo(procinfo)
    res = "0.0" + " " + vmrss
    print(res)
    out = open(FILE_OUTPUT,"a")
    out.write(res+"\n")
    out.close()


    while(True):
        t = round(time.clock() - start_time,2)
        if t == n:
            procinfo = readfile(FILE)
            vmrss = findmeminfo(procinfo)
            res = str(t) + " " + vmrss
            print(res)
            out = open(FILE_OUTPUT,"a")
            out.write(res+"\n")
            out.close()
            n = round(n+FINESSE,2)
        if t == TIME:
            break
        
import subprocess
import sys
from socket import *
import time

def main():
    time.sleep(5)
    
    c = client()
    c.connect(port=25000, host="192.168.1.3")

    try:
        cmd = ['ls','-al']
        process = subprocess.Popen(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )

        while True:
            out = process.stdout.read(1)
            if out == '' and process.poll() != None:
                break
            if out != '':
                #sys.stdout.write(out)
                #sys.stdout.flush()
                c.send(out)
    except:
        c.send("DONE")
        c.close()

    c.send("DONE")
    c.close()

if(__name__ == "__main__"):
    main()

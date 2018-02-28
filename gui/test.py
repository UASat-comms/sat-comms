import subprocess
import sys
from sockets import *
import time
import os

def main():
    time.sleep(3)

    os.chdir("./../src/basicSystem")
    os.system("make -j 4")

    c = client()
    c.connect(port=25001, host="192.168.1.3")

    cmd = c.receive()
    cmd = cmd.split(' ')

    try:
        process = subprocess.Popen(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )

        msg = str()
        while True:
            out = process.stdout.read(1)
            if out == '' and process.poll() != None:
                break
            if out != '':
                if(out != '\n'):
                    msg += out
                elif(out == '\n'):
                    print(msg)
                    c.send(msg)
                    msg = ""

    except Exception as e:
        print("Error occured!: ")
        print(e)

    c.send("DONE")
    c.close()

if(__name__ == "__main__"):
    main()

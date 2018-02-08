from sockets import *

if __name__ == "__main__":
    c = client()
    c.connect(port=21700)
    print(c.receive())
    c.close()

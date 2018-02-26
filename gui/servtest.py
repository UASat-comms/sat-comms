from sockets import *

def f(client, addr):
    msg = client.receive()
    while(msg != "DONE"):
        print(msg)
        msg = client.receive()
    client.close()

STOP_FLAG = list()
STOP_FLAG.append(0)
flagLock = threading.Lock()

s = server(port = 25001,
           host = "192.168.1.3",
           func = f,
           stopFlag = STOP_FLAG,
           flagLock = flagLock,
           connections = 3,
           sigEnble = 1,
           sigTime = 25)

s.run()

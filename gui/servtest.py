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
           func = f,
           stopFlag = STOP_FLAG,
           flagLock = flagLock,
           connections = 3,
           sigEnble = 1,
           sigTime = 15)

s.run()

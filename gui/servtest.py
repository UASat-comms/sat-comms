from sockets import *

def f(client, addr):
  client.send("Hello")
  client.close()

STOP_FLAG = list()
STOP_FLAG.append(0)
flagLock = threading.Lock()

s = server(port = 15000, 
           func = f,
           stopFlag = STOP_FLAG,
           flagLock = flagLock,
           connections = 3,
           sigEnble = 1,
           sigTime = 3)

s.run()

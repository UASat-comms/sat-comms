from sockets import *
import time

c = client()
c.connect(port=21707)
while(True):
  try:
    msg = raw_input()
    c.send(msg)
    print(msg)
  except Exception as e:
    c.send("DONE")
    break
c.close()
time.sleep(3)

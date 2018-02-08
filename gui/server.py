from sockets import *
import hashes

def f(client, addr):
  lh = hashes.random_hash(length = 5)
  print("Connection to " + str(addr[0]) + " established.")
  plh = pickle.dumps(lh)
  client.send(plh)
  pha = client.receive()
  ha = pickle.loads(pha)
  print(ha)
  client.close()

if __name__ == "__main__":
  s = server(f, port = 22152, connections = 10)
  try:
    s.run()
  except Exception as e:
    print("Excetpion occured!")
    s.close()
    print(e)

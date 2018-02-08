from sockets import *
import hashes

if __name__ == "__main__":
  c = client()
  c.connect(port = 22152)
  plh = c.receive()
  lh = pickle.loads(plh)
  ha = hashes.find_hash(lh, length = 5)
  pha = pickle.dumps(ha)
  c.send(pha)
  c.close()

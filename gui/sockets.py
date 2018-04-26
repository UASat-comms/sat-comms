import socket
import zlib
import threading
import hashlib
import signal

try:
	import cpickle as pickle
except:
	import pickle as pickle

msgIdentifierLength = 15
defaultHost = socket.gethostname()
defaultPort = 15000

class client(object):
	def __init__(self, sock = None):
		if(sock == None):
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		else:
			self.sock = sock

	def connect(self, host = defaultHost, port = defaultPort):
		self.sock.connect((host, port))

	def send(self, msg):
		global msgIdentifierLength
		msg = zlib.compress(msg, zlib.Z_BEST_COMPRESSION)

		raw_len = len(msg)
		str_buffer = (msgIdentifierLength - len(str(raw_len))) * '0'
		str_buffer += str(raw_len)

		STR_LEN = str_buffer
		msg = STR_LEN + msg

		MSG_LEN = len(msg)

		bytes_sent = 0
		while bytes_sent < MSG_LEN:
			sent = self.sock.send(msg[bytes_sent:])
			if sent == 0:
				raise RuntimeError('[Err]: Socket connection broken')
			bytes_sent += sent

	def receive(self):
		# Get the length of the msg first.
		ident_chunks = list()
		ident_bytes_recd = 0
		while ident_bytes_recd < msgIdentifierLength:
			chunk = self.sock.recv(msgIdentifierLength - ident_bytes_recd)
			if chunk == '':
				raise RuntimeError("[Err]: Socket connection broken")
			ident_chunks.append(chunk)
			ident_bytes_recd += len(chunk)

		MSG_LEN = int(''.join(ident_chunks))

		# Get the real message now that we know how long it is.
		chunks = list()
		bytes_recd = 0
		while bytes_recd < MSG_LEN:
			chunk = self.sock.recv(MSG_LEN - bytes_recd)
			if chunk == '':
				raise RuntimeError('[Err]: Socket connection broken')
			chunks.append(chunk)
			bytes_recd += len(chunk)

		return zlib.decompress(''.join(chunks))

	def close(self):
		self.sock.close()

class server(object):
	def __init__(self,
	  func,
	  stopFlag,
	  flagLock,
	  sigEnble = 0,
	  sigTime = 5,
	  port = defaultPort,
	  connections = 1,
	  closeAfterConnectLimit = 1,
	  host = defaultHost):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.threads = list()
		self.connections = connections
		self.closeAfterConnectLimit = closeAfterConnectLimit
		self.sock.bind((host, port))
		self.func = func
		self.stopFlag = stopFlag
		self.flagLock = flagLock
		self.sigEnble = sigEnble
		self.sigTime = sigTime
		self.elapsedConnections = 0

	def run(self):
		self.sock.listen(self.connections)

		while True:
			if((self.elapsedConnections == self.connections) and (self.closeAfterConnectLimit)):
				break
			self.flagLock.acquire()
			if(self.stopFlag[0] == 1):
				self.flagLock.release()
				print("Closing socket.")
				self.close()
				break
			else:
				self.flagLock.release()

			if(self.sigEnble):
				signal.signal(signal.SIGALRM, self.handler)
				signal.alarm(self.sigTime)
			print("Accepting sockets..")

			try:
				clientSocket, addr = self.sock.accept()
				signal.alarm(0)
				self.elapsedConnections += 1
				print("Socket accepted.")
				client_socket = client(sock = clientSocket)
				clientThread = threading.Thread(target = self.func, args = ([client_socket, addr]))
				clientThread.start()
			except Exception as e:
				print(e)
				break


	def handler(self, signum, frame):
		print("Timeout occured")
		self.flagLock.release()
		self.close()

	def close(self):
		self.sock.close()

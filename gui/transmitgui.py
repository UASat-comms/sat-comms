import Tkinter
import os
from sockets import *
import threading
import time
import constants

# This integer had to be wrapped in an array so that it could be passed by
# reference to another function/thread.
STOP_FLAG = [0]
FLAG_LOCK = threading.Lock()

# The name of the file to be sent through the sat-comms system. This file will
# be located in the home directory of the Tx RPi.
fname = ""

SYSTEMBOXFUNC = None
OUT = None

def RPiServerFunc(client, addr):
	try:
		cmd = "sudo ./send"
		client.send(cmd)

		while(True):
			msg = client.receive()
			print("Received MSG: " + msg)
			FLAG_LOCK.acquire()
			if(msg == "DONE"):
				break
			else:
				OUT.insert(Tkinter.END, msg + "\n")
				OUT.see("end")
			FLAG_LOCK.release()

	except Exception as e:
		print(e)
		client.close()
	client.close()

def serverFunc(client, addr):
	# Get the identifier message from the connecting machine.
	identifier = client.receive()

	# Identify the connecting machine and respond appropriately.
	if(identifier == constants.RxGUIIdentifier):
		SYSTEMBOXFUNC("[Info]: Received connection from Rx GUI.\n")

		# Tell the Rx Side to start and get things going.
		client.send(constants.RxGUIGO)

		# When the Rx side has started listening on the Rx RPi, we can
		# tell the Tx RPi to go.
		reply = client.receive()
		if(reply != constants.TxGUIGO):
			SYSTEMBOXFUNC("[Err]: Did not receive 'Tx GO' reply. Received: " + reply)

		SYSTEMBOXFUNC("Starting Tx RPi now.")

		# Tell the Remote Tx RPi to start
		cmdString = "sshpass -p Sat-comms7 ssh pi@192.168.1.2 'sh TEST.sh 26000 192.168.1.3' &"
		txStartResult = os.system(cmdString)

		# Check to see if the sshpass utility was successful in making the rmeote call.
		if(txStartResult > 0):
			SYSTEMBOXFUNC("[Err]: sshpass failed to start the 'REMOTE_COMMANDS.sh' shell script!\n")
			client.close()
			return

		# The remote RPi program will pause for a short period to allow this program
		# to get a server running to receive its connection; start the server and listen.
		serv = server(host="192.168.1.3",func=RPiServerFunc,sigEnble=0,sigTime=10,port=26000,flagLock=FLAG_LOCK,stopFlag=STOP_FLAG)
		serv.run()

		SYSTEMBOXFUNC("RPi Server Func running!")

	else:
		SYSTEMBOXFUNC("[Err]: Received connection from uknown machine!\n")
		client.close()
		return
	client.close()

class simpleapp_tk(Tkinter.Tk):
	def __init__(self, parent):
		Tkinter.Tk.__init__(self, parent)
		self.parent = parent
		self.initialize()

	def initialize(self):
		self.grid()

		# Label for Tx IP Address
		self.TxLabelVariable = Tkinter.StringVar()
		self.TxLabelVariable.set("Tx IP Address:")
		self.TxLabel = Tkinter.Label(self, textvariable=self.TxLabelVariable,
										anchor="w",fg="white",bg="blue")
		self.TxLabel.grid(column=0,row=0,columnspan=1,sticky="EW")

		# Entry field for Tx IP Address
		self.TxVariable = Tkinter.StringVar()
		self.TxVariable.set("192.168.1.1")
		self.Tx = Tkinter.Entry(self, textvariable=self.TxVariable)
		self.Tx.grid(column=0,row=1,columnspan=1,sticky="EW")

		# Label for Tx Password
		self.TxPasswordLabelVariable = Tkinter.StringVar()
		self.TxPasswordLabelVariable.set("Tx SSH Password:")
		self.TxPasswordLabel = Tkinter.Label(self,textvariable=self.TxPasswordLabelVariable,
										anchor="w",fg="white",bg="blue")
		self.TxPasswordLabel.grid(column=1,row=0,columnspan=1,sticky="EW")

		# Entry field for Tx Password
		self.TxPasswordVariable = Tkinter.StringVar()
		self.TxPasswordVariable.set("")
		self.TxPassword = Tkinter.Entry(self,textvariable=self.TxPasswordVariable)
		self.TxPassword.grid(column=1,row=1,columnspan=1,sticky="EW")

		# Label for local file to send
		self.FileLabelVariable = Tkinter.StringVar()
		self.FileLabelVariable.set("Enter local file to send:")
		self.FileLabel = Tkinter.Label(self,textvariable=self.FileLabelVariable,
										anchor="w",fg="white",bg="blue")
		self.FileLabel.grid(column=0,row=5,columnspan=2,sticky="EW")

		# Entry field for local file
		self.FileVariable = Tkinter.StringVar()
		self.FileVariable.set("File Name")
		self.File = Tkinter.Entry(self,textvariable=self.FileVariable)
		self.File.grid(column=0,row=6,columnspan=2,sticky="EW")

		# Button to initialize System
		self.CommandButton = Tkinter.Button(self,text="Transfer File & Start System",
											command=self.CommandButtonClick)
		self.CommandButton.grid(column=0,row=7,columnspan=2)

		# Label for System Textbox
		self.SystemLabelVariable = Tkinter.StringVar()
		self.SystemLabelVariable.set("System Information:")
		self.SystemLabel = Tkinter.Label(self,textvariable=self.SystemLabelVariable,
									anchor="w",fg="white",bg="blue")
		self.SystemLabel.grid(column=0,row=8,columnspan=2,sticky="EW")

		# Textbox for System Information
		self.SystemOutput = Tkinter.Text(self,width=125,height=10,relief=Tkinter.RIDGE,borderwidth=3)
		self.SystemOutput.grid(column=0,row=9,columnspan=2,sticky="EW")

		# Label for Textbox for comms from Pi #1
		self.Pi1LabelVariable = Tkinter.StringVar()
		self.Pi1LabelVariable.set("Output From Tx RPi:")
		self.Pi1Label = Tkinter.Label(self,textvariable=self.Pi1LabelVariable,
									anchor="w",fg="white",bg="blue")
		self.Pi1Label.grid(column=0,row=10,columnspan=2,sticky="EW")

		# Textbox for comms from Pi #1
		self.Output0 = Tkinter.Text(self,width=125,height=10,relief=Tkinter.RIDGE,borderwidth=3)
		self.Output0.grid(column=0,row=11,columnspan=2,sticky="EW")

		# Button to close system
		self.CloseButton = Tkinter.Button(self, text="Close System",
											command=self.CloseButtonClick)
		self.CloseButton.grid(column=0,row=14,columnspan=2)

		# Just make the window behave nicer.
		self.grid_columnconfigure(0,weight=1)
		self.resizable(False,False)
		self.update()
		self.geometry(self.geometry())
		self.Tx.focus_set()
		self.Tx.selection_range(0, Tkinter.END)

	# Helper function to easily print messages to the system output textbox.
	def addToSystemOutput(self, string):
		self.SystemOutput.insert(Tkinter.END, string)
		self.SystemOutput.see("end")

	# What happens when you hit the 'GO' button.
	def CommandButtonClick(self):
		global SYSTEMBOXFUNC
		SYSTEMBOXFUNC = self.addToSystemOutput

		global OUT
		OUT = self.Output0

		self.addToSystemOutput("[Info]: System started.\n")

		self.addToSystemOutput("[Info]: Starting server.\n")
		# [Critical]: the number of connections MUST be 2 since
		# we have 1 connection from Rx GUI and one from Tx RPI
		serv = server(host="192.168.1.3",func=serverFunc,sigEnble=1,sigTime=10,connections=1,port=25000,stopFlag=STOP_FLAG, flagLock=FLAG_LOCK)
		serv.run()
		self.addToSystemOutput("[Info]: Server completed running.\n")

	def CloseButtonClick(self):
		exit()

def main():
	app = simpleapp_tk(None)
	app.title("Sat-Comms Remote Command System")
	app.mainloop()

if __name__ == "__main__":
	main()

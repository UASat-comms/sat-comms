import Tkinter
import passwords
import os
from sockets import *
import threading

externalCommands = list()
lock = threading.Lock()

# This integer had to be wrapped in an array so that it could be passed by
# reference to another function/thread.
STOP_FLAG = [0]
FLAG_LOCK = threading.Lock()

def sendCommand(client, addr):
    try:
        lock.acquire()
        client.send("ls")
        client.close()
        FLAG_LOCK.acquire()
        if(len(externalCommands) == 0):
            STOP_FLAG[0] = 1
        FLAG_LOCK.release()
        lock.release()
    except Exception as e:
        print(e)
        lock.release()
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
        self.TxVariable.set("192.168.1.2")
        self.Tx = Tkinter.Entry(self, textvariable=self.TxVariable)
        self.Tx.grid(column=0,row=1,sticky="EW")

        # Label for Rx IP Address
        self.RxLabelVariable = Tkinter.StringVar()
        self.RxLabelVariable.set("Rx IP Address:")
        self.RxLabel = Tkinter.Label(self, textvariable=self.RxLabelVariable,
                                        anchor="w",fg="white",bg="blue")
        self.RxLabel.grid(column=0,row=2,columnspan=1,sticky="EW")

        # Entry field for Rx IP Address
        self.RxVariable = Tkinter.StringVar()
        self.RxVariable.set("192.168.1.1")
        self.Rx = Tkinter.Entry(self,textvariable=self.RxVariable)
        self.Rx.grid(column=0,row=4,sticky="EW")

        # Label for local file to send
        self.FileLabelVariable = Tkinter.StringVar()
        self.FileLabelVariable.set("Enter local file to send:")
        self.FileLabel = Tkinter.Label(self,textvariable=self.FileLabelVariable,
                                        anchor="w",fg="white",bg="blue")
        self.FileLabel.grid(column=0,row=5,sticky="EW")

        # Entry field for local file
        self.FileVariable = Tkinter.StringVar()
        self.FileVariable.set("File Name")
        self.File = Tkinter.Entry(self,textvariable=self.FileVariable)
        self.File.grid(column=0,row=6,sticky="EW")

        # Button to initialize System
        self.CommandButton = Tkinter.Button(self,text="Transfer File",
                                            command=self.CommandButtonClick)
        self.CommandButton.grid(column=0,row=7)

        # Textbox for output to user
        self.Output = Tkinter.Text(self)
        self.Output.grid(column=0,row=8,columnspan=1,sticky="EW")

        # Button to close system
        self.CloseButton = Tkinter.Button(self, text="Close System",
                                            command=self.CloseButtonClick)
        self.CloseButton.grid(column=0,row=9)

        # Just make the window behave nicer.
        self.grid_columnconfigure(0,weight=1)
        self.resizable(False,False)
        self.update()
        self.geometry(self.geometry())
        self.Output.insert(Tkinter.END, "==> GUI Initiated.\n")
        self.Tx.focus_set()
        self.Tx.selection_range(0, Tkinter.END)

    def CommandButtonClick(self):
        self.Output.insert(Tkinter.END,"Starting File Transfer System:\n")

        # Copy the local file to the Tx RPI
        #self.Output.insert(Tkinter.END,"1.) Copying <"
        #                    + self.FileVariable.get() + "> to <"
        #                    + self.TxVariable.get() + ">...")
        #[NOTE]: Temporaily set to Rx side.
        #os.system("sshpass -p \"" + passwords.TxPassword + "\" scp " +
        #            self.FileVariable.get() + " pi@"
        #           + self.RxVariable.get() + ":~/")
        #self.Output.insert(Tkinter.END, "DONE.\n")

        # Spawn 2 other terminals
        self.Output.insert(Tkinter.END,"2.) Spawning additional terminals..")
        os.system("sudo open -a Terminal test.sh")
        os.system("sudo open -a Terminal test.sh")
        self.Output.insert(Tkinter.END,"DONE.\n")


        # Generate commands for other terminals
        self.Output.insert(Tkinter.END, "3.) Generating external commands..")
        # [NOTE]: Something with the way I'm joining the strings throws an error..
        #sendCommand = "sshpass -p " + passwords.RxPassword \
        #            + " ssh pi@" + self.RxVariable.get() \
        #            + " '~/REMOTE_COMMANDS.sh " + self.FileVariable.get() \
        #            + "'"
        #recvCommand = "sshpass -p " + passwords.RxPassword \
        #            + " ssh pi@" + self.RxVariable.get() \
        #            + " '~/REMOTE_COMMANDS.sh'"
        # [NOTE]: double send for testing
        #externalCommands.append(sendCommand)
        #externalCommands.append(sendCommand)
        self.Output.insert(Tkinter.END, "DONE\n")

        # Start up the server to send command to other terminals
        self.Output.insert(Tkinter.END,"4.) Starting server to connect to other terminals.\n")
        serv = server(func=sendCommand,connections=2,port=21706,stopFlag=STOP_FLAG, flagLock=FLAG_LOCK)
        serv.run()
        self.Output.insert(Tkinter.END,"5.) Server has been closed.\n")

    def CloseButtonClick(self):
        exit()


def main():
    app = simpleapp_tk(None)
    app.title("Sat-Comms Remote Command System")
    app.mainloop()

if __name__ == "__main__":
    main()

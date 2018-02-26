import Tkinter
import os
from sockets import *
import threading
import time

outputBoxes = [0, 1]
lock = threading.Lock()

OUT0 = None
OUT1 = None

# This integer had to be wrapped in an array so that it could be passed by
# reference to another function/thread.
STOP_FLAG = [0]
FLAG_LOCK = threading.Lock()

def sendCommand(client, addr):
    try:
        lock.acquire()
        outputBoxNum = outputBoxes.pop()
        lock.release()
        cmd = ""
        if(addr[0] == "192.168.1.1"):
            cmd = "./serialrecvtest 512"
        else:
            time.sleep(2)
            cmd = "./serialsendtest testdata"
        client.send(cmd)
        while(True):
            msg = client.receive()
            if(msg == "DONE"): break
            FLAG_LOCK.acquire()
            if(outputBoxNum == 0):
                OUT0.insert(Tkinter.END,msg + "\n")
            else:
                OUT1.insert(Tkinter.END,msg + "\n")
            FLAG_LOCK.release()
    except Exception as e:
        print(e)
        client.close()
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
        self.TxLabel.grid(column=0,row=0,columnspan=2,sticky="EW")

        # Entry field for Tx IP Address
        self.TxVariable = Tkinter.StringVar()
        self.TxVariable.set("192.168.1.2")
        self.Tx = Tkinter.Entry(self, textvariable=self.TxVariable)
        self.Tx.grid(column=0,row=1,columnspan=2,sticky="EW")

        # Label for Rx IP Address
        self.RxLabelVariable = Tkinter.StringVar()
        self.RxLabelVariable.set("Rx IP Address:")
        self.RxLabel = Tkinter.Label(self, textvariable=self.RxLabelVariable,
                                        anchor="w",fg="white",bg="blue")
        self.RxLabel.grid(column=0,row=2,columnspan=2,sticky="EW")

        # Entry field for Rx IP Address
        self.RxVariable = Tkinter.StringVar()
        self.RxVariable.set("192.168.1.1")
        self.Rx = Tkinter.Entry(self,textvariable=self.RxVariable)
        self.Rx.grid(column=0,row=4,columnspan=2,sticky="EW")

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
        self.CommandButton = Tkinter.Button(self,text="Transfer File",
                                            command=self.CommandButtonClick)
        self.CommandButton.grid(column=0,row=7,columnspan=2)

        # Label for System Textbox
        self.SystemLabelVariable = Tkinter.StringVar()
        self.SystemLabelVariable.set("System Information:")
        self.SystemLabel = Tkinter.Label(self,textvariable=self.SystemLabelVariable,
                                    anchor="w",fg="white",bg="blue")
        self.SystemLabel.grid(column=0,row=8,columnspan=2,sticky="EW")

        # Textbox for System Information
        self.SystemOutput = Tkinter.Text(self,width=100,height=10,relief=Tkinter.RIDGE,borderwidth=3)
        self.SystemOutput.grid(column=0,row=9,columnspan=2,sticky="EW")

        # Label for Textbox for comms from Pi #1
        self.Pi1LabelVariable = Tkinter.StringVar()
        self.Pi1LabelVariable.set("Output From Tx RPi:")
        self.Pi1Label = Tkinter.Label(self,textvariable=self.Pi1LabelVariable,
                                    anchor="w",fg="white",bg="blue")
        self.Pi1Label.grid(column=0,row=10,columnspan=1,sticky="EW")

        # Label for Textbox for comms from Pi #2
        self.Pi2LabelVariable = Tkinter.StringVar()
        self.Pi2LabelVariable.set("Output From Rx RPi:")
        self.Pi2Label = Tkinter.Label(self,textvariable=self.Pi2LabelVariable,
                                    anchor="w",fg="white",bg="blue")
        self.Pi2Label.grid(column=1,row=10,columnspan=1,sticky="EW")

        # Textbox for comms from Pi #1
        self.Output0 = Tkinter.Text(self,width=75,height=25,relief=Tkinter.RIDGE,borderwidth=3)
        self.Output0.grid(column=0,row=11,columnspan=1,sticky="EW")

        # Textbox for comms from Pi #2
        self.Output1 = Tkinter.Text(self,width=75,height=25,relief=Tkinter.RIDGE,borderwidth=3)
        self.Output1.grid(column=1,row=11,columnspan=1,sticky="EW")

        # Button to close system
        self.CloseButton = Tkinter.Button(self, text="Close System",
                                            command=self.CloseButtonClick)
        self.CloseButton.grid(column=0,row=12,columnspan=2)

        # Just make the window behave nicer.
        self.grid_columnconfigure(0,weight=1)
        self.resizable(False,False)
        self.update()
        self.geometry(self.geometry())
        self.Tx.focus_set()
        self.Tx.selection_range(0, Tkinter.END)

    def CommandButtonClick(self):
        self.SystemOutput.insert(Tkinter.END,"Starting File Transfer System:\n")

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
        #self.Output0.insert(Tkinter.END,"2.) Spawning additional terminals..")
        #os.system("sudo open -a Terminal test.sh")
        #os.system("sudo open -a Terminal test.sh")
        #self.Output0.insert(Tkinter.END,"DONE.\n")

        outputBoxes = [0, 1]

        global OUT0
        global OUT1
        OUT0 = self.Output0
        OUT1 = self.Output1

        os.system("sshpass -p Sat-comms7 ssh pi@192.168.1.1 'sh REMOTE_COMMANDS.sh' &")
        os.system("sshpass -p Sat-comms7 ssh pi@192.168.1.2 'sh REMOTE_COMMANDS.sh' &")

        # Start up the server to send command to other terminals
        self.SystemOutput.insert(Tkinter.END,"4.) Starting server to connect to other terminals.\n")
        serv = server(host="192.168.1.3",func=sendCommand,sigEnble=1,sigTime=10,connections=2,port=25001,stopFlag=STOP_FLAG, flagLock=FLAG_LOCK)
        serv.run()
        self.SystemOutput.insert(Tkinter.END,"5.) Server has been closed.\n")

    def CloseButtonClick(self):
        exit()


def main():
    app = simpleapp_tk(None)
    app.title("Sat-Comms Remote Command System")
    app.mainloop()

if __name__ == "__main__":
    main()

import Tkinter
import os
from sockets import *
import threading
import time

OUT0 = None
OUT1 = None

# This integer had to be wrapped in an array so that it could be passed by
# reference to another function/thread.
STOP_FLAG = [0]
FLAG_LOCK = threading.Lock()

# The name of the file to be sent through the sat-comms system. This file will
# be located in the home directory of the Tx RPi.
fname = ""

def sendCommand(client, addr):
    try:
        cmd = ""
        if(addr[0] == "192.168.1.1"):
            outputBoxNum = 0
            time.sleep(2)
            cmd = "sudo ./basicsend ./" + fname
            #cmd = "sudo ./basicsend ./../serialTesting/testdata"
        else:
            outputBoxNum = 1
            cmd = "sudo ./basicrecv"
        client.send(cmd)

        while(True):
            msg = client.receive()
            if(msg == "DONE"): break
            FLAG_LOCK.acquire()
            if(outputBoxNum == 0):
                OUT0.insert(Tkinter.END,msg + "\n")
                OUT0.see("end")
            else:
                OUT1.insert(Tkinter.END,msg + "\n")
                OUT1.see("end")
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

        # Label for Rx IP Address
        self.RxLabelVariable = Tkinter.StringVar()
        self.RxLabelVariable.set("Rx IP Address:")
        self.RxLabel = Tkinter.Label(self, textvariable=self.RxLabelVariable,
                                        anchor="w",fg="white",bg="blue")
        self.RxLabel.grid(column=0,row=2,columnspan=1,sticky="EW")

        # Entry field for Rx IP Address
        self.RxVariable = Tkinter.StringVar()
        self.RxVariable.set("192.168.1.2")
        self.Rx = Tkinter.Entry(self,textvariable=self.RxVariable)
        self.Rx.grid(column=0,row=3,columnspan=1,sticky="EW")

        # Label for Rx Password
        self.RxPasswordLabelVariable = Tkinter.StringVar()
        self.RxPasswordLabelVariable.set("Rx SSH Password:")
        self.RxPasswordLabel = Tkinter.Label(self,textvariable=self.RxPasswordLabelVariable,
                                    anchor="w",fg="white",bg="blue")
        self.RxPasswordLabel.grid(column=1,row=2,columnspan=1,sticky="EW")

        # Entry field for Rx Password
        self.RxPasswordVariable = Tkinter.StringVar()
        self.RxPasswordVariable.set("")
        self.RxPassword = Tkinter.Entry(self,textvariable=self.RxPasswordVariable)
        self.RxPassword.grid(column=1,row=3,columnspan=1,sticky="EW")

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

        # Label for Textbox for comms from Pi #2
        self.Pi2LabelVariable = Tkinter.StringVar()
        self.Pi2LabelVariable.set("Output From Rx RPi:")
        self.Pi2Label = Tkinter.Label(self,textvariable=self.Pi2LabelVariable,
                                    anchor="w",fg="white",bg="blue")
        self.Pi2Label.grid(column=0,row=12,columnspan=2,sticky="EW")

        # Textbox for comms from Pi #2
        self.Output1 = Tkinter.Text(self,width=125,height=10,relief=Tkinter.RIDGE,borderwidth=3)
        self.Output1.grid(column=0,row=13,columnspan=2,sticky="EW")

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

    def CommandButtonClick(self):
        self.SystemOutput.insert(Tkinter.END,"[Info]: Starting File Transfer System:\n")
        self.SystemOutput.see("end")

        # ========
        # Transfer the file we want to transmit thorugh the system to the home directory of the
        # Tx RPI
        self.SystemOutput.insert(Tkinter.END, "[Info]: Attempting to transfer local file to Tx RPi..\n")
        self.SystemOutput.see("end")

        # Make sure the file and Tx password fields are not empty.
        if(self.TxPassword.get() == "" or self.File.get() == ""):
            self.SystemOutput.insert(Tkinter.END, "[Error]: File name and Tx password cannot be empty!\n")
            self.SystemOutput.see("end")
            return

        # if not empty, attempt to transfer the file
        cmdString = "sshpass -p " + self.TxPassword.get() + " scp " + self.File.get() + " pi@" + self.Tx.get() + ":~/sat-comms/src/basicSystem"
        cmdResult = os.system(cmdString)

        # See if the file transfer was successful
        if(cmdResult != 0):
            self.SystemOutput.insert(Tkinter.END, "[Error]: Local file failed to transfer to Tx RPi!\n")
            self.SystemOutput.see("end")
            return
        else:
            self.SystemOutput.insert(Tkinter.END, "[Info]: Local file was transferred successfully.\n")
            self.SystemOutput.see("end")

        # Set the global file name now so the Tx RPi will receive the name of the file.
        global fname
        fname = self.File.get()
        # =======

        # =======
        # The threads that communicate to the RPIs need to be able to insert text into the textboxes
        # in the GUI, so make those textboxes global
        global OUT0
        global OUT1
        OUT0 = self.Output0
        OUT1 = self.Output1
        # =======

        # =======
        # The python programs on the RPis need to be started. Use 'sshpass' utility
        # to start those programs.

        self.SystemOutput.insert(Tkinter.END,"[Info]: Attempting to start remote python scripts..\n")
        self.SystemOutput.see("end")
        txStartResult = os.system("sshpass -p " + self.TxPassword.get() + " ssh pi@" + self.Tx.get() + " 'sh REMOTE_COMMANDS.sh' &")
        rxStartResult = os.system("sshpass -p " + self.RxPassword.get() + " ssh pi@" + self.Rx.get() + " 'sh REMOTE_COMMANDS.sh' &")

        # Make sure the programs started OK.
        if(txStartResult or rxStartResult):
            self.SystemOutput.insert(Tkinter.END,"[Error]: Something went wrong with starting remote python scripts.\n")
            self.SystemOutput.see("end")
            return
        else:
            self.SystemOutput.insert(Tkinter.END,"[Info]: Remote python scripts started.\n")
            self.SystemOutput.see("end")
        # =======

        # =======
        # Start up the server to send commands the python scripts on the RPis.
        self.SystemOutput.insert(Tkinter.END,"[Info]: Starting server to connect to remote python scripts.\n")
        self.SystemOutput.see("end")
        serv = server(host="192.168.1.3",func=sendCommand,sigEnble=1,sigTime=10,connections=2,port=25001,stopFlag=STOP_FLAG, flagLock=FLAG_LOCK)
        serv.run()
        # =======

        self.SystemOutput.insert(Tkinter.END,"[Info]: Server has been closed. Other threads may still be running.\n")
        self.SystemOutput.see("end")

        self.SystemOutput.insert(Tkinter.END,"[Critical]: Close GUI before doing another file transfer!")
        self.SystemOutput.see("end")

    def CloseButtonClick(self):
        exit()


def main():
    app = simpleapp_tk(None)
    app.title("Sat-Comms Remote Command System")
    app.mainloop()

if __name__ == "__main__":
    main()

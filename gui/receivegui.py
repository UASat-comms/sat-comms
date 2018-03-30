import Tkinter
import os
from sockets import *
import threading
import time

class simpleapp_tk(Tkinter.Tk):
    def __init__(self, parent):
        Tkinter.Tk.__init__(self, parent)
        self.parent = parent
        self.initialize()

    def initialize(self):
        self.grid()

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

        # Button to initialize System
        self.CommandButton = Tkinter.Button(self,text="Transfer File & Start System",
                                            command=self.CommandButtonClick)
        self.CommandButton.grid(column=0,row=7,columnspan=1)

        # Button to connect to server GUI.
        self.ConnectButton = Tkinter.Button(self,text="Connect to Tx GUI",
                                            command=self.ConnectButtonClick)
        self.ConnectButton.grid(column=1,row=7,columnspan=1)

        # Label for System Textbox
        self.SystemLabelVariable = Tkinter.StringVar()
        self.SystemLabelVariable.set("System Information:")
        self.SystemLabel = Tkinter.Label(self,textvariable=self.SystemLabelVariable,
                                    anchor="w",fg="white",bg="blue")
        self.SystemLabel.grid(column=0,row=8,columnspan=2,sticky="EW")

        # Textbox for System Information
        self.SystemOutput = Tkinter.Text(self,width=125,height=10,relief=Tkinter.RIDGE,borderwidth=3)
        self.SystemOutput.grid(column=0,row=9,columnspan=2,sticky="EW")

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

    # Helper function to easily print messages to the system output textbox.
    def addToSystemOutput(self, string):
        self.SystemOutput.insert(Tkinter.END, string)
        self.SystemOutput.see("end")

    def ConnectButtonClick(self):
        self.addToSystemOutput("[Info]: Starting Rx program.\n")

        # Create the client object and try to connect to the Tx GUI. If the
        # connection fails, notify user and return.
        c = client()
        try:
            c.connect(port=25000,host="192.168.1.3")
            self.addToSystemOutput("[Info]: Successfully connected to Tx GUI.\n")
        except Exception as e:
            self.addToSystemOutput("[Err]: Failed to connect to Tx GUI!\n")
            return

        # Notify the Tx GUI that the connection it has just received is from the
        # Rx GUI, not a RPi.
        c.send("RxGUI")

        # Get the response of the Tx GUI. If 'GO' is the reply, then start the
        # RX RPi by using 'sshpass' utility.
        reply = c.receive()
        if(reply == "GO"):
            self.addToSystemOutput("[Info]: Tx GUI successfully recognized Rx GUI.\n")
        else:
            self.addToSystemOutput("[Err]: Tx GUI did not recognize the Rx GUI!\n")

        # Close the connection to the Tx GUI.
        c.close()

    def CommandButtonClick(self):
        self.SystemOutput.insert(Tkinter.END,"[Info]: Starting receiveing system.\n")
        self.SystemOutput.see("end")

    def CloseButtonClick(self):
        exit()

def main():
    app = simpleapp_tk(None)
    app.title("Sat-Comms Remote Command System")
    app.mainloop()

if __name__ == "__main__":
    main()

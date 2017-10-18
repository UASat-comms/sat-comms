# sat-comms

## Overview
This is a project so communicate via RF, laser, and UV following the cubesat model.

## Notes

### Setting Up & Interfacing with RPi 3B

#### Setup
If imaging a new RPi, a handy setup shell script has been provided at sat-comms/usefulCommands/setupSystem.sh. To even gain access to the script on a running RPi, see "Communicating with the RPi."

This script does several things:
* Updates Linux packages.
* Downloads the bcm2835 library for the wiringPi.h & wiringSerial.h libraries. (For serial communication.)
* Installs vim for easy file editing over a network.
* Disables the console-over-serial connection, which is an RPi default configuration.
* Instructs the RPi to join an ad-hoc network on boot. (More on that below.)
**The system will need to be rebooted after the script runs to enable all the changes.**

Run the script via "sudo sh setupSystem.sh" and reboot the Raspberry Pi.

#### Communicating with the RPi.
There are a couple of ways to access and control a RPi with a freshly imaged OS.
1. The first, and probably the easiest, way is to connect a mouse & keyboard as well as an HDMI cable (plugged into a monitor) to the RPi. Connecting power to the RPi will cause it to boot and the user can use the GUI of the RPi to make changes through either the GUI or the command console.
2. Alternatively, If a mouse & keyboard are not available, one can use SSH to connect to the RPi through the ethernet port. From a fresh image, SSH is **not** enabled by default. To enable SSH, examine the instructions below. Once SSH is enabled, connect the RPi to some network via ethernet. Once connected, one will have to figure out the IP address that was assigned to the RPi. If using a personal router, logging into your router (usually by typing something such as "10.0.0.1" into the URL bar in a browser & logging in, but it varies according to manufacturer) will allow you see the assigned IP address. Finding the IP address on a large network (such as at a University) might be difficult. Using terminal or an SSH client, attempt to connect to the RPi. The default login credentials are "pi" for the user and "raspberry" for the password. **Change the password when possible!**
     * Insert the SD card into a computer.
     * Using a terminal, command line, etc., locate the SD card in the filesystem.
     * The SD card will have a "boot" folder located in the highest folder on the SD card. Change directories to this folder.
     * Once in the boot folder, create an empty file named "ssh." (*The existence of the "ssh" file in "boot" enables SSH when a RPi is booted.*)
     * Specifically, the commands on a Mac OS are as follows:
          * Open terminal.
          * cd /Volumes/boot/
          * touch ssh

An ad-hoc communication system is used to communicate with the RPis so that the cubes do not need to be tethered to another machine to receive commands. An ad-hoc network is essence a wifi network without a router, as the connections are peer-to-peer. Once a RPi is booted, you should see a network become available named "MyNetwork." This is the ad-hoc network used to communicate wirelessly with the RPis.

The default RPi address from the setup script is 192.168.1.1.  If you would to talk to multiple RPis at once, you MUST change the IP of the RPIs such that they all have IPs in the range of '192.168.1.1 <-> 192.168.1.255', but no two RPis may have the same IP address.  To edit the IP, edit either the 'ad-hoc-interfaces' file before running the setup script, or after running the script you can edit '/etc/network/interfaces' to make the change. The netmask is 255.255.255.0 always." **One will most likely need to set a manual IP address for their machine in their machine's network settings. Ensure that it is within the ad-hoc network's IP range. Make sure to NOT select an IP for your machine that is already taken by a RPi or you will not be able to join the network.**

One may use an ethernet connection to connect to the internet in parallel with the wireless ad-hoc network. Make sure the IP range set up by the ad-hoc network does not exist in the same range as the ethernet connection. For example, if your ad-hoc network is in the range '10.0.0.1 <-> 10.0.0.255,' then you will experience problems if the IP given to your RPi's ethernet interface is also in the '10.0.0.1 <-> 10.0.0.255' range, as can be assigned by DHCP. You could change the ad-hoc network range to be something along the lines of '192.166.2.1 <-> 192.166.2.255' to ensure that the wireless and wired interfaces will not interfere with one another."

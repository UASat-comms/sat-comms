#!/bin/bash

# Used for marking the upper-boundary of a set of system commands.
uppr() 
{
  echo '=================================================================='
  echo '------------------------------------------------------------------'
  printf '\n\n\n'
}

# Used for marking the lower-boundary of a set of system commands.
lwr() 
{
  printf '\n\n\n'
  echo '------------------------------------------------------------------'
  echo '=================================================================='
  printf '\n\n\n'
}

# Prints two newlines.
spcr()
{
  printf '\n\n'
}

uppr
echo "Starting system setup..."
lwr

uppr
echo "Updating package information ->"
echo "Mandatory: no"
echo "Desc.: To update the available Linux packages."
spcr
sudo apt-get update
sudo apt-get upgrade
lwr

uppr
echo "Installing: bcm2835 v.52 ->"
echo "Mandatory: yes"
echo "Desc.: To install dependencies for wiringPi C/C++ library (wiringPi.h, wiringSerial.h)"
spcr
wget 'http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz'
tar zxvf bcm2835-1.52.tar.gz
cd bcm2835-1.52
./configure
make
sudo make check
sudo make install
spcr
echo "Current pinout setup for wiringPi:"
gpio readall
lwr

uppr
echo "Installing VIM ->"
echo "Mandatory: no"
echo "Desc.: File editor"
spcr
sudo apt-get install vim
lwr

uppr
echo "Stopping and disabling console over serial ->"
echo "Mandatory: yes"
echo "Desc.: Prevents the RPi from wanting to send a remote console over serial connection."
sudo systemctl stop serial-getty@ttyS0.service
sudo systemctl disable serial-getty@ttyS0.service
lwr

uppr
echo "Swapping interfaces file with ad-hoc interfaces file ->"
echo "Mandatory: no"
echo "Desc.: Enables one to communicate with RPi over ad-hoc network"
echo "       with no router.  RPi can still use ethernet port to connect"
echo "       to the internet. This overrides the wi-fi on the connecting"
echo "       PC/MAC; therefore, you will not be able to connect to the internet"
echo "       over wi-fi on the connecting PC/MAC."
echo ""
echo "       The default RPi address from this script is 192.168.1.1.  If you would like"
echo "       to talk to multiple RPis at once, you MUST change the IP of the RPIs such"
echo "       that they all have IPs in the range of '192.168.1.1 <-> 192.168.1.255', but no"
echo "       two RPis may have the same IP address.  To edit the IP, edit either the"
echo "       './ad-hoc-interfaces' file before running this script, or after running"
echo "       this script you can edit '/etc/network/interfaces' to make the change."
echo "       The netmask is 255.255.255.0 always."
echo ""
echo "       !! -> For the network changes to take affect, the RPi(s) MUST be rebooted."
echo "       (You can use 'sudo reboot' to do this.)"
echo ""
echo "       !! -> You will need to set a static IP on your machine in your machine's"
echo "       network settings.  Your machine's IP also MUST be in the range of"
echo "       '192.168.1.1 <-> 192.168.1.255' in order for your machine to connect."
echo ""
echo "       !! -> The DEFAULT ssh credentials for a RPi are:"
echo "       user: 'pi'"
echo "       pass: 'raspberry'"
echo "       Though you should change the credentials so that others may not tamper"
echo "       with your device."
echo ""
echo "       !! -> To ssh into a RPi, use 'ssh USER@IP_ADDRESS'"
echo "       E.g.: 'ssh pi@192.168.1.1'"
echo "       You will be prompted for that machine's password."
echo ""
echo "       !!!!! -> SSH must be enabled in a RPi's settings before you can use any"
echo "       of the above information. If you do not have a mouse, wired keyboard," 
echo "       hdmi cord, and a hdmi-compliant screen, you will not be able to enable"
echo "       SSH via the GUI. You will need some form of a micro-sd card"
echo "       reader. Put the sd card into a computer (I think it has to be a UNIX"
echo "       based system becuase of the sd card formatting, can't remember exactly)"
echo "       and do the following:"
echo "       1. Open a terminal/command prompt."
echo "       2. 'cd /Volumes/boot' <- 'boot' is the sd card."
echo "       3. 'touch ssh'"
echo "       The existence of a 'ssh' file in the 'boot' directory signals that SSH"
echo "       is enabled on a RPi."
echo ""
echo "       !!!!! -> Make sure the IP range set up by the ad-hoc network does not exist"
echo "       in the same range as the ethernet connection. For example, if your ad-hoc"
echo "       network is in the range '10.0.0.1 <-> 10.0.0.255,' then you will"
echo "	     experience problems if the IP given to your RPi's ethernet interface is"
echo "       also in the '10.0.0.1 <-> 10.0.0.255' range, as can be assigned by DHCP."
echo "       You could change the ad-hoc network range to be something like"
echo "       '192.166.2.1 <-> 192.166.2.255' to ensure that the wireless and wired"
echo "       interfaces will not interfere with one another."
spcr
echo "Backing up existing /etc/network/interfaces file."
sudo cp /etc/network/interfaces ./interfaces.backup
echo "Placing ad-hoc-interfaces into place."
sudo cp ./ad-hoc-interfaces /etc/network/interfaces
lwr

uppr
echo "Done with system setup"
lwr

uppr
echo "RPi will need to be restarted for some changes to take affect."
echo "Use 'sudo reboot' to restart the RPi immediately, or use"
echo "'sudo shutdown now' to completely shutdown the RPi."
lwr

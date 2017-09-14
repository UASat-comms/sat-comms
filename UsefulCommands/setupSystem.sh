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
echo "Done with system setup"
lwr

uppr
echo "RPi will need to be restarted for some changes to take affect."
lwr

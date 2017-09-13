#!/bin/bash

echo "Starting dependencies install."

# This is necessary to get the latest package
# information.
echo "Updating package information"
sudo apt-get update
sudo apt-get upgrade

echo "Installing: bcm2835 v.52 dependency..."
echo ""
echo ""
wget 'http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz'
tar zxvf bcm2835-1.52.tar.gz
cd bcm2835-1.52
./configure
make
sudo make check
sudo make install
echo ""
echo ""
echo "Current pinout setup for wiringPi:"
gpio readall
echo ""
echo ""
echo "Installing VIM:"
sudo apt-get install vim
echo ""
echo ""
echo "Done."

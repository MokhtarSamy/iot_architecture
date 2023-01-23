#!bin/bash
sudo mount /dev/sda1 /media/usb-port
python3 /home/pi/projet1/udev/rule.py
sudo unmount /merdia/usb-port

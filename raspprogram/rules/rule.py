import shutil 

code = ""
f = open("/media/usb-port/raspcom/pass.txt", "r")
if(code == f.read()):
   shutil.copyfile("/media/usb-port/raspcom/images/matthis.py", "/home/pi/projet1/photos_train")

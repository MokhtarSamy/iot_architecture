# La distribution et l'utilisation:

Il existe plusieurs dossiers avec:

Le dossier arduino:

dépendances à inclure
#include <SPI.h> 
#include <MFRC522.h>
#include <TimeLib.h> 
#include <SimpleSleep.h>

Pour de l'arduino le programme se charge tout simplement via le port serie avec l'application.

Le programme communication.ino dans virtuel_voiture s'occupe comme le nom l'indique à simuler la voiture.
Il permet donc d'activer et de désactiver l'autre arduino et la raspberry ainsi que récuper le code via pin input de l'arduino.

Le programme projet_badge_active_rasp.ino permet grâce à la connexion du port serie de lancer plusieurs commandes:
1 rajouter un badge, 2 enlever un badge, 3 lire un badge -> après chaque commande il faut présenter la carte.
De plus en fonctionnement normal il récupère l'id du badge le vérifie puis attend l'entrée du code de la raspberry.
S'il reçoit le bon code il renvoie le résultat valide à la voiture virtuelle (arduino) son code.

Le dossier camera_detection_ordi est le code de base d'ia utilisé sur ordi.

Le dossier filesusb représente ce qui se trouve dans l'usb à connecter à la raspberry et permet de passer l'image à la raspberry.

Le dossier raspprogram est le code sur la raspberry:

Le raspberry pi dans ce projet est un os Raspian 2019

dépendances: libc numpy GPIO Picamera face_recognition time
Camera et Pin à activer via sudo raspi-config

La partie principale est le code reconnaisance_facial.py qui permet de reconnaitre un visage puis envoyer le code pin à l'arduino avec le badge.
Pour lancer cette partie de manière automatique nous pouvons rajouter vers la fin du fichier /etc/rc.local
un appel au programme: exemple python3 /home/pi/.../reconnaisance_faciale.py.
C'est un dossier spécialement prévu pour l'utilisateur root qui se lance quasiment à la fin du boot.
Vous pouvez vérifier le temps de validation de la camera via systemd-analyse

De plus dans le dossier rules vous trouverez comment récupérer les images de l'usb automatiquement.

(malheureusement j'ai perdu une petite partie qui permettait grâce à une règle @reboot d'avoir un pin pour éteindre la raspberry)
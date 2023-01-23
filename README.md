# La distribution et l'utilisation:

Il existe plusieurs dossier avec:

arduino:

dépendance à inclure
#include <SPI.h> 
#include <MFRC522.h>
#include <TimeLib.h> 
#include <SimpleSleep.h>

Pour de l'arduino le program ce charge tous simplement via le port avec l'application

Le program communication.ino dans virtuel_voiture s'ocupe comme le nom l'indique à simuler la voiture elle permet donc à activer et désactiver l'autre arduino et la rasp et récuper le code via pin input de l'arduino

Le program projet_badge_active_rasp.ino permet grace à la connection Série du port de lancer plusieurs command:
1 rajouter un badge, 2 enlever un badge, 3 lire un badge après chaque command il faut présenté la carte.
De plus en fonctionement normal il récuper l'id du badge le vérifie puis attend l'entré du code de la rasp.
Si il ressois le bon code il renvois le résultat valide à la voiture virtuel (arduino) le code.

Le dosier camera_detection_ordi et le code de base d'ia utiliser sur ordi

Le dosier filesusb représente ce qui ce trouve dans notre usb comme example pour passer l'image à la rasp.

Le dosier raspprogram et le code sur la raspberry:

La partie principale est le code reconnaisance_facial.py qui permet de reconnaitre un visage puis envoyé le code pin à l'arduino avec le badge.
Pour l'ancer cette partie de maniere automatique nous pouvons rajouter vers la fin du fichier /etc/rc.local
un appele exemple python3 /home/pi/.../reconnaisance_faciale.py
c'est un dossier spécialement prévue pour l'utilisateur root qui ce lance quasiment à la fin du boot.

De plus dans le dosier rules vue trouvais commant récuperer les image de l'usb automatiquement.

(malheureusement j'ai perdue une petit partie qui permetais grace à une reglle @reboot d'avoir un pin pour étaindre la rasp)
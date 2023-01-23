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

Le program projet_badge_active_rasp.ino permet grace à la connection Série du port lancé plusieurs command:
1 rajouter un badge, 2 enlever un badge 3 lire un badge après chaque command il faut présenté la carte


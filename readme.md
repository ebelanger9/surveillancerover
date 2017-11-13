# Rover de surveillance

Un robot de surveillance conçu dans le cadre du cours _Ingénierie : Méthodes et
pratiques II_.

Conçu par :
- Émile Bélanger
- Marc-André Bilodeau
- Gabriel Lapointe
- Stanislas Benjamin Nkawe
- Gabriel Tremblay
- Pierre-Olivier Otis

# Instructions de téléchargement :

Pour télécharger ce repos, dans une invite de commande _Git Bash_, écrire la
commande dans le dossier que vous désirez :

> git clone https://github.com/ebelanger9/surveillancerover.git

Notez que pour changer de dossier dans l'invite de commande, il faut utiliser

> cd _nomdudossier_

# Instructions pour l'upload

Pour téléverser le code du rover dans le rover, s'assurer d'être sur le bon port
COM avec la bonne carte (Duemilenove) puis appuyer sur téléverser.

Pour téléverser dans la carte Wifi du Rover, s'assurer que la librairie a bel et
bien été ajoutée au logiciel arduino en copiant le lien  http://arduino.esp8266.com/stable/package_esp8266com_index.json dans Fichiers->
Préférences->Informations de cartes additionnelles. Ensuite, télécharger la
librairie dans Outils->Cartes->Gestionnaire de carte; descendre jusqu'en bas et
télécharger la librairie esp8266. Changer le type de carte pour une carte HUZZAH
et régler la vitesse à 115200 baud pour ensuite téléverser sur le bon port COM.

Pour téléverser dans la carte Wifi de l'ordinateur, procéder de la même manière
que spécifiée ci-haut pour télécharger la librairie, appuyer sur le bouton
_Flash_ de la carte Wifi et téléverser sur le bon port COM.

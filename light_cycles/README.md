# Titan light tube sim 

## Programme simulant des neon titanb avec Raylib

### Ce programme simule des cycles de temps sur le principe suivant :

Chaque machine un temps d'ecriture différentes : 

- Argile             --> 1mot minute 
- Papier             --> 6.5s par mot 
- Machine à ecrire   --> 5.6s par mot
- Vielle ordinateur  --> 3.2s par mot 
- Ordinateur moderne --> 1.7s par mot 

Les valeurs sontr des moyenne que j'ai chronometrer. 

Chaque valeur est diviser par 3 pour avoir 3 phase dans ce lapse de temps :

1. Allumage douce de la lumiere 
2. Temps de lumiere constant 
3. Lumiere qui s'etaeint de maniere douce 

**A Faire :**

- Retravailler les couleur. 
- Voir si j'auitomatise le programme pour que chaque cycle fasse soit :  
  
   1. 5min si 25min/ 8min si 40min  
   2. Je relie a une "machine" qui permet de changer les cycles manuellement 


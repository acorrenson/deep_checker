# Projet de Statistiques, 2021
# Deep checker
## Arthur Correnson, Igor Martayan, Manon Sourisseau

## Introduction 


### Création d'un simulateur de jeu de dames

Pour avoir les données sur lesquels faire travailler notre heuristique, nous avons créé notre propre simulateur de jeu de dames. 
Le simaleur est écrit dans le langage de programmation C, afin de produire rapidemment une base de donneés d'apprentissage suffisemment grande. 

% Décrire comment marche le simulateur

% Décrire les optimisations de code faites

% Décrire la forme des données finales. 




### Heuristique


#### Présentation de KNN

Notre heuristique suit l'algorithme de KNN (K-Nearest Neighbors).
Cet algorithme de machine learning est une algorithme d'apprentissage supervisé, qui fonctionne de la manière suivante :

+ On calcule la distance entre l'entrée et chaque éléments de la  base de donnée 
+ On sélectionne les K plus proches voisins selon la distance calculés.
+ On attribut à notre entrée la moyenne des valeurs des K plus proches voisins. 

#### Adaptation de KNN à notre heuristique

Dans une situation de plateau donnée, on attribue un score à chaque coups possible avec l'aide de KNN :

+ choix de la distance : Nombre de bytes dans un Xor entre deux coups.
+ choix de K = 4 
+ La valeur associée aux K plus proches voisins correspond à la moyenne des scores.

Enfin, on sélectionne le coups possible ayant le plus grand score. 


## Conclusion


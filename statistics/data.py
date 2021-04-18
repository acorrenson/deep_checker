import pandas
    

data = []
with open("example.csv") as fichier :
    for game in fichier :
        coups = game.split()
        liste_couples = []
        for i in range (0,len(coups) - 1,2) :
            couple = (int(coups[i],16),int(coups[i+1],16))
            liste_couples.append(couple)
        data.append(liste_couples)


def distance_coups(coup1, coup2) :
    return bin(coup1^coup2).count('1')


# Here read the data

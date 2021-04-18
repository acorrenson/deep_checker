import pandas
import numpy as np



# Here read the data
data = []
with open("boards.csv") as fichier :
    for game in fichier :
        coups = game.split()
        liste_couples = []
        for i in range (0,len(coups) - 1,2) :
            couple = (int(coups[i],16),int(coups[i+1],16))
            liste_couples.append(couple)
        data.append(liste_couples)


# Distance computing functions

def distance_coups(coup1, coup2) :
    return bin(coup1^coup2).count('1')

def winner(game):
    return 1 - (len(game) % 2)

def recup_coups_winner(game,player):
    coups = []
    for i in range (player, len(game)-1, 2):
        coups.append((game[i], game[i+1]))
    return coups

def label(games):
    dictios = []
    for i in range (len(games)):
        dictio = {}
        game = games[i]
        player = winner(game)
        if player == 0 :
            coups_winner = recup_coups_winner(game, player)
            for j in range(len(coups_winner)) :
                dictio[coups_winner[j]] = 1 / ( len(coups_winner) - j )
        dictios.append(dictio) 
    return dictios
    
print(label(data))
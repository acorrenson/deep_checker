import pandas
import numpy as np


data = []

with open("boards.csv") as fichier:
    for game in fichier:
        coups = game.split()
        liste_couples = []
        for i in range(0, len(coups) - 1, 2):
            couple = (int(coups[i], 16), int(coups[i+1], 16))
            liste_couples.append(couple)
        data.append(liste_couples)


def move_distance(move1, move2):
    """
        Compute a distance btw 2 moves (edition distance)
    """
    return bin(move1 ^ move2).count('1')


def get_winner(game):
    """
        Returns the winner of the game (0 or 1)
    """
    return 1 - (len(game) % 2)


def get_player_moves(game, player):
    """
        Returns the sequence of moves of the given player
    """
    moves = []
    for i in range(player, len(game)-1, 2):
        moves.append((game[i], game[i+1]))
    return moves


def heuristic1(moves, i, win):
    return (1 if win else -1)/np.sqrt(len(moves) - i)


def heuristic2(moves, i, win):
    return (1 if win else -1)/(len(moves) - i)


def get_labels(games):
    """
        Associates scores to moves
    """
    labels = dict()
    for game in games:
        win = get_winner(game) == 0
        moves = get_player_moves(game, 0)
        for j in range(len(moves)):
            if moves[j] in labels:
                labels[moves[j]].append(heuristic1(moves, j, win))
            else:
                labels[moves[j]] = [heuristic1(moves, j, win)]
    return labels


labels = get_labels(data)
for k in labels:
    print(
        f'score({hex(k[0][0])}, {hex(k[0][1])}, {hex(k[1][0])}, {hex(k[1][1])}) : {sum(labels[k])/len(labels[k])}')

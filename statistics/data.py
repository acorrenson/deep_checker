from utils import vectorize_scores
import numpy as np
import joblib


def get_winner(game):
    """
    Returns the winner of the game (0 or 1)
    """
    return 1 - (len(game) % 2)


def get_player_moves(game: tuple, player: int):
    """
    Returns the sequence of moves of the given player
    """
    moves = []
    for i in range(player, len(game) - 1, 2):
        moves.append((game[i], game[i + 1]))
    return moves


def get_scores(games: list, heuristic):
    """
    Associates scores to moves
    """
    scores = dict()
    for game in games:
        win = get_winner(game) == 0
        moves = get_player_moves(game, 0)
        for j in range(len(moves)):
            if moves[j] in scores:
                scores[moves[j]].append(heuristic(moves, j, win))
            else:
                scores[moves[j]] = [heuristic(moves, j, win)]
    return scores


def heuristic1(moves: tuple, i: int, win: bool):
    return (1 if win else -1) / np.sqrt(len(moves) - i)


def heuristic2(moves: tuple, i: int, win: bool):
    return (1 if win else -1) / (len(moves) - i)


data = []

with open("boards.csv") as f:
    for i, game in enumerate(f):
        if i > 1:
            break
        moves = game.split()
        liste_couples = []
        for i in range(0, len(moves) - 1, 2):
            couple = (int(moves[i], 16), int(moves[i + 1], 16))
            liste_couples.append(couple)
        data.append(liste_couples)


scores = get_scores(data, heuristic1)
X, Y = vectorize_scores(scores)

joblib.dump((X, Y), "scores.save")

for k in scores:
    print(
        f"h({hex(k[0][0]), hex(k[0][1])} -> {hex(k[1][0]), hex(k[1][1])}) = {scores[k]}")

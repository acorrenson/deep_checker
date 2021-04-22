from utils import vectorize_scores, vectorize_scores_old
import numpy as np
import joblib
import sys


max_games = 5000
if len(sys.argv) > 1:
    max_games = int(sys.argv[1])
out = "scores.save"
if len(sys.argv) > 2:
    out = sys.argv[2]
games = []

with open("boards.csv") as f:
    for i, line in enumerate(f):
        if i >= max_games:
            break
        boards = line.split()
        game = []
        for i in range(0, len(boards) - 1, 2):
            state = (int(boards[i], 16), int(boards[i + 1], 16))
            game.append(state)
        games.append(game)


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
    Associates scores to states
    """
    scores = dict()
    for game in games:
        win = game[-1][1] == 0
        moves = get_player_moves(game, 0)
        for j in range(len(moves)):
            if moves[j][1] in scores:
                scores[moves[j][1]].append(heuristic(moves, j, win))
            else:
                scores[moves[j][1]] = [heuristic(moves, j, win)]
    return scores


def get_scores_old(games: list, heuristic):
    """
    Associates scores to moves
    """
    scores = dict()
    for game in games:
        win = game[-1][1] == 0
        moves = get_player_moves(game, 0)
        for j in range(len(moves)):
            if moves[j] in scores:
                scores[moves[j]].append(heuristic(moves, j, win))
            else:
                scores[moves[j]] = [heuristic(moves, j, win)]
    return scores


def heuristic_sqrt(moves: tuple, i: int, win: bool):
    return (1 if win else -1) / np.sqrt(len(moves) - i)


def heuristic_takes(moves: tuple, i: int, win: bool):
    op1 = moves[i][0][1]
    n1 = bin(op1).count("1")
    op2 = moves[-1][1][1]
    n2 = bin(op2).count("1")
    return n1 - n2


# scores = get_scores_old(games, heuristic_sqrt)
# X, Y = vectorize_scores_old(scores)

scores = get_scores(games, heuristic_takes)
X, Y = vectorize_scores(scores)

joblib.dump((X, Y), out)

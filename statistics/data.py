import numpy as np
from sklearn import neighbors
import joblib

data = []

with open("boards.csv") as fichier:
    for game in fichier:
        coups = game.split()
        liste_couples = []
        for i in range(0, len(coups) - 1, 2):
            couple = (int(coups[i], 16), int(coups[i + 1], 16))
            liste_couples.append(couple)
        data.append(liste_couples)


def move_distance(move1, move2):
    """
    Compute a distance between 2 moves (edition distance)
    """
    return bin(move1 ^ move2).count("1")


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
    for i in range(player, len(game) - 1, 2):
        moves.append((game[i], game[i + 1]))
    return moves


def heuristic1(moves, i, win):
    return (1 if win else -1) / np.sqrt(len(moves) - i)


def heuristic2(moves, i, win):
    return (1 if win else -1) / (len(moves) - i)


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


def vectorize_state(state):
    vec = np.zeros(64, dtype=np.uint8)
    player = state[0]
    opponent = state[1]
    for i in range(32):
        vec[i] = player & 1
        vec[32 + i] = opponent & 1
        player >>= 1
        opponent >>= 1
    return vec


def vectorize_move(move):
    A = vectorize_state(move[0])
    B = vectorize_state(move[1])
    return np.concatenate([A, B])


def vectorize_labels(labs):
    X = np.zeros((len(labs), 128), dtype=np.uint8)
    y = np.zeros((len(labs), 1), dtype=np.float32)
    for i, (move, score) in enumerate(labs.items()):
        X[i, :] = vectorize_move(move)
        y[i, :] = sum(score) / len(score)
    return X, y


def vectorial_distance(x1, x2):
    return np.logical_xor(x1, x2).sum()


labels = get_labels(data[:100])
X, y = vectorize_labels(labels)

move = get_player_moves(data[0], 0)[0]

knn = neighbors.KNeighborsRegressor(8, metric=vectorial_distance)
knn_model = knn.fit(X, y)

joblib.dump(knn_model, "knn.save")

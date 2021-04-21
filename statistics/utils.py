import numpy as np


def vectorize_state(state):
    player = state[0]
    opponent = state[1]
    vec = np.zeros(64, dtype=np.uint8)
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


def vectorize_scores(scores):
    X = np.zeros((len(scores), 128), dtype=np.uint8)
    Y = np.zeros((len(scores), 1), dtype=np.float32)
    for i, (move, score) in enumerate(scores.items()):
        X[i, :] = vectorize_move(move)
        Y[i, :] = sum(score) / len(score)
    return X, Y


def vectorial_distance(x1, x2):
    return np.logical_xor(x1, x2).sum()

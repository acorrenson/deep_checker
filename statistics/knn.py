from data import vectorize_move
import numpy as np
import joblib
import sys


def usage():
    print(
        f"{sys.argv[0]} init_p1 init_p2 state1_p1 state1_p2 [state3_p1 state3_p2 ... ]"
    )


if __name__ == "__main__":
    knn_model = joblib.load("knn.save", "r")

    if len(sys.argv) < 5 or (len(sys.argv) - 3) % 2 == 1:
        usage()
        exit(1)

    init_p1 = int(sys.argv[1], 16)
    init_p2 = int(sys.argv[2], 16)

    scores = []

    imax = 0
    for i in range(3, len(sys.argv) - 1, 2):
        state_p1 = int(sys.argv[i], 16)
        state_p2 = int(sys.argv[i + 1], 16)
        move = vectorize_move(((init_p1, init_p2), (state_p1, state_p2)))
        move_vec = np.array([move])
        score = knn_model.predict(move_vec)
        scores.append(score)
        if score > scores[imax]:
            imax = (i - 3) // 2

    print(imax)

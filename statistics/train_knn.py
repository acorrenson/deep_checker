from utils import vectorial_distance
from sklearn import neighbors
from sklearn.model_selection import train_test_split
import joblib
import sys

scores = "scores.save"
if len(sys.argv) > 1:
    scores = sys.argv[1]
out = "knn.save"
if len(sys.argv) > 2:
    out = sys.argv[2]

X, Y = joblib.load(scores, "r")
X_train, X_test, y_train, y_test = train_test_split(X, Y)

knn = neighbors.KNeighborsRegressor(4, metric=vectorial_distance)
knn.fit(X_train, y_train)

print(knn.predict(X_test[:4]))
print(y_test[:4])
print(knn.score(X_test[:100], y_test[:100]))

joblib.dump(knn, out)

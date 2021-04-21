from utils import vectorial_distance
from sklearn import neighbors
import joblib

X, Y = joblib.load("scores.save", "r")

knn = neighbors.KNeighborsRegressor(8, metric=vectorial_distance)
knn_model = knn.fit(X, Y)

joblib.dump(knn_model, "knn.save")

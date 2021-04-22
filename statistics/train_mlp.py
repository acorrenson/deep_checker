from sklearn.neural_network import MLPRegressor
from sklearn.model_selection import train_test_split
import joblib

X, Y = joblib.load("scores.save", "r")
X_train, X_test, y_train, y_test = train_test_split(X, Y.ravel())

mlp = MLPRegressor(
    hidden_layer_sizes=(64, 64, 32, 16), activation="relu", solver="adam"
)
mlp.fit(X_train, y_train)

print(mlp.predict(X_test[:4]))
print(y_test[:4])
print(mlp.score(X_test, y_test))

joblib.dump(mlp, "mlp.save")

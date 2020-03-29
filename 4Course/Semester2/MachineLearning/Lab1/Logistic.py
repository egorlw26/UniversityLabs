import numpy as np
import matplotlib.pyplot as plt
from DatasetFuncs import *
from sklearn.linear_model import LogisticRegression


def sigmoid(scores):
    return 1 / (1 + np.exp(-scores))


def logistic_regression(features, target, num_steps, learning_rate):

    weights = np.zeros(features.shape[1])
    b = 0
    errors = []
    for step in range(num_steps):
        scores = np.dot(features, weights) + b
        predictions = sigmoid(scores)

        output_error_signal = target - predictions

        gradientWeight = np.dot(features.T, output_error_signal) / features.shape[0]
        gradiendB = np.mean(output_error_signal)
        b += gradiendB * learning_rate
        weights += learning_rate * gradientWeight
    return weights, b

def getProbabilities(dataset, newP):
    weights, b = logistic_regression(dataset[:,0:2], dataset[:,2], 10000, 0.1)
    score = np.dot(weights, np.array(newP)) + b
    print("SCORE", score)
    prediction = sigmoid(score)
    return prediction

def predict(dataset, newP):
    prediction = getProbabilities(dataset, newP)
    newP.append(int(round(prediction)))
    dataset = np.append(dataset, [newP], axis=0)
    return dataset

def predictOutOfBox(dataset, newP):
    classifier = LogisticRegression(random_state = 0)
    classifier.fit(dataset[:,0:2], dataset[:,2])
    predictions = classifier.predict_proba([newP])[0]
    label = np.argmax(predictions)
    newP.append(label)
    dataset = np.append(dataset, [newP], axis=0)
    return dataset

if __name__ == '__main__':
    np.random.seed(1)
    num_observations = 50

    x1 = np.random.multivariate_normal([0.5, 0.5], [[1, 0.5], [0.5, 1]], num_observations)
    x2 = np.random.multivariate_normal([5, 5], [[1, 0.5], [0.5, 1]], num_observations)

    print("X1", x1)
    print("X2", x2)

    simulated_separableish_features = np.vstack((x1, x2)).astype(np.float32)
    simulated_labels = np.hstack((np.zeros(num_observations), np.ones(num_observations)))

    print("SSF", simulated_separableish_features)
    print("SL", simulated_labels)

    datasetMyPreds = []
    for i in range(len(simulated_separableish_features)):
        datasetMyPreds.append(np.append(simulated_separableish_features[i], simulated_labels[i]))
    datasetMyPreds = np.array(datasetMyPreds)

    datasetOOBPreds = datasetMyPreds


    datasetMyPreds = predict(datasetMyPreds, list(simulated_separableish_features[0]))
    datasetMyPreds = predict(datasetMyPreds, [0, 0.5])
    datasetMyPreds = predict(datasetMyPreds, [0.5, 0])

    datasetOOBPreds = predictOutOfBox(datasetOOBPreds, [3, 0])


    fig, (ax1, ax2) = plt.subplots(1, 2, sharey=True)
    ax1.set_title("InHouseLogistic")
    ax2.set_title("SkLearnLogistic")
    plotDataset(datasetMyPreds, ax1)
    plotDataset(datasetOOBPreds, ax2)

    plt.show()

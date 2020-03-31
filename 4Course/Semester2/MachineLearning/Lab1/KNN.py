from UsefullFuncs import *
from math import sqrt
from sklearn.neighbors import KNeighborsClassifier
import numpy as np

def calcDistance(firstP, secondP):
    return sqrt((firstP[0] - secondP[0])**2 + (firstP[1] - secondP[1])**2)

def getKNearest(dataset, newP, k):
    list = []
    for row in dataset:
        point = [row[0], row[1]]
        list.append([calcDistance(newP, point), row[-1]])
        list.sort(key=lambda x: x[0])
    return list[:k]

def getProbabilities(dataset, newP):
    nearest = getKNearest(dataset, newP, 10)
    total_rows = len(nearest)
    probs = {}
    for point in nearest:
        probs[int(point[1])] = 0.0
    for point in nearest:
        probs[int(point[1])] += 1
    for key in probs:
        probs[key] /= float(total_rows)

    return probs

def predict(dataset, newP):
    probs = getProbabilities(dataset, newP)
    label = max(probs, key=probs.get)
    newP.append(label)
    dataset = np.append(dataset, [newP], axis=0)
    return dataset

def outOfBoxPredict(dataset, newP):
    classifier = KNeighborsClassifier(n_neighbors = 5)
    classifier.fit(dataset[:,0:2], dataset[:,2])
    predictions = classifier.predict_proba([newP])[0]
    label = np.argmax(predictions)
    newP.append(label)
    dataset = np.append(dataset, [newP], axis=0)
    return dataset

if __name__ == '__main__':
    datasetMyPreds = createDataset()
    datasetOOBPreds = datasetMyPreds

    #datasetMyPreds = predict(datasetMyPreds, [5, 7])
    #datasetOOBPreds = outOfBoxPredict(datasetOOBPreds, [10, 1])

    fig, axs = plt.subplots(2, 2, sharey=True)
    axs[0, 0].set_title("InHouseNaiveBayes")
    axs[0, 1].set_title("SkLearnNaiveBayes")

    heatmap(datasetMyPreds, getProbabilities, axs[0, 0])

    plotDataset(datasetMyPreds, axs[0, 0])
    plotDataset(datasetOOBPreds, axs[0, 1])

    plotROCcurve(datasetMyPreds, 0, getProbabilities, axs[1, 0])
    plotROCcurve(datasetMyPreds, 1, getProbabilities, axs[1, 1])

    plt.show()

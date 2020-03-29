from DatasetFuncs import *
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
    total_rows = len(dataset)
    probs = {}
    for point in nearest:
        probs[int(point[1])] = 0
    for point in nearest:
        probs[int(point[1])] += 1
    return probs

def predict(dataset, newP):
    probs = getProbabilities(dataset, newP)
    label = max(probs, key=probs.get)
    newP.append(label)
    dataset = np.append(dataset, [newP], axis=0)
    print("Label:", label)
    return dataset

def outOfBoxPredict(dataset, newP):
    classifier = KNeighborsClassifier(n_neighbors = 5)
    classifier.fit(dataset[:,0:2], dataset[:,2])
    predictions = classifier.predict_proba([newP])[0]
    label = np.argmax(predictions)
    newP.append(label)
    print("Label:", label)
    dataset = np.append(dataset, [newP], axis=0)
    return dataset

if __name__ == '__main__':
    datasetMyPreds = createDataset()
    datasetOOBPreds = datasetMyPreds

    datasetMyPreds = predict(datasetMyPreds, [5, 7])
    datasetOOBPreds = outOfBoxPredict(datasetOOBPreds, [10, 1])

    fig, (ax1, ax2) = plt.subplots(1, 2, sharey=True)
    ax1.set_title("InHouseKNN")
    ax2.set_title("SkLearnKNN")

    plotDataset(datasetMyPreds, ax1)
    plotDataset(datasetOOBPreds, ax2)

    plt.show()

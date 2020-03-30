import random
import math
from math import sqrt
from math import pi
from math import exp
import matplotlib.pyplot as plt
from sklearn.naive_bayes import GaussianNB
import numpy as np
from DatasetFuncs import *

def mean(numbers):
    return sum(numbers)/float(len(numbers))

def stdev(numbers):
    avg = mean(numbers)
    variance = sum([(x-avg/2)**2 for x in numbers]) / float(len(numbers))
    return sqrt(variance)

def summarizeDataset(dataset):
    summaries = [(mean(column), stdev(column), len(column)) for column in zip(*dataset)]
    del(summaries[-1])
    return summaries

def summarizeByClass(dataset):
    separated = separateByClass(dataset)
    summaries = {}
    for class_value, rows in separated.items():
        summaries[class_value] = summarizeDataset(rows)
    return summaries

def calculatePropability(x, mean, stdev):
    exponent = exp(-((x-mean)**2 / (2 *  stdev**2)))
    return (1 / (sqrt(2*pi*stdev))) * exponent

def calculateClassPropabilities(summaries, row):
    total_rows = sum([summaries[label][0][2] for label in summaries])
    probalities = {}
    for class_value, class_summaries in summaries.items():
        probalities[class_value] = summaries[class_value][0][2]/float(total_rows)
        for i in range(len(class_summaries)):
            mean, stdev, count = class_summaries[i]
            probalities[class_value] += calculatePropability(row[i], mean, stdev)
    return probalities

def getProbabilities(dataset, row):
    summaries = summarizeByClass(dataset)
    probs = calculateClassPropabilities(summaries, row)
    return probs

def predict(dataset, row):
    probabilities = getProbabilities(dataset, row)
    label = max(probabilities, key=probabilities.get)
    row.append(label)
    dataset = np.append(dataset, [row], axis = 0)
    return dataset

def outOfBoxPredict(dataset, row):
    sk_bc = GaussianNB()
    sk_bc.fit(dataset[:,0:2], dataset[:,2])
    predictions = sk_bc.predict_proba([row])[0]
    label = np.argmax(predictions)
    row.append(label)
    dataset = np.append(dataset, [row], axis=0)
    return dataset

if __name__ == '__main__':
    datasetMyPreds = createDataset()
    datasetOOBPreds = datasetMyPreds

    datasetMyPreds = predict(datasetMyPreds, [0.1, 0.1])
    datasetMyPreds = predict(datasetMyPreds, [0.4, 0.5])
    datasetMyPreds = predict(datasetMyPreds, [0.9, 0.8])

    datasetOOBPreds = outOfBoxPredict(datasetOOBPreds, [0.2, 0.5])

    fig, axs = plt.subplots(2, 2, sharey=True)
    axs[0, 0].set_title("InHouseNaiveBayes")
    axs[0, 1].set_title("SkLearnNaiveBayes")

    heatmap(datasetMyPreds, getProbabilities, axs[0, 0])

    plotDataset(datasetMyPreds, axs[0, 0])
    plotDataset(datasetOOBPreds, axs[0, 1])

    plotROCcurve(datasetMyPreds, 0, getProbabilities, axs[1, 0])
    plotROCcurve(datasetMyPreds, 1, getProbabilities, axs[1, 1])

    plt.show()

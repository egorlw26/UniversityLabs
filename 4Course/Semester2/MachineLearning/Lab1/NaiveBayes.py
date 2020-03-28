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
    return (1 / (sqrt(2*pi)*stdev)) * exponent

def calculateClassPropabilities(summaries, row):
    total_rows = sum([summaries[label][0][2] for label in summaries])
    probalities = {}
    for class_value, class_summaries in summaries.items():
        probalities[class_value] = summaries[class_value][0][2]/float(total_rows)
        for i in range(len(class_summaries)):
            mean, stdev, count = class_summaries[i]
            probalities[class_value] *= calculatePropability(row[i], mean, stdev)
    return probalities

def predict(dataset, row):
    summaries = summarizeByClass(dataset)
    probabilities = calculateClassPropabilities(summaries, row)
    label = max(probabilities, key=probabilities.get)
    print("Label:", label)
    row.append(label)
    dataset = np.append(dataset, [row], axis = 0)
    return dataset

def outOfBoxPredict(dataset, row):
    sk_bc = GaussianNB()
    sk_bc.fit(dataset[:,0:2], dataset[:,2])
    predictions = sk_bc.predict_proba([row])[0]
    label = np.argmax(predictions)
    print(label)
    row.append(label)
    dataset = np.append(dataset, [row], axis=0)
    return dataset

if __name__ == '__main__':
    dataset = createDataset()
    dataset = predict(dataset, [1, 1])
    dataset = predict(dataset, [10, 6])

    dataset = outOfBoxPredict(dataset, [10, 15])

    plotDataset(dataset)
    plt.show()

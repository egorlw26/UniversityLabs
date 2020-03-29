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

def getProbabilities(dataset, row):
    summaries = summarizeByClass(dataset)
    probs = calculateClassPropabilities(summaries, row)
    return probs

def predict(dataset, row):
    probabilities = getProbabilities(dataset, row)
    print("PREDICT", probabilities)
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

def getDataForROC(dataset):
    labels = dataset[:,2]
    preds = []
    for row in dataset:
        prob = getProbabilities(dataset, row)
        preds.append(prob[row[-1]])
    return labels, preds


def ROC_curves(dataset):
    dataset_copy = dataset
    labels, preds = getDataForROC(dataset_copy)
    fpr, tpr, _ = roc_curve(labels, preds, pos_label = 2)
    return fpr, tpr

if __name__ == '__main__':

    print(calculatePropability(1.0, 1.0, 1.0))
    print(calculatePropability(2.0, 1.0, 1.0))
    print(calculatePropability(0.0, 1.0, 1.0))

    datasetMyPreds = createDataset()
    datasetOOBPreds = datasetMyPreds

    datasetMyPreds = predict(datasetMyPreds, [0.1, 0.1])
    datasetMyPreds = predict(datasetMyPreds, [0.2, 0.3])

    datasetOOBPreds = outOfBoxPredict(datasetOOBPreds, [0.2, 0.5])

    fig, axs = plt.subplots(2, 2, sharey=True)
    axs[0, 0].set_title("InHouseNaiveBayes")
    axs[0, 1].set_title("SkLearnNaiveBayes")

    xx, yy = np.meshgrid(
      np.arange(0, 1.2, 0.02),
      np.arange(0, 1.2, 0.02))

    dots = np.c_[xx.ravel(), yy.ravel()]

    Z = []
    for dot in dots:
        probs = getProbabilities(datasetMyPreds, dot)
        label = max(probs, key=probs.get)
        Z.append(label)
    Z = np.array(Z).reshape(xx.shape)
    axs[0, 0].contourf(xx, yy, Z, cmap=plt.cm.RdBu, alpha=.75)

    plotDataset(datasetMyPreds, axs[0, 0])
    plotDataset(datasetOOBPreds, axs[0, 1])

    lw = 2
    fpr, tpr = ROC_curves(datasetMyPreds)
    roc_auc = auc(fpr, tpr)
    axs[1, 0].plot(fpr, tpr, color='darkorange',
         lw=lw, label='ROC curve (area = %0.2f)' % roc_auc)
    axs[1, 0].plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')



    plt.show()

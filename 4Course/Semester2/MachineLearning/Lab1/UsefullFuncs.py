import numpy as np
import random
import math
from math import sqrt
from math import pi
from math import exp
import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, auc

def createRowsForDataset(dataset, xrange, yrange, label):
    for i in range(40):
        x = np.random.rand() * (xrange[1] - xrange[0]) + xrange[0]
        y = np.random.rand() * (yrange[1] - yrange[0]) + yrange[0]
        dataset.append([x, y, label])

def createDataset():
    dataset = []
    createRowsForDataset(dataset, [0.0, 0.5], [0.1, 1.0], 0)
    createRowsForDataset(dataset, [0.6, 1.0], [0.1, 1.0], 1)
    return np.array(dataset)

def separateByClass(dataset):
    separated = {}
    for i in range(len(dataset)):
        vector = dataset[i]
        class_value = vector[-1]
        if(class_value not in separated):
            separated[class_value] = list()
        separated[class_value].append(vector)
    return separated

def plotDataset(dataset, axs):
    separated = separateByClass(dataset)
    for i in range(len(separated)):
        axs.scatter(
        [x[0] for x in separated[i]],
        [x[1] for x in separated[i]])

def getDataForROC(dataset, label, getProbabsFunc):
    labels = []
    for row in dataset:
        labels.append(row[-1] == label)
    preds = []
    for row in dataset:
        prob = getProbabsFunc(dataset, row)
        if row[-1] == label:
            preds.append(prob[row[-1]])
        else:
            preds.append(1.0 - prob[row[-1]])

    return labels, preds

def ROCcurves(dataset, label, getProbabsFunc):
    dataset_copy = dataset
    labels, preds = getDataForROC(dataset_copy, label, getProbabsFunc)
    fpr, tpr, _ = roc_curve(labels, preds)
    return fpr, tpr

def plotROCcurve(dataset, label, getProbabsFunc, subplot):
        lw = 2
        fpr, tpr = ROCcurves(dataset, label, getProbabsFunc)
        roc_auc = auc(fpr, tpr)
        subplot.set_title("Roc curve for "+str(label)+" class")
        subplot.plot(fpr, tpr, color='darkorange',
             lw=lw, label='ROC curve (area = %0.2f)' % roc_auc)
        subplot.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')

def heatmap(dataset, getProbabsFunc, subplot):
    xx, yy = np.meshgrid(
      np.arange(0, 1.2, 0.02),
      np.arange(0, 1.2, 0.02))

    dots = np.c_[xx.ravel(), yy.ravel()]

    Z = []
    for dot in dots:
        probs = getProbabsFunc(dataset, dot)
        label = max(probs, key=probs.get)
        Z.append(label)
    Z = np.array(Z).reshape(xx.shape)
    subplot.contourf(xx, yy, Z, cmap=plt.cm.RdBu, alpha=.75)

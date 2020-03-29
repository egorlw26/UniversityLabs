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

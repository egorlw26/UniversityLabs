import numpy as np
import random
import math
from math import sqrt
from math import pi
from math import exp
import matplotlib.pyplot as plt

def createRowsForDataset(dataset, xrange, yrange, label):
    for i in range(10):
        x = np.random.rand() * (xrange[1] - xrange[0]) + xrange[0]
        y = np.random.rand() * (yrange[1] - yrange[0]) + yrange[0]
        dataset.append([x, y, label])

def createDataset():
    dataset = []
    dataset.append([3.39, 2.3, 0])
    dataset.append([3.11, 1.78, 0])
    dataset.append([1.34, 3.36, 0])
    dataset.append([3.58, 4.67, 0])
    dataset.append([2.28, 2.86, 0])
    dataset.append([7.42, 4.69, 1])
    dataset.append([5.74, 3.53, 1])
    dataset.append([9.17, 2.51, 1])
    dataset.append([7.79, 3.42, 1])
    dataset.append([7.93, 0.79, 1])
    createRowsForDataset(dataset, [15, 20], [0, 5], 2)
    createRowsForDataset(dataset, [10, 15], [2, 5], 3)
    createRowsForDataset(dataset, [0, 20], [5, 10], 4)
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

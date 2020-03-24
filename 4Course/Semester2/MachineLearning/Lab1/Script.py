import csv
import random
import math
import matplotlib.pyplot as plt
import numpy

def loadCsvFile():
    lines = csv.reader(open(r'.\GBP_USD Historical Data.csv'))
    dataset = list(lines)
    for i in range(1, len(dataset)):
        for j in range(1, len(dataset[i])-1):
            dataset[i][j] = float(dataset[i][j])
        dataset[i][-1] = float(dataset[i][-1][:-1])
    return dataset

if __name__ == '__main__':
    dataset = loadCsvFile()
    days = [x[0] for x in dataset[1:]]
    prices = [x[1] for x in dataset[1:]]
    days.reverse()
    prices.reverse()

    fig, ax = plt.subplots()
    ax.plot_date(days, prices, marker='', linestyle='-')
    fig.autofmt_xdate()
    plt.show()

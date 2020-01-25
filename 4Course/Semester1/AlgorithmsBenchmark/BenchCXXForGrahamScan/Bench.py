import matplotlib.pyplot as plt
import numpy as np

with open("outputs.txt") as f:
    lineList = f.readlines()

x = []
y = []

for line in lineList:
    arr = line.split(' ')
    x.append(int(arr[0]))
    y.append(float(arr[1]))

constLog = x[-1]*np.log(x[-1])/y[-1]

x1 = np.arange(x[0], x[-1], x[1]-x[0])
y1 = x1*np.log(x1)/constLog

constS = x[-1]*x[-1]/y[-1]

y2 = x1*x1/constS

plt.plot(x, y, 'b')
plt.plot(x1, y2, 'r')
plt.savefig("outputs5000.png")
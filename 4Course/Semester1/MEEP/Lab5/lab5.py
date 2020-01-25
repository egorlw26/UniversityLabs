import numpy as np
import math
import matplotlib.pyplot as plt

A11 = np.array([[0.4, 0.2], [0.1, 0.2]])
A12 = np.array([[0.3, 0.2]]).T
A21 = np.array([[0.2, 0.3]])
A22 = 0.5
B1 = np.array([[0.2, 0.3], [0.7, 0.8]])
B2 = np.array([[0.1, 0]]).T
c1 = lambda t: np.array([[1000, 400]]).T * np.exp(0.05*t)
c2 = 200

x0 = np.array([[5000, 2400]]).T

EmA22inv = np.linalg.inv(np.identity(1) - A22)
A1 = A11 + A12*EmA22inv*A21
B = B1 + B2*EmA22inv*A21

# print("A1 = ",A1)
# print("B = ",B)

c = lambda t: c1(t) - A12*(EmA22inv)*c2 - B2*(EmA22inv)*c2

dx1dtwC = lambda x,t: np.linalg.inv(B).dot((np.identity(2) - A1).dot(x) - c(t))
dx1dtwoC = lambda x,t: np.linalg.inv(B).dot((np.identity(2) - A1).dot(x))
dx1dt2 = lambda x,t : (np.identity(2)-A1).dot(np.linalg.inv(B).dot(x))

dt = 0.01

t = np.arange(0, 1, dt)
x1wC = [x0]
x1woC = [x0]
x2 = [x0]

for i in t[1:]:
    x1wC.append(x1wC[-1] + dx1dtwC(x1wC[-1],i)*dt)
    x1woC.append(x1woC[-1] + dx1dtwoC(x1woC[-1],i)*dt)
    x2.append(x2[-1] + dx1dt2(x2[-1], i)*dt)

x1wC = np.array(x1wC)
x1woC = np.array(x1woC)
x2 = np.array(x2)
fig, axs = plt.subplots(2, 2)
axs[0, 0].set_title("With c(t)")
axs[0, 0].plot(t, x1wC[:,0])
axs[0, 0].plot(t, x1wC[:,1])
axs[0, 1].set_title("With/without c(t)")
axs[0, 1].plot(t, x1wC[:,0])
axs[0, 1].plot(t, x1woC[:,0])
axs[0, 1].plot(t, x1wC[:,1])
axs[0, 1].plot(t, x1woC[:,1])
axs[1, 0].set_title("Some stuff")
axs[1, 0].plot(t, x2[:,0])
axs[1, 0].plot(t, x2[:,1])
plt.show()
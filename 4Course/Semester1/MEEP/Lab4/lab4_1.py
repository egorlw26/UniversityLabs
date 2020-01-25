import numpy as np
import matplotlib.pyplot as plt

beta = 626.2
delta = 600
ro = 0.2

dt = 0.001

def Ni_pl_1(Ni):
    return dt*(26*Ni**2 - 600*Ni - 0.2*Ni**3)/(1+Ni) + Ni


Ns = np.arange(0, 200, 1)
t = np.arange(0, 1, dt)

for n in Ns:
    N = [n]
    for i in t[1:]:
        N.append(Ni_pl_1(N[-1]))
    plt.plot(t, N)
plt.show()    
import numpy as np
import matplotlib.pyplot as plt
import math
import random

a1 = 10
b12 = 0.5
c1 = 1

a2 = 8
b21 = 0.5
c2 = 1

dt = 0.001

def N1i_pl_1(N1i, N2i):
    return dt*(a1*N1i - b12*N1i*N2i - c1*N1i**2) + N1i
    
def N2i_pl_1(N2i, N1i):
    return dt*(a2*N2i - b21*N1i*N2i - c2*N2i**2) + N2i
    
t = np.arange(0, 10, dt)

for i in range(0, 500):
    N2 = [random.random()*10]
    N1 = [random.random()*10]
    
    for i in t[1:]:
        N1.append(N1i_pl_1(N1[-1], N2[-1]))
        N2.append(N2i_pl_1(N2[-1], N1[-2]))
    plt.plot(N1, N2)

plt.plot(8, 4, 'o')
plt.plot(0, 0, 'o')
plt.plot(0, 10, 'o')
plt.plot(10, 0, 'o')
plt.show()
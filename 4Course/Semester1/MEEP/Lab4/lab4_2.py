import numpy as np
import matplotlib.pyplot as plt
import math

def y1(x): # y(0) = 25 
    return 100/(3*math.e**(x/100) + 1)
    
def y2(x): #y(0) = 150
    return 100/((-1/3)*math.e**(x/100) + 1)
    
x = np.arange(0, 50, 0.1)

plt.plot(x, y1(x))
plt.plot(x, y2(x))
plt.show()
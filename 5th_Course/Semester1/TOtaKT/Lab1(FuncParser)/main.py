import math
from funcParser import FuncParser
import matplotlib.pyplot as plt
import numpy as np

funcExample1 = "2*sin(1/(exp(3*x)+1)-tg(x+PI/2)"
funcExample2 = "tg(sin(x*x/0.25)*cos(-x + PI)) + sin(x + PI/2)"
funcExample3 = "(2+2"

if __name__ == "__main__":
    Xs = np.arange(-2, 2, 0.01)
    Ys = [FuncParser(funcExample1, {'x': x}).parseExpression() for x in Xs]

    plt.plot(Xs, Ys)
    plt.show()
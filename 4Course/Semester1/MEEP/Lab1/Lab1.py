import matplotlib.pyplot as plt
import numpy as np

price = [1.25, 1.5, 1.8, 2.25, 2.45, 2.85,
3.3, 3.75, 4.1, 4.75, 5, 5.5]

demand = [106.2, 81.4, 65.3, 61.5, 47.9, 43.7, 
35.1, 34.6, 35, 25.2, 12.1, 10]

supply = [7.1, 21, 30.4, 40.1, 48.6, 55.8, 
65, 82.3, 90.5, 101.3, 112.5, 120]


def zeroMatrix(n):
    matrix = [[0]*n for i in range(n)]
    return matrix

def solveMatrixEquation(left_matrix, right):
    n = len(right)
    for i in range(n):
        d = left_matrix[i][i]
        for j in range(n):
            left_matrix[i][j] /= d
        right[i] /= d
        
        for j in range(n):
            if(j == i): continue
            coef = left_matrix[j][i]
            for k in range(i, n):
                left_matrix[j][k] -= left_matrix[i][k]*coef
            right[j] -= right[i]*coef
    return right


def approximator(x, y, power):
    x_matrix = zeroMatrix(power+1)
    for i in range(power+1):
        for j in range(power+1):
            price_sum = 0
            for k in range(len(x)):
                price_sum += x[k]**(i+j)
            x_matrix[i][j] = price_sum
    
    y_matrix = [0]*(power+1)
    for i in range(power+1):
        y_sum = 0
        for j in range(len(x)):
            y_sum += (x[j]**i)*y[j]
        y_matrix[i] = y_sum
        
    return solveMatrixEquation(x_matrix, y_matrix)
    
def getNewY(polynom, x):
    result = 0
    for i in range(len(polynom)):
        result += (x**i)*polynom[i]
    return result
    
def printFunc(coefs):
    result = ""
    for i in range(len(coefs)):
        result += str(coefs[i]) + " * x^"+str(i) + "\n"
    return result
            
def intersectionPoint(x, y1, y2):
    for i in range(len(x)):
        if(abs(y1[i] - y2[i]) <= 0.3):
            return [x[i], y1[i]]
    return [1, 2]
    
def inPointElasticy(coefs, x,y):
    d = 0
    for i in range(len(coefs)):
        d += coefs[i]*i*x
  
    return d*x/y

def elasticity(x_range, y_range):
    results = [0]*(len(x_range)-1)
    for i in range(len(x_range)-1):
        results[i] = (y_range[i+1] - y_range[i])*(x_range[i] + x_range[i+1])/((x_range[i+1] - x_range[i])*(y_range[i]) + y_range[i+1])
    return results
    
def quota(number, demand, supply):
    n_demand = [x for x in demand]
    n_supply = [y for y in supply]
    for i in range(len(demand)):
        if n_demand[i] > number:
            n_demand[i] = number
        if n_supply[i] > number:
            n_supply[i] = number
    return [n_demand, n_supply]
    
def lnFuncCoef(i_x, i_y):
    x = [a for a in i_x]
    y = [b for b in i_y]
    y_sum = 0
    for i in y:
        y_sum += i
    ln_sum = 0
    for i in x:
        ln_sum += np.log(i)
    y_log_sum = 0
    for i in range(len(x)):
        y_log_sum += y[i]*np.log(x[i])
    log_s_sum = 0
    for i in x:
        log_s_sum += np.log(i)**2
    
    # a = (y_sum - y_log_sum/2)/(len(x) - ln_sum/2)
    a = (y_sum - (y_log_sum * ln_sum)/log_s_sum)/(len(x) - (ln_sum**2)/log_s_sum)
    b = (y_log_sum - a*ln_sum)/log_s_sum
    return [a, b]
    
    
def getLnY(x, coefs):
    return coefs[0] + coefs[1]*np.log(x)
    
 
coefs_demand = approximator(price, demand, 2)
coefs_supply = approximator(price, supply, 2)
ln_coefs_demand = lnFuncCoef(price, demand)
ln_coefs_supply = lnFuncCoef(price, supply)

x_range = np.arange(min(price), max(price), 0.01)
y_range_demand = getNewY(coefs_demand, x_range)
y_range_supply = getNewY(coefs_supply, x_range)
y_ln_range_demand = getLnY(x_range, ln_coefs_demand)
y_ln_range_supply = getLnY(x_range, ln_coefs_supply)

demand_supply = quota(35, y_range_demand, y_range_supply)
demand_supply1 = quota(70, y_range_demand, y_range_supply)

inters = intersectionPoint(x_range, y_range_demand, y_range_supply )
inters_ln = intersectionPoint(x_range, y_ln_range_demand, y_ln_range_supply)
inters_quota_35 = intersectionPoint(x_range, y_range_demand, demand_supply[1])
inters_quota_70 = intersectionPoint(x_range, y_range_demand, demand_supply1[1])


# print(printFunc(coefs_demand))
print(inPointElasticy(coefs_demand, inters[0], inters[1]))
print(inPointElasticy(coefs_supply, inters[0], inters[1]))


print(elasticity(price, demand))
print(elasticity(price, supply))
plt.subplot(1, 2, 1)
plt.title('Polynominal')
plt.xlabel('Quantity')
plt.ylabel('Price')
plt.plot(demand, price, 'r-.')
plt.plot(supply, price, 'g-.')
plt.plot(y_range_demand, x_range, 'r')
plt.plot(y_range_supply, x_range, 'g')

plt.plot(demand_supply1[1], x_range, 'orange')
plt.plot(demand_supply[1], x_range, 'yellow')

plt.plot(inters[1], inters[0], 'bo')
plt.plot(inters_quota_35[1], inters_quota_35[0], 'bo')
plt.plot(inters_quota_70[1], inters_quota_70[0], 'bo')


plt.subplot(1, 2, 2)
plt.title('Logarithm')
plt.xlabel('Quantity')
plt.plot(demand, price, 'r-.')
plt.plot(supply, price, 'g-.')

plt.plot(y_ln_range_demand, x_range, 'r')
plt.plot(y_ln_range_supply, x_range, 'g')

plt.plot(inters_ln[1], inters_ln[0], 'bo')

plt.savefig("output.png")
plt.show()
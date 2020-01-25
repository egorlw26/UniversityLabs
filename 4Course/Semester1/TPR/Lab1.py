import numpy as np

c = np.array([[3, 1], [1, 2]])

sum_limit = 5

un_limit_low = 0
un_limit_upper = 4

a = []
a.append(max(c[0])*max(un_limit_low, un_limit_upper, 0) 
       + min(c[0])*(sum_limit - max(un_limit_low, un_limit_upper, 0)))
a.append(max(c[1])*max(un_limit_low, un_limit_upper, 0) 
       + min(c[1])*(sum_limit - max(un_limit_low, un_limit_upper, 0)))

print("Found a vector: ", a)

print("Analitic solve: ", list(np.linalg.solve(c, np.array(a))))

L_coefs = np.array([
    [2*(c[0][0]**2+c[1][0]**2), 2*((c[0][1]*c[0][0])+(c[1][1]*c[1][0])), 1],
    [2*((c[0][1]*c[0][0])+(c[1][1]*c[1][0])), 2*(c[0][1]**2 + c[1][1]**2), 1],
    [1, 1, 0]])

L_values = np.array([
    2*(c[0][0]*a[0] + c[1][0]*a[1]), 
    2*(c[0][1]*a[0] + c[1][1]*a[1]), 
    sum_limit ])
    
print("Kuhn — Tucker solve: ",list(np.linalg.solve(L_coefs, L_values))[:2])
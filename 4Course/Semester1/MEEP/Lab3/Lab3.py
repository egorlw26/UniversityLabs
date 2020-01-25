import numpy as np

x11 = 900
x12 = 550
y1 = 1000
x21 = 550
x22 = 700
y2 = 850

x1 = x11 + x12 + y1
x2 = x21 + x22 + y2

a11 = x11*1.0/x1
a12 = x12*1.0/x1

a21 = x21*1.0/x2
a22 = x22*1.0/x2

A = np.array([[a11, a12], [a21, a22]])

s = np.array([0.4, 0.5])

p = s.dot(np.linalg.inv(np.identity(2) - A))

print("p =", p)

A2 = np.array([[0.3, 0.3, 0.2], [0.2, 0.15, 0.1], [0.15, 0.4, 0.25]])
eig = np.linalg.eig(A2)[0]

print("Eig values =", eig)

x = np.array([1, 2, 3, 4])

y = [np.linalg.det(A2 - x*np.identity(3)) for x in x]

f = np.polyfit(x, y, 3)

print("Polynom coefs =", f)

frobValue = np.max(abs(eig))
print("Frobenius value =", frobValue)

v = [1, 0.575, 0.93]
v2 = np.array([1, 1.20604, 0.784187])
print("xA =", v)
print("pA =", v2)

print("B =", np.linalg.inv(np.identity(3) - A2))



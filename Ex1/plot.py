import matplotlib.pyplot as plt
import numpy as np

import csv

def get_data(L):
    with file('ising'+str(L)+'.dat') as f:
        rdr = csv.reader(f, delimiter = ' ');
        data = np.array([[float(i) for i in row if i!=''] for row in rdr if row!=[]]) 
    return data

def plot_data(data, idx):
    i = 0
    for datai in data:
        plt.plot(datai[:,0], datai[:,idx], label = "L = "+str(ls[i]))
        i += 1

ls = [10, 15, 20, 25]

data = [get_data(i) for i in ls]


Tc = 4.51

## Energy plot
E = plt.figure()
plt.xlabel(r'$T/J k_B$')
plt.ylabel(r'$<E>/J$')

plot_data(data, 1)
plt.axvline(Tc, color='black')
plt.legend(loc='upper left')

plt.savefig('energy.pdf')

## Magnetization plot
M = plt.figure()
plt.xlabel(r'$T/J k_B$')
plt.ylabel(r'$<M>$')

plot_data(data, 2)
plt.axvline(Tc, color='black')
plt.legend()

plt.savefig('magnetization.pdf')

## Xi plot
X = plt.figure()
plt.xlabel(r'$T/J k_B$')
plt.ylabel(r'$\Xi(T)$')

plot_data(data, 3)
plt.axvline(Tc, color='black')
plt.legend()

plt.savefig('xi.pdf')

## Cv plot
C = plt.figure()
plt.xlabel(r'$T/J k_B$')
plt.ylabel(r'$C_V(T)$')

plot_data(data, 4)
plt.axvline(Tc, color='black')
plt.legend()

plt.savefig('cv.pdf')

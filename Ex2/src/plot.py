#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np

import csv
import subprocess as sp
import threading as trd

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

class ising_thread(trd.Thread):
    def __init__(self, L):
        super(ising_thread, self).__init__()
        self.L = L

    def run(self):
        sp.call(['../build/src/ex2', repr(self.L)])


if __name__ == '__main__':

    ls = [10, 15, 20, 25, 30, 35, 40, 45, 50]

    #subprocs = [sp.call(['../build/src/ex2', repr(i)]) for i in ls]
    threads = [ising_thread(i) for i in ls]
    [thread.start() for thread in threads]
    [thread.join() for thread in threads]

    data = [get_data(i) for i in ls]

    Tc = 4.51

## Finite Size Scaling
    xi = [np.max([i for i in datai[:,3] if not np.isnan(i)]) for datai in data]
#idx = data[0][:, 0].searchsorted(4.71)
#xi = [datai[idx, 3] for datai in data]
    SE = plt.figure()
    plt.xlabel(r'$L$')
    plt.ylabel(r'$\chi(T)$')

    plt.plot(ls, xi)
    plt.savefig('scaling_estimate.pdf')

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
    plt.ylabel(r'$\chi(T)$')

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

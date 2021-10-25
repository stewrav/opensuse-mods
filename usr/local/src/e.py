#!/usr/bin/python
# encoding: utf-8

# Euler's constant, e = sum(n=0 --> infinity) of 1/n!
# i.e. 1/1 + 1/1 + 1/2 + 1/6 + 1/24 + ...

from math import factorial

n = int(2)
e = float(2)

while True:
    rf = 1 / float( factorial(n) )
    e += rf
    print( n, rf, e )
    n += 1
    if( n > 170 ):
        break

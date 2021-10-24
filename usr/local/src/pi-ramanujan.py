#!/usr/bin/python
# encoding: utf-8

# Ramanujan's formula for pi:

# 1/π =  2√2   ∞ (4k)!(1103 + 26390k)
#       ----   Σ --------------------
#       9801  k=0   (k!)^4 . 396^4k

from math import factorial, sqrt

total = float( 0 )
k     = int( 0 )
term1 = float( (2*sqrt(2)) / 9801 )

while True:
    term2_numerator   = float( factorial(4*k) * (1103 + (26390*k)) )
    term2_denominator = float( (factorial(k)**4) * (396**(4*k)) )
    term2 = term2_numerator / term2_denominator
    total = total + term2
    print("k=" + str(k) + ", pi = " + str(1/(term1 * total)))

    # Get variables ready for next iteration
    k += 1
    if(k>21):
        # long int (from the factorial(k)**4) will
        # get too large to convert to float
        break

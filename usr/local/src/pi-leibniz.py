#!/usr/bin/python
# encoding: utf-8

# Calculate pi using the Leibniz series:
# π/4 = 1 - ⅓ + ⅕ - ⅐ + ⅑ ...

# number of iterations (just to make the output more interesting)
count = 1

# denominator
denom = 1

# sign of term (1 or -1)
sign = 1

# quarter pi total
qpt = 0

# Loop forever
while True:
    # Calculate the quarter pi total
    qpt = qpt + (1/float(denom))

    # Show four times the quarter pi total, i.e. pi
    print(count, 4*qpt)
    
    # Set the variables ready for the next iteration
    sign = -1 * sign
    denom = sign * ( abs(denom) + 2 )
    count += 1

    # Stop once we get accurate enough but not as
    # accurate as 3.1415926535897932384626433
    if( 4*qpt > 3.14159264 and 4*qpt < 3.14159266 ):
        # should be 73,584,742 iterations
        break

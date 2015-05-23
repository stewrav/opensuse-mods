#!/bin/sh
#
#  Program: yesterNdays
#  Authors: Randal L. Schwartz, and
#         : Lawson Hanson, 19961010
#  Purpose: Prints yester N day's date
#    Usage: yesterNdays #
#     e.g.: yesterNdays 5
#  Returns: Sat Oct  5 13:50:23 1996
#
if [ $# -ne 1 ]; then
    echo "Usage: yesterNdays #"
    echo " e.g.: yesterNdays 5"
    exit 1
fi
#
perl -e "print scalar localtime (time - $1 * 86400)"
echo ""

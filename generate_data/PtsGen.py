import random
import numpy as np
import pandas as pd

random.seed(42)

RANGE_OF_MAGNITUDE = 1000



def gen2Dpts():
    n = int(input("number of 2D pts to generate? :"))

    n= 2*n

    mylist=[]

    for i in range (n):
        mag = random.randint(0,RANGE_OF_MAGNITUDE)
        sign = random.randint(0,2)
        pt = mag* (random.random())

        if(sign):
            pt = -1 * pt

        mylist.append(pt)

    return mylist


def gen3Dpts():
    n = int(input("number of 3D pts to generate? :"))

    n= 3*n

    mylist=[]

    for i in range (n):
        mag = random.randint(0,RANGE_OF_MAGNITUDE)
        sign = random.randint(0,2)
        pt = mag* (random.random())

        if(sign):
            pt = -1 * pt

        mylist.append(pt)

    return mylist


twoDPts = gen2Dpts()
twoDPts = np.array(twoDPts,np.float64)
twoDPts_row = twoDPts.reshape(1, -1)
np.savetxt('2d_gen_pts.txt', twoDPts_row, delimiter=',', fmt="%.6f")

threeDPts = gen3Dpts()
threeDPts = np.array(threeDPts,np.float64)
threeDPts_row = threeDPts.reshape(1, -1)
np.savetxt('3d_gen_pts.txt', threeDPts_row, delimiter=',', fmt="%.6f")

print("Points generation completed.")


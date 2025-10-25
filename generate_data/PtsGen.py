import random
import numpy as np
import pandas as pd

random.seed(42)

RANGE_OF_MAGNITUDE = 1e100



def gen2Dpts():
    n = int(input("number of 2D pts to generate? :"))
    n = 2 * n  

    mags = np.random.uniform(-RANGE_OF_MAGNITUDE, RANGE_OF_MAGNITUDE + 1, size=n)
    pts = mags * np.random.random(size=n)

    signs = np.random.randint(0, 2, size=n)
    pts[signs == 1] *= -1

    return pts


def gen3Dpts():
    n = int(input("number of 3D pts to generate? :"))

    n= 3*n

    mags = np.random.uniform(-RANGE_OF_MAGNITUDE, RANGE_OF_MAGNITUDE + 1, size=n)
    pts = mags * np.random.random(size=n)

    signs = np.random.randint(0, 2, size=n)
    pts[signs == 1] *= -1

    return pts


twoDPts = gen2Dpts()
twoDPts = np.array(twoDPts,np.float64)
twoDPts_row = twoDPts.reshape(1, -1)
np.savetxt('2d_gen_pts.txt', twoDPts_row, delimiter=',', fmt="%.6f")

threeDPts = gen3Dpts()
threeDPts = np.array(threeDPts,np.float64)
threeDPts_row = threeDPts.reshape(1, -1)
np.savetxt('3d_gen_pts.txt', threeDPts_row, delimiter=',', fmt="%.6f")

print("Points generation completed.")


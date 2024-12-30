import sys
import math


EPSILON = 0.001
MAX_ITERS_NUM = 1000
DEFAULT_ITERS_NUM = 200



argc = len(sys.argv)
if argc == 4:
    try:
        iter = int(sys.argv[2])
    except:
        print("Invalid maximum iteration!")
        exit()
    pointsFile = sys.argv[3]
elif argc == 3:
    iter = DEFAULT_ITERS_NUM
    pointsFile = sys.argv[2]
else:
    print("An Error Has Occurred")
    exit()


try:
    k = int(sys.argv[1])
except:
    print("Invalid number of clusters!")
    exit()



if iter <= 1 or iter >= MAX_ITERS_NUM:
    print("Invalid maximum iteration!")
    exit()


pointsArr = []
with open(pointsFile, "r") as file:
    ctr = 1
    for line in file:
        pointsArr.append([float(val) for val in line.strip().split(",")])

numOfPoints = len(pointsArr)
dimension = len(pointsArr[0])

if(k <= 1 or k >= numOfPoints):
    print("Invalid number of clusters!")
    exit()


centroidsArr = []
for i in range(k):
    centroidsArr.append(pointsArr[i])

def dist(p1, p2):
    dist = 0.0
    for i in range(dimension):
        dist += math.pow(p1[i] - p2[i], 2)
    return math.sqrt(dist)


for it in range(iter):
    newCentroids = [[0.0] * dimension for _ in range(k)]
    
    countArr =[0]
    for i in range(k-1):
        countArr.append(0)
    for point in pointsArr:
        closestCentIndex = 0
        #calculate the distance of the point from all of the centroids and find the closest one
        minDist = dist(centroidsArr[0], point)
        for index in range(len(centroidsArr)):
            currDist = dist(centroidsArr[index], point)
            #found a closer centroid
            if currDist < minDist:
                #update centroid
                closestCentIndex = index
                minDist = currDist
        countArr[closestCentIndex] += 1
        for d in range(dimension):
            newCentroids[closestCentIndex][d] += point[d]

    for cl in range(k):
        for dim in range(dimension):
            newCentroids[cl][dim] /= countArr[cl]

    
    smallerThanEps = True
    for c in range(len(centroidsArr)):
        if(abs(dist(newCentroids[c],centroidsArr[c])) >= EPSILON):
            smallerThanEps = False
    if(smallerThanEps):
        break
    

    centroidsArr = newCentroids
    


for c in range(k):
    for d in range(dimension - 1):
        formatted = "%.4f"%centroidsArr[c][d]
        print(f"{formatted},", end="")
    formatted = "%.4f"%centroidsArr[c][dimension - 1]
    print(formatted)

  

        
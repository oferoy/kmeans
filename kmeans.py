from decimal import Decimal
import sys
import math

k = int(sys.argv[1])
iter = int(sys.argv[2])
pointsFile = sys.argv[3]
EPSILON = 0.001

pointsRead = []
with open(pointsFile, "r") as file:
    ctr = 1
    for line in file:
        pointsRead.append(line.strip().split(","))



numOfPoints = len(pointsRead)
dimension = len(pointsRead[0])

pointsArr = pointsRead
for i in range(numOfPoints):            
    for j in range(dimension):
        pointsArr[i][j] = float(pointsRead[i][j])



centroidsArr = []
for i in range(k):
    centroidsArr.append(pointsArr[i])


sumArr = []
for i in range(k):
    sumArr.append([])
    for j in range(dimension):
        sumArr[i].append(0)

def resetSum():
    for i in range(k):
         for j in range(dimension):
             sumArr[i][j] = 0


countArr =[]
for i in range(k):
    countArr.append(0)

def resetCount():
    for i in range(k):
        countArr[i] = 0

def copyCentroids(original, copy):
    for i in range(len(original)):
        copy.append([])
        for j in range(len(original[0])):
            copy[i].append(original[i][j])
           
def roundCen():
    for i in range(len(centroidsArr)):
        for j in range(len(centroidsArr[0])):
            centroidsArr[i][j] = round(centroidsArr[i][j], 4)

def distFromCent(centroid, point):
    dist = 0
    for i in range(dimension):
        dist = dist + math.pow(point[i] - centroid[i], 2)
    return math.sqrt(dist)



for i in range(2):
    resetCount()
    resetSum()
    for point in pointsArr:
        closestCentIndex = 0
        #calculate the distance of the point from all of the centroids and find the closest one
        minDistance = distFromCent(centroidsArr[0], point)
        for index in range(len(centroidsArr)):
            currDist = distFromCent(centroidsArr[index], point)
            #found a closer centroid
            if(currDist < minDistance):
                #update centroid
                closestCentIndex = index
                minDistance = currDist
        #update counter and sum of the closest cluster
        
        countArr[closestCentIndex] += 1
        for j in range(dimension):
            sumArr[closestCentIndex][j] += point[j]

    lastCentroidsArr = []
    copyCentroids(centroidsArr, lastCentroidsArr)
    
    #calculate the mean of each cluster
    for cluster in range(k):
        for d in range(dimension):
            centroidsArr[cluster][d] = sumArr[cluster][d] / countArr[cluster]
    
    # smallerThanEps = True
    # for c in range(len(centroidsArr)):
    #     if(abs(distFromCent(lastCentroidsArr[c],centroidsArr[c])) >= EPSILON):
    #         smallererThanEps = False
    # if(smallerThanEps):
    #     break  
roundCen()
print(centroidsArr)







           
            


        
        
        

           
                  
        




    

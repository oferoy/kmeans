from decimal import Decimal
import sys
import math


# Parameters
k = int(sys.argv[1])  # Number of clusters
iter = int(sys.argv[2])  # Maximum number of iterations
pointsFile = sys.argv[3]  # File with points
EPSILON = 0.001  # Convergence threshold

# Read points from file and convert them into Decimal for better precision
pointsRead = []
with open(pointsFile, "r") as file:
    for line in file:
        pointsRead.append([Decimal(val) for val in line.strip().split(",")])

numOfPoints = len(pointsRead)
dimension = len(pointsRead[0])

# Function to compute Euclidean distance between two points (using Decimal for precision)
def distFromCent(centroid, point):
    dist = Decimal(0)
    for i in range(dimension):
        dist += (point[i] - centroid[i]) ** 2
    return dist.sqrt()

# Function to update the centroids based on the mean of assigned points
def update_centroids(k, points, assignments):
    new_centroids = [[Decimal(0)] * dimension for _ in range(k)]
    count = [0] * k
    for i, point in enumerate(points):
        cluster = assignments[i]
        count[cluster] += 1
        for j in range(dimension):
            new_centroids[cluster][j] += point[j]
    
    # Calculate the new centroids by averaging the points in each cluster
    for i in range(k):
        if count[i] > 0:
            for j in range(dimension):
                new_centroids[i][j] /= Decimal(count[i])
    
    return new_centroids

# Main K-means algorithm
def k_means(k, points, max_iters=1000, epsilon=0.001):
    # Initialize centroids (the first k points in the dataset)
    centroids = [points[i] for i in range(k)]
    
    for iteration in range(2):
        # Step 1: Assign each point to the closest centroid
        assignments = []
        for point in points:
            min_dist = distFromCent(centroids[0], point)
            closest_centroid = 0
            for i in range(1, k):
                dist = distFromCent(centroids[i], point)
                if dist < min_dist:
                    min_dist = dist
                    closest_centroid = i
            assignments.append(closest_centroid)
        
        # Step 2: Update centroids based on assignments
        new_centroids = update_centroids(k, points, assignments)
        
        # Step 3: Check for convergence (if centroids do not change significantly)
        converged = True
        for i in range(k):
            for j in range(dimension):
                if abs(new_centroids[i][j] - centroids[i][j]) >= epsilon:
                    converged = False
                    break
            if not converged:
                break
        
        # If the centroids have converged, break the loop
        if converged:
            print(f"Converged after {iteration + 1} iterations.")
            break
        
        centroids = new_centroids  # Update centroids for the next iteration
    return centroids, assignments

# Run the k-means algorithm
centroids, assignments = k_means(k, pointsRead, max_iters=iter, epsilon=EPSILON)

# Print final centroids (rounded to 4 decimal places)
for centroid in centroids:
    print([round(float(val), 4) for val in centroid])

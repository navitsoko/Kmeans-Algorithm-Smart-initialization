# Kmeans-Algorithm
 The K-means++ algorithm is used to choose initial centroids for the K-means algorithm.
 The mplementation of Kmeans++ algorithm is in Python and integrated with the K-means-clustering algorithm ported to a C extension using the C API.

# Introduction to K-means
The K-means algorithm is a popular clustering method for finding a partition of N unlabeled observations into K distinct clusters, where K is a parameter of the method.

Given a set of N datapoints x1, x2, . . . , xN ∈ Rd , the goal is to group the data into K ∈ N clusters, each datapoint is assigned to exactly one cluster and the number of clusters K is such that 1 < K < N. Each cluster k is represented by it’s centroid which is the mean µk ∈ Rd of the cluster’s members.

 # K-means++ Algorithm
 
 1. Choose one center uniformly at random among the data point.
 2. For each data point x not chosen yet, compute D(x), the distance between x and the nearest center that has already been chosen.
 3. Choose one new data point at random as a new center, using a weighted probability distribution where a point x is chosen with probability proportional to  P(x_l) = D(x_l)/sum(D(x_i)).
 4. Repeat Steps 2 and 3 until k centers have been chosen.
 5. Now that the initial centers have been chosen, proceed using standard k-means clustering.

# K-means Algorithm
Initialize centroids as first k datapoints: µk = xk, ∀k ∈ K.
repeat:
Assign every xi to the closest cluster k: argmin_k d(xi, µk), ∀k 1 ≤ k ≤ K.
Update the centroids: µk = 1/|k|* sum(xi : xi∈k ).
until convergence: (∆µk < epsilon) OR (iteration_number = iter).

# Description
1. kmeans_pp.py: The main interface. Contains all of the command line argument interface, reading the data, the Numpy, K-means++ implementation, the
interface with the C extension and outputting the results.
2. kmeansmodule.c: A C extension containing the K-means-clustering implementation.
with step 1 of the algorithm (finding the initial centroids) replaced by values passed
from the K-means++ algorithm implemented in kmeans_pp.py.
3. setup.py: The setup file.

# kmeans_pp.py
1. Reading user CMD arguments:
• k: Number of required clusters. 1 < K < N, K ∈ N.
• iter: (Optional) argument determines the number of K-means iterations, if not provided the default value is 300. 1 < iter < 1000, iter ∈ N. 
• eps: the epsilon value for convergence. epsilon = 0.001.
• file_name_1: The path to the file 1 that will contain N observations.
• file_name_2: The path to the file 2 that will contain N observations.
• The data points privided in a txt or .csv file.
2. Combine both input files by inner join using the first column in each file as a key.
3. After join, sort the data points by the ’key’ in ascending order.
4. Implementation of the k-means++ algorithm.
5. Interfacing with the C extension - Importing C module mykmeanssp, calling fit() method with passing the initial centroids, the datapoints and other
arguments if needed, and getting the final centroids returned by fit().
6. Outputting results.

# kmeansmodule.c
The C extension which will be mainly, the implementation of the K-means-clustering algorithm (another repository). excluding step 1 which will be replaced by the K-means++ algorithm result.
The final module (as seen in Python) should be named mykmeanssp.
The module API provides a function called fit(): (a) The Function receives the initial centroids, datapoints and other necessary arguments.
(b) Skips step 1 from k-means and run it.
(c) Return the centroids.

# setup.py
This is the build used to create the *.so file that will allow kmeans_pp.py to import mykmeanssp.

# Build and Running
 To build the Python extension run: setup.py build_ext --inplace
 For example run : 'kmeans_pp.py 3 100 0.01 input_1.txt input_2.txt'
 

 









 
 
 

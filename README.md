# K-means Clustering — Python & C

Implementation of the **K-means clustering algorithm** in both Python and C.

The project implements the same clustering workflow in two languages, providing experience with numerical algorithms, input validation, iterative convergence, low-level memory management, and data representation.

## Features

- Implements K-means clustering in both **Python** and **C**
- Assigns each data point to its nearest centroid using Euclidean distance
- Recomputes cluster centroids iteratively
- Stops when all centroid movements fall below a convergence threshold
- Supports a configurable maximum number of iterations
- Validates command-line arguments and input data
- Reads multidimensional data points from standard input
- Outputs final centroids with consistent numeric formatting

## Algorithm

The algorithm starts by selecting the first `K` data points as the initial centroids.

For each iteration:

1. Assign every data point to its nearest centroid.
2. Compute the mean of all points assigned to each cluster.
3. Update the cluster centroids.
4. Check whether the centroids have converged.
5. Stop when convergence is reached or the maximum number of iterations is exceeded.


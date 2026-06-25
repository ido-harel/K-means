import sys

def euclidean(p, q):
    return sum((p[i] - q[i]) ** 2 for i in range(len(p))) ** 0.5

def kmeans(datapoints, K, max_iter):
    epsilon = 0.001 
    # initialization
    centroids = [datapoints[i][:] for i in range(K)]  
    d = len(datapoints[0])  

    for _ in range(max_iter):
        clusters = [[] for _ in range(K)] 
        # assign each point to correct cluster
        for point in datapoints:
            distances = [euclidean(point, centroids[k]) for k in range(K)]
            clusters[distances.index(min(distances))].append(point)

        # recompute each centroid 
        new_centroids = [
            [sum(p[j] for p in clusters[k]) / len(clusters[k]) for j in range(d)]
            if clusters[k] else centroids[k][:]
            for k in range(K)
        ]

        if all(euclidean(new_centroids[k], centroids[k]) < epsilon for k in range(K)):
            centroids = new_centroids
            break  
        centroids = new_centroids 

    return centroids

def main():
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("An Error Has Occurred")
        sys.exit(1)

    try:
        if not sys.argv[1].lstrip('+').isdigit():
            raise ValueError  
        K = int(sys.argv[1])
    except ValueError:
        print("Incorrect number of clusters!")
        sys.exit(1)

    max_iter = 400  # default 

    if len(sys.argv) == 3:
        try:
            if not sys.argv[2].lstrip('+').isdigit():
                raise ValueError  
            max_iter = int(sys.argv[2])
        except ValueError:
            print("Incorrect maximum iteration!")
            sys.exit(1)

    # iter's full valid range (1 < iter < 800) is known without reading any data,
    # so it's checked immediately rather than after the stdin read below.
    if not (1 < max_iter < 800):
        print("Incorrect maximum iteration!")
        sys.exit(1)

    # K's upper bound needs N, but the lower bound doesn't - check it before
    # reading stdin so we don't pay for a read on bad input.
    if K <= 1:
        print("Incorrect number of clusters!")
        sys.exit(1)

    # read datapoints from stdin
    datapoints = []
    d = None  # dimension 
    for line in sys.stdin:
        line = line.strip()
        if line:
            try:
                point = [float(x) for x in line.split(',')]
            except ValueError:
                print("An Error Has Occurred")
                sys.exit(1)
            if d is None:
                d = len(point)
            elif len(point) != d:
                print("An Error Has Occurred")
                sys.exit(1)
            datapoints.append(point)

    if len(datapoints) == 0:
        print("An Error Has Occurred")
        sys.exit(1)

    N = len(datapoints)  

    # now that N is known, finish validating K's upper bound (K < N)
    if not (K < N):
        print("Incorrect number of clusters!")
        sys.exit(1)

    centroids = kmeans(datapoints, K, max_iter)

    for c in centroids:
        print(','.join('%.4f' % x for x in c)) 

main()

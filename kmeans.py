import sys


def euclidean(p, q):
    return sum((p[i] - q[i]) ** 2 for i in range(len(p))) ** 0.5


def kmeans(datapoints, K, max_iter):
    epsilon = 0.001 
    centroids = [datapoints[i][:] for i in range(K)]  # first K points as initial centroids, per spec
    d = len(datapoints[0])  # dimensionality of each point

    for _ in range(max_iter):
        clusters = [[] for _ in range(K)]  # clusters[k] = points currently assigned to centroid k
        for point in datapoints:
            # assign each point to whichever centroid is nearest
            distances = [euclidean(point, centroids[k]) for k in range(K)]
            clusters[distances.index(min(distances))].append(point)

        # recompute each centroid as the mean of its assigned points,
        # one coordinate (j) at a time; keep the old centroid if its cluster is empty
        new_centroids = [
            [sum(p[j] for p in clusters[k]) / len(clusters[k]) for j in range(d)]
            if clusters[k] else centroids[k][:]
            for k in range(K)
        ]

        if all(euclidean(new_centroids[k], centroids[k]) < epsilon for k in range(K)):
            centroids = new_centroids
            break  # converged: every centroid moved less than epsilon this iteration

        centroids = new_centroids  # not converged yet, keep iterating

    return centroids


def main():
    # usage: kmeans.py K [iter], so argv is 2 or 3 elements (script name + 1 or 2 args)
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("An Error Has Occurred")
        sys.exit(1)

    try:
        if not sys.argv[1].lstrip('+').isdigit():
            raise ValueError  # strict digit-only check, mirrors C's strtol (rejects "3.0", "1e1", "3 ")
        K = int(sys.argv[1])
    except ValueError:
        print("Incorrect number of clusters!")
        sys.exit(1)

    max_iter = 400  # default per spec when iter arg is omitted
    if len(sys.argv) == 3:
        try:
            if not sys.argv[2].lstrip('+').isdigit():
                raise ValueError  # same strict digit-only guard as K, applied to iter
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
    # reading stdin so we don't pay for a (possibly huge) read on bad input.
    if K <= 1:
        print("Incorrect number of clusters!")
        sys.exit(1)

    # read datapoints from stdin: one point per line, coordinates comma-separated
    datapoints = []
    d = None  # dimension of the first row; every later row must match it
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

    N = len(datapoints)  # only known now that stdin has been fully consumed

    # now that N is known, finish validating K's upper bound (K < N)
    if not (K < N):
        print("Incorrect number of clusters!")
        sys.exit(1)

    centroids = kmeans(datapoints, K, max_iter)

    for c in centroids:
        print(','.join('%.4f' % x for x in c))  # 4 decimal places, per output spec


main()

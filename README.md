# HW1: K-means Clustering - Task Checklist

## 1. Setup & Environment
- [ V ] Create GitHub repository and add partner
- [ V ] Add `.gitignore` to ignore compiled binaries (`kmeans`) and compressed archives (`*.tar.gz`)
- [ V ] Set up the course's Docker environment

## 2. Python Implementation (`kmeans.py`)
- [ ] **Argument Parsing & Validation**
  - [ ] Parse `K` and `iter` from command line arguments
  - [ ] Handle default value for `iter = 400` if not provided
  - [ ] Validate 1 < K < N (Print "Incorrect number of clusters!" on failure)
  - [ ] Validate 1 < iter < 800 (Print "Incorrect maximum iteration!" on failure)
- [ ] **Data Ingestion**
  - [ ] Read data points dynamically from standard input (`STDIN`)
  - [ ] Parse vectors using `.split()` and cast elements to `float`
  - [ ] Handle the extra trailing empty row gracefully
- [ ] **K-means Core Logic**
  - [ ] Initialize centroids as the first K data points
  - [ ] Implement iterative loop (up to `iter` times)
  - [ ] Assign each data point to closest centroid using Euclidean distance
  - [ ] Update centroids to be the mean of their cluster members
  - [ ] Implement convergence check (Delta_mu_k < epsilon where epsilon = 0.001)
- [ ] **Output & Verification**
  - [ ] Format outputs strictly to 4 decimal places (`%.4f`)
  - [ ] Print final centroids to `STDOUT` matching Moodle test outputs
  - [ ] Verify execution command: `python3 kmeans.py 3 100 < input_data.txt`

## 3. C Implementation (`kmeans.c`)
- [ ] **Argument Parsing & Validation**
  - [ ] Parse `K` and `iter` from `argv`
  - [ ] Default `iter` to 400 if omitted
  - [ ] Add strict constraint validation and specific error printing
- [ ] **Memory Management & Data Ingestion**
  - [ ] Set up `#define _GNU_SOURCE` if using `getline()`
  - [ ] Read input from `STDIN` and store coordinates using `double` arrays
  - [ ] Dynamically allocate memory for data points and centroids
- [ ] **K-means Core Logic**
  - [ ] Initialize centroids with first K points
  - [ ] Implement clustering loop with Euclidean Distance convergence
  - [ ] Properly free all dynamically allocated memory blocks before exit
- [ ] **Compilation & Output**
  - [ ] Add `#include` for allowed headers: `stdlib.h`, `stdio.h`, `math.h`
  - [ ] Format output to 4 decimal places (`%.4f`) using `printf`
  - [ ] Ensure clean compilation on Docker using:
        `gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans -lm`
  - [ ] Verify successful exit returns 0 errors return 1

## 4. Submission Prep
- [ ] Cross-test C and Python outputs against all 3 Moodle example files
- [ ] Create a directory named `id1_id2_assignment1` containing only `kmeans.c` and `kmeans.py`
- [ ] Compress via Docker terminal: `tar -czvf id1_id2_assignment1.tar.gz id1_id2_assignment1`
- [ ] Submit the `.tar.gz` file on Moodle before the deadline

## 5. Edge cases - Print error message and terminate
- [ ] If there is a point or some points of different dimantions.
- [ ]  
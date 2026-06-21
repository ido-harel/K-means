# HW1: K-means Clustering - Task Checklist

## 1. Setup & Environment
- [ ] [cite_start]Create GitHub repository and add partner [cite: 2]
- [ ] Add `.gitignore` to ignore compiled binaries (`kmeans`) and compressed archives (`*.tar.gz`)
- [ ] [cite_start]Set up the course's Docker environment [cite: 25, 38]

## 2. Python Implementation (`kmeans.py`)
- [ ] **Argument Parsing & Validation**
  - [ ] [cite_start]Parse `K` and `iter` from command line arguments [cite: 40, 52]
  - [ ] [cite_start]Handle default value for `iter = 400` if not provided [cite: 22]
  - [ ] [cite_start]Validate $1 < K < N$ (Print `"Incorrect number of clusters!"` on failure) [cite: 18, 61]
  - [ ] [cite_start]Validate $1 < iter < 800$ (Print `"Incorrect maximum iteration!"` on failure) [cite: 18, 61]
- [ ] **Data Ingestion**
  - [ ] [cite_start]Read data points dynamically from standard input (`STDIN`) [cite: 44]
  - [ ] [cite_start]Parse vectors using `.split()` and cast elements to `float` [cite: 47, 67]
  - [ ] [cite_start]Handle the extra trailing empty row gracefully [cite: 59]
- [ ] **K-means Core Logic**
  - [ ] [cite_start]Initialize centroids as the first $K$ data points [cite: 10]
  - [ ] [cite_start]Implement iterative loop (up to `iter` times) [cite: 10]
  - [ ] [cite_start]Assign each data point to closest centroid using Euclidean distance [cite: 10, 12]
  - [ ] [cite_start]Update centroids to be the mean of their cluster members [cite: 10]
  - [ ] [cite_start]Implement convergence check ($\Delta\mu_k < \epsilon$ where $\epsilon = 0.001$) [cite: 10, 21]
- [ ] **Output & Verification**
  - [ ] [cite_start]Format outputs strictly to 4 decimal places (`%.4f`) [cite: 53]
  - [ ] [cite_start]Print final centroids to `STDOUT` matching Moodle test outputs [cite: 49, 58]
  - [ ] [cite_start]Verify execution command: `python3 kmeans.py 3 100 < input_data.txt` [cite: 40]

## 3. C Implementation (`kmeans.c`)
- [ ] **Argument Parsing & Validation**
  - [ ] [cite_start]Parse `K` and `iter` from `argv` [cite: 30, 52]
  - [ ] [cite_start]Default `iter` to 400 if omitted [cite: 22]
  - [ ] [cite_start]Add strict constraint validation and specific error printing [cite: 18, 61]
- [ ] **Memory Management & Data Ingestion**
  - [ ] [cite_start]Set up `#define _GNU_SOURCE` if using `getline()` [cite: 33]
  - [ ] [cite_start]Read input from `STDIN` and store coordinates using `double` arrays [cite: 32, 67]
  - [ ] [cite_start]Dynamically allocate memory for data points and centroids [cite: 63]
- [ ] **K-means Core Logic**
  - [ ] [cite_start]Initialize centroids with first $K$ points [cite: 10]
  - [ ] [cite_start]Implement clustering loop with Euclidean Distance convergence [cite: 10, 12, 13]
  - [ ] [cite_start]Properly free all dynamically allocated memory blocks before exit [cite: 63]
- [ ] **Compilation & Output**
  - [ ] [cite_start]Add `#include` for allowed headers: `stdlib.h`, `stdio.h`, `math.h` [cite: 36]
  - [ ] [cite_start]Format output to 4 decimal places (`%.4f`) using `printf` [cite: 53]
  - [ ] Ensure clean compilation on Docker using:
        [cite_start]`gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans -lm` [cite: 26, 35]
  - [ ] [cite_start]Verify successful exit returns `0` and errors return `1` [cite: 64]

## 4. Submission Prep
- [ ] [cite_start]Cross-test C and Python outputs against all 3 Moodle example files [cite: 58]
- [ ] [cite_start]Create a directory named `id1_id2_assignment1` containing **only** `kmeans.c` and `kmeans.py` [cite: 73, 74, 75]
- [ ] [cite_start]Compress via Docker terminal: `tar -czvf id1_id2_assignment1.tar.gz id1_id2_assignment1` [cite: 76, 77, 78]
- [ ] [cite_start]Submit the `.tar.gz` file on Moodle before the deadline [cite: 3, 70]

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.001
#define DEFAULT_ITER 400

typedef struct Cord {
    double value;
    struct Cord *next;
} Cord;

typedef struct Vector {
    Cord *cords;
    struct Vector *next;
} Vector;

typedef struct DataSet {
    double *points;
    int rows;
    int cols;
} DataSet;

/* Function prototypes */
/* Argument parsing and validation */
int parse_args(int argc, char **argv, int *k, int *iter);
int parse_integer_arg(const char *arg, int *value);

/* Data -> Linked List -> Flat Array */
int read_stdin_to_list(Vector **head_out, int *rows_out, int *cols_out);
int append_cord(Cord **head, Cord **tail, double value);
int append_vector(Vector **head, Vector **tail, Cord *cords);
double *list_to_flat_array(Vector *head, int rows, int cols);
int read_data(DataSet *data);

/* Memory cleanup */
void free_cords(Cord *head);
void free_vectors(Vector *head);
void free_dataset(DataSet *data);

double *init_centroids(const DataSet *data, int k);

/* K-Means algorithms */
double squared_distance(const double *point, const double *centroid, int cols);
int find_closest_centroid(const double *point, const double *centroids, int k, int cols);
void reset_clusters(double *sums, int *counts, int k, int cols);
void add_point_to_cluster(const double *point, int cluster_index, double *sums, int *counts, int cols);
int update_centroids(double *centroids, const double *sums, const int *counts, int k, int cols, double epsilon);
int run_kmeans(const DataSet *data, double *centroids, int k, int iter, double epsilon);
void print_centroids(const double *centroids, int k, int cols);

/* Functions logic */
int parse_integer_arg(const char *arg, int *val){
    char *end;
    long result;
    if (arg == NULL || *arg == '\0') {
        return 1;
    }
    result = strtol(arg, &end, 10);
    if (*end != '\0') {
        return 1;
    }
    *val = (int)result;
    return 0; 
}
int parse_args(int argc, char **argv, int *k, int *iter)
{
    /* 1 - genral error, 2 - k error, 3 - iter error */
    if (argc != 2 && argc != 3) return 1; 
    if (parse_integer_arg(argv[1], k)) return 2; 
    if (*k <= 1) return 2;
    if (argc == 2) *iter = DEFAULT_ITER;
    else {
        if (parse_integer_arg(argv[2], iter)) {
            return 3; 
        }
        if (*iter <= 1 || *iter >= 800) {
            return 3; 
        }
    }
    return 0;
}

int read_stdin_to_list(Vector **head_out, int *rows_out, int *cols_out)
{
    Vector *head;
    Vector *tail;
    Cord *cord_head;
    Cord *cord_tail;
    double value;
    int rows;
    int cols;
    int curr_cols;
    int scan_status;
    int separator;

    head = NULL;
    tail = NULL;
    cord_head = NULL;
    cord_tail = NULL;
    rows = 0;
    cols = 0;
    curr_cols = 0;

    while ((scan_status = scanf("%lf", &value)) == 1) {
        if (!append_cord(&cord_head, &cord_tail, value)) {
            free_cords(cord_head);
            free_vectors(head);
            *head_out = NULL;
            return 0;
        }
        curr_cols++;
        separator = getchar();

        while (separator == ' ' || separator == '\t' || separator == '\r') {
            separator = getchar();
        }
        if (separator == ',') continue;
        if (separator == '\n' || separator == EOF) {
            if (rows == 0) cols = curr_cols;
            else if (curr_cols != cols) {
                free_cords(cord_head);
                free_vectors(head);
                *head_out = NULL;
                return 0;
            }
            if (!append_vector(&head, &tail, cord_head)) {
                free_cords(cord_head);
                free_vectors(head);
                *head_out = NULL;
                return 0;
            }
            rows++;
            cord_head = NULL;
            cord_tail = NULL;
            curr_cols = 0;
            if (separator == EOF) break;
        } 
        else {
            free_cords(cord_head);
            free_vectors(head);
            *head_out = NULL;
            return 0;
        }
    }
    if (scan_status == 0 || curr_cols != 0 || rows == 0) {
        free_cords(cord_head);
        free_vectors(head);
        *head_out = NULL;
        return 0;
    }
    *head_out = head;
    *rows_out = rows;
    *cols_out = cols;
    return 1;
}

int append_cord(Cord **head, Cord **tail, double value)
{
    Cord *node;
    node = (Cord *) malloc(sizeof(Cord));
    if (node == NULL) return 0;
    node->value = value;
    node->next = NULL;
    if (*head == NULL) *head = node;
    else (*tail)->next = node;
    *tail = node;
    return 1;
}

int append_vector(Vector **head, Vector **tail, Cord *cords)
{
    Vector *node;
    node = (Vector *) malloc(sizeof(Vector));
    if (node == NULL) return 0;
    node->cords = cords;
    node->next = NULL;
    if (*head == NULL) *head = node;
    else (*tail)->next = node;
    *tail = node;
    return 1;
}

double *list_to_flat_array(Vector *head, int rows, int cols)
{
    double *points;
    Vector *curr_vec;
    Cord *curr_cord;
    int i;
    int j;

    points = (double *) malloc(rows * cols * sizeof(double));
    if (points == NULL) return NULL;
    curr_vec = head;

    for (i = 0; i < rows; i++) {
        if (curr_vec == NULL) {
            free(points);
            return NULL;
        }
        curr_cord = curr_vec->cords;
        for (j = 0; j < cols; j++) {
            if (curr_cord == NULL) {
                free(points);
                return NULL;
            }
            points[i * cols + j] = curr_cord->value;
            curr_cord = curr_cord->next;
        }
        curr_vec = curr_vec->next;
    }
    return points;
}

int read_data(DataSet *data)
{
    Vector *head;
    double *points;
    int rows;
    int cols;
    head = NULL;
    points = NULL;
    rows = 0;
    cols = 0;

    data->points = NULL;
    data->rows = 0;
    data->cols = 0;
    if (!read_stdin_to_list(&head, &rows, &cols)) {
        free_vectors(head);
        return 0;
    }

    points = list_to_flat_array(head, rows, cols);
    free_vectors(head);
    if (points == NULL) {
        return 0;
    }
    data->points = points;
    data->rows = rows;
    data->cols = cols;
    return 1;
}

void free_cords(Cord *head)
{
    Cord *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void free_vectors(Vector *head)
{
    Vector *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free_cords(tmp->cords);
        free(tmp);
    }
}

void free_dataset(DataSet *data)
{
    free(data->points);
    data->points = NULL;
    data->rows = 0;
    data->cols = 0;
}

double *init_centroids(const DataSet *data, int k)
{
    double *centroids;
    int i;
    int j;

    centroids = (double *) malloc(k * data->cols * sizeof(double));
    if (centroids == NULL) return NULL;

    for (i = 0; i < k; i++) {
        for (j = 0; j < data->cols; j++) {
            centroids[i * data->cols + j] = data->points[i * data->cols + j];
        }
    }
    return centroids;
}

double squared_distance(const double *point, const double *centroid, int cols){
    double sum = 0.0;
    double diff;
    int i;
    for(i=0; i< cols; i++){
        diff = point[i] - centroid[i];
        sum += diff * diff;
    }
    return sum;
}

int find_closest_centroid(const double *point, const double *centroids, int k, int cols){
    int i;
    double dst;
    double min_dst = squared_distance(point, centroids, cols);
    int closest = 0;
    for(i=1; i< k; i++){
        dst = squared_distance(point, centroids + i * cols, cols);
        if(dst < min_dst){
            min_dst = dst;
            closest = i;
        }
    }
    return closest;
}

void reset_clusters(double *sums, int *counts, int k, int cols)
{
    int i;
    int j;
    for (i = 0; i < k; i++) {
        counts[i] = 0;
        for (j = 0; j < cols; j++) {
            sums[i * cols + j] = 0.0;
        }
    }
}

void add_point_to_cluster(const double *point, int cluster_index, double *sums, int *counts, int cols)
{
    int j;
    for (j = 0; j < cols; j++) {
        sums[cluster_index * cols + j] += point[j];
    }
    counts[cluster_index]++;
}

int update_centroids(double *centroids, const double *sums, const int *counts, int k, int cols, double epsilon){
    int i;
    int j;
    double old_value;
    double new_value;
    double diff;
    double delta_squared;
    double epsilon_squared;
    int converged;

    converged = 1; 
    epsilon_squared = epsilon * epsilon;
    for (i = 0; i < k; i++) {
        delta_squared = 0.0;
        if (counts[i] == 0) continue;
        for (j = 0; j < cols; j++) {
            old_value = centroids[i * cols + j];
            new_value = sums[i * cols + j] / counts[i];
            diff = new_value - old_value;
            delta_squared += diff * diff;
            centroids[i * cols + j] = new_value;
        }
        if (delta_squared >= epsilon_squared) {
            converged = 0;
        }
    }
    return converged;
}

int run_kmeans(const DataSet *data, double *centroids, int k, int iter, double epsilon)
{
    double *sums;
    int *counts;
    const double *point;
    int iteration;
    int i;
    int closest;
    int converged;

    sums = (double *) calloc(k * data->cols, sizeof(double));
    counts = (int *) calloc(k, sizeof(int));

    if (sums == NULL || counts == NULL) {
        free(sums);
        free(counts);
        return 0;
    }
    for (iteration = 0; iteration < iter; iteration++) {
        reset_clusters(sums, counts, k, data->cols);
        for (i = 0; i < data->rows; i++) {
            point = data->points + i * data->cols;
            closest = find_closest_centroid(point, centroids, k, data->cols);
            add_point_to_cluster(point, closest, sums, counts, data->cols);
        }
        converged = update_centroids(centroids, sums, counts, k, data->cols, epsilon);
        if (converged) break;
    }
    free(sums);
    free(counts);
    return 1;
}

void print_centroids(const double *centroids, int k, int cols)
{
    int i;
    int j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < cols; j++) {
            if (j < cols - 1) printf("%.4f,", centroids[i * cols + j]);
            else printf("%.4f\n", centroids[i * cols + j]);
        }
    }
}

int main(int argc, char **argv)
{
    int k;
    int iter;
    int parse_status;
    DataSet data;
    double *centroids;

    parse_status = parse_args(argc, argv, &k, &iter);
    if (parse_status == 1) {
        printf("An Error Has Occurred\n");
        return 1;
    } else if (parse_status == 2) {
        printf("Incorrect number of clusters!\n");
        return 1;
    } else if (parse_status == 3) {
        printf("Incorrect maximum iteration!\n");
        return 1;
    }

    if (!read_data(&data)) {
        printf("An Error Has Occurred\n");
        return 1;
    }

    if (k >= data.rows) {
        printf("Incorrect number of clusters!\n");
        free_dataset(&data);
        return 1;
    }

    centroids = init_centroids(&data, k);
    if (centroids == NULL) {
        printf("An Error Has Occurred\n");
        free_dataset(&data);
        return 1;
    }

    if (!run_kmeans(&data, centroids, k, iter, EPSILON)) {
        printf("An Error Has Occurred\n");
        free(centroids);
        free_dataset(&data);
        return 1;
    }
    print_centroids(centroids, k, data.cols);

    free(centroids);
    free_dataset(&data);
    return 0;
}



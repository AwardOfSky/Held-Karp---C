#ifndef HELDKARP_INCLUDED
#define HELDKARP_INCLUDED
 
// Global Macros
#define MAX_INTEGER 2147483647
#define BITS_MAX_CITIES 5
#define MAX_CITIES (1 << BITS_MAX_CITIES)
#define SMALL_CITY_SET MAX_CITIES - BITS_MAX_CITIES
#define MAX_BUF_SIZE 4
#define BENCH_RUNS 3
#define BENCH_SAME_COST 1
#define BENCH_MAX_DISTANCES 10000
#define BENCH_STARTING_CITY 0
#define WARMUP_CITY_NUMBER 16
#define STANDARD_THREAD_NUM 8

//hashmap
typedef struct m_node {
    long key;
    long value;
    struct m_node *next;
} m_node;

typedef struct s_node {
    int key;
    int value;
    struct s_node *next;
} s_node;

int size;
s_node **arr;
//matrix
int **matrix;
int cities;
int starting_city;
int max_distance;
//combinations
int **combinations;
int set;
int elements;
int total_combs;
//held karp
int result;
int *result_path;
struct timespec start;
struct timespec finish;
double elapsed;
//threads
pthread_t *threads;
char *mutexes;
int resets;
int error;
unsigned int procs;

struct thread_args {
    int id;
    int iteration;
} thread_args;


//hashmap
void create(int n);
void put(int key, int value);
int get(int key);
//matrix
void set_random_matrix(int cities, int max_distance, int same_distance);
void print_matrix(void);
void set_cities(int n);
void set_starting_city(int n);
void set_max_distance(int n);
//combinations
void set_combinations(int n, int r);
int nCr(int n, int r);
void get_next_comb(void);
//held karp
int calculate_held_karp(void);
void print_path(void);
void inicial_data(int cities, int max_distance, int starting_city, int same_distance);
//threads
void *thread_task(void *arguments);

#endif

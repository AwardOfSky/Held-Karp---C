#ifndef HELDKARP_INCLUDED
#define HELDKARP_INCLUDED

// Global Macros
#define MAX_INTEGER 2147483647
#define BITS_MAX_CITIES 5
#define MAX_CITIES (1 << BITS_MAX_CITIES)
#define SPEED_UP_LOOP MAX_CITIES - BITS_MAX_CITIES + 1
#define MAX_BUF_SIZE 4
#define BENCH_RUNS 3
#define BENCH_SAME_COST 1
#define BENCH_MAX_DISTANCES 10000
#define BENCH_STARTING_CITY 0
#define WARMUP_CITY_NUMBER 5

//hashmap
typedef struct node {
    int first;
    int second;
    int vfirst;
    int vsecond;
    struct node *next;
} node;

int size;
node **arr;
//matrix
int **matrix;
int cities;
int starting_city;
//combinations
int *combinations;
int set;
int elements;
int combs_left;
int total_combs;
//held karp
int result;
int *result_path;
clock_t end;
clock_t begin;

//hashmap
void create(int n);
void put(int first, int second, int vfirst, int vsecond);
int get(int first, int second, int get_first);
void print_node_list(int index);
void print_all_node_lists(void);
//matrix
void set_random_matrix(int cities, int max_distance, int same_distance);
void print_matrix(void);
void set_cities(int n);
void set_starting_city(int n);
//combinations
void set_combinations(int n, int r);
int nCr(int n, int r);
void get_next_comb(void);
//held karp
int calculate_held_karp(void);
void print_path(void);
void inicial_data(int cities, int max_distance, int starting_city, int same_distance);

#endif

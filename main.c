#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef linux
#include <unistd.h>
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif
#include "helper.h"
#include "heldkarp.h"


void benchmark(void);
void standard(void);
void clean_up(int final);
void set_thread_num(void);
unsigned int get_processors();

int main() {
    char bench_buf[MAX_BUF_SIZE];
    
    srand(time(NULL));
    procs = get_processors();
    
    // User input
    printf("Would you like to benchmark? (y/n): ");
    scanf("%s", bench_buf);
    if(check_yes(bench_buf)) {
    	benchmark();
    } else {
	standard();
    }
    
    return 0;
}

void benchmark(void) {
    double fastest_run;
    int input_start;
    int input_end;

    printf("Running benchmark from ? to ? cities:\n");
    printf("Inicial city: ");
    scanf("%d", &input_start);
    printf("Last city: ");
    scanf("%d", &input_end);

    input_end = constrain(input_end, 0, MAX_CITIES);
    input_start = constrain(input_start, 0, input_end);

    printf("Presenting best results from %d runs.\n", BENCH_RUNS);
    
    for(int a = input_start - 1; a <= input_end; a++) {

	fastest_run = (double)MAX_INTEGER;

	if(a == input_start - 1)  {
	    printf("Warming up the processor...\n");
	}
	
	// Perform a predefined number of calculations
	for(int b = 0; b < BENCH_RUNS; b++) {

	    // Warm up processor on first iteration
	    if(a == input_start - 1)  {
		inicial_data(WARMUP_CITY_NUMBER, BENCH_MAX_DISTANCES,
			     BENCH_STARTING_CITY, BENCH_SAME_COST);
	    } else {
		inicial_data(a, BENCH_MAX_DISTANCES,
			     BENCH_STARTING_CITY, BENCH_SAME_COST);
	    }
	    
	    clock_gettime(CLOCK_MONOTONIC, &start);
	    result = calculate_held_karp();
	    clock_gettime(CLOCK_MONOTONIC, &finish);
	    elapsed = (finish.tv_sec - start.tv_sec);
	    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	    
	    if(elapsed < fastest_run) {
		fastest_run = elapsed;
	    }

	    clean_up(0);
	}
			
	// Choose the maximum
	if(a == input_start - 1) {
	    printf("Starting benchmark...\n");
	} else {
	    printf("Best time for %d cities was %.6f seconds\n", a,
		   fastest_run);
	}
    }
}

void standard(void) {
    int input_n;
    int input_maxd;
    int input_start;
    char input_buf[MAX_BUF_SIZE];
    int same_dist = 0;

    //User input
    printf("Enter the number of cities: ");
    scanf("%d", &input_n);
    printf("Enter the maximun distance beetween this cities: ");
    scanf("%d", &input_maxd);
    printf("Enter the starting city: ");
    scanf("%d", &input_start);
    printf("Is the cost beetween two cities the same in both directions? (y/n): ");
    same_dist = scanf("%s", input_buf);
    
    if(check_yes(input_buf)) {
	same_dist = 1;
    }
    
    // Set inicial data
    inicial_data(input_n, input_maxd, input_start, same_dist);
    
    // Perform and time computation
    clock_gettime(CLOCK_MONOTONIC, &start);
    result = calculate_held_karp();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    //Print results
    printf("\nResults\t\t:\n");
    printf("Optimal cost\t: %d\n", result);
    print_path();
    printf("Time taken\t: %.6f seconds.\n", elapsed);

    // Print statistics
    printf("\nStatistics\t:\n");
    printf("No. Cities\t: %d\n", cities);
    printf("No. Threads\t: %d\n", procs);
    printf("Hashmap entries\t: %d\n", (cities - 1) << (cities - 2));

    // Clean up and exit
    clean_up(1);
}

void clean_up(int final) {
    if(final) {
	for(int i = 0; i < size; i++) {
	    free(arr[i]);
	}
    }
    
    free(mutexes);
    free(matrix);
    free(result_path);
    free(threads);
    free(arr);
}

unsigned int get_processors() {
#ifdef linux
    return sysconf(_SC_NPROCESSORS_ONLN);    
#elif defined(_WIN32) || defined(WIN32)
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    return STANDARD_THREAD_NUM;
#endif
}

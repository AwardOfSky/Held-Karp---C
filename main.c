#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helper.h"
#include "heldkarp.h"

void benchmark(void);
void standard(void);
void clean_up(void);

int main() {
    char bench_buf[MAX_BUF_SIZE];
    
    srand(time(NULL));
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
		
		// Perform calculation over a number of times
		for(int b = 0; b < BENCH_RUNS; b++) {

			// Warm up processor on first iteration
			if(a == input_start - 1)  {
				inicial_data(WARMUP_CITY_NUMBER, BENCH_MAX_DISTANCES,
					BENCH_STARTING_CITY, BENCH_SAME_COST);
			} else {
				inicial_data(a, BENCH_MAX_DISTANCES,
					BENCH_STARTING_CITY, BENCH_SAME_COST);
			}
			
			begin = clock();
			result = calculate_held_karp();
			end = clock();
			if((double)(end - begin) < fastest_run) {
				fastest_run = (double)(end - begin);
			}

			clean_up();
		}
				
		// Choose the maximum
		if(a == input_start - 1) {
			printf("Starting benchmark...\n");
		} else {
			printf("Best time for %d cities was %.3f seconds\n", a, fastest_run / CLOCKS_PER_SEC);
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
    begin = clock();
    result = calculate_held_karp();
    end = clock();

    // Print results
    printf("\nOptimal cost\t: %d\n", result);
    print_path();
    printf("Time taken\t: %.3f seconds.\n", (double)(end - begin) / CLOCKS_PER_SEC);
    
    // Clean up and exit
    clean_up();
}

void clean_up(void) {
    free(arr);
    free(combinations);
    free(matrix);
    free(result_path);
}

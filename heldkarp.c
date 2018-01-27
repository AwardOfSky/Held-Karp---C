#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helper.h"
#include "heldkarp.h"

void create(int n) {
    size = next_power_2(n);
    arr = (node **)malloc(sizeof(*arr) * size);
}

void put(int first, int second, int vfirst, int vsecond) {
    int index = (first * 31 + second) & (size - 1);
    node *temp = malloc(sizeof(node));
    temp->first = first;
    temp->second = second;
    temp->vfirst = vfirst;
    temp->vsecond = vsecond;
    if(arr[index] != NULL) {
		temp->next = arr[index];
    }
    arr[index] = temp;
}

int get(int first, int second, int get_first) { //segmentation fault if empty node list
    node *cur = arr[(first * 31 + second) & (size - 1)];
    while (cur->next != NULL && cur->first != first && cur->second != second) {
		cur = cur->next;
    }
    if(get_first) {
		return cur->vfirst;
    } else {
		return cur->vsecond;
    }
}

void print_node_list(int index) {
    node *cur = arr[index];
    while (cur != NULL) {
        printf("(%d, %d) - (%d, %d) | ", cur->first, cur->second, cur->vfirst, cur->vsecond);
        cur = cur->next;
    }
}

void print_all_node_lists(void) {
    for(int i = 0; i < size; i++) {
        printf("Index %d: ", i);
        print_node_list(i);
        printf("\n");
    }
}

int next_power_2(int a) {
    int b = 1;
    while (b < a) {
        b <<= 1;
    }
    return b;
}

void set_random_matrix(int cities, int max_distance, int same_distance) {
    matrix = (int **)malloc(sizeof(*matrix) * cities);
    for (int i = 0; i < cities; i++) {
        matrix[i] = (int *)malloc(sizeof(*matrix[i]) * cities);
    }
    for(int i = 0; i < cities; i++) {
        for(int j = 0; j < cities; j++) {
            if(i > j && same_distance) {
				matrix[i][j] = matrix[j][i];
			} else {
				matrix[i][j] = rand() % (max_distance + 1);
			}
        }
    }
}

void print_matrix(void) {
    for(int i = 0; i < cities; i++) {
        for(int j = 0; j < cities; j++) {
            printf("%d, ", matrix[i][j]);
        }
        printf("\n");
    }
}

void set_cities(int n) {
    cities = constrain(n, 1, MAX_CITIES);
}

void set_starting_city(int n) {
    starting_city = constrain(n, 0, cities - 1);
}

void set_combinations(int n, int r) {
    total_combs = nCr(n, r);
    combs_left = total_combs;
    set = n;
    elements = r;
    combinations = (int *)malloc(sizeof(*combinations) * elements);
    for(int i = 0; i < elements; i++) {
        combinations[i] = i + 1;
    }
}

void get_next_comb(void) {
    if (combs_left == total_combs) {
		combs_left--;
    } else {
        int i = elements - 1;
        while (combinations[i] == set - elements + i + 1) {
			i--;
        }
        combinations[i]++;
        for (int j = i + 1; j < elements; j++) {
			combinations[j] = combinations[i] + j - i;
        }
        combs_left--;
    }
}

void print_path(void) {
    printf("Resulting path\t: [");
    for(int i = 0; i < cities + 1; i++) {
        printf("%d ", result_path[i]);
    }
    printf("]\n");
}

void inicial_data(int cities, int max_distance, int starting_city, int same_distance) {
    // Set number of cities
    set_cities(cities);
    // Create hashmap
    create((cities - 1) << (cities - 2));
    // Crate random matrix
    set_random_matrix(cities, max_distance, same_distance);
    // Set starting city
    set_starting_city(starting_city);
}

int calculate_held_karp() {
    int bits;
    int optimal;
    int parent;
    int min_1;
    int min_2;
    
    // Set distances between main cities (except inicial)
    for(int i = 1; i < cities; i++) {
        put(1 << i, i, matrix[0][i], 0);
    }
    
    // Iterate subsets of increasing length until we reach all cities
    for(int subset_size = 2; subset_size < cities; subset_size++) {
	
	// Get combinations for a specific set
	set_combinations(cities - 1, subset_size);
	
	// Iterate over all combinations - optimizations are crucial here
	while(combs_left > 0) {

	    get_next_comb(); // Into array 'combinations'
	    
	    // Allocate bits for all elements in subset
	    bits = 0;
	    for(int j = 0; j < subset_size; j++) {
			bits |= 1 << combinations[j];
	    }
	    
	    // Find the shortest path to get to this subset
	    for(int j = 0; j < subset_size; j++) {
		
			// Set bits of previous set for each city in subset
			int prev = bits & ~(1 << combinations[j]);
			
			// Find all combinations to get to each city in subset
			// This pairs have in account the cost of the previous subset
			int temp_array_size = (subset_size - 1) << 1;
			int *temp = (int *)malloc(sizeof(*temp) * temp_array_size);
			int counter = 0;
			int choosen_k = 0;
			
			min_1 = MAX_INTEGER;
			min_2 = MAX_INTEGER;
			for(int k = 0; k < subset_size; k++) {
				if(k != j) { 
				node *cur = arr[(prev * 31 + combinations[k]) & (size - 1)];
				while (cur->next != NULL && cur->first != prev
					   && cur->second != combinations[k]) {
					cur = cur->next;
				}
				
				temp[counter++] = cur->vfirst
					+ matrix[combinations[k]][combinations[j]];
				temp[counter++] = combinations[k];
				}
			}
			
			if(cities < SPEED_UP_LOOP) {
				int stamin;
				int min_0 = MAX_INTEGER;
				for(int k = 0; k < temp_array_size; k+=2) {
					stamin = (temp[k] << BITS_MAX_CITIES) | temp[k + 1];
					if(stamin < min_0) {
						min_0 = stamin;
						choosen_k = k;
					}
				}
			} else {
				long stamin;
				long min_0 = MAX_INTEGER;
				for(int k = 0; k < temp_array_size; k+=2) {
					stamin = temp[k];
					stamin = (stamin << MAX_CITIES) | temp[k + 1];
					if(stamin < min_0) {
						min_0 = stamin;
						choosen_k = k;
					}
				}
			}

			// Insert minimun distance into hashmap
			put(bits, combinations[j], temp[choosen_k], temp[choosen_k + 1]);
			
			free(temp);
			}
		}
    } // performance critical part is over
    
    // Fill all bits except the one corresponding to the start city
    bits = (1 << cities) - 2;
    
    // Gather pairs based on subsets leading to start city
    int temp_array_size = (cities - 1) * 2;
    int *temp = (int *)malloc(sizeof(*temp) * temp_array_size);
    int counter = 0;
    
    min_1 = MAX_INTEGER;
    min_2 = MAX_INTEGER;
    
    for(int i = 1; i < cities; i++) {
		temp[counter++] = get(bits, i, 1) + matrix[i][0];
		temp[counter++] = i;
    }
    
    // Calculate the shortest distance among the pairs found
    for(int i = 0; i < temp_array_size; i+=2) {
		if(temp[i] < min_1 || (temp[i] == min_1 && temp[i + 1] < min_2)) {
			min_1 = temp[i];
			min_2 = temp[i + 1];
		}
    }
    
    // Optimal path and cost to get there
    optimal = min_1;
    parent = min_2;
    
    // We will store the optimal path here
    result_path = (int *)malloc(sizeof(*result_path) * (cities + 1));
    result_path[cities] = 0;
    result_path[cities - 1] = 0;
    
    // Backtrack to find the full 
    for(int i = 0; i < cities - 1; i++) {
		result_path[i] = parent;
		int new_bits = bits & ~(1 << parent);
		parent = get(bits, parent, 0);
		bits = new_bits;
    }
    
    // Start and end at the starting city
    while(result_path[0] != starting_city) {
		int temp = result_path[0];
		for(int i = 1; i < cities + 1; i++) {
			result_path[i - 1] = result_path[i];
		}
		result_path[cities - 1] = temp;
    }
    result_path[cities] = starting_city;

    free(temp);
    
    // Return optimal cost
    return optimal;
}

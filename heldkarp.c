#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "helper.h"
#include "heldkarp.h"

void create(int n) {
    size = next_power_2(n);
#if cities < SMALL_CITY_SET
    arr = (s_node **)malloc(sizeof(*arr) * size);
#else
    arr = (m_node **)malloc(sizeof(*arr) * size);
#endif
}

void put(int key, int value) {
#if cities < SMALL_CITY_SET
    s_node *temp = malloc(sizeof(s_node));
#else
    m_node *temp = malloc(sizeof(m_node));
#endif
    int index = ((key >> BITS_MAX_CITIES) * 31
    		 + (key & (MAX_CITIES - 1))) & (size - 1);
    temp->key = key;
    temp->value = value;
    temp->next = arr[index];
    arr[index] = temp;
}

int get(int key) { //segmentation fault if empty node list
#if cities < SMALL_CITY_SET
    s_node *cur;
#else
    m_node cur;
#endif
    cur = arr[((key >> BITS_MAX_CITIES) * 31
    		       + (key & (MAX_CITIES - 1))) & (size - 1)];
    while (cur->next != NULL && cur->key != key) {
	cur = cur->next;
    }
    if(cur->next == NULL && cur->key != key) {
	error = 1;
    }		        
    return cur->value;
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

void set_max_distance(int n) {
    max_distance = constrain(n, 0, (1 << (SMALL_CITY_SET)) - 1);
}

void set_combinations(int n, int r) {
    
    total_combs = nCr(n, r);
    set = n;
    elements = r;
    combinations = (int **)malloc(sizeof(*combinations) * total_combs);
    
    // First case
    combinations[0] = (int *)malloc(sizeof(*combinations[0]) * elements);
    for(int i = 0; i < elements; i++) {
	combinations[0][i] = i + 1;
    }

    // Allocate and set following combinations based on the first one
    for (int i = 1; i < total_combs; i++) {
        combinations[i] = (int *)malloc(sizeof(*combinations[i]) * elements);
	
        int k = elements - 1;
        while (combinations[i - 1][k] == set - elements + k + 1) {
	    k--;
        }

	combinations[i][k] = combinations[i - 1][k] + 1;
	
	for(int j = 0; j < k; j++) {
	    combinations[i][j] = combinations[i - 1][j];
	}
	
        for (int j = k + 1; j < elements; j++) {
	    combinations[i][j] = combinations[i][k] + j - k;
        }	
    }    
    
}

void print_path(void) {
    printf("Resulting path\t: [");
    for(int i = 0; i < cities + 1; i++) {
        printf("%d ", result_path[i]);
    }
    printf("]\n");
}

void inicial_data(int cities, int max_distance,
		  int starting_city, int same_distance) {
    // Set number of cities
    set_cities(cities);
    // Set max distance beetween nodes
    set_max_distance(max_distance);
    // Create hashmap
    create((cities - 1) << (cities - 2));
    // Crate random matrix
    set_random_matrix(cities, max_distance, same_distance);
    // Set starting city
    set_starting_city(starting_city);

    // Set thread error variables
    resets = 0;
    error = 0;
    
    // Set hoomemade mutexes
    mutexes = (char *)malloc(sizeof(*mutexes) * size);
    // Set thread array
    threads = (pthread_t *)malloc(sizeof(*threads) * procs);
}

void *thread_task(void *arguments) {
    struct thread_args *args = arguments;
    int subset_size = args->iteration;
    int id = args->id;
    int interval = (int)(total_combs / procs);
    int start = interval * id;
    int end;
    if(id != procs - 1) {	
	end = interval * (id + 1);
    } else {
	end = total_combs;
    }

    // Set type of node for computation (s_node takes 2/3 of m_node space)
#if cities < SMALL_CITY_SET
    s_node *node;
#else
    m_node *node;
#endif
    
    // Iterate over all combinations - optimizations are crucial here
    for(int i = start; i < end; i++) {
	    
	// Allocate bits for all elements in subset       
	int bits = 0;
	for(int j = 0; j < subset_size; j++) {
	    bits |= 1 << combinations[i][j];
	}
	    
	// Find the shortest path to get to this subset
	for(int j = 0; j < subset_size; j++) {

	    // Set bits of previous set for each city in subset
	    int prev = bits & ~(1 << combinations[i][j]);

	    // Find all combinations to get to each city in subset
	    // This pairs have in account the cost of the previous subset
	    int temp_array_size = subset_size - 1;
	    int *temp = (int *)malloc(sizeof(*temp) * temp_array_size);
	    int counter = 0;

	    for(int k = 0; k < subset_size; k++) {
		if(k != j) {
		    int key = (prev << BITS_MAX_CITIES) | combinations[i][k];
		    node = arr[(31 * prev + combinations[i][k])
		    	      & (size - 1)];
		    
		    while (node->next != NULL && node->key != key) {
			node = node->next;
		    }

		    if(node->key != key) {
			error = 1;
		    }		   

		    temp[counter++] = (((node->value >> BITS_MAX_CITIES)
			   + matrix[combinations[i][k]][combinations[i][j]])
			  << BITS_MAX_CITIES) | combinations[i][k];
		}
	    }
	    
	    int key = (bits << BITS_MAX_CITIES) | combinations[i][j];
	    int index = ((key >> BITS_MAX_CITIES) * 31
	    		 + (key & (MAX_CITIES - 1))) & (size - 1);

	    // As soon as we get the index we "register" the thread access
	    mutexes[index] = id;

	    // We select minimun paths in the meanwhile
	    int min_0 = MAX_INTEGER;
	    for(int k = 0; k < temp_array_size; k++) {
		if(temp[k] < min_0) {
		    min_0 = temp[k];
		}
	    }

	    // Built node to add to list
	    node = malloc(sizeof(s_node));
	    node->key = key;
	    node->value = min_0;
	    node->next = arr[index];

	    // Check if any thread got this index,
	    // If it did, step back one iteration to avoid conflicts
	    if(mutexes[index] != id) {
	    	j--;
	    } else {
		arr[index] = node;
	    }
	    
	    free(temp);
	}

	// If there was an invalid read from the hashmap it means that our high
	// speed mutex messed up the threads (very low % - restart algorithm)
	if(error) {
	    pthread_exit(NULL);
	}
    }
    pthread_exit(NULL);
}

int calculate_held_karp() {
    int bits;
    int optimal;
    int parent;
    struct thread_args *args = malloc(sizeof(*args) * procs);

    // Set distances between main cities (except inicial)
    for(int i = 1; i < cities; i++) {
        put(((1 << i) << BITS_MAX_CITIES) | i, matrix[0][i] << BITS_MAX_CITIES);
    }
    
    // Iterate subsets of increasing length until we reach all cities
    for(int subset_size = 2; subset_size < cities && !error; subset_size++) {
	
	// Get combinations for a specific set
	set_combinations(cities - 1, subset_size);
	
	// Start thread tasks
	for(int i = 0; i < procs; i++) {
	    args[i].id = i;
	    args[i].iteration = subset_size;
	    pthread_create(&threads[i], NULL, thread_task, &args[i]);
	}
	
	// Wait for all threads to finish in order to continue
	for(int i = 0; i < procs; i++) {
	    pthread_join(threads[i], NULL);
	}
	
	free(combinations);
    } // performance critical part is over

    // In case of invalid writes due to thread errors, recall the function
    if(error) {
	error = 0;
	resets++;
	for(int i = 0; i < size; i++) {
	    free(arr[i]);
	}
	
	return calculate_held_karp();
    }
    
    // Fill all bits except the one corresponding to the start city
    bits = (1 << cities) - 2;
    
    int temp_array_size = cities - 1;
    int *temp = (int *)malloc(sizeof(*temp) * temp_array_size);
    
    for(int i = 1; i < cities; i++) {	
	temp[i - 1] = (get((bits << BITS_MAX_CITIES) | i) >> BITS_MAX_CITIES)
	    + matrix[i][0];
	temp[i - 1] = (temp[i - 1] << BITS_MAX_CITIES) | i;
    }

    int min_0 = MAX_INTEGER;
    for(int k = 0; k < temp_array_size; k++) {
	if(temp[k] < min_0) {
	    min_0 = temp[k];
	}
    }
    
    // Optimal path and cost to get there
    optimal = min_0 >> BITS_MAX_CITIES;
    parent = min_0 & (MAX_CITIES - 1);
    
    // We will store the optimal path here
    result_path = (int *)malloc(sizeof(*result_path) * (cities + 1));
    result_path[cities] = 0;
    result_path[cities - 1] = 0;

    // Backtrack to find the full 
    for(int i = 0; i < cities - 1; i++) {
	result_path[i] = parent;
	int new_bits = bits & ~(1 << parent);
	parent = get((bits << BITS_MAX_CITIES) | parent) & (MAX_CITIES - 1);
	bits = new_bits;
    }
    
    // Start and end at the starting city
    while(result_path[0] != starting_city) {
	int temp_res = result_path[0];
	for(int i = 1; i < cities + 1; i++) {
	    result_path[i - 1] = result_path[i];
	}
	result_path[cities - 1] = temp_res;
    }
    result_path[cities] = starting_city;

    free(temp);
    free(args);
    
    // Return optimal cost
    return optimal;
}

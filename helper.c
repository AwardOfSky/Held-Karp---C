#include <string.h>
#include "helper.h"

int constrain(int number, int min, int max) {
    if(number < min) {
	return min;
    } else if (number > max) {
	return max;
    } else {
	return number;
    }
}

int check_yes(char *str) {
    int result = 0;
    if(!strcmp(str, "y") || !strcmp(str, "yes")
       || !strcmp(str, "Y") || !strcmp(str, "YES")) {
	result = 1;
    }
    return result;
}

int nCr(int n, int r) {
    long double temp = 1;
    long double fact;
    int start;
    if(r != n) {
        if(r < n - r) {
            start = n - r + 1;
            fact = r;
        } else {
            start = r + 1;
            fact = n - r;
        }
        for(int i = start; i < n + 1; i++) {
            temp *= i;
        }
        for(int i = fact - 1; i > 1; i--) {
            fact *= i;
        }
        return temp / fact;
    } else {
        return 1;
    }
}

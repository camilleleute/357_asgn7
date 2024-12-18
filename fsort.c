/* Defines functions for sorting arrays in parallel.
 * CSC 357, Assignment 7
 * Given code, Winter '24 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "fsort.h"

/* fsort: Sorts an array using a parallelized merge sort.
 * TODO: Implement this function. It should sort the "n" elements of "base",
 *       each of "width" bytes, creating a child process to sort the latter
 *       half of the array if and only if "n > min", and returning 1 if the
 *       requisite child processes could not be created and 0 otherwise. */
int fsort(
 void *base, size_t n, size_t width, size_t min,
 int (*cmp)(const void *, const void *))
{
	return mergeSort(base, n, width, min, cmp);
}

void merge(void* base, size_t mid, size_t n, size_t width,
  	    int (*cmp)(const void *, const void *)) {
	char * arr = (char *)base;
	char * merged = malloc(n * width);
	//make 2 sub arrays


	size_t L = 0;
	size_t R = mid;
	size_t J = 0;

	while ((L < mid) && (R < n)) {
		if ((cmp(arr + (L * width), (arr + (R *width)))) <= 0) {
			memcpy(merged + J * width, arr + L * width, width);
			L++;
		} else {
			memcpy(merged + J*width, arr + R*width, width);
			R++;
		}
		J++;
	}
	
	while (L < mid) {
		memcpy(merged + J * width, arr + L * width, width);
                L++;
		J++;
	}

	while (R < n) {
		memcpy(merged + J*width, arr + R*width, width);
                R++;
		J++;
	}

	memcpy(arr, merged, n*width);
	free(merged);
} 

int mergeSort(void *base, size_t n, size_t width, size_t min,
 int (*cmp)(const void *, const void *)) {
	if (n < 2) return 0;
	size_t mid = n/2;
	if (n > min) {
		int fds[2];
		if (pipe(fds) == -1) {
        		perror("pipe");
        		return 1;
    		}
		pid_t pid = fork();
		if (pid == -1) {
			perror("fork failure");
			close(fds[0]);
			close(fds[1]);
			return 1;
		}
		
		if (pid == 0) { //child
			close(fds[0]);
			int x = mergeSort((char *)base + mid * width, n - mid, width,min, cmp);
        		if (x == 0 ) {
				write(fds[1], (char *)base + mid * width, (n - mid) * width);
        		}
			close(fds[1]);
        		exit(x);
		} else { //parent
			int status;
			close(fds[1]); 
	       		mergeSort(base, mid, width,min, cmp);
        		read(fds[0], (char *)base + mid * width, (n - mid) * width);
        		close(fds[0]);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
				 merge(base, mid, n, width, cmp);
			} else {
				return 1;
			}
		}	

	} else {
		qsort(base, n, width, cmp);
		return 0;
	}
	return 0;
}




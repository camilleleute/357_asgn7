/* Declares functions for sorting arrays in parallel.
 * CSC 357, Assignment 7
 * Given code, Winter '24
 * NOTE: Do not alter this file. */

#include <stddef.h>
void merge(void*, size_t, size_t, size_t, int (*cmp)(const void *, const void *));
int mergeSort(void *, size_t, size_t, size_t, int (*)(const void *, const void *));
int fsort(void *, size_t, size_t, size_t, int (*)(const void *, const void *));

#ifndef __SHELLSORT_H__
#define __SHELLSORT_H__

void shellsort_int(void *base, size_t num, size_t size,
                   int (*compar)(const void *, const void *));

#endif
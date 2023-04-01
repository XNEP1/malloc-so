#ifndef HEAP_MEMORY_H
#define HEAP_MEMORY_H

#ifndef NULL
    #define NULL 0
#endif

/* Funções */

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

#endif
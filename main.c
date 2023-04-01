#include <stdio.h>
#include <string.h>

#include "malloc.h"

#define TAMANHO(BLOCO) *((unsigned int *)(BLOCO-8))
#define ESTADO(BLOCO) *((unsigned int *)(BLOCO-16))

int main(int argc, char **argv)
{
    void *a = NULL;
    int i;
    for (i = 0; i < 100; i++)
    {
        a = malloc(100);
        strcpy(a, "TESTE");
        printf("%2d: %p %s %d %d\n", i, a, (char *)a, TAMANHO(a), ESTADO(a));
        free(a);
    }

    return (0);
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "malloc.h"
#include "bbrk.h"

//#define DEBUG

#define LIVRE 0x0
#define OCUPADO 0x1

#define META_SIZE 16

#define TAMANHO(BLOCO) *((unsigned int *)(BLOCO-8))
#define ESTADO(BLOCO) *((unsigned int *)(BLOCO-16))
#define PROXIMO_BLOCO(BLOCO) BLOCO+TAMANHO(BLOCO)+16


/* Variáveis Globais */

void *topoInicialHeap = NULL;
void *inicioHeap = NULL;
void *topoHeap = NULL;
void *finalLista = NULL;

/* Funções privadas */

void *criarNo(size_t num_bytes){
    if(num_bytes == 0)
        num_bytes = 1;

    void *bloco = finalLista + 16;
    TAMANHO(bloco) = num_bytes;
    ESTADO(bloco) = LIVRE;

    finalLista += num_bytes + 16;
    return bloco;
}

void *divideNo(void *bloco, size_t num_bytes){
    void *aux;
    aux = bloco;
    aux += num_bytes + META_SIZE;
    TAMANHO(aux) = TAMANHO(bloco) - num_bytes - META_SIZE;
    TAMANHO(bloco) = num_bytes;
    ESTADO(bloco) = LIVRE;
    ESTADO(aux) = LIVRE;

    return bloco;
}

/* Funções publicas */

void iniciaAlocador(){
    #ifdef DEBUG 
    write(1, "IniciaAlocador: ", sizeof("IniciaAlocador: ") - 1); 
    #endif
    topoInicialHeap = bbrk(0);
    inicioHeap = topoInicialHeap;
    topoHeap = topoInicialHeap;
    finalLista = topoInicialHeap;
    #ifdef DEBUG 
        write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
    #endif
}

void* malloc(size_t size){
    void *bloco = NULL;
    void *i;
    void *aux;

    if(topoInicialHeap == NULL)
        iniciaAlocador();

    #ifdef DEBUG 
        char k[40] = {0};
        sprintf(k, "%lu", size);
        write(1, "(", sizeof("("));
        write(1, k, sizeof(k));
        write(1, ")", sizeof(")"));
        write(1, "malloc: ", sizeof("malloc: ") - 1);
    #endif

    if(finalLista != inicioHeap){ // A heap não está vazia
        for(i=(inicioHeap + META_SIZE); i<finalLista; i=PROXIMO_BLOCO(i)){
            if((ESTADO(i) == LIVRE)){
                if(TAMANHO(i) < size)
                    continue;
                else if(TAMANHO(i) == size){
                    bloco = i;
                    break;
                }
                else if(TAMANHO(i) > (size + META_SIZE)){
                    // divide nó
                    #ifdef DEBUG 
                        sprintf(k, "%u", TAMANHO(i));
                        write(1, "Dividindo nodo de tamanho:", sizeof("Dividindo nodo de tamanho:") -1);
                        write(1, k, sizeof(k));
                        write(1, ": ", sizeof(": ") -1);
                    #endif
                    aux = i;
                    aux += size + META_SIZE;
                    TAMANHO(aux) = TAMANHO(i) - size - META_SIZE;
                    TAMANHO(i) = size;
                    ESTADO(i) = LIVRE;
                    ESTADO(aux) = LIVRE;
                    break;
                }
                else
                    continue;
            }
        }
    }
    if(bloco == NULL){
        #ifdef DEBUG 
            write(1, "gerando novo bloco: ", sizeof("gerando novo bloco ") - 1);
        #endif
        if((finalLista + size + META_SIZE) > topoHeap){ // Acabou o brk, então aloca mais.
            aux = bbrk((((size + META_SIZE)/4096)+1) * 4096);
            if(aux == (void *)-1){ // erro
                #ifdef DEBUG 
                    write(1, "finalizado com erro brk.\n", sizeof("finalizado com erro brk.\n") - 1);
                #endif
                return NULL;
            }
            else
                topoHeap = aux;
        } 

        bloco = criarNo(size);
    }

    ESTADO(bloco) = OCUPADO;
    #ifdef DEBUG 
        write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
    #endif
    return bloco;
}

void free(void* ptr){
    #ifdef DEBUG 
        write(1, "free: ", sizeof("free: ") - 1);
    #endif
    if(ptr != NULL){
        ESTADO(ptr) = LIVRE;
    }
    #ifdef DEBUG 
        write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
    #endif
}

void *calloc(size_t  nmemb, size_t  size){
    #ifdef DEBUG 
        write(1, "calloc: ", sizeof("calloc: ") - 1);
    #endif
    long unsigned int t = nmemb * size;
    void *ptr = malloc(t);
    if(ptr != NULL)
        memset(ptr, 0, size);

    #ifdef DEBUG 
        write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
    #endif
    return ptr;
}

void *realloc(void *ptr, size_t size){
    void *aux;

    #ifdef DEBUG 
        write(1, "realloc: ", sizeof("realloc: ") - 1);
    #endif
    if(ptr == NULL){
        #ifdef DEBUG 
            write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
        #endif
        return malloc(size);
    }
    else if(size == 0){
        free(ptr);
        #ifdef DEBUG 
            write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
        #endif
        return NULL;
    }

    if(TAMANHO(ptr) > size + META_SIZE){
        // divide nó
        aux = ptr;
        aux += size + META_SIZE;
        TAMANHO(aux) = TAMANHO(ptr) - size - META_SIZE;
        TAMANHO(ptr) = size;
        ESTADO(ptr) = OCUPADO;
        ESTADO(aux) = LIVRE;
        #ifdef DEBUG 
            write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
        #endif
        return ptr;
    }

    void *new_ptr = malloc(size);
    if(new_ptr == NULL){
        #ifdef DEBUG 
            write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
        #endif
        return NULL;
    }

    memcpy(new_ptr, ptr, TAMANHO(ptr));
    free(ptr);
    #ifdef DEBUG 
        write(1, "finalizado.\n", sizeof("finalizado.\n") - 1);
    #endif
    return new_ptr;
}
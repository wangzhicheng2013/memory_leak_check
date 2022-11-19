#ifndef __CMMEMORY_H__
#define __CMMEMORY_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif


void*	CMemAlloc(size_t size);
void*	CMemRealloc(void* address, size_t size);
void	CMemFree(void* address);

typedef char * DataType;
typedef struct memory_node_ {
	DataType address;
	size_t size;
	struct memory_node_ *next;
} memory_node;
typedef struct memory_linklist_ {
    memory_node *memory_head;
    pthread_mutex_t lock;
    int debug;
} memory_linklist;
int  init_memory_check(int debug);
void uninit_memory_check();
void insert_memory_node(char *addr, size_t sz);
void remove_memory_node(char *addr);
void check_memory_leak(size_t *leaked_size);
memory_linklist MemList;

#ifdef __cplusplus
}
#endif

#endif


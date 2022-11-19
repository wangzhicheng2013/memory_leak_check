#include "cmemory.h"
int init_memory_check(int debug) {
    pthread_mutex_init(&MemList.lock, NULL);
    MemList.debug = debug;
    MemList.memory_head = (memory_node *)malloc(sizeof(memory_node));
	return MemList.memory_head != NULL ? 0 : -1;
}
void uninit_memory_check() {
    pthread_mutex_destroy(&MemList.lock);
	memory_node *p = MemList.memory_head;
	while (p != NULL) {
		memory_node *tmp = p;
		p = p->next;
		free(tmp);
	}
}
void insert_memory_node(char *addr, size_t sz) {
    if (0 == MemList.debug) {
        return;
    }
	memory_node *p = (memory_node *)malloc(sizeof(memory_node));
	if (NULL == p) {
		return;
	}
	p->address = addr;
	p->size = sz;
    pthread_mutex_lock(&MemList.lock);
	if (MemList.memory_head != NULL) {
		p->next = MemList.memory_head->next;
		MemList.memory_head->next = p;
	}
    pthread_mutex_unlock(&MemList.lock);
}
void remove_memory_node(char *addr) {
    if (0 == MemList.debug) {
        return;
    }
    pthread_mutex_lock(&MemList.lock);
	if (NULL == MemList.memory_head) {
        pthread_mutex_unlock(&MemList.lock);
		return;
	}
	memory_node *p = MemList.memory_head->next;
	memory_node *pre = MemList.memory_head;
	while (p != NULL) {
		if (p->address == addr) {
			pre->next = p->next;
			free(p);
			p = pre->next;
		}
		else {
			pre = p;
			p = p->next;
		}
	}	
    pthread_mutex_unlock(&MemList.lock);
}
void check_memory_leak(size_t *leaked_size) {
	*leaked_size = 0;
	if (NULL == MemList.memory_head) {
		return;
	}
	memory_node *p = MemList.memory_head->next;
	while (p != NULL) {
		*leaked_size += p->size;
		p = p->next;
	}
}
void* CMemAlloc(size_t size) {
	char *p = (char *)malloc(size);
    if (p != NULL) {
        insert_memory_node(p, size);
    }
	return p;
}
void* CMemRealloc(void* address, size_t size) {
    char *p = (char *)realloc(address, size);
    if (p != NULL) {
        insert_memory_node(p, size);
    }
	return p;
}

void CMemFree(void* address) {
    if (address != NULL) {
        free(address);
        remove_memory_node((char *)address);
    }
}
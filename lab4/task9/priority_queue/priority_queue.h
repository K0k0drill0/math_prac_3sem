#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include "../data_structs.h"
#include "../errors.h"

typedef enum Priority_queue_type {
    PQ_BINARY,
    PQ_BINOMIAL,
    PQ_FIBONACCI,
    PQ_SKEW,
    PQ_LEFTIST,
    PQ_TREAP,

    PQ_INVALID
} Priority_queue_type;

typedef struct Priority_queue {
    void* ds;
    Priority_queue_type type;
    size_t ds_size;
    int (*set_null) (void* ds);
    int (*init) (void* ds, int (*comparator)(const Application*, const Application*) ); // TODO_ARGS
    int (*free) (void* ds);
    int (*insert) (void* ds, Application* a);
    int (*get_max) (void* ds, Application** res);
    int (*del_max) (void* ds, Application** res);
    int (*meld) (void* res, void* pq1, void* pq2);
    int (*copy_meld) (void* res, void* pq1, void* pq2);
    int (*copy) (void* dest, void* source);
    int (*size) (void* ds, unsigned int* res);
} Priority_queue;

int Priority_queue_set_null(Priority_queue* pq);
int Priority_queue_init(Priority_queue* pq, Priority_queue_type type, int (*comparator) (const Application*, const Application*));
int Priority_queue_copy(Priority_queue* dest, Priority_queue* source);
int Priority_queue_free(Priority_queue* pq);

int Priority_queue_meld(Priority_queue* res, Priority_queue* pq1, Priority_queue* pq2);
int Priority_queue_copy_meld(Priority_queue* res, Priority_queue* pq1, Priority_queue* pq2);

int Priority_queue_get_max(Priority_queue* pq, Application** res);
int Priority_queue_del_max(Priority_queue* pq, Application** res);
int Priority_queue_insert(Priority_queue* pq, Application* a);

int Priority_queue_size(Priority_queue* pq, unsigned int* res);

#endif
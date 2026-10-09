#ifndef MOCK_LIST_H
#define MOCK_LIST_H

#include <stddef.h>
#include <stdint.h>

#ifndef I32
typedef int32_t i32;
#define I32 1
#endif

typedef struct Item {
	void* data;
	size_t size;
	int type;
	struct Item* next;
} Item;

typedef struct list {
	Item* head;
	Item* tail;
	size_t count;

	/* methods as function pointers to mimic expected API */
	Item* (*createItem)(struct list*, void* data, size_t size, int type);
	void (*append)(struct list*, Item*);
	void (*prepend)(struct list*, Item*);
	void (*insertAt)(struct list*, size_t idx, Item*);
	Item* (*getAt)(struct list*, size_t idx);
	int (*removeItem)(struct list*, Item*);
	Item* (*popFront)(struct list*);
	Item* (*popBack)(struct list*);
	size_t (*size)(struct list*);
	int (*isEmpty)(struct list*);
	void (*clear)(struct list*);
	void (*destroy)(struct list*);
	void (*pprint)(struct list*);
	void (*foreach)(struct list*, void (*fn)(Item*, void*), void* ctx);
} list;

#ifdef __cplusplus
extern "C" {
#endif

/* Public constructor matching observed usage: List() */
list* List(void);

#ifdef __cplusplus
}
#endif

#endif /* MOCK_LIST_H */

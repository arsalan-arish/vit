#include "mock_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations for internal helpers */
static Item* mock_createItem(list* l, void* data, size_t size, int type);
static void mock_append(list* l, Item* it);
static void mock_prepend(list* l, Item* it);
static void mock_insertAt(list* l, size_t idx, Item* it);
static Item* mock_getAt(list* l, size_t idx);
static int mock_removeItem(list* l, Item* it);
static Item* mock_popFront(list* l);
static Item* mock_popBack(list* l);
static size_t mock_size(list* l);
static int mock_isEmpty(list* l);
static void mock_clear(list* l);
static void mock_destroy(list* l);
static void mock_pprint(list* l);
static void mock_foreach(list* l, void (*fn)(Item*, void*), void* ctx);

/* Public constructor matching observed usage: List() */
list* List(void) {
	list* l = (list*)malloc(sizeof(list));
	if (!l) return NULL;
	l->head = l->tail = NULL;
	l->count = 0;

	l->createItem = mock_createItem;
	l->append = mock_append;
	l->prepend = mock_prepend;
	l->insertAt = mock_insertAt;
	l->getAt = mock_getAt;
	l->removeItem = mock_removeItem;
	l->popFront = mock_popFront;
	l->popBack = mock_popBack;
	l->size = mock_size;
	l->isEmpty = mock_isEmpty;
	l->clear = mock_clear;
	l->destroy = mock_destroy;
	l->pprint = mock_pprint;
	l->foreach = mock_foreach;

	return l;
}

/* Implementation details */
static Item* mock_createItem(list* l, void* data, size_t size, int type) {
	(void)l;
	Item* it = (Item*)malloc(sizeof(Item));
	if (!it) return NULL;
	if (size > 0) {
		it->data = malloc(size);
		if (!it->data) { free(it); return NULL; }
		memcpy(it->data, data, size);
	} else {
		it->data = NULL;
	}
	it->size = size;
	it->type = type;
	it->next = NULL;
	return it;
}

static void mock_append(list* l, Item* it) {
	if (!it) return;
	if (!l->head) l->head = l->tail = it;
	else { l->tail->next = it; l->tail = it; }
	l->count++;
}

static void mock_prepend(list* l, Item* it) {
	if (!it) return;
	it->next = l->head;
	l->head = it;
	if (!l->tail) l->tail = it;
	l->count++;
}

static void mock_insertAt(list* l, size_t idx, Item* it) {
	if (!it) return;
	if (idx == 0) { mock_prepend(l, it); return; }
	if (idx >= l->count) { mock_append(l, it); return; }
	Item* prev = l->head;
	for (size_t i = 1; i < idx; ++i) prev = prev->next;
	it->next = prev->next;
	prev->next = it;
	l->count++;
}

static Item* mock_getAt(list* l, size_t idx) {
	Item* cur = l->head;
	size_t i = 0;
	while (cur && i < idx) { cur = cur->next; ++i; }
	return cur;
}

static int mock_removeItem(list* l, Item* it) {
	if (!l->head || !it) return 0;
	Item* cur = l->head;
	Item* prev = NULL;
	while (cur) {
		if (cur == it) {
			if (prev) prev->next = cur->next;
			else l->head = cur->next;
			if (cur == l->tail) l->tail = prev;
			if (cur->data) free(cur->data);
			free(cur);
			l->count--;
			return 1;
		}
		prev = cur;
		cur = cur->next;
	}
	return 0;
}

static Item* mock_popFront(list* l) {
	if (!l->head) return NULL;
	Item* it = l->head;
	l->head = it->next;
	if (!l->head) l->tail = NULL;
	it->next = NULL;
	l->count--;
	return it;
}

static Item* mock_popBack(list* l) {
	if (!l->head) return NULL;
	if (l->head == l->tail) {
		Item* it = l->head;
		l->head = l->tail = NULL;
		l->count--;
		it->next = NULL;
		return it;
	}
	Item* cur = l->head;
	while (cur->next != l->tail) cur = cur->next;
	Item* it = l->tail;
	cur->next = NULL;
	l->tail = cur;
	l->count--;
	it->next = NULL;
	return it;
}

static size_t mock_size(list* l) { return l->count; }
static int mock_isEmpty(list* l) { return l->count == 0; }

static void mock_clear(list* l) {
	Item* cur = l->head;
	while (cur) {
		Item* nxt = cur->next;
		if (cur->data) free(cur->data);
		free(cur);
		cur = nxt;
	}
	l->head = l->tail = NULL;
	l->count = 0;
}

static void mock_destroy(list* l) {
	if (!l) return;
	mock_clear(l);
	free(l);
}

static void mock_pprint(list* l) {
	printf("[list size=%zu]\n", l->count);
	Item* cur = l->head;
	size_t idx = 0;
	while (cur) {
		if (cur->size == sizeof(i32) && cur->type == I32) {
			i32 v = 0;
			memcpy(&v, cur->data, sizeof(v));
			printf("  [%zu] I32: %d\n", idx, (int)v);
		} else {
			printf("  [%zu] data(%zu bytes)\n", idx, cur->size);
		}
		cur = cur->next;
		++idx;
	}
}

static void mock_foreach(list* l, void (*fn)(Item*, void*), void* ctx) {
	Item* cur = l->head;
	while (cur) {
		fn(cur, ctx);
		cur = cur->next;
	}
}

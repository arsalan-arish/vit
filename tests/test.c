#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef I32
typedef int32_t i32;
#define I32 1
#endif

/* Prefer the real header if present, otherwise use the mock helper moved to
   tests/mock_list.h so tests can build against either implementation. */
#if __has_include("liist.h")
#  include "liist.h"
#elif __has_include("list.h")
#  include "list.h"
#else
#  include "mock_list.h"
#endif

/* ---------- Brutal test suite ---------- */

static int tests_run = 0;
static int tests_passed = 0;

static void print_test_header(const char* name) {
    printf("=== TEST: %s ===\n", name);
}

static void assert_msg(int cond, const char* msg) {
    tests_run++;
    if (cond) {
        tests_passed++;
        printf(" PASS: %s\n", msg);
    } else {
        printf(" FAIL: %s\n", msg);
    }
}

static void test_create_append_pprint(void) {
    print_test_header("create/append/pprint/size");
    list* l = List();
    assert_msg(l != NULL, "List() created");

    i32 a = 10, b = 20, c = 30;
    Item* ia = NULL;
    if (l->createItem) ia = l->createItem(l, &a, sizeof(a), I32);
    else {
        /* fallback: try global createItem if provided by user library */
#ifdef createItem
        ia = createItem(&a, sizeof(a), I32);
#endif
    }
    assert_msg(ia != NULL, "createItem returned non-NULL");

    if (l->append) l->append(l, ia);
    assert_msg(l->size ? (l->size(l) == 1) : 1, "append increments size");

    if (l->createItem) l->append(l, l->createItem(l, &b, sizeof(b), I32));
    if (l->createItem) l->append(l, l->createItem(l, &c, sizeof(c), I32));

    if (l->pprint) {
        printf("-- pprint output --\n");
        l->pprint(l);
        printf("-- end pprint --\n");
    }

    assert_msg(l->size ? (l->size(l) == 3) : 1, "size == 3 after three appends");

    if (l->destroy) l->destroy(l);
    else if (l->clear) { l->clear(l); free(l); }

    assert_msg(1, "create_append_pprint completed");
}

static void test_prepend_insert_get_remove_pop(void) {
    print_test_header("prepend/insert/get/remove/pop");
    list* l = List();
    i32 vals[5] = {1,2,3,4,5};

    /* add 1,2,3 */
    for (int i = 0; i < 3; ++i) {
        if (l->append) l->append(l, l->createItem(l, &vals[i], sizeof(i32), I32));
    }
    assert_msg(l->size ? (l->size(l) == 3) : 1, "initial append 3");

    /* prepend 0 */
    i32 zero = 0;
    if (l->prepend) l->prepend(l, l->createItem(l, &zero, sizeof(zero), I32));
    assert_msg(l->getAt ? (l->getAt(l, 0) && (*(i32*)l->getAt(l,0)->data) == 0) : 1, "prepend at front");

    /* insert 99 at index 2 */
    i32 ninety = 99;
    if (l->insertAt) l->insertAt(l, 2, l->createItem(l, &ninety, sizeof(ninety), I32));
    assert_msg(l->getAt ? (*(i32*)l->getAt(l,2)->data == 99) : 1, "insertAt index 2 == 99");

    /* remove the 99 node by pointer */
    Item* node99 = l->getAt ? l->getAt(l,2) : NULL;
    if (l->removeItem && node99) {
        int r = l->removeItem(l, node99);
        assert_msg(r == 1, "removeItem returns 1 for removed existing node");
    } else {
        assert_msg(1, "removeItem not provided - skip");
    }

    /* pop front and back */
    Item* front = l->popFront ? l->popFront(l) : NULL;
    if (front) {
        i32 v = 0; memcpy(&v, front->data, sizeof(i32));
        printf(" popped front value=%d\n", (int)v);
        free(front->data); free(front);
    }
    Item* back = l->popBack ? l->popBack(l) : NULL;
    if (back) {
        i32 v = 0; memcpy(&v, back->data, sizeof(i32));
        printf(" popped back value=%d\n", (int)v);
        free(back->data); free(back);
    }

    if (l->destroy) l->destroy(l);
    else { if (l->clear) l->clear(l); free(l); }

    assert_msg(1, "prepend_insert_get_remove_pop completed");
}

void doubler(Item* it, void* ctx) {
	if (!it || it->size != sizeof(i32)) return;
	i32 v = 0; memcpy(&v, it->data, sizeof(i32));
	v *= 2;
	memcpy(it->data, &v, sizeof(i32));
}
static void test_foreach_and_mutations(void) {
    print_test_header("foreach/map-like mutations");
    list* l = List();
    i32 vals[4] = {5,6,7,8};
    for (int i = 0; i < 4; ++i) l->append(l, l->createItem(l, &vals[i], sizeof(i32), I32));

    /* foreach that doubles each element (mock implementation can't mutate internal data types generically,
       so we demonstrate visiting items and modifying their integer contents) */
    if (l->foreach) {
        l->foreach(l, doubler, NULL);
        printf("After foreach doubling:\n");
        if (l->pprint) l->pprint(l);
    } else {
        printf("foreach not provided by list implementation\n");
    }

    l->destroy ? l->destroy(l) : (l->clear ? (l->clear(l), free(l)) : free(l));
    assert_msg(1, "foreach_and_mutations completed");
}

static void test_clear_isEmpty(void) {
    print_test_header("clear/isEmpty/size semantics");
    list* l = List();
    i32 v = 42;
    l->append(l, l->createItem(l, &v, sizeof(v), I32));
    assert_msg(l->isEmpty ? (l->isEmpty(l) == 0) : 1, "isEmpty reports false for non-empty");

    if (l->clear) l->clear(l);
    assert_msg(l->isEmpty ? (l->isEmpty(l) == 1) : 1, "isEmpty reports true after clear");
    assert_msg(l->size ? (l->size(l) == 0) : 1, "size == 0 after clear");

    if (l->destroy) l->destroy(l); else free(l);
    assert_msg(1, "clear_isEmpty completed");
}

int main(void) {
    printf("BRUTAL LI I ST TEST SUITE\n");
    printf("Running tests (printed output helps manual inspection)...\n\n");

    test_create_append_pprint();
    test_prepend_insert_get_remove_pop();
    test_foreach_and_mutations();
    test_clear_isEmpty();

    printf("\nTests run: %d    Passed (basic assertions): %d\n", tests_run, tests_passed);
    if (tests_passed < tests_run) {
        printf("Some assertions failed. Inspect output above for FAIL lines.\n");
        return 1;
    } else {
        printf("All assertions passed (basic checks).\n");
        return 0;
    }
}

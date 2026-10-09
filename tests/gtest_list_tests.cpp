#include <gtest/gtest.h>
#include <cstring>

#if __has_include("liist.h")
#  include "liist.h"
#elif __has_include("list.h")
#  include "list.h"
#else
#  include "mock_list.h"
#endif

static int get_i32(Item* it) {
	if (!it || !it->data) return 0;
	i32 v = 0;
	memcpy(&v, it->data, sizeof(v));
	return (int)v;
}

TEST(ListBasic, CreateAppendSizeGetAt) {
	list* l = List();
	ASSERT_NE(l, nullptr);
	EXPECT_EQ(l->size(l), 0u);

	i32 a = 10, b = 20, c = 30;
	Item* ia = l->createItem(l, &a, sizeof(a), I32);
	Item* ib = l->createItem(l, &b, sizeof(b), I32);
	Item* ic = l->createItem(l, &c, sizeof(c), I32);

	l->append(l, ia);
	l->append(l, ib);
	l->append(l, ic);

	EXPECT_EQ(l->size(l), 3u);
	EXPECT_EQ(get_i32(l->getAt(l, 0)), 10);
	EXPECT_EQ(get_i32(l->getAt(l, 1)), 20);
	EXPECT_EQ(get_i32(l->getAt(l, 2)), 30);

	l->clear(l);
	l->destroy(l);
}

TEST(ListInsertPrepend, InsertAtAndPrepend) {
	list* l = List();
	ASSERT_NE(l, nullptr);

	i32 a = 1, b = 2, c = 3, d = 4;
	Item* ia = l->createItem(l, &a, sizeof(a), I32);
	Item* ib = l->createItem(l, &b, sizeof(b), I32);
	Item* ic = l->createItem(l, &c, sizeof(c), I32);
	Item* id = l->createItem(l, &d, sizeof(d), I32);

	l->append(l, ia); // [1]
	l->append(l, ib); // [1,2]
	l->insertAt(l, 1, ic); // [1,3,2]
	l->prepend(l, id); // [4,1,3,2]

	EXPECT_EQ(l->size(l), 4u);
	EXPECT_EQ(get_i32(l->getAt(l, 0)), 4);
	EXPECT_EQ(get_i32(l->getAt(l, 1)), 1);
	EXPECT_EQ(get_i32(l->getAt(l, 2)), 3);
	EXPECT_EQ(get_i32(l->getAt(l, 3)), 2);

	l->clear(l);
	l->destroy(l);
}

TEST(ListRemovePop, RemoveAndPopEdgeCases) {
	list* l = List();
	ASSERT_NE(l, nullptr);

	// operations on empty list
	EXPECT_EQ(l->popFront(l), nullptr);
	EXPECT_EQ(l->popBack(l), nullptr);
	EXPECT_EQ(l->removeItem(l, nullptr), 0);

	i32 vals[5] = {10,20,30,40,50};
	Item* items[5];
	for (int i=0;i<5;++i) items[i] = l->createItem(l, &vals[i], sizeof(i32), I32);
	for (int i=0;i<5;++i) l->append(l, items[i]);

	EXPECT_EQ(l->size(l), 5u);

	// remove middle
	EXPECT_EQ(l->removeItem(l, items[2]), 1);
	EXPECT_EQ(l->size(l), 4u);
	// remove head
	EXPECT_EQ(l->removeItem(l, items[0]), 1);
	EXPECT_EQ(l->size(l), 3u);
	// remove tail
	EXPECT_EQ(l->removeItem(l, items[4]), 1);
	EXPECT_EQ(l->size(l), 2u);

	// remove non-present
	Item fake = {0};
	EXPECT_EQ(l->removeItem(l, &fake), 0);

	// pop front/back
	Item* pf = l->popFront(l);
	EXPECT_NE(pf, nullptr);
	if (pf) { free(pf->data); free(pf); }
	Item* pb = l->popBack(l);
	EXPECT_NE(pb, nullptr);
	if (pb) { free(pb->data); free(pb); }

	EXPECT_EQ(l->size(l), 0u);

	l->destroy(l);
}

TEST(ListForeachPprint, ForeachAndPprintAndZeroSizeItem) {
	list* l = List();
	ASSERT_NE(l, nullptr);

	// zero-size item
	Item* zero = l->createItem(l, nullptr, 0, 0);
	l->append(l, zero);
	EXPECT_EQ(l->getAt(l,0)->size, 0u);

	// add integers
	i32 a=5,b=7;
	l->append(l, l->createItem(l, &a, sizeof(a), I32));
	l->append(l, l->createItem(l, &b, sizeof(b), I32));

	int sum = 0;
	l->foreach(l, [](Item* it, void* ctx){
		int* p = (int*)ctx;
		if (it->data && it->size == sizeof(i32)) {
			i32 v=0; memcpy(&v, it->data, sizeof(v)); *p += (int)v;
		}
	}, &sum);

	EXPECT_EQ(sum, 12);

	// call pprint (should not crash)
	l->pprint(l);

	l->destroy(l);
}

TEST(ListEdgeCases, InsertOutOfBoundsAndNulls) {
	list* l = List();
	ASSERT_NE(l, nullptr);

	// insert at large index -> append
	i32 x = 99;
	Item* it = l->createItem(l, &x, sizeof(x), I32);
	l->insertAt(l, 1000, it);
	EXPECT_EQ(l->size(l), 1u);
	EXPECT_EQ(get_i32(l->getAt(l,0)), 99);

	// appending null should be no-op
	l->append(l, nullptr);
	EXPECT_EQ(l->size(l), 1u);

	// prepend null should be no-op
	l->prepend(l, nullptr);
	EXPECT_EQ(l->size(l), 1u);

	// createItem with NULL data and size 0
	Item* z = l->createItem(l, nullptr, 0, 0);
	l->append(l, z);
	EXPECT_EQ(l->getAt(l,1)->data, nullptr);

	l->destroy(l);
}

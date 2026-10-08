#include <types.h>
#include <vit/subsystems/linked_list.h>

void test_linked_list(void) {
    list* test = List();
    int x = 10;
    test->append(test, test->createItem(&x, sizeof(x)));
    test->pprint(test);
}

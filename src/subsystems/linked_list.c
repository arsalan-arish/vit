#include <stdlib.h>
#include <string.h>

#include <types.h>
#include <vit/subsystems/linked_list.h>


list List(void) {
    return (list) {0};
}


node* list__createNode(u64 dataLen, void** data_buffer) {
    node* new = malloc(sizeof(node));
    if (!new) return new;

    *data_buffer = malloc(dataLen);
    if (!(*data_buffer)) return new;

    new->item.data = data_buffer;
    new->item.dataLen = dataLen;
    return new;
}

item list_append(list* list, u64 dataLen) {

    // create node & data buffer
    void* data_buffer;
    node* new = list__createNode(dataLen, &data_buffer);
    if (!new || !data_buffer) list->err.code = LIST_INDEX_OUT_OF_RANGE; return (item) {0};
    new->next = null;

    // Attach the node
    if (!list->len) {
        list->first = new;
        new->prev = null;
    } else {
        list->last->next = new;
        new->prev = list->last;
    }
    list->last = new;
    
    // update metadata
    list->len++;

    return (item) {
        .data = data_buffer,
        .dataLen = dataLen
    };
}

item list_prepend(list* list, u64 dataLen) {
    
    // create node & data buffer
    void* data_buffer;
    node* new = list__createNode(dataLen, &data_buffer);
    if (!new || !data_buffer) list->err.code = LIST_INDEX_OUT_OF_RANGE; return (item) {0};
    new->prev = null;
    
    // Attach the node
    if (!list->len) {
        list->last = new;
        new->next = null;
    } else {
        new->next = list->first;
        list->first->prev = new;
    }
    list->first = new;

    // update metadata
    list->len++;

    return (item) {
        .data = data_buffer,
        .dataLen = dataLen
    };
}

item list_insert(list* list, u64 dataLen, i64 index, b8 replace) {
    /* Insert is only allowed within the current range (< len), or equal to len, in which case it will be appended */
    if (index >= 0) {
        if (index == list->len) return list_append(list, dataLen);
    } else {
        if (list_getInvertedIndex(list, index) == list->len) return list_append(list, dataLen);
    }

    node* currentNode = list__findNodeByIndex(list, index);
    if (list->err.is) return (item) {0};
    
    void* data_buffer;
    node* new = list__createNode(dataLen, &data_buffer);
    if (!new || !data_buffer) list->err.code = LIST_INDEX_OUT_OF_RANGE; return (item) {0};

    // Override currentNode / push it forward depending on 'replace' parameter
    if (replace) {
        new->next = currentNode->next;
        new->prev = currentNode->prev;

        if (!currentNode->prev || !currentNode->next)
            list->first = new;
        else 
            currentNode->prev->next = new;

        //! Old code something fishy ?? Anyways a simplified form is the above 4 lines
        /*
        currentNode->prev == null?
                ({list->first = new;})
            :   ({currentNode->prev->next = new;});
        currentNode->next == null?
                ({list->first = new;})
            :   ({currentNode->prev->next = new;});
        */
        //! ?
   } else {
        if (!currentNode->prev) list->first = new; 
        new->next = currentNode;
        new->prev = currentNode->prev;
        currentNode->prev = new;
    }

    // update list metadata
    list->len++;

    return (item) {
        .data = data_buffer,
        .dataLen = dataLen
    };
}

item list_pop(list* list, b8 toReturn) {
    /* If toReturn is true, the user will have to manually call free_item() function to free the item once it has been used */

    if (!list->len) list->err.code = LIST_POP_FROM_EMPTY_LIST; return (item) {0};
    
    // Rewire
    node* toRemove = list->last;
    list__extractNodeFromList(list, toRemove);
    
    // update metadata
    list->len--;

    item ret;
    if (toReturn) {
        ret = list__extractItemFromNode(toRemove);
    } else {
        ret = (item) {0};
    }
    list__free_node(toRemove);

    return ret;
}

item list_popIndex(list* list, i32 index, b8 toReturn) {
    /* If toReturn is true, the user will have to manually call free_item() function to free the item once it has been used */

    node* toRemove = list__findNodeByIndex(list, index);
    if (list->err.is) return (item) {0};

    // Update metadata
    list->len--;
    
    item ret;
    if (toReturn) {
        ret = list__extractItemFromNode(toRemove);
    } else {
        ret = (item) {0};
    }
    list__free_node(toRemove);

    return ret;
}

void list_removeValue(list* list, void* data, u64 dataLen) {
    // Find the node
    node* currentNode = list->first;
    for (usize i = 0; i < list->len; i++) {
        if (dataLen != currentNode->item.dataLen) continue;
        if (!memcmp(currentNode->item.data, data, dataLen)) break;
        currentNode = currentNode->next;
    }
    // If value not found
    if (!currentNode) list->err.code = LIST_VALUE_NOT_FOUND; return;

    list__extractNodeFromList(list, currentNode);
    list__free_node(currentNode);

    // Update metadata
    list->len--;
}

void list_removeIndex(list* list, i64 index) {
    node* node = list__findNodeByIndex(list, index);
    if (list->err.is) return;

    list__extractNodeFromList(list, node);
    list__free_node(node);

    // Update metadata
    list->len--;
}

item list_get(list* list, i64 index) {

    node* currentNode = list__findNodeByIndex(list, index);
    if (list->err.is) return (item) {0};
    
    return currentNode->item;
}

u64 list_search(list* list, void* data, u64 dataLen) {
    /* O(n) Linear Search */
    node* currentNode = list->first;
    for (usize i = 0; i < list->len; i++) {
        if (currentNode->item.dataLen != dataLen) continue;
        if (!memcmp(currentNode->item.data, data, currentNode->item.dataLen)) return i;
        currentNode = currentNode->next;
    }
    return -1;
}

isize list_getInvertedIndex(list* list, i64 index) {
    return index > 0 ? - (list->len - index) : list->len + index;
}

isize list__optimizeIndex(list* list, i64 index) {
    // Index optimization; if index is closer to the opposite side of the list (2 sides because -ive indexing is supported), invert it so that the list is traversed from the closer side, saving compute & time.
    if ((index > 0 && index > list->len / 2) || (index < -1 && llabs(index) > list->len / 2))
        return list_getInvertedIndex(list, index);   
    return index;
}

void list__extractNodeFromList(list* list, node* node) {
    // Unwire the node
    if (!node->prev) {
        list->first = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (!node->next) {
        list->last = node->prev;
    } else {
        node->next->prev = node->prev;
    }
}

item list__extractItemFromNode(node* node) {
    // We do not need the item separately, because each node has one item only
    item ret = node->item;
    node->item = (item) {0};
    return ret;
}

void list_free_item(item item) {
    if (!item.data) return; // Basically if the item struct is all 0 padded, return. Also if item.data is a nullptr, return; 
    free(item.data);
}

void list__free_node(node *node) {
    if (!node) return;
    list_free_item(node->item);
    free(node);
}

void list_free_list(list* list) {
    // Free all the nodes from the heap
    if (!list || !list->len) return;
    node* currentNode = list->first;
    node* nextNode;
    for (usize i = 0; i < list->len; i++) {
        nextNode = currentNode->next;
        list__free_node(currentNode);
        currentNode = nextNode;
    }
}


node* list__findNodeByIndex(list* list, i64 index) {

    list__checkIndexInRange(list, index);
    if (list->err.is) return null;

    index = list__optimizeIndex(list, index);
    node* currentNode;
    if (index >= 0) {
        // Traverse the list
        currentNode = list->first;
        for (usize i = 0; i < index; i++) {
            currentNode = currentNode->next;
        }
    } else {
        // Traverse the list from the end
        currentNode = list->last;
        for (isize i = -1; i > index; i--) {
            currentNode = currentNode->prev;
        }
    }

    return currentNode;
}

void list__checkIndexInRange(list* list, i64 index) {
    // Check if index out of range
    if (index >= 0) {
        if (!(index < list->len)) list->err.code = LIST_INDEX_OUT_OF_RANGE; return;
    } else {
        if (!(llabs(index+1) < list->len)) list->err.code = LIST_INDEX_OUT_OF_RANGE; return;
    }
}
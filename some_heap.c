
#include <stdio.h>
#include <stdlib.h>

#include "some_heap.h"

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity) {
    heap_t *minHeap = (heap_t*)malloc(sizeof(heap_t));
    minHeap->capacity = capacity;
    return minHeap;
}

void heap_free(heap_t *heap) {
    free(heap);
}

unsigned int heap_size(heap_t *heap) {
    return heap->size;
}

unsigned int heap_parent(unsigned int index) {
    if ((ceil(index / 2.0) - 1) < 0) {
        return 0;
    }
    return ceil(index / 2.0) - 1;
}

unsigned int heap_left_child(unsigned int index) {
    return index * 2 + 1;
}

unsigned int heap_right_child(unsigned int index) {
    return index * 2 + 2;
}

unsigned int heap_level(unsigned int index) {
    // base
    if (index == 0) {
        return 0;
    }
    // recursive
    return heap_level(heap_parent(index)) + 1;
}

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_value_t tmp = heap->data[index1].value;
    heap->data[index1].value = heap->data[index2].value;
    heap->data[index2].value = tmp;
}

void heap_bubble_up(heap_t *heap, int index) {
    unsigned int parent_index = heap_parent(index);
    heap_value_t parentVal = heap->data[parent_index].value;

    while (index > 0 && heap->data[parent_index].value > heap->data[index].value) {
        heap_swap(heap, parent_index, index);
        index = parent_index;
        parent_index = heap_parent(index);
    }
}

void heap_bubble_down(heap_t *heap, int index) {
    unsigned int left_child_index = heap_left_child(index);
    unsigned int right_child_index = heap_right_child(index);
    
    while (index < heap->size && (heap->data[index].value > heap->data[left_child_index].value || heap->data[index].value > heap->data[right_child_index].value)) {
        if (heap->data[left_child_index].value > heap->data[right_child_index].value) {
            heap_swap(heap, index, right_child_index);
            index = right_child_index;
        } else {
            heap_swap(heap, index, left_child_index);
            index = left_child_index;
        }
        left_child_index = heap_left_child(index);
        right_child_index = heap_right_child(index);
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity) {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return NULL;
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}

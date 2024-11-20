
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "some_heap.h"

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity)
{
    heap_t *minHeap = (heap_t *)malloc(sizeof(heap_t));
    minHeap->capacity = capacity;
    minHeap->size = 0;

    minHeap->data = (heap_node_t *)malloc(capacity * sizeof(heap_node_t));

    for (int idx = 0; idx < capacity; idx++)
    {
        minHeap->data[idx].key = 0;
    }

    return minHeap;
}

void heap_free(heap_t *heap)
{
    if (heap)
    {
        free(heap->data);
        free(heap);
    }
}

unsigned int heap_size(heap_t *heap)
{
    return heap->size;
}

unsigned int heap_parent(unsigned int index)
{
    if (index == 0)
    {
        return 0;
    }
    return (index - 1) / 2;
}

unsigned int heap_left_child(unsigned int index)
{
    return index * 2 + 1;
}

unsigned int heap_right_child(unsigned int index)
{
    return index * 2 + 2;
}

unsigned int heap_level(unsigned int index)
{
    // base
    if (index == 0)
    {
        return 0;
    }
    // recursive
    return heap_level(heap_parent(index)) + 1;
}

void heap_print(heap_t *heap)
{
    for (int ix = 0; ix < heap_size(heap); ix++)
    {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2)
{
    heap_node_t tmp = heap->data[index1];
    heap->data[index1] = heap->data[index2];
    heap->data[index2] = tmp;
}

void heap_bubble_up(heap_t *heap, int index)
{
    int curVal = heap->data[index].value.as_int;
    unsigned int parentIdx = heap_parent(index);
    int parentVal = heap->data[parentIdx].value.as_int;

    while (index > 0 && parentVal > curVal)
    {
        heap_swap(heap, parentIdx, index);
        index = parentIdx;
        curVal = heap->data[index].value.as_int;
        parentIdx = heap_parent(index);
        parentVal = heap->data[parentIdx].value.as_int;
    }
}

void heap_bubble_down(heap_t *heap, int index)
{
    while (index < heap->size)
    {
        unsigned int leftChildIdx = heap_left_child(index);
        unsigned int rightChildIdx = heap_right_child(index);

        unsigned long long curVal = heap->data[index].value.as_int;
        unsigned long long leftChildVal = (leftChildIdx < heap->size) ? heap->data[leftChildIdx].value.as_int : ULLONG_MAX;
        unsigned long long rightChildVal = (rightChildIdx < heap->size) ? heap->data[rightChildIdx].value.as_int : ULLONG_MAX;

        if (curVal <= leftChildVal && curVal <= rightChildVal)
        {
            break;
        }

        if (leftChildVal < rightChildVal)
        {
            heap_swap(heap, index, leftChildIdx);
            index = leftChildIdx;
        }
        else
        {
            heap_swap(heap, index, rightChildIdx);
            index = rightChildIdx;
        }
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data)
{
    if (heap_size(heap) == heap->capacity)
    {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}

heap_value_t heap_remove_min(heap_t *heap)
{
    if (heap_size(heap) == 0)
    {
        return (heap_value_t){.as_int = (unsigned long long)KEY_NOT_PRESENT};
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}

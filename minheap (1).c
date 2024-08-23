/*
 * Our Priority Queue implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include "minheap.h"
#include <limits.h>
#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/*
 * Swaps contents of heap->arr[index1] and heap->arr[index2] if both
 * 'index1' and 'index2' are valid indices for minheap 'heap'.
 * Has no effect otherwise.
 */
void swap(MinHeap* heap, int index1, int index2){
    // Check for valid indices
    if (index1 >= 1 && index1 <= heap->size &&
        index2 >= 1 && index2 <= heap->size) {
        // Swap the nodes at index1 and index2
        // update arr and indexMap as well
        HeapNode temp = heap->arr[index1];
        heap->arr[index1] = heap->arr[index2];
        heap->arr[index2] = temp;
        heap->indexMap[heap->arr[index1].id] = index1;
        heap->indexMap[heap->arr[index2].id] = index2;
    }
}
/*
 * Floats up the element at index 'nodeIndex' in minheap 'heap' such that
 * 'heap' is still a minheap.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
void floatUp(MinHeap* heap, int nodeIndex){
    while (nodeIndex > 0 ) {
        int parentIndex = (nodeIndex/2);
        if (heap->arr[nodeIndex].priority < heap->arr[parentIndex].priority) {
            // Swap current node with parent
            swap(heap, nodeIndex, parentIndex);
            // Update nodeIndex to parent index
            nodeIndex = parentIndex;
        } else {
            break;
        }
    }
}

/*
 * Returns the index of the left child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getLeftChildIdx(int nodeIndex){
    return 2 * nodeIndex;
}

/*
 * Returns the index of the right child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getRightChildIdx(int nodeIndex){
    return 2 * nodeIndex + 1;
}
/*
 * Returns the index of the parent of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getParentIdx(int nodeIndex){
    return (nodeIndex / 2);
}

/*
 * Returns True if 'nodeIndex' is currently a valid index in minheap 'heap'.
 * False otherwise.
 */
bool isValidIndex(MinHeap* heap, int nodeIndex){
    return (nodeIndex > 0 && nodeIndex <= heap->size);
}
/*
 * Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex){
    return heap->arr[nodeIndex].priority;
}
/*
 * Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex){
    //if (isValidIndex(heap, nodeIndex)) {
        //return heap->arr[nodeIndex];
    //}
    return heap->arr[nodeIndex];
}
/*
 * Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap* heap, int nodeIndex){
    return heap->arr[nodeIndex].id;
}

/*
 * Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap* heap, int id){
    int j=0;
    if (heap->size > 0) {
        for (int i = 1; i <= heap->size; i++) {
            if (heap->arr[i].id == id) {
                j = i;
            }
        }
    }
    return j;
}

/*********************************************************************
 * Required functions
 ********************************************************************/
HeapNode getMin(MinHeap* heap)
{
    return heap->arr[1];
}

void heapify(MinHeap* heap, int nodeIndex)
{
    // special case for heap of size 1
    if(heap->size == 1){
        heap->indexMap[heap->arr[ROOT_INDEX].id] = ROOT_INDEX;
        return;
    }
    int leftChildIndex = getLeftChildIdx(nodeIndex);
    int rightChildIndex = getRightChildIdx(nodeIndex);
    int smallest = nodeIndex;
    // Find the smallest among nodeIndex, left child and right child
    if (leftChildIndex <= heap->size &&
        heap->arr[leftChildIndex].priority < heap->arr[smallest].priority) {
        smallest = leftChildIndex;
    }
    if (rightChildIndex <= heap->size &&
        heap->arr[rightChildIndex].priority < heap->arr[smallest].priority) {
        smallest = rightChildIndex;
    }
    //smallest not current node, swap, recursion
    if (smallest != nodeIndex) {
        swap(heap, nodeIndex, smallest);
        heapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap)
{
    HeapNode min = heap->arr[ROOT_INDEX];
    heap->arr[ROOT_INDEX] = heap->arr[heap->size];
    heap->indexMap[heap->arr[ROOT_INDEX].id]=ROOT_INDEX;
    heap->size--;// Reduce heap size
    heapify(heap,ROOT_INDEX);// Restore heap property using heapify
    return min;

}

bool insert(MinHeap* heap, int priority, int id)
{
    // New node with given priority, id
    HeapNode newNode = {priority, id};
    // new node at end of heap
    heap->size++;
    heap->arr[heap->size] = newNode;
    heap->indexMap[id] = heap->size;
    floatUp(heap, heap->size);
    return true;
}

int getPriority(MinHeap* heap, int id)
{
    if (heap != NULL && id >= 0 && id < heap->capacity) {
        int index = heap->indexMap[id];
        if (index > 0 && index <= heap->size) {
            return heap->arr[index].priority;
        }
    }
    return -1;
}

bool decreasePriority(MinHeap* heap, int id, int newPriority)
{
    if (heap == NULL || id < 0 || id >= heap->capacity || heap->indexMap[id] == -1) {
        return false;
    }
    int index = heap->indexMap[id];
    if ( 1<= index && index <= heap->size && getPriority(heap,id)> newPriority){
        heap->arr[index].priority = newPriority;
        floatUp(heap, index);
        return true;
    }
    return false;
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/
void printHeap(MinHeap* heap)
{
    printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
           heap->capacity);
    printf("index: priority [ID]\t ID: index\n");
    for (int i = 0; i < heap->capacity; i++)
        printf("%d: %d [%d]\t\t%d: %d\n", i, heap->arr[i].priority,
               heap->arr[i].id, i, heap->indexMap[i]);
    printf("%d: %d [%d]\t\t\n", heap->capacity,
           heap->arr[heap->capacity].priority, heap->arr[heap->capacity].id);
    printf("\n\n");
}

/***** Memory management (sample solution) **********************************/
MinHeap* newHeap(int capacity)
{
    MinHeap* heap = (MinHeap*) calloc(1,sizeof(MinHeap));
    if (!heap) {
        return NULL; // Memory allocation failed
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->arr = (HeapNode*) calloc((capacity + 1), sizeof(HeapNode));
    heap->indexMap = (int*) malloc((capacity + 1) * sizeof(int));
    if (heap->arr == NULL || heap->indexMap == NULL) {
        free(heap->arr);
        free(heap->indexMap);
        free(heap);
        return NULL;
    }
    for (int i = 0; i <= capacity; i++) {
        heap->indexMap[i] = -1;
    }
    return heap;
}

void deleteHeap(MinHeap* heap)
{
    if (heap) {
        // Free the array with heap's nodes
        if (heap->arr) {
            free(heap->arr);
        }
        if(heap->indexMap){
            free(heap->indexMap);
        }
        // Free heap structure
        free(heap);
    }
}

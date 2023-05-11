// Matthew Tran
// Project 5: Being up a Tree, You’re in a Heap of Trouble
// Week 1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"

// creating a structure of pquue
// the structure will have an array and keep track of the count and length
typedef struct pqueue
{
    void **data;
    int count;
    int length;
    int (*compare)();
} PQ;

// creating the queue
// initializing all of the variables
// allocating memory for the pqueue and the array
// O(1)
PQ *createQueue (int(*compare)())
{
    PQ *priorityQueue;
    priorityQueue = malloc(sizeof(PQ));
    assert(priorityQueue != NULL);
    priorityQueue->count = 0;
    priorityQueue->length = 10;
    priorityQueue->compare = compare;
    priorityQueue->data = malloc(sizeof(void*) * priorityQueue->length);
    assert(priorityQueue->data != NULL);
    return priorityQueue;
}

// destroying the array and the pqueue itself
// O(1)
void destroyQueue(PQ *pq)
{
    assert(pq != NULL);
    free(pq->data);
    free(pq);
}


// returns the count, which keeps track of the number of entries
// O(1)
int numEntries(PQ *pq)
{
    return pq->count;
}

// returns left child index
// O(1)
int leftChild(int i)
{
    int ret;
    ret = i*2 + 1;
    return ret;
}

// returns right child index
// O(1)
int rightChild(int i)
{
    int ret;
    ret = i*2 + 2;
    return ret;
}

// Will swap the things inside of the array at the 
// specified indexes
// O(1)
void swap(PQ *pq, int i, int j)
{
    void *temp;
    temp = pq->data[i];
    pq->data[i]=pq->data[j];
    pq->data[j]=temp;
}

// adds entry and will resort the binary heap so that it 
// keeps it a priority queue
// if there is no more space in the array, it will
// reallocate the array but will double the memory
// using a while loop, it will swap with its parent if it is smaller
// O(h) or O(logn)
void addEntry(PQ *pq, void *entry)
{
    
    if (pq->count == pq->length)
    {
        pq->length = pq->length*2;
        pq->data = realloc(pq->data,sizeof(void*)*pq->length);
    }
    
    pq->data[pq->count]=entry;
    pq->count++;
    
    int i = pq->count-1;
    while (i>0 && (*pq->compare)(pq->data[i],pq->data[(i-1)/2]) < 0)
    {
        swap(pq,i,(i-1)/2);
        i = (i-1)/2;
    }
}

// A function that I wrote for the remove function
// recurses through and does the following:
    // check if left child exists. If it does not then stop
    // check right child. if does, see which is smaller
    // See if the child is smaller than its parent
    // if it is, swap
    // recurse again
    // O(h) or O(logn)
void organizeAll(PQ *pq, int i)
{
    if(leftChild(i) >= pq->count) // base case
    {
        return;
    }

    int smallerIndex;
    smallerIndex = leftChild(i);

    if(rightChild(i) < pq->count) 
    {
        if((*pq->compare)(pq->data[leftChild(i)],pq->data[rightChild(i)]) >= 0)
        {
            smallerIndex = rightChild(i);
        }
    }

    if((*pq->compare)(pq->data[i],pq->data[smallerIndex]) < 0)
    {
        return;
    }
    swap(pq,smallerIndex,i);

    organizeAll(pq, smallerIndex);
}


// function that will remove the top element, which is the smallest
// swaps with the last element
// calls organizeAll to resort the binary heap to maintain 
// priority queue
// O(h) or O(logn)
void *removeEntry(PQ *pq)
{
    void *ret;
    ret = pq->data[0];

    //swap 
    swap(pq,0,pq->count - 1);
    pq->count--;
    organizeAll(pq,0);
    return ret;

    /* OLD ITERATIVE ATTEMPT
    int i =0;
    void *ret;
    ret = pq->data[i];
    
    pq->data[0] = pq->data[pq->count -1];
    pq->count--;

    while((2*i)+1 < pq->count)
    {
        void *smaller;

        if((2*i)+2 < pq->count)
        {
            if((*pq->compare)(pq->data[(2*i)+1],pq->data[(2*i)+2]) > 0)
            {
                smaller = pq->data[(2*i)+2];
                i = (2*i)+2;
            }else{
                smaller = pq->data[(2*i)+1];
                i = (2*i)+1;
            }
        }else{
            smaller = pq->data[(2*i)+1];
            i = (2*i)+1;
        }
        

        if((*pq->compare)(smaller,pq->data[(i-1)/2]) < 0)
        {
            void *temp;
            temp = pq->data[(i-1)/2];
            pq->data[(i-1)/2] = smaller;
            smaller = temp;
        }
        
    }
    return ret;
    */
}
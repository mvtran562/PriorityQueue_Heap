// Matthew Tran
// Huffman Coding Project Week 5

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "pack.h"
#include "pqueue.h"

typedef struct node NODE;

// Depth function
// This will return how many connections there are from node to node
int depth(NODE *curr)
{
    if(curr->parent == NULL)
    {
        return 0;
    }

    return 1 + depth(curr->parent);
}

// compare function
// This is my compare function which will 
// return the difference of the counts
int compare(NODE *first, NODE *second)
{
    return first->count - second->count;
}

// function that makes a parent node with the specified count, left node, and right
// if it is a leaf, it will not try and set the parent nodes for the left and right
NODE *makeNode(int count, NODE *left, NODE *right)
{
    NODE *newNode;
    newNode = malloc(sizeof(NODE));
    assert(newNode != NULL);
    newNode->count = count;
    newNode->parent = NULL;

   
    if (left != NULL && right != NULL)
    {
        //newNode->count = left->count + right->count;
        left->parent = newNode;
        right->parent = newNode;
    }
    return newNode;
}

// Destroys the nodes from the myLeaves array
void destroyNode(NODE *arr[257])
{
    int i;
    for(i=0;i<257;i++)
    {
        if(arr[i] != NULL)
        {    
            free(arr[i]);
        }
    }
}

int main (int argc, char *argv[])
{

    // Making sure all of the args are ok
    if(argc != 3)
    {
        printf("args not correct\n");
        exit(1);
    }
    FILE *toCount;
    toCount = fopen(argv[1],"r");
    if(toCount == NULL)
    {
        printf("file not read\n");
        exit(1);
    }
    

    


    // creating a priority queue
    PQ *pq = createQueue(compare);

    // make an array of the leaves
    NODE *myLeaves[257];
    // array of these counts;
    int numLetters[257];
    
    // Initialize by filling with NULL
    int i;
    for (i = 0; i<257;i++)
    {
        myLeaves[i] = NULL;
        numLetters[i]= 0;
    }
    
    // stores frequency in the counts arrays
    int c;
    while ((c=getc(toCount))!= EOF)
    {
        numLetters[c]++;
    }

    // fill node array with the leaves and add the leaves to a 
    // priority queue
    int j;
    for (j=0; j<256;j++)
    {
        if(numLetters[j] >0)
        {
            myLeaves[j] = makeNode(numLetters[j],NULL, NULL);
            // build priority queue / tree
            addEntry(pq, myLeaves[j]);            
        }
    }
    // end of file marker
    myLeaves[256] = makeNode(0, NULL,NULL);
    addEntry(pq,myLeaves[256]);
    
    
    

    // Creating parents by removing the smallest values of the queue
    // then putting them into a parent that will be reinserted into the queue
    while(numEntries(pq) > 1)
    {
        
        NODE *first = removeEntry(pq);
        NODE *second = removeEntry(pq);

        NODE *third = makeNode(first->count + second->count,first,second);
        
        
        addEntry(pq,third);
    }
   

    // prints the bits. Goes through all of the characters plus EOF
    int t;
    for(t = 0; t<257;t++)
    {
         // checks if there is actually a character or EOF there
        if(myLeaves[t] != NULL)
        {
             // prints letter if it can be printed. Otherwise print octal
            if(isprint(t) != 0)
            {
                printf("'%c'",t);
            }else{
                
                printf("%03o",t);
            }
            // print the number of bits by multiplying by the depth
            printf("%d x %d bits = %d bits \n",numLetters[t],depth(myLeaves[t]), numLetters[t] * depth(myLeaves[t]));
        }
        
    }
    
    // calling pack
    pack(argv[1],argv[2],myLeaves);

    fclose(toCount);
    destroyQueue(pq);
    destroyNode(myLeaves);

    exit(0);
}
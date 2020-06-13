#include "objects.h"

struct heap
{
enemy *heapArr;
int last;
int size;
int maxSize;
};

heap* CreateHeap(int maxSize);
bool heapInsert(heap*pHeap,enemy dataIn);
bool heapDelete(heap*pHeap,enemy &dataOut);
int heapCount(heap*pHeap);
bool heapFull(heap*pHeap);
bool heapEmpty(heap*pHeap);
void heapDestroy(heap*pheap);

void reheapUp(heap*pHeap,int chilLoc);
void reheapDown(heap*pHeap,int root);

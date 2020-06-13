#include "binheap.h"
#include <stdlib.h>
#include "objects.h"

heap* CreateHeap(int maxSize)
{
	heap * pHeap=new heap;
	pHeap->heapArr=new enemy[maxSize];
	if(!pHeap)
		return NULL;

	pHeap->maxSize=maxSize;
	pHeap->last=-1;
	pHeap->size=0;

	return pHeap;

}
/////////////////////////////////////////
/*
BuildHeap
Given an array , rearrange data so that they form a heap
Pre heap is array containing data in nonheap order
    size is number of elements in array
Post array is now a heap
1 Set walker to 1
2 Loop (walker<size)
  1 reheapUp(heap,Walker)
  2 increment walker
3 end Loop
end buildHeap
*/
////////////////////////////////////////
/*Algorithm insertHeap (heap,last,data)
Inserts data into heap
Pre heap is a valid heap structure
    last is reference param to last node in heap
	data contains data to be inserted
Post data have been inserted into heap
return true if successful
if(heap full)
  return false
end if
increment last
move data to last node
reheapUp(heap,last)
end insertHeap
*/
bool heapInsert(heap*pHeap,enemy dataIn)
{
	if(pHeap->size==0) //Heap Empty
	{
		pHeap->size=1;
		pHeap->last=0;
		pHeap->heapArr[pHeap->last]=dataIn;
		return true;
	}
	if(pHeap->last==pHeap->maxSize-1)
		return false;
	++(pHeap->last);
	++(pHeap->size);
	pHeap->heapArr[pHeap->last]=dataIn;
	reheapUp(pHeap,pHeap->last);
	return true;
}

///////////////////////////////////////////////////
void reheapUp(heap*pHeap,int chilLoc)
{
	 //Complete it here 
}
///////////////////////////////////////////
bool heapDelete(heap*pHeap,enemy &dataOut)
{
	if(pHeap->size==0) 
		return false;
	dataOut=pHeap->heapArr[0];
	pHeap->heapArr[0]=pHeap->heapArr[pHeap->last];
	(pHeap->last)--;
	(pHeap->size)--;
	reheapDown(pHeap,0);
	return true;
}
//////////////////////////////////////////////////
void reheapDown(heap*pHeap,int root)
{ 
//Complete it here 
return;
}


////////////////////////////////////////////////
int heapCount(heap*pHeap)
{

	return pHeap->size;
}
/////////////////////////////////////////////////
bool heapFull(heap*pHeap)
{
	return (pHeap->size == pHeap->maxSize);
}
////////////////////////////////////////////////
bool heapEmpty(heap*pHeap)
{
	return (pHeap->size==0);
}
//////////////////////////////////////////////////
void heapDestroy(heap*pheap)
{
	//Complete it here 
}


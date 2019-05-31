#include<stdio.h>
#include<stddef.h>
#include<math.h>
#include "mymalloc.h"

struct block *freeSpace =(void*)memory;
int direction = 1;

void Initialize()
{
	 freeSpace->size= -20000;
	 freeSpace->next=NULL;
	 freeSpace->prev=NULL;
}

struct block* FindFirstBlock(int noOfBytes)
{
	struct block *ptr = freeSpace, *result = NULL;
	while (result == NULL)
	{
		if (direction > 0)
		{
			while (ptr->next!=NULL)
			{
				if(abs(ptr->size) >= (noOfBytes + (int)sizeof(struct block) + (int)sizeof(int)))
				{
					result = ptr;
					break;
				}
				ptr = ptr->next;
			}
			if (result == NULL && abs(ptr->size) >= (noOfBytes + (int)sizeof(struct block) + (int)sizeof(int)))
				return ptr;
			else
				direction = -direction;
		}
		if (direction < 0)
		{
			while (ptr->prev!=NULL)
			{
				if(abs(ptr->size) >= (noOfBytes + (int)sizeof(struct block) + (int)sizeof(int)))
				{
					result = ptr;
					break;
				}
				ptr = ptr->prev;
			}
			if (result == NULL && abs(ptr->size) >= (noOfBytes + (int)sizeof(struct block) + (int)sizeof(int)))
				return ptr;
			else
				direction = -direction;
		}
	}
	return result;
}
void InsertBlock(struct block* curBlock, int noOfBytes)
{
	struct block* newBlock = (void*)((char*)curBlock + sizeof(struct block) + noOfBytes + sizeof(int));
	newBlock->size = -(abs(curBlock->size) - noOfBytes - (int)sizeof(struct block) - (int)sizeof(int));
	newBlock ->next = curBlock->next;
	newBlock->prev = curBlock->prev;
	curBlock->size = noOfBytes + sizeof(struct block) + sizeof(int);
	//*((int*)((char*)curBlock + sizeof(struct block) + noOfBytes)) = curBlock->size;
	*((int*)((char*)curBlock + sizeof(struct block) + noOfBytes)) = curBlock->size;
	*((int*)((char*)newBlock + abs(newBlock->size) - sizeof(int))) = newBlock->size;
	if (curBlock->prev != NULL)
		curBlock->prev->next = newBlock;
	if (curBlock->next != NULL)
		curBlock->next->prev = newBlock;
	if (freeSpace == curBlock)
		freeSpace = newBlock;
}
void *MyMalloc(int noOfBytes)
{
	 void *result;
	 struct block *ptr;
	 if (!(freeSpace->size))
	 {
			Initialize();
			printf("Memory initialized\n");
	 }
	 ptr = FindFirstBlock(noOfBytes);
	 direction = -direction;
	 if (ptr->size == (noOfBytes + sizeof(struct block) + sizeof(int)))
	 {
			if (freeSpace == ptr)
			{
				freeSpace = freeSpace->next;
				ptr->next->prev = NULL;
			}
			else
			{
				ptr->next->prev = ptr->prev;
				ptr->prev->next = ptr->next;
			}
			*(((char*)ptr + noOfBytes + sizeof(struct block))) = ptr->size;
			result = (char*)ptr + sizeof(struct block);
	 }
	 else if (ptr == NULL)
	 {
			result = NULL;
			printf("Sorry. No sufficient memory to allocate\n");
	 }
	 else 
	 {
		 InsertBlock(ptr, noOfBytes);
		 result = (char*)ptr + sizeof(struct block);
	 }
	 return result;
}
void MergeLeft(void* ptr, int prevBlockSize, int blockSize, int userSize)
{
	*((int*)((char*)ptr - sizeof(struct block) + prevBlockSize)) = prevBlockSize - blockSize;
	*((int*)((char*)ptr + userSize)) = prevBlockSize - blockSize;
}
void MergeRight(void* ptr, int blockSize, int userSize)
{
	struct block *newBlock = (struct block*)((char*)ptr - sizeof(struct block)), *temp;
	temp = (struct block*)(((char*)ptr + userSize + sizeof(int)));
	newBlock->prev = temp->prev;
	newBlock->next = temp->next;
	if (newBlock->next != NULL)
		newBlock->next->prev = newBlock;
	if (newBlock->prev != NULL)
		newBlock->prev->next = newBlock;
	newBlock->size = temp->size - blockSize;
	*((int*)((char*)ptr + userSize - temp->size)) = newBlock->size;
	freeSpace = newBlock;
}
void MergeBoth(void* ptr, int prevBlockSize, int nextBlockSize, int blockSize, int userSize)
{
	struct block *newBlock = (struct block*)((char*)ptr - sizeof(struct block) + prevBlockSize), *secondPtr, *temp;
	secondPtr = (struct block*)(((char*)ptr + userSize + sizeof(int)));
	MergeLeft(ptr, prevBlockSize, blockSize, userSize);
	newBlock->size = newBlock->size + nextBlockSize;
	*((int*)((char*)ptr + userSize - nextBlockSize)) = newBlock->size;
	while (newBlock->next != NULL && newBlock->next != secondPtr)
		newBlock = newBlock->next;
	if (newBlock->next != NULL)
	{
		temp = newBlock;
		newBlock->next = secondPtr->next;
		if (newBlock->next != NULL)
			newBlock->next->prev = temp;
	}
	else
	{
		while (newBlock->prev != NULL && newBlock->prev != secondPtr)
			newBlock = newBlock->prev;
		if (newBlock->next != NULL)
		{
			temp = newBlock;
			newBlock->prev = secondPtr->prev;
			if (newBlock->prev != NULL)
				newBlock->prev->next = temp;
		}
	}
	freeSpace = newBlock;
}

void MyFree(void *ptr)
{
	int userSize, blockSize, prevBlockSize, nextBlockSize;
	struct block *newBlock;
	blockSize = (int)*((int*)((char*)ptr - sizeof(struct block)));
	if (blockSize <= 0)
	{
		printf("This memory has already been releaased!\n");
		return;
	}
	userSize = blockSize - sizeof(struct block) - sizeof(int);
	prevBlockSize = (int)*((int*)((char*)ptr - sizeof(struct block) - sizeof(int)));
	nextBlockSize = (int)*((int*)((char*)ptr + userSize + sizeof(int)));
	if (((char*)memory < ((char*)ptr - sizeof(struct block) - sizeof(int))) && (((char*)(memory + 20000) > ((char*)ptr + userSize + sizeof(int)))) && prevBlockSize < 0 && nextBlockSize < 0)
	{
		MergeBoth(ptr, prevBlockSize, nextBlockSize, blockSize, userSize);
		return;
	}

	else if ((char*)memory < ((char*)ptr - sizeof(struct block) - sizeof(int)) && prevBlockSize < 0)
	{
		MergeLeft(ptr, prevBlockSize, blockSize, userSize);
		return;
	}
	else if ((char*)(memory + 20000) > ((char*)ptr + userSize + sizeof(int)) && nextBlockSize < 0)
	{
		MergeRight(ptr, blockSize, userSize);
		return;
	}
	else
	{
		newBlock = (struct block*)((char*)ptr - sizeof(struct block));
		newBlock->size = -(newBlock->size);
		newBlock->prev = freeSpace->prev;
		newBlock->next = freeSpace;
		newBlock->next->prev = newBlock;
		freeSpace = newBlock;
		(int)*((int*)((char*)ptr + userSize)) = newBlock->size;
	}		

}

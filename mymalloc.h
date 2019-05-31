#include<stdio.h>
#include<stddef.h>

char memory[20000];

typedef struct block{
 int size;
 struct block *next;
 struct block *prev; 
};

void Initialize();
struct block* FindFirstBlock(int noOfBytes);
void InsertBlock(struct block* firstBlock, int noOfBytes);
void *MyMalloc(int noOfBytes);
void MergeLeft(void* ptr, int prevBlockSize, int blockSize, int userSize);
void MergeRight(void* ptr, int blockSize, int userSize);
void MyFree(void* ptr);

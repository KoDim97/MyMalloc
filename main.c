#include<stdio.h>
#include "mymalloc.h"

int main(){
 int *p, *r, *k, *a, *b, *c;
 char *q, *w, *e;
	//p=(int*)MyMalloc(50*sizeof(int));
	//q=(char*)MyMalloc(250*sizeof(char));
	//r=(int*)MyMalloc(1000*sizeof(int));
	//b=(int*)MyMalloc(410*sizeof(int));
	//e=(char*)MyMalloc(1330*sizeof(char));
	//c=(int*)MyMalloc(454*sizeof(int));
	//k=(int*)MyMalloc(890*sizeof(int));
	//w=(char*)MyMalloc(700*sizeof(char));
	//a=(int*)MyMalloc(340*sizeof(int));
	//MyFree(p);
	//MyFree(q);
	//MyFree(r);
	//MyFree(b);
	//MyFree(e);
	//MyFree(c);
	//MyFree(k);
	//MyFree(w);
	//MyFree(a);

 p=(int*)MyMalloc(50*sizeof(int));
 q=(char*)MyMalloc(250*sizeof(char));
 r=(int*)MyMalloc(1000*sizeof(int));
 b=(int*)MyMalloc(410*sizeof(int));
 MyFree(p);
 MyFree(r);
 MyFree(q);
 e=(char*)MyMalloc(1330*sizeof(char));
 c=(int*)MyMalloc(454*sizeof(int));
 k=(int*)MyMalloc(890*sizeof(int));
 w=(char*)MyMalloc(700*sizeof(char));
 a=(int*)MyMalloc(340*sizeof(int));
 MyFree(w);
 MyFree(b);
 MyFree(c);
 MyFree(e);
 MyFree(k);
 MyFree(a);
 printf("Allocation and deallocation is done successfully!\n");
 return 0;
}
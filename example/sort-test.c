#include "../tinyll.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

bool Is_Greater_Int(void* a,void* b)
{
  return *((int*)a) >= *((int*)b);
}

int main(int argv, char** argc)
{
    int size = 1000;
    if(argv == 2)
      size = atoi(argc[1]);
    struct TinyLL* Integer_List = Create_New_Empty_List(NULL);
    int index = 0;
    while(index < size)
    {
    	int* tmp = malloc(sizeof(int));
    	*(tmp) = rand();
    	if(rand()%2)
    		*(tmp) = *(tmp) * -1;
    	Insert_Element_At_The_End(Integer_List, tmp);
    	index++;
    }
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    Integer_List = Strand_Sort(Integer_List,Is_Greater_Int);
    end = clock();
    cpu_time_used = (((double) (end - start)) * 1000.0) / CLOCKS_PER_SEC;
    printf(" %i  %f \n",size,cpu_time_used);
    return 0;
}

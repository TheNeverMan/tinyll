#include "../tinyll.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* iterator usage examples */
void* Print_LL_On_Screen(void* Data, bool first, bool last)
{
	if(!Data)
		return NULL;
	double tmp =  *((double*) Data);
	if(first)
		printf("[");
	printf("%f, ", tmp);
	if(last)
		printf("]\n");
	return NULL;
}


void* Print_LL_String(void* Data, bool first, bool last)
{
	if(!Data)
		return NULL;
	 putchar(*((char*) Data));
	if(last)
		printf("\n");
	return NULL;
}

void* Get_LL_Size(void* Data, bool first, bool last)
{
	static int length;
	if(first && last)
		return &length;
	if(first)
		length = 0;
	length++;
	return NULL;
}

void* Find_Maximum_Element_In_LL(void* Data, bool first, bool last)
{
	static double max;
	if(first && last)
		return &max;
	double element = *((double*) Data);
	if(first)
		max = element;
	if(max < element)
		max = element;
	return NULL;
}

static void* Print_Num(void* Data, bool first, bool last)
{
  int num = *((int*)Data);
  if(first)
    printf("{ ");
  printf("%i, ",num);
  if(last)
    printf("}\n");
  return NULL;
}

void* Calculate_LL_Average(void* Data, bool first, bool last)
{
	static double sum;
	static double avg;
	static int size;
	if(first && last)
		return &avg;
	double element = *((double*) Data);
	if(first)
		sum = avg = size = 0;
	size++;
	sum += element;
	if(last)
		avg = sum / size;
	return NULL;
}

/* insertion and deletion examples*/
void Insert_Word_Space_After_Space(struct TinyLL* List)
{
	struct TinyLL_Node* tmp = List->First;
	while(tmp)
	{
		if(*((char*)tmp->Data) == ' ')
		{
			struct TinyLL_Node* tmp_next;
			char spacja[] = "space";
			int index = strlen(spacja);
			while(index --> 0)
			{
				char* znak = malloc(sizeof(char));
				*(znak) = spacja[index];
				Insert_Element_After_Position(List, znak, tmp);
			}
			tmp_next = tmp->Next;
			Delete_Element_At_Position(List, tmp);
			tmp = tmp_next;
			continue;
		}
		tmp = tmp->Next;
	}
}

bool Is_Greater_Int(void* a,void* b)
{
  return *((int*)a) >= *((int*)b);
}

int main()
{
	struct TinyLL* Doubles_List = Create_New_Empty_List(NULL);
	int seed = time(NULL);
	srand(seed);
	/*Create linked list of 1000000 doubles and populate it */
	int index = 0;
	while(index < 100000)
	{
		double* tmp = malloc(sizeof(double));
		*(tmp) = rand() % 10;
		Insert_Element_At_The_End(Doubles_List, tmp);
		index++;
	}
	/*Iterate_Over(Doubles_List,Print_LL_On_Screen);*/
	Iterate_Over(Doubles_List,Get_LL_Size);
	printf("Linked list size: %i\n", *((int*)Get_LL_Size(NULL,TRUE,TRUE)));
	Iterate_Over(Doubles_List,Calculate_LL_Average);
	printf("Linked list average value: %f\n", *((double*)Calculate_LL_Average(NULL,TRUE,TRUE)));
	Iterate_Over(Doubles_List, Find_Maximum_Element_In_LL);
	printf("Largest element in linked list: %f\n", *((double*) Find_Maximum_Element_In_LL(NULL, TRUE, TRUE)));
	Destroy_List(Doubles_List);
	/* modify a string */
	char test_string[] = "Test String for an array";
	index = 0;
	struct TinyLL* String_List = Create_New_Empty_List(NULL);
	while(index < strlen(test_string))
	{
		char* tmp = malloc(sizeof(char));
		*(tmp) = test_string[index];
		Insert_Element_At_The_End(String_List, tmp);
		index++;
	}
	Iterate_Over(String_List, Print_LL_String);
	/* do funny things with new linked list*/
	Insert_Word_Space_After_Space(String_List);
	Iterate_Over(String_List, Print_LL_String);
	Destroy_List(String_List);
	struct TinyLL* Integer_List = Create_New_Empty_List(NULL);
	index = 0;
	while(index < 400)
	{
		int* tmp = malloc(sizeof(double));
		*(tmp) = rand() % 1001;
		Insert_Element_At_The_End(Integer_List, tmp);
		index++;
	}
	printf("List before sorting: ");
	Iterate_Over(Integer_List, Print_Num);
	Integer_List = Strand_Sort(Integer_List,Is_Greater_Int);
	printf("Strand sorted list: ");
	Iterate_Over(Integer_List, Print_Num);
	Destroy_List(Integer_List);
	printf("Seed: %i\n",seed);
	return 0;
}

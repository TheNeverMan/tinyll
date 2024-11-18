#pragma once
#include<stdlib.h>

#define FALSE 0
#define TRUE 255

struct TinyLL_Node
{
  void* Data;
  struct TinyLL_Node* Next;
};

struct TinyLL
{
  struct TinyLL_Node* First;
  struct TinyLL_Node* Last;
  void (*Destroy_Data)(void*);
};

typedef int bool;

struct TinyLL* Create_New_Empty_List(void (*Destroy_Data)(void*));
void Destroy_List(struct TinyLL* List);
void Insert_Element_At_The_Beginning(struct TinyLL* List, void* Data);
void Insert_Element_At_The_End(struct TinyLL* List, void* Data);
void Insert_Element_After_Position(struct TinyLL* List, void* Data, struct TinyLL_Node* After);
void Delete_Element_At_The_End(struct TinyLL* List);
void Delete_Element_At_Position(struct TinyLL* List, struct TinyLL_Node* To_Be_Deleted);
void Iterate_Over(struct TinyLL* List, void* (*Iterator_Function)(void*, bool, bool));
struct TinyLL* Strand_Sort(struct TinyLL* List, bool (*Is_Greater_Or_Equal)(void*,void*));
struct TinyLL_Node* Get_Node_Before(struct TinyLL* List, struct TinyLL_Node* Node);
struct TinyLL_Node* Destroy_Node(struct TinyLL* List, struct TinyLL_Node* Node);

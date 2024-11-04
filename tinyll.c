 #include "tinyll.h"



struct TinyLL* Create_New_Empty_List(void (*Destroy_Data)(void*))
{
  struct TinyLL* out = malloc(sizeof(struct TinyLL));
  out->Destroy_Data = Destroy_Data;
  out->First = NULL;
  out->Last = NULL;
  return out;
}

struct TinyLL_Node* Destroy_Node(struct TinyLL* List, struct TinyLL_Node* Node)
{
  if(List->Destroy_Data)
    List->Destroy_Data(Node->Data);
  else
    free(Node->Data);
  struct TinyLL_Node* Tmp = Node->Next;
  free(Node);
  return Tmp;
}

void Destroy_List(struct TinyLL* List)
{
  struct TinyLL_Node* Deleted_Node = List->First;
  while(Deleted_Node)
    Deleted_Node = Destroy_Node(List, Deleted_Node);
  free(List);
}


/* iterator function has following structure
void* pointer is pointer to actual data of iterated element
bools are set to true false on first element of the list, so iterator can reset its static variables
then they are set to false true on the last element of the list
true true is used to access return data from outside of the iteration (look at examples in main.c)*/
void Iterate_Over(struct TinyLL* List, void* (*Iterator_Function)(void*, bool , bool))
{
  struct TinyLL_Node* Iterated_Node = List->First;
  while(Iterated_Node)
  {
  	bool first = FALSE;
  	bool last = FALSE;
  	if(Iterated_Node == List->First)
			first = TRUE;
		if(Iterated_Node == List->Last)
			last = TRUE;
    Iterator_Function(Iterated_Node->Data, first, last);
    Iterated_Node = Iterated_Node->Next;
  }
}

void Insert_Element_After_Position(struct TinyLL* List, void* Data, struct TinyLL_Node* After)
{
  /*first element is inserted*/
  struct TinyLL_Node* New = malloc(sizeof(struct TinyLL_Node));
  if((!List->Last))
  {
    List->First = New;
    List->Last = New;
    New->Data = Data;
    New->Next = NULL;
    return;
  }
  struct TinyLL_Node* New_Next = After->Next;
  New->Data = Data;
  New->Next = New_Next;
  After->Next = New;
  if(!(New->Next))
    List->Last = New;
}

void Insert_Element_At_The_Beginning(struct TinyLL* List, void* Data)
{
  Insert_Element_After_Position(List, Data, List->First);
}

void Insert_Element_At_The_End(struct TinyLL* List, void* Data)
{
  Insert_Element_After_Position(List, Data, List->Last);
}

struct TinyLL_Node* Get_Node_Before(struct TinyLL* List, struct TinyLL_Node* Node)
{
  if(List->First == Node)
    return NULL;
  struct TinyLL_Node* out = List->First;
  while(out)
  {
    if(out->Next == Node)
      return out;
    out = out->Next;
  }
  return NULL;
}

void Delete_Element_At_Position(struct TinyLL* List, struct TinyLL_Node* To_Be_Deleted)
{
  struct TinyLL_Node* Before_Node = Get_Node_Before(List, To_Be_Deleted);
  if(!Before_Node)
    List->First = To_Be_Deleted->Next;
  else if(!(To_Be_Deleted->Next))
  {
    List->Last = Before_Node;
    Before_Node->Next = NULL;
  }
  else
    Before_Node->Next = To_Be_Deleted->Next;
  Destroy_Node(List, To_Be_Deleted);
}

void Delete_Element_At_The_End(struct TinyLL* List)
{
  Delete_Element_At_Position(List, List->Last);
}

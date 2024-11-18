#include "tinyll.h"
#include <stdio.h>

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
  struct TinyLL_Node* Tmp = NULL;
  if(List->Destroy_Data)
    List->Destroy_Data(Node->Data);
  else
    free(Node->Data);
  Tmp = Node->Next;
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
  struct TinyLL_Node* New_Next = NULL;
  if((!List->Last))
  {
    List->First = New;
    List->Last = New;
    New->Data = Data;
    New->Next = NULL;
    return;
  }
  New_Next = After->Next;
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
  struct TinyLL_Node* out = NULL;
  if(List->First == Node)
    return out;
  out = List->First;
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

static struct TinyLL_Node* Insert_After_And_Remove(struct TinyLL* To, struct TinyLL* From, struct TinyLL_Node* After, struct TinyLL_Node* To_Be_Inserted)
{
  struct TinyLL_Node* After_Next = After->Next;
  struct TinyLL_Node* To_Be_Inserted_Next = To_Be_Inserted->Next;
  if(!After->Next)
    To->Last = To_Be_Inserted;
  After->Next = To_Be_Inserted;
  After->Next->Next = After_Next;
  if(!To_Be_Inserted_Next)
    From->Last = NULL;
  From->First = To_Be_Inserted_Next;
  return To_Be_Inserted_Next;
}

static void Insert_First_And_Remove(struct TinyLL* To, struct TinyLL* From)
{
  struct TinyLL_Node* To_First = To->First;
  struct TinyLL_Node* To_Be_Inserted_Next = From->First->Next;
  if(!To->Last) /* what the fuck adding to empty list i guess */
    To->Last = From->First;
  To->First = From->First;
  To->First->Next = To_First;
  if(!To_Be_Inserted_Next)
    From->Last = NULL;
  From->First = To_Be_Inserted_Next;
}

static void Merge(struct TinyLL* Main, struct TinyLL* To_Be_Merged, bool (*Is_Greater_Or_Equal)(void*,void*))
{
  struct TinyLL_Node* Iterator_Main = Main->First;
  struct TinyLL_Node* Iterator_Merged = NULL;
  if(!Iterator_Main)
  {
    Main->First = To_Be_Merged->First;
    Main->Last = To_Be_Merged->Last;
    *Main = *To_Be_Merged;
    return;
  }
  while(To_Be_Merged->First && Is_Greater_Or_Equal(Main->First->Data,To_Be_Merged->First->Data))
    Insert_First_And_Remove(Main,To_Be_Merged);
  Iterator_Merged = To_Be_Merged->First;
  Iterator_Main = Main->First;
  while(Iterator_Merged)
    if(Is_Greater_Or_Equal(Iterator_Merged->Data,Iterator_Main->Data) && !(Iterator_Main->Next && Is_Greater_Or_Equal(Iterator_Merged->Data,Iterator_Main->Next->Data)))
      Iterator_Merged = Insert_After_And_Remove(Main,To_Be_Merged,Iterator_Main,Iterator_Merged);
    else
      if(Iterator_Main->Next)
        Iterator_Main = Iterator_Main->Next;
}




struct TinyLL* Strand_Sort(struct TinyLL* List, bool (*Is_Greater_Or_Equal)(void*,void*))
{
  struct TinyLL* out = Create_New_Empty_List(List->Destroy_Data);
  while(List->First)
  {
    struct TinyLL* Sub_List = Create_New_Empty_List(List->Destroy_Data);
    struct TinyLL_Node* Iterator = NULL;
    struct TinyLL_Node* Prev_Iterator = NULL;
    if(!List->First->Next)
    {
      Merge(out,List,Is_Greater_Or_Equal);
      free(Sub_List);
      break;
    }
    Sub_List->First = List->First;
    Sub_List->Last = List->First;
    List->First = List->First->Next;
    Sub_List->Last->Next = NULL;
    Iterator = List->First->Next;
    Prev_Iterator = List->First;
    while(Iterator)
    {
      if(Is_Greater_Or_Equal(Iterator->Data,Sub_List->Last->Data))
      {
        struct TinyLL_Node* tmp = Iterator->Next;
        Sub_List->Last->Next = Iterator;
        Sub_List->Last = Iterator;
        Sub_List->Last->Next = NULL;
        Iterator = tmp;
        Prev_Iterator->Next = Iterator;
        if(!Prev_Iterator->Next)
          List->Last = Prev_Iterator;
        continue;
      }
      Prev_Iterator = Iterator;
      Iterator = Iterator->Next;
    }
    Merge(out,Sub_List,Is_Greater_Or_Equal);
    free(Sub_List);
  }
  free(List);
  return out;
}

#include <iostream>
#include "list.h"

struct foo
{
    int bar;
    struct list_elem elem;
};

void insert(struct list *foo_list)
{
  int i,j;
  for(i=0;i<4;i++)
  {
     struct foo *f=(struct foo *)malloc(sizeof(struct foo));
     list_push_front(foo_list,&f->elem);
     f->bar=i;
  }
}

void print(struct list *foo_list)
{
  struct list_elem *e;
  for (e = list_begin (foo_list); e != list_end (foo_list);
           e = list_next (e))
  {
          struct foo *f=list_entry (e,struct foo, elem);

          printf("\nvalue:%d\n",f->bar);
  }
}

void test_list_demo()
{
   struct list foo_list;
   list_init (&foo_list);
   insert(&foo_list);
   print(&foo_list);
}

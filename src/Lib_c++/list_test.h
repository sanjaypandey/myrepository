#include <iostream>
#include "list.h"

struct foo
{
    int bar;
    float f;
    struct list_elem elem;
    char c;
};

void insert(struct list *foo_list)
{
  int i,j;
  for(i=0;i<8;i++)
  {
     struct foo *f=(struct foo *)malloc(sizeof(struct foo));
     list_push_front(foo_list,&f->elem);
     f->bar=i;
     f->c='a';
     f->f=1.0;
  }
}

void print(struct list *foo_list)
{
  struct list_elem *e;
  for (e=list_begin (foo_list); e != list_end (foo_list);
           e = list_next (e))
  {
          struct foo *f=list_entry (e,struct foo,elem);
        //  printf("\n address:%u  %u  %u ",f,&f->elem,&f->bar);

        //  printf("\n actual:%u  %u  ",(uint8_t *)(e),offsetof(struct foo,elem));

          printf("value:%d %c %f\n",f->bar,f->c,f->f);
  }
}

void test_list_demo()
{
   struct list foo_list;
   list_init (&foo_list);
   insert(&foo_list);
   print(&foo_list);
}


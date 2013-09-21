#ifndef __LIB_MALLOC_H
#define __LIB_MALLOC_H

#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "list.h"

using namespace std;

void Malloc_init (void);
void *Malloc (size_t) __attribute__ ((malloc));
void *Calloc (size_t, size_t) __attribute__ ((malloc));
void *Realloc (void *, size_t);
void Free (void *);

#endif

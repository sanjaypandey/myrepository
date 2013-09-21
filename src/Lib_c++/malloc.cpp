#include "malloc.h"
#define ASSERT assert


//This is the maximum page size consider by the virtual memory.
#define PGSIZE (4*1024)

/** A simple implementation of malloc().

   The size of each request, in bytes, is rounded up to a power
   of 2 and assigned to the "descriptor" that manages blocks of
   that size.  The descriptor keeps a list of free blocks.  If
   the free list is nonempty, one of its blocks is used to
   satisfy the request.

   Otherwise, a new page of memory, called an "arena", is
   obtained from the page allocator (if none is available,
   malloc() returns a null pointer).  The new arena is divided
   into blocks, all of which are added to the descriptor's free
   list.  Then we return one of the new blocks.

   When we free a block, we add it to its descriptor's free list.
   But if the arena that the block was in now has no in-use
   blocks, we remove all of the arena's blocks from the free list
   and give the arena back to the page allocator.

   We can't handle blocks bigger than 2 kB using this scheme,
   because they're too big to fit in a single page with a
   descriptor.  We handle those by allocating contiguous pages
   with the page allocator and sticking the allocation size at
   the beginning of the allocated block's arena header.
*/

struct desc
{
   unsigned block_Size;
   unsigned blocks_per_arena;
   struct list freeList;
};

/** Magic number for detecting arena corruption. */
#define ARENA_MAGIC 0x9a548eed


struct arena
{
  struct desc *desc;
  unsigned magic;
  unsigned free_cnt;
};

struct block
{
  struct list_elem free_elem;
};

/**
our set of descriptors.
*/
struct desc descs[10];
static size_t desc_cnt=0;

/**
Conversion function from the block to arena and arena to block.
Arena if a frame from which we get the block_size of given bytes.
*/
static struct arena *block_to_arena (struct block *);
static struct block *arena_to_block (struct arena *, size_t idx);

/**
Initialises the malloc descriptors.
*/
void Malloc_init()
{
   size_t block_size;
   for(block_size=16;block_size<PGSIZE/2;block_size=block_size*2)
   {
      ASSERT (desc_cnt <= sizeof descs / sizeof *descs);
      struct desc *desc=&descs[desc_cnt++];
      desc->block_Size=block_size;
      desc->blocks_per_arena=(PGSIZE-sizeof(struct arena))/block_size;
      list_init(&desc->freeList);
   }
}

/** Obtains and returns a new block of at least SIZE bytes.
   Returns a null pointer if memory is not available.
*/
void *Malloc (size_t size)
{
   struct desc *d;
   struct arena *a;
   struct block *b;
   for(size_t i=0;i<desc_cnt;i++)
   {
       d=&descs[i];
       if(d->block_Size==size)
       {
          break;
        }
   }
   if(d==descs+desc_cnt)
   {
     printf("\nno more memory are available.. just returned.\n");
     return NULL;
   }
   if(list_empty(&d->freeList))
   {
      // struct
   }

}

/** Returns the number of bytes allocated for BLOCK.
static size_t block_size (void *block)
{
  struct block *b = block;
  struct arena *a = block_to_arena (b);
  struct desc *d = a->desc;

  return d != NULL ? d->block_size : PGSIZE * a->free_cnt - pg_ofs (block);
}
*/

/** Returns the arena that block B is inside. */
static struct arena *block_to_arena (struct block *b)
{
  struct arena *a ;//= pg_round_down (b);

  /* Check that the arena is valid. */
  ASSERT (a != NULL);
  ASSERT (a->magic == ARENA_MAGIC);

  /* Check that the block is properly aligned for the arena. */
//  ASSERT (a->desc == NULL|| (pg_ofs (b) - sizeof *a) % a->desc->block_size == 0);
 // ASSERT (a->desc != NULL || pg_ofs (b) == sizeof *a);

  return a;
}

/** Returns the (IDX - 1)'th block within arena A. */
static struct block *arena_to_block (struct arena *a, size_t idx)
{
  ASSERT (a != NULL);
  ASSERT (a->magic == ARENA_MAGIC);
//  ASSERT (idx < a->desc->blocks_per_arena);
  return (struct block *) ((uint8_t *) a+ sizeof *a
                           +idx * a->desc->block_Size);
}


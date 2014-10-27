#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "jconer/arena.hpp"

#define DEFAULT_BLOCK_SIZE 8192
#define ALIGNMENT               8
#define ALIGNMENT_MASK          (ALIGNMENT - 1)
#define ROUNDUP(x)              (((x) + ALIGNMENT_MASK) & ~ALIGNMENT_MASK)

typedef struct _block {
    /* Total number of bytes owned by this block available to pass out.
     * Read-only after initialization.  The first such byte starts at
     * ab_mem.
     */
    size_t ab_size;

    /* Total number of bytes already passed out.  The next byte available
     * to pass out starts at ab_mem + ab_offset.
     */
    size_t ab_offset;

    /* An arena maintains a singly-linked, NULL-terminated list of
     * all blocks owned by the arena.  These are linked via the
     * ab_next member.
     */
    struct _block *ab_next;

    /* Pointer to the first allocatable byte owned by this block.  Read-
     * only after initialization.
     */
    void *ab_mem;
} block;

/* The arena manages two kinds of memory, blocks of raw memory
   and a list of PyObject* pointers.  PyObjects are decrefed
   when the arena is freed.
*/

struct _arena {
    /* Pointer to the first block allocated for the arena, never NULL.
       It is used only to find the first block when the arena is
       being freed.
     */
    block *a_head;

    /* Pointer to the block currently used for allocation.  It's
       ab_next field should be NULL.  If it is not-null after a
       call to block_alloc(), it means a new block has been allocated
       and a_cur should be reset to point it.
     */
    block *a_cur;

#if defined(_DEBUG)
    /* Debug output */
    size_t total_allocs;
    size_t total_size;
    size_t total_blocks;
    size_t total_block_size;
    size_t total_big_blocks;
#endif
};

static block *
block_new(size_t size)
{
    /* Allocate header and block as one unit.
       ab_mem points just past header. */
    block *b = (block *)malloc(sizeof(block) + size);
    if (!b)
        return NULL;
    b->ab_size = size;
    b->ab_mem = (void *)(b + 1);
    b->ab_next = NULL;
    b->ab_offset = ROUNDUP((long)(b->ab_mem)) -
      (long)(b->ab_mem);
    return b;
}

static void
block_free(block *b) {
    while (b) {
        block *next = b->ab_next;
        free(b);
        b = next;
    }
}

static void *
block_alloc(block *b, size_t size)
{
    void *p;
    assert(b);
    size = ROUNDUP(size);
    if (b->ab_offset + size > b->ab_size) {
        /* If we need to allocate more memory than will fit in
           the default block, allocate a one-off block that is
           exactly the right size. */
        /* TODO(jhylton): Think about space waste at end of block */
        block *newbl = block_new(
                        size < DEFAULT_BLOCK_SIZE ?
                        DEFAULT_BLOCK_SIZE : size);
        if (!newbl)
            return NULL;
        assert(!b->ab_next);
        b->ab_next = newbl;
        b = newbl;
    }

    assert(b->ab_offset + size <= b->ab_size);
    p = (void *)(((char *)b->ab_mem) + b->ab_offset);
    b->ab_offset += size;
    return p;
}

Arena *
arena_New()
{
    Arena* arena = (Arena *)malloc(sizeof(Arena));
    if (!arena)
        return NULL;

    arena->a_head = block_new(DEFAULT_BLOCK_SIZE);
    arena->a_cur = arena->a_head;
    if (!arena->a_head) {
        free((void *)arena);
        return NULL;
    }

#if defined(_DEBUG)
    arena->total_allocs = 0;
    arena->total_size = 0;
    arena->total_blocks = 1;
    arena->total_block_size = DEFAULT_BLOCK_SIZE;
    arena->total_big_blocks = 0;
#endif
    return arena;
}

void
arena_Free(Arena *arena)
{
    assert(arena);
#if defined(_DEBUG)
    /*
    fprintf(stderr,
        "alloc=%d size=%d blocks=%d block_size=%d big=%d objects=%d\n",
        arena->total_allocs, arena->total_size, arena->total_blocks,
        arena->total_block_size, arena->total_big_blocks,
        PyList_Size(arena->a_objects));
    */
#endif
    block_free(arena->a_head);
    /* This property normally holds, except when the code being compiled
       is sys.getobjects(0), in which case there will be two references.
    */
    free(arena);
}

void *
arena_Malloc(Arena *arena, size_t size)
{
    void *p = block_alloc(arena->a_cur, size);
    if (!p)
        return NULL;
#if defined(_DEBUG)
    arena->total_allocs++;
    arena->total_size += size;
#endif
    /* Reset cur if we allocated a new block. */
    if (arena->a_cur->ab_next) {
        arena->a_cur = arena->a_cur->ab_next;
#if defined(_DEBUG)
        arena->total_blocks++;
        arena->total_block_size += arena->a_cur->ab_size;
        if (arena->a_cur->ab_size > DEFAULT_BLOCK_SIZE)
            ++arena->total_big_blocks;
#endif
    }
    return p;
}

#include <stdio.h>

#define HEAP_SIZE (64*1024)
#define ALIGNMENT 32

// size_t aligned_size = size_t((size+(ALIGNMENT-1))/ALIGNMENT) * ALIGNMENT;

typedef struct block {
    void * addr;
    size_t len;
    struct block * prev;
    struct block * next;
} block;

static char heap[HEAP_SIZE];
static block * alloced_head = NULL;
static block * alloced_tail = NULL;
static block * free_head = NULL;
static block * free_tail = NULL;

void * myalloc(size_t size) {    
    if (free_head == NULL)
    {
        block * new_block = (block*)heap;
        new_block->addr = (char*)new_block + sizeof(block);
        new_block->len = HEAP_SIZE - sizeof(block);
        new_block->prev = NULL;
        new_block->next = NULL;

        free_head = new_block;
        free_tail = new_block;
    }

    if (size < 1) return NULL;
    
    block * curr_block = free_head;
    while (curr_block != NULL)
    {
        if (curr_block->len >= size)
        {
            if (curr_block->len > size)
            {
                block * new_free_blk = (block*)((char*)curr_block + sizeof(block) + size);
                new_free_blk->addr = (char*)new_free_blk + sizeof(block);
                new_free_blk->len = curr_block->len - sizeof(block) - size;
                new_free_blk->next = NULL;

                if (free_head == curr_block)
                {
                    free_head = new_free_blk;
                    free_tail = new_free_blk;
                    new_free_blk->prev = NULL;
                } 
                else 
                {
                    new_free_blk->prev = free_tail;
                    free_tail->next = new_free_blk;
                }
            }

            curr_block->len = size;
            curr_block->next = NULL;
            if (alloced_head == NULL)
            {
                alloced_head = curr_block;
                alloced_tail = curr_block;
                curr_block->prev = NULL;
            }
            else
            {
                curr_block->prev = alloced_tail;
                alloced_tail = curr_block;
            }
            
            return curr_block->addr;
        }
        
        curr_block = curr_block->next;
    }
    
    return NULL;
}

void myfree(void * ptr) {
    if (ptr == NULL)
    {
        return;
    }
    
    block * curr_block = alloced_head;
    while (curr_block != NULL)
    {
        if (curr_block->addr == ptr)
        {
            if (curr_block == alloced_tail)
            {
                alloced_tail = curr_block->prev;
            }
            else 
            {
                curr_block->prev->next = curr_block->next;
            }

            free_tail->next = curr_block;
            free_tail = curr_block;

            return;
        }
        
        curr_block = curr_block->next;
    }
}

void dump_alloced() {
    block * curr_block = alloced_head;
    printf("Alloced List:\n");
    printf("  head: %p tail: %p\n\n", alloced_head, alloced_tail);
    while (curr_block != NULL)
    {
        printf("  addr: %p\n  len: %zu\n  prev: %p\n  next: %p\n\n", curr_block->addr, curr_block->len, curr_block->prev, curr_block->next);
        
        curr_block = curr_block->next;
    }
}

void dump_free() {
    block * curr_block = free_head;
    printf("Free List:\n");
    printf("  head: %p tail: %p\n\n", free_head, free_tail);
    while (curr_block != NULL)
    {
        printf("  addr: %p\n  len: %zu\n  prev: %p\n  next: %p\n\n", curr_block->addr, curr_block->len, curr_block->prev, curr_block->next);
        
        curr_block = curr_block->next;
    }
}

int main(void) {
    void * ptr = myalloc(5);
    void * ptr2 = myalloc(32);
    void * ptr3 = myalloc(41);

    myfree(ptr2);
    void* ptr4 = myalloc(20);


    myfree(ptr);
    // myfree(ptr3);
    // myfree(ptr4);
    
    return 0;
}
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

void new_bitmap(struct bitmap *b, mfs_size_t size)
{
    b->size = size;
    b->bits = (unsigned char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memset(b->bits, 0, size);
    b->prev_pos = 0;
    return 0;
}

void free_bitmap(struct bitmap *b)
{
    munmap(b->bits, b->size);
    b->size = 0;
    return;
}

void bitmap_set(struct bitmap *b, mfs_size_t index, unsigned char value)
{
    if(index >= 0 && index < b->size)
    {
        *(b->bits + index) = value;
    }
    return;
}

unsigned char bitmap_get(struct bitmap *b, mfs_size_t index)
{
    if(index >= 0 && index < b->size)
    {
        return *(b->bits + index);
    }
    else return -1;
}

mfs_size_t bitmap_acquire(struct bitmap *b)
{
    for(mfs_size_t i = b->prev_pos; i < b->size; i++)
        if(*(b->bits + i) == (unsigned char)0)
            return b->prev_pos = i;
    for(mfs_size_t i = 0; i < b->prev_pos; i++)
        if(*(b->bits + i) == (unsigned char)0)
            return b->prev_pos = i;
    return -1;
}
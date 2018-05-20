#ifndef BITMAP_H
#define BITMAP_H

#include "config.h"

struct bitmap 
{
    mfs_size_t size;
    unsigned char *bits;
    mfs_size_t prev_pos;
};

void new_bitmap(struct bitmap *b, mfs_size_t size);
void free_bitmap(struct bitmap *b);
void bitmap_set(struct bitmap *b, mfs_size_t index, unsigned char value);
unsigned char bitmap_get(struct bitmap *b, mfs_size_t index);
mfs_size_t bitmap_acquire(struct bitmap *b);

#endif //BITMAP_H
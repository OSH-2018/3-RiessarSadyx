#ifndef BITMAP_H
#define BITMAP_H

struct bitmap 
{
    size_t size;
    unsigned char *bits;
    int prev_pos;
};

void new_bitmap(struct bitmap *b, size_t size);
void free_bitmap(struct bitmap *b);
void bitmap_set(struct bitmap *b, size_t index, unsigned char value);
unsigned char bitmap_get(struct bitmap *b, size_t index);
size_t bitmap_acquire(struct bitmap *b);

#endif //BITMAP_H
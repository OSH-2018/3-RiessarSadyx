#ifndef MFS_H
#define MFS_H

#include "config.h"
#include "bitmap.h"
#include <fuse.h>

#define INODE_SIZE 256
#define DIRECT_DATABLOCK_CNT ((INODE_SIZE - sizeof(struct stat) - sizeof(struct inode *)) / sizeof(void *) - 1) // if INODE_SIZE is 256, it should be 12

struct inode 
{
    struct stat st;
    struct inode *parent;
    void *direct_datablock[DIRECT_DATABLOCK_CNT];
    void *triple_indirect_datablock;
};

struct directory
{
    char filename[MAX_FILENAME + 1];
    struct inode *entrance;
};

struct bitmap inode_bitmap;
struct bitmap datablock_bitmap;
struct inode *inodes[BLK_CNT];
void *datablocks[BLK_CNT];

struct inode *root;

struct inode *new_inode();
void *new_datablock();

struct inode *search_directory(struct inode *dir, const char *filename);
struct inode *parse_path(struct inode *dir, const char *path);

void *mfs_init(struct fuse_conn_info *fci);
int mfs_getattr(const char *path, struct stat *stbuf);
int mfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
int mfs_open(const char *path, struct fuse_file_info *fi);
int mfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int mfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int mfs_mknod(const char *path, mode_t mode);

#endif //MFS_H
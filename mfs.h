#ifndef MFS_H
#define MFS_H

#include "config.h"
#include "bitmap.h"
#include <fuse.h>
#include <stdlib.h>

#define INODE_SIZE 512
#define DIRECT_DATABLOCK_CNT ((INODE_SIZE - MAX_FILENAME - sizeof(struct stat) - 3 * sizeof(struct inode *)) / sizeof(void *)) // if INODE_SIZE is 512 and MAX_FILENAME is 256, it should be 11

/*
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
*/

/*
struct inode_inner
{
	struct stat st; //sizeof(struct stat) is 144 bytes
	struct inode *parent;
	struct inode *next;
};

struct file_entry
{
	char name[MAX_FILENAME];
	struct inode *node;
	struct file_entry *next;
};

#define DIRECT_DATABLOCK_CNT ((INODE_SIZE - sizeof(struct inode_inner)) / sizeof(void *) - 1)
struct inode
{
	struct stat st; //sizeof(struct stat) is 144 bytes
	struct inode *parent;
	struct inode *next;

	union
	{
		struct
		{
			void *direct_datablock[DIRECT_DATABLOCK_CNT];
			void *triple_indirect_datablock;
		};

		struct
		{
			struct file_entry *child;
		};
	};
}
*/

struct inode
{
    struct stat st;     //sizeof(struct stat) is 144 bytes
    struct inode *parent;
    void *direct_datablock[DIRECT_DATABLOCK_CNT];
    char name[MAX_FILENAME];

	union
    {
		void *triple_indirect_datablock;
		struct inode *child;
	};
    struct inode *next;
};

struct bitmap inode_bitmap;
struct bitmap datablock_bitmap;
struct inode *inodes;
void *datablocks;

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
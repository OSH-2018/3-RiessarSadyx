#include "mfs.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

struct inode *new_inode()
{
    int index = bitmap_acquire(&inode_bitmap);
    if(index == -1)return NULL;

    struct inode *ni = inodes + index;
    memset(ni, 0, sizeof(struct inode));
    return ni;
}

void *new_datablock()
{
    int index = bitmap_acquire(&datablock_bitmap);
    if(index == -1)return NULL;

    void *nd = datablocks + index;
    memset(nd, 0, BLK_SIZE);
    return nd;
}

struct inode *search_directory(struct inode *dir, const char *filename)
{
	if (!S_ISDIR(dir->st.st_mode)) return NULL;	
	for (struct inode *n = dir->child; n != NULL; n = n->next)
		if (strcmp(n->name, filename)) return n;

	return NULL;
}

struct inode *parse_path(struct inode *dir, const char *path_)
{
	if (*path_ == '/') path_++;

	size_t len = strlen(path_);
	if (len == 0) return dir;
	
	char path[len + 1];
	strcpy(path, path_);

	char* path_ptr = path;
	while (dir != NULL && len > (path_ptr - path) && *path_ptr != 0)
	{
		char* cur = path_ptr;
		while (*cur != '/' && *cur != 0) cur++;
		*cur = 0;

		dir = search_directory(dir, path_ptr);
		path_ptr = cur + 1;
	}
	return dir;
}

void *mfs_init(struct fuse_conn_info *fci)
{
    new_bitmap(&inode_bitmap, BLK_CNT);
    new_bitmap(&datablock_bitmap, BLK_CNT);

    inodes = (struct inode *)mmap(NULL, BLK_CNT * sizeof(struct inode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    datablocks = (void *)mmap(NULL, MFS_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    root = new_inode();
    root->st.st_ino = root - inodes;
    root->st.st_mode = __S_IFDIR | 0755;
    root->st.st_nlink = 2;
    root->st.st_uid = fuse_get_context()->uid;
    root->st.st_gid = fuse_get_context()->gid;
    root->st.st_size = 0;
    root->st.st_blksize = BLK_SIZE;
    root->st.st_blocks = 1;
    
    root->parent = root;

    root->direct_datablock[0] = new_datablock();
    
    return NULL;
}

int mfs_getattr(const char *path, struct stat *stbuf)
{
    struct inode *entrance = parse_path(root, path);

    if(!entrance)return -ENOENT;
    
    memcpy(stbuf, &entrance->st, sizeof(struct stat));
    return 0;
}

int mfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    struct inode *entrance = parse_path(root, path);

    if(!entrance)return -ENOENT;
    if(!S_ISDIR(entrance->st.st_mode))return -ENOTDIR;

    filler(buf, ".", &entrance->st, 0);
    filler(buf, "..", &entrance->parent->st, 0);

    for (struct inode *n = entrance->child; n != NULL; n = n->next)
		filler(buf, n->name, &n->st, 0);
    return 0;
}

int mfs_open(const char *path, struct fuse_file_info *fi)
{
	if (fi->flags != 0) return -EADDRINUSE;

	struct inode *node = parse_path(root, path);
	if (node == NULL) return -ENOENT;

	fi->fh = (uint64_t) node;
	fi->flags = 1;

    return 0;
}

int mfs_release(const char *path, struct fuse_file_info *fi)
{
    if(fi->flags == 0)return -EBADFD;

    fi->fh == NULL;
    fi->flags = 0;

    return 0;
}

int mfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	if (fi->flags == 0) return -EBADFD;
	struct inode *entrance = (struct inode*) fi->fh;
	
    if(S_ISDIR(entrance->))
}

int mfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	if (fi->flags == 0) return -EBADFD;
	struct inode* node = (struct inode*) fi->fh;

	struct inode *node = ()
}

int mfs_mknod(const char *path, mode_t mode)
{

}

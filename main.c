#include "mfs.h"

static const struct fuse_operations mfs_op =
{
    .init = mfs_init,
    .getattr = mfs_getattr,
    .readdir = mfs_readdir,
    .open = mfs_open,
    .read = mfs_read,
    .write = mfs_write,
    .mknod =mfs_mknod,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc,argv, &mfs_op, NULL);
}
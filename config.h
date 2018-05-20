#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define FUSE_USE_VERSION 26
#define _FILE_OFFSET_BITS 64

#define MFS_SIZE ((long long)4 * 1024 * 1024 * 1024)
#define BLK_SIZE 4096
#define BLK_CNT (MFS_SIZE / BLK_SIZE)
#define MAX_FILENAME 256

typedef uint64_t mfs_size_t;
 
#endif //CONFIG_H
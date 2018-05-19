#ifndef CONFIG_H
#define CONFIG_H

#define FUSE_USE_VERSION 26
#define _FILE_OFFSET_BITS 64

#define MFS_SIZE (size_t)(4 * 1024 * 1024 * 1024)
#define BLK_SIZE (size_t)4096
#define BLK_CNT (MFS_SIZE / BLK_SIZE)
#define MAX_FILENAME 191
 
#endif //CONFIG_H
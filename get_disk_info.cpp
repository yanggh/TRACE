#include <stdio.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#include <errno.h>
#include "get_disk_info.h"

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
    extern "C" { /* 如果没有采用C++，顺序预编译 */
#endif
/* 采用C编译器编译的C语言代码段 */
long get_disk_percent(const char* disk)
{
    struct statfs disk_info;
    int ret = 0;

    if ((ret = statfs(disk, &disk_info)) == -1)
    {
	    fprintf(stderr, "Failed to get file disk infomation,errno:%u", errno);
	    return -1;
    }
    long total_size = disk_info.f_blocks * disk_info.f_bsize;
    long available_size = disk_info.f_bavail * disk_info.f_bsize;
    long free_size = disk_info.f_bfree * disk_info.f_bsize;

    long percent = free_size * 100/ total_size;

    return percent;
}
#ifdef __cplusplus /* 结束使用C编译器 */
    }
#endif

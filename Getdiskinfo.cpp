#include <stdio.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#include <errno.h>
#include "Getdiskinfo.h"

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
    extern "C" { /* 如果没有采用C++，顺序预编译 */
#endif
/* 采用C编译器编译的C语言代码段 */
int get_disk_percent(const char* disk)
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

    int percent = (total_size - free_size) * 100/ (total_size - free_size + available_size) + 1;

    return percent;
}
#ifdef __cplusplus /* 结束使用C编译器 */
    }
#endif

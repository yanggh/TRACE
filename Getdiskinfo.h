#ifndef __GET_DISK_INFO__
#define __GET_DISK_INFO__
#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
    extern "C" { /* 如果没有采用C++，顺序预编译 */
#endif
/* 采用C编译器编译的C语言代码段 */
int get_disk_percent(const char* disk);
#ifdef __cplusplus /* 结束使用C编译器 */
    }
#endif
#endif

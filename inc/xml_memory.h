/*
 * xml_memory.h
 *
 *  Created on: 2022年3月3日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2022-3-3   xfwangqiang     创建
 *  V1.0.1  2022-3-6   xfwangqiang     增加了XML_MEM_CHECK宏，用于使能xml软件栈对内存的检查功能
 *========================================================*/

#include "xml_typedefs.h"
#include "xml_config.h"

// 定义内存操作抽象的数据结构

#if (OS_VER == OS_RTTHREAD)
#include <rtthread.h>
struct xmlmemops
{
    void *(*malloc)(rt_size_t size);
    void (*free)(void *ptr);
    void *(*memset)(void *s, int c, rt_ubase_t count);
};
#else
struct xmlmemops
{
    void *(*malloc)(size_t size);
    void (*free)(void *ptr);
    void *(*memset)(void *s, int ch, size_t n);
};
#endif

#ifdef XML_MEM_CHECK
/** 开启XML_MEM_CHECK宏后，可以使能对动态内存的检查，会统计共申请了多少片内存
 *  空间，一共占用多少字节，通过函数xml_memory_show可以打印出情况。
 */
struct xmlmemblock {
    void *ptr;
    int size;
    struct xmlmemblock *next;
};
#endif /* XML_MEM_CHECK */


/**
 * @brief    配置一个内存抽象操作集，用于从外部设置内存操作函数
 *
 * @param ops 函数集
 */
void xml_mconfig(struct xmlmemops *ops);

/**
 * @brief    在Log中打印内存占用情况
 *
 */
void xml_memory_show(void);

/**
 * @brief    动态分配一个内存块
 *
 * @param size 目录容量
 *
 * @return   内存块指针
 */
void *xml_malloc(int size);

/**
 * @brief    释放一个内存块
 *
 * @param ptr 内存块指针
 */
void xml_free(void *ptr);

/**
 * @brief  将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s
 *
 * @param s 内存块指针
 * @param ch 目标值
 * @param n 内存块大小
 */
void *xml_memset(void *s, int ch, size_t n);
/*
 * xml_memory.c
 *
 *  Created on: 2022年3月3日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2022-3-3   xfwangqiang     创建
 *========================================================*/

#include "../inc/xml_memory.h"
#include "../inc/xml_config.h"

#if (OS_VER == OS_RTTHREAD)
#include <rtthread.h>
#define local_malloc rt_malloc
#define local_free rt_free
#define local_memset rt_memset
#else
#include <stdlib.h>
#include <memory.h>
#define local_malloc malloc
#define local_free free
#define local_memset memset
#endif

static struct xmlmemops ops = {
    .malloc = local_malloc,
    .free = local_free,
    .memset = local_memset,
};
static struct xmlmemops *xml_ops = &ops;

/**
 * @brief    配置一个内存抽象操作集，用于从外部设置内存操作函数
 *
 * @param ops 函数集
 */
void xml_mconfig(struct xmlmemops *ops)
{
    xml_ops = ops;
}

/**
 * @brief    动态分配一个内存块
 *
 * @param size 目录容量
 *
 * @return   内存块指针
 */
void *xml_malloc(int size)
{
    if (0 == size)
    {
        return NULL;
    }
    return xml_ops->malloc(size);
}

/**
 * @brief    释放一个内存块
 *
 * @param ptr 内存块指针
 */
void xml_free(void *ptr)
{
    if (NULL != ptr)
    {
        xml_ops->free(ptr);
    }
}

/**
 * @brief  将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s
 *
 * @param s 内存块指针
 * @param ch 目标值
 * @param n 内存块大小
 */
void *xml_memset(void *s, int ch, size_t n)
{
    if (NULL != s)
    {
        return xml_ops->memset(s, ch, n);
    }
    return NULL;
}
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
 *========================================================*/

#include "xml_typedefs.h"
#include "xml_config.h"

// 定义内存操作抽象的数据结构
struct xmlmemops
{
    void *(*malloc)(size_t size);
    void (*free)(void *ptr);
    void *(*memset)(void *s, int ch, size_t n);
};

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
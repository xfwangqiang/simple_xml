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
 *  V1.0.1  2022-3-6   xfwangqiang     增加了XML_MEM_CHECK宏，用于使能xml软件栈对内存的检查功能
 *========================================================*/

#include "../inc/xml_memory.h"
#include "../inc/xml_config.h"
#include "../inc/xml_log.h"

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

#ifdef XML_MEM_CHECK
static int xml_block_check_ptr(char *ptr1, char *ptr2)
{
    return ptr1 == ptr2;
}

static struct xmlmemblock *header = NULL;
static struct xmlmemblock *laster = NULL;
static int block_num = 0;
static int total = 0;
static void xml_block_create(void *ptr, int size)
{
    struct xmlmemblock *temp;
    if (NULL == laster)
    {
        laster = xml_ops->malloc(sizeof(struct xmlmemblock));
        if (NULL == laster)
        {
            xml_log("Can't alloc a memory block for check");
            return;
        }
        header = laster;
    }
    else
    {
        temp = xml_ops->malloc(sizeof(struct xmlmemblock));
        if (NULL == temp)
        {
            xml_log("Can't alloc a memory block for check");
            return;
        }
        laster->next = temp;
        laster = temp;
    }
    laster->ptr = ptr;
    laster->size = size;
    laster->next = NULL;
    block_num++;
    total += size;
}

static void xml_block_delete(void *ptr)
{
    struct xmlmemblock *temp = header;
    struct xmlmemblock *index = header;

    for (; NULL != index; index = index->next)
    {
        if (xml_block_check_ptr(index->ptr, ptr))
        {
            if (temp == index)
            {
                header = index->next;
            }
            else
            {
                temp->next = index->next;
            }
            block_num--;
            total -= index->size;
            xml_ops->free(index);
            break;
        }
        temp = (index != temp) ? index : temp;
    }
}
/**
 * @brief    在Log中打印内存占用情况
 *
 */
void xml_memory_show(void)
{
    xml_log("\nMemory in xml stack\n\ttotal : %d\n", total);
    xml_log("\tblocks : %d\n", block_num);
}

#else /* XML_MEM_CHECK */
/**
 * @brief    在Log中打印内存占用情况
 *
 */
void xml_memory_show(void)
{

}

#endif /* XML_MEM_CHECK */

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
    #ifdef XML_MEM_CHECK
    void *ptr = xml_ops->malloc(size);
    if (NULL != ptr)
    {
        xml_block_create(ptr, size);
    }
    return ptr;
    #else
    return xml_ops->malloc(size);
    #endif
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
        #ifdef XML_MEM_CHECK
        xml_block_delete(ptr);
        #endif /* XML_MEM_CHECK */
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
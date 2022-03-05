/*
 * xml_file.c
 *
 *  Created on: 2022年2月19日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2022-02-19  xfwangqiang     创建
 *========================================================*/

#include "../inc/xml_file.h"



#if (OS_VER == OS_WIN)

void *local_open(char *file_name, char *mode)
{
    FILE *file = NULL;
    int no = fopen_s(&file, file_name, mode);
    if ((NULL == file) || (no < 0))
    {
        return NULL;
    }
    return file;
}

char *local_gets(char *buf, int size, void *file)
{
    return fgets(buf, size, file);
}

int local_puts(char *buf, void *file)
{
    return fputs(buf, file);
}

void local_close(void *file)
{
    fclose(file);
}
#elif ((OS_VER == OS_LINUX) || (OS_VER == OS_VXWORKS))
#include <stdio.h>
void *local_open(char *file_name, char *mode)
{
    FILE *file = fopen(file_name, mode);
    return file;
}

char *local_gets(char *buf, int size, void *file)
{
    return fgets(buf, size, file);
}

int local_puts(char *buf, void *file)
{
    return fputs(buf, file);
}

void local_close(void *file)
{
    fclose(file);
}
#elif (OS_VER == OS_RTTHREAD)
#include <rtthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "../inc/xml_string.h"
void *local_open(char *file_name, char *mode)
{
    int flags = O_RDONLY;
    if (0 == xml_strcmp(mode, "w+"))
    {
        flags = O_RDWR | O_CREAT;
    }
    int fd = open(file_name, flags);
    return (void *)fd;
}

char *local_gets(char *buf, int size, void *file)
{
    int fd = (int)file;
    size = read(fd, buf, size);
    if (size > 0)
    {
        return buf;
    }
    return NULL;
}

int local_puts(char *buf, void *file)
{
    int fd = (int)file;
    return write(fd, buf, xml_strlen(buf));
}

void local_close(void *file)
{
    int fd = (int)file;
    close(fd);
}
#else

void *local_open(char *file_name, char *mode)
{
    return NULL;
}
char *local_gets(char *buf, int size, void *file)
{
    return NULL;
}

int local_puts(char *buf, void *file)
{
    return 0;
}

void local_close(void *file)
{
}
#endif

static struct xmlfileops ops = {
    .open = local_open,
    .gets = local_gets,
    .puts = local_puts,
    .close = local_close};

static struct xmlfileops *xml_ops = &ops;

/**
 * @brief    配置一个文件抽象操作集，用于从外部设置文件操作函数
 *
 * @param ops 函数集
 */
void xml_fconfig(struct xmlfileops *ops)
{
    xml_ops = ops;
}

/**
 * @brief    打开一个文件，需要指定文件名及模式
 *
 * @param file_name 文件名
 * @param mode      模式
 *
 * @return   一个文件对象或句柄
 */
void *xml_fopen(char *file_name, char *mode)
{
    return xml_ops->open(file_name, mode);
}

/**
 * @brief    从文件读取一行到Buffer中
 *
 * @param buf  buffer
 * @param size buffer的长度
 * @param file 文件对象或句柄
 *
 * @return 字符串的指针
 */
char *xml_fgets(char *buf, int size, void *file)
{
    return xml_ops->gets(buf, size, file);
}

/**
 * @brief    将Buffer中的数据写入到文件
 *
 * @param buf  buffer
 * @param size buffer的长度
 * @param file 文件对象或句柄
 *
 * @return 写入的长度
 */
int xml_fputs(char *buf, void *file)
{
    return xml_ops->puts(buf, file);
}

/**
 * @brief    关闭文件
 *
 * @param file 文件对象或句柄
 */
void xml_fclose(void *file)
{
    xml_ops->close(file);
}
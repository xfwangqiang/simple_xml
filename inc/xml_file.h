/*
 * xml_file.h
 *
 *  Created on: 2022年2月19日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2022-02-19  xfwangqiang     创建
 *========================================================*/

#include "xml_typedefs.h"
#include "xml_config.h"

// 定义文件操作抽象的数据结构
struct xmlfileops {
    void * (*open)(char *file_name, char *mode);
    char * (*gets)(char *buf, int size, void *file);
    int (*puts)(char *buf, void *file);
    void (*close)(void *file);
};


/**
 * @brief    配置一个文件抽象操作集，用于从外部设置文件操作函数
 *
 * @param ops 函数集
 */
void xml_fconfig(struct xmlfileops *ops);

/**
 * @brief    打开一个文件，需要指定文件名及模式
 *
 * @param file_name 文件名
 * @param mode      模式
 *
 * @return   一个文件对象或句柄
 */
void *xml_fopen(char *file_name, char *mode);


/**
 * @brief    从文件读取一行到Buffer中
 *
 * @param buf  buffer
 * @param size buffer的长度
 * @param file 文件对象或句柄
 *
 * @return 字符串的指针
 */
char *xml_fgets(char *buf, int size, void *file);

/**
 * @brief    将Buffer中的数据写入到文件
 *
 * @param buf  buffer
 * @param size buffer的长度
 * @param file 文件对象或句柄
 *
 * @return 写入的长度
 */
int xml_fputs(char *buf, void *file);

/**
 * @brief    将Buffer中的数据写入到文件
 *
 * @param file 文件对象或句柄
 * @param buf  buffer
 * @param size buffer的长度
 *
 * @return 写入的长度
 */
int xml_fwrite(void *file, void *buf, int size);

/**
 * @brief    关闭文件
 *
 * @param file 文件对象或句柄
 */
void xml_fclose(void *file);
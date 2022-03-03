/*
 * xml_typedefs.h
 *
 *  Created on: 2022年3月3日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2022-3-3   xfwangqiang     创建
 *========================================================*/

#ifndef XML_TYPEDEFS_H
#define XML_TYPEDEFS_H


#include "xml_config.h"

#if (OS_VER == OS_RTTHREAD)
#else
#include <stdlib.h>
#endif

#ifndef NULL
#define NULL    ((void *)0)
#endif

#define XML_NULL    NULL


#endif /* XML_TYPEDEFS_H */
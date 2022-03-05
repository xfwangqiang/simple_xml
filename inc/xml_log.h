/*
 * xml_log.h
 *
 *  Created on: 2022年3月3日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2022-3-3    xfwangqiang     创建
 *========================================================*/


#ifndef XML_LOG_H
#define XML_LOG_H

#include "xml_typedefs.h"
#include "xml_config.h"

#if (OS_VER == OS_RTTHREAD)
#include <rtthread.h>
#define xml_log rt_kprintf
#else
#include <stdio.h>
#define xml_log printf
#endif

#endif /* XML_LOG_H */
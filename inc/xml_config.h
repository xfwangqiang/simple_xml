
#ifndef XML_CONFIG_H
#define XML_CONFIG_H

#ifdef USER_CONFIG
#include "USER_CONFIG"
#else

#define OS_WIN 0
#define OS_LINUX 1
#define OS_VXWORKS 2
#define OS_RTTHREAD 3
#define OS_UNKOWN 4

#ifndef OS_VER
//#define OS_VER		(OS_WIN)
// #define OS_VER       (OS_LINUX)
//#define OS_VER		(OS_VXWORKS)
#define OS_VER		(OS_RTTHREAD)
// #define OS_VER		(OS_UNKOWN)
#endif
#endif

/* XML配置 */

/* 默认浮点数属性的小数点位数 */
#define XML_FLOAT_ATTRIBUTE_POINT_NUM  (2)

/* XML配置结束 */

#endif /* XML_CONFIG_H */

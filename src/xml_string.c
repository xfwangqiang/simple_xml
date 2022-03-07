/*
 * xml_string.c
 *
 *  Created on: 2017年5月23日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2017-06-05  xfwangqiang     创建
 *  V1.0.0  2017-06-15  xfwangqiang     增加xml_tolower的函数
 *		增加了xml_strtohex的函数
 *		增加了xml_ishex的函数
 *		增加了xml_isdigit的函数
 *		优化了函数中对指针的检测
 *  v1.0.1  2018-09-16 xfwangqiang      修改了xml_strsplit
 *      当字符串列表只有一个时返回值的错误的bug
 *  V1.0.2  2019-10-29 xfwangqiang      增加了xml_strcatreverse函数
 *      优化了xml_ishex函数的代码
 *      优化了xml_strtohex函数的代码
 *      增加了xml_strtofloat函数的代码
 *  V1.0.3  2020-07-24 xfwangqiang      增加了xml_strfind函数
 *      增加了static函数get_split_index
 *      优化了xml_strsplitlist函数的代码
 *      优化了xml_strsplit函数的代码
 *  V1.0.4  2022-03-07 xfwangqiang      增加了xml_inttostr函数
 *      增加了xml_hextostr函数
 *      将xml_getfloatstr函数变更为xml_floattostr函数，增加了最大小数点位数
 *      增加了xml_strreverse函数
 *========================================================*/

#include "../inc/xml_string.h"
#include "../inc/xml.h"
#include "../inc/xml_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return the size of string, don't include '\0'
int xml_strlen( char *src )
{
	int size = 0;
	for ( ; ('\0' != *src) && (NULL != src); src++, size++ )
	{
		;
	}
	return size;
}


// copy string from src to des, return index of copy char
int xml_strcpy( char *des, char *src )
{
	int index = 0;
	if ((NULL == des) || (NULL == src))
	{
		return 0;
	}
	for ( ; '\0' != *src; src++, des++, index++ )
	{
		*des = *src;
	}
	*des = '\0';
	return index;
}

// copy string from src to des, return index of copy char
int xml_strncpy( char *des, char *src, int size )
{
	int index = 0;
	if ((NULL == des) || (NULL == src))
	{
		return 0;
	}
	for ( ; ('\0' != *src) && (size > 0); src++, des++, index++, size-- )
	{
		*des = *src;
	}
	*des = '\0';
	return index;
}

// copy string from src to des, return index of copy char
int xml_strcat( char *des, char *src )
{
	int index = 0;
	if ((NULL == des) || (NULL == src))
	{
		return 0;
	}

	for ( ; '\0' != *des; des++ )
	{
		;
	}

	for ( ; '\0' != *src; src++, des++, index++ )
	{
		*des = *src;
	}
	*des = '\0';
	return index;
}

// reverse copy string from src to des, return index of copy char
int xml_strcatreverse(char *des, char *src)
{
	int index = 0;
	char *p_src = src;
	if ((NULL == des) || (NULL == src))
	{
		return 0;
	}

	for (; '\0' != *des; des++)
	{
		;
	}

	for (; '\0' != *p_src; p_src++)
	{
		;
	}
	for (p_src--; '\0' != *src; src++, des++, index++, p_src--)
	{
		*des = *p_src;
	}
	*des = '\0';
	return index;
}

// copy string from src to des, return index of copy char
int xml_strncat( char *des, char *src, int size )
{
	int index = 0;

	if ((NULL == des) || (NULL == src))
	{
		return 0;
	}
	for ( ; '\0' != *des; des++ )
	{
		;
	}

	for ( ; ('\0' != *src) && (size > 0); src++, des++, index++, size-- )
	{
		*des = *src;
	}
	*des = '\0';
	return index;
}

// search the identifier in string
int xml_searchname( char *string, char *name, int size )
{
	int len = 0;
	for ( ; (0 == xml_checkidentify( *string )) && ('\0' != *string); string++ )
	{
		;
	}
	for ( ;(1 == xml_checkidentify( *string )) && ('\0' != *string) && (size > 0) ;  string++, name++, size--  )
	{
		*name = *string;
		len++;
	}
	*name = '\0';
	return len;
}


// check the identifier char
int xml_checkidentify( char c )
{
	if ( (c >= 'a') && (c <= 'z'))
	{
		return 1;
	}
	else if ( (c >= 'A') && (c <= 'Z'))
	{
		return 1;
	}
	else if ( ('_' == c) || (':' == c) )
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 函数名称：xml_strfind
// 函数功能：搜索字符串中目标字符串的位置
//
// 输入参数： 1 -- 源字符串
//           2 -- offset
//           3 -- 目标字符串
// 返回值：目标字符串的位置
// 说明：搜索字符串中目标字符串的位置
//============================================================================
int xml_strfind(char *string, int offset, char *des)
{
    char *p_string = NULL;
    char *p_des = NULL;
    if ((NULL == string) || (NULL == des) || ('\0' == *string) || ('\0' == *des))
    {
        return -1;
    }
    for (string += offset; '\0' != *string; string++, offset++)
    {
        if (*string == *des)
        {
            p_string = string;
            p_des = des;
            for ( ;('\0' != *p_string) && ('\0' != *p_des) && (*p_string == *p_des); )
            {
                p_string++;
                p_des++;
            }
            if ('\0' == *p_des)
            {
                return offset;
            }
        }
    }
    return -1;
}

//============================================================================
// 函数名称：xml_strindexof
// 函数功能：得到字符串索引
//
// 输入参数： 1 -- 字符串
//			2 -- 偏移
//			3 -- 字符
// 输出参数：
// 返回值：目标字符的索引值
// 说明：得到字符串索引
//============================================================================
int xml_strindexof( char *string, int offset, char c )
{
	string += offset;
	for ( ;'\0' != *string; string++, offset++ )
	{
		if ( *string == c )
		{
			break;
		}
	}
	if ( '\0' == *string )
	{
		offset = -1;
	}
	return offset;
}


static int get_split_index( char *string, int offset, char c )
{
    int i[4] = { 0 };
    int index, ret;

    if ('\0' == c)
    {
        i[0] = xml_strindexof(string, offset, ' ');
        i[1] = xml_strindexof(string, offset, '\t');
        i[2] = xml_strindexof(string, offset, '\r');
        i[3] = xml_strindexof(string, offset, '\n');
        for (index = 1; index < 4; index++)
        {
            if (i[index] >= 0)
            {
                if (i[0] >= 0)
                {
                    i[0] = i[0] < i[index] ? i[0] : i[index];
                }
                else
                {
                    i[0] = i[index];
                }
            }
        }
        ret = i[0];
    }
    else
    {
        ret = xml_strindexof(string, offset, c);
    }
    return ret;
}

//============================================================================
// 函数名称：xml_strsplitlist
// 函数功能：使用特定字符分割字符得到字符串列表中指定的字符
//
// 输入参数： 1 -- 字符串
// 			2 -- 目标字符
//			3 -- 字符串列表的索引
// 输出参数： 4 -- 得到的字符串
// 返回值：得到字符串的长度
// 说明：使用特定字符分割字符得到字符串列表中指定的字符
//============================================================================
int xml_strsplitlist( char *string, char c, int index, char *buffer )
{
	int start = 0;
	int end = -1;
	for ( ; index >= 0; index-- )
	{
		start = end + 1;
        end = get_split_index(string, start, c);
        if ( -1 == end )
		{
			end = xml_strlen( string );
		}
	}
	return xml_strncpy( buffer, string + start, end - start);
}


//============================================================================
// 函数名称：xml_strsplit
// 函数功能：使用特定字符分割字符得到字符串列表中元素个数
//
// 输入参数： 1 -- 字符串
// 			2 -- 目标字符
// 输出参数：
// 返回值：得到字符串列表中元素个数
// 说明：使用特定字符分割字符得到字符串列表中元素个数
//============================================================================
int xml_strsplit( char *string, char c )
{
	int index = 0;
	int num = 0;
	for ( ; ; )
	{
        index = get_split_index(string, index, c);
        if ( -1 == index )
		{
			num++;
			break;
		}
		else
		{
			index++;
			num++;
		}
	}
	return num;
}


//============================================================================
// 函数名称：xml_strtrim
// 函数功能：去掉首尾的空格
//
// 输入参数： 1 -- 去年空格前的字符串
// 输出参数： 2 -- 去掉空格后的字符串
// 返回值：去掉后字符串的长度
// 说明：去掉首尾的空格
//============================================================================
int  xml_strtrim( char *string, char *buffer )
{
	int size = 0;
	for ( ; (' ' == *string) && ('\0' != *string); string++ )
	{
		;
	}
	for ( ; (' ' != *string) && ('\0' != *string) ; string++, buffer++ )
	{
		*buffer = *string;
		size++;
	}
	*buffer = '\0';
	return size;
}



//============================================================================
// 函数名称：xml_strnew
// 函数功能：新建一个字符串
//
// 输入参数： 1 -- 源字符串
// 返回值：新建字符串的地址
// 说明：在堆中开辟一块同样大小的内存块，将源字符串的内容拷备到新的字符串里，返回新的字符串地址，
// 使用完毕后一定要记得释放，否则会发生内存泄漏.
//============================================================================
char *xml_strnew( char *string )
{
	int size;
	char *newstr;
	if ( (NULL == string) || ('\0' == string[0]) )
	{
		return NULL;
	}
	size = xml_strlen( string );
	newstr = (char *)xml_malloc( size + 1 );
	xml_strcpy( newstr, string );
	return newstr;
}



//============================================================================
// 函数名称：xml_strcmp
// 函数功能：字符串比较
//
// 输入参数： 1 -- 源字符串1
//			  2 -- 源字符串2
// 返回值：0 -- 相等
// 说明：
//============================================================================
int xml_strcmp( char *str1, char *str2 )
{
	int tmp;

    if (str1 == str2) return(0);
    if (str1 == NULL) return(-1);
    if (str2 == NULL) return(1);
    do {
        tmp = *str1++ - *str2;
        if (tmp != 0) return(tmp);
    } while (*str2++ != 0);
	return 0;
}

//============================================================================
// 函数名称：xml_strreverse
// 函数功能：字符串前后倒换
//
// 输入参数： 1 -- 源字符串1
// 返回值：0 -- 字符串长度
// 说明：
//============================================================================
int xml_strreverse(char *str)
{
    int size = xml_strlen(str);
    int index, end;
    int temp = 0;

    if (size <= 0)
    {
        return 0;
    }
    for (index = 0, end = size - 1 - index; index < end; index++, end--)
    {
        temp = str[index];
        str[index] = str[end];
        str[end] = temp;
    }
    return size;
}



//============================================================================
// 函数名称：xml_isint
// 函数功能：判断字符串是否为整型
//
// 输入参数： 1 -- 字符串
// 返回值：0 -- 不是 1 -- 是
// 说明：判断字符串是否为整型
//============================================================================
int xml_isint(char *strvalue)
{
	if ((NULL == strvalue) || ('\0' == strvalue[0]) )
	{
		return 0;
	}
	if (('+' == *strvalue) || ('-' == *strvalue))
	{
		strvalue++;
	}
	forstrloop(strvalue)
	{
		if ( !xml_isdigit( *strvalue ) )
		{
			return 0;
		}
	}
	return 1;
}



//============================================================================
// 函数名称：xml_ishex
// 函数功能：判断字符串是否为16进制整型
//
// 输入参数： 1 -- 字符串
// 返回值：0 -- 不是 1 -- 是
// 说明：判断字符串是否为16进制整型
//============================================================================
int xml_ishex( char *strvalue )
{
	char c;
	if ((NULL == strvalue) || ('\0' == strvalue[0]) )
	{
		return 0;
	}
	if ('0' != *(strvalue++))
	{
		return 0;
	}
	if ('x' != *(strvalue++))
	{
		return 0;
	}

	forstrloop(strvalue)
	{
		c = xml_tolower( *strvalue );
		if ( !xml_isdigit( c ) && ((c < 'a') || (c > 'f')))
		{
			return 0;
		}
	}
	return 1;
}


//============================================================================
// 函数名称：xml_isfloat
// 函数功能：判断字符串是否为浮点型
//
// 输入参数： 1 -- 字符串
// 返回值：0 -- 不是 1 -- 是
// 说明：判断字符串是否为浮点型
//============================================================================
int xml_isfloat(char *strvalue)
{
	int point = 0;
	if ((NULL == strvalue) || ('\0' == *strvalue))
	{
		return 0;
	}

	if (('+' == *strvalue) || ('-' == *strvalue))
	{
		strvalue++;
	}
	forstrloop(strvalue)
	{
		if (((*strvalue < '0') || (*strvalue > '9')) && ('.' != *strvalue))
		{
			return 0;
		}
		else if ('.' == *strvalue)
		{
			if (point >= 1)
			{
				return 0;
			}
			point++;
		}
	}
	return 1;
}

//============================================================================
// 函数名称：xml_strtoint
// 函数功能：将字符串转换为整型
//
// 输入参数： 1 -- 字符串
// 返回值：整形值
// 说明：将字符串转换为整型
//============================================================================
int xml_strtoint(char *strvalue)
{
	int value = 0;
	int sign = 1;
	if ('-' == *strvalue)
	{
		sign = -1;
		strvalue++;
	}
	else if ( '+' == *strvalue)
	{
		strvalue++;
	}

	forstrloop( strvalue )
	{
		value *= 10;
		value += *strvalue - '0';
	}
	return value * sign;
}

//============================================================================
// 函数名称：xml_inttostr
// 函数功能：将整型数值以十进制转换成字符串
//
// 输入参数： 1 -- 数值
//           2 -- buffer
//           3 -- buffer的长度
// 返回值：数值长度
// 说明：将整型数值以十进制转换成字符串，例如value = 100, buffer = "100"
//============================================================================
int xml_inttostr(int value, char *buffer, int size)
{
    int index, offset;
    char *ptr = NULL;
    if (0 == size)
    {
        return 0;
    }
    offset = 0;
    if (value < 0)
    {
        *(buffer++) = '-';
        size -= 1;
        value *= -1;
        offset = 1;
    }
    ptr = buffer;
    size -= 1;
    for (index = 0; index < size; index++)
    {
        *(buffer++) = (value % 10) + '0';
        value /= 10;
        if (0 == value)
        {
            break;
        }
    }
    *buffer = '\0';
    xml_strreverse(ptr);
    return index + 1 + offset;
}

//============================================================================
// 函数名称：xml_strtohex
// 函数功能：将字符串转换为整型
//
// 输入参数： 1 -- 字符串
// 返回值：整形值
// 说明：将字符串转换为整型
//============================================================================
int xml_strtohex( char *strvalue )
{
	char c = 0;
	int value = 0;
	strvalue += 2;
	forstrloop( strvalue )
	{
		value *= 16;
		c = xml_tolower( *strvalue );
		value += xml_isdigit(c) ? c - '0' : c - 'a' + 10;
	}
	return value;
}

//============================================================================
// 函数名称：xml_hextostr
// 函数功能：将整型数值以十六进制转换成字符串
//
// 输入参数： 1 -- 数值
//           2 -- buffer
//           3 -- buffer的长度
// 返回值：数值长度
// 说明：将整型数值以十六进制转换成字符串，例如value = 0x100, buffer = "0x100"
//============================================================================
int xml_hextostr(int value, char *buffer, int size)
{
    int index, temp, offset;
    unsigned int uvalue = (unsigned long)value;
    char *ptr = NULL;
    if (size <= 2)
    {
        return 0;
    }
    *(buffer++) = '0';
    *(buffer++) = 'x';
    ptr = buffer;
    size -= 3;
    for (index = 0; index < size; index++)
    {
        temp = uvalue & 0x0f;
        if ((temp >= 0) && (temp <= 9))
        {
            *(buffer++) = temp + '0';
        }
        else
        {
            *(buffer++) = (temp - 10) + 'a';
        }
        uvalue >>= 4;
        if (0 == uvalue)
        {
            break;
        }
    }
    *buffer = '\0';
    xml_strreverse(ptr);
    return index + 3;
}

//============================================================================
// 函数名称：xml_strtofloat
// 函数功能：将字符串转换为浮点型
//
// 输入参数： 1 -- 字符串
// 返回值：浮点值
// 说明：将字符串转换为浮点型
//============================================================================
float xml_strtofloat(char *strvalue)
{
	float value = 0.0f;
	float unit = 0.1f;
	int sign = 1;
	if ('-' == *strvalue)
	{
		sign = -1;
		strvalue++;
	}
	else if ('+' == *strvalue)
	{
		strvalue++;
	}
	forstrloop(strvalue)
	{
		if ('.' == *strvalue)
		{
			break;
		}
		value *= 10;
		value += *strvalue - '0';
	}
	strvalue++;
	forstrloop(strvalue)
	{
		value += (*strvalue - '0') * unit;
		unit *= 0.1f;
	}
	return value * sign;
}

//============================================================================
// 函数名称：xml_floattostr
// 函数功能：将浮点数转为字符串
//
// 输入参数： 1 -- 浮点值
//           2 -- 小数点后保留位
//			 3 -- 字符串buffer
//           4 -- 字符串buffer长度
// 返回值：字符串长度
// 说明：将浮点数转为字符串
//============================================================================
int xml_floattostr(float value , int point, char *buffer, int size)
{
	char int_string[20] = { 0 };
	char float_string[20] = { 0 };
	int float_str_cnt = 0;
	int int_value, offset;
	float float_value;
	unsigned char byte_value;
	int sign = 1;

	if ( value < (-0.000001f) )
	{
		sign = -1;
        value *= -1;
	}

	int_value = (int)value;
	float_value = value - int_value;

    xml_inttostr(int_value, int_string, 20);
    for (; point != 0; point--)
    {
        float_value *= 10;
        byte_value = (int)(float_value);
        float_string[float_str_cnt++] = byte_value + '0';
        float_value -= byte_value;
        if ((float_value > (-0.000001f)) && (float_value < 0.000001f))
		{
			break;
		}
	}
	offset = 0;
	if ( -1 == sign )
	{
        offset = xml_strncpy(buffer, "-", size);
        offset += xml_strncat(buffer + offset, int_string, size - offset);
    }
    else
    {
        offset += xml_strncpy(buffer + offset, int_string, size - offset);
    }
    offset += xml_strncat(buffer + offset, ".", size - offset);
	offset += xml_strncat(buffer + offset, float_string, size - offset );
	return offset;
}



//============================================================================
// 函数名称：xml_tolower
// 函数功能：将字符转换为小写
//
// 输入参数： 1 -- 字符串
// 返回值：小写字符
// 说明：将字符转换为小写
//============================================================================
int xml_tolower(int c)
{
	int temp = 'a' - 'A';

	if ( (c < 'A') || (c > 'Z') )
	{
		return c;
	}
	return (c + temp);
}



//============================================================================
// 函数名称：xml_isdigit
// 函数功能：将字符是否为数字
//
// 输入参数： 1 -- 字符串
// 返回值：小写字符
// 说明：将字符转换为小写
//============================================================================
int xml_isdigit( int ch )
{
	if ( (ch >= '0') && (ch <= '9'))
	{
		return 1;
	}
	return 0;
}

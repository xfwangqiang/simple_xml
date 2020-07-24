/*
 * xml_string.c
 *
 *  Created on: 2017��5��23��
 *      Author: xfwangqiang
 */

/*========================================================*
 * �����޸ļ�¼��
 * <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
 *  V1.0.0  2017-06-05  xfwangqiang     ����
 *  V1.0.0  2017-06-15  xfwangqiang     ����xml_tolower�ĺ���
 *		������xml_strtohex�ĺ���
 *		������xml_ishex�ĺ���
 *		������xml_isdigit�ĺ���
 *		�Ż��˺����ж�ָ��ļ��
 *  v1.0.1  2018-09-16 xfwangqiang      �޸���xml_strsplit
 *      ���ַ����б�ֻ��һ��ʱ����ֵ�Ĵ����bug
 *  V1.0.2  2019-10-29 xfwangqiang      ������xml_strcatreverse����
 *      �Ż���xml_ishex�����Ĵ���
 *      �Ż���xml_strtohex�����Ĵ���
 *      ������xml_getfloatstr�����Ĵ���
 *  V1.0.3  2020-07-24 xfwangqiang      ������xml_strfind����
 *      ������static����get_split_index
 *      �Ż���xml_strsplitlist�����Ĵ���
 *      �Ż���xml_strsplit�����Ĵ���
 *========================================================*/


#include "xml_string.h"
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
// �������ƣ�xml_strfind
// �������ܣ������ַ�����Ŀ���ַ�����λ��
//
// ��������� 1 -- Դ�ַ���
//           2 -- offset
//           3 -- Ŀ���ַ���
// ����ֵ��Ŀ���ַ�����λ��
// ˵���������ַ�����Ŀ���ַ�����λ��
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
// �������ƣ�xml_strindexof
// �������ܣ��õ��ַ�������
//
// ��������� 1 -- �ַ���
//			2 -- ƫ��
//			3 -- �ַ�
// ���������
// ����ֵ��Ŀ���ַ�������ֵ
// ˵�����õ��ַ�������
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
// �������ƣ�xml_strsplitlist
// �������ܣ�ʹ���ض��ַ��ָ��ַ��õ��ַ����б���ָ�����ַ�
//
// ��������� 1 -- �ַ���
// 			2 -- Ŀ���ַ�
//			3 -- �ַ����б������
// ��������� 4 -- �õ����ַ���
// ����ֵ���õ��ַ����ĳ���
// ˵����ʹ���ض��ַ��ָ��ַ��õ��ַ����б���ָ�����ַ�
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
// �������ƣ�xml_strsplit
// �������ܣ�ʹ���ض��ַ��ָ��ַ��õ��ַ����б���Ԫ�ظ���
//
// ��������� 1 -- �ַ���
// 			2 -- Ŀ���ַ�
// ���������
// ����ֵ���õ��ַ����б���Ԫ�ظ���
// ˵����ʹ���ض��ַ��ָ��ַ��õ��ַ����б���Ԫ�ظ���
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
// �������ƣ�xml_strtrim
// �������ܣ�ȥ����β�Ŀո�
//
// ��������� 1 -- ȥ��ո�ǰ���ַ���
// ��������� 2 -- ȥ���ո����ַ���
// ����ֵ��ȥ�����ַ����ĳ���
// ˵����ȥ����β�Ŀո�
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
// �������ƣ�xml_strnew
// �������ܣ��½�һ���ַ���
//
// ��������� 1 -- Դ�ַ���
// ����ֵ���½��ַ����ĵ�ַ
// ˵�����ڶ��п���һ��ͬ����С���ڴ�飬��Դ�ַ��������ݿ������µ��ַ���������µ��ַ�����ַ��
// ʹ����Ϻ�һ��Ҫ�ǵ��ͷţ�����ᷢ���ڴ�й©.
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
	newstr = (char *)malloc( size + 1 );
	xml_strcpy( newstr, string );
	return newstr;
}



//============================================================================
// �������ƣ�xml_strcmp
// �������ܣ��ַ����Ƚ�
//
// ��������� 1 -- Դ�ַ���1
//			  2 -- Դ�ַ���2
// ����ֵ��0 -- ���
// ˵����
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
// �������ƣ�xml_isint
// �������ܣ��ж��ַ����Ƿ�Ϊ����
//
// ��������� 1 -- �ַ���
// ����ֵ��0 -- ���� 1 -- ��
// ˵�����ж��ַ����Ƿ�Ϊ����
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
// �������ƣ�xml_ishex
// �������ܣ��ж��ַ����Ƿ�Ϊ16��������
//
// ��������� 1 -- �ַ���
// ����ֵ��0 -- ���� 1 -- ��
// ˵�����ж��ַ����Ƿ�Ϊ16��������
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
// �������ƣ�xml_isfloat
// �������ܣ��ж��ַ����Ƿ�Ϊ������
//
// ��������� 1 -- �ַ���
// ����ֵ��0 -- ���� 1 -- ��
// ˵�����ж��ַ����Ƿ�Ϊ������
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
// �������ƣ�xml_strtoint
// �������ܣ����ַ���ת��Ϊ����
//
// ��������� 1 -- �ַ���
// ����ֵ������ֵ
// ˵�������ַ���ת��Ϊ����
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
// �������ƣ�xml_strtohex
// �������ܣ����ַ���ת��Ϊ����
//
// ��������� 1 -- �ַ���
// ����ֵ������ֵ
// ˵�������ַ���ת��Ϊ����
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
// �������ƣ�xml_strtofloat
// �������ܣ����ַ���ת��Ϊ������
//
// ��������� 1 -- �ַ���
// ����ֵ������ֵ
// ˵�������ַ���ת��Ϊ������
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
// �������ƣ�xml_getfloatstr
// �������ܣ���������תΪ�ַ���
//
// ��������� 1 -- buffer
//			 2 -- �ַ���
// ����ֵ������ֵ
// ˵������������תΪ�ַ���
//============================================================================
int xml_getfloatstr(char *buffer, float value )
{
	char int_string[20] = { 0 };
	int int_str_cnt = 0;
	char float_string[20] = { 0 };
	int float_str_cnt = 0;
	int int_value, offset;
	float float_value;
	unsigned char byte_value;
	int sign = 1;

	if ( value < (-0.000001f) )
	{
		sign = -1;
	}

	value *= sign;

	int_value = (int)value;
	float_value = value - int_value;

	for ( ;; )
	{
		byte_value = int_value % 10;
		if ( 0 == int_value )
		{
			break;
		}
		int_string[int_str_cnt++] = byte_value + '0';
		int_value /= 10;
	}
	if ( 0 == int_str_cnt )
	{
		int_str_cnt = 1;
		int_string[0] = '0';
	}
	for ( ;; )
	{
		byte_value = float_value * 10;
		if ((float_value > (-0.000001f)) && (float_value < 0.000001f))
		{
			break;
		}
		float_value -= byte_value;
		float_string[float_str_cnt++] = byte_value + '0';
	}
	if ( 0 == float_str_cnt )
	{
		float_str_cnt = 1;
		float_string[0] = '0';
	}
	offset = 0;
	if ( -1 == sign )
	{
		offset = xml_strcat(buffer, "-");
	}
	offset += xml_strcatreverse(buffer + offset, int_string );
	offset += xml_strcat(buffer + offset, ".");
	offset += xml_strcat(buffer + offset, float_string );
	return offset;
}



//============================================================================
// �������ƣ�xml_tolower
// �������ܣ����ַ�ת��ΪСд
//
// ��������� 1 -- �ַ���
// ����ֵ��Сд�ַ�
// ˵�������ַ�ת��ΪСд
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
// �������ƣ�xml_isdigit
// �������ܣ����ַ��Ƿ�Ϊ����
//
// ��������� 1 -- �ַ���
// ����ֵ��Сд�ַ�
// ˵�������ַ�ת��ΪСд
//============================================================================
int xml_isdigit( int ch )
{
	if ( (ch >= '0') && (ch <= '9'))
	{
		return 1;
	}
	return 0;
}

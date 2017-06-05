/*
 * xml_element.c
 *
 *  Created on: 2017年5月23日
 *      Author: xfwangqiang
 */


/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2017-06-05  xfwangqiang     创建
 *========================================================*/



#include <stdio.h>
#include <stdlib.h>
#include "xml_string.h"
#include "xml_element.h"
#include "xml_node.h"


// 声明本地函数
static int getattrvalue(char *string, char *buffer);
static struct xmlnode *setattribute(char *name, char *value);
static int getattrname(char *string, char *buffer);
static int getattrnum(char *string);


//============================================================================
// 函数名称：xmlelement_create
// 函数功能：创建一个元素节点
//
// 输入参数： 1 -- 元素对象名字
//			2 -- 元素对象文本
// 输出参数：
// 返回值：元素节点对象
// 说明：创建一个元素节点
//============================================================================
struct xmlelement *xmlelement_create( char *string, char *text, enum xmlnode_type type )
{
	struct xmlelement *element = NULL;
	char name[128] = { 0 };
	int listlen = xml_strsplit( string, ' ' );
	if ( listlen > 1 )
	{
		xml_strsplitlist( string, ' ', 0, name );
		element = (struct xmlelement *)malloc( sizeof(struct xmlelement) );
		xmlnode_init( element, xml_strnew(name), NULL, type );
		element->attribute = NULL;
		xmlelement_parserattr( element, string );
	}
	else
	{
		element = (struct xmlelement *)malloc( sizeof(struct xmlelement) );
		xmlnode_init( element, xml_strnew(string), NULL, type );
		element->attribute = NULL;
	}
	element->text = xml_strnew( text );
	element->gettag = xmlelement_gettag;
	element->getattribute = xmlelement_getattribute;
	element->gettext = xmlelement_gettext;
	return element;
}



//============================================================================
// 函数名称：getattrnum
// 函数功能：得到元素属性个数
//
// 输入参数： 1 -- 字符串
// 输出参数：
// 返回值：元素的属性个数
// 说明：得到元素属性个数
//============================================================================
static int getattrnum(char *string)
{
	int size = xml_strsplit(string, '=');
	if (size > 1)
	{
		size--;
	}
	return size;
}



//============================================================================
// 函数名称：xmlelement_getattrnum
// 函数功能：得到元素属性个数
//
// 输入参数： 1 -- 元素对象
// 输出参数：
// 返回值：元素的属性个数
// 说明：得到元素属性个数
//============================================================================
int xmlelement_getattrnum( struct xmlelement *element )
{
	struct xmlnode *attribute;
	int size = 0;
	if (NULL == element)
	{
		return 0;
	}
	attribute = element->attribute;
	for (; NULL != attribute; size++ )
	{
		attribute = attribute->next;
	}

	return size;
}


//============================================================================
// 函数名称：xmlelement_parserattr
// 函数功能：解析属性
//
// 输入参数： 1 -- 元素对象
//			2 -- 字符串
// 输出参数：
// 返回值：
// 说明：解析属性
//============================================================================
void  xmlelement_parserattr( struct xmlelement *element, char *string )
{
	int index;
	char temp[128];
	char name[128];
	char value[128];
	int size = getattrnum( string );
	if ( size > 0 )
	{
		for ( index = 0; index < size; index++ )
		{
			xml_strsplitlist( string, '=', index, temp );
			getattrname( temp, name );
			xml_strsplitlist( string, '=', index + 1, temp );
			getattrvalue( temp, value );
			xmlnode_add( &(element->attribute), setattribute( name, value ) );
		}
	}
	else
	{
		element->attribute = NULL;
	}
}


//============================================================================
// 函数名称：getattrname
// 函数功能：读取元素的属性名
//
// 输入参数： 1 -- 元素对象属性名字
//			2 -- 元素对象属性值
// 输出参数：
// 返回值：
// 说明：读取元素的属性名
//============================================================================
static int getattrname( char *string, char *buffer )
{
	int index, len;
	char temp[128] = { 0 };
	int size = xml_strsplit( string, ' ' );
	len = 0;
	if ( size <= 1 )
	{
		return 0;
	}
	for ( index = size - 1; index >= 0; index-- )
	{
		len = xml_strsplitlist( string, ' ', index, temp );
		if ( len > 0 )
		{
			xml_strcpy( buffer, temp );
			break;
		}
	}
	return len;
}


//============================================================================
// 函数名称：getattrvalue
// 函数功能：读取元素的属性值
//
// 输入参数： 1 -- 元素对象属性名字
//			2 -- 元素对象属性值
// 输出参数：
// 返回值：元素的属性节点对象
// 说明：读取元素的属性值
//============================================================================
static int getattrvalue( char *string, char *buffer )
{
	int len;
	char temp[128] = { 0 };
	int size = xml_strsplit( string, '\"' );
	len = 0;
	if ( size < 3 )
	{
		return 0;
	}
	len = xml_strsplitlist( string, '\"', 1, temp );
	if ( len > 0 )
	{
		xml_strcpy( buffer, temp );
	}
	return len;
}


//============================================================================
// 函数名称：setattribute
// 函数功能：设置元素节点的属性
//
// 输入参数： 1 -- 元素对象属性名字
//			2 -- 元素对象属性值
// 输出参数：
// 返回值：元素的属性节点对象
// 说明：设置元素节点的属性
//============================================================================
static struct xmlnode *setattribute( char *name, char *value  )
{
	char *newname = xml_strnew( name );
	char *newvalue = xml_strnew( value );
	return new_xmlnode( newname, newvalue, ATTRIBUTE_NODE );
}

//============================================================================
// 函数名称：xmlelement_gettag
// 函数功能：得到元素节点的标签
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 元素对象的文本节点的标签
// 返回值：标签的长度
// 说明：得到元素节点的标签
//============================================================================
int xmlelement_gettag( void *this, char *buffer )
{
	struct xmlelement *element = (struct xmlelement *)this;
	return xml_strcpy( buffer, element->base.name);
	// return 1;
}



//============================================================================
// 函数名称：xmlelement_checkattribute
// 函数功能：检测元素节点的属性与输入是否相符
//
// 输入参数： 1 -- 元素对象自身
//			  2 -- 元素对象属性节点的名称
//			  3 -- 输入的字符串
// 输出参数：
// 返回值：0 -- 失败 1 -- 成功
// 说明：检测元素节点的属性与输入是否相符
//============================================================================
int xmlelement_checkattribute(void *this, char *name, char *buffer)
{
	char attr[100] = { 0 };
	int size = xmlelement_getattribute(this, name, attr);
	if (size > 0)
	{
		if (0 == xml_strcmp(attr, buffer))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return -1;
}

//============================================================================
// 函数名称：xmlelement_getattribute
// 函数功能：得到元素节点的属性
//
// 输入参数： 1 -- 元素对象自身
//			2 -- 元素对象属性节点的名称
// 输出参数：
// 返回值：none
// 说明：得到元素节点的属性
//============================================================================
int xmlelement_getattribute( void *this, char *name, char *buffer )
{
	int size = 0;
	struct xmlelement *element = (struct xmlelement *)this;
	struct xmlnode *list = NULL;
	if ( NULL == element )
	{
		return 0;
	}
	list = element->attribute;
	for ( ;; )
	{
		if ( NULL == list )
		{
			return 0;
		}
		if ( 0 == xml_strcmp(list->name, name) )
		{
			size = xml_strcpy( buffer, list->value );
			break;
		}
		list = list->next;
	}
	return size;
}



//============================================================================
// 函数名称：xmlelement_getattrbyint
// 函数功能：得到元素节点的属性的整型值
//
// 输入参数： 1 -- 元素对象自身
//			  2 -- 元素对象属性节点的名称
// 输出参数： 3 -- 元素对象的属性整型值
// 返回值：0 -- 失败 1 -- 成功
// 说明：得到元素节点的属性的整型值
//============================================================================
int xmlelement_getattrbyint(void *this, char *name, int *value)
{
	char buffer[100] = { 0 };
	int size = xmlelement_getattribute(this, name, buffer);
	if (size > 0)
	{
		if (xml_isint(buffer))
		{
			*value = xml_strtoint(buffer);
			return 1;
		}
	}
	return 0;
}

//============================================================================
// 函数名称：xmlelement_getattrbyfloat
// 函数功能：得到元素节点的属性的浮动类型值
//
// 输入参数： 1 -- 元素对象自身
//			  2 -- 元素对象属性节点的名称
// 输出参数： 3 -- 元素对象的属性浮动类型值
// 返回值：0 -- 失败 1 -- 成功
// 说明：得到元素节点的属性的浮动类型值
//============================================================================
int xmlelement_getattrbyfloat(void *this, char *name, float *value)
{
	char buffer[100] = { 0 };
	int size = xmlelement_getattribute(this, name, buffer);
	if (size > 0)
	{
		if (xml_isfloat(buffer))
		{
			*value = xml_strtofloat(buffer);
			return 1;
		}
	}
	return 0;
}





//============================================================================
// 函数名称：xmlelement_setattribute
// 函数功能：设置元素节点的属性
//
// 输入参数： 1 -- 元素对象自身
//			  2 -- 元素对象属性节点的名称
//			  3 -- 元素对象属性值
// 输出参数：
// 返回值：none
// 说明：设置元素节点的属性
//============================================================================
int xmlelement_setattribute(void *this, char *name, char *buffer)
{
	int size = 0;
	struct xmlelement *element = (struct xmlelement *)this;
	struct xmlnode *list = NULL;
	if (NULL == element)
	{
		return 0;
	}
	list = element->attribute;
	for (;; )
	{
		if (NULL == list)
		{
			break;
		}
		if (0 == xml_strcmp(list->name, name))
		{
			if (NULL != list->value)
			{
				xml_free(list->value);
			}
			list->value = xml_strnew(buffer);
			if (NULL == list->value)
			{
				size = 0;
			}
			else
			{
				size = xml_strlen(list->value);
			}
			return size;
		}
		list = list->next;
	}
	xmlnode_add(&(element->attribute), setattribute(name, buffer));
	size = xml_strlen(buffer);
	return size;
}


//============================================================================
// 函数名称：xmlelement_setattrbyint
// 函数功能：设置元素节点的属性的整型值
//
// 输入参数： 1 -- 元素对象自身
//			  2 -- 元素对象属性节点的名称
//			  3 -- 元素对象的属性整型值
// 输出参数： 
// 返回值：0 -- 失败 1 -- 成功
// 说明：设置元素节点的属性的整型值
//============================================================================
int xmlelement_setattrbyint(void *this, char *name, int value)
{
	char buffer[100] = { 0 };
	int size;
	sprintf(buffer, "%d", value);
	size = xmlelement_setattribute(this, name, buffer);
	if (size > 0)
	{
		size = 1;
	}
	return size;
}


//============================================================================
// 函数名称：xmlelement_indexofattr
// 函数功能：得到元素节点的属性
//
// 输入参数： 1 -- 元素对象自身
//			2 -- 元素对象属性索引号
// 输出参数：属性值
// 返回值：属性值字符串长度
// 说明：得到元素节点的属性
//============================================================================
int xmlelement_indexofattr(void *this, int index, char *buffer)
{
	int size = 0;
	struct xmlelement *element = (struct xmlelement *)this;
	struct xmlnode *list = NULL;
	if (NULL == element)
	{
		return 0;
	}
	list = element->attribute;

	for (; index > 0; index-- )
	{
		if (NULL == list)
		{
			break;
		}
		list = list->next;
	}
	if (0 == index)
	{
		if (NULL == list)
		{
			return 0;
		}
		size = xml_strcpy(buffer, list->value);
	}

	return size;
}


//============================================================================
// 函数名称：xmlelement_gettext
// 函数功能：得到元素节点的文本
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 元素对象的文本节点的文本
// 返回值：文本的长度
// 说明：得到元素节点的文本
//============================================================================
int xmlelement_gettext( void *this, char *buffer )
{
	struct xmlelement *element = (struct xmlelement *)this;
	return xml_strcpy( buffer, element->text );
}

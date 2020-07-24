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
 *  V1.0.1  2017-06-13  xfwangqiang     追加了得到子元素的功能
 * 			以及打印子元素到字符串的功能，追加了得到子元素个数
 *  V1.0.2  2017-06-29  xfwangqiang     发现并解决了xmlelement_delate函数的BUG
 *  V1.0.3  2017-06-30  xfwangqiang     发现并解决了xmlelement_addchild函数的BUG,
 			BUG为未指定子元素节点的父节点。
 * 			追加了xmlelement_getchildnum的函数
 *  V1.0.3  2020-07-24  xfwangqiang     修复了函数xmlelement_create，getattrname中分隔只能识别空格的问题
 *          修正了xmlelement_delete函数名的拼写错误
 *          修复了xmlelement_delete函数中删除最后一个节点失败的Bug
 *          增加了xmlelement_getchildlist函数
 *          优化了xmlelement_makeattrstr函数中的警告
 *========================================================*/



#include <stdio.h>
#include <stdlib.h>
#include "xml_string.h"
#include "xml_element.h"
#include "xml_node.h"
#include "xml.h"


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
	int listlen = xml_strsplit( string, '\0' );
	if ( listlen > 1 )
	{
		xml_strsplitlist( string, '\0', 0, name );
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
// 函数名称：xmlelement_create
// 函数功能：删除一个元素节点
//
// 输入参数： 1 -- 元素对象
// 输出参数：
// 返回值：none
// 说明：删除一个元素节点
//============================================================================
int xmlelement_delete( void *this )
{
	struct xmlelement *element = (struct xmlelement *)this;
	struct xmlnode *father = NULL;
	if ( NULL == element )
	{
		return 1;
	}
	if ( NULL != element->text )
	{
		xml_free( element->text );
		element->text = NULL;
	}
	if ( NULL != element->attribute )
	{
		xmlnode_removelinklist( &(element->attribute) );
		element->attribute = NULL;
	}
	if ( NULL != &(element->base))
	{
		father = xmlnode_getfather( element );
        if (NULL == father)
        {
            del_xmlnode(element);
        }
        else
        {
		    xmlnode_remove( (struct xmlnode **)&(father->child), (struct xmlnode *)&(element->base) );
        }
		element = NULL;
	}
	return 1;
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
	int size = xml_strsplit( string, '\0' );
	len = 0;
	if ( size <= 1 )
	{
		return 0;
	}
	for ( index = size - 1; index >= 0; index-- )
	{
        len = xml_strsplitlist(string, '\0', index, temp);
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
	for ( ; NULL != list; )
	{
		if ( 0 == xml_strcmp(list->name, name) )
		{
			size = xml_strcpy( buffer, list->value );
			break;
		}
		list = xmlnode_getnext( list );
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
	for ( ; NULL != list; )
	{
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
		list = xmlnode_getnext(list);
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
	if ( NULL == element )
	{
		return 0;
	}
	return xml_strcpy( buffer, element->text );
}




//============================================================================
// 函数名称：xmlelement_getchild
// 函数功能：得到元素节点的子元素节点
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 元素对象的子元素节点的标签
// 返回值：元素对象的子元素节点
// 说明：得到元素节点的子元素节点
//============================================================================
struct xmlelement * xmlelement_getchild(void *this, char *tag)
{
	struct xmlelement *element = (struct xmlelement *)this;
	char name[100] = { 0 };
	int size;
	element = (struct xmlelement *)xmlnode_getchild(element);
	for (; NULL != element; )
	{
		size = xmlnode_getname(element, name);
		if (size > 0)
		{
			if (0 == xml_strcmp(name, tag))
			{
				break;
			}
		}
		element = (struct xmlelement *)xmlnode_getnext(element);
	}
	return element;
}




//============================================================================
// 函数名称：xmlelement_getchildlist
// 函数功能：得到元素节点的子元素节点列表
//
// 输入参数： 1 -- 元素对象自身
//           2 -- 元素对象的子元素节点的标签
//           3 -- 返回子元素列表
//           4 -- 子元素列表长度
// 输出参数： 3 -- 元素对象的子元素节点的标签
// 返回值：子元素列表长度
// 说明：得到元素节点的子元素节点列表
//============================================================================
int xmlelement_getchildlist(void *this, char *tag, struct xmlelement **list, int list_size)
{
    struct xmlelement *element = (struct xmlelement *)this;
    char name[100] = {0};
    int size, list_index;
    element = (struct xmlelement *)xmlnode_getchild(element);
    for (list_index = 0; NULL != element;)
    {
        size = xmlnode_getname(element, name);
        if (size > 0)
        {
            if (0 == xml_strcmp(name, tag))
            {
                list[list_index++] = element;
                if (list_index >= list_size)
                {
                    break;
                }
            }
        }
        element = (struct xmlelement *)xmlnode_getnext(element);
    }
    return list_index;
}


//============================================================================
// 函数名称：xmlelement_indexofchild
// 函数功能：索引元素节点的子元素节点
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 索引
// 输出参数： none
// 返回值：元素对象的子元素节点
// 说明：索引元素节点的子元素节点
//============================================================================
struct xmlelement * xmlelement_indexofchild( void *this, int index )
{
	struct xmlelement *element = (struct xmlelement *)this;

	element = (struct xmlelement *)xmlnode_getchild(element);
	for (; (NULL != element) && (0 != index); index-- )
	{
		element = (struct xmlelement *)xmlnode_getnext(element);
	}
	return element;
}



//============================================================================
// 函数名称：xmlelement_getchildnum
// 函数功能：得到元素节点的子元素节点数量
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： nonde
// 返回值：元素对象的子元素节点数量
// 说明：得到元素节点的子元素节点数量
//============================================================================
int xmlelement_getchildnum(void *this)
{
	int number = 0;
	struct xmlelement *element = (struct xmlelement *)this;

	element = (struct xmlelement *)xmlnode_getchild(element);
	for (; NULL != element; number++)
	{		
		element = (struct xmlelement *)xmlnode_getnext(element);
	}
	return number;
}


//============================================================================
// 函数名称：xmlelement_makeheadstr
// 函数功能：生成元素节点的头字符串
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 生成的字符串
// 返回值：0 -- 失败 1 -- 成功
// 说明：生成元素节点的头字符串
//============================================================================
int xmlelement_makeheadstr(void *this, char *string)
{
	struct xmlelement *element = (struct xmlelement *)this;
	char buffer[1024] = { 0 };
	int size = xmlnode_getname(element, buffer);
	int ret = 0;
	if (size <= 0)
	{
		return 0;
	}
	sprintf(string, "<%s", buffer);
	if (xmlelement_makeattrstr(element, buffer))
	{
		xml_strcat(string, buffer);
	}
	size = xmlelement_gettext(element, buffer);
	if (size > 0)
	{
		xml_strcat(string, ">");
		xml_strcat(string, buffer);
		if (NULL == xmlnode_getchild(element))
		{
			size = xmlelement_makeendstr(element, buffer);
			if (size > 0)
			{
				xml_strcat(string, buffer);
			}
			ret = 2;
		}
		else
		{
			ret = 1;
		}
	}
	else
	{
		if (NULL == xmlnode_getchild(element))
		{
			xml_strcat(string, "/>");
			ret = 4;
		}
		else
		{
			xml_strcat(string, ">");
			ret = 3;
		}
	}
	return ret;
}


//============================================================================
// 函数名称：xmlelement_makeendstr
// 函数功能：生成元素节点的结束字符串
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 生成的字符串
// 返回值：0 -- 失败 1 -- 成功
// 说明：生成元素节点的结束字符串
//============================================================================
int xmlelement_makeendstr(void *this, char *string)
{
	char buffer[1024] = { 0 };
	struct xmlelement *element = (struct xmlelement *)this;
	int size = xmlelement_gettag(element, buffer);
	if (size > 0)
	{
		sprintf(string, "</%s>", buffer);
	}
	return size;
}

//============================================================================
// 函数名称：xmlelement_makeattrstr
// 函数功能：生成元素节点的属性字符串
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 生成的字符串
// 返回值：0 -- 失败 1 -- 成功
// 说明：生成元素节点的属性字符串
//============================================================================
int xmlelement_makeattrstr(void *this, char *string)
{
	char name[100] = { 0 };
	char value[100] = { 0 };
	char temp[256] = { 0 };
	int size, ret;
	struct xmlnode *attr = NULL;
	struct xmlelement *element = (struct xmlelement *)this;
	if (NULL == element)
	{
		return 0;
	}
	attr = element->attribute;
	ret = 0;
	for (; NULL != attr; )
	{
		size = xmlnode_getname(attr, name);
		if (size <= 0)
		{
			break;
		}
		size = xmlnode_getvalue(attr, value);
		if (size <= 0)
		{
			break;
		}
		sprintf(temp, " %s=\"%s\"", name, value);
		if ( 0 == ret )
		{
			xml_strcpy( string, temp );
		}
		else
		{
			xml_strcat(string, temp);
		}
		ret++;
		attr = xmlnode_getnext(attr);
	}
	return ret;
}



//============================================================================
// 函数名称：xmlelement_addchild
// 函数功能：增加子元素
//
// 输入参数： 1 -- 元素对象自身
//			  2 -- 子元素对象
// 输出参数： none
// 返回值：0 -- 失败 1 -- 成功
// 说明：增加子元素
//============================================================================
int xmlelement_addchild( void *this, struct xmlelement *child )
{
	struct xmlelement *element = (struct xmlelement *)this;
	if ( (NULL == element) || (NULL == child) )
	{
		return 0;
	}
	xmlnode_add( &(element->base.child), &(child->base));
	return xmlnode_setfather( &(child->base), &(element->base));
}


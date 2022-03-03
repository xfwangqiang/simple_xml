/*
 * xml.c
 *
 *  Created on: 2017年5月22日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2017-06-05  xfwangqiang     创建
 *========================================================*/


#include <stdio.h>
#include <stdlib.h>
#include "../inc/xml_node.h"
#include "../inc/xml_string.h"
#include "../inc/xml.h"
#include "../inc/xml_memory.h"

//============================================================================
// 函数名称：new_xml_node
// 函数功能：创建一个新的XML节点对象--构造函数
//
// 输入参数： 1 -- 节点名称
//			2 -- 节点值
//			3 -- 节点类型
// 输出参数：
// 返回值：节点对象指针
// 说明：创建一个新的XML节点对象--构造函数
//============================================================================
struct xmlnode *new_xmlnode(char *name, char *value, enum xmlnode_type type )
{
	struct xmlnode *node = (struct xmlnode *)xml_malloc( sizeof(struct xmlnode) );
	node->name = name;
	node->value = value;
	node->type = type;
	node->father = NULL;
	node->prev = NULL;
	node->next = NULL;
	node->child = NULL;
	node->getname = xmlnode_getname;
	node->getvalue = xmlnode_getvalue;
	return node;
}


//============================================================================
// 函数名称：del_xml_node
// 函数功能：删除一个XML节点对象--析构函数
//
// 输入参数： 1 -- 节点名称
//			2 -- 节点值
//			3 -- 节点类型
// 输出参数：
// 返回值：节点对象指针
// 说明：删除一个XML节点对象--析构函数
//============================================================================
void del_xmlnode( void *this )
{
	struct xmlnode *node = (struct xmlnode *)this;
	xml_free( node->name );
	xml_free( node->value );
	node->name = NULL;
	node->value = NULL;
	node->type = 0;
	node->father = NULL;
	node->prev = NULL;
	node->next = NULL;
	node->child = NULL;
	node->getname = NULL;
	node->getvalue = NULL;
	xml_free( node );
}


//============================================================================
// 函数名称：xmlnode_init
// 函数功能：初始化一个新的XML节点对象--构造函数
//
// 输入参数： 1 -- 节点对象
// 			2 -- 节点名称
//			3 -- 节点值
//			4 -- 节点类型
// 输出参数：
// 返回值：
// 说明：初始化一个新的XML节点对象--构造函数
//============================================================================
void xmlnode_init( void *this , char *name, char *value, enum xmlnode_type type )
{
	struct xmlnode *node = (struct xmlnode *)this;
	node->name = name;
	node->value = value;
	node->type = type;
	node->father = NULL;
	node->prev = NULL;
	node->next = NULL;
	node->child = NULL;
	node->getname = xmlnode_getname;
	node->getvalue = xmlnode_getvalue;
}


void xmlnode_create( char *name, char *value, enum xmlnode_type type )
{	
	struct xmlnode *node = (struct xmlnode *)xml_malloc( sizeof(struct xmlnode) );
	node->name = xml_strnew( name );
	node->value = xml_strnew( value );
	node->type = type;
	node->father = NULL;
	node->prev = NULL;
	node->next = NULL;
	node->child = NULL;
	node->getname = xmlnode_getname;
	node->getvalue = xmlnode_getvalue;
}


//============================================================================
// 函数名称：xml_get_node_name
// 函数功能：得到一个节点对象的名字
//
// 输入参数： 1 -- 节点对象自身
// 输出参数：	2 -- 输出名字
// 返回值：名字的长度
// 说明：得到一个节点对象的名字
//============================================================================
int xmlnode_getname(  void *this, char *buffer  )
{
	struct xmlnode *node = (struct xmlnode *)this;
	return xml_strcpy( buffer, node->name );
}



//============================================================================
// 函数名称：xml_get_node_name
// 函数功能：得到一个节点对象的值
//
// 输入参数： 1 -- 节点对象自身
// 输出参数：	2 -- 输出值
// 返回值：名字的值的字符串长度
// 说明：得到一个节点对象的值
//============================================================================
int xmlnode_getvalue(  void *this, char *buffer  )
{
	struct xmlnode *node = (struct xmlnode *)this;
	return xml_strcpy( buffer, node->value);
}



//============================================================================
// 函数名称：xmlnode_getfather
// 函数功能：得到一个节点对象父节点
//
// 输入参数： 1 -- 节点对象自身
// 输出参数：
// 返回值：节点对象第父节点
// 说明：得到一个节点对象父节点
//============================================================================
struct xmlnode * xmlnode_getfather(void *this)
{
	struct xmlnode *node = (struct xmlnode *)this;
	if (NULL == node)
	{
		return NULL;
	}
	return node->father;
}


//============================================================================
// 函数名称：xmlnode_setfather
// 函数功能：设置一个节点对象父节点
//
// 输入参数： 1 -- 节点对象自身
//			  2 -- 父节点对象
// 输出参数：
// 返回值：节点对象第父节点
// 说明：设置一个节点对象父节点
//============================================================================
int xmlnode_setfather(void *this, struct xmlnode *father)
{
	struct xmlnode *node = (struct xmlnode *)this;
	if (NULL == node)
	{
		return 0;
	}

	node->father = father;
	return 1;
}


//============================================================================
// 函数名称：xmlnode_getchild
// 函数功能：得到一个节点对象第一个子节点
//
// 输入参数： 1 -- 节点对象自身
// 输出参数：
// 返回值：节点对象第一个子节点
// 说明：得到一个节点对象第一个子节点
//============================================================================
struct xmlnode * xmlnode_getchild(void *this)
{
	struct xmlnode *node = (struct xmlnode *)this;
	if (NULL == node)
	{
		return NULL;
	}
	return node->child;
}


//============================================================================
// 函数名称：xmlnode_getprev
// 函数功能：得到一个节点对象上一个兄弟节点
//
// 输入参数： 1 -- 节点对象自身
// 输出参数：
// 返回值：节点对象上一个兄弟节点
// 说明：得到一个节点对象上一个兄弟节点
//============================================================================
struct xmlnode * xmlnode_getprev(void *this)
{
	struct xmlnode *node = (struct xmlnode *)this;
	if (NULL == node)
	{
		return NULL;
	}
	return node->prev;
}


//============================================================================
// 函数名称：xmlnode_getnext
// 函数功能：得到一个节点对象下一个兄弟节点
//
// 输入参数： 1 -- 节点对象自身
// 输出参数：
// 返回值：节点对象下一个兄弟节点
// 说明：得到一个节点对象下一个兄弟节点
//============================================================================
struct xmlnode * xmlnode_getnext(void *this)
{
	struct xmlnode *node = (struct xmlnode *)this;
	if (NULL == node)
	{
		return NULL;
	}
	return node->next;
}




//============================================================================
// 函数名称：xmlnode_add
// 函数功能：从一个链表中增加一个节点对象
//
// 输入参数： 1 -- 链表对象的地址
// 			  2 -- 节点对象自身
// 输出参数：
// 返回值：none
// 说明：链表中元素个数
//============================================================================
int xmlnode_add( struct xmlnode **list, struct xmlnode *node )
{
	struct xmlnode *temp = NULL;
	int size = 1;
	if ( NULL == *list )
	{
		*list = node;
		node->next = NULL;
		node->prev = NULL;
	}
	else
	{
		temp = *list;
		for ( ;; )
		{
			if ( NULL == temp->next )
			{
				temp->next = node;
				node->next = NULL;
				node->prev = temp;
				size++;
				break;
			}
			temp = temp->next;
			size++;
		}
	}
	return size;
}


//============================================================================
// 函数名称：xmlnode_remove
// 函数功能：从一个链表中删除一个节点对象
//
// 输入参数： 1 -- 链表对象的地址
// 			2 -- 节点对象自身
// 输出参数：
// 返回值：none
// 说明：从一个链表中删除一个节点对象
//============================================================================
int xmlnode_remove( struct xmlnode **list, struct xmlnode *node )
{
	struct xmlnode *prev, *next;

	prev = node->prev;
	next = node->next;

	if ( NULL == prev )		// head
	{
		del_xmlnode( node );
		if ( NULL == next )
		{
			// the link list has only one
			*list = NULL;
		}
		else
		{
			next->prev = NULL;
			*list = next;
		}
	}
	else
	{
		del_xmlnode( node );
		if ( NULL == next )
		{
			// the last one of link list
			prev->next = NULL;
		}
		else
		{
			prev->next = next;
			next->prev = prev;
		}
	}

	return 1;
}

//============================================================================
// 函数名称：xml_remove_linklist
// 函数功能：删除一个链表对象
//
// 输入参数： 1 -- 链表对象的地址
// 输出参数：
// 返回值：none
// 说明：删除一个链表对象
//============================================================================
void xmlnode_removelinklist( struct xmlnode **list )
{
	struct xmlnode *pnode = (*list)->next;
	xmlnode_remove( list, *list );
	if ( NULL == pnode )
	{
		return ;
	}
	xmlnode_removelinklist(  list  );
}




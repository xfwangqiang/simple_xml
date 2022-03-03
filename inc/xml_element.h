/*
 * xml_element.h
 *
 *  Created on: 2017年5月23日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2017-06-05  xfwangqiang     创建
 *  V1.0.1  2020-07-24  xfwangqiang     增加了xmlelement_delete函数的声明
 *========================================================*/


#ifndef XML_ELEMENT_H_
#define XML_ELEMENT_H_

#include "xml_node.h"
#include "xml_typedefs.h"
#include "xml_config.h"

// 定义元素节点的数据结构
struct xmlelement {
	struct xmlnode base;			// 继承自xml_node的类
	struct xmlnode *attribute;		// XML元素的属性链表
	char *text;					// XML元素的文本
	xml_get_func gettag;			// 得到元素节点的标签
	xml_get_func1 getattribute;		// 得到元素节点的属性
	xml_get_func gettext;			// 得到元素节点的文本
};


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
struct xmlelement *xmlelement_create( char *string, char *text, enum xmlnode_type type );



//============================================================================
// 函数名称：xmlelement_create
// 函数功能：删除一个元素节点
//
// 输入参数： 1 -- 元素对象
// 输出参数：
// 返回值：none
// 说明：删除一个元素节点
//============================================================================
int xmlelement_delete( void *this );


//============================================================================
// 函数名称：xmlelement_getattrnum
// 函数功能：得到元素属性个数
//
// 输入参数： 1 -- 元素对象
// 输出参数：
// 返回值：元素的属性个数
// 说明：得到元素属性个数
//============================================================================
int xmlelement_getattrnum(struct xmlelement *element);



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
void  xmlelement_parserattr( struct xmlelement *element, char *string );

//============================================================================
// 函数名称：xmlelement_gettag
// 函数功能：得到元素节点的标签
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 元素对象的文本节点的标签
// 返回值：标签的长度
// 说明：得到元素节点的标签
//============================================================================
int xmlelement_gettag( void *this, char *buffer );


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
int xmlelement_checkattribute(void *this, char *name, char *buffer);

//============================================================================
// 函数名称：xml_element_get_attribute
// 函数功能：得到元素节点的属性
//
// 输入参数： 1 -- 元素对象自身
//			2 -- 元素对象属性节点的名称
// 输出参数：
// 返回值：none
// 说明：FPGA的读取访问操作
//============================================================================
int xmlelement_getattribute( void *this, char *name, char *buffer );



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
int xmlelement_getattrbyint(void *this, char *name, int *value);



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
int xmlelement_getattrbyfloat(void *this, char *name, float *value);

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
int xmlelement_setattribute(void *this, char *name, char *buffer);



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
int xmlelement_setattrbyint(void *this, char *name, int value);

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
int xmlelement_indexofattr(void *this, int index, char *buffer);

//============================================================================
// 函数名称：xmlelement_gettext
// 函数功能：得到元素节点的文本
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 元素对象的文本节点的文本
// 返回值：文本的长度
// 说明：得到元素节点的文本
//============================================================================
int xmlelement_gettext( void *this, char *buffer );



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
int xmlelement_getchildlist(void *this, char *tag, struct xmlelement **list, int list_size);


//============================================================================
// 函数名称：xmlelement_getchild
// 函数功能：得到元素节点的子元素节点
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 元素对象的子元素节点的标签
// 返回值：元素对象的子元素节点
// 说明：得到元素节点的子元素节点
//============================================================================
struct xmlelement * xmlelement_getchild(void *this, char *tag);


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
struct xmlelement * xmlelement_indexofchild( void *this, int index );


//============================================================================
// 函数名称：xmlelement_getchildnum
// 函数功能：得到元素节点的子元素节点数量
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： nonde
// 返回值：元素对象的子元素节点数量
// 说明：得到元素节点的子元素节点数量
//============================================================================
int xmlelement_getchildnum(void *this);

//============================================================================
// 函数名称：xmlelement_makeheadstr
// 函数功能：生成元素节点的头字符串
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 生成的字符串
// 返回值：0 -- 失败 1 -- 成功
// 说明：生成元素节点的头字符串
//============================================================================
int xmlelement_makeheadstr(void *this, char *string);


//============================================================================
// 函数名称：xmlelement_makeendstr
// 函数功能：生成元素节点的结束字符串
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 生成的字符串
// 返回值：0 -- 失败 1 -- 成功
// 说明：生成元素节点的结束字符串
//============================================================================
int xmlelement_makeendstr(void *this, char *string);

//============================================================================
// 函数名称：xmlelement_makeattrstr
// 函数功能：生成元素节点的属性字符串
//
// 输入参数： 1 -- 元素对象自身
// 输出参数： 2 -- 生成的字符串
// 返回值：0 -- 失败 1 -- 成功
// 说明：生成元素节点的属性字符串
//============================================================================
int xmlelement_makeattrstr(void *this, char *string);


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
int xmlelement_addchild( void *this, struct xmlelement *child );

#endif /* XML_ELEMENT_H_ */
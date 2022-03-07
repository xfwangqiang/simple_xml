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
 *  V1.0.2  2022-03-06  xfwangqiang     增加了xmlelement_deletechilds函数的声明
 *          优化了一些函数的注释
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

/**
 * @brief   创建一个元素节点
 *
 * @param   string  xml元素的tag名称和属性
 * @param   text    xml元素的text节点
 * @param   type    xml node的类型
 *
 * @return 成功：返回一个元素节点对象，否则返回NULL
 */
struct xmlelement *xmlelement_create( char *string, char *text, enum xmlnode_type type );

/**
 * @brief  删除一个元素节点，释放元素的text节点，attribute列表, childs列表，最后释放
 * 自身的内存。当包含子元素节点时，存在递归调用的可能。
 *
 * @param   this 元素对象
 * @return  成功：1， 失败：其它
 */
int xmlelement_delete( void *this );

/**
 * @brief   得到元素属性个数
 *
 * @param   element 元素对象
 *
 * @return  元素属性数量
 */
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

/**
 * @brief   得到元素节点的标签
 *
 * @param   this 元素对象自身
 * @param   buffer 元素对象的文本节点的标签
 *
 * @return  标签的字符长度
 */
int xmlelement_gettag( void *this, char *buffer );


/**
 * @brief   检测元素节点的属性与输入是否相符
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   buffer  输入的字符串
 *
 * @return 成功：1， 失败：其它
 */
int xmlelement_checkattribute(void *this, char *name, char *buffer);

/**
 * @brief   得到元素节点的属性字符串
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   buffer  字符串属性值的buffer
 *
 * @return 返回字符串属性值的字符长度
 */
int xmlelement_getattribute( void *this, char *name, char *buffer );

/**
 * @brief   得到元素节点的属性整形值
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   value   整形值的指针
 *
 * @return 成功：1， 失败：其它
 */
int xmlelement_getattrbyint(void *this, char *name, int *value);

/**
 * @brief   得到元素节点的属性浮点值
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   value   浮点值的指针
 *
 * @return 成功：1， 失败：其它
 */
int xmlelement_getattrbyfloat(void *this, char *name, float *value);

/**
 * @brief   设置元素节点的属性
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   buffer   元素对象属性值
 *
 * @return 成功：> 0， 失败：0
 */
int xmlelement_setattribute(void *this, char *name, char *buffer);



/**
 * @brief   设置元素节点的属性的整型值
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   value   元素对象属性的整型值
 *
 * @return 成功：> 0， 失败：0
 */
int xmlelement_setattrbyint(void *this, char *name, int value);


/**
 * @brief   设置元素节点的属性的整型值-HEX
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   value   元素对象属性的整型值
 *
 * @return 成功：> 0， 失败：0
 */
int xmlelement_setattrbyhex(void *this, char *name, int value);


/**
 * @brief   设置元素节点的属性的浮点值
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性节点的名称
 * @param   value   元素对象属性的浮点值
 *
 * @return 成功：> 0， 失败：0
 */
int xmlelement_setattrbyfloat(void *this, char *name, float value);

/**
 * @brief   通过索引值得到元素节点的属性
 *
 * @param   this    元素对象自身
 * @param   name    元素对象属性索引号
 * @param   buffer  字符串属性值的buffer
 *
 * @return 成功：> 0， 失败：0
 */
int xmlelement_indexofattr(void *this, int index, char *buffer);

/**
 * @brief   得到元素节点的文本
 *
 * @param   this    元素对象自身
 * @param   buffer  字符串属性值的buffer
 *
 * @return 文本的长度
 */
int xmlelement_gettext(void *this, char *buffer);


/**
 * @brief   设置元素节点的文本
 *
 * @param   this    元素对象自身
 * @param   buffer  字符串属性值的buffer
 *
 * @return 成功 ： 1， 失败：其它
 */
int xmlelement_settext( void *this, char *buffer );

/**
 * @brief   得到元素节点的子元素节点列表
 *
 * @param   this        元素对象自身
 * @param   tag         元素对象的子元素节点的标签
 * @param   list        返回子元素列表
 * @param   list_size   子元素列表长度
 *
 * @return 子元素列表长度
 */
int xmlelement_getchildlist(void *this, char *tag, struct xmlelement **list, int list_size);

/**
 * @brief   得到元素节点的子元素节点, 当tag为NULL时，直接返回第一个子节点元素
 *
 * @param   this        元素对象自身
 * @param   tag         元素对象的子元素节点的标签
 *
 * @return 子元素节点列表中的第一个子节点
 */
struct xmlelement * xmlelement_getchild(void *this, char *tag);

/**
 * @brief   索引元素节点的子元素节点
 *
 * @param   this        元素对象自身
 * @param   index       索引
 *
 * @return 元素对象的子元素节点
 */
struct xmlelement * xmlelement_indexofchild( void *this, int index );

/**
 * @brief   得到元素节点的子元素节点数量
 *
 * @param   this        元素对象自身
 *
 * @return 元素对象的子元素节点数量
 */
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


/**
 * @brief   增加子元素
 *
 * @param   this        元素对象自身
 * @param   child       子元素对象
 *
 * @return 成功：1， 失败：0
 */
int xmlelement_addchild( void *this, struct xmlelement *child );

/**
 * @brief   删除子元素列表, 当本函数被xmlelement_delete调用时，存在递归调用的情况。
 *
 * @param   this        元素对象自身
 *
 * @return 成功：1， 失败：0
 */
int xmlelement_deletechilds(void *this);

#endif /* XML_ELEMENT_H_ */

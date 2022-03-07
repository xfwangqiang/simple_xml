/*
 * xml.h
 *
 *  Created on: 2017年5月22日
 *      Author: xfwangqiang
 */

/*========================================================*
 * 程序修改记录：
 * <版本号> <修改日期>, <修改人员>: <修改功能概述>
 *  V1.0.0  2017-06-05  xfwangqiang     创建
 *  V1.0.1  2020-07-24  xfwangqiang     增加了enum xml_status中的两个状态，用于解析XML注释
 *========================================================*/


#ifndef XML_H_
#define XML_H_

#include "xml_config.h"
#include "xml_node.h"
#include <stdio.h>
#include "xml_typedefs.h"
#include "xml_config.h"

enum xml_block_type
{
	EN_BLOCK0,		// being processed
	EN_BLOCK1,		// <string >
	EN_BLOCK2,		// <string/>
	EN_BLOCK3,		// </string>
	EN_BLOCK4,		// <?string?>
	EN_BLOCK5,		// <!-- string -->
	EN_BLOCK6		// ERROR
};

enum xml_status
{
    EN_EMPTY, // empty , no <
    EN_SCH,   // searching, have <
    EN_OK,    // >
    EN_CHCK_COMMENT,    // check <!--, hava <
    EN_SCH_COMMENT, // searching, have <!--
};

#define CN_BLOCK_MAX		(1024)
#define CN_BLOCK_TEXT		(128)
struct xml_block
{
	enum xml_status status;
	enum xml_block_type type;
	char buffer[CN_BLOCK_MAX];
	char text[CN_BLOCK_TEXT];
};

/**
 * @brief   加载一个xml文件
 *
 * @param   path xml文件的路径
 *
 * @return 返回xml根节点元素对象
 * */
struct xmlelement * xml_load( char * path );

/**
 * @brief   将一个xml元素（根节点）保存到指定的xml文件中
 *
 * @param   tree xml根节点元素对象
 * @param   path xml文件的路径
 *
 * @return 1 : 成功, 其它失败
 * */
int xml_save(struct xmlelement *tree, char *path);

int xml_saveelement(FILE *file, struct xmlelement *element);


int xml_savechildelement(FILE *file, struct xmlelement *element);


int xml_savenextelement(FILE *file, struct xmlelement *element);


struct xmlelement *xml_createelement( struct xml_block *block, enum xmlnode_type type );

int xml_push( struct xmlelement *element );
struct xmlelement *xml_pop( void );

// check the type of block
int xml_parserblocktype( struct xml_block *block );
// parser the xml block
int xml_parserblockname( struct xml_block *block );
// parser the xml block
void xml_parserblock( struct xml_block *block );

// put char from line to block
// return : -1 error
//	         0  over
//	 	 n  index
int xml_getblocktext( char *line, int offset, struct xml_block *block );
// init block
void xml_initblock( struct xml_block *block );
// put the char from line to bock's buffer, return the size of char
int xml_getblock( char *line,  int offset, struct xml_block *block );

void xml_print( struct xmlelement *tree );

#endif /* XML_H_ */

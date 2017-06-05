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
 *========================================================*/


#ifndef XML_H_
#define XML_H_

#include "xml_node.h"

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
	EN_EMPTY,	// empty , no <
	EN_SCH,		// searching, have <
	EN_OK		// >
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


struct xmlelement * xml_load( char * path );


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


//============================================================================
// 函数名称：xml_free
// 函数功能：释放一个内存块
//
// 输入参数： 1 -- 内存地址
// 输出参数：
// 返回值：
// 说明：释放一个内存块
//============================================================================
void xml_free( void *ptr );


void xml_print( struct xmlelement *tree );

#endif /* XML_H_ */

/*
 * xml.h
 *
 *  Created on: 2017��5��22��
 *      Author: xfwangqiang
 */

/*========================================================*
 * �����޸ļ�¼��
 * <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
 *  V1.0.0  2017-06-05  xfwangqiang     ����
 *  V1.0.1  2020-07-24  xfwangqiang     ������enum xml_status�е�����״̬�����ڽ���XMLע��
 *========================================================*/


#ifndef XML_H_
#define XML_H_

#include "xml_node.h"
#include <stdio.h>

#define OS_WIN		0
#define OS_LINUX	1
#define OS_VXWORKS	2
//#define OS_VER		(OS_WIN)
#define OS_VER		(OS_LINUX)
//#define OS_VER		(OS_VXWORKS)

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


struct xmlelement * xml_load( char * path );


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


//============================================================================
// �������ƣ�xml_free
// �������ܣ��ͷ�һ���ڴ��
//
// ��������� 1 -- �ڴ��ַ
// ���������
// ����ֵ��
// ˵�����ͷ�һ���ڴ��
//============================================================================
void xml_free( void *ptr );


void xml_print( struct xmlelement *tree );

#endif /* XML_H_ */

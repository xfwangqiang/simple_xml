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
 *  V1.0.1  2017-06-29  xfwangqiang     优化了XML文件的存储，减少了栈内存的使用
 *  V1.0.2  2020-07-24  xfwangqiang     优化了一些翻译警告
 *          修复了xml_createelement函数中因为元素长度太长造成的Bug
 *          修复了xml_getblock函数中的对注释支持不好的Bug
 *========================================================*/


#include "../inc/xml.h"
#include "../inc/xml_string.h"
#include "../inc/xml_file.h"
#include "../inc/xml_node.h"
#include "../inc/xml_element.h"
#include "../inc/xml_memory.h"
#include "../inc/xml_log.h"

static char xmlversion[256] = "sx 1.01.00";

// declear local function
static int xml_savetable( FILE *file, int deep );

/**
 * @brief   加载一个xml文件
 * 
 * @param   path xml文件的路径
 * 
 * @return 返回xml根节点元素对象
 * */
struct xmlelement * xml_load( char * path )
{
	struct xml_block block = { 0 };
	struct xmlelement *element = NULL;
	struct xmlelement *father = NULL;
    void *file = xml_fopen(path, "r+");
    char buf[1024] = { 0 };
	int temp = 0;
	if ( NULL == file )
	{
		xml_log( "the %s file can't be open!", path );
		return NULL;
	}
	xml_initblock( &block );
    if (NULL == xml_fgets(buf, 1024, file))
    {
		xml_log( "\r\nxml file error!" );
		xml_fclose(file);
		return NULL;
	}
	temp = 0;
	while ( 1 )
	{
		// TODO rser a line
		temp = xml_getblock( buf, temp, &block );
		if ( -1 == temp )
		{
			xml_log( "\r\nxml file error!" );
            xml_fclose(file);
            break;
		}
		else if ( 0 == temp )
		{
			if ( NULL == xml_fgets( buf, 1024, file ) )
			{
				//xml_log( "\r\nxml file over!" );
                xml_fclose(file);
                break;
			}
		}
		else
		{
			temp++;
			if ( EN_OK == block.status )
			{
				xml_parserblock( &block );
				if ( EN_BLOCK1 == block.type )
				{
					while( 1 )
					{
						temp = xml_getblocktext( buf, temp, &block );
						if ( 0 == temp )
						{
							if ( NULL == xml_fgets(buf, 1024, file) )
							{
								xml_log( "\r\nxml file over, but error!" );
                                xml_fclose(file);
                                return NULL;
							}
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					xml_strcpy( block.text, "none");
				}


				if ( EN_BLOCK1 == block.type )
				{
					if ( NULL == father )
					{
						element = xml_createelement( &block, DOCUMENT_NODE );
					}
					else
					{						
						element = xml_createelement( &block, ELEMENT_NODE );
						xmlnode_add( &(father->base.child), &(element->base) );
						xmlnode_setfather(&(element->base), &(father->base));
						xml_push( father );
					}
					father = element;
				}
				else if ( EN_BLOCK2 == block.type )
				{
					element = xml_createelement( &block, ELEMENT_NODE );
					xmlnode_add( &(father->base.child),  &(element->base)  );
					xmlnode_setfather(&(element->base), &(father->base));
				}
				else if ( EN_BLOCK3 == block.type )
				{
					element = father;
					father = xml_pop();
				}
			}

//			 xml_log( "\r\ntype = %d \tstring = %s, text = %s, buffer = %s, p = %d", block.type, block.buffer, block.text, buf, temp );
			xml_initblock( &block );
		}
	}

	return element;
}

/**
 * @brief   将一个xml元素（根节点）保存到指定的xml文件中
 *
 * @param   tree xml根节点元素对象
 * @param   path xml文件的路径
 *
 * @return 1 : 成功, 其它失败
 * */
int xml_save(struct xmlelement *tree, char *path)
{
    void *file = xml_fopen(path, "w+");
	
	if (NULL == file)
	{
		xml_log("the %s file can't be open!", path);
		return 0;
	}

	xml_fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n", file);
	
	xml_saveelement(file, tree);

    xml_fclose(file);

    return 1;
}


int xml_saveelement(FILE *file, struct xmlelement *element)
{
	char buffer[1024] = { 0 };
	int size;
	static int deep = 0;

	size = xmlelement_makeheadstr(element, buffer);
	if (size <= 0)
	{
		return 0;
	}
    xml_fputs(buffer, file);
	switch (size)
	{
	case 1:
	case 3:
		deep++;
		// xml_savetable(file, deep);
		xml_savechildelement(file, element);
		deep--;
        xml_fputs("\n", file);
		xml_savetable( file, deep );
		xmlelement_makeendstr( element, buffer );
		xml_fputs( buffer, file );

	case 4:
	case 2:
		if ( NULL != xmlnode_getnext(element ) )
		{
            xml_fputs("\n", file);
			xml_savetable( file, deep );
		}
		break;
	}
	return 1;
}

static int xml_savetable( FILE *file, int deep )
{
	int index;

	for (index = 0; index < deep; index++ )
	{
        xml_fputs("\t", file);
	}
	return 1;
}


int xml_savechildelement(FILE *file, struct xmlelement *element)
{
	if (NULL == element)
	{
		return 0;
	}
	element = (struct xmlelement *)xmlnode_getchild(element);
	for (; NULL != element; )
	{
		xml_saveelement(file, element);
		element = (struct xmlelement *)xmlnode_getnext(element);
	}
	return 1;
}


struct xmlelement *xml_createelement( struct xml_block *block, enum xmlnode_type type )
{
	int size;
	struct xmlelement *element = NULL;
	// char temp[128] = { 0 };
    char *temp = NULL;
	if ( (EN_BLOCK1 != block->type) && (EN_BLOCK2 != block->type) )
	{
		return NULL;
	}
	size = xml_strlen( block->buffer );
    temp = xml_malloc(size);
    if (NULL == temp)
    {
        return NULL;
    }
	if ( EN_BLOCK1 == block->type )
	{
		xml_strncpy( temp, block->buffer + 1, size - 2 );
		element = xmlelement_create( temp, block->text, type );
	}
	else if ( EN_BLOCK2 == block->type )
	{		
		xml_strncpy( temp, block->buffer + 1, size - 3 );
		element = xmlelement_create( temp, NULL, type );
	}
    xml_free(temp);
    temp = NULL;
	return element;
}


#define CN_XML_STACK_MAX	(100)
struct xmlelement *xml_stack[CN_XML_STACK_MAX] = { 0 };
int xml_stack_ptr = 0;
int xml_push( struct xmlelement *element )
{
	if ( xml_stack_ptr >= CN_XML_STACK_MAX )
	{
		return 0;
	}
	xml_stack[xml_stack_ptr] = element;
	xml_stack_ptr++;
	return xml_stack_ptr;
}


struct xmlelement *xml_pop( void )
{
	struct xmlelement *element;
	if ( (xml_stack_ptr > CN_XML_STACK_MAX) || (xml_stack_ptr < 1) )
	{
		return NULL;
	}
	xml_stack_ptr--;
	element = xml_stack[xml_stack_ptr];
	xml_stack[xml_stack_ptr] = NULL;
	return element;
}




// init block
void xml_initblock( struct xml_block *block )
{
	block->type = EN_BLOCK0;
	block->status = EN_EMPTY;
	xml_memset( block->buffer, 0, CN_BLOCK_MAX);
	xml_memset( block->text, 0, CN_BLOCK_TEXT);
}


// put the char from line to bock's buffer
// return : -1 error
//	         0  over
//	 	 n  index
int xml_getblock( char *line, int offset, struct xml_block *block )
{
	int index = 0;
    
    for ( ;; )
    {
        switch (block->status)
        {
        case EN_EMPTY:
            index = xml_strindexof(line, offset, '<');
            if (-1 == index)
            {
                return 0;
            }
            offset = index;
            block->status = EN_CHCK_COMMENT;
            break;
        case EN_SCH:
            if (0 == xml_strlen(block->buffer))
            {
                if (('!' == line[offset]) && ('-' == line[offset + 1]) && ('-' == line[offset + 2]))
                {
                    offset += 3;
                    block->status = EN_SCH_COMMENT;
                    break;
                }
            }
            index = xml_strindexof(line, offset, '>');
            if (-1 == index)
            {
                xml_strcat(block->buffer, line + offset);
                return 0;
            }
            block->status = EN_OK;
            xml_strncat(block->buffer, line + offset, index + 1 - offset);
            return index;
        case EN_OK:
            return -1;
        case EN_CHCK_COMMENT:
            index = xml_strfind(line, offset, "<!--");
            if ( -1 == index )
            {
                block->status = EN_SCH;
                break;
            }
            offset = index;
            block->status = EN_SCH_COMMENT;
        case EN_SCH_COMMENT:
            index = xml_strfind(line, offset, "-->");
            if (-1 == index)
            {
                return 0;
            }
            offset = index;
            block->status = EN_EMPTY;
            break;
        default:
            return -1;
            break;
        }
    }

    return -1;
}

// put char from line to block
// return : -1 error
//	         0  over
//	 	 n  index
int xml_getblocktext( char *line, int offset, struct xml_block *block )
{
	int index = xml_strindexof( line, offset, '<' );
	if ( -1 == index )
	{
		xml_strcat( block->text, line + offset );
		return 0;
	}
	xml_strncat( block->text, line + offset, index - offset );

	return index;

}



// parser the type of block
int xml_parserblocktype( struct xml_block *block )
{
	char *buffer = block->buffer;
	int size = xml_strlen( buffer );
	int cnt = 0;
	if ( '!' == buffer[1]  )	cnt++;
	if ( '-' == buffer[2] )	cnt++;
	if ( '-' == buffer[3] )	cnt++;
	if ( '-' == buffer[size - 2])	cnt++;
	if ( '-' == buffer[size - 3])	cnt++;
	if ( 5 == cnt )
	{
		block->type = EN_BLOCK5;
		return 1;
	}

	if ( ('?' == buffer[1]) && ('?' == buffer[size - 2]) )
	{
		block->type = EN_BLOCK4;
		return 1;
	}

	if ( ('/' == buffer[1]) && ('/' != buffer[size - 2]) )
	{
		block->type = EN_BLOCK3;
		return 1;
	}

	if ( ('/' != buffer[1]) && ('/' == buffer[size - 2]) )
	{
		block->type = EN_BLOCK2;
		return 1;
	}

	if ( ('/' != buffer[1]) && ('/' != buffer[size - 2]) )
	{
		block->type = EN_BLOCK1;
		return 1;
	}

	block->type = EN_BLOCK0;
	return 0;
}

// parser the xml block
//int xml_parserblockname( struct xml_block *block )
//{
//	char temp[128];
//	if ( (block->type >= 1) && (block->type <= 3) )
//	{
//		xml_searchname( block->buffer, temp, 128 );
//		xml_strcpy( block->name, temp );
//		return 1;
//	}
//	xml_strcpy( block->name, "none" );
//	return 0;
//}

// parser the xml block
void xml_parserblock( struct xml_block *block )
{
	xml_parserblocktype( block );
//	xml_parserblockname( block );
}

static void xml_printattr( struct xmlnode *node );

void xml_print( struct xmlelement *tree )
{
	if ( NULL == tree )
	{
		return ;
	}
	xml_log( "element ----------------\n" );
    xml_log("\tname = %s, text = %s\n", tree->base.name, tree->text);
    xml_printattr( tree->attribute );
	tree = (struct xmlelement *)xmlnode_getchild( tree );
	for ( ; NULL != tree;  )
	{
		xml_print( tree );
		tree = (struct xmlelement *)xmlnode_getnext( tree );
	}
    xml_log("element end-------------\n");
}


static void xml_printattr( struct xmlnode *node )
{
	for ( ;NULL != node; )
	{
        xml_log("\tattribute : name = %s, value = %s\n", node->name, node->value);
        node = node->next;
	}
}





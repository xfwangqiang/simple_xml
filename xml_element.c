/*
 * xml_element.c
 *
 *  Created on: 2017��5��23��
 *      Author: xfwangqiang
 */


/*========================================================*
 * �����޸ļ�¼��
 * <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
 *  V1.0.0  2017-06-05  xfwangqiang     ����
 *  V1.0.1  2017-06-13  xfwangqiang     ׷���˵õ���Ԫ�صĹ���
 * 			�Լ���ӡ��Ԫ�ص��ַ����Ĺ��ܣ�׷���˵õ���Ԫ�ظ���
 *  V1.0.2  2017-06-29  xfwangqiang     ���ֲ������xmlelement_delate������BUG
 *  V1.0.3  2017-06-30  xfwangqiang     ���ֲ������xmlelement_addchild������BUG,
 			BUGΪδָ����Ԫ�ؽڵ�ĸ��ڵ㡣
 * 			׷����xmlelement_getchildnum�ĺ���
 *  V1.0.3  2020-07-24  xfwangqiang     �޸��˺���xmlelement_create��getattrname�зָ�ֻ��ʶ��ո������
 *          ������xmlelement_delete��������ƴд����
 *          �޸���xmlelement_delete������ɾ�����һ���ڵ�ʧ�ܵ�Bug
 *          ������xmlelement_getchildlist����
 *          �Ż���xmlelement_makeattrstr�����еľ���
 *========================================================*/



#include <stdio.h>
#include <stdlib.h>
#include "xml_string.h"
#include "xml_element.h"
#include "xml_node.h"
#include "xml.h"


// �������غ���
static int getattrvalue(char *string, char *buffer);
static struct xmlnode *setattribute(char *name, char *value);
static int getattrname(char *string, char *buffer);
static int getattrnum(char *string);


//============================================================================
// �������ƣ�xmlelement_create
// �������ܣ�����һ��Ԫ�ؽڵ�
//
// ��������� 1 -- Ԫ�ض�������
//			2 -- Ԫ�ض����ı�
// ���������
// ����ֵ��Ԫ�ؽڵ����
// ˵��������һ��Ԫ�ؽڵ�
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
// �������ƣ�xmlelement_create
// �������ܣ�ɾ��һ��Ԫ�ؽڵ�
//
// ��������� 1 -- Ԫ�ض���
// ���������
// ����ֵ��none
// ˵����ɾ��һ��Ԫ�ؽڵ�
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
// �������ƣ�getattrnum
// �������ܣ��õ�Ԫ�����Ը���
//
// ��������� 1 -- �ַ���
// ���������
// ����ֵ��Ԫ�ص����Ը���
// ˵�����õ�Ԫ�����Ը���
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
// �������ƣ�xmlelement_getattrnum
// �������ܣ��õ�Ԫ�����Ը���
//
// ��������� 1 -- Ԫ�ض���
// ���������
// ����ֵ��Ԫ�ص����Ը���
// ˵�����õ�Ԫ�����Ը���
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
// �������ƣ�xmlelement_parserattr
// �������ܣ���������
//
// ��������� 1 -- Ԫ�ض���
//			2 -- �ַ���
// ���������
// ����ֵ��
// ˵������������
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
// �������ƣ�getattrname
// �������ܣ���ȡԪ�ص�������
//
// ��������� 1 -- Ԫ�ض�����������
//			2 -- Ԫ�ض�������ֵ
// ���������
// ����ֵ��
// ˵������ȡԪ�ص�������
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
// �������ƣ�getattrvalue
// �������ܣ���ȡԪ�ص�����ֵ
//
// ��������� 1 -- Ԫ�ض�����������
//			2 -- Ԫ�ض�������ֵ
// ���������
// ����ֵ��Ԫ�ص����Խڵ����
// ˵������ȡԪ�ص�����ֵ
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
// �������ƣ�setattribute
// �������ܣ�����Ԫ�ؽڵ������
//
// ��������� 1 -- Ԫ�ض�����������
//			2 -- Ԫ�ض�������ֵ
// ���������
// ����ֵ��Ԫ�ص����Խڵ����
// ˵��������Ԫ�ؽڵ������
//============================================================================
static struct xmlnode *setattribute( char *name, char *value  )
{
	char *newname = xml_strnew( name );
	char *newvalue = xml_strnew( value );
	return new_xmlnode( newname, newvalue, ATTRIBUTE_NODE );
}

//============================================================================
// �������ƣ�xmlelement_gettag
// �������ܣ��õ�Ԫ�ؽڵ�ı�ǩ
//
// ��������� 1 -- Ԫ�ض�������
// ��������� 2 -- Ԫ�ض�����ı��ڵ�ı�ǩ
// ����ֵ����ǩ�ĳ���
// ˵�����õ�Ԫ�ؽڵ�ı�ǩ
//============================================================================
int xmlelement_gettag( void *this, char *buffer )
{
	struct xmlelement *element = (struct xmlelement *)this;
	return xml_strcpy( buffer, element->base.name);
	// return 1;
}



//============================================================================
// �������ƣ�xmlelement_checkattribute
// �������ܣ����Ԫ�ؽڵ�������������Ƿ����
//
// ��������� 1 -- Ԫ�ض�������
//			  2 -- Ԫ�ض������Խڵ������
//			  3 -- ������ַ���
// ���������
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵�������Ԫ�ؽڵ�������������Ƿ����
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
// �������ƣ�xmlelement_getattribute
// �������ܣ��õ�Ԫ�ؽڵ������
//
// ��������� 1 -- Ԫ�ض�������
//			2 -- Ԫ�ض������Խڵ������
// ���������
// ����ֵ��none
// ˵�����õ�Ԫ�ؽڵ������
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
// �������ƣ�xmlelement_getattrbyint
// �������ܣ��õ�Ԫ�ؽڵ�����Ե�����ֵ
//
// ��������� 1 -- Ԫ�ض�������
//			  2 -- Ԫ�ض������Խڵ������
// ��������� 3 -- Ԫ�ض������������ֵ
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵�����õ�Ԫ�ؽڵ�����Ե�����ֵ
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
// �������ƣ�xmlelement_getattrbyfloat
// �������ܣ��õ�Ԫ�ؽڵ�����Եĸ�������ֵ
//
// ��������� 1 -- Ԫ�ض�������
//			  2 -- Ԫ�ض������Խڵ������
// ��������� 3 -- Ԫ�ض�������Ը�������ֵ
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵�����õ�Ԫ�ؽڵ�����Եĸ�������ֵ
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
// �������ƣ�xmlelement_setattribute
// �������ܣ�����Ԫ�ؽڵ������
//
// ��������� 1 -- Ԫ�ض�������
//			  2 -- Ԫ�ض������Խڵ������
//			  3 -- Ԫ�ض�������ֵ
// ���������
// ����ֵ��none
// ˵��������Ԫ�ؽڵ������
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
// �������ƣ�xmlelement_setattrbyint
// �������ܣ�����Ԫ�ؽڵ�����Ե�����ֵ
//
// ��������� 1 -- Ԫ�ض�������
//			  2 -- Ԫ�ض������Խڵ������
//			  3 -- Ԫ�ض������������ֵ
// ��������� 
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵��������Ԫ�ؽڵ�����Ե�����ֵ
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
// �������ƣ�xmlelement_indexofattr
// �������ܣ��õ�Ԫ�ؽڵ������
//
// ��������� 1 -- Ԫ�ض�������
//			2 -- Ԫ�ض�������������
// �������������ֵ
// ����ֵ������ֵ�ַ�������
// ˵�����õ�Ԫ�ؽڵ������
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
// �������ƣ�xmlelement_gettext
// �������ܣ��õ�Ԫ�ؽڵ���ı�
//
// ��������� 1 -- Ԫ�ض�������
// ��������� 2 -- Ԫ�ض�����ı��ڵ���ı�
// ����ֵ���ı��ĳ���
// ˵�����õ�Ԫ�ؽڵ���ı�
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
// �������ƣ�xmlelement_getchild
// �������ܣ��õ�Ԫ�ؽڵ����Ԫ�ؽڵ�
//
// ��������� 1 -- Ԫ�ض�������
// ��������� 2 -- Ԫ�ض������Ԫ�ؽڵ�ı�ǩ
// ����ֵ��Ԫ�ض������Ԫ�ؽڵ�
// ˵�����õ�Ԫ�ؽڵ����Ԫ�ؽڵ�
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
// �������ƣ�xmlelement_getchildlist
// �������ܣ��õ�Ԫ�ؽڵ����Ԫ�ؽڵ��б�
//
// ��������� 1 -- Ԫ�ض�������
//           2 -- Ԫ�ض������Ԫ�ؽڵ�ı�ǩ
//           3 -- ������Ԫ���б�
//           4 -- ��Ԫ���б���
// ��������� 3 -- Ԫ�ض������Ԫ�ؽڵ�ı�ǩ
// ����ֵ����Ԫ���б���
// ˵�����õ�Ԫ�ؽڵ����Ԫ�ؽڵ��б�
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
// �������ƣ�xmlelement_indexofchild
// �������ܣ�����Ԫ�ؽڵ����Ԫ�ؽڵ�
//
// ��������� 1 -- Ԫ�ض�������
// ��������� 2 -- ����
// ��������� none
// ����ֵ��Ԫ�ض������Ԫ�ؽڵ�
// ˵��������Ԫ�ؽڵ����Ԫ�ؽڵ�
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
// �������ƣ�xmlelement_getchildnum
// �������ܣ��õ�Ԫ�ؽڵ����Ԫ�ؽڵ�����
//
// ��������� 1 -- Ԫ�ض�������
// ��������� nonde
// ����ֵ��Ԫ�ض������Ԫ�ؽڵ�����
// ˵�����õ�Ԫ�ؽڵ����Ԫ�ؽڵ�����
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
// �������ƣ�xmlelement_makeheadstr
// �������ܣ�����Ԫ�ؽڵ��ͷ�ַ���
//
// ��������� 1 -- Ԫ�ض�������
// ��������� 2 -- ���ɵ��ַ���
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵��������Ԫ�ؽڵ��ͷ�ַ���
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
// �������ƣ�xmlelement_makeendstr
// �������ܣ�����Ԫ�ؽڵ�Ľ����ַ���
//
// ��������� 1 -- Ԫ�ض�������
// ��������� 2 -- ���ɵ��ַ���
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵��������Ԫ�ؽڵ�Ľ����ַ���
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
// �������ƣ�xmlelement_makeattrstr
// �������ܣ�����Ԫ�ؽڵ�������ַ���
//
// ��������� 1 -- Ԫ�ض�������
// ��������� 2 -- ���ɵ��ַ���
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵��������Ԫ�ؽڵ�������ַ���
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
// �������ƣ�xmlelement_addchild
// �������ܣ�������Ԫ��
//
// ��������� 1 -- Ԫ�ض�������
//			  2 -- ��Ԫ�ض���
// ��������� none
// ����ֵ��0 -- ʧ�� 1 -- �ɹ�
// ˵����������Ԫ��
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


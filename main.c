/*
 * main.c
 *
 *  Created on: 2017Äê5ÔÂ22ÈÕ
 *      Author: xfwangqiang
 */

#include <stdio.h>
#include "xml.h"
#include "xml_node.h"
#include "xml_element.h"
#include "xml_string.h"

int main( void )
{	
	struct xmlelement *tree = xml_load( "xmltest.xml" );
	struct xmlelement *element = (struct xmlelement *)tree->base.child;
	 xml_print( element );
	return 1;
}


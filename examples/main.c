/*
 * main.c
 *
 *  Created on: 2017年5月22日
 *      Author: xfwangqiang
 */

#include <stdio.h>
#include "../inc/xml_export.h"

#define EXAMPLE_FILE        ("xmltest.xml")

static void test1(void)
{
    char buffer[100] = { 0 };
    int size = xml_inttostr(123, buffer, 100);
    if (0 != size)
    {
        xml_log("%s\n", buffer);
    }
    size = xml_hextostr(100, buffer, 100);
    if (0 != size)
    {
        xml_log("%s\n", buffer);
    }
    size = xml_inttostr(99, buffer, 100);
    if (0 != size)
    {
        xml_log("%s\n", buffer);
    }
}

int main( int argc, char *argv[] )
{	
    char *file_name = EXAMPLE_FILE;
    if (argc >= 2)
    {
        file_name = argv[1];
    }

    struct xmlelement *tree = xml_load(file_name);
    struct xmlelement *element = (struct xmlelement *)tree->base.child;
	xml_print( element );
    xmlelement_setattrbyint(element, "test", 2);
    xml_save(tree, "test.xml");

    test1();
	return 1;
}


/*
 * main.c
 *
 *  Created on: 2017年5月22日
 *      Author: xfwangqiang
 */

#include <stdio.h>
#include "../inc/xml_export.h"

#define EXAMPLE_FILE ("xmltest.xml")

static void test1(void)
{
    char buffer[100] = {0};
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

void xml_example_run(void *param)
{
    char *file_name = (char *)param;
    struct xmlelement *tree = xml_load(file_name);
    struct xmlelement *element = (struct xmlelement *)tree->base.child;
    xml_print(element);
    xmlelement_setattrbyint(element, "test", 2);
    xml_save(tree, "test.xml");
    test1();
    xml_free(file_name);
    file_name = NULL;
}

#if (OS_VER == OS_RTTHREAD)
#include <rtthread.h>
#endif

int xml_example(int argc, char *argv[])
{
    char *file_name = EXAMPLE_FILE;
    if (argc >= 2)
    {
        file_name = argv[1];
    }
    file_name = xml_strnew(file_name);
    xml_log("xml file : %s\n", file_name);
    #if (OS_VER != OS_RTTHREAD)
    xml_example_run(file_name);
    #else
    rt_thread_t xml_thread = rt_thread_create("xml_demo",
                                        xml_example_run, file_name,
                                        10240, 10, 5);
    if (RT_NULL != xml_thread)
    {
        rt_thread_startup(xml_thread);
    }
    else
    {
        xml_free(file_name);
        file_name = NULL;
        xml_log("Can not create thread for xml_demo!");
    }
    #endif

    return 1;
}

#if (OS_VER == OS_RTTHREAD)
MSH_FUNCTION_EXPORT_CMD(xml_example, xml_test, simaple xml example);
#endif

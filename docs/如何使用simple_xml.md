# 如何使用simple_xml
## 使用哪些接口
大部分xml解析器都使用了xml node对象，并以xml node对象为基础设计了主要的功能接口。本xml解析器的最大特点就是基于xml node对象，派生了xml element对象，使得相关函数对xml element的操作更容易。因此，对于应用simple_xml来说，主要关注xml element对象相应的接口。  
1. 导入导出xml文件  
simple_xml解析器包含了文件读取和保存的功能，不像cJSON库，只能解析字符串。  
主要的接口函数有两个：
* 加载函数
```
/**
 * @brief   加载一个xml文件
 * 
 * @param   path xml文件的路径
 * 
 * @return 返回xml根节点元素对象
 * */
struct xmlelement * xml_load( char * path );
```
函数xml_load返回的对象为xml根节点，对xml格式的文件已经完成了解析，xml文件中的所有信息都已经链接在xml根节点对象中，下面所有的操作都要以这个根节点为起始。见xml_test.c文件下面的xml_example_run函数中的用法。
* 保存函数
```
/**
 * @brief   将一个xml元素（根节点）保存到指定的xml文件中
 *
 * @param   tree xml根节点元素对象
 * @param   path xml文件的路径
 *
 * @return 1 : 成功, 其它失败
 * */
int xml_save(struct xmlelement *tree, char *path);
```
如何对xml根节点下的子元素或属性只要做过修改，都需要调试xml_save函数才能完成保存到xml文件中，否则修改会丢失。保存后xml根节点元素依然还存在，如果确认不再需要，请释放内存，防止内存泄漏。见xml_test.c文件下面的xml_example_run函数中的用法。

2. xml element  
xml element主要由tag, attribute, text, child element列表组成。在获得一个xml根节点元素后，主要就是用下面的接口来设置和读取。
* 创建xml element对象
```
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
```
例如：
```
struct xmlelement *tree = xmlelement_create("name", "test create", ELEMENT_NODE);

<name>test create
</name>
```
* 销毁xml element对象
```
/**
 * @brief  删除一个元素节点，释放元素的text节点，attribute列表, childs列表，最后释放
 * 自身的内存。当包含子元素节点时，存在递归调用的可能。
 * 
 * @param   this 元素对象
 * @return  成功：1， 失败：其它
 */
int xmlelement_delete( void *this );
```
释放childs列表的同时，也会检测每个child是否也有child，如果有，也会一起删除。
* 获得xml element对象的tag
```
/**
 * @brief   得到元素节点的标签
 * 
 * @param   this 元素对象自身
 * @param   buffer 元素对象的文本节点的标签
 * 
 * @return  标签的字符长度
 */
int xmlelement_gettag( void *this, char *buffer );
```
* 获得xml element对象的attribute
xml element对象可能有多个属性，下面这几个函数都是会用得到的。
```

/**
 * @brief   得到元素属性个数
 *
 * @param   element 元素对象
 *
 * @return  元素属性数量
 */
int xmlelement_getattrnum(struct xmlelement *element);
```
以字符串形式得到属性
```
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
```
通过索引值得到元素节点的属性
```
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
```
以整形值形式得到属性
```
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
```
以浮点值形式得到属性
```
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
```
检测元素的某属性值是否等于目标字符串
```
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
```
* 设置xml element对象的attribute  

以字符串格式设置属性值
```
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
```
以十进制整形格式设置属性值
```
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

```
以十六进制整形格式设置属性值
```
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

```
以浮点形格式设置属性值
```
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
```
* 获得xml element对象的text
```
/**
 * @brief   得到元素节点的文本
 *
 * @param   this    元素对象自身
 * @param   buffer  字符串属性值的buffer
 *
 * @return 文本的长度
 */
int xmlelement_gettext(void *this, char *buffer);
```
* 设置xml element对象的text
```
/**
 * @brief   设置元素节点的文本
 *
 * @param   this    元素对象自身
 * @param   buffer  字符串属性值的buffer
 *
 * @return 成功 ： 1， 失败：其它
 */
int xmlelement_settext( void *this, char *buffer );
```
* 遍历xml element对象的child element列表  
方式1, 直接得到子元素节点列表，用户自己再使用for循环来遍历每个子节点
```
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
```
方式2，通过tag得到第一个子元素节点，再配合xmlnode_getnext()来遍历每个子节点。推荐使用这种，不需要额外分配内存。
```

/**
 * @brief   得到元素节点的子元素节点, 当tag为NULL时，直接返回第一个子节点元素
 *
 * @param   this        元素对象自身
 * @param   tag         元素对象的子元素节点的标签
 *
 * @return 子元素节点列表中的第一个子节点
 */
struct xmlelement * xmlelement_getchild(void *this, char *tag);
```
方式3，通过xmlelement_getchildnum得到子节点数量，再使用xmlelement_indexofchild遍历每个子节点。
```
/**
 * @brief   得到元素节点的子元素节点数量
 *
 * @param   this        元素对象自身
 *
 * @return 元素对象的子元素节点数量
 */
int xmlelement_getchildnum(void *this);


/**
 * @brief   索引元素节点的子元素节点
 *
 * @param   this        元素对象自身
 * @param   index       索引
 *
 * @return 元素对象的子元素节点
 */
struct xmlelement * xmlelement_indexofchild( void *this, int index )
```
* 增加child element
```
/**
 * @brief   增加子元素
 *
 * @param   this        元素对象自身
 * @param   child       子元素对象
 *
 * @return 成功：1， 失败：0
 */
int xmlelement_addchild( void *this, struct xmlelement *child );
```
* 删除child element  
child element也是一个普通的元素，删除方法是使用xmlelement_delete函数。
如果是删除所有的child，可以使用下面的函数：
```
/**
 * @brief   删除子元素列表, 当本函数被xmlelement_delete调用时，存在递归调用的情况。
 *
 * @param   this        元素对象自身
 *
 * @return 成功：1， 失败：0
 */
int xmlelement_deletechilds(void *this);
```
## 如何扩展文件系统
以Linux系统为例，实现open, gets, puts, close 4个函数，
```
#include <stdio.h>
void *local_open(char *file_name, char *mode)
{
    FILE *file = fopen(file_name, mode);
    return file;
}

char *local_gets(char *buf, int size, void *file)
{
    return fgets(buf, size, file);
}

int local_puts(char *buf, void *file)
{
    return fputs(buf, file);
}

void local_close(void *file)
{
    fclose(file);
}
```
定义一个xmlfileops的结构体，
```
static struct xmlfileops ops = {
    .open = local_open,
    .gets = local_gets,
    .puts = local_puts,
    .close = local_close};
```
在使用其它xml软件栈的函数之前，使用xml_fconfig函数将xmlfileops的结构体对象传入xml软件栈
```
xml_fconfig(&ops);
```
## 如何扩展动态内存管理
以Linux系统为例，实现malloc, free, memset 3个函数，这里使用了宏定义，也可以使用函数的方式
```
#include <memory.h>
#define local_malloc malloc
#define local_free free
#define local_memset memset
```
定义一个xmlmemops的结构体，
```
static struct xmlmemops ops = {
    .malloc = local_malloc,
    .free = local_free,
    .memset = local_memset,
};
```
在使用其它xml软件栈的函数之前，使用xml_mconfig函数将xmlmemops的结构体对象传入xml软件栈
```
xml_mconfig(&ops);
```

## 内存检查功能
simple_xml的软件栈中使用动态内存，对于动态内存操作不当时，容易产生内存泄漏，造成软件质量问题。为此，我们增加了内存检查功能。  
* 使用方法  
在编译选项中加入XML_MEM_CHECK宏，就可以开启内存检查功能，通过调用xml_memory_show函数就可以打印出内存使用情况。例如：
```
Memory in xml stack
        total : 0
        blocks : 0
```
* 检查原理  
在xml_malloc中将申请到的内存块的地址和大小的保存到链表中，更新内存块数和内存容量。  
在xml_free中，在链表中查找到内存块，并从链表中删除，更新内存块数和内存容量
* 风险  
检查的策略中也使用了动态内存，使用此功能时，会影响最大内存的需求，推荐只在单元测试中打开此功能
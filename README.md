# simple_xml 简介
基于C语言的XML解析器，已有的一些开源解析器都与操作系统相关，对于一些并不主流的操作系统以及嵌入式操作系统，想应用XML文件，就显得比较困难。开发本项目的目的在于开发一个能在多平台应用的代码，但因为接触的平台并不多，需要大家的帮助来慢慢改善。目前代码已经应用在WIN10、Linux、rt-thread、VxWorks等主流操作系统。
   

# simple_xml 特点
* 实现了xml node对象，是本解析器最核心的功能实现
* 基于xml node对象，派生了xml element对象，使得相关函数对xml element的操作更容易
* 应用程序主要使用xml element对象的函数来读取或写入xml element的属性，文本，子节点等
* 提供了字符串的相关函数集，减少对标准库的依赖，便于跨平台的支持
* 支持全功能OS平台、也支持运行在裸机环境，但使用了动态内存分配，裸机环境下要考虑文件系统以及动态内核分配的实现
* 提供了在使用xml会用得上的一些函数，比如xml_ishex,用来判断字符串是否为hex格式的字符串
* 提供了文件操作抽象层和内存操作抽象层，用于分离与平台相关的API，主要目的是为了方便扩展未支持的文件系统和动态内存管理库

# 目录结构
| 名称          | 描述                                                    |
| ------------- | ------------------------------------------------------- |
| docs           | 计划对各模块进行详细的解释说明，目前还没有完整 |
| examples    | 包含一个main.c和xmltest.xml作为示例          |
| inc | simple_xml的头文件                                  |
| src      | simple_xml的源文件                                            |
| LICENSE | MIT License | 
| README.md | 对SimpleXML的说明 | 
| Sconscript | rt-thread软件包中编译脚本 |


# 移植方法
1. 将inc和src两个文件夹Copy到自己的目录
2. 在调试simple_xml的API的.c文件中包含头文件xml_export.h
## 已支持OS平台
3. 配置xml_config.h中的宏OS_VER为目标系统
## 未支持平台
4. 配置xml_config.h中的宏OS_VER为OS_UNKOWN
5. 调用xml_fconfig函数和xml_mconfig函数配置文件抽象操作函数和内存抽象操作函数

# 使用示例
由于目前已经没有VxWorks的工作环境，暂无法测试，但从使用经验上来看，跟Linux基本一样。
## 基于GCC+Makefile构建
仅在Linux的环境下做了测试。在WIN10下makefile的脚本未作兼容，使用起来会有问题，建议在WIN10使用的示例直接使用VS2019来搭建测试环境。
1. 进入目录example/build/make
2. 在命令行输入:  
```bash
$ make
```
* 3. 在命令行输入:  
```bash
$ ./simple_xml ../../xmltest.xml
```
4. 检查打印内容是否与xmltest.xml一致
## 基于GCC+Scons构建
## 基于rt-thread构建
* 手动移植
1. 将整个simple_xml目录放入rt-thread BSP里的packages目录
2. 手动在rtconfig.h头文件中加入
```C
#define PKG_USING_SIMPLE_XML
#define PKG_USING_SIMPLE_XML_EXAMPLE
```
3. 在ENV工具中编译
```bash
> scons
```
4. 下载固件到目标板或qemu中。
5. 将example/xmltest.xml放入rt-thread的文件系统中。
6. 输入以下指令
```bash
msh />xml_test
```
* 使用软件包
1. 在ENV环境下使用menuconfig，找到simple_xml
```
RT-Thread online packages 
    → IoT - internet of things 
        → simple_xml  : Based on the XML parser of the C language.
```
2. 下载软件包
```bash
> pkgs --update
```
3. 同手动移植第3步
# 仓库
* github : [https://github.com/xfwangqiang/simple_xml.git](https://github.com/xfwangqiang/simple_xml.git)
* gitee : [https://gitee.com/xfwangqiang/simple_xml.git](https://gitee.com/xfwangqiang/simple_xml.git)
# 文档
* [如何使用simple_xml](docs/如何使用simple_xml.md)
# 日志
* 2022年，在RT-Thread RTOS平台应用，以软件包的形式提供。对文件目录作了优化。增加了文件模块和内存管理模块，抽象了文件的读写和内存操作，便于扩展文件系统和动态管理库。增加了基于makefile的构建工程作为示例。
* 2020年，在Linux平台应用。  
* 2018年，在VxWorks平台应用。  

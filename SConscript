Import('RTT_ROOT')
Import('rtconfig')
from building import *

cwd = GetCurrentDir()
src = Glob('src/*.c')
CPPPATH = [cwd + '/inc']
if GetDepend(['PKG_USING_SIMPLE_XML_EXAMPLE']):
    src += Glob('examples/xml_test.c')


group = DefineGroup('simple_xml', src, depend = ['PKG_USING_SIMPLE_XML'], CPPPATH = CPPPATH)

Return('group')

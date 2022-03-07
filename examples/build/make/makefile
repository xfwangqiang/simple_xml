
build = ./build

COMPILER := 
CC = $(COMPILER)gcc
CCOPT = -DOS_VER=1 -DXML_MEM_CHECK
LD = $(COMPLIER)ld

RM = rm
MKDIR = mkdir

# .c file list
main_file = ../../main.c 
example_file = ../../xml_test.c
src = ../../../src
src_file = $(main_file)
src_file += $(example_file)
src_files += $(src)/xml.c
src_files += $(src)/xml_string.c
src_files += $(src)/xml_file.c
src_files += $(src)/xml_node.c
src_files += $(src)/xml_element.c
src_files += $(src)/xml_memory.c

# .obj file list
obj_files = $(build)/main.o 
obj_files += $(build)/xml_test.o 
obj_files += $(build)/xml.o 
obj_files += $(build)/xml_string.o 
obj_files += $(build)/xml_file.o 
obj_files += $(build)/xml_node.o
obj_files += $(build)/xml_element.o
obj_files += $(build)/xml_memory.o
target = simaple_xml


.PHONY: example
example: build_fold
	$(CC) $(CCOPT) -o $(build)/main.o -c $(main_file)
	$(CC) $(CCOPT) -o $(build)/xml_test.o -c $(example_file)
	$(CC) $(CCOPT) -o $(build)/xml.o -c $(src)/xml.c
	$(CC) $(CCOPT) -o $(build)/xml_string.o -c $(src)/xml_string.c
	$(CC) $(CCOPT) -o $(build)/xml_file.o -c $(src)/xml_file.c
	$(CC) $(CCOPT) -o $(build)/xml_node.o -c $(src)/xml_node.c
	$(CC) $(CCOPT) -o $(build)/xml_element.o -c $(src)/xml_element.c
	$(CC) $(CCOPT) -o $(build)/xml_memory.o -c $(src)/xml_memory.c
	$(CC) $(CCOPT) -o $(target) $(obj_files)
	@echo "Ouput $(target) OK!"

build_fold:
	$(MKDIR) -p $(build)

	
.PHONY: clean
clean: 
	$(RM) -rf $(build)
	$(RM) $(target)
module_name := mathUtils
root_obj_dir  := ./../../obj
source_subdirs := $(shell find ../ -type d -name "??*")
root_include_dir := ./../../include/
include_dirs  := $(shell find $(root_include_dir) -type d -name "??*")

sources := mathUtils.cpp
depend_headers := $(shell find $(root_include_dir) -name "*.h")

compile_flags := -Wall
CC = g++

$(root_obj_dir)/$(module_name).o: $(module_name).o
	cp *.o $(root_obj_dir) 

$(module_name).o: $(sources) $(depend_headers)
	$(CC) $(compile_flags) -c $(sources) $< $(addprefix -I, $(include_dirs)/) $(addprefix -I./, $(source_subdirs)/)
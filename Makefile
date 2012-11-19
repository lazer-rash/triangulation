result_name := triangulation
root_binaries_dir := bin
root_obj_dir  := obj
root_source_dir  := src
source_subdir := $(shell cd $(root_source_dir);find . -type d -name "??*")
root_include_dir  := include

link_flags := -lglut -lGL -lGLU 
CC = g++

all: $(root_binaries_dir)
	if [ ! -d "$(root_obj_dir)" ]; then  mkdir -p $(root_obj_dir)/; fi
	make compile_src
	make boild_obj	

$(root_binaries_dir):
	mkdir -p $(root_binaries_dir)

run:
	./$(root_binaries_dir)/$(result_name)

clean:
	rm -f ./$(root_binaries_dir)/$(result_name)
	rm -rf obj
	rm -rf bin
	rm -rf $$(find . -type f -name "*.o")

compile_src:
	cd $(root_source_dir);for i in $$(find . -type d -name "??*") ; do cd $$i/; make; cd .. ; done

boild_obj:
	cd $(root_obj_dir); $(CC) -o $(result_name) *.o $(link_flags) ; mv $(result_name) ../$(root_binaries_dir)

backup:
	cp -r ./ ../backup/
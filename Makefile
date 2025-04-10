 # makefile for use on Linux; may also work on other operating systems...
.SILENT:

src_files := $(shell find -type f -name '*.cpp')
backend_files := $(shell find ./src/backend ./src/utils ./src/main.cpp -type f -name '*.cpp' ! -name 'test*')
routing_only_files := $(shell find ./src/backend ./src/utils ./src/main.cpp -type f -name '*.cpp' ! -name 'test*' | sed '/\/fragmentation\//d; /\/transport\//d')
integration: $(src_files)
	echo 'Building integration project...'
	g++ $(src_files) -lpthread -o integration
	echo 'done!'

backend: $(backend_files)
	echo 'Building integration project...'
	g++ $(backend_files) -lpthread -o integration
	echo 'done!'

routing_only: $(routing_only_files)
	echo 'Building integration project...'
	g++ $(routing_only_files) -lpthread -o integration
	echo 'done!'

clean:
	rm integration
	rm *.net


 # makefile for use on Linux; may also work on other operating systems...
.SILENT:

src_files := $(shell find -type f -name '*.cpp')

integration: $(src_files)
	echo 'Building integration project...'
	g++ $(src_files) -lpthread -o integration
	echo 'done!'

clean:
	rm integration


# Makefile

cc := gcc
cflags := -std=c11 -fPIE -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes

target := ccc
test_target := ccc_test

base_dir   := $(shell pwd)
src_dir    := $(base_dir)/src
build_dir  := $(base_dir)/build
test_dir   := $(base_dir)/test

_objs := main.o lex.o
objs := $(patsubst %,$(build_dir)/%,$(_objs))

test_objs := $(build_dir)/test.o $(build_dir)/lex.o

.PHONY: build cgreen clean test

$(target): build $(objs)
	$(cc) -o $@ $(objs)

$(build_dir)/%.o: $(src_dir)/%.c
	$(cc) -c -o $@ $< $(cflags)

build:
	if [ ! -d $(build_dir) ]; then mkdir $(build_dir); fi

clean:
	rm -rf $(target) $(test_target) build/* $(test_dir)/*.dylib

cgreen:
	if [ ! -e $(test_dir)/libcgreen.dylib ]; then \
		cgreen_tmp=$(shell mktemp -d $(test_dir)/cgreen.XXXXXX); \
		git clone https://github.com/cgreen-devs/cgreen $$cgreen_tmp; \
		cd $(build_dir) && cmake -DCMAKE_INSTALL_PREFIX=$$cgreen_tmp $$cgreen_tmp && make && make install && cd $(base_dir); \
		cp $$cgreen_tmp/lib/libcgreen*.dylib $$cgreen_tmp/include/cgreen/cgreen.h $(test_dir) && rm -rf $$cgreen_tmp; \
	fi;

test: build cgreen $(test_objs)
	$(cc) -o $(test_target) $(test_objs) -L$(test_dir) -lcgreen
	$(base_dir)/$(test_target)

$(build_dir)/test.o: $(test_dir)/test.c
	$(cc) -c -o $@ $< -I$(test_dir) -I$(src_dir)

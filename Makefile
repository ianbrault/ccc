# Makefile

cc := gcc
cflags := -std=c11 -fPIE -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes

target := ccc
test_target := ccc_test

base_dir   := $(shell pwd)
src_dir    := $(base_dir)/src
build_dir  := $(base_dir)/build
test_dir   := $(base_dir)/test
cgreen_dir := $(test_dir)/cgreen

cgreen_inc := $(cgreen_dir)/include
cgreen_lib := $(cgreen_dir)/lib

_objs := main.o lex.o
objs := $(patsubst %,$(build_dir)/%,$(_objs))

.PHONY: clean test cgreen

$(target): $(objs)
	if [ ! -d $(build_dir) ]; then mkdir $(build_dir); fi
	$(cc) -o $@ $^

$(build_dir)/%.o: $(src_dir)/%.c
	$(cc) -c -o $@ $< $(cflags)

clean:
	rm -rf $(target) build/*

test: cgreen $(build_dir)/test.o
	$(cc) -o $(test_target) $(build_dir)/test.o -L$(cgreen_lib) -lcgreen
	$(base_dir)/$(test_target)

$(build_dir)/test.o: $(test_dir)/test.c
	$(cc) -c -o $@ $< -I$(cgreen_inc)

cgreen:
	if [ ! -d $(build_dir) ]; then mkdir $(build_dir); fi
	if [ ! -d $(cgreen_lib) ]; then cd $(build_dir) && cmake -DCMAKE_INSTALL_PREFIX=$(cgreen_dir) $(cgreen_dir) && make && make install && cd $(base_dir); fi

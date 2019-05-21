# Makefile

cc := gcc
cflags := -std=c11 -fPIE -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes

target := ccc

base_dir  := $(shell pwd)
src_dir   := $(base_dir)/src
build_dir := $(base_dir)/build
check_dir := $(base_dir)/test/check

_objs := main.o lex.o
objs := $(patsubst %,$(build_dir)/%,$(_objs))

.PHONY: clean test

$(target): $(objs)
	if [ ! -d $(build_dir) ]; then mkdir $(build_dir); fi
	$(cc) -o $@ $^

$(build_dir)/%.o: $(src_dir)/%.c
	$(cc) -c -o $@ $< $(cflags)

clean:
	rm -rf $(target) build

test:
	

# Makefile

cc := gcc
cflags := -std=c11 -fPIE -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes

target := ccc
src_dir := src
obj_dir := build

_objs := main.o lex.o
objs := $(patsubst %,$(obj_dir)/%,$(_objs))

.PHONY: clean test cmocka

$(target): $(objs)
	$(cc) -o $@ $^

$(obj_dir)/%.o: $(src_dir)/%.c
	$(cc) -c -o $@ $< $(cflags)

clean:
	rm -rf $(target) build/*

test: cmocka

cmocka:
	cd build && cmake ../test/cmocka && make && cd ..

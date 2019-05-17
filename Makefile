# Makefile

cc := gcc
cflags := -std=c11 -pie -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wformat=2

target := ccc
src_dir := src
obj_dir := build

_objs := main.o
objs := $(patsubst %,$(obj_dir)/%,$(_objs))

.PHONY: clean

$(target): $(objs)
	$(cc) -o $@ $^

$(obj_dir)/%.o: $(src_dir)/%.c
	$(cc) -c -o $@ $< $(cflags)

clean:
	rm -rf $(target) $(objs)

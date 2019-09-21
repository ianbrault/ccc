# Makefile

cc := gcc
cflags := -std=c11 -Isrc -fPIE -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes

target := ccc
test_target := ccc_test

base_dir   := $(shell pwd)
src_dir    := $(base_dir)/src
build_dir  := $(base_dir)/build
test_dir   := $(base_dir)/test

_objs := main.o error.o lex.o eval.o
objs := $(patsubst %,$(build_dir)/%,$(_objs))

# _test_objs := test.o lex.o eval.o utils.o
_test_objs := test.o lex.o utils.o
test_objs := $(patsubst %,$(build_dir)/%,$(_test_objs))

.PHONY: build cgreen clean test

$(target): build $(objs)
	$(cc) -o $@ $(objs)

$(build_dir)/%.o: $(src_dir)/%.c
	$(cc) -c -o $@ $< $(cflags)

build:
	if [ ! -d $(build_dir) ]; then mkdir $(build_dir); fi

clean:
	rm -rf $(target) $(test_target) build/* $(test_dir)/*.dylib $(test_dir)/cgreen

cgreen:
	if [ ! -e $(test_dir)/libcgreen.dylib ]; then \
		cgreen_tmp=$$(mktemp -d $(test_dir)/cgreen.XXXXXX); \
		git clone https://github.com/cgreen-devs/cgreen $$cgreen_tmp; \
		cd $(build_dir) && cmake -DCMAKE_INSTALL_PREFIX=$$cgreen_tmp $$cgreen_tmp && make && make install && cd $(base_dir); \
		cp $$cgreen_tmp/lib/libcgreen*.dylib $(test_dir) && mv $$cgreen_tmp/include/cgreen $(test_dir) && rm -rf $$cgreen_tmp; \
	fi;

test: build cgreen $(test_objs)
	$(cc) -o $(test_target) $(test_objs) -L$(test_dir) -lcgreen
	$(base_dir)/$(test_target)

$(build_dir)/test.o: $(test_dir)/test.c
	$(cc) -c -o $@ $< -I$(test_dir) -I$(src_dir)

$(build_dir)/utils.o: $(test_dir)/utils.c
	$(cc) -c -o $@ $< -I$(test_dir) -I$(src_dir)

test_clean:
	rm -rf $(test_target) build/*

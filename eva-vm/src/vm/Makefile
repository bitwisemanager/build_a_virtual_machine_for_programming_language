CC=clang++
CFLAGS=-std=c++17 -Wall -ggdb3

O=../../../build
OBJS= $(O)/eva_vm.o

.PHONY: all run clean generate debug prepare

all: clean $(O)/eva_vm

$(O)/eva_vm: $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

run: all
	@$(O)/eva_vm

clean: prepare
ifeq ($(wildcard $(O)/.*),)
	@rm $(O)/*
endif

$(O)/%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

generate:
	cd ../parser; $(MAKE) $@

debug: $(O)/eva_vm
	gdb $< --tui

prepare:
ifeq ($(wildcard $(O)/.*),)
	@mkdir $(O)
endif

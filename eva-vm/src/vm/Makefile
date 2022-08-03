CC=clang++
CFLAGS=-std=c++17 -Wall -ggdb3

O=../../../build
OBJS= $(O)/eva_vm.o

all: clean $(O)/eva_vm

$(O)/eva_vm: $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

run: all
	@$(O)/eva_vm

clean: prepare
	@rm $(O)/*

$(O)/%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

debug: $(O)/eva_vm
	gdb $< --tui

prepare:
ifeq ($(wildcard $(O)/.*),)
	@mkdir $(O)
endif

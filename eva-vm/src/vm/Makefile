CC=clang++
CFLAGS=-std=c++17 -Wall -ggdb3

O=../../../build
OBJS= $(O)/eva_vm.o

all: prepare eva_vm

eva_vm: $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(O)/$@

run: all
	@$(O)/eva_vm

clean: prepare
	@rm $(O)/*

$(O)/%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

prepare:
ifeq ($(wildcard $(O)/.*),)
	@mkdir $(O)
endif

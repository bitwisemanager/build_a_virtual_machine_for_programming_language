
MAKE_VM=cd ./eva-vm/src/vm; $(MAKE) --no-print-directory

.PHONY: all run clean generate

all:
	@$(MAKE_VM)

run:
	@$(MAKE_VM) $@

clean:
	@$(MAKE_VM) $@

generate:
	@$(MAKE_VM) $@

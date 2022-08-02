
MAKE_VM=cd ./eva-vm/src/vm; $(MAKE) --no-print-directory

all:
	@$(MAKE_VM)

run:
	@$(MAKE_VM) $@

clean:
	@$(MAKE_VM) $@

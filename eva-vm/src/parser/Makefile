
MAKE_VM=cd ../vm; $(MAKE) --no-print-directory
PARSER=$(shell commang -v syntax-cli 2> /dev/null)

.PHONY: all run clean generate

all: generate
	@$(MAKE_VM)

run:
	@$(MAKE_VM) $@

clean:
	@$(MAKE_VM) $@

generate:
ifdef PARSER
	$(shell syntax-cli -g eva_grammar.bnf -m LALR1 -o EvaParser.h ; mv "EvaParser.h" "eva_parser.h")
else
	@echo "\`syntax-cli not found. Please install it:\`"
	@echo "$ npm install -g syntax-cli"
endif

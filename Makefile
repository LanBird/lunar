SRC_DIR = source
TST_DIR = test
INC_DIR = include
OBJ_DIR = build/obj
BIN_DIR = build/bin

CC = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -llua -pthread
MKDIR = mkdir -p

TARGETS = freebsd linux macos solaris

.PHONY:	all build test install documentation $(TARGETS)

.DEFAULT:	all

all:	build test

clean:
	@rm $(OBJ_DIR)/*.o
	@rm $(BIN_DIR)/lunar $(BIN_DIR)/lunard $(BIN_DIR)/lunar_tests

directories:
	@$(MKDIR) $(OBJ_DIR)
	@$(MKDIR) $(BIN_DIR)

build: $(OSTYPE)
	@if [ "$(OSTYPE)" == "" ]; then echo "Can't determine operating system type." && \
	  echo && echo "Available target systems are:" && \
	  echo "  $(TARGETS)" && exit 1; fi

freebsd:
	@cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" \
	  LDFLAGS="$(LDFLAGS)" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"
	@cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)"\
	  LDFLAGS="$(LDFLAGS)" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"

linux:
	@cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" \
	  LDFLAGS="$(LDFLAGS) -lm -ldl" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"
	@cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)"\
	  LDFLAGS="$(LDFLAGS) -lm -ldl" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"

macos:
	@cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" \
	  LDFLAGS="$(LDFLAGS)" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"
	@cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)"\
	  LDFLAGS="$(LDFLAGS)" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"

solaris:
	@cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" \
	  LDFLAGS="$(LDFLAGS)" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"
	@cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)"\
	  LDFLAGS="$(LDFLAGS)" INC_DIR="$$(cd .. && cd $(INC_DIR) && pwd)" \
	  OBJ_DIR="$$(cd .. && cd $(OBJ_DIR) && pwd)" \
	  BIN_DIR="$$(cd .. && cd $(BIN_DIR) && pwd)"

install:	build test
	@echo "Installing Lunar ..."

documentation:
	@echo "Generating documentation ..."

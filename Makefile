SRC_DIR = source
TST_DIR = test
INC_DIR = include
OBJ_DIR = build/obj
TMP_DIR = build/tmp
BIN_DIR = build/bin

CC = gcc
CFLAGS += -I../$(INC_DIR) -O2 -Wall
LDFLAGS += -llua -pthread
MKDIR = mkdir -p

TARGETS = freebsd linux macos solaris
 
.PHONY:	all build test install documentation $(TARGETS)

.DEFAULT:	all

all:	build test

clean:
	@rm $(OBJ_DIR)/*.o

directories:
	@$(MKDIR) $(OBJ_DIR)
	@$(MKDIR) $(TMP_DIR)
	@$(MKDIR) $(BIN_DIR)

build: $(OSTYPE)
	@if [ "$(OSTYPE)" == "" ]; then echo "Can't determine operating system type." && \
	  echo && echo "Available target systems are:" && \
	  echo "  $(TARGETS)" && exit 1; fi

freebsd:
	cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"
	cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

linux:
	cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS) -lm -ldl"
	cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS) -lm -ldl"

macos:
	cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"
	cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

solaris:
	cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"
	cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

test:	directories build
	./$(BIN_DIR)/lunar_tests

install:	build test
	@echo "Installing Lunar ..."

documentation:
	@echo "Generating documentation ..."

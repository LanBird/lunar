SRC_DIR = source
TST_DIR = test
INC_DIR = include
OBJ_DIR = build/obj
TMP_DIR = build/tmp
BIN_DIR = build/bin

CC = gcc
CFLAGS = -I../$(INC_DIR) -O2 -Wall
LDFLAGS = -static -L../lua-5.1.4/src -llua -lpthread
MKDIR = mkdir -p
 
.PHONY:	all build test install documentation

all:	build test

clean:
	@rm $(OBJ_DIR)/*.o

directories:
	@$(MKDIR) $(OBJ_DIR)
	@$(MKDIR) $(TMP_DIR)
	@$(MKDIR) $(BIN_DIR)

build: directories
	cd $(SRC_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

test:	directories build
	cd $(TST_DIR) && $(MAKE) bin CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"
	./$(BIN_DIR)/lunar_tests

install:	build test
	@echo "Installing Lunar ..."

documentation:
	@echo "Generating documentation ..."

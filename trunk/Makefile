
CFLAGS = $(shell echo $$CFLAGS) -Iinclude
LDFLAGS = -llua -pthread

OBJ_DIR = $(shell echo $$OBJROOT)
BIN_DIR = $(shell echo $$SYMROOT)
DEP_DIR = $(shell echo $$TMPROOT)

ifeq ($(OBJ_DIR),)
  OBJ_DIR = build
endif
ifeq ($(BIN_DIR),)
  BIN_DIR = $(OBJ_DIR)
endif
ifeq ($(DEP_DIR),)
  DEP_DIR = $(OBJ_DIR)
endif

SRC_DIR = source
INC_DIR = include

OBJ = $(shell ls $(SRC_DIR)/*.c | sed "s'$(SRC_DIR)\(.*\)\.c$$'$(OBJ_DIR)\1.o'")
BIN = $(shell grep "int[ \t\n]\+main[ \t\n]*(.*)[ \t\n]*{" $(SRC_DIR)/*.c \
              | sed "s'^$(SRC_DIR)\(.*\)\.c:.*'$(BIN_DIR)\1'" )

include $(patsubst $(BIN_DIR)/%,$(DEP_DIR)/%.ld.d,$(BIN))

$(OBJ):	$(OBJ_DIR)/%.o:	$(DEP_DIR)/%.cc.d
	@$(MAKE) -f $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.cc.d,$@) $@

$(DEP_DIR)/%.cc.d:
	gcc -M -I$(INC_DIR) $(patsubst $(DEP_DIR)/%.cc.d,$(SRC_DIR)/%.c,$@) \
	| sed "s'^\(.*\.o\):'$(OBJ_DIR)/\1:'" > $@
	echo "	$(CC) -c $(CFLAGS) \$$< -o \$$@" >> $@

$(DEP_DIR)/%.ld.d:
	gcc -M -I$(INC_DIR) $(patsubst $(DEP_DIR)/%.ld.d,$(SRC_DIR)/%.c,$@) -o $@.tmp
	files="" ; \
	for file in $$(cat $@.tmp); do \
		if [ -f "$(SRC_DIR)/$$(basename $${file%.h}.c)" ]; then \
	    files="$${files}$(OBJ_DIR)/$$(basename $${file%.h}.o) " ; \
		fi ; \
	done ; \
	echo "$(patsubst $(DEP_DIR)/%.ld.d,$(BIN_DIR)/%,$@):	$${files}" > $@
	echo "	$(CC) $(LDFLAGS) \$$(filter %.o,\$$^) -o \$$@" >> $@
	rm $@.tmp

.PHONY:	all clean clean-dependencies dependencies build install test documentation $(BIN) $(OBJ)
.DEFAULT:	all

all:	build test

clean:	clean-dependencies
	rm -f $(BIN) $(OBJ)

clean-dependencies:
	rm -f $(DEP_DIR)/*.d

dependencies:	$(patsubst $(BIN_DIR)/%,$(DEP_DIR)/%.ld.d,$(BIN)) $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.cc.d,$(OBJ))

build:	$(BIN)

install:
	echo "Installing Lunar"

test:
	echo "Building and running tests ..."

documentation:
	echo "Building documentation"

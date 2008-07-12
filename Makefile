
CFLAGS = $(shell echo $$CFLAGS) -Iinclude
LDFLAGS = -llua -pthread

OBJ_DIR = $(shell echo $$OBJROOT)
BIN_DIR = $(shell echo $$SYMROOT)
DEP_DIR = $(shell echo $$OBJROOT)

ifeq ($(OBJ_DIR),)
  OBJ_DIR = build
endif
ifeq ($(BIN_DIR),)
  BIN_DIR = $(OBJ_DIR)
endif
ifeq ($(DEP_DIR),)
  DEP_DIR = $(OBJ_DIR)/dep
endif

SRC_DIR = source
INC_DIR = include

OBJ = $(shell ls $(SRC_DIR)/*.c | sed "s'$(SRC_DIR)\(.*\)\.c$$'$(OBJ_DIR)\1.o'")
BIN = $(shell grep "int[ \t\n]\+main[ \t\n]*(.*)[ \t\n]*{" $(SRC_DIR)/*.c \
              | sed "s'^$(SRC_DIR)\(.*\)\.c:.*'$(BIN_DIR)\1'" )

$(OBJ):	$(OBJ_DIR)/%.o:	$(DEP_DIR)/%.cc.d
	@$(MAKE) -f $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.cc.d,$@) $@

$(BIN):	$(BIN_DIR)/%:	$(DEP_DIR)/%.ld.d
	echo $(BIN)
	@$(MAKE) -f $(patsubst $(BIN_DIR)/%,$(DEP_DIR)/%.ld.d,$@) $@

$(DEP_DIR)/%.cc.d:
	gcc -M -I$(INC_DIR) $(patsubst $(DEP_DIR)/%.cc.d,$(SRC_DIR)/%.c,$@) \
	| sed "s'^\(.*\.o\):'$(OBJ_DIR)/\1:'" > $@
	echo "	$(CC) -c $(CFLAGS) \$$< -o \$$@" >> $@

$(DEP_DIR)/%.ld.d:
	gcc -M -I$(INC_DIR) $(patsubst $(DEP_DIR)/%.ld.d,$(SRC_DIR)/%.c,$@) -o $@.tmp
	echo "-include Makefile" > $@
	files="" ; \
	for file in $$(cat $@.tmp); do \
		if [ -f "$(SRC_DIR)/$$(basename $${file%.h}.c)" ]; then \
	    files="$${files}$(OBJ_DIR)/$$(basename $${file%.h}.o) " ; \
		fi ; \
	done ; \
	echo "$(patsubst $(DEP_DIR)/%.ld.d,$(BIN_DIR)/%,$@):	$${files}" >> $@
	echo "	$(CC) $(LDFLAGS) \$$(filter %.o,\$$^) -o \$$@" >> $@
	rm $@.tmp

.PHONY:	all $(BIN) $(OBJ)
.DEFAULT:	all

all:	$(BIN)
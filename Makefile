
CFLAGS = $(shell echo $$CFLAGS) -Iinclude
LDFLAGS = $(shell echo $$LDFLAGS) -pthread

OBJ_DIR = $(shell echo $$OBJROOT)
BIN_DIR = $(shell echo $$SYMROOT)
DEP_DIR = $(shell echo $$TMPROOT)

ifeq ($(OBJ_DIR),)
  OBJ_DIR = build/obj
endif
ifeq ($(BIN_DIR),)
  BIN_DIR = build/bin
endif
ifeq ($(DEP_DIR),)
  DEP_DIR = build/dep
endif

SRC_DIR = source
INC_DIR = include
TST_DIR = test

OBJ = $(shell ls $(SRC_DIR)/*.c | sed "s'$(SRC_DIR)\(.*\)\.c$$'$(OBJ_DIR)\1.o'")
BIN = $(shell grep "int[ \t\n]\+main[ \t\n]*(.*)[ \t\n]*{" $(SRC_DIR)/*.c \
              | sed "s'^$(SRC_DIR)\(.*\)\.c:.*'$(BIN_DIR)\1'" )
TST = $(shell grep "test_info[ \t\n]\+test_.\+{" $(TST_DIR)/*.c \
              | sed "s'$(TST_DIR)\(.*\)\.c:.*'$(OBJ_DIR)/$(TST_DIR)\1.o'")

.PHONY:	all clean clean-dependencies dependencies build install test documentation $(BIN) $(OBJ)
.DEFAULT:	all

all:	build test

clean:	clean-dependencies
	rm -f $(BIN) $(OBJ) $(TST)

clean-dependencies:
	rm -f $(DEP_DIR)/$(SRC_DIR)/*.d
	rm -f $(DEP_DIR)/$(TST_DIR)/*.d

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

dependencies:	$(DEP_DIR) $(patsubst $(BIN_DIR)/%,$(DEP_DIR)/%.ld.d,$(BIN)) $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.cc.d,$(OBJ))

build:	$(BIN_DIR) $(OBJ_DIR) $(BIN)

install:
	echo "Installing Lunar"

test:	$(BIN_DIR)/$(TST_DIR)/test
	./$(BIN_DIR)/$(TST_DIR)/test

documentation:
	echo "Building documentation"

include $(patsubst $(OBJ_DIR)/%.o,            $(DEP_DIR)/$(SRC_DIR)/%.cc.d, $(OBJ))
include $(patsubst $(OBJ_DIR)/$(TST_DIR)/%.o, $(DEP_DIR)/$(TST_DIR)/%.cc.d, $(TST))

include $(patsubst $(BIN_DIR)/%,              $(DEP_DIR)/%.ld.d,            $(BIN))

$(DEP_DIR)/%.cc.d:	%.c
	mkdir -p $$(dirname $@)
	if [ "$$(dirname $@)" == "$(DEP_DIR)/$(TST_DIR)" ]; then \
	  $(CC) -M $(CFLAGS) -I$(TST_DIR) $< \
	  | sed "s'^\(.*\.o\):'$(OBJ_DIR)/$(TST_DIR)/\1:'" > $@ ; \
	else \
	  $(CC) -M $(CFLAGS) $< \
	  | sed "s'^\(.*\.o\):'$(OBJ_DIR)/\1:'" > $@ ; \
	fi
	echo "	$(CC) -c $(CFLAGS) \$$< -o \$$@" >> $@

$(DEP_DIR)/%.ld.d:
	mkdir -p $$(dirname $@)
	$(CC) -M $(CFLAGS) $(patsubst $(DEP_DIR)/%.ld.d,$(SRC_DIR)/%.c,$@) -o $@.tmp
	files="" ; \
	for file in $$(cat $@.tmp); do \
	  if [ -f "$(SRC_DIR)/$$(basename $${file%.h}.c)" ]; then \
	    files="$${files}$(OBJ_DIR)/$$(basename $${file%.h}.o) " ; \
	  fi ; \
	done ; \
	echo "$(patsubst $(DEP_DIR)/%.ld.d,$(BIN_DIR)/%,$@):	$${files}" > $@
	echo "	$(CC) $(LDFLAGS) $$^ -o \$$@" >> $@
	rm $@.tmp

.INTERMEDIATE: $(TST_DIR)/tests.h

$(TST_DIR)/tests.h:
	files=1 ; \
	for file in $(TST); do \
	  files=$$((files+1)) ; \
	  echo "extern struct test_info test_$$(basename $${file%.o});" ; \
	done > $@ ; \
	echo "struct test_info * tests[$${files}] = {" >> $@
	for file in $(TST); do \
	  echo "  &test_$$(basename $${file%.o})," ; \
	done >> $@
	echo "  0" >> $@
	echo "};" >> $@

$(BIN_DIR)/$(TST_DIR)/test:	$(OBJ_DIR)/$(TST_DIR)/test.o $(TST_DIR)/tests.h $(TST) $(filter-out $(patsubst $(BIN_DIR)/%,$(OBJ_DIR)/%.o,$(BIN)),$(OBJ))
	$(CC) $(LDFLAGS) $(filter %.o,$^) -o $@

$(OBJ_DIR)/$(TST_DIR)/test.o:	$(TST_DIR)/tests.h $(TST_DIR)/test.c
	$(CC) -I$(TST_DIR) $(CFLAGS) $(TST_DIR)/test.c -c -o $@
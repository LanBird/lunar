SRC_DIR = .
INC_DIR = ../include
OBJ_DIR = ../build/obj
TMP_DIR = ../build/tmp
BIN_DIR = ../build/bin

VPATH = $(INC_DIR)

TEST_O = $(OBJ_DIR)/test.o \
         $(OBJ_DIR)/test_database.o \
         $(OBJ_DIR)/test_intmath.o \
         $(OBJ_DIR)/test_sha1.o

EXT_O  = $(OBJ_DIR)/database.o \
         $(OBJ_DIR)/intmath.o \
         $(OBJ_DIR)/sha1.o

ALL_O  = $(TEST_O)
       

BIN    = $(BIN_DIR)/lunar_tests

.PHONY:	obj bin

obj:	$(ALL_O)
bin:	$(BIN)

$(OBJ_DIR)/test.o:	test.c test.h tests.h
$(OBJ_DIR)/test_database.o:	test_database.c database.h
$(OBJ_DIR)/test_intmath.o:	test_intmath.c intmath.h
$(OBJ_DIR)/test_sha1.o:	test_sha1.c sha1.h

$(BIN_DIR)/lunar_tests:	$(OBJ_DIR)/test.o \
  $(OBJ_DIR)/test_database.o $(OBJ_DIR)/database.o \
  $(OBJ_DIR)/test_intmath.o  $(OBJ_DIR)/intmath.o \
  $(OBJ_DIR)/test_sha1.o     $(OBJ_DIR)/sha1.o
	$(CC) $(LDFLAGS) $(TEST_O) $(EXT_O) -o $@

$(ALL_O):
	$(CC) -c $(CFLAGS) $$(basename $@ | sed 's/.o/.c/') -o $@

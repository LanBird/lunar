# directory setup
SRC_DIR = .
INC_DIR = ../include
OBJ_DIR = ../build/obj
BIN_DIR = ../build/bin

# use include directory when looking for input .h files
VPATH = $(INC_DIR)

# objects for the test framework
TEST_O = $(OBJ_DIR)/test.o \
         $(OBJ_DIR)/test_intmath.o \
         $(OBJ_DIR)/test_sha1.o \
         $(OBJ_DIR)/test_storage.o

# external objects being tested
EXT_O  = $(OBJ_DIR)/intmath.o \
         $(OBJ_DIR)/rtctl.o \
         $(OBJ_DIR)/sha1.o \
         $(OBJ_DIR)/storage.o

# all objects that can be rebuild
ALL_O  = $(TEST_O)

# available executables
BIN = $(BIN_DIR)/lunar_tests

.PHONY:	obj bin

obj:	$(ALL_O)
bin:	$(BIN)

# dependencies for object compilation
$(OBJ_DIR)/test.o:	test.c test.h tests.h test.h
$(OBJ_DIR)/test_database.o:	test_database.c database.h test.h storage.h sha1.h intmath.h
$(OBJ_DIR)/test_intmath.o:	test_intmath.c intmath.h test.h
$(OBJ_DIR)/test_sha1.o:	test_sha1.c sha1.h test.h
$(OBJ_DIR)/test_storage.o:	test_storage.c database.h test.h

# object compilation rule
$(ALL_O):
	$(CC) -c $(CFLAGS) -I$(INC_DIR) $$(basename $@ | sed "s'\.o'.c';") -o $@

# executable compilation rule
$(BIN_DIR)/lunar_tests:	$(OBJ_DIR)/test.o \
  $(TEST_O) $(EXT_O)
	$(CC) $(LDFLAGS) $(TEST_O) $(EXT_O) -o $@


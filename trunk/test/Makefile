SRC_DIR = .
INC_DIR = ../include
OBJ_DIR = ../build/obj
TMP_DIR = ../build/tmp
BIN_DIR = ../build/bin

CC = gcc
CFLAGS = -I$(INC_DIR) -O2 -Wall
LDFLAGS = -lpthread
MKDIR = mkdir -p
 
VPATH = $(OBJ_DIR):$(INC_DIR):$(SRC_DIR)

OBJ = test.o \
      test_database.o \
      test_intmath.o \
      test_sha1.o

BIN = lunar_tests

.PHONY:	obj bin

obj:	$(OBJ)
bin:	$(BIN)

lunar_tests:	test.o \
  test_database.o database.o \
  test_intmath.o  intmath.o \
  test_sha1.o     sha1.o
	@$(MKDIR) $(BIN_DIR)
	$(CC) $(LDFLAGS) $? -o $(BIN_DIR)/$@


test.o:	test.c test.h tests.h
test_database.o:	test_database.c database.h
test_intmath.o:	test_intmath.c intmath.h
test_sha1.o:	test_sha1.c sha1.h

.c.o:
	@$(MKDIR) $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@

SRC_DIR = .
INC_DIR = ../include
OBJ_DIR = ../build/obj
TMP_DIR = ../build/tmp
BIN_DIR = ../build/bin

CC = gcc
CFLAGS = -I$(INC_DIR) -O2 -Wall
LDFLAGS = -lpthread
MKDIR = mkdir -p
 
VPATH = $(INC_DIR)

OBJ = $(OBJ_DIR)/test.o \
      $(OBJ_DIR)/test_database.o \
      $(OBJ_DIR)/test_intmath.o \
      $(OBJ_DIR)/test_sha1.o

BIN = $(BIN_DIR)/lunar_tests

.PHONY:	obj bin

obj:	$(OBJ)
bin:	$(BIN)

$(OBJ_DIR)/test.o:	test.c test.h tests.h
$(OBJ_DIR)/test_database.o:	test_database.c database.h
$(OBJ_DIR)/test_intmath.o:	test_intmath.c intmath.h
$(OBJ_DIR)/test_sha1.o:	test_sha1.c sha1.h

$(BIN_DIR)/lunar_tests:	$(OBJ_DIR)/test.o \
  $(OBJ_DIR)/test_database.o $(OBJ_DIR)/database.o \
  $(OBJ_DIR)/test_intmath.o  $(OBJ_DIR)/intmath.o \
  $(OBJ_DIR)/test_sha1.o     $(OBJ_DIR)/sha1.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ):
	$(CC) -c $(CFLAGS) $< -o $@

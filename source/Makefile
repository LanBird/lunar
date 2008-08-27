SRC_DIR = .
INC_DIR = ../include
OBJ_DIR = ../build/obj
TMP_DIR = ../build/tmp
BIN_DIR = ../build/bin

CC = gcc
CFLAGS = -I$(INC_DIR) -O2 -Wall
LDFLAGS = -lpthread
 
VPATH = $(INC_DIR)

OBJ = $(OBJ_DIR)/client.o \
      $(OBJ_DIR)/database.o \
      $(OBJ_DIR)/deflate.o \
      $(OBJ_DIR)/file.o \
      $(OBJ_DIR)/gz.o \
      $(OBJ_DIR)/intmath.o \
      $(OBJ_DIR)/lua_binding.o \
      $(OBJ_DIR)/lunar.o \
      $(OBJ_DIR)/lunard.o \
      $(OBJ_DIR)/server.o \
      $(OBJ_DIR)/sha1.o \
      $(OBJ_DIR)/target.o

BIN = $(BIN_DIR)/lunar \
      $(BIN_DIR)/lunard

.PHONY:	obj bin

obj:	$(OBJ)
bin:	$(BIN)

$(BIN_DIR)/lunar:	$(OBJ_DIR)/lunar.o \
  $(OBJ_DIR)/client.o \
  $(OBJ_DIR)/database.o \
  $(OBJ_DIR)/deflate.o \
  $(OBJ_DIR)/file.o \
  $(OBJ_DIR)/gz.o \
  $(OBJ_DIR)/intmath.o \
  $(OBJ_DIR)/server.o \
  $(OBJ_DIR)/sha1.o \
  $(OBJ_DIR)/target.o
$(BIN_DIR)/lunard:	$(OBJ_DIR)/lunard.o \
  $(OBJ_DIR)/database.o \
  $(OBJ_DIR)/deflate.o \
  $(OBJ_DIR)/file.o \
  $(OBJ_DIR)/gz.o \
  $(OBJ_DIR)/intmath.o \
  $(OBJ_DIR)/server.o \
  $(OBJ_DIR)/sha1.o \
  $(OBJ_DIR)/target.o

$(OBJ_DIR)/client.o:	client.c client.h
$(OBJ_DIR)/database.o:	database.c database.h
$(OBJ_DIR)/deflate.o:	deflate.c deflate.h
$(OBJ_DIR)/file.o:	file.c file.h
$(OBJ_DIR)/gz.o:	gz.c gz.h
$(OBJ_DIR)/intmath.o:	intmath.c intmath.h
$(OBJ_DIR)/lua_binding.o:	lua_binding.c lua_binding.h
$(OBJ_DIR)/lunar.o:	lunar.c lunar.h
$(OBJ_DIR)/lunard.o:	lunard.c lunard.h
$(OBJ_DIR)/server.o:	server.c server.h
$(OBJ_DIR)/sha1.o:	sha1.c sha1.h
$(OBJ_DIR)/target.o:	target.c target.h

$(OBJ):
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	$(CC) $(LDFLAGS) $^ -o $@

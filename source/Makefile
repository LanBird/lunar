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

OBJ = client.o \
      database.o \
      deflate.o \
      file.o \
      gz.o \
      intmath.o \
      lua_binding.o \
      lunar.o \
      lunard.o \
      server.o \
      sha1.o \
      target.o

BIN = lunar \
      lunard

.PHONY:	obj bin

obj:	$(OBJ)
bin:	$(BIN)

lunar:	lunar.o client.o database.o deflate.o file.o gz.o intmath.o server.o sha1.o target.o
lunard:	lunard.o database.o deflate.o file.o gz.o intmath.o server.o sha1.o target.o

client.o:	client.c client.h
database.o:	database.c database.h
deflate.o:	deflate.c deflate.h
file.o:	file.c file.h
gz.o:	gz.c gz.h
intmath.o:	intmath.c intmath.h
lua_binding.o:	lua_binding.c lua_binding.h
lunar.o:	lunar.c lunar.h
lunard.o:	lunard.c lunard.h
server.o:	server.c server.h
sha1.o:	sha1.c sha1.h
target.o:	target.c target.h

.c.o:
	@$(MKDIR) $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@

$(BIN):
	@$(MKDIR) $(BIN_DIR)
	$(CC) $(LDFLAGS) $? -o $(BIN_DIR)/$@

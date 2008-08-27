SRC_DIR = .
INC_DIR = ../include
OBJ_DIR = ../build/obj
TMP_DIR = ../build/tmp
BIN_DIR = ../build/bin

VPATH = $(INC_DIR)

CORE_O =   $(OBJ_DIR)/database.o \
           $(OBJ_DIR)/deflate.o \
           $(OBJ_DIR)/file.o \
           $(OBJ_DIR)/gz.o \
           $(OBJ_DIR)/intmath.o \
           $(OBJ_DIR)/language_binding.o \
           $(OBJ_DIR)/lua_binding.o \
           $(OBJ_DIR)/sha1.o \
           $(OBJ_DIR)/target.o

CLIENT_O = $(OBJ_DIR)/client.o

SERVER_O = $(OBJ_DIR)/server.o

LUNAR_O =  $(OBJ_DIR)/lunar.o \
           $(CLIENT_O) \
           $(CORE_O)

LUNARD_O = $(OBJ_DIR)/lunard.o \
           $(SERVER_O) \
           $(CORE_O)

ALL_O    = $(CORE_O) $(CLIENT_O) $(SERVER_O) \
           $(OBJ_DIR)/lunar.o $(OBJ_DIR)/lunard.o

BIN = $(BIN_DIR)/lunar \
      $(BIN_DIR)/lunard

.PHONY:	obj bin

obj:	$(ALL_O)
bin:	$(BIN)

$(BIN_DIR)/lunar:	$(LUNAR_O)
	$(CC) $(LDFLAGS) $(LUNAR_O) -o $@
$(BIN_DIR)/lunard:	$(LUNARD_O)
	$(CC) $(LDFLAGS) $(LUNARD_O) -o $@

$(OBJ_DIR)/client.o:	client.c client.h
$(OBJ_DIR)/database.o:	database.c database.h
$(OBJ_DIR)/deflate.o:	deflate.c deflate.h
$(OBJ_DIR)/file.o:	file.c file.h
$(OBJ_DIR)/gz.o:	gz.c gz.h
$(OBJ_DIR)/intmath.o:	intmath.c intmath.h
$(OBJ_DIR)/language_binding.o:	language_binding.c language_binding.h
$(OBJ_DIR)/lua_binding.o:	lua_binding.c lua_binding.h
$(OBJ_DIR)/lunar.o:	lunar.c lunar.h
$(OBJ_DIR)/lunard.o:	lunard.c lunard.h
$(OBJ_DIR)/server.o:	server.c server.h
$(OBJ_DIR)/sha1.o:	sha1.c sha1.h
$(OBJ_DIR)/target.o:	target.c target.h

$(ALL_O):
	$(CC) -c $(CFLAGS) $$(basename $@ | sed 's/.o/.c/') -o $@

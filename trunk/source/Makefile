# directory setup
SRC_DIR = .
INC_DIR = ../include
OBJ_DIR = ../build/obj
BIN_DIR = ../build/bin

# use include directory when looking for input .h files
VPATH = $(INC_DIR)

# objects present in all lunar binaries
CORE_O =   $(OBJ_DIR)/deflate.o \
           $(OBJ_DIR)/file.o \
           $(OBJ_DIR)/gz.o \
           $(OBJ_DIR)/intmath.o \
           $(OBJ_DIR)/language_binding.o \
           $(OBJ_DIR)/lua_binding.o \
           $(OBJ_DIR)/rtctl.o \
           $(OBJ_DIR)/sha1.o \
           $(OBJ_DIR)/storage.o \
           $(OBJ_DIR)/target.o

# objects currently in development
DEV_O = $(OBJ_DIR)/pattern_trie.o \
        $(OBJ_DIR)/hash_map.o

CLIENT_O = $(OBJ_DIR)/client.o

SERVER_O = $(OBJ_DIR)/server.o

# objects for executables
LUNAR_O =  $(OBJ_DIR)/lunar.o \
           $(CLIENT_O) \
           $(CORE_O)

LUNARD_O = $(OBJ_DIR)/lunard.o \
           $(SERVER_O) \
           $(CORE_O)

# all objects that can be rebuilt
ALL_O    = $(CORE_O) $(CLIENT_O) $(SERVER_O) \
           $(OBJ_DIR)/lunar.o $(OBJ_DIR)/lunard.o

# available executables
BIN = $(BIN_DIR)/lunar \
      $(BIN_DIR)/lunard

.PHONY:	obj bin

obj:	$(ALL_O)
bin:	$(BIN)

# dependencies for object compilation
$(OBJ_DIR)/client.o:	client.c client.h
$(OBJ_DIR)/database.o:	database.c database.h intmath.h sha1.h storage.h
$(OBJ_DIR)/deflate.o:	deflate.c deflate.h
$(OBJ_DIR)/file.o:	file.c file.h
$(OBJ_DIR)/gz.o:	gz.c gz.h
$(OBJ_DIR)/hash_map.o:	hash_map.c hash_map.h storage.h sha1.h rtctl.h
$(OBJ_DIR)/intmath.o:	intmath.c intmath.h
$(OBJ_DIR)/language_binding.o:	language_binding.c language_binding.h
$(OBJ_DIR)/lua_binding.o:	lua_binding.c lua_binding.h
$(OBJ_DIR)/lunar.o:	lunar.c lunar.h
$(OBJ_DIR)/lunard.o:	lunard.c lunard.h
$(OBJ_DIR)/pattern_trie.o:	pattern_trie.c pattern_trie.h storage.h rtctl.h
$(OBJ_DIR)/rtctl.o:	rtctl.c rtctl.h storage.h
$(OBJ_DIR)/server.o:	server.c server.h
$(OBJ_DIR)/sha1.o:	sha1.c sha1.h
$(OBJ_DIR)/storage.o:	storage.c storage.h rtctl.h
$(OBJ_DIR)/target.o:	target.c target.h

# object compilation rule
$(ALL_O):
	$(CC) -c $(CFLAGS) -I$(INC_DIR) $$(basename $@ | sed "s'\.o'.c';") -o $@

# executable compilation rules
# lunar
$(BIN_DIR)/lunar:	$(LUNAR_O)
	$(CC) $(LDFLAGS) $(LUNAR_O) -o $@
# lunard
$(BIN_DIR)/lunard:	$(LUNARD_O)
	$(CC) $(LDFLAGS) $(LUNARD_O) -o $@


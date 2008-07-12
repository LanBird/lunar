
CFLAGS = $(shell echo $$CFLAGS)
LDFLAGS = -llua -pthread

OBJECTS_DIR = $(shell echo $$OBJROOT)
BINARIES_DIR = $(shell echo $$SYMROOT)

ifeq ($(OBJECTS_DIR),)
  OBJECTS_DIR = build
endif
ifeq ($(BINARIES_DIR),)
  BINARIES_DIR = $(OBJECTS_DIR)
endif

SOURCE_DIR = ./source
INCLUDE_DIR = ./include

OBJECTS = sha1.o lunar.o lunard.o file.o database.o intmath.o
BINARIES = lunar lunard

all:	$(addprefix $(BINARIES_DIR)/,$(BINARIES))

clean:
	cd $(OBJECTS_DIR) && rm -f $(OBJECTS)
	cd $(BINARIES_DIR) && rm -f $(BINARIES)

install:
	echo install

test:
	echo test

documentation:
	echo documentation

.PHONY:	all clean install test documentation $(BINARIES)

$(BINARIES_DIR)/lunar:	$(OBJECTS_DIR)/lunar.o $(OBJECTS_DIR)/sha1.o $(OBJECTS_DIR)/database.o $(OBJECTS_DIR)/intmath.o
	$(CC) $(LDFLAGS) $^ -o $@
	
$(BINARIES_DIR)/lunard:	$(OBJECTS_DIR)/lunard.o
	$(CC) $(LDFLAGS) $^ -o $@
	
$(addprefix $(OBJECTS_DIR)/,$(OBJECTS)):	$(OBJECTS_DIR)/%.o:	$(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c -Iinclude $< -o $@
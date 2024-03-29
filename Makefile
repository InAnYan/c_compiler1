EXEC_NAME=ccomp

CC=clang++
LD=clang++

BIN_DIR=bin
INC_DIR=include
SRC_DIR=src
OBJ_DIR=obj
SUBDIRS=. File AST Token Visitors

INCLUDES_DIRS=
LIBS_DIRS=/usr/local/lib

LIBS=
DEFINES= 

CFLAGS=-g -Wall -std=c++17
LDFLAGS=

DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJ_DIR)/$*.d

FULL_EXEC=$(BIN_DIR)/$(EXEC_NAME)
FULL_CFLAGS=$(CFLAGS) -I$(INC_DIR) $(addprefix -I, $(INCLUDES_DIRS)) $(addprefix -D, $(DEFINES)) $(DEPFLAGS)
FULL_LDFLAGS=$(LDFLAGS) $(addprefix -L, $(LIBS_DIRS)) $(addprefix -l, $(LIBS))

SRC_SUBDIRS=$(addprefix $(SRC_DIR)/, $(SUBDIRS))
INC_SUBDIRS=$(addprefix $(INC_DIR)/, $(SUBDIRS))

INCS=$(wildcard *.hpp $(foreach fd, $(INC_SUBDIRS), $(fd)/*.hpp))
SRCS=$(wildcard *.cpp $(foreach fd, $(SRC_SUBDIRS), $(fd)/*.cpp))

OBJS=$(subst $(SRC_DIR), $(OBJ_DIR), $(SRCS:.cpp=.o))
DEPFILES := $(OBJS:.o=.d)

all: $(FULL_EXEC)

dirs:
	mkdir obj bin obj/File obj/AST obj/Token obj/Visitors

$(FULL_EXEC): $(OBJS)
	$(CC) $(OBJS) $(FULL_LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(FULL_CFLAGS) -c $< -o $@

clean:
	rm $(FULL_EXEC) $(OBJS)

run:
	./bin/$(PROJECT_NAME).out

$(DEPFILES):

include $(wildcard $(DEPFILES))

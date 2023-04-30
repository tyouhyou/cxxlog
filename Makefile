CC		:= g++
CFLAGS	:= -std=c++17 -Wall -Wextra -Wno-unknown-pragmas

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
SOURCEDIRS	:= $(SRC)
OBJDIR   	:= $(BIN)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
else
EXECUTABLE	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
OBJDIR   	:= $(shell find $(BIN) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
endif

CINCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
CLIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))
DEBUG		:= -g -D_DEBUG
SOURCES		:= $(wildcard $(SOURCEDIRS)/*.cpp)
OBJECTS		:= $(SOURCES:.cpp=.o )
EXFLAGS		:= -D_NOT_CLOSE_LOG -D_LOG_LOCK

%.o: %.cpp
	$(CC) $(CFLAGS) $(EXFLAGS) $(DEBUG) $(CINCLUDES) -c $< -o $@

all: $(BIN)/$(EXECUTABLE)
	-$(RM) $(OBJECTS)

.PHONY: clean
clean:
	-$(RM) $(BIN)/$(EXECUTABLE)
	-$(RM) $(OBJECTS)

.PHONY: clear
clear:
	-$(RM) $(OBJECTS)

run:
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(EXFLAGS) $(DEBUG) $(CINCLUDES) $(CLIBS) $^ -o $@ $(LIBRARIES)

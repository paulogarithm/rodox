BIN = lib/adder.so

CC = gcc
CXX = g++

CFLAGS += -Wall -Wextra -Wpedantic
CPPFLAGS += -iquote./src
LDFLAGS += -llua5.4
DEBUG ?= non

SRC = src/adder.c

OBJS := $(SRC:.c=.o)
OBJS := $(OBJS:.cpp=.o)

ifeq ($(DEBUG),oui)
CFLAGS+=-g3
endif

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ -fpic -std=c99

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ -fpic -std=c++17

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) $(LDFLAGS) -fpic -shared -o $(BIN)

clean:
	$(RM) $(OBJS)
	echo $(OBJS)


fclean: clean
	$(RM) $(BIN)

re: fclean all

.PHONY: re fclean clean all

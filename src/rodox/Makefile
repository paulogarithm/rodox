#|
#| Rodox common library Makefile
#|

CC ?= gcc
AR = ar
NAME = rodox
RODOXLIB = ../../lib
BIN = $(RODOXLIB)/lib$(NAME).a

CFLAGS += -Wall -Wextra -Wpedantic -std=c99
CPPFLAGS += -iquote.
LDLIBS += -lm

SRC = chain.c hashtables.c
OBJS = $(SRC:.c=.o)

GREEN = \33[32m
RESET = \33[m

all: $(BIN)

$(BIN): $(OBJS)
	$(AR) rc $(BIN) $(OBJS) $(LDLIBS)
	printf "librodox.a: $(GREEN)Build with success$(RESET)\n"

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BIN)

re: fclean all

.PHONY: re fclean clean all

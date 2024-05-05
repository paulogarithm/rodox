#|
#| Rodox Module Makefile
#|

CC ?= gcc
RODOX = ../..
RODOXLIB = $(RODOX)/lib
BIN = $(RODOXLIB)/$(MOD).so

CFLAGS += -Wall -Wextra -Wpedantic -std=c99
CPPFLAGS += -iquote./$(MOD) -iquote$(RODOX)/src/rodox
LDFLAGS += -llua5.4 -lm -L./$(RODOXLIB) -lrodox

SRC = $(wildcard $(MOD)/*.c)
OBJS = $(SRC:.c=.o)
GREEN = \33[32m
RESET = \33[m

all: $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) -shared -fpic $(OBJS) -o $(BIN) $(LDFLAGS)
	printf "$(MOD): $(GREEN)Build with success$(RESET)\n"

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BIN)

re: fclean all

.PHONY: re fclean clean all

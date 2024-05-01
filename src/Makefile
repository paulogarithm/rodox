MODULES = ./modules
DIRECTORY = $(sort $(dir $(wildcard $(MODULES)/*/)))
BUILD = build.sh
LIB = ../lib

all:
	for n in $(DIRECTORY); do \
		$(MODULES)/$(BUILD) $(basename $$n); \
	done;

fclean:
	$(RM) $(wildcard $(LIB)/*.so) $(wildcard $(LIB)/*.dll)

re: fclean all

.PHONY: all fclean
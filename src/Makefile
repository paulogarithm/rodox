BUILD = ./modules/build.sh
MOD = events adder
COMMON = ./rodox

define do
	$(MAKE) -C $(COMMON) $(1)
	$(BUILD) $(1) $(MOD)
endef

all:
	$(call do,$@)
clean:
	$(call do,$@)
fclean:
	$(call do,$@)
re:
	$(call do,$@)

.PHONY: all clean fclean re

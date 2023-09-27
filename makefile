####################
# PREREQUISITES
####################

# THE FOLLOWINGS MUST BE DEFINED BEFORE:
# > TB-DIR   = ./toolbox
# > DEBUG    = 3
# > COMPILER = gcc
# > CFLAGS   = -I$(D-TB-INC)


####################
# DEFINITIONS
####################

# COMPILATION PATHS
D-TB-OBJ = $(TB-DIR)/.obj
D-TB-INC = $(TB-DIR)/inc
D-TB-SRC = $(TB-DIR)/src


####################
# POPULATE FOLDERS
####################

TB-DEPS = $(wildcard $(D-TB-INC)/*.h)
TB-SRC = $(wildcard $(D-TB-SRC)/*.c)
TB-OBJ = $(patsubst $(D-TB-SRC)/%.c,$(D-TB-OBJ)/%.o,$(TB-SRC))


####################
# COMPILATION RULES
####################

$(D-TB-OBJ)/%.o: $(D-TB-SRC)/%.c $(TB-DEPS)
	@echo "Compiling: $< into $@"
	mkdir -p $(D-TB-OBJ)
	$(COMPILER) -g$(DEBUG) -c -o $@ $< $(CFLAGS)


####################
# HELP
####################

.PHONY: help_tb
help:
	@echo ""
	@echo "Next commands works with current makefile."
	@echo ""
	@echo "If it's needed to contact the maintainer, please send an email to Joseba R.G."
	@echo ""
	@echo "Commands for cleaning toolbox:"
	@echo "    make clean_tb : deletes compilation results and temporary files from toolbox library."
	@echo ""


####################
# RULES FOR CLEANING
####################

# CLEAN TOOLBOX
.PHONY: clean_tb
clean_cm:
	@echo "DELETING TOOLBOX FILES"
	rm -f -r $(D-TB-OBJ)

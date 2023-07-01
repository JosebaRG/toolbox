####################
# DEFINITIONS
####################

# TARGET BINARY NAMES
TDIR = ./deploy
TARGET = exe

# DEBUG CONFIGURATION
# 0: NO DEBUG
# 1: MINIMAL
# 2: DEFAULT (IF EMPTY)
# 3: MAXIMAL
DEBUG = 3

# MAIN CONFIGURATION
COMPILER = gcc
CFLAGS = -I$(D-INC)

# EXTERNAL LIBRARIES IF NEEDED
LIBS = # -lm
LDIR = ./lib

# COMPILATION PATHS
D-OBJ = ./.obj
D-INC = ./inc
D-SRC = ./src


####################
# POPULATE FOLDERS
####################

DEPS = $(wildcard $(D-INC)/*.h)
SRC = $(wildcard $(D-SRC)/*.c)
OBJ = $(patsubst $(D-SRC)/%.c,$(D-OBJ)/%.o,$(SRC))


############################################################
#                      MAKEFILE START                      #
############################################################

# DEFAULT RULE FOR MAKE
$(TDIR)/$(TARGET): $(OBJ)
	@echo "Target compilation"
	mkdir -p $(TDIR)
	$(COMPILER) -g$(DEBUG) -o $@ $^ $(DEPS) $(CFLAGS) $(LIBS)

# EXECUTE COMMAND FOR TESTING
.PHONY: call
call:
	echo "TESTING"
	$(TDIR)/$(TARGET)


####################
# COMPILATION RULES
####################

$(D-OBJ)/%.o: $(D-SRC)/%.c $(DEPS)
	@echo "Compiling: $< into $@"
	mkdir -p $(D-OBJ)
	$(COMPILER) -g$(DEBUG) -c -o $@ $< $(CFLAGS)


####################
# HELP
####################

.PHONY: help
help:
	@echo ""
	@echo "Next commands works with current makefile."
	@echo ""
	@echo "If it's needed to contact the maintainer, please send an email to Joseba R.G."
	@echo ""
	@echo "Commands for compilation:"
	@echo "    make			: compiles everything and leaves the bynary files in ./deploy."
	@echo ""
	@echo "Commands for cleaning:"
	@echo "    make clean	: deletes compilation results and temporary files."
	@echo ""


####################
# RULES FOR CLEANING
####################

# CLEAN COMMAND
.PHONY: clean
clean:
	@echo "DELETING FILES"
	rm -f -r $(D-OBJ)
	rm -f $(TDIR)/$(TARGET)
	rm -d $(TDIR) # DELETE ONLY IF EMPTY FOLDER

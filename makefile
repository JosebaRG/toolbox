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
CFLAGS = -I$(D-STR) -I$(D-XML)

# EXTERNAL LIBRARIES IF NEEDED
LIBS = # -lm
LDIR = ./lib

# COMPILATION PATHS
# TARGET, OBJECT, SOURCE, FUNCTIONS, PLATFORM
D-OBJ = ./obj
D-STR = ./libstring
D-XML = ./libjxml

####################
# ELEMENTS
####################

# LIST OF HEADER FILES
DEPS = \
	$(D-STR)/libstring.h \
	\
	$(D-XML)/libjxml.h



# LIST OF SOURCE FILES
SRC  = \
	libstring.c \
	\
	libjxml.c

# LIST OF OBJECT FILES
OBJ = $(patsubst %.c,$(D-OBJ)/%.o,$(SRC))

############################################################
#                      MAKEFILE START                      #
############################################################

# DEFAULT RULE FOR MAKE
$(TDIR)/$(TARGET): $(OBJ)
	echo "Compilation"
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

# LIBSTRING
$(D-OBJ)/libstring.o: $(D-STR)/libstring.c $(DEPS)
	mkdir -p $(D-OBJ)
	$(COMPILER) -g$(DEBUG) -c -o $@ $< $(CFLAGS)

# LIBXML
$(D-OBJ)/libjxml.o: $(D-XML)/libjxml.c $(DEPS)
	mkdir -p $(D-OBJ)
	$(COMPILER) -g$(DEBUG) -c -o $@ $< $(CFLAGS)

####################
# HELP
####################

.PHONY: help
help:
	@echo ""
	@echo "Next commands works with curren makefile."
	@echo ""
	@echo "If it's needed to contact the maintainer, please send an email to Joseba R.G."
	@echo ""
	@echo "Commands for compilation:"
	@echo "    make               : compiles everything and leaves the bynary files in ./deploy."
	@echo ""
	@echo "Commands for cleaning:"
	@echo "    make clean         : deletes compilation results and temporary files."
	@echo ""

####################
# RULES FOR CLEANING
####################

# CLEAN COMMAND
.PHONY: clean
clean:
	echo "DELETING FILES"
	rm -f -r $(D-OBJ)
	rm -f $(TDIR)/$(TARGET)
	rm -d $(TDIR) # DELETE ONLY IF EMPTY FOLDER

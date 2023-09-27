####################
# DEFINITIONS
####################

# MAIN CONFIGURATION
COMPILER = gcc
CFLAGS   = -I$(D-INC) -I$(D-TB-INC)

# DEBUG CONFIGURATION
# 0: NO DEBUG
# 1: MINIMAL
# 2: DEFAULT (IF EMPTY)
# 3: MAXIMAL
DEBUG = 3

# TARGET BINARY NAMES
TDIR   = ./deploy
TARGET = exe

# MAIN FILE
SRC-MAIN = main.c
DIR-MAIN = .

# EXTERNAL LIBRARIES IF NEEDED
LIBS = # -lm
LDIR = ./lib

# COMPILATION PATHS
D-OBJ = ./.obj
D-INC = ./inc
D-SRC = ./src


####################
# GIT REPOSITORIES
####################

# GIT DIRECTIONS
TB-GIT = git@github.com:JosebaRG/toolbox.git
TB-DIR = ./toolbox

# BRANCHES
TB-DEV-BRANCH   = develop
TB-STBL-BRANCH  = master
TB-STBL-COMMIT  = 4930f29


####################
# INCLUDES
####################

# INCLUDE MAKES FROM REPOSITORIES
-include $(TB-DIR)/makefile


####################
# POPULATE FOLDERS
####################

DEPS = $(wildcard $(D-INC)/*.h)
SRC = $(wildcard $(D-SRC)/*.c) $(DIR-MAIN)/$(SRC-MAIN)
OBJ = $(patsubst $(D-SRC)/%.c,$(D-OBJ)/%.o,$(SRC)) $(patsubst $(DIR-MAIN)/%.c,$(D-OBJ)/%.o,$(SRC-MAIN))


############################################################
#                      MAKEFILE START                      #
############################################################

# DEFAULT RULE FOR MAKE
$(TDIR)/$(TARGET): $(OBJ) $(D-TB-OBJ)
	@echo "Target compilation"
	mkdir -p $(TDIR)
	$(COMPILER) -g$(DEBUG) -o $@ $^ $(DEPS) $(CFLAGS) $(LIBS)

# EXECUTE COMMAND FOR TESTING
.PHONY: call
call:
	@echo "TESTING"
	$(TDIR)/$(TARGET)


####################
# COMPILATION RULES
####################

.PHONY: all
all:
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
	@echo "    make : compiles everything and leaves the bynary files in ./deploy."
	@echo ""
	@echo "Commands for cleaning:"
	@echo "    make clean    : deletes compilation results and temporary files."
	@echo "    make clean_tb : deletes compilation results and temporary files trom toolbox."
	@echo ""
	@echo "Commands for downloading modules and working with repositories:"
	@echo "    make download    : downloads the repositories on stable commit from master branches."
	@echo "    make download_tb : downloads the TOOLBOX repository on stable commit from master branch."
	@echo "    make work        : downloads the repositories on working copy from develpoment branch."
	@echo "    make work_tb     : downloads the TOOLBOX repository on working copy from develpoment branch."
	@echo "    make stable      : updates the repositories on stable commit from master branch."
	@echo "    make stable_tb   : updates the TOOLBOX repository on stable commit from master branch."
	@echo "    make develop     : updates the repositories from develpoment branch."
	@echo "    make develop_tb  : updates the TOOLBOX repository from develpoment branch."
	@echo ""

.PHONY: help_tb


####################
# RULES FOR CLEANING
####################

# CLEAN COMMAND
.PHONY: clean
clean: clean_tb
	@echo "DELETING MAIN FILES"
	rm -f -r $(D-OBJ)
	rm -f $(TDIR)/$(TARGET)
	rm -d $(TDIR) # DELETE ONLY IF EMPTY FOLDER

# CLEAN TOOLBOX
.PHONY: clean_tb

####################
# REPOSITORY RULES
####################

# DOWNLOAD REPOSITORIES ON STABLE MODE
.PHONY: download
download: download_tb

.PHONY: download_tb
download_tb:
	git clone -b $(TB-STBL-BRANCH) $(TB-GIT) $(TB-DIR)
	cd $(TB-DIR); git checkout $(TB-STBL-COMMIT)

# DOWNLOAD REPOSITORIES ON WORKING MODE
.PHONY: work
work: work_tb

.PHONY: work_tb
work_tb: 
	git clone -b $(TB-DEV-BRANCH) $(TB-GIT) $(TB-DIR)

# CHECKOUT REPOSITORY TO STABLE VERSION
.PHONY: stable
stable: stable_tb

.PHONY: stable_tb
stable_tb:
	cd $(TB-DIR); git checkout $(TB-STBL-BRANCH); git checkout $(TB-STBL-COMMIT)

# CHECKOUT REPOSITORY TO WORKING VERSION
.PHONY: develop
develop: develop_tb

.PHONY: develop_tb
develop:
	cd $(TB-DIR); git checkout $(TB-DEV-BRANCH) #; git checkout -

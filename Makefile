# * Author: Remco de Boer <remco.de.boer@ihep.ac.cn>
# * Date: January 11th, 2019
# * Based on the NIKHEFproject2018 repository


# * PATH DEFINITIONS * #
BINDIR = bin
EXEDIR = exe
INCLUDE_PATH = -Iinc
LIBNAME = BossAfterburner


# * COMPILER FLAGS * #
COMPILER = clang++ #g++
ROOTINC    := -I$(shell root-config --incdir)
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs --evelibs --glibs)
CFLAGS = -fPIC -w -g -W ${ROOTCFLAGS}
LFLAGS = ${ROOTLIBS} -g -lGenVector  -lRooFit -lRooFitCore -lRooStats -lMinuit


# * INVENTORIES OF OBJECTS AND THEIR EVENTUAL BINARIES * #
# * for the objects (inc and src)
OBJECTS  = $(notdir $(wildcard src/*.cxx))
OBJ_BIN  = $(OBJECTS:.cxx=.o)
OBJ_BIN := $(addprefix ${BINDIR}/, ${OBJ_BIN})

# * for the scripts (executables)
SCRIPTS  = $(notdir $(wildcard scripts/*.C))
SCR_BIN  = $(SCRIPTS:.C=.exe)
SCR_BIN := $(addprefix ${EXEDIR}/, ${SCR_BIN})

# * Compile objects: the parametesr are the rules defines below and above
all : ${OBJ_BIN} LINK ${SCR_BIN}
	@echo "COMPILING DONE"


# * COMPILE RULES * #
# * for the objects (inc and src)
${BINDIR}/%.o : src/%.cxx inc/%.h
	@echo "Compiling object \"$(notdir $<)\""
	@$(COMPILER) $(CFLAGS) ${INCLUDE_PATH} -c $< -o $@

# * for linking the objects generated above.
LINK :
	@ar q lib${LIBNAME}.a ${OBJ_BIN}

# * for the scripts (executables)
${EXEDIR}/%.exe : scripts/%.C
	@echo "Compiling script \"$(notdir $<)\""
	@$(COMPILER) $< -o $@ ${CFLAGS} ${INCLUDE_PATH} -L. -l${LIBNAME} ${LFLAGS}
	#$(COMPILER) $< -o $@ ${ROOTINC} ${INCLUDE_PATH} -L. -l${LIBNAME} ${LFLAGS}

# * REMOVE ALL BINARIES * #
# * This rule can be called using "make clean"
clean:
	@rm -f lib${LIBNAME}.a
	@echo "Deleted libary \"lib${LIBNAME}.a\""
	@rm -f ${BINDIR}/*
	@echo "Deleted all binary files in \"${BINDIR}\""
	@rm -f ${EXEDIR}/*
	@echo "Deleted all binary files in \"${EXEDIR}\""
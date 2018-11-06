# ! Still under development -- copied from the NIKHEFproject2018 repository

# # Author: Remco de Boer
# # Date: May 20th, 2018

# BINDIR = bin/
# INCLUDE_PATH = \
#   -Icore \
#   -Ialgorithms \
#   -Iobjs

# # Compiler
# CC = g++

# # Compiler flags: 'load' ROOT
# ROOTCFLAGS := $(shell root-config --cflags)
# ROOTLIBS   := $(shell root-config --glibs)
# CFLAGS = -fPIC -w -g -W ${ROOTCFLAGS} -O3 #-std=c++0x
# LFLAGS = ${ROOTLIBS} -g -lGenVector -lMinuit -O3

# # Automatically decide what to compile
# # Core parts of the framework
# CORE  = $(notdir $(wildcard core/*.cxx))
# COREOBJS  = $(CORE:.cxx=.o)
# COREOBJS := $(addprefix ${BINDIR}, ${COREOBJS})

# # Data classes
# OBJECTS = $(notdir $(wildcard objs/*.cxx))
# OBJOBJS = $(OBJECTS:.cxx=.do)
# OBJOBJS := $(addprefix ${BINDIR}, ${OBJOBJS})

# # User algorithms
# ALGORITHMS = $(notdir $(wildcard algorithms/*.cxx))
# ALGOBJS = $(ALGORITHMS:.cxx=.ao)
# ALGOBJS := $(addprefix ${BINDIR}, ${ALGOBJS})

# # Executable
# EXE = execute

# # Compile core, user algorithms and make an executable for each user algorithm
# all : ${COREOBJS} ${ALGOBJS} ${OBJOBJS} ${EXE}
# 	@echo "COMPILING DONE"

# # Compile core algorithms
# ${BINDIR}%.o : core/%.cxx core/%.h
# 	@echo "Compiling $(notdir $<)"
# 	@$(CC) $(CFLAGS) ${INCLUDE_PATH} -c $< -o $@

# ${BINDIR}%.ao : algorithms/%.cxx algorithms/%.h
# 	@echo "Compiling $(notdir $<)"
# 	@$(CC) $(CFLAGS) ${INCLUDE_PATH} -c $< -o $@

# ${BINDIR}%.do : objs/%.cxx objs/%.h
# 	@echo "Compiling $(notdir $<)"
# 	@$(CC) $(CFLAGS) ${INCLUDE_PATH} -c $< -o $@

# ${BINDIR}Steering.o : core/Steering.cxx
# 	@echo "Compiling Steering file"
# 	@$(CC) $(CFLAGS) ${INCLUDE_PATH} -c core/Steering.cxx -o ${BINDIR}Steering.o

# ${EXE} : ${COREOBJS} ${ALGOBJS} ${OBJOBJS}
# 	@echo "Making executable \"${EXE}\""
# 	@$(CC) -o ${EXE} $(COREOBJS) ${OBJOBJS} ${ALGOBJS} $(LFLAGS)

# # Remove all executables and object files
# # This rule can be called using "make clean"
# clean:
# 	@rm -f ${BINDIR}*
# 	@rm -f ${EXE}
# 	@echo "Deleted all binary files and the executable"



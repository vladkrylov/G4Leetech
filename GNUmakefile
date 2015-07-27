name := LeetechParallel
G4TARGET := $(name)
G4EXLIB := true
MPI_interface := ./mpi_interface

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
include $(MPI_interface)/G4MPI.gmk

LDLIBS1 += -lG4UImpi
CPPFLAGS +=-I$(MPI_interface)/include
EXTRALIBS = $(shell root-config --glibs)

CPPFLAGS += -I$(shell root-config --incdir)
LDLIBS   += $(shell root-config --libs)

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
	
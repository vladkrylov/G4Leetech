# $Id: GNUmakefile,v 1.2 2000-10-19 12:22:10 stanaka Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := micromegasParallel
G4TARGET := $(name)
G4EXLIB := true
MPI_interf := ./mpi_interface
MPI := /home/vlad/Program_Files/openmpi-1.4.2/build

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
include mpi_interface/G4MPI.gmk

LDLIBS1 += -lG4UImpi
CPPFLAGS +=-I$(MPI_interf)/include
EXTRALIBS = $(shell root-config --glibs)

ifdef ROOTSYS
  CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
  LDLIBS   += $(shell $(ROOTSYS)/bin/root-config --libs)
else
  CPPFLAGS += -I$(shell root-config --incdir)
  LDLIBS   += $(shell root-config --libs)
endif

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
	
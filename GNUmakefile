# $Id: GNUmakefile,v 1.2 2000-10-19 12:22:10 stanaka Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := micromegasParallel
G4TARGET := $(name)
G4EXLIB := true
MPI_interf := $(G4WORKDIR)/MicromegasParallel/mpi_interface
MPI := /home/vlad/Program_Files/openmpi-1.6.3/_installation

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

# subdir:
#	$(MAKE) mpi_interface/GNUmakefile
# .PHONY : subdir

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
include mpi_interface/G4MPI.gmk

LDLIBS1 += -lmpi_cxx
LDLIBS1 += -lG4UImpi
LDLIBS1 += -L/home/vlad/Program_Files/geant4.9.5/build/outputs/library/Linux-g++

EXTRALIBS = $(shell root-config --glibs)

ifdef ROOTSYS
  CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
  LDLIBS   += $(shell $(ROOTSYS)/bin/root-config --libs)
endif

CPPFLAGS +=-I/home/vlad/Program_Files/geant4.9.5/geant4.9.5.p02/no_include

CPPFLAGS +=-I/home/vlad/Program_Files/root/include
# CPPFLAGS +=-I/u/ofedorch/root/include/
# CPPFLAGS +=-I/lal/root/pro/include/root/

CPPFLAGS +=-I$(MPI_interf)/include
CPPFLAGS +=-I$(MPI)/include

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
#mpiclean:
#	$(MAKE) -f mpi_interface/GNUmakefile clean
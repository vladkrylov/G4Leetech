name := Leetech
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

EXTRALIBS = $(shell root-config --glibs)
CPPFLAGS += -I$(shell root-config --incdir)
LDLIBS   += $(shell root-config --libs)

CPPFLAGS += -ggdb
LDLIBS += -ggdb

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
	

#!/usr/bin/python
import getopt
import sys
import os
from subprocess import call

G4_work_dir = os.environ['G4WORKDIR']
G4HepRapp = G4_work_dir
MPexe = "%s/bin/Linux-g++/micromegasParallel" % G4_work_dir
G4java = 0

call([MPexe, '%s/MicromegasParallel/vis.mac' % G4_work_dir])

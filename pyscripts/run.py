#!/usr/bin/python
import getopt
import sys
import os
from subprocess import call

G4_work_dir = os.environ['G4WORKDIR']

number_of_processes = 1
MPexe = "%s/bin/Linux-g++/micromegasParallel" % G4_work_dir

opts, args = getopt.getopt(sys.argv[1:], 'n:')
for o, a in opts:
	if o == '-n':
		number_of_processes = a

call(['mpirun', '-np', number_of_processes, MPexe, '%s/MicromegasParallel/run.mac' % G4_work_dir])

#!/usr/bin/python
import getopt
import sys
import os
from subprocess import call, Popen

launch_heprapp = False
opts, args = getopt.getopt(sys.argv[1:], 'v')
for o, a in opts:
        if o == '-v':
            launch_heprapp = True

G4_work_dir = os.environ['G4WORKDIR']
project_dir = "%s/LeetechParallel" % G4_work_dir
G4HepRapp = "%s/HepRApp.jar" % G4_work_dir
exe = "%s/bin/Linux-g++/LeetechParallel" % G4_work_dir
G4java = "/home/vlad/Program_Files/java/jre1.6.0_30/bin/java"
HepRappFile = "%s/G4Data1.heprep" % project_dir

os.chdir(project_dir)
call([exe, "%s/vis.mac" % project_dir])

if launch_heprapp:
    Popen([G4java, '-jar', G4HepRapp, '-file', HepRappFile])
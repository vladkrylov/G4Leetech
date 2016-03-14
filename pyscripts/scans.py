import getopt
import sys
import os

from run import run
from change_parameter import change_parameter
from subprocess import call

# check for Geant4
try:
    G4_work_dir = os.environ['G4WORKDIR']
except KeyError:
    print "No Geant4 found"
    exit(1)
################################################

number_of_processes = 1
opts, args = getopt.getopt(sys.argv[1:], 'n:')
for o, a in opts:
    if o == '-n':
        number_of_processes = a

entrance_coll = range(1,21) # mm
E = 3500 # keV

project_dir = os.path.join(G4_work_dir, "Leetech")
run_mac = os.path.join(project_dir, 'run.mac')

change_parameter(run_mac, "/Micromegas/gun/ParticleEnergy", "%f keV" % E)

for d in entrance_coll:
    change_parameter(run_mac, "/Leetech/det/SetCollimatorEntranceGapX", "%f mm" % d)
    change_parameter(run_mac, "/Leetech/det/SetCollimatorEntranceGapY", "%f mm" % d)
    
    out_file = os.path.abspath("/home/vlad/10g4work/LeetechRuns/EntranceCollScan/opening=%dmm_E=%dkeV/result.root" % (d, E))
    run(number_of_processes=4,
        number_of_events=100000,
        out_file=out_file,
        readme_enable=False)
#     call(['python', ])
    

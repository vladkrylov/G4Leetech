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

exit_coll = range(1,21) # mm
E = 3500 # keV

project_dir = os.path.join(G4_work_dir, "Leetech")
run_mac = os.path.join(project_dir, 'run.mac')

change_parameter(run_mac, "/Micromegas/gun/ParticleEnergy", "%f keV" % E)

for d in exit_coll:
    change_parameter(run_mac, "/Leetech/det/SetCollimatorExit1GapX", "%f mm" % d)
    change_parameter(run_mac, "/Leetech/det/SetCollimatorExit1GapY", "%f mm" % d)
    
    out_file = os.path.abspath(os.path.join(G4_work_dir, "/exp/leetech/simulations/ExitCollScan_Entr1mm/opening=%dmm_E=%dkeV/leetech.root" % (d, E)))
    run(number_of_processes=4,
        number_of_events=10000,
        out_file=out_file,
        readme_enable=False)
#     call(['python', ])
    

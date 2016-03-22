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
entrance_opening = 1; # mm

opts, args = getopt.getopt(sys.argv[1:], 'n:p:')
for o, a in opts:
    if o == '-n':
        number_of_processes = a
    if o == '-p':
        entrance_opening = int(a)

exit_coll = range(1,21) # mm
E = 3500 # keV
 
project_dir = os.path.join(G4_work_dir, "Leetech")
run_mac = os.path.join(project_dir, 'run.mac')
 
change_parameter(run_mac, "/Micromegas/gun/ParticleEnergy", "%f keV" % E)
change_parameter(run_mac, "/Leetech/gun/SourceROOTFile", "/exp/leetech/simulations/EntranceCollScan/B=400gauss/opening=%dmm_E=3500keV/leetech.root" % entrance_opening)
 
for d in exit_coll:
    change_parameter(run_mac, "/Leetech/det/SetCollimatorExit1GapX", "%f mm" % d)
    change_parameter(run_mac, "/Leetech/det/SetCollimatorExit1GapY", "%f mm" % d)
     
    number_of_events = 100000
    if d < 5:
        number_of_events = 1000000
     
    out_file = os.path.abspath(os.path.join(G4_work_dir, "/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr%dmm/opening=%dmm/leetech.root" % (entrance_opening, d)))
    run(number_of_processes=1,
        number_of_events=number_of_events,
        out_file=out_file,
        readme_enable=False)


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

number_of_processes = 2
E = 3500 # keV
entrance_opening = 1 # mm

project_dir = os.path.join(G4_work_dir, "Leetech")
run_mac = os.path.join(project_dir, 'run.mac')
 
change_parameter(run_mac, "/Micromegas/gun/ParticleEnergy", "%f keV" % E)
# change_parameter(run_mac, "/Leetech/gun/SourceROOTFile", "/exp/leetech/simulations/EntranceCollScan/B=400gauss/opening=%dmm_E=3500keV/leetech.root" % entrance_opening)
change_parameter(run_mac, "/Leetech/gun/SourceROOTFile", "/home/vlad/10g4work/LeetechRuns/EntranceCollScans/EntranceCollScan_7detectors/opening=19mm_E=3500keV/leetech.root")
 
# for d in exit_coll:
d = 1
change_parameter(run_mac, "/Leetech/det/SetCollimatorExit1GapX", "%f mm" % d)
change_parameter(run_mac, "/Leetech/det/SetCollimatorExit1GapY", "%f mm" % d)
 
number_of_events = 100
 
for process_id in range(number_of_processes):
    # change output file
    #     out_file = os.path.abspath(os.path.join(G4_work_dir, "/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr%dmm/opening=%dmm/leetech_p%d.root" 
    out_file = "/home/vlad/10g4work/Leetech/tests_Entr%dmmopening=%dmm/leetech_p%d.root" % (entrance_opening, d, process_id)
    # run the simulation
    run(number_of_processes=1,
        number_of_events=number_of_events,
        out_file=out_file,
        readme_enable=False)


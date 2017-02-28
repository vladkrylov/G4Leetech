import os

from run import run
from change_parameter import change_parameter
from subprocess import call

# run_mac = os.path.join('/exp/leetech/simulations/geant4_workdir/G4Leetech/run.mac')
# out_dir = os.path.join('/exp/leetech/simulations')

run_mac = os.path.join('/home/vlad/10g4work/G4Leetech/run.mac')
out_dir = os.path.join('/home/vlad/10g4work/LeetechRuns')

E = 3500 # keV
change_parameter(run_mac, "/Leetech/gun/ParticleEnergy", "%f keV" % E)

targets = [0.1, 0.5, 1, 2, 3, 4, 5, 6, 7, 8]  # degrees
for d in targets:
    change_parameter(run_mac, "/Micromegas/det/setCupThick", "%f mm" % d)
    
    d_string = str(d).replace('.', '_')
    out_file = os.path.abspath(os.path.join(out_dir,
               "t_leetech_%smm.root" % d_string))
    run(number_of_processes=4,
        number_of_events=1e4,
        out_file=out_file,
        readme_enable=False)


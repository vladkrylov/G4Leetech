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

rmss = [0.2, 0.4, 0.6, 0.8, 1, 1.5, 2]  # degrees
for rms in rmss:
    change_parameter(run_mac, "/Leetech/gun/ParticleDirectionRMS", "%f deg" % rms)
    
    rms_string = str(rms).replace('.', '_')
    out_file = os.path.abspath(os.path.join(out_dir,
               "BeamTime6/4gps/leetech.root"))
    run(number_of_processes=4,
        number_of_events=2e4,
        out_file=out_file,
        readme_enable=False)


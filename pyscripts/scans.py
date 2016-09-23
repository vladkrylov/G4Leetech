import os

from run import run
from change_parameter import change_parameter
from subprocess import call

run_mac = os.path.join('/exp/leetech/simulations/geant4_workdir/Leetech/run.mac')

E = 3500 # keV
change_parameter(run_mac, "/Leetech/gun/ParticleEnergy", "%f keV" % E)

rmss = range(0, 3, 2)  # degrees
for rms in rmss:
    change_parameter(run_mac, "/Leetech/gun/ParticleDirectionRMS", "%d deg" % int(rms))
     
    out_file = os.path.abspath("/exp/leetech/simulations/BeamSizeEffect/sgm=%d/leetech.root" % int(rms))
    run(number_of_processes=8,
        number_of_events=1e8,
        out_file=out_file,
        readme_enable=False)


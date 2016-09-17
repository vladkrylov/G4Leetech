import os

from run import run
from change_parameter import change_parameter
from subprocess import call

run_mac = os.path.join('/exp/leetech/simulations/geant4_workdir/Leetech/run.mac')

E = 3500 # keV
change_parameter(run_mac, "/Leetech/gun/ParticleEnergy", "%f keV" % E)

angles = range(0, 36, 5)  # degrees
for phi in angles:
    change_parameter(run_mac, "/Micromegas/det/setRotationDeg", "%d deg" % int(phi))
     
    out_file = os.path.abspath("/exp/leetech/simulations/LeetechRotation/phi=%d_deg/phi=%d_deg.root" % (int(phi), int(phi)))
    run(number_of_processes=8,
        number_of_events=1e8,
        out_file=out_file,
        readme_enable=False)


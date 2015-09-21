import getopt
import sys
import os
from run import run
from change_parameter import change_parameter
from merge_root_files import merge as merge_root_files

number_of_processes = 1
opts, args = getopt.getopt(sys.argv[1:], 'n:')
for o, a in opts:
    if o == '-n':
        number_of_processes = a

target = 4 # mm
E = 3500 # keV
Bs = range(20, 101, 20) # Gauss

change_parameter("/Micromegas/det/setCupThick", "%f mm" % target)
change_parameter("/Micromegas/gun/ParticleEnergy", "%f MeV" % E)

for B in Bs:
    change_parameter("/Micromegas/det/setField", "%f gauss" % B)
    
    out_dir = os.path.abspath("../../LeetechRuns/scan/d=%dmm_E=%dMev_B=%dG" % (target, E, B))
    run(number_of_processes=number_of_processes,
        out_user_dir_name=out_dir,
        readme_enable=False)
    
    merge_root_files(out_dir)
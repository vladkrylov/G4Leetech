import getopt
import sys
import os
import re
from subprocess import call
from output_structure import setOutDirName
from linecache import getlines
from time import sleep

try:
	from mergeRootFiles import merge
except ImportError:
	merge = None

# set up some parameters and default options
G4_work_dir = os.environ['G4WORKDIR']

number_of_processes = 1
exe = os.path.join(G4_work_dir, "bin/Linux-g++/Leetech")
project_dir = os.path.join(G4_work_dir, "Leetech")
favorite_editor = "vim"

out_user_dir_name = None
readme_enable = True

run_command = [exe, os.path.join(project_dir, 'run.mac &')]

# check for user provided options
opts, args = getopt.getopt(sys.argv[1:], 'n:o:', ["remote", "noreadme"])
for o, a in opts:
	if o == '-n':
		number_of_processes = a
	elif o == '-o':
		out_user_dir_name = a
	elif o == '--noreadme':
		readme_enable = False
		
# create the output directory and modify run.mac file appropriately
# output_dir = setOutDirName(project_dir, out_user_dir_name)
output_dir = os.path.abspath(out_user_dir_name)
os.makedirs(output_dir)

call(['cp', os.path.join(project_dir, 'run.mac'), os.path.join(output_dir, 'run.mac')])
os.system('git log -1 > %s' % os.path.join(output_dir, 'version.txt'))

# open the readme file for careful user if enabled
readme = os.path.join(output_dir, "README.md")
if readme_enable:
	with open(readme, "w") as readme_file:
		readme_file.write("Add the description of current simulation run here.")
	call([favorite_editor, readme])
 	
# run the simulation
p = call(command_list)



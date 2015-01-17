#!/usr/bin/python
import getopt
import sys
import os
from subprocess import call
from output_structure import setOutDirName

# set up some parameters and default options
G4_work_dir = os.environ['G4WORKDIR']

number_of_processes = 1
MPexe = "%s/bin/Linux-g++/LeetechParallel" % G4_work_dir
project_dir = "%s/LeetechParallel" % G4_work_dir
favorite_editor = "vim"

command_list = ['mpirun']
out_user_dir_name = None
readme_enable = True

# check for user provided options
opts, args = getopt.getopt(sys.argv[1:], 'n:o:', ["remote", "noreadme"])
for o, a in opts:
	if o == '-n':
		number_of_processes = a
	elif o == '--remote':
		command_list.insert(0, 'nohup')
	elif o == '-o':
		out_user_dir_name = a
	elif o == '--noreadme':
		readme_enable = False
		
arg_list = ['-np', str(number_of_processes), MPexe, '%s/run.mac' % project_dir]
command_list.extend(arg_list)

# create the output directory and modify run.mac file appropriately
output_dir = setOutDirName(project_dir, out_user_dir_name)

call(['cp', "%s/run.mac" % project_dir, "%s/run.mac" % output_dir])

# open the readme file for careful user if enabled
if readme_enable:
	readme = "%s/README.md" % output_dir
	with open(readme, "w") as readme_file:
		readme_file.write("Add the description of current simulation run here.")
	call([favorite_editor, readme])
	
# run the simulation
call(command_list)
# print command_list







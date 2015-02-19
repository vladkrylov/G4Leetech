#!/usr/bin/python
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
MPexe = "%s/bin/Linux-g++/LeetechParallel" % G4_work_dir
project_dir = "%s/LeetechParallel" % G4_work_dir
favorite_editor = "vim"

command_list = ['mpirun']
out_user_dir_name = None
readme_enable = True
send_results = False

# check for user provided options
opts, args = getopt.getopt(sys.argv[1:], 'n:o:', ["remote", "noreadme", "email="])
for o, a in opts:
	if o == '-n':
		number_of_processes = a
	elif o == '--remote':
		command_list.insert(0, 'nohup')
	elif o == '-o':
		out_user_dir_name = a
	elif o == '--noreadme':
		readme_enable = False
	elif o == '--email':
		send_results = True
		email = a
		
arg_list = ['-np', str(number_of_processes), MPexe, '%s/run.mac' % project_dir]
command_list.extend(arg_list)

# create the output directory and modify run.mac file appropriately
output_dir = setOutDirName(project_dir, out_user_dir_name)

call(['cp', "%s/run.mac" % project_dir, "%s/run.mac" % output_dir])

# open the readme file for careful user if enabled
readme = "%s/README.md" % output_dir
if readme_enable and not os.path.exists(readme):
	with open(readme, "w") as readme_file:
		readme_file.write("Add the description of current simulation run here.")
	call([favorite_editor, readme])
	
# run the simulation
p = call(command_list)
# print command_list
# print output_dir

# merge output root files produced by MPI processes to one
# if merge:
# 	# determine the basename of output root files 
# 	with open("%s/run.mac" % project_dir, 'r') as run_mac:
# 		for l in run_mac.readlines():
# 			m = re.match(r"(/Leetech/RootFile) (.+)", l)
# 			if m:
# 				basename = m.groups()[-1].split('/')[-1]
# 	merge(basename, output_dir)

if send_results:
	from output_structure import which
	if which("mail"):
		tar_folder = output_dir.split('/')[-1]
		tar_name = "%s.tar.gz" % tar_folder
		os.chdir('/'.join(output_dir.split('/')[:-1]))
		call(['tar', '-czvf', tar_name, tar_folder])
	# echo "Text" | mail -s "pc-instrument2 Geant4 run report" -a 3.5MeV_1mm_Al_target.tar.gz vladkrilov9@gmail.com
		call(["echo \"See run results attached\" | ", "mail", '-s', "%s results" % tar_folder, '-a ', tar_name, email])
	else:
		print "Cannot send the email: mail command is not available."
	
	
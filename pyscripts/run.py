import getopt
import sys
import os
import re
from subprocess import call
from output_structure import setOutDirName
from linecache import getlines
from time import sleep
from change_parameter import change_parameter
from sre_parse import isdigit

try:
	from mergeRootFiles import merge
except ImportError:
	merge = None

# set up some parameters and default options
G4_work_dir = os.environ['G4WORKDIR']

exe = os.path.join(G4_work_dir, "bin/Linux-g++/Leetech")
project_dir = os.path.join(G4_work_dir, "Leetech")
favorite_editor = "vim"

run_command = [exe, os.path.join(project_dir, 'run.mac')]

def set_out_dir(user_dir):
	output_dir = os.path.abspath(user_dir)
	if os.path.exists(output_dir):
		if isdigit(output_dir[-1]):
			output_dir = output_dir[:-1] + str(int(output_dir[-1]) + 1)
		else:
			output_dir = output_dir + "_1"
	os.makedirs(output_dir)
	return output_dir
		
# create the output directory and modify run.mac file appropriately
def run(number_of_processes, out_user_dir, readme_enable):
	if out_user_dir:
		output_dir = set_out_dir(out_user_dir)
	else:
		output_dir = project_dir
	
	change_parameter('/run/numberOfThreads', str(number_of_processes))
	change_parameter('/Leetech/RootFile', os.path.join(output_dir, 'leetech'))
	
	call(['cp', os.path.join(project_dir, 'run.mac'), os.path.join(output_dir, 'run.mac')])
	os.system('git log -1 > %s' % os.path.join(output_dir, 'version.txt'))
	 
	# open the readme file for careful user if enabled
	readme = os.path.join(output_dir, "README.md")
	if readme_enable:
		with open(readme, "w") as readme_file:
			readme_file.write("Add the description of current simulation run here.")
		call([favorite_editor, readme])
	   	
	# run the simulation
	p = call(run_command)

if __name__ == '__main__':
	out_user_dir_name = None
	number_of_processes = 1
	readme_enable = True
	
	# check for user provided options
	opts, args = getopt.getopt(sys.argv[1:], 'n:o:', ["remote", "noreadme"])
	for o, a in opts:
		if o == '-n':
			number_of_processes = a
		elif o == '-o':
			out_user_dir_name = a.replace(" ", "_")
		elif o == '--noreadme':
			readme_enable = False
	
	run(number_of_processes=number_of_processes,
		out_user_dir_name=out_user_dir_name,
		readme_enable=readme_enable)

import getopt
import sys
import os
import re

from subprocess import call
from linecache import getlines
from time import sleep
from change_parameter import change_parameter
from sre_parse import isdigit

# set up some parameters and default options
# check for Geant4
try:
	G4_work_dir = os.environ['G4WORKDIR']
except KeyError:
	print "No Geant4 found"
	exit(1)
# check for ROOT
try:
	ROOTSYS_dir = os.environ['ROOTSYS']
except KeyError:
	print "No ROOT found"
	exit(1)

exe = os.path.join(G4_work_dir, "bin/Linux-g++/Leetech")
project_dir = os.path.join(G4_work_dir, "G4Leetech")
favorite_editor = "vim"

run_command = [exe, os.path.join(project_dir, 'run.mac')]

# create the output directory and modify run.mac file appropriately
def run(number_of_processes, out_file, number_of_events, readme_enable):
	# open the readme file for careful user if enabled
# 	readme = os.path.join(output_dir, "README.md")
# 	if readme_enable:
# 		with open(readme, "w") as readme_file:
# 			readme_file.write("Add the description of current simulation run here.")
# 		call([favorite_editor, readme])
   	
   	# where to place the results
   	out_file = os.path.abspath(os.path.join(os.getcwd(), out_file))
	(out_path, filename) = os.path.split(out_file)
	# filename without extension
	filebase = filename.split('.')[0]
	try:
		os.makedirs(out_path)
	except OSError:
		pass
	
	run_mac = os.path.join(project_dir, 'run.mac')
	# set the number of threads filebase to run.mac
	change_parameter(run_mac, '/run/numberOfThreads', str(number_of_processes))
	change_parameter(run_mac, "/Leetech/OutRootFile", filebase)
	change_parameter(run_mac, '/run/beamOn', int(number_of_events))
	# copy the run.mac to the output file
	call(['cp', run_mac, os.path.join(out_path, 'run.mac')])
	
	print os.getcwd()
	# run the simulation
	p = call(run_command)
	
	# merge root files
	out_files = [os.path.abspath(f) 
				for f in os.listdir(os.getcwd()) 
				if os.path.isfile(os.path.abspath(f)) 
				and f.startswith(filebase + "_t")
				and f.endswith(".root")]
	if out_files:
		call(['hadd', '-f', out_file] + out_files)
		for f in out_files:
			os.remove(f)
		
   		
   	print out_file, filebase


if __name__ == '__main__':
	out_file = 'leetech.root'
	number_of_processes = 1
	number_of_events = 1
	readme_enable = True
	
	# check for user provided options
	opts, args = getopt.getopt(sys.argv[1:], 'n:t:o:', ["noreadme"])
	for o, a in opts:
		if o == '-t':
			number_of_processes = a
		elif o == '-n':
			number_of_events = a
		elif o == '-o':
			out_file = a.replace(" ", "_")
		elif o == '--noreadme':
			readme_enable = False
			
	run(number_of_processes=number_of_processes,
		number_of_events=number_of_events,
		out_file=out_file,
		readme_enable=readme_enable)

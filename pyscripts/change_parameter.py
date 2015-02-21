#!/usr/bin/python
import os
import re
import inspect
from argparse import ArgumentParser
from subprocess import call

current_path = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

parser = ArgumentParser()
parser.add_argument("Parameter", help="Parameter you want to change in run.mac file")
parser.add_argument("Value", help="Value of corresponding parameter")
args = parser.parse_args()


mac_file_name = "%s/../run.mac" % current_path
mac_temp_file_name = "%s.temp" % mac_file_name
call(['cp', mac_file_name, mac_temp_file_name])

with open(mac_file_name, 'r') as mac_file:
    content = mac_file.read()

pattern = "(?<!#)%s.*\n" % args.Parameter
# pattern = re.compile(pattern_base, flags=re.DOTALL | re.MULTILINE)
# pattern_base = "const uint32_t thisSlaveCANstdID = "
# pattern = r"%s\d*;" % pattern_base
subs = "%s %s\n" % (args.Parameter, args.Value)
content = re.sub(pattern, subs, content)
with open(mac_temp_file_name, 'w') as tmp_file:
    tmp_file.write(content)

# replace the file with rewritten content
call(['rm', mac_file_name])
call(['mv', mac_temp_file_name, mac_file_name])
import os
import sys
import re
import inspect
from subprocess import call

current_path = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

def change_parameter(mac_file_name, name, value):
#     mac_file_name = "%s/../../LeetechSimpleMagnetV.0.4/run.mac" % current_path
    mac_temp_file_name = "%s.temp" % mac_file_name
    call(['cp', mac_file_name, mac_temp_file_name])
    
    with open(mac_file_name, 'r') as mac_file:
        content = mac_file.read()
    
    pattern = "(?<!#)%s.*\n" % name # find uncommented string started with name
    substitution_string = "%s %s\n" % (name, value)
    content = re.sub(pattern, substitution_string, content)
    with open(mac_temp_file_name, 'w') as tmp_file:
        tmp_file.write(content)
    
    # replace the file with rewritten content
    call(['rm', mac_file_name])
    call(['mv', mac_temp_file_name, mac_file_name])
    
if __name__ == '__main__':
    change_parameter(sys.argv[1], sys.argv[2])
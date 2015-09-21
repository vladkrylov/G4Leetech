#!/usr/bin/python

import os
import re
import sys
import inspect
from subprocess import call

def modify_root_merge_script(path_to_rootfiles, rootfiles_list, result_file):
    # assume that MergeRootFiles.cxx is in the same directory as this file
    current_path = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
    merge_name = "%s/MergeRootFiles.cxx" % current_path
    merge_tmpname = "%s.tmp" % merge_name
    
    # make a copy of the script
    call(['cp', merge_name, merge_tmpname])
    with open(merge_name, 'r') as merge_cxx:
        content = merge_cxx.read()
    
    # FIXME    
    m = re.match(r"(.*const int numberOfFiles = )(\d+)(;.*)(.*mergeFilenames\[numberOfFiles\] = \{\n)(.*)(\};.*)(.*TFile resultFile\()(.*)(, \"RECREATE\"\);.*)", content, re.DOTALL)
    if m:
        with open(merge_tmpname, 'w') as tmp_file:
            tmp_file.write(m.group(1))
            tmp_file.write(str(len(rootfiles_list)))
            tmp_file.write(m.group(3))
            tmp_file.write(m.group(4))
            tmp_file.write(',\n'.join(['\"%s/%s\"' % (path_to_rootfiles, s) for s in rootfiles_list]))
            tmp_file.write(m.group(6))
            tmp_file.write(m.group(7))
            tmp_file.write("\"%s/%s\"" % (path_to_rootfiles, result_file))
            tmp_file.write(m.group(9))
            
        # replace the script with rewritten file
        call(['mv', merge_tmpname, merge_name])
        return merge_name
    else:
        print "Cannot modify %s file. Please add the definition of mergeFilenames = {...}" % merge_name
        return None
def no_dir_found(dir_name):
    raise Exception("%s: no such directory found." % dir_name)
    
def find_base_name(path_to_mergefiles):
    if not os.path.exists(path_to_mergefiles):
        return []
    all_files = [fname for fname in os.listdir(path_to_mergefiles) 
                           if os.path.isfile(("%s/%s") % (path_to_mergefiles, fname))]
    base_names = []
    for fname in all_files:
        m = re.match(r"(\d*)([a-zA-Z_]+)(\d*)(\.root$)", fname)
        if m:
            base_names.append(m.group(2))
    
    repeated_basenames = [name for name in base_names if base_names.count(name) > 1]
    if len(repeated_basenames) == 0:
        print "No .root files found to merge."
        return None
    else:
        return repeated_basenames[0]
        
def merge(path_to_mergefiles):
    filenames_base = find_base_name(path_to_mergefiles)
    if isinstance(path_to_mergefiles, basestring) and os.path.exists(path_to_mergefiles):
        out_filename = "Result.root"
        
        merge_filenames = [fname for fname in os.listdir(path_to_mergefiles) 
                                 if re.match(".*%s.*\.root$" % filenames_base, fname)]
        if len(merge_filenames) > 0:
            print "Merging %d files:" % len(merge_filenames)
            for fn in merge_filenames:
                print "\t%s" % fn
            print "into %s/%s" % (path_to_mergefiles, out_filename)
            
            root_script_name = modify_root_merge_script(path_to_mergefiles, merge_filenames, out_filename)
            if root_script_name:
                # call the script
                call(['root', '-l', root_script_name])
        else:
            print "No .root files were found to merge after the run..."
    else:
        print "mergeRootFiles: wrong path to output ROOT files provided."
       


if __name__ == "__main__":
    if len(sys.argv) > 1:
        full_merge_path = os.path.abspath(sys.argv[1])
        if base_name:
            merge(full_merge_path)
            
            
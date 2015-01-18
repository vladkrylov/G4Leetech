#!/usr/bin/python

import os
import re
import sys

try:
    from ROOT import gROOT
except:
    print """
Merging of ROOT files cannot be performed.

PyRoot libraries were not found. 
Check if you have installed them and added their path to PYTHONPATH enviromental variable.

For more instructions see here:
https://root.cern.ch/drupal/content/how-use-use-python-pyroot-interpreter 
"""
else:
    from ROOT import TFile, TTree, TList
            
    def Merge(filenames_base, path_to_files):
        if isinstance(path_to_files, basestring) and os.path.exists(path_to_files):
            out_filename = "Result.root"
            tree_name = "T"
            
            merge_filenames = [fname for fname in os.listdir(path_to_files) 
                                    if re.match(".*%s.*\.root$" % filenames_base, fname)]
            if len(merge_filenames) > 0:
                print "Merging %d files:" % len(merge_filenames)
                for fn in merge_filenames:
                    print "\t%s" % fn
                print "into %s/%s" % (path_to_files, out_filename)
                 
                first_file = TFile("%s/%s" % (path_to_files, merge_filenames[0]),'read')
                first_tree = first_file.Get(tree_name)
                first_tree.SetBranchStatus('*',1)
                 
                out_file = TFile("%s/%s" % (path_to_files, out_filename), "RECREATE")
                out_tree = first_tree.CloneTree()
                 
                for fname in merge_filenames[1:]:
                    f = TFile("%s/%s" % (path_to_files, fname), 'read')
                    t = f.Get(tree_name)
                    t.SetBranchStatus('*',1)
                    out_tree.CopyEntries(t)
                
                out_file.cd()
                out_tree.Write()
                out_file.Close()
            else:
                print "No root files were found to merge after the run..."
        else:
            print "mergeRootFiles: wrong path to output ROOT files provided."
            
    def findBaseName(path_to_files):
        if not os.path.exists(path_to_files):
            return []
        all_files = [fname for fname in os.listdir(path_to_files) 
                               if os.path.isfile(("%s/%s") % (path_to_files, fname))]
        base_names = []
        for fname in all_files:
            m = re.match(r"(\d*)([a-zA-Z]+)(\d*)(\.root$)", fname)
            if m:
                base_names.append(m.group(2))
        
        repeated_basenames = [name for name in base_names if base_names.count(name) > 1]
        if len(repeated_basenames) == 0:
            print "No .root files found to merge."
            return None
        else:
            return repeated_basenames[0]
     
    if __name__ == "__main__":
        if len(sys.argv) > 1:
            path_to_files = sys.argv[1]
            if not os.path.exists(path_to_files) and not os.path.exists("%s/%s" % (os.getcwd(), path_to_files)):
                print "%s: no such directory found." % sys.argv[1]
                path_to_files = None
        else:
            path_to_files = os.getcwd()
        
        if path_to_files:
            base_name = findBaseName(path_to_files)
            if base_name:
                Merge(base_name, path_to_files)
            
            
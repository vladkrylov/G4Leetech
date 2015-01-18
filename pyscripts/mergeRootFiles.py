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
    from copy import deepcopy
            
    def Merge(filenames_base, path_to_files):
        if isinstance(path_to_files, basestring) and os.path.exists(path_to_files):
            merge_filenames = [fname for fname in os.listdir(path_to_files) 
                                    if re.match(".*%s.*\.root$" % filenames_base, fname)]
            tree_name = "T"
             
            first_file = TFile("%s/%s" % (path_to_files, merge_filenames[0]),'read')
            first_tree = first_file.Get(tree_name)
             
            out_file = TFile("%s/Result.root" % path_to_files, "RECREATE")
            out_tree = first_tree.CloneTree()
             
            for fname in merge_filenames[1:]:
                f = TFile("%s/%s" % (path_to_files, fname), 'read')
                t = f.Get(tree_name)
                out_tree.CopyEntries(t)
            
            out_file.Write()
            out_file.Close()
        else:
            print "mergeRootFiles: wrong path to output ROOT files provided."
            
#     def findOutputFiles(path_to_files):
#         if not os.path.exists(path_to_files):
#             return []
#         all_files = [fname for fname in os.listdir(path_to_files) 
#                                if os.path.isfile("%s/%s") % (path_to_files, fname)]
#         
#     
#     if __name__ == "__main__":
#         if len(sys.argv < 2):
#             path_to_files = sys.argv[1]
#             if not os.path.exists(path_to_files):
#                 path_to_files = "%s/%s" % (os.getcwd(), path_to_files)
#                 if not os.path.exists(path_to_files):
#                     raise Exception("%s: no such directory found.")
#         else:
#             path_to_files = os.getcwd()
#             
#         files_to_merge = findOutputFiles(path_to_files)
#         Merge(files_to_merge)
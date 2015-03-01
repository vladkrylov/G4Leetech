import fileinput
import re
import os

# setting up the output directory
def setOutDirName(project_dir, out_user_dir_name=None):
    parent_out_dirname = os.path.abspath("%s/../LeetechRuns" % project_dir)
    if not os.path.exists(parent_out_dirname):
        try:
            os.mkdir(parent_out_dirname)
        except OSError:
            raise Exception('''Output directory %s already exists but it seems you don't 
                  have permissions to access this folder.''' % parent_out_dirname)
        except:
            print "Unexpected error:", sys.exc_info()[0]
            raise
    existing_out_dirs = [d for d in os.listdir(parent_out_dirname) if os.path.isdir("%s/%s" % (parent_out_dirname, d))]
            
    if out_user_dir_name:
        if out_user_dir_name in existing_out_dirs:
            raise Exception("Output directory %s already exists. Choose please another name." % out_user_dir_name)
        else:
            current_out_dirname = "%s/%s" % (parent_out_dirname, out_user_dir_name)
    else:
        existing_ids = [int(d[-4:]) for d in existing_out_dirs if re.match(r"Run_[0-9]{4}$", d)]
        if not existing_ids:
            current_run_id = 1
        else:
            current_run_id = max(existing_ids) + 1
        current_out_dirname = "%s/Run_%s" % (parent_out_dirname, "%.4d" % current_run_id)
        
    os.mkdir(current_out_dirname)
    writeRunMac(project_dir, current_out_dirname)
    
    return current_out_dirname

# modifies (or adds) a string in run.mac which specifies output ROOT files names
def writeRunMac(project_dir, output_dirname):
    written = False
    relative_out_path = "/".join(output_dirname.split("/")[-2:])
    for line in fileinput.input("%s/run.mac" % project_dir, inplace=True):
        if re.match(r'/Leetech/RootFile +', line):
            print ("%s %s" % (line.split()[0], "%s/LeetechRun" % output_dirname)).rstrip()
            written = True
        else:
            print line.rstrip()
            
    if not written:
        with open("%s/run.mac" % project_dir, 'a') as run_mac:
            run_mac.write("/Leetech/RootFile %s/LeetechRun" % output_dirname)

# find an executable
# taken here http://stackoverflow.com/questions/377017/test-if-executable-exists-in-python
def which(program):
    import os
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file

    return None
            
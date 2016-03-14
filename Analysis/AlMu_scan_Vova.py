import os
import re
from array import array
import ROOT

def getX(dir):
    p = re.compile(".*d=(\d+).*")
    m = p.match(dir.split("/")[-1])
    return int(m.group(1))

def getY(root_file, tree_name):
    counts = 0
    tree = f.Get(tree_name)
    if tree:
        N = tree.GetBranch("Energy").GetEntries()
        for event in tree:
            if event.Charge == -1:  # counting electrons
                counts += 1
    return float(counts)/N
        

c = ROOT.TCanvas('B field scan')
 
scan_path = '/home/vlad/10g4work/LeetechRuns/AlMuScan/Vova'
data_listdir = ["%s/%s" % (scan_path, d) for d in os.listdir(scan_path) if os.path.isdir(("%s/%s") % (scan_path, d))]

targets = []
hits = []
  
for dir in data_listdir:
    d = getX(dir)
    if d:
        targets.append(d)
        f = ROOT.TFile("%s/leetech.root" % dir)
        try:
            hits.append(getY(f, "Hits"))
            print d, '\t', hits[-1]
        except:
            hits.append(0)
            print d, '\t', "Empty results file"
        f.Close()
        
        
if targets and hits:
  g = ROOT.TGraph(len(targets), array("d", targets), array("d", hits))

  g.Draw("A*")
  g.SetTitle("Title")
  g.GetXaxis().SetTitle("Target thickness, mm")
  g.GetYaxis().SetTitle("Number of events")

  c.SetLogy()
  c.SetGrid()
  c.Update()
    
  raw_input('Press <ret> to end -> ')




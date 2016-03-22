from ROOT import *
from array import array
from subprocess import call
from numpy import zeros

import math
import os
import sys

x = []
y = []
dy = []

title = ''
xlabel = 'Entrance collimators opening, mm'
ylabel = 'Hits'

Npoints = 0
with open('/home/vlad/10g4work/LeetechRuns/EntranceCollScans/B=400gauss/entrance_fit_exit=20mm.txt', 'r') as f:
    for line in f:
        if Npoints == 0:
            Npoints += 1
        else:
            Npoints += 1
            data = line.split()
            
            x.append(float(data[0]))
            y.append(float(data[1]))
            dy.append(float(data[2]))

canvas = TCanvas( "canvas", "canvas", 50, 50, 1200, 600 )
g = TGraphErrors(len(x), array("d", x), array("d", y), zeros(len(x)), array("d", dy))
    
g.Draw("A*")
g.SetMarkerSize(1)
g.SetMarkerStyle(20)
g.SetMarkerColor(kBlue)
g.SetLineWidth(2)

g.SetTitle(title)
g.GetXaxis().SetTitle(xlabel)
g.GetYaxis().SetTitle(ylabel)
canvas.SetGrid()

g.GetXaxis().SetLabelSize(0.05);
g.GetXaxis().SetTitleSize(0.05);
g.GetXaxis().SetTitleOffset(0.95);

g.GetYaxis().SetLabelSize(0.05);
g.GetYaxis().SetTitleSize(0.06);
g.GetYaxis().SetTitleOffset(0.78);

# ---------------------------------------------------------------------------------- 
raw_input('Press Enter to exit')

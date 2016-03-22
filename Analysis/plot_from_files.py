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
xlabel = 'Exit collimators opening, mm'
ylabel = 'Sigma, MeV'

filenames = ["/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr1mm/exit_fit_entrance=1mm.txt"
            , "/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr2mm/exit_fit_entrance=2mm.txt"
            , "/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr3mm/exit_fit_entrance=3mm.txt"
            , "/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr4mm/exit_fit_entrance=4mm.txt"
            , "/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr5mm/exit_fit_entrance=5mm.txt"
            , "/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr7mm/exit_fit_entrance=7mm.txt"
             ]
graphs = []
markers = [20, 21, 22, 33, 34, 20, 25, 26, 27, 28, 30]
colors =  [4,  3,  2,  6,  9,  41, 46, 38, 28, 1,  43]

Npoints = 0
with open(filenames[0], 'r') as f:
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
graphs.append(TGraphErrors(len(x), array("d", x), array("d", y), zeros(len(x)), array("d", dy)))

g = graphs[0]
g.Draw("A*")
g.SetMarkerSize(1)
g.SetMarkerStyle(markers[0])
g.SetMarkerColor(colors[0])
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

# draw 2nd and other graphs if present
i = 0;
for fname in filenames[1:]:
    x = []
    y = []
    dy = []
    Npoints = 0
    
    with open(fname, 'r') as f:
        for line in f:
            if Npoints == 0:
                Npoints += 1
            else:
                Npoints += 1
                data = line.split()
                
                x.append(float(data[0]))
                y.append(float(data[1]))
                dy.append(float(data[2]))
    graphs.append(TGraphErrors(len(x), array("d", x), array("d", y), zeros(len(x)), array("d", dy)))
    i += 1
    
    g = graphs[i]
    g.Draw("*same")
    g.SetMarkerSize(1)
    g.SetMarkerStyle(markers[i])
    g.SetMarkerColor(colors[i])
    g.SetLineWidth(2)
    
    
graphs[0].SetMaximum(0.055)

# Legend:
leg = TLegend( 0.15, 0.70, 0.38, 0.85 )
leg.SetFillColor(kWhite)
leg.AddEntry(graphs[0], " {\Delta_{entr}}}=1mm", "P")
leg.AddEntry(graphs[1], " {\Delta_{entr}}}=2mm", "P")
leg.AddEntry(graphs[2], " {\Delta_{entr}}}=3mm", "P")
leg.AddEntry(graphs[3], " {\Delta_{entr}}}=4mm", "P")
leg.AddEntry(graphs[4], " {\Delta_{entr}}}=5mm", "P")
leg.AddEntry(graphs[5], " {\Delta_{entr}}}=7mm", "P")
leg.Draw()

canvas.Update()

# ---------------------------------------------------------------------------------- 
raw_input('Press Enter to exit')

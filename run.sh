#!/bin/bash

echo "Simulation started."

mpirun -np 4 ../bin/Linux-g++/micromegasParallel run12.mac
root -l UniteRoots.cxx
mv United.root new_G12.root

mpirun -np 4 ../bin/Linux-g++/micromegasParallel run12.mac
root -l UniteRoots.cxx
mv United.root new_G12_1.root

echo "Done..."
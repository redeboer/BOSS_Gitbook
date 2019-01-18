#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from basf2 import *
from modularAnalysis import inputMdst
from modularAnalysis import ntupleFile
from modularAnalysis import ntupleTree
from modularAnalysis import analysis_main

# load input ROOT file
inputMdst('default', 'mixed_mdst.root')

# define Ntuple tools
toolsEvt = ['EventMetaData', 'e+']
toolsEvt += ['MCGenTruthForTopoAna', '^e+']

ntupleFile('mixed.root')
ntupleTree('evt', '', toolsEvt)

# Process the events
process(analysis_main)

# print out the summary
print(statistics)

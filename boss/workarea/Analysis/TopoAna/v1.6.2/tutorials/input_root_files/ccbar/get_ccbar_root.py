#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from basf2 import *
from modularAnalysis import inputMdst
from modularAnalysis import ntupleFile
from modularAnalysis import ntupleTree
from modularAnalysis import analysis_main

# load input ROOT file
inputMdst('default', '/gpfs/home/belle/chenyq/Belle2/gb2/sub00/mdst_000001_prod00004775_task00000001.root')

# define Ntuple tools
toolsEvt = ['EventMetaData', 'e+']
toolsEvt += ['MCGenTruthForTopoAna', '^e+']

ntupleFile('ccbar.root')
ntupleTree('evt', '', toolsEvt)

# Process the events
process(analysis_main)

# print out the summary
print(statistics)

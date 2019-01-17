#!/bin/bash
time (../../../../bin/topoana.exe ccbar_topoana.card >ccbar_topoana.OUT 2>ccbar_topoana.err) 2>ccbar_topoana.time
mv ccbar_topoana.OUT ccbar_topoana.out
exit 0

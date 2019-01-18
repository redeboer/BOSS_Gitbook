#!/bin/bash
time (../../../bin/topoana.exe mixed_topoana.card >mixed_topoana.OUT 2>mixed_topoana.err) 2>mixed_topoana.time
mv mixed_topoana.OUT mixed_topoana.out
exit 0

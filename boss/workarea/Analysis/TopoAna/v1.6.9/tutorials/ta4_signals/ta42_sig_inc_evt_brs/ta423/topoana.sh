#!/bin/bash
time (../../../../bin/topoana.exe uubar_topoana.card >uubar_topoana.OUT 2>uubar_topoana.err) 2>uubar_topoana.time
mv uubar_topoana.OUT uubar_topoana.out
exit 0

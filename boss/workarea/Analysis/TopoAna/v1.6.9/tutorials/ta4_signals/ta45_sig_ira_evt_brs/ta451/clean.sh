#!/bin/bash
for file in mixed_topoana.root mixed_topoana.txt mixed_topoana.tex mixed_topoana.pdf mixed_topoana.out mixed_topoana.err mixed_topoana.time 
do
if [ -e $file ]
then
echo $file
rm $file
fi
done
exit 0

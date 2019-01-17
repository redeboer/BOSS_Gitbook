#!/bin/bash
for file in uubar_topoana.root uubar_topoana.txt uubar_topoana.tex uubar_topoana.pdf uubar_topoana.out uubar_topoana.err uubar_topoana.time 
do
if [ -e $file ]
then
echo $file
rm $file
fi
done
exit 0

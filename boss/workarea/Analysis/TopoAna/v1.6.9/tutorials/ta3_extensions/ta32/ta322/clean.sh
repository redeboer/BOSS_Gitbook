#!/bin/bash
for file in ccbar_topoana.root ccbar_topoana.txt ccbar_topoana.tex ccbar_topoana.pdf ccbar_topoana.out ccbar_topoana.err ccbar_topoana.time 
do
if [ -e $file ]
then
echo $file
rm $file
fi
done
exit 0

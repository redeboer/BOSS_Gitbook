#!/bin/bash
gb2_ds_get /belle/MC/release-01-00-02/DB00000294/MC10/prod00003969/s00/e1002/4S/r00000/mixed/mdst/sub00/mdst_000001_prod00003969_task10010000001.root 
mv sub00/mdst_000001_prod00003969_task10010000001.root mixed_mdst.root
rm sub00/ -r
exit 0

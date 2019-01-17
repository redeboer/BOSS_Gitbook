#!/bin/bash
basf2 get_mixed_root.py -n 50000
mv mixed.root mixed1.root 
basf2 get_mixed_root.py --skip-events 50000 -n 50000
mv mixed.root mixed2.root
exit 0

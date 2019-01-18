#!/bin/bash
grep ^add evt.pdl |awk '{print $5"\t"$9"\t"$4}' >pid_3pchrg_txtpnm.dat_from_evt_pdl
exit 0

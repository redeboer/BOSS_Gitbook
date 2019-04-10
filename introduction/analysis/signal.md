---
description: On exclusive Monte Carlo data analysis and cut flow
---

# Analysing signal shape

## Exclusive Monte Carlo simulations

### Designing decay cards

A decay should follow the pattern of:

```text
Decay <particle name>
  <branching fraction 1> <daughter 1a> <daughter 1b> <generator> <parameters>;
  <branching fraction 1> <daughter 2a> <daughter 2b ><generator> <parameters>;
Enddecay
```

If you do not overwrite a certain decay, the decay card called `DECAY.dec` in the `BesEvtGen` package will be used. Definitions of particles \(including their physical widths\) can be found in the file `pdt.table`. Both files can be found here \(in the case of BOSS 7.0.4\):

```text
/afs/ihep.ac.cn/bes3/offline/Boss/7.0.4/Generator/BesEvtGen/BesEvtGen-00-03-98/share
```

## Analysis

* Signal width
  * Rough estimate based on statistics
  * Performing fits
* Dalitz plots


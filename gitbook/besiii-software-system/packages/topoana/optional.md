---
description: >-
  The following fields do not have to be specified, but are useful for larger
  analyses.
---

# Optional fields

{% hint style="info" %}
**@todo** Many of the below fields still have to be described.
{% endhint %}

## Maximum fields

### Maximum number of entries to be processed

Speaks for itself :\) Do not use scientific notations like `1e5` for $$10^5$$, but use `10000`.

### Maximum hierarchy of heading decay branches to be processed in each event

### Maximum number of decay trees to be printed

### Maximum number of decay final states to be printed

### Maximum number of entries to be saved in a single output root file

### Maximum hierarchy of heading decay branches to be processed in each event

### Maximum number of decay trees to be analyzed

## `Cuts`

### Cut to select entries

### Method to apply cut to array variables (Two options: T and F. Default: F)

## Ignore fields

### Suppress the first branches of decay trees in the output txt/tex/pdf files (Two options: Y and N. default: N)

Initial decays \(e.g. $$e^+e^- \rightarrow J/\psi$$\) are not listed in the tables.

### Suppress the first branches of decay trees in the output txt/tex/pdf files (Two options: Y and N. Default: N)

The following columns are not written:

* **Decay trees and their respective final states**: iDcyTr and iDcyFSt
* **Table Signal particles**: iSigP
* **Signal decay final states**: iSigDcyFSt2 and iDcyFSt

### Ignore gISR photons (Two options: Y and N. Default: Y)

### Ignore gFSR photons (Two options: Y and N. Default: Y)

### Ignore the decay of the following particles

### Ignore the decay of the daughters of the following particles

## What to perform

### Process charge conjugate objects together (Two options: Y and N. Default: N)

### Skip the topology analysis of decay trees and decay final states (Two options: Y and N. Default: N)

Set this field to `Y` if you do not want to generate the tables that list _all_ decay topologies. **It is important to set this field if you are dealing with large data** and are only interested in certain inclusive decays! In this case, you should also make use of the signal fields.

### Perform the topology analysis of the decay branches begun with the following particles

### Perform the topology analysis of the exclusive decay branches matched with the following inclusive decay branches

## Signal fields

### Signal particles

If this field is filled, an additional table is generated with **counts of the signal particles** you specified. List the particles using line breaks \(do not use commas\).

### Signal inclusive decay branches

Here you can list the final state\(s\) of the signal decay\(s\) that you are looking for. Naturally, the order of the decay particles does not matter. The syntax is as follows:

* Start a line with a number \(starting with `0`\), then in the same line add a space or tab, and name list the decay particle according to the PDG plain name \(e.g. `pi+` for $$\pi^+$$\).
* Continue the next line of the same state description with a `1`and so forth.
* You can name several inclusive decay states by starting each series with a `0` again.
* See an example for the syntax [here](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/Analysis/TopoAna/v1.6.9/tutorials/ta4_signals/ta42_sig_inc_evt_brs/ta423/uubar_topoana.card).

### Signal sequential decay branches

### Signal inclusive sequential decay branches

### Signal intermediate-resonance-allowed decay branches

### Signal inclusive or intermediate-resonance-allowed sequential decay branches

### Signal decay trees

### Signal decay final states


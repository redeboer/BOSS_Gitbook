# TopoAna

## Format of a `topoana` card

This paragraph lists all fields that can be used in your `topoana` card.

### Required fields

The following fields **have to be specified** to your card.

* **`% Names of input root files`** One file per line without tailing characters, such as comma, semicolon and period. Just like in the `TChain::Add()` method, absolute and relative paths are supported, but wildcards \(`[]?*`\) are not. 
* **`% Tree name`** Name of the `TTree` that contains the MC truth data. Usually, this tree has been written by the `MctruthForTopo`algorithm and is called `"MctruthForTopoAna"`.
* **`% Branch name of the number of particles`** This branch is required for reading the two arrays specified below. In the `MctruthForTopo` package, it is called `"Nmcps"`.
* **`% Branch name of the array of particle identifications`** Usually called `"Pid"` in the `MctruthForTopo` package.
* **`% Branch name of the array of the mother indeces of particles`** Usually called `"Midx"` in the `MctruthForTopo` package.
* **`% Main name of output files`** When you run `topoana.exe`, four files with the same name but in different formats \(root/txt/tex/pdf\) will be written as output. The filename extensions are appended automatically, so it is not necessary to add these extensions to this field.

### Optional fields

* **`% Maximum number of entries to be processed`** 
* **`% Signal particles`** 
* **`% Maximum hierarchy of heading decay branches to be processed in each event`** 
* **`% Maximum number of decay trees to be analyzed`** 
* **`% Process charge conjugate objects together (Two options: Y and N. Default: N)`**
* **`% Cut to select entries`**
* **`% Method to apply cut to array variables (Two options: T and F. Default: F)`**
* **`% Ignore gISR photons (Two options: Y and N. Default: Y)`**
* **`% Ignore gFSR photons (Two options: Y and N. Default: Y)`**
* **`% Maximum hierarchy of heading decay branches to be processed in each event`**
* **`% Ignore the decay of the following particles`**
* **`% Ignore the decay of the daughters of the following particles`**
* **`% Maximum number of decay trees to be printed`**
* **`% Maximum number of decay final states to be printed`**
* **`% Suppress the first branches of decay trees in the output txt/tex/pdf files (Two options: Y and N. default: N)`**
* **`% Suppress the topology tags in the output tex/pdf files (Two options: Y and N. Default: N)`**
* **`% Maximum number of entries to be saved in a single output root file`**
* **`% Skip the topology analysis of decay trees and decay final states (Two options: Y and N. Default: N)`**
* **`% Perform the topology analysis of the decay branches begun with the following particles`**
* **`% Perform the topology analysis of the exclusive decay branches matched with the following inclusive decay branches`**
* **`% Signal inclusive decay branches`**
* **`% Signal sequential decay branches`**
* **`% Signal inclusive sequential decay branches`**
* **`% Signal intermediate-resonance-allowed decay branches`**
* **`% Signal inclusive or intermediate-resonance-allowed sequential decay branches`**
* **`% Signal decay trees`**
* **`% Signal decay final states`**


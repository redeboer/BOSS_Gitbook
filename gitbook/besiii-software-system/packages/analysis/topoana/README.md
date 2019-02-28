# TopoAna

This package is an extremely helpful tool for analysing [inclusive Monte Carlo samples](../../../../appendices/glossary.md#inclusive-monte-carlo-simulation). Inclusive MC samples give us valuable information about the **background** of your analysis, as it allows you to know the true contributions to that background. If you know what components that background exists of, you can:

* try to make smart cuts to remove those background components;
* use a particular function that describes that background component best when applying a fit to the real data.

The problem with inclusive samples, however, is that they can include thousands of decay modes. The `topoana` package allows you to make certain selections and to generate tables that list frequencies of particles and decay modes that are of interest to you.

{% hint style="info" %}
_Credit for the package goes to **Zhou Xingyu**_  
For more information about the motivation behind `topoana`, see [these slides](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/Analysis/TopoAna/v1.6.9/doc/readme.pdf).
{% endhint %}

## Preparing initial event selection

The `topoana` package has to be run over a ROOT file that you have to prepare yourself. The ROOT file has to contain a `TTree` with specific information of the Monte Carlo truth:

* the **run ID** number
* the **event ID** number
* a **track index** \(just a counter\)**,** which is necessary for loading the following arrays
* an array contain the **PDG code for each track** in this event
* an array containing the PDG code for the **mother of each track** \(if available\)

See the [documentation of `MctruthForTopo`](https://redeboer.github.io/BOSS_Afterburner/classMctruthForTopoAna.html#a7d5e77225fb2195719b0df23af66bbb9) for how these branches are typically called.

You can design a procedure to write this MC truth information yourself, but you can also use either of the following two methods:

1. Add the [`MctruthForTopo`](https://redeboer.github.io/BOSS_Afterburner/classMctruthForTopoAna.html) algorithm package to the job options of your analysis, using: `ApplicationMgr.DLLs += {"MctruthForTopoAnaAlg"}; ApplicationMgr.TopAlg += {"MctruthForTopoAna"};` This is the quickest solution, but it does not allow you to perform cuts: all the events will be written to the `TTree`.
2. Go through the code of the `MctruthForTopo` algorithm and take over the relevant components in your own initial event selection package, so that you can implement it within your cut procedure.
3. Use the [`CreateMCtruthCollection`](https://redeboer.github.io/BOSS_Afterburner/classTrackSelector.html#a3bee37dd275d6b15ca491ae1d493c05a) and [`WriteMcTruthForTopoAna`](https://redeboer.github.io/BOSS_Afterburner/classTrackSelector.html#ac65fb01ccb38c60af713518e0afb0ba6) in the [`TrackSelector`](../../../../the-boss-afterburner/initial/#base-derived-algorithm-structure) base algorithm.

### Structure of the `Event::McParticleCol` collection

The `TTree` containing Monte Carlo data that is needed for `topoana` is created by looping over the [`Event::McParticleCol`](http://bes3.to.infn.it/Boss/7.0.2/html/namespaceEvent.html#b6a28637c54f890ed93d8fd13d5021ed) in each event and writing the branches described above. To gain a better understanding of what a package like [`MctruthForTopo`](https://redeboer.github.io/BOSS_Afterburner/classMctruthForTopoAna.html) does, let's have a look at the the contents of the MC truth particle collection in one event:

| Index | Particle |  |  | Mother |  |  |
| ---: | ---: | :---: | :---: | ---: | :---: | :--- |
| 0 | 23 | `Z0` | $$Z^0$$ |  |  |  |
| 1 | 22 | `gamma` | $$\gamma$$ |  |  |  |
| 2 | 4 | `c` | $$c$$ | 23 | `Z0` | $$Z^0$$ |
| 3 | -4 | `anti-c` | $$\bar{c}$$ | 23 | `Z0` | $$Z^0$$ |
| 4 | 91 | `cluster` |  | -4 | `anti-c` | $$\bar{c}$$ |
| 5 | 443 | `J/psi` | $$J/\psi$$ | 91 | `cluster` |  |
| 6 | 11 | `e-` | $$e^-$$ |  |  |  |
| 7 | 421 | `D0` | $$D^0$$ | 443 | `J/psi` | $$J/\psi$$ |
| 8 | 333 | `phi` | $$\phi$$ | 443 | `J/psi` | $$J/\psi$$ |
| 9 | -321 | `K-` | $$K^-$$ | 421 | `D0` | $$D^0$$ |
| 10 | 221 | `eta` | $$\eta$$ | 421 | `D0` | $$D^0$$ |
| 11 | 321 | `K+` | $$K^+$$ | 333 | `phi` | $$\phi$$ |
| 12 | -321 | `K-` | $$K^-$$ | 333 | `phi` | $$\phi$$ |
| 13 | -13 | `mu+` | $$\mu^+$$ | 321 | `K+` | $$K^+$$ |
| 14 | 14 | `nu_mu` | $$\nu_\mu$$ | 321 | `K+` | $$K^+$$ |
| 15 | -11 | `e+` | $$e^+$$ | -13 | `mu+` | $$\mu^+$$ |
| 16 | 12 | `nu_e` | $$\nu_e$$ | -13 | `mu+` | $$\mu^+$$ |
| 17 | -14 | `anti-nu_mu` | $$\bar{\nu}_{\mu}$$ | -13 | `mu+` | $$\mu^+$$ |

## Installing `topoana`

Execute [`setup.sh`](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/Analysis/TopoAna/v1.6.9/setup.sh) and see the instructions there on how to source it. If you have done this, you can use the command `topoana.exe` the output generated through the [previous step](./#preparing-initial-event-selection).

## Format of a `topoana` card

If you have [prepared a ROOT file](./#preparing-initial-event-selection) and [installed `topoana.exe`](./#installing-topoana), you can analyse the output. The `topoana`package will generate some tables containing statistics of certain signal particles and signal decay modes. You can specify these signal particles and branches through a `topoana` card and run the analysis with the command `topoana.exe yourtopoanacard.card`.

A `topoana` card file \(`.card`extension\) is a text file that defines the way in which you execute `topoana.exe` on your data set. In this file, you for instance specify the input ROOT files that you want to analyse.

The syntax of the `topoana` card is slightly reminiscent of `bash`. Starting a line with:

* `#` means that the line is a comment and is therefore ignored;
* `%` means that the the line represents a field.

A opening curly brace \(`{`\) following a `%` sign means that a field block is opened. The next line\(s\) contain the value\(s\) of that field. Close the block with a closing curly brace \(`}`\). See [here](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/Analysis/TopoAna/v1.6.9/tutorials/ta1_basics/mixed_topoana.card) for an example of a `topoana` card.

The following pages list **all fields** that can be used in your `topoana` card: [required](required.md) and [optional fields](optional.md).

## Tips on the results

_\(From_ `topoana` _terminal output.\)_

1. Statistics of the topologies are summarized in three types files: `pdf`, `tex` and `txt`. Although these are different formats, they contain the same information. The `pdf` file is the easiest to read. It has been converted from the `tex` file using the `pdflatex` command. If necessary, you can check the contents of the `txt` file as well \(e.g. using text processing commands\).
2. Tags of the topologies are inserted in all the entries of `TTree` for `topoana` in the output ROOT file\(s\). The ROOT files may have been split up, in which case you should load them using a `TChain`. Except for this, the `TTree` for `topoana` data of the output ROOT file is entirely the same as that of the input ROOT file\(s\). In addition, the topology tags are identical with those listed in the txt, tex, and pdf files.

## Submitting a `topoana.exe` job

Just like a BOSS job, you can submit a `topoana` job to the queue. This is useful if your data is extensive and you want to log out while the job is executed. Just write your command in a `bash` script like this:

{% code-tabs %}
{% code-tabs-item title="yourbashfile.sh" %}
```bash
{ topoana.exe yourtopoanacard.card; } &> yourlogfile.log
```
{% endcode-tabs-item %}
{% endcode-tabs %}

The pipe \(`>`\) with the curly braces ensures that all output \(including warnings\) is written to the log file \(here, `yourlogfile.log`\).

Make sure that you make the `bash` script executable using `chmod +x yourbashfile.sh`. You can then submit your job to the queue using:

```bash
hep_sub -g physics yourbashfile.sh
```

and keep an eye on your jobs using:

```bash
hep_q -u $USER
```


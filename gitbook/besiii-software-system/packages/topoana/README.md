# TopoAna

## Preparing initial event selection

Use the [`MctruthForTopo`](https://redeboer.github.io/BOSS_Afterburner/classMctruthForTopoAna.html) algorithm package.

## Format of a `topoana` card

A `topoana` card file \(`.card`extension\) is a text file that defines the way in which you execute `topoana.exe` on your data set. In this file, you for instance specify the input ROOT files that you want to analyse.

The syntax of the `topoana` card is slightly reminiscent of `bash`. Starting a line with:

* `#` means that the line is a comment and is therefore ignored;
* `%` means that the the line represents a field.

A opening curly brace \(`{`\) following a `%` sign means that a field block is opened. The next line\(s\) contain the value\(s\) of that field. Close the block with a closing curly brace \(`}`\). See [here](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/Analysis/TopoAna/v1.6.9/tutorials/ta1_basics/mixed_topoana.card) for an example of a `topoana` card.

The following pages list **all fields** that can be used in your `topoana` card: [required](required.md) and [optional fields](optional.md).

## Tips on the results

_\(From `topoana` terminal output.\)_

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


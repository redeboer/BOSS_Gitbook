# Running jobs

Particle physicists perform analyses on either data from measurements or on data from Monte Carlo simulation. In **BOSS**, it is possible to generate your own Monte Carlo simulations and to treat its output as ordinary data. There are there for three basic steps in running a Monte Carlo job on **BOSS**:

1. `sim`: you perform a Monte Carlo simulation and generate a raw data file \(`rtraw`\).
2. `rec`: you reconstruct particle tracks from the raw data and write out a reconstructed data file \(`dst`\).
3. `ana`: you analyse the reconstructed tracks and generate a [CERN ROOT](https://root.cern.ch/input-and-output) file containing trees that describe event and track variables \(`root`\).

When you are analysing measurement data, you won't have to perform steps 1 and 2: the BESIII collaboration reconstructs all data samples whenever a new version of BOSS is released. \(See [Organisation of the IHEP server](https://github.com/redeboer/BOSS_Afterburner/tree/902bbfd0a1c109e93d69e39a384ddfed810d8a02/organisation-of-the-ihep-server/README.md), under "Reconstructed data sets", for where these files are located.\)

The steps are performed from your `jobOptions*.txt` files of your own package in your work area. These files are executed using the `boss.exe` command. As an example, you can run the `jobOptions` in in the `TestRelease` package:

```text
cd /ihepbatch/bes/$USER/TestRelease/TestRelease-*/run/
boss.exe jobOptions_sim.txt
boss.exe jobOptions_rec.txt
boss.exe jobOptions_ana_rhopi.txt
```

This is essentially it! Of course, for your own analysis, you will have to tweak the parameters in these `jobOptions_*.txt` files and, more importantly, modify the `RhopiAlg` source code to your own tastes.

{% hint style="info" %}
**Todo**: Describe \`Rhopi.
{% endhint %}

In the following, we will go through some extra tricks that you will need to master in order to do computational intensive analyses using **BOSS**.

## Submitting a job

The `TestRelease` example typically simulates, reconstructs, and analyses only 10 events. For serious work, you will have to generate thousands of events and this will take a long time. You can therefore submit your job to a so-called 'queue'. For this, there are two options: either you submit them using the command `hep_sub` or using the command `boss.conder`. The latter is easiest: you can use it just like `boss.exe`.

With `hep_sub`, however, you essentially forward a shell script to the queue, which then executes the commands in there. So you'll have to make that first. Let's say, you make a shell script `test.sh` in the `run` folder that looks like this:

```text
#!/bin/bash
boss.exe jobOptions_sim.txt
```

The first line clarifies that you use `bash`, the second does what you did when running a job: calling `boss.exe`, but of course, you can make this script to execute whatever `bash` commands you want.

@todo Not sure if you should `chmod +x` it?

Now you can simply submit the shell script to the queue using:

```text
hep_sub test.sh -g physics
```

and it will be executed in the background. Here, the option `-g` tellst that you are from the `physics` group. You can check whether the job is \(still\) running in the queue using:

```text
hep_q -u $USER
```

Note that `hep_q` would list all jobs from all users.

## Splitting up jobs

Jobs that take a long time to be executed in the queue will be killed by the server. It is therefore recommended that you analyse a maximum of **10,000 events** per job, particularly if you perform Monte Carlo simulations. Of course, you will be wanting to work with much larger data samples, sou you will have to submit parallel jobs. This can be done by writing different `jobOptions*.txt` files, where you modify the input/output file and random seed number.

What is much more convenient, however, is developing a `jobOptions*.txt` template file that is used to generate. In these, you for instance replace the specific paths and seed number you used by generic tokens like `INPUTFILE`, `OUTPUTFILE`, and `RANDOMSEED`. You can then use a `bash` script \(or comparable shell scripts\) to replace these tokens by unique paths and seed numbers. For example, have a look at the [`awk`](https://www.tldp.org/LDP/abs/html/awk.html) command.

{% hint style="info" %}
**Todo**: Describe process of generating `jobOptions*.txt` files  using the scripts in the repository.

**Todo**: Mention that `-1` in `ApplicationMgr.EvtMax` stands for 'all events'.
{% endhint %}


# Running jobs

Particle physicists perform analyses on either data from measurements or on data from Monte Carlo simulation. In BOSS, it is possible to generate your own Monte Carlo simulations and to treat its output as ordinary data. There are there for three basic steps in running a Monte Carlo job on BOSS:

1. **`sim`**: you perform a Monte Carlo simulation and generate a raw data file \(`rtraw`\).
2. **`rec`**: you reconstruct particle tracks from the raw data and write out a reconstructed data file \(`dst`\).
3. **`ana`**: you analyse the reconstructed tracks and generate a [CERN ROOT](https://root.cern.ch/input-and-output) file containing trees that describe event and track variables \(`root`\).

When you are analysing measurement data, you won't have to perform steps 1 and 2: the BESIII collaboration reconstructs all data samples whenever a new version of BOSS is released. \(See [Organisation of the IHEP server](), under "Reconstructed data sets", for where these files are located.\)

The steps are performed from `jobOptions*.txt` files of your own package in your work area. These files are executed using the `boss.exe` command. Usually, you use the `TestRelease` package to run other packages. In the case of `RhopiAlg`:

```bash
cd "$BOSSWORKAREA/workarea/TestRelease/TestRelease-*/run/"
boss.exe jobOptions_sim.txt
boss.exe jobOptions_rec.txt
boss.exe jobOptions_ana_rhopi.txt
```

This is essentially it! Of course, for your own analysis, you will have to tweak the parameters in these `jobOptions_*.txt` files and in `TestRelease` to integrate and run your own packages.

In the following, we will go through some extra tricks that you will need to master in order to do computational intensive analyses using **BOSS**.

## Submitting a job    <a id="submitting-a-job"></a>

The `TestRelease` package typically simulates, reconstructs, and analyses only a few hundred events. For serious work, you will have to generate thousands of events and this will take a long time. You can therefore submit your job to a so-called 'queue'. For this, there are two options: either you submit them using the command `hep_sub` or using the command `boss.condor`. The latter is easiest: you can use it just like `boss.exe`.

With `hep_sub`, however, you essentially forward a shell script to the queue, which then executes the commands in there. So you will **first put the command for your job in make a shell script** \(`.sh`\). Let's say, you make a shell script `test.sh` in the `run` folder that looks like this:

```bash
#!/bin/bash
boss.exe jobOptions_sim.txt
```

The first line clarifies that you use `bash`, the second does what you did when running a job: calling `boss.exe`, but of course, you can make this script to execute whatever `bash` commands you want.

The 'queue' \(`hep_sub`\) executes bash scripts using `./`, not the command `bash`. You therefore have to make the script executable. This is done through `chmod +x <yourscript>.sh` \('change mode to executable'\).

Now you can submit the shell script to the queue using:

```bash
hep_sub -g physics test.sh
```

and your job will be executed by the computing centre. Here, the option `-g` tells that you are from the `physics` group. A \(more or less\) equivalent to this command is `boss.condor test.sh`.

You can check whether the job is \(still\) running in the queue using:

```bash
hep_q -u $USER
```

Note that `hep_q` would list all jobs from all users. The first column of the table you see here \(if you have submitted any jobs\) is the job ID. If you have made some mistake in your analysis code, you can use this ID to **remove a job**, like this:

```bash
hep_rm 26345898.0
```

Alternatively, you can remove _all_ your jobs from the queue using `hep_rm -a`.

## Splitting up jobs    <a id="splitting-up-jobs"></a>

Jobs that take a long time to be executed in the queue will be killed by the server. It is therefore recommended that you work with a maximum of **10,000 events** per job if you perform Monte Carlo simulations \(the `sim` step consumes much computer power\). Of course, you will be wanting to work with much larger data samples, sou you will have to submit parallel jobs. This can be done by writing different `jobOptions*.txt` files, where you modify the input/output files and random seed number.

You can do all this by hand, but it is much more convenient to generate these files with some script \(whether C++, bash or `tcsh`\) that can generate `jobOptions*.txt` files from a certain _template file_. In these, you for instance replace the specific paths and seed number you used by generic tokens like `INPUTFILE`, `OUTPUTFILE`, and `RANDOMSEED`. You can then use the script to replace these unique tokens by a path or a unique number. Have a look at the [`awk`](https://www.tldp.org/LDP/abs/html/awk.html) and [`sed`](https://www.gnu.org/software/sed/manual/sed.html) commands to get the idea.

## Splitting scripts in the BOSS Afterburner

The BOSS Afterburner offers some bash scripts that can do job splitting for you. In the case of Monte Carlo simulation, reconstruction, and analysis, you work with run numbers and with unique random seed numbers, whereas in data analysis, you have to make selections of `dst` data files. The[`jobs` folder of the BOSS Afterburner](https://github.com/redeboer/BOSS_IniSelect/tree/master/jobs) therefore contains two different types of generation scripts.

As you can see, the folder contains a `templates` folder with files that contain the templates with tokens and there are also output folders `ana`, `rec`, `sim` \(for the three steps in MC simulations\), and `sub` \(shell scripts that you submit to the 'queue'\). Then there is also a folder `dec` containing your decay charts for exclusive Monte Carlo simulations \(see section [Monte Carlo simulations](https://besiii.gitbook.io/boss/besiii-software-system/jobs/simulations)\) and a folder called `filenames`. The last one is important for generating job options for data analysis \(see complications described under 2.\).

Finally, script procedures that are shared by both type of job option generation have been grouped into functions in the `CommonFunctions.sh` shell script.

### Job options for Monte Carlo simulations

These are built on four job option template files: one for the `sim` step, one for `rec`, one for `ana`, and one for the shell script that you feed to the 'queue' \(`hep_sub`\). Here, the complicating ingredient is the random seed number \(which has to be unique for each `sim` job option file\) and the list of run numbers \(which will determine the parameters in simulation and reconstruction\).

Usage is illustrated in the [`GenerateScript_sim.sh`](https://github.com/redeboer/BOSS_IniSelect/blob/master/jobs/GenerateScript_sim.sh) script.

### Job options for data \(or Monte Carlo\) analysis

These only result in `ana` job option files and in a shell script that you use to submit to the queue. Here, the complicating ingredient is the list of `dst` that you feed to the `ana` job option file. This list of filenames has to be split up into subgroups \(one for each job option file\) and has to be inserted into the C++ code \(at `EventCnvSvc.digiRootInputFile`\), formatted as a C++ vector of strings. You will also want not to put too many `dst` files there, because you then run the risk that job will be killed if the run time is too long. The generation of this type of job option files is therefore comprised of two parts:

1. Generate a list of \(`dst`\) files you want to analyse. You can do this by feeding a text file of directories and/or filenames that you want to analyse to a function called `CreateFilenameInventoryFromFile` in `CommonFunctions.sh`. Alternatively, you can use the function `CreateFilenameInventoryFromDirectory`, which lists all files within that directory. The output is a number of text files with a maximum number of lines each that together make up the complete list of files.
2. Insert the paths listed in each of the text files listed above into the `ana` template. For this, the lines of the text files are also converted into C++ vector-of-strings format \(i.e., separated by commas and surrounded by quotation `"` marks\).

Usage of both the relevant functions in the `CommonFunctions.sh` script and of the [`CreateJobFiles_ana.sh`](https://github.com/redeboer/BOSS_IniSelect/blob/master/jobs/CreateJobFiles_ana.sh) is illustrated in[`GenerateScript_ana.sh`](https://github.com/redeboer/BOSS_IniSelect/blob/master/jobs/GenerateScript_ana.sh).

{% hint style="info" %}
#### Analysing all events

In data analysis, you usually want to use all events available: cuts are applied to get rid of events you don't want. It is therefore better to use `-1`, which stands for '_all_ events', for the maximum number of events in `ApplicationMgr.EvtMax`.
{% endhint %}


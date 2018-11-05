The BOSS Afterburner {#mainpage}
================================

[TOC]

**This repository is underdevelopment.** It has been set up as an unofficial extension to the BOSS analysis framework of the [BESIII collaboration](http://bes3.ihep.ac.cn "Offical BESIII webpage") with two related purposes:

1. Serve as an accessible introduction and user guide to **BOSS**.
2. Provide basic tools to analyse and visualise output of **BOSS**.

If you do not have an IHEP networking account, it is therefore better to check out the official [Offline Software page](http://english.ihep.cas.cn/bes/doc/2247.html "Offical BOSS webpage") of BESIII -- this framework can only be of use if you are a collaborator of this experiment and if you have access to the software of this collaboration. You can also have a look at the section [Further reading](#further-reading).

The MarkDown generated material on [this repository page](https://github.com/redeboer/BOSS_Afterburner) serves as a simple introduction. For more details, you can go to the [parallel Doxygen-generated webpage](https://redeboer.github.io/BOSS_Afterburner/ "Doxygen page of the BOSS Afterburner") of the BOSS Afterburner.

See also @ref tips ([MarkDown](docs/doxygen/tips.md)).

# Introduction
The [Beijing Spectrometer (BESIII)](http://bes3.ihep.ac.cn/) is a particle experiment situated at the [Beijing Electron-Positron Collider (BEPCII)](http://english.ihep.cas.cn/doc/1840.html). It is primarily designed to perform highly sensitive studies of light hadron and charm physics.

@todo Elaborate or refer to official pages

Output of the detector is analysed using the [BESIII Offline Software System (BOSS)](http://english.ihep.cas.cn/bes/doc/2247.html).


# The BOSS framework

## Organisation of the IHEP server
The IHEP server runs on [Scientific Linux CERN](https://linux.web.cern.ch/linux/scientific.shtml) (SLC). The server offers several versions. Usually, people use either SLC5, SLC6, or SLC7. The domain names for these are `lxslc7.ihep.ac.cn`, where the `7` in this case refers to SLC7. If you are running on Linux or a Linux terminal, the server can be easily accessed using:

	ssh -Y <your user name>@lxslc7.ihep.ac.cn

Here, the option `-Y` ensures *X11 forwarding*, allowing you to open graphical applications.

In Windows, there are some nice tools that allow you to access the server. First of all, to be able to use SSH, use will either have to use [PuTTY](https://www.putty.org/) or more extensive software like [Xmanager](https://www.netsarang.com/products/xmg_overview.html). You can also just search for some Linux terminals for Windows. In addition, I would recommend you to work with the (S)FTP client [WinSCP](https://winscp.net/eng/index.php). It allows you to easily navigate the file structure of the IHEP server and to quickly transfer---even synchronise---files up and down to your own computer.

When you have logged into the server, you usually start in your home (`~`) folder. Move to the root of the server (`cd /`) and you'll see that is a large number of other directories. A few of these directories contain space that is assigned to your user account. Here is an overview:

| Path                                   | Data quotum | Max. number of files  | Remark                  |
| -------------------------------------- |:-----------:|:---------------------:|:-----------------------:|
| `/afs/ihep.ac.cn/users/<letter>/$USER` |      500 MB |                    NA | home (`~`)              |
| `/besfs/users/$USER`                   |       50 GB |               300,000 |                         |
| `/ihepbatch/bes/$USER`                 |      200 MB |                    NA |                         |
| `/workfs/bes/$USER`                    |        5 GB |                50,000 | no `hep_sub` available  |
| `/scratchfs/bes/$USER`                 |      500 GB |                    NA | max. 2 weeks            |

Of course, here, and in what follows, `$USER` refers to your user name.

Some other important directories are the following:

- [BOSS Software directory](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/How_to_setup_BOSS_environment_on_lxslc)
	+ `/afs/ihep.ac.cn/bes3/offline/Boss` (also referred to with `$BesArea`)

- [Raw data files](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Raw_Data)
	+ `/bes3fs/offline/data/raw`
	+ `/besfs2/offline/data/raw` (only rounds 3 and 6)
	+ `/besfs/offline/data/randomtrg` (random trigger data)

- [Reconstructed data sets](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Production)
	+ `/besfs3/offline/data/`
	+ `/besfs2/offline/` (older versions)

These directories will be important later in this 'tutorial'.


## Setup of your BOSS and Afterburner environment
***Note:** In it's current version, this tutorial assumes you use a `bash` terminal.*

I advise you to set up your environment in the same way I did. There are two main directories that you will be using: (1) the *work area* that contains your run scripts for **BOSS** and (2) the *BOSS Afterburner* repository. I placed the work area in my `ihepbatch` folder (see [Organisation of the IHEP server](#organisation-of-the-ihep-server)) and the BOSS Afterburner in my `besfs` folder. Data generated with **BOSS** through the work area scripts is written then to the `BOSS_Afterburner/data` directory on `besfs` (see later).

### (1) Set up your work area
@todo Test the procedure described below!
@todo Rewrite for TC shell.

You will be running your **BOSS** analyses from this folder. It contains a *Configuration Management Tool* folder (`cmthome`), which is used to set up path variables for **BOSS**, and a `workarea` folder where you develop your own **BOSS** packages (including the `jobOptions*.txt` files).

**Step 1:** Go to your *IHEP batch* folder:

	cd /ihepbatch/bes/$USER

At this stage, you'll have to decide which version of BOSS you have to use. At the time of writing, version 7.0.3 is the latest stable version, though it could be that for your analysis you have to use data sets that were reconstructed with older versions of **BOSS**. Here, I'll just use `7.0.3`, but you can replace this number with whatever version you need. 

**Step 2:** We'll first set up the necessary references to **BOSS** using the *Configuration Management Tool* (CMT). This is done by copying the `cmthome` folder from BOSS Software directory to the `ihepbatch` folder where you currently are:

	cp -R /afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-7.0.3 .

and navigate into that copy:

	cd cmthome-7.0.3

**Step 3:** You'll now have to modify the file called `requirements` so that it handles your username properly. We'll use the `vi` editor here, but you can use whatever editor you prefer:

	vi requirements

The file contains the following lines:

	#macro WorkArea "/ihepbatch/bes/maqm/workarea"

	#path_remove CMTPATH "${WorkArea}"
	#path_prepend CMTPATH "${WorkArea}"

Uncomment them (remove the hash `#`) and replace `maqm` with your own user name. Like this:

	macro WorkArea "/ihepbatch/bes/$USER/workarea"

	path_remove CMTPATH "${WorkArea}"
	path_prepend CMTPATH "${WorkArea}"

**Step 4:** Now you can use the scripts to set all references to **BOSS** at once using:

	source setupCMT.sh  # starts connection to the CMT
	cmt config          # initiates configuration
	source setup.sh     # sets path variables

Just to be sure, you can check whether the path variables have been set correctly:

	echo $CMTPATH

If everything went well, it should print:

	/ihepbatch/bes/$USER/workarea-7.0.3:/afs/ihep.ac.cn/bes3/offline/Boss/7.0.3:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/gaudi/GAUDI_v23r9:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/LCGCMT/LCGCMT_65a

**Step 5:** Adapt your `bash` profile (`.bash_profile`) and *run commands* file (`.bashrc`) so that **BOSS** is loaded automatically every time you log into the server. The easy solution is simply copy-pasting and running these commands:

	echo -e "if test -f .bashrc; then\n\tsource .bashrc\nfi" >> ~/.bash_profile
	echo "IHEPBATCH=\"/ihepbatch/bes/$USER\"" >> ~/.bashrc
	echo "CMTHOME=\"/afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-7.0.3\"" >> ~/.bashrc
	echo "source $IHEPBATCH/cmthome-7.0.3/setupCMT.sh" >> ~/.bashrc
	echo "source $IHEPBATCH/cmthome-7.0.3/setup.sh" >> ~/.bashrc
	echo "source $IHEPBATCH/workarea-7.0.3/TestRelease/TestRelease-00-00-86/cmt/setup.sh" >> ~/.bashrc
	echo "export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/" >> ~/.bashrc

However, to avoid becoming a [copy ninja](https://pics.me.me/kakashi-went-from-the-copy-ninja-to-the-copy-paste-14969048.png "Kakashi programming"), you'd better modify these files yourself. First, add these lines to your bash profile (`vi ~/bash_profile`):

	if test -f .bashrc; then
		source .bashrc
	fi

These lines force the server to load your `.bashrc` run commands file. In that file, you should add the following lines:

	IHEPBATCH="/ihepbatch/bes/$USER"
	CMTHOME="/afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-7.0.3"
	source $IHEPBATCH/cmthome-7.0.3/setupCMT.sh
	source $IHEPBATCH/cmthome-7.0.3/setup.sh
	source $IHEPBATCH/workarea-7.0.3/TestRelease/TestRelease-00-00-86/cmt/setup.sh
	export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/

Notice that the commands we used in step 4 are used here again. There is also a reference to the `workarea` (see step 7). The last line allows you to submit **BOSS** jobs to the queue---for now, don't worry what this means.

You can now either log in again to the server or use `source ~/.bashrc` to reload the run commands.

**Step 6:** To test whether everything went correctly, you can try to run **BOSS**:

	boss.exe

It should result in a (trivial) error message like this:

	**************************************************
	               BOSS version: 7.0.3
	************** BESIII Collaboration **************

	the jobOptions file is : jobOptions.txt
	ERROR! the jobOptions file is empty!

**Step 7:** It is convention to place your **BOSS** packages in a *workarea* folder next to the `cmthome` folder we have been using so far. Let's create it:

	cd /ihepbatch/bes/$USER
	mkdir workarea-7.0.3

We'll get back to this folder when we [set up a BOSS package](#set-up-a-boss-package).

### (2) Set up the BOSS Afterburner
This is the simple part. Go to the *BES file system* folder:

	cd /besfs/users/$USER

[Clone](https://help.github.com/articles/cloning-a-repository/) the *BOSS Afterburner* repository:

	git clone https://github.com/redeboer/BOSS_Afterburner

The *BOSS Aafterburner* will be used to analyse output from **BOSS**.


## Set up a BOSS package
Now that you have configured your **BOSS** work area, you can start developing packages. The typical example package to get you started is the `TestRelease` package. We will need to copy this package into your [your work area](#set-up-your-work-area). So let's navigate there:

	cd /ihepbatch/bes/$USER/workarea-7.0.3

Now copy the typical `TestRelease` example to your *work area* and navigate into it:

	cp –r $BesArea/TestRelease ./
	cd TestRelease/TestRelease-*

You are now in a folder like `TestRelease/TestRelease-00-00-86`, where `00-00-86` represents the version. Using `ls`, you can see that it contains some folders:

- `cmt`: the *Configuration Management Tool* that you will use to connect to **BOSS**
- `CVS`: a folder used when generating documentation
- `run`: which contains the `jobOptions` that are run with `boss.exe`

We can set up the `TestRelease` by going into `cmt` and 'broadcasting' to **BOSS** from there:

	cd cmt
	cmt broadcast cmt config
	cmt broadcast make
	source setup.sh

This will initialise the package so that you can run it from the run folder. This is done using `boss.exe`:

	cd ../run
	boss.exe jobOptions_sim.txt

which, in this case, will run a Monte Carlo simulation.

Note that in step 7 [when we set up the work area](#set-up-your-work-area) we added a line to the `.bashrc` that ensures that the `TestRelease` package is laoded every time you log in, so you won't have to do this every time yourself.


## Running a job
Particile physicists perform analyses on either data from measurements or on data from Monte Carlo simulation. In **BOSS**, it is possible to generate your own Monte Carlo simulations and to treat its output as ordinary data. There are there for three basic steps in running a Monte Carlo job on **BOSS**:

1. `sim`: you perform a Monte Carlo simulation and generate a raw data file (`rtraw`)
2. `rec`: you reconstruct particle tracks from the raw data and write out a reconstructed data file (`dst`)
3. `ana`: you analyse the reconstructed tracks and generate a [CERN ROOT](https://root.cern.ch/input-and-output) file containing trees that describe event and track variables (`root`)

When you are analysing measurement data, you won't have to perform steps 1 and 2: the BESIII collaboration reconstructs all data samples whenever a new version of BOSS is released. (See [Organisation of the IHEP server](organisation-of-the-ihep-server), under "Reconstructed data sets", for where these files are located.)

The steps are performed from your the `jobOptions*.txt` files of your own package in your work area. As an example, you can run the `jobOptions` in in the `TestRelease` package:

	cd /ihepbatch/bes/$USER/TestRelease/TestRelease-*/run/
	boss.exe jobOptions_sim.txt
	boss.exe jobOptions_rec.txt
	boss.exe jobOptions_ana_rhopi.txt

But be careful: for these jobs to work successfully, you will probably have to edit them and set the right paths.


## Submitting a job
The `TestRelease` example typically simulates, reconstructes, and analyses only 10 events. For serious work, it you will have to generate thousands of events and this will take a long time. You can therefore submit your job to a so-called 'queque'. For this, there are two options: either you submit them using the command `hep_sub` or using the command `boss.conder`. The latter is easiest: you can use it just like `boss.exe`.

With `hep_sub`, however, you essentially forward a shell script to the queue, which then executes the commands in there. So you'll have to make that first. Let's say, you make a shell script `test.sh` in the `run` folder that looks like this:

	#!/bin/bash
	boss.exe jobOptions_sim.txt

The first line clarifies that you use `bash`, the second does what you did when running a job: calling `boss.exe`, but of course, you can make this script to execute whatever `bash` commands you want.

@todo Not sure if you should `chmod +x` it?

Now you can simply submit the shell script to the queue using:

	hep_sub test.sh -g physics

and it will be executed in the background. Here, the option `-g` tellst that you are from the `physics` group. You can check whether the job is (still) running in the queue using:

	hep_q -u $USER

Note that `hep_q` would list all jobs from all users.


## Splitting up jobs
Jobs that take a long time to be executed in the queue will be killed by the server. It is therefore recommended that you analyse a maximum of **10,000 events** per job, particularly if you perform Monte Carlo simulations. Of course, you will be wanting to work with much larger data samples, sou you will have to submit parallel jobs. This can be done by writing different `jobOptions*.txt` files, where you modify the input/output file and random seed number.

What is much more convenient, however, is developing a `jobOptions*.txt` template file that is used to generate. In these, you for instance replace the specific paths and seed number you used by generic tokens like `INPUTFILE`, `OUTPUTFILE`, and `RANDOMSEED`. You can then use a `bash` script (or comparable shell scripts) to replace these tokens by unique paths and seed numbers. For example, have a look at the [`awk`](https://www.tldp.org/LDP/abs/html/awk.html) command.



# Further reading

## IHEP and the BESIII collaboration
- BESIII:
	- http://bes3.ihep.ac.cn/
	- http://bes.ihep.ac.cn/

- IHEP: 
	- http://english.ihep.cas.cn/ [English]
	- http://www.ihep.cas.cn/ [Chinese]
	- http://web.ihep.ac.cn/

- http://english.ihep.cas.cn/doc/259.html


## The BOSS Analysis Framework
- BOSS software source code CVS repository:
	- [for users inside IHEP](http://koala.ihep.ac.cn/cgi-bin/viewcvs.cgi/BossCvs/)
	- [for users outside IHEP](http://docbes3.ihep.ac.cn/viewvc/cgi-bin/viewvc.cgi/BESIII/BossCvs/)

- Doxygen documentation:
	- http://bes3.to.infn.it/Boss/7.0.2/html/classes.html (external)
	- https://boss.ihep.ac.cn/~offlinesoftware/MdcPatRecDoc04/classes.html (`MdcPatRec` Class Index)

- http://twiki.ihep.ac.cn/twiki/view/BES/BOSS/WebHome (TWiki)
- https://indico.cern.ch/event/408139/contributions/979815/attachments/815741/1117758/CHEP06-Weidong_Li.pdf


# About
From October 2018 to May 2019, I will be doing my master research at the BESIII Collaboration as a visiting master student from Utrecht University (The Netherlands). Because I, like many others, will spend a lot of time getting used to the **BOSS** framework, I decided to supply the code I write for my own analysis with `Doxygen` formated comments. The resulting documentation can then be used as a guide for anyone who wants to get familiar with the **BOSS** framework.

In writing my code, I have attempted to set up an object-oriented `C++` class structure that is both general in usage (that is, using *base and derived classes*) and represents the data structure of analysis output from the **BOSS** framework. In this way, I hope that the class documentation in this repository can also be used to help the user understand the **BOSS** framework itself.
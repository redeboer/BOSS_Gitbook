The BOSS Afterburner {#mainpage}
================================

**This repository is underdevelopment.** It has been set up as an *unofficial* extension to the BOSS analysis framework of the [BESIII collaboration](http://bes3.ihep.ac.cn "Offical BESIII webpage") with two related purposes:

1. Serve as an accessible introduction and user guide to **BOSS**.
2. Provide basic tools to analyse and visualise output of **BOSS**.

If you do not have an IHEP networking account, it is therefore better to check out the official [Offline Software page](http://english.ihep.cas.cn/bes/doc/2247.html "Offical BOSS webpage") of BESIII --- this framework can only be of use if you are a collaborator of this experiment and if you have access to the software of this collaboration. You can also have a look at the section [Further reading](#further-reading).

The MarkDown generated material on this repository page serve as a simple introduction. For more details, you can go to the [parallel Doxygen generated webpage](https://redeboer.github.io/BOSS_Afterburner/ "Doxygen page of the BOSS Afterburner") of the BOSS Afterburner.


Table of contents
=================
- [Introduction](#introduction)
- [The BOSS framework](#the-boss-framework)
	- [Organisation of the IHEP server](#organisation-of-the-ihep-server)
	- [Setup of your BOSS and Afterburner environment](#setup-of-your-boss-and-afterburner-environment)
		- [(1) Your work area](#1-your-work-area)
		- [(2) BOSS Afterburner](#2-boss-afterburner)
	- [Set up a BOSS package](#set-up-a-boss-package)
- [Further reading](#further-reading)
	- [The BOSS Analysis Framework](#the-boss-analysis-framework)
- [About](#about)


# Introduction

The [Beijing Spectrometer (BESIII)](http://bes3.ihep.ac.cn/) is a particle experiment situated at the [Beijing Electron-Positron Collider (BEPCII)](http://english.ihep.cas.cn/doc/1840.html). It is primarily designed to perform highly sensitive studies of light hadron and charm physics.

@todo Elaborate or refer

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
| `/afs/ihep.ac.cn/users/<u>/<username>` |      500 MB |                    NA | home (`~`)              |
| `/besfs/users/<username>`              |       50 GB |               300,000 |                         |
| `/ihepbatch/bes/<username>`            |      200 MB |                    NA |                         |
| `/workfs/bes/<username>`               |        5 GB |                50,000 | no `hep_sup` available  |
| `/scratchfs/bes/<username>`            |      500 GB |                    NA | max. 2 weeks            |

Some other important directories are the following:

- [BOSS Software directory](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/How_to_setup_BOSS_environment_on_lxslc)

  `/afs/ihep.ac.cn/bes3/offline/Boss`

- [Raw data files](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Raw_Data)

  `/bes3fs/offline/data/raw`
  `/besfs2/offline/data/raw` (only rounds 3 and 6)
  `/besfs/offline/data/randomtrg` (random trigger data)

- [Reconstructed data sets](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Production)

  `/besfs3/offline/data/`
  `/besfs2/offline/` (older versions)

These directories will be important later in this 'tutorial'.


## Setup of your BOSS and Afterburner environment
I advise you to set up your environment in the same way I did. There are two main directories that you will be using: (1) the **work area** that contains your run scripts for **BOSS** and (2) the *BOSS Afterburner* repository.

### (1) Your work area

Go to your *IHEP batch* folder:

	cd /ihepbatch/bes/<your username>

Now, create a **workarea** directory and a 'CMT home' directory, where you add the version number of the **BOSS** version you want to use to the directory name. You can for instance take `<BOSS version>` to be `7.0.3`:

	mkdir cmthome-<BOSS version>
	mkdir workarea-<BOSS version>

@todo Finish tutorial about setting up your BOSS environment (e.g. cmt home)

### (2) BOSS Afterburner
Go to the *BES file system* folder:

	cd /besfs/users/<your username>

[Clone](https://help.github.com/articles/cloning-a-repository/) the *BOSS Afterburner* repository:

	git clone https://github.com/redeboer/BOSS_Afterburner

Now you're all set to go!

## Set up a BOSS package
The typical example that is used as a starting point in **BOSS** is the `TestRelease` package. We will need to copy this package into your work area.

First go to [your work area](#your-work-area):

	cd /ihepbatch/bes/<your username>



	cp –r $BesArea/TestRelease ./

1. `cd TestRelease/TestRelease-*/cmt`

2. `cmt broadcast cmt config`

3. `cmt broadcast make`

4. `source setup.csh`

# Further reading

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
From October 2018 to May 2019, I will be doing my master research at the BESIII Collaboration as a visiting master student from Utrecht University (The Netherlands). Because I, like many others, will spend a lot of time getting used to the **BOSS** framework, I decided to supply the code I write for my own analysis with `Doxygen` formated comments. The resulting documentation can then be used as a gudie for anyone who wants to get familiar with the **BOSS** framework.

In writing my code, I have attempted to set up an object-oriented `C++` class structure that is both general in usage (that is, using *base and derived classes*) and represents the data structure of analysis output from the **BOSS** framework. In this way, I hope that class documentation in this repository can also be used to help the user understand the **BOSS** framework itself.
<!-- @todo Elaborate acknowledgements
- Prof. Shen Xiaoyan
- Cao Ning
- Ma Runqiu -->
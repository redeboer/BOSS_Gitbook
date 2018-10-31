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
	- [Setup a BOSS package](#setup-a-boss-package)
- [Technicalities](#technicalities)
- [About](#about)
- [Further reading](#further-reading)
	- [The BOSS Analysis Framework](#the-boss-analysis-framework)


# Introduction



# The BOSS framework

## Organisation of the IHEP server
The IHEP server runs on [Scientific Linux](https://www.scientificlinux.org/) (SL). The server offers several versions. Usually, people use either SL5, SL6, or SL7. The domain names for these are `lxslc7.ihep.ac.cn`, where the `7` in this case refers to SL7. If you are running on Linux or a Linux terminal, the server can be easily accessed using:

	ssh -Y <your user name>@lxslc7.ihep.ac.cn

Here, the option `-Y` ensures *X11 forwarding*, allowing you to open graphical applications.

In Windows, there are some nice tools that allow you to access the server. First of all, to be able to use SSH, use will either have to use [PuTTY](https://www.putty.org/) or more extensive software like [Xmanager](https://www.netsarang.com/products/xmg_overview.html). You can also just search for some Linux terminals for Windows. In addition, I would recommend you to work with the (S)FTP client [WinSCP](https://winscp.net/eng/index.php). It allows you to easily navigate the file structure of the IHEP server and to quickly transfer---even synchronise---files up and down to your own computer.


## Setup of your BOSS and Afterburner environment
I advise you to set up your environment in the same way I did. There are two main directories that you will be using: (1) the **work area** that contains your run scripts for **BOSS** and (2) the *BOSS Afterburner* repository.

### (1) Your work area

Go to your *IHEP batch* folder:

	cd /ihepbatch/bes/<your username>

Now, create a **workarea** directory and a 'CMT home' directory, where you add the version number of the **BOSS** version you want to use to the directory name. You can for instance take `<BOSS version>` to be `7.0.3`:

	mkdir cmthome-<BOSS version>
	mkdir workarea-<BOSS version>

@todo Finish tutorial about setting up your BOSS environment (e.g. cmt home)

| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |

### (2) BOSS Afterburner
Go to the *BES file system* folder:

	cd /besfs/users/<your username>

[Clone](https://help.github.com/articles/cloning-a-repository/) the *BOSS Afterburner* repository:

	git clone https://github.com/redeboer/BOSS_Afterburner

Now you're all set to go!

## Setup a BOSS package
The typical example that is used as a starting point in **BOSS** is the `TestRelease` package. We will need to copy this package into your work area.

First go to [your work area](#your-work-area):
	cd /ihepbatch/bes/<your username>



	cp –r $BesArea/TestRelease ./

3. `cd TestRelease/TestRelease-*/cmt`

4. `cmt broadcast cmt config`

5. `cmt broadcast make`

6. `source setup.csh`


# Technicalities

# About
From October 2018 to May 2019, I will be doing my master research at the BESIII Collaboration as a visiting master student from Utrecht University (The Netherlands). Because I, like many others, will spend a lot of time getting used to the **BOSS** framework, I decided to supply the code I write for my own analysis with `Doxygen` formated comments. The resulting documentation can then be used as a gudie for anyone who wants to get familiar with the **BOSS** framework.

In writing my code, I have attempted to set up an object-oriented `C++` class structure that is both general in usage (that is, using *base and derived classes*) and represents the data structure of analysis output from the **BOSS** framework. In this way, I hope that class documentation in this repository can also be used to help the user understand the **BOSS** framework itself.
<!-- @todo Elaborate acknowledgements
- Prof. Shen Xiaoyan
- Cao Ning
- Ma Runqiu -->

# Further reading

## The BOSS Analysis Framework
- http://bes3.to.infn.it/Boss/7.0.2/html/classes.html (external)
- https://docbes3.ihep.ac.cn/viewvc/cgi-bin/viewvc.cgi/BESIII/BossCvs/ (internal)
- https://boss.ihep.ac.cn/~offlinesoftware/MdcPatRecDoc04/classes.html (`MdcPatRec` Class Index)
- http://twiki.ihep.ac.cn/twiki/view/BES/BOSS/WebHome (TWiki)
- https://indico.cern.ch/event/408139/contributions/979815/attachments/815741/1117758/CHEP06-Weidong_Li.pdf\
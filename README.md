# Main Page {#mainpage}

**This repository is underdevelopment.** It has been set up as an *unofficial* extension to the BOSS analysis framework of the [BESIII collaboration](http://bes3.ihep.ac.cn "Offical BESIII webpage") with two related purposes:

1. Serve as an accessible introduction and user guide to the `BOSS` framework.
2. Provide basic tools to analyse output of `BOSS` and generate plots.

If you do not have an IHEP networking account, it is therefore better to check out the official [Offline Software page](http://english.ihep.cas.cn/bes/doc/2247.html "Offical BOSS webpage") of BESIII -- this framework can only be of use if you are a collaborator of this experiment and if you have access to the software of this collaboration. You can also have a look at the section [Further reading](#further-reading).


## Table of contents
- [Main Page {#mainpage}](#main-page-mainpage)
	- [Table of contents](#table-of-contents)
- [Introduction](#introduction)
- [The BOSS framework](#the-boss-framework)
	- [Organisation of the IHEP server](#organisation-of-the-ihep-server)
	- [Setup of your environment](#setup-of-your-environment)
	- [Setup a BOSS package](#setup-a-boss-package)
- [Technicalities](#technicalities)
- [About](#about)
- [Further reading](#further-reading)
	- [The `BOSS` Analysis Framework](#the-boss-analysis-framework)


# Introduction


# The BOSS framework

## Organisation of the IHEP server
The IHEP server runs on Scientific Linux (SL). There are several versions available that can be accessed simultaneously. Usually, people use either SL5, SL6, or SL7.


## Setup of your environment
I advise you to set up your environment in the same way I did. There are two main directories that you will be using: (1) the **work area** that  that 

1. Your **Work area** Place your `BOSS` packages in `/ihepbatch/bes/<your username>`.
- Place the `BOSS_Afterburner` in `/besfs/users/<your username>`.

## Setup a BOSS package
The typical example that is used as a starting point in `BOSS` is the `TestRelease` package. We will need to copy this package into your work area.
1. :(同一个节点下) cd 到上一页你创建的workarea路径下`
3. `cp –r $BesArea/TestRelease ./`
4. `cd TestRelease/TestRelease-*/cmt`
5. `cmt broadcast cmt config`
6. `cmt broadcast make`
7. `source setup.csh`


# Technicalities

# About
From October 2018 to May 2019, I will be doing my master research at the BESIII Collaboration as a visiting master student from Utrecht University (The Netherlands). Because I, like many others, will spend a lot of time getting used to the `BOSS` framework, I decided to supply the code I write for my own analysis with `Doxygen` formated comments. The resulting documentation can then be used as a gudie for anyone who wants to get familiar with the `BOSS` framework.

In writing my code, I have attempted to set up an object-oriented `C++` class structure that is both general in usage (that is, using *base and derived classes*) and represents the data structure of analysis output from the `BOSS` framework. In this way, I hope that class documentation in this repository can also be used to help the user understand the `BOSS` framework itself.
<!-- @todo Elaborate acknowledgements
- Prof. Shen Xiaoyan
- Cao Ning
- Ma Runqiu -->

# Further reading

## The `BOSS` Analysis Framework
- http://bes3.to.infn.it/Boss/7.0.2/html/classes.html (external)
- https://docbes3.ihep.ac.cn/viewvc/cgi-bin/viewvc.cgi/BESIII/BossCvs/ (internal)
- https://boss.ihep.ac.cn/~offlinesoftware/MdcPatRecDoc04/classes.html (`MdcPatRec` Class Index)
- http://twiki.ihep.ac.cn/twiki/view/BES/BOSS/WebHome (TWiki)
- https://indico.cern.ch/event/408139/contributions/979815/attachments/815741/1117758/CHEP06-Weidong_Li.pdf\
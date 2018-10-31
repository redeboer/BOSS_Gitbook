# The BOSS Framework {#boss}

## Introduction

### Organisation of the IHEP server
The IHEP server runs on Scientific Linux (SL). There are several versions available that can be accessed simultaneously. Usually, people use either SL5, SL6, or SL7.


### Setup of your environment
I advise you to set up your environment in the same way I did. There are two main directories that you will be using: (1) the **work area** that  that 

1. Your **Work area** Place your `BOSS` packages in `/ihepbatch/bes/<your username>`.
- Place the `BOSS_Afterburner` in `/besfs/users/<your username>`.

### Setup a BOSS package
The typical example that is used as a starting point in `BOSS` is the `TestRelease` package. We will need to copy this package into your work area.
1. :(同一个节点下) cd 到上一页你创建的workarea路径下`
3. `cp –r $BesArea/TestRelease ./`
4. `cd TestRelease/TestRelease-*/cmt`
5. `cmt broadcast cmt config`
6. `cmt broadcast make`
7. `source setup.csh`
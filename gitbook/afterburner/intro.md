# Introduction

_The BOSS Afterburner_ repository is meant as an extension of the conventional BOSS analysis framework. It contains the tutorial pages you are currently looking at, but also comes with two major components that are to facilitate event selection and analysis.

First of all, there is the _base-derived algorithm structure_. This framework is an advanced version of the `Rhopi` algorithm: a framework for [**initial event selection**](initial/).

Like `RhopiAlg`, output of this initial event selection is a ROOT file containing a set of `TTree`s. Usually, you design your own scripts from scratch to load, apply additional cuts, plot, and then fit the data in there. This is where the second part of this repository comes in: the framework for [**final event selection**](final/), or the "Afterburner".

In the following pages, we will will go through these two components, explaining not only the usage, but also the motivation for its design. We will end with a note on contributing to this framework, because one of the main aims of this setup is **to facilitate collaboration on analysis code within BESIII**.

{% hint style="danger" %}
**This part of the tutorial is not yet finished.**
{% endhint %}

## Temporary notes

### Outdated group structure

![Initial class structure of the BOSS Afterburner](../.gitbook/assets/boss_afterburner.png)

### Generic event selection packages

{% hint style="warning" %}
**@todo** Describe `FSFilter` by BESIII.
{% endhint %}


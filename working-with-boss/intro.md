# The BOSS Analysis Framework

{% hint style="warning" %}
**This tutorial is still being developed.**
{% endhint %}

@todo Write introduction to the [BESIII Offline Software System \(BOSS\)](http://english.ihep.cas.cn/bes/doc/2247.html).

* Built on Gaudi \(which in turn was developed by LHCb\)

## Configuration Management Tool \(CMT\)

The BOSS analysis framework is organised according to the so-called "package-oriented principle". The underlying idea is that in a software framework that is developed by several people is best split up into several packages that are developed independently or in subgroups.

The task of the CMT is to streamline these package, that is, to name them automatically based on content and modifications and to connect the packages to each other. CMT additionally allows users to ascribe properties to the packages and their constituents.

See for more information:

* [Official website of CMT](http://www.cmtsite.net/) \(partially French\)
* [LHCb on CMT](https://lhcb-comp.web.cern.ch/lhcb-comp/support/CMT/cmt.htm) \(historical page\)
* [A presentation on BESIII Analysis by Shen Chengping](http://www.hep.umn.edu/bes3/MN_BES3_files/BESIII_intro_shencp.pdf): Includes \(somewhat outdated\) instructions on package management

## Concurrent Versions System \(CVS\)

Packages and source code of BOSS are monitored and maintained by CVS. This is a somewhat outdated revision control system comparable to Subversion and Git.

More information:

* [Wikipedia on CVS](https://en.wikipedia.org/wiki/Concurrent_Versions_System)


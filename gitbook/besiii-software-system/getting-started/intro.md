# The BOSS Analysis Framework

## What is BOSS?

{% hint style="warning" %}
**@todo** Write introduction to the [BESIII Offline Software System \(BOSS\)](http://english.ihep.cas.cn/bes/doc/2247.html).
{% endhint %}

* Built on [Gaudi](https://dayabay.bnl.gov/dox/GaudiKernel/html/annotated.html) \(which in turn was developed by LHCb\)

## Configuration Management Tool \(CMT\)

The BOSS analysis framework is organised according to the so-called "package-oriented principle". The underlying idea is that in a software framework that is developed by several people is best split up into several packages that are each developed independently or in subgroups.

The task of the CMT is to streamline and checkout different versions of these packages, that is, [to name them automatically based on content and modifications and to connect the packages to each other](http://polywww.in2p3.fr/activites/physique/glast/workbook/pages/cmtMRvcmt/defCMTpackage.htm) \(to manage dependencies\). This is done in conjunction with CVS \([see below](intro.md#concurrent-versions-system-cvs)\). CMT additionally allows users to ascribe properties to the packages and their constituents.

See for more information:

* [An accessible introduction to CMT](http://polywww.in2p3.fr/activites/physique/glast/workbook/pages/cmtMRvcmt/cmtIntroduction.htm)
* [Official website of CMT](http://www.cmtsite.net/) \(partially French\)
* [LHCb on CMT](https://lhcb-comp.web.cern.ch/lhcb-comp/support/CMT/cmt.htm) \(historical page\)
* [A presentation on BESIII Analysis by Shen Chengping](http://www.hep.umn.edu/bes3/MN_BES3_files/BESIII_intro_shencp.pdf): Includes \(somewhat outdated\) instructions on package management

## Concurrent Versions System \(CVS\)

Packages and source code of BOSS are monitored and maintained by CVS. This is a revision control system comparable to Subversion and Git.

More information:

* [An accessible introduction to CVS](http://polywww.in2p3.fr/activites/physique/glast/workbook/pages/softwareOverview/cvsBasics.htm)
* [Commonly used CVS commands](http://polywww.in2p3.fr/activites/physique/glast/workbook/pages/softwareOverview/commonCVScommands.htm)
* [Wikipedia on CVS](https://en.wikipedia.org/wiki/Concurrent_Versions_System)

{% hint style="info" %}
CVS can be used as a tool for collaboration, but is somewhat outdated. This is [one of the motivations](../../the-boss-afterburner/intro.md#motivation) for designing the BOSS Afterburner framework.
{% endhint %}


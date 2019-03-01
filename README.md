# The BOSS Gitbook

{% hint style="danger" %}
**These pages are currently being reorganised**

The _BOSS Gitbook_ that you are currently looking only is to contain tutorial pages on BOSS and its official packages.

For pages on The BOSS Afterburner and its analysis framework see [_The BOSS Afterburner_ Gitbook](https://redeboer.gitbook.io/boss_afterburner).
{% endhint %}

{% hint style="warning" %}
Feedback on these pages is very welcome! See [About](appendices/about.md) for contact details.
{% endhint %}

## Contents of these

This repository has been set up as an unofficial extension to the BOSS analysis framework of the [BESIII collaboration](http://bes3.ihep.ac.cn). The repository has three major components:

1. The [tutorial pages on GitBook](https://besiii.gitbook.io/boss) that you are currently looking at. These pages are to serve as an accessible introduction and user guide to the BOSS software framework used in the BESIII collaboration.
2. A [base-derived algorithm structure for the **initial event selection**](https://redeboer.github.io/BOSS_Afterburner/group__BOSS.html). This part of the repository is integrated in BOSS and is designed to facilitate collaboration on code for initial event selection in hadron research at BESIII.
3. An [analysis framework for **final event selection**](https://redeboer.github.io/BOSS_Afterburner/group__BOSS__Afterburner.html). This part of the repository is designed to formalise and standardise loading, plotting, and fitting procedures that you perform on the output of the initial event selection.

The source code for all of the above is [available on GitHub](https://github.com/redeboer/BOSS_Afterburner) and can be contributed to through `git`.

{% hint style="info" %}
If you do not have an IHEP networking account, it is better to check out the official [Offline Software page](http://english.ihep.cas.cn/bes/doc/2247.html) of BESIII — this framework can only be of use if you are a member of the BESIII collaboration and if you have access to the software of this collaboration. You can also have a look at the links in the section [Further reading](appendices/references.md).
{% endhint %}

## Accessing _The BOSS Afterburner_

There are three channels through which you can access _The BOSS Afterburner_ online.

1. **GitBook** \([besiii.gitbook.io/boss](https://besiii.gitbook.io/boss)\), which provides an accessible graphical interface for the tutorial-like pages on using BOSS, on the initial event selection packages, and on the final event selection framework in this repository.
2. **GitPages** \([redeboer.github.io/BOSS\_Afterburner](https://redeboer.github.io/BOSS_Afterburner/)\), which contains class documentation for the initial and final event selection frameworks of _The BOSS Afterburner_. This platform can be consulted for more details about the design of these two frameworks.
3. **GitHub** \([github.com/redeboer/BOSS\_Afterburner](https://github.com/redeboer/BOSS_Afterburner)\), which is the complete repository itself. It contains the source code \(Markdown, C++, HTML, bash, etc.\) for all components. Collaboration on all of the above is possible through this platform, using `git`.

More briefly put, GitHub 'feeds' the content on GitBook and GitPages.

![Means of accessing The BOSS Afterburner](.gitbook/assets/boss_repository-1.png)


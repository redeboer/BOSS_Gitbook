# Main page

**This repository is still under development!**
Feedback is very welcome, see [here](https://besiii.gitbook.io/boss-gitbook/appendices/about) for contact details.

## _The BOSS Afterburner_ repository

This repository has been set up as an unofficial extension to the BOSS analysis framework of the [BESIII collaboration](http://bes3.ihep.ac.cn). The repository has three major components:

1. The [tutorial pages on GitBook](https://besiii.gitbook.io/boss-gitbook) that you are currently looking at. These pages are to serve as an accessible introduction and user guide to the BOSS software framework used in the BESIII collaboration.
2. A [base-derived algorithm structure for the **initial event selection**](https://redeboer.github.io/BOSS_Afterburner/group__BOSS.html). This part of the repository is integrated in BOSS and is designed to facilitate collaboration on code for initial event selection in hadron research at BESIII.
3. An [analysis framework for **final event selection**](https://redeboer.github.io/BOSS_Afterburner/group__BOSS__Afterburner.html). This part of the repository is designed to formalise and standardise loading, plotting, and fitting procedures that you perform on the output of the initial event selection.

The source code for all of the above is [available on GitHub](https://github.com/redeboer/BOSS_Afterburner) and can be contributed to through `git`.

## Accessing _The BOSS Afterburner_

There are three channels through which you can access _The BOSS Afterburner_ online.

1. **GitBook** \([besiii.gitbook.io/boss-gitbook](https://besiii.gitbook.io/boss-gitbook)\), which provides an accessible graphical interface for the tutorial-like pages on using BOSS, on the initial event selection packages, and on the final event selection framework in this repository.
2. **GitPages** \([redeboer.github.io/BOSS\_Afterburner](https://redeboer.github.io/BOSS_Afterburner/)\), which contains class documentation for the initial and final event selection frameworks of _The BOSS Afterburner_. This platform can be consulted for more details about the design of these two frameworks.
3. **GitHub** \([github.com/redeboer/BOSS\_Afterburner](https://github.com/redeboer/BOSS_Afterburner)\), which is the complete repository itself. It contains the source code \(Markdown, C++, HTML, bash, etc.\) for all components. Collaboration on all of the above is possible through this platform, using `git`.

More briefly put, GitHub 'feeds' the content on GitBook and GitPages.

![Means of accessing The BOSS Afterburner](gitbook/.gitbook/assets/boss_repository.png)

# Motivation

{% hint style="danger" %}
**The `TrackSelector` package and derived packages are still being developed and tested.**

Tutorial pages are not yet available, but you can meanwhile have a look at the class descriptions:  
[**https://redeboer.github.io/BOSS\_Afterburner/hierarchy.html**](https://redeboer.github.io/BOSS_Afterburner/hierarchy.html)\*\*\*\*
{% endhint %}

The BOSS Afterburner comes with event selection packages that follow an object-oriented design: all functionality that is shared by all \(light hadron\) analyses are shared through the [`TrackSelector`](https://redeboer.github.io/BOSS_Afterburner/classTrackSelector.html) base algorithm, while the user is to further specialise the analysis in derived algorithms \(such as [`D0phi_KpiKK`](https://redeboer.github.io/BOSS_Afterburner/classD0phi__KpiKK.html)\).

Some aims of this setup:

* Make event selection easier to understand for beginners. They should not have to reinvent the wheel.
* Improve readability of code for event selection packages. This is achieved through \(1\) encapsulating parts of the algorithm in class methods and \(2\) following strict rules for code comments in Doxygen formatting. In this way, event selections become more accessible \(both through Doxygen pages and the structure code\) for readers or reviewers who are not familiar with the aims and procedures of certain event selection packages.
* Provide a platform for collaborating on code, while still doing one's own specific research. The user is has the freedom and flexibility to design the event selection to one's own needs \(in the derived algorithms\), but is at the same time continuously testing the functionality of the base classes.
* \(Hopefully in the future:\) Standardized output and debugging tests that are to make the analysis results more reliable.

{% hint style="warning" %}
**@todo** Write tutorial page on usage
{% endhint %}


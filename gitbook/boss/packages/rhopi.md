# RhopiAlg

## Introduction

One of the basic analysis packages that is already provided in BOSS is the `RhopiAlg` package. Within BESIII, almost everyone knows it, because it is used as the starting point for developing your own packages. `RhopiAlg` is an illustration of a typical procedure in particle physics: reconstructing a decayed particle. For this, you will have to make apply cuts on measured parameters and this package is an illustration of this procedure.

The `RhopiAlg` analyses the decay of the $$\rho(770) $$ meson. As you can see in the in the [PDG listing for this meson](http://pdg.lbl.gov/2018/listings/rpp2018-list-rho-770.pdf), the $$\rho(770)$$ meson predominantly decays through $$\rho\rightarrow\pi\pi$$ \(almost 100%\), whether it concerns a $$\rho^+$$ , $$\rho^0$$ , or $$\rho^-$$. This means that we can reconstruct this meson purely through this _2-particle decay mode_.

Additionally, when we consider the charged $$\rho^\pm$$ mesons, one of the decay products is the neutral pion: $$\rho^\pm \rightarrow \pi^\pm\pi^0$$. This meson is again neutral and cannot be detected, so has to be reconstructed. But here again, there is one dominant decay mode: $$\pi^0 \rightarrow \gamma\gamma$$ \($$98.823 \pm 0.034 \%$$, see [its PDG listing](http://pdg.lbl.gov/2018/listings/rpp2018-list-pi-zero.pdf)\). This means that we can reconstruct the $$\rho^\pm$$ meson almost exclusively through its $$\rho^\pm \rightarrow \pi^\pm\pi^0 \rightarrow \pi^\pm\gamma\gamma$$ decay channel.

In reconstructing $$\pi^0$$ and $$\rho^0$$, you will run into another common phenomenon in hadron research: the width of the decaying particle. The width of $$\rho^0$$ is much wider than $$\pi^0$$ and therefore results in interesting differences in the eventual invariant mass spectra. In the final event selection, you will for instance see that a fit of the invariant mass peaks results in different widths.

## Where to find it?

The original `RhopiAlg` package \(version `0.0.23`\) is located here,

```text
/afs/ihep.ac.cn/bes3/offline/Boss/$BOSSVERSION/Analysis/Physics/RhopiAlg/RhopiAlg-00-00-23
```

You can also [have a look at it in the BOSS Afterburner](https://github.com/redeboer/BOSS_Afterburner/tree/master/boss/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-00-00-23) or [import the original packages](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/import_packages.sh) for any version of BOSS.

A better commented and expanded version is also available within the BOSS Afterburner \([version 1.0.0](https://github.com/redeboer/BOSS_Afterburner/tree/master/boss/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-01-00-00)\). In this tutorial, we will work with this version, but, as with `TestRelease`, you can choose to work with a copy from the `$BesArea`.

The `RhopiAlg` package is one of the dependencies of `TestRelease`: you actually use `TestRelease` to run an analysis from `RhopiAlg` \(or any other package\). This also means that you have to follow the normal procedure for updating a package [as described above](https://besiii.gitbook.io/boss-gitbook/~/drafts/-LTf4GgC5cleb1Fpaf0c/primary/docs-boss/setup-package#updating-a-package).

## Declaring and defining properties like cuts

See [header `.h` file](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-01-00-00/RhopiAlg/Rhopi.h) for declarations and [source `.cxx` code](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-01-00-00/src/Rhopi.cxx) for definitions of cuts.

## Determining vertex position

## Writing properties

## Looping over charged and neutral tracks

## Kalman kinematic $$n$$-constraints fit procedure

* `fit4c` refers to the 4-constraints of the center-of-mass system, namely the 4-momentum of the system \(collision energy and total 3-momentum\). You can use this fit for the reconstruction of $$\pi^0 \rightarrow \gamma\gamma$$. **Note that the** $$\chi_\mathrm{red}^2$$ **of the fit is the same for any combination, as the four constraints are the same in each event.**
* `fit5c` refers to the 5-constraint reconstruction of $$\rho^\pm \rightarrow \pi^\pm\pi^0 \rightarrow \pi^\pm\gamma\gamma$$.

## Cut flow

## Output `root` file

{% hint style="warning" %}
**@todo** Describe `RhopiAlg` and how to work with it.
{% endhint %}


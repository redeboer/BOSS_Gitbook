# RhopiAlg

See full [Doxygen documentation](https://redeboer.github.io/BOSS_IniSelect/classRhopiAlg.html) on GitPages.

## What does this example package teach?

The `RhopiAlg` is the starting point for beginners using BOSS. It teaches:

* The usual Gaudi [`Algorithm`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html) structure of the [`initialize`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html#ab889608fa1b738d0dbfef7751e8598ae), [`execute`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classIAlgorithm.html#a751b04dd35a3877d8799fcd07d0a6892), and [`finalize`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html#a9ffbd123ceb6c35e7c0344428d911fdc) steps.
* The use of logging using [`MsgStream`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classMsgStream.html).
* Declaring and booking [`NTuple::Tuple`s](https://dayabay.bnl.gov/dox/GaudiKernel/html/classNTuple_1_1Tuple.html) \(the eventual `TTree`s\) and adding items \(the eventual branches\) using [`NTuple::Tuple::addItem`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classNTuple_1_1Tuple.html#a78033967fbd89f7f18e7d6d7d43f41ac).
* Accessing data of charged tracks and neutral tracks in the using data from [`EvtRecEvent`](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecEvent.html) and [`EvtRecTrack`](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html) classes.
* Identifying particles \(PID\) using the [ParticleID](http://bes3.to.infn.it/Boss/7.0.2/html/classParticleID.html) class.
* Making a selection of these tracks \(using iterators\) over which you loop again later.
* Applying a Kalman kinematic fit with [constraints](http://bes3.to.infn.it/Boss/7.0.2/html/classTrackPool.html#5ecaf22a24d60b2979d0ccd3b0c1df10) and a [resonance](http://bes3.to.infn.it/Boss/7.0.2/html/classKalmanKinematicFit.html#1163bb8ab7e0ebc53c81b2a4d840ebb0) using [`KalmanKinematicFit`](http://bes3.to.infn.it/Boss/7.0.2/html/classKalmanKinematicFit.html).
* Computing invariant masses using [`HepLorentzVector`](https://www-zeuthen.desy.de/geant4/clhep-2.0.4.3/classCLHEP_1_1HepLorentzVector.html) from the CLHEP library.
* Computing the angle between a photon and a pion.
* Reconstructed data from the detectors is accessed through the classes in the below table. This package only makes use of the MDC, EMC, and TOF detectors.

| Detector |  | Class | Accessed through |
| :--- | :--- | :--- | :--- |
| **MDC** | **Main Drift Chamber** | **\`\`**[**`RecMdcTrack`**](http://bes3.to.infn.it/Boss/7.0.2/html/classRecMdcTrack.html)**\`\`** | **\`\`**[**`EvtRecTrack::mdcTrack`**](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html#b20b980cd2f97e76870d85310f3701a9)**\`\`** |
| **MDC** | \*\*\*\*$$dE/dx$$ **info** | **\`\`**[**`RecMdcDedx`**](http://bes3.to.infn.it/Boss/7.0.2/html/classRecMdcDedx.html)**\`\`** | **\`\`**[**`EvtRecTrack::mdcDedx`**](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html#946473c9e8d949a44e1254f70014ce6e)**\`\`** |
| **MDC** | **Kalman track** | **\`\`**[**`RecMdcKalTrack`**](http://bes3.to.infn.it/Boss/7.0.2/html/classRecMdcKalTrack.html)**\`\`** | **\`\`**[**`EvtRecTrack::mdcKalTrack`**](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html#b992dd00fcd938cf17b4a6090ca16a81)**\`\`** |
| **TOF** | **Time-of-Flight** | **\`\`**[**`RecTofTrack`**](http://bes3.to.infn.it/Boss/7.0.2/html/classRecTofTrack.html)**\`\`** | **\`\`**[**`EvtRecTrack::tofTrack`**](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html#2c997882dd8ad532f01f0a10f93de2a0)**\`\`** |
| **EMC** | **EM-Calorimeter** | **\`\`**[**`RecEmcShower`**](http://bes3.to.infn.it/Boss/7.0.2/html/classRecEmcShower.html)**\`\`** | **\`\`**[**`EvtRecTrack::emcShower`**](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html#ac7a7d3cc71a349c2e9de6cf19865ecf)**\`\`** |
| MUC | Muon Chamber | \`\`[`RecMucTrack`](http://bes3.to.infn.it/Boss/7.0.2/html/classRecMucTrack.html)\`\` | \`\`[`EvtRecTrack::mucTrack`](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html#a3a8ae89c68adcec20ac3fb7248a1b31)\`\` |
| \_\_ | _Extension through all_ | \`\`[`RecExtTrack`](http://bes3.to.infn.it/Boss/7.0.2/html/classRecExtTrack.html)\`\` | \`\`[`EvtRecTrack::extTrack`](http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecTrack.html#3fd94beab03bbde9f056f832b106868a)\`\` |

## Introduction

One of the basic analysis packages that is already provided in BOSS is the `RhopiAlg` package. Within BESIII, almost everyone knows it, because it is used as the starting point for developing your own packages. `RhopiAlg` is an illustration of a typical procedure in particle physics: reconstructing a decayed particle. For this, you will have to make apply cuts on measured parameters and this package is an illustration of this procedure.

The `RhopiAlg` analyses the decay of the $$\rho(770)$$ meson. As you can see in the in the [PDG listing for this meson](http://pdg.lbl.gov/2018/listings/rpp2018-list-rho-770.pdf), the $$\rho(770)$$ meson predominantly decays through $$\rho\rightarrow\pi\pi$$ \(almost 100%\), whether it concerns a $$\rho^+$$ , $$\rho^0$$ , or $$\rho^-$$. This means that we can reconstruct this meson purely through this _2-particle decay mode_.

Additionally, when we consider the charged $$\rho^\pm$$ mesons, one of the decay products is the neutral pion: $$\rho^\pm \rightarrow \pi^\pm\pi^0$$. This meson is again neutral and cannot be detected, so has to be reconstructed. But here again, there is one dominant decay mode: $$\pi^0 \rightarrow \gamma\gamma$$ \($$98.823 \pm 0.034 \%$$, see [its PDG listing](http://pdg.lbl.gov/2018/listings/rpp2018-list-pi-zero.pdf)\). This means that we can reconstruct the $$\rho^\pm$$ meson almost exclusively through its $$\rho^\pm \rightarrow \pi^\pm\pi^0 \rightarrow \pi^\pm\gamma\gamma$$ decay channel.

In reconstructing $$\pi^0$$ and $$\rho^0$$, you will run into another common phenomenon in hadron research: the width of the decaying particle. The width of $$\rho^0$$ is much wider than $$\pi^0$$ and therefore results in interesting differences in the eventual invariant mass spectra. In the final event selection, you will for instance see that a fit of the invariant mass peaks results in different widths.

## Where to find it?

The original `RhopiAlg` package \(version `0.0.23`\) is located here,

```text
/afs/ihep.ac.cn/bes3/offline/Boss/$BOSSVERSION/Analysis/Physics/RhopiAlg/RhopiAlg-00-00-23
```

You can also [have a look at it in the BOSS Afterburner](https://github.com/redeboer/BOSS_IniSelect/tree/master/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-00-00-23) or [import the original packages](https://github.com/redeboer/BOSS_IniSelect/blob/master/workarea/import_packages.sh) for any version of BOSS.

A better commented and expanded version is also available within the BOSS Afterburner \([version 1.0.0](https://github.com/redeboer/BOSS_IniSelect/tree/master/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-01-00-00)\). In this tutorial, we will work with this version, but, as with `TestRelease`, you can choose to work with a copy from the `$BesArea`.

The `RhopiAlg` package is one of the dependencies of `TestRelease`: you actually use `TestRelease` to run an analysis from `RhopiAlg` \(or any other package\). This also means that you have to follow the normal procedure for updating a package [as described above](../../../tutorials/getting-started/setup-package.md#updating-a-package).

## Declaring and defining properties like cuts

See [header `.h` file](https://github.com/redeboer/BOSS_IniSelect/blob/master/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-01-00-00/RhopiAlg/RhopiAlg.h) for declarations and [source `.cxx` code](https://github.com/redeboer/BOSS_IniSelect/blob/master/workarea/Analysis/Physics/RhopiAlg/RhopiAlg-01-00-00/src/RhopiAlg.cxx) for definitions of cuts.

## Determining vertex position

## Writing properties

## Looping over charged and neutral tracks

## Kalman kinematic $$n$$-constraints fit procedure

* `fit4c` refers to the 4-constraints coming from the original $$\pi^0 \rightarrow \gamma\gamma$$ meson \(or other mesons, depending on the collision energy\), namely, the 4-momentum of the system \(collision energy and sum of the 3-momenta\). Note that the $$\chi^2_\text{red}$$ of the fit is the same for any combination, as the for constraints are the same in each event.
* `fit5c`  is used when an additional constraint is applied. In the `Rhopi` package, this fifth constraint refers to the constraint reconstruction of $$\rho^\pm \rightarrow \pi^\pm\pi^0 \rightarrow \pi^\pm\gamma\gamma$$, namely the mass of the pion.

## Cut flow

## Output `root` file

{% hint style="warning" %}
**@todo** Describe `RhopiAlg` and how to work with it.
{% endhint %}


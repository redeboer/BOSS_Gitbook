# Fitting

{% hint style="danger" %}
**This part of the tutorial is not yet finished** and currently only contains a rough sketch of the contents.
{% endhint %}

See fit methods defined in namespace [`CommonFunctions::Fit`](https://redeboer.github.io/BOSS_Afterburner/namespaceCommonFunctions_1_1Fit.html).

## Fitting meson resonances

Best overview of types of fits and theoretical motivations for each of those can be found in section ["Resonances" of the _PDG_](http://pdg.lbl.gov/2018/reviews/rpp2018-rev-resonances.pdf).

* BESIII is a _formation experiment_.

See documentation for all `RooFit` parametrisations [here](https://root.cern/doc/master/group__Roofit.html).

### Single and double Gaussian

Characterisation of detector resolution\(s\).

### Breit-Wigner parametrisation

* Only works in case of _narrow structure_ and if there are no other resonances nearby
* Can be used to extract _pole parameters_ such as particle width
* Possible: energy dependent parameters

### Flatté parametrisation

* Analytical continuation of the Breit-Wigner parametrisation
* Does not allow for extraction of pole parameters, only ratios

## Background parametrisation

* [Polynomial](https://root.cern/doc/master/classRooPolynomial.html)
* [Chebychev polynomial](https://root.cern.ch/doc/master/classRooChebychev.html)
* [Argus background shape](https://root.cern/doc/master/classRooArgusBG.html)

## Ideas for a `RooFit` wrapper

Probably won't be developed in the _BOSS Afterburner_. For now, `RooFit` procedures are enveloped in the `CommonFunctions.h` header.

An example of a `RooFit` wrapper would be `RooRarFit` by the BaBar Collaboration:

* [Documentation HTML pages](http://hepwww.rl.ac.uk/fwilson/RooRarFit.html)
* Presentation on RooRarFit by Lei Zhang \(UC Riverside, 2007\)

{% hint style="warning" %}
**@todo** Describe `FSFilter` by BESIII.
{% endhint %}




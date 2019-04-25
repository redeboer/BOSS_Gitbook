# Fitting procedures

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

## Background shapes

* [Polynomial](https://root.cern/doc/master/classRooPolynomial.html)​
* ​[Chebychev polynomial](https://root.cern.ch/doc/master/classRooChebychev.html)​
* ​[Argus background shape](https://root.cern/doc/master/classRooArgusBG.html)​

## Other literature

* [LHCb GitBook on RooFit](https://lhcb.github.io/ostap-tutorials/fitting/decorations.html)


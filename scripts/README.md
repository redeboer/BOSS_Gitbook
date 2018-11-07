Script folder
=============

This folder contains simple ROOT macros/scripts that are used for testing or to be able to quickly show something.

[TOC]

# Comparison of `RooFit` versus `TH1::Fit`
See the following scripts:

- [`FitDoubleGaussian_RooFit.C`](FitDoubleGaussian_RooFit.C).
- [`FitDoubleGaussian_TH1Fit.C`](FitDoubleGaussian_TH1Fit.C).

For a nice tutorial of `RooFit`, see [this official page](https://root.cern.ch/roofit-20-minutes). For a nice tutorial of the `TH1:Fit` method in ROOT, see [this tutorial](https://root.cern.ch/root/htmldoc/guides/users-guide/FittingHistograms.html).

In both these scripts, we analyse output of the `RhopiAlg` algorithm by fitting a double Gaussian to the invariant mass spectrum of \f$ \pi^0 \rightarrow \gamma\gamma \f$ (neutral pion decaying to two photons). The parameters are set in the same way, but `TH1::Fit` converses more easily than `RooFit`.

The charm of `RooFit` a function can be nicely build up bit by bit with probability distribution objects, but at least for a simple formula like a double Gaussian, I do not see much advantage over working simply with `TF1` objects.

# Comparison of `TChain` versus `hadd`ed files
See script [`CompareChainVsHadd.C`](CompareChainVsHadd.C).

A simple test, looping over a tree with 263.990 event for 100 times, resulted in the following results. Here, 'uncompiled' stands for `root TestScript.C` and 'compile' stands for `root TestScript.C+`.

|            | `hadd`    | `TChain`  |       |
|:----------:|:---------:|:---------:|:-----:|
| compiled   | 576 ns    | 771 ns    | 33.9% |
| uncompiled | 767 ns    | 969 ns    | 26.3% |
|            | **33.2%** | **25.7%** |       |

**Conclusion:**
1. Best to use `hadd`!
2. If your compilation takes e.g. 10 seconds, you win back this time if you run over around 50M events. But this number is probably much lower for more complicated per-event computations.
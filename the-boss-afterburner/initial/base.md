# Base algorithm

{% hint style="danger" %}
**The `TrackSelector` package and derived packages are still being developed and tested.**

Tutorial pages are therefore not yet complete. You can meanwhile have a look at the current design of the class structure [here](https://redeboer.github.io/BOSS_Afterburner/modules.html).
{% endhint %}

The base algorithm [`TrackSelector`](https://redeboer.github.io/BOSS_Afterburner/classTrackSelector.html) encapsulates the procedures that are shared by other analysis. The procedures are shared in two ways:

1. **Default procedure in algorithm steps** The `TrackSelector` base algorithm always performs the [`initialize`](https://redeboer.github.io/BOSS_Afterburner/classTrackSelector.html#a66444e69d52ba9b5d861916fa072c4a4), [`execute`](https://redeboer.github.io/BOSS_Afterburner/classTrackSelector.html#a6866699cb5218a5c8dc313f5ff5910cb), and [`finalize`](https://redeboer.github.io/BOSS_Afterburner/classTrackSelector.html#ac12f71603152e155a688861d11c51c46) algorithm steps, so any derived algorithm will do the same. For example, `TrackSelector::execute` stores a vector of pointers to tracks, because you have to do this for your analysis anyway. You can use that result for your own analysis in the continuation `execute_rest` step of your derived algorithm.
2. **Modulation** If you develop some procedure for your own _derived_ algorithm and you think that that procedure could be useful for other types of analyses as well, you can modulate this procedure into a class method \(function\). This method can then be added to the `TrackSelector` base algorithm, so that it is available to others as well.

## Major components

### Collection of charged and neutral tracks

### Write general event info

The `TrackSelector`also allows you to write info for _all_ events, that is, _without cuts_.

* Multiplicities
* Vertex info
* Neutral and charged track info

### Monte Carlo truth for `topoana`

Helper methods that can be used by derived classes _after_ initial selection. Components taken from [`MctruthForTopoAnaAlg`](https://github.com/redeboer/BOSS_Afterburner/tree/master/boss/workarea/Analysis/TopoAna/MctruthForTopoAnaAlg/MctruthForTopoAnaAlg-00-00-00) package.

### Helper functions that can be used by derived classes

* 

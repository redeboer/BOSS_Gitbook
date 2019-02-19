# Motivation

{% hint style="danger" %}
**The `TrackSelector` package and derived packages are still being developed and tested.**

Tutorial pages are not yet available, but you can meanwhile have a look at the class descriptions:  
[**https://redeboer.github.io/BOSS\_Afterburner/modules.html**](https://redeboer.github.io/BOSS_Afterburner/modules.html)\*\*\*\*
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

## Problems with booking procedures

### `NTuple` booking procedure

The typical booking procedure [used in for instance the `RhopiAlg`](../../besiii-software-system/packages/rhopi.md#declaring-and-defining-properties-like-cuts) is not the most convenient. It requires many steps, so it is easy to make a mistake when you modify your code. An overview of the procedure:

1. **Declaration** of the [`NTuple::Tuple`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classNTuple_1_1Tuple.html) pointer \(eventual `TTree`\) and [`NTuple::Item`s](https://dayabay.bnl.gov/dox/GaudiKernel/html/classNTuple_1_1Item.html) \(eventual branches\) in the header file. This is required because you need a memory address \(branch\) to which you store values and then [write](https://dayabay.bnl.gov/dox/GaudiKernel/html/classINTuple.html#a89cd8df7b0cff68a1cbb45d9c8fe7df5) them.
2. 3. **Add** the `NTuple::Item` s \(branches\) to their branches using [`NTuple::Tuple::addItem`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classNTuple_1_1Tuple.html#a78033967fbd89f7f18e7d6d7d43f41ac). _Note that this procedure can become complicated and difficult to modulate when using indexed items._

### Declaring properties

[Similarly to booking the `NTuple`s](motivation.md#ntuple-booking-procedure), declaring package properties is not too convenient either. An overview:

1. **Declaration** of memory addresses in the header file, such as `double`s for cuts and `bool`leans for switches.
2. **Declaring the property** in the constructor using [`Algorithm::declareProperty`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html#acc63d83555ffb3833df7334468551d7d). Here you assign a name to the property. It is inconvenient that you have to go back and forth between the header file and the source code to keep the declared properties in agreement with the declared memory addresses.
3. _For `bool` switches:_ Typically, the `bool`properties are used to decide whether or not to book and write certain `NTuple`s, so you have to use them again in the `initialize`  and `execute` steps. It would therefore be nice if these `bool` switch properties are declared automatically along with the corresponding `NTuple`s.

### Setup in `TrackSelector`

The `NTuple` and property procedures and relations described above suggest the design of a class structure that encapsulates everything. Ideally, you then just have to name these properties and `NTuple`s in the constructor so that you instantly have a nice overview of all of them there and can easily modify your code.

There is however a challenge due to the structure of the Gaudi framework: the essential functions [`Algorithm::declareProperty`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html#acc63d83555ffb3833df7334468551d7d) and [`INTupleSvc::book`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classINTupleSvc.html#a501f6331df3de22c81e91f3f3f7704b6)  require an instance of `Algorithm`. \(see for instance [`Algorithm::ntupleSvc`](https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html#aa0e741efefa5312b20f9a213c3ab4cbb)\). A solution would be to keep track of the all instances of the constructed encapsulation classes of `NTuple`s and properties using a `static`list of pointers. Methods defined in the `TrackSelector`then take care of the booking procedures automatically by looping over these lists for you and using the data members in these classes to perform all the steps required.

A problem remains the correspondence between names of the properties and `NTuple`s: you will have to call them later through the code and the question is how? That is, whether through their names, or through their instances.

{% hint style="warning" %}
**@todo** Improve and describe solution in `TrackSelector`.
{% endhint %}


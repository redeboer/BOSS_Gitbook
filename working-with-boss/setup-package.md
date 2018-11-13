# Set up a BOSS package

## Set up a BOSS package

Now that you have configured your **BOSS** work area, you can start developing packages. The typical example package to get you started is the `TestRelease` package. We will need to copy this package into your [your work area](../#set-up-your-work-area). So let's navigate there:

```text
cd /ihepbatch/bes/$USER/workarea-7.0.3
```

Now copy the typical `TestRelease` example to your _work area_ and navigate into it:

```text
cp –r $BesArea/TestRelease ./
cd TestRelease/TestRelease-*
```

You are now in a folder like `TestRelease/TestRelease-00-00-86`, where `00-00-86` represents the version. Using `ls`, you can see that it contains some folders:

* `cmt`: the _Configuration Management Tool_ that you will use to connect to **BOSS**
* `CVS`: a folder used when generating documentation
* `run`: which contains the `jobOptions` that are run with `boss.exe`

We can set up the `TestRelease` by going into `cmt` and 'broadcasting' to **BOSS** from there:

```text
cd cmt
cmt broadcast cmt config
cmt broadcast make
source setup.sh
```

This will initialise the package so that you can run it from the run folder. This is done using `boss.exe`:

```text
cd ../run
boss.exe jobOptions_sim.txt
```

which, in this case, will run a Monte Carlo simulation.

Note that in step 7 [when we set up the work area](../#set-up-your-work-area) we added a line to the `.bashrc` that ensures that the `TestRelease` package is loaded every time you log in, so you won't have to do this every time yourself.

## Creating packages 'the official way'

@todo Go through Chinese documentation and [this page](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Getting_Started) and write out.


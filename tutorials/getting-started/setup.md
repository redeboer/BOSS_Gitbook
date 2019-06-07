# Setup of your BOSS environment

{% hint style="warning" %}
**Warning:** In it's current version, this tutorial assumes you use a `bash` terminal. It should work for TC-shell as well, but if you experience problems, please [contact me](../../appendices/about.md).
{% endhint %}

In this section, you will learn how to 'install' BOSS. Since BOSS has already been compiled on the server, installing actually means that you set up _path variables_ in `bash`. You can then access certain versions of those BOSS builds from your IHEP user account.

There are two main directories that you will be using:

1. the _BOSS workarea_, from which access to BOSS is managed and which contains your own BOSS packages \(mostly event selection\), and
2. an _analysis folder_, which contains scripts that you design yourself to analyse output from BOSS \(for example fitting histograms\). You can even decide to have this folder locally on your own pc.

It is totally up to you where you place these folders. A common procedure is to put the _analysis folder_ in the `ihepbatch` folder, but have a look at the [organisation of the IHEP server](server.md) to decide if there is some other folder that meets your needs.

{% hint style="info" %}
#### In case of large data

If you work with large data samples, you may want to write your output to a different folder in[`scratchfs`](server.md#main-directories). Take care to modify the paths your job option files accordingly!
{% endhint %}

## Set up the BOSS environment

{% hint style="info" %}
**The below procedure is** [**also available as a bash script**](https://github.com/redeboer/BOSS_IniSelect/blob/master/setup/SetupBoss.sh)**.**
{% endhint %}

In this part of the tutorial, we will do two things: \(1\) setup the necessary references to BOSS and \(2\) preparing your local 'workarea'. You will be running your BOSS packages \(mainly event selection\) from this folder. Next to your workarea folder, there will be a [_Configuration Management Tool_](intro.md#configuration-management-tool-cmt) folder \([`cmthome*`](https://github.com/redeboer/BOSS_IniSelect/tree/master/cmthome)\), which manages access to the BOSS installation. In the end you will have a file structure like this:

```text
/besfs/users/$USER/boss/
 - cmthome    (manages access to BOSS)
 - workarea   (contains your analysis code)
```

### **Step 1:** Define your _BOSS workarea_ folder

For the sake of making this tutorial work in a general setting, we will first define a `bash` variable here \(you can just execute this command in `bash`\):

```bash
BOSSWORKAREA="/besfs/users/$USER/boss"
```

This variable points to the path that will contain your workarea. You can change what is between the quotation marks \(`""`\) by whatever folder you prefer in case you want your workarea to be placed in some other path, for instance by `/ihepbatch/bes/$USER`.

Now, move into that directory.

```bash
cd "$BOSSWORKAREA"
```

At this stage, you'll have to decide which version of BOSS you have to use. At the time of writing, **version 7.0.4** is the latest stable version, though it could be that for your analysis you have to use data sets that were reconstructed with older versions of BOSS. Here, I'll just use `7.0.4`, but you can replace this number with whatever version you need.

For convenience, we'll again define the version number as a variable here.

```bash
BOSSVERSION="7.0.4"
```

{% hint style="info" %}
An overview of all BOSS versions and their release notes can be found [here](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/ReleaseNotes) \(requires login\).
{% endhint %}

### **Step 2: Import environment scripts**

We'll first have to obtain some scripts that can set up the necessary references to BOSS. This is done by copying the `cmthome*` folder from the BOSS Software directory \(which contains all source code for BOSS\) to your workarea:

```bash
mkdir "$BOSSWORKAREA/cmthome"
cp -Rf /afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-$BOSSVERSION/* "$BOSSWORKAREA/cmthome"
cd "$BOSSWORKAREA/cmthome"*
```

Note from the `cp` command that we have omitted the version from the original folder name. You can choose to keep it as well, but here we chose to use the convention is that `cmthome` and `workarea` without a version number refers to the latest stable version of BOSS.

### **Step 3: Modify** `requirements`

In `cmthome*` , you'll now have to modify a file called `requirements`, so that it handles your username properly. We'll use the `vi` editor here, but you can use whatever editor you prefer:

```bash
vi requirements
```

The file contains the following lines:

```bash
#macro WorkArea "/ihepbatch/bes/maqm/workarea"

#path_remove CMTPATH "${WorkArea}"
#path_prepend CMTPATH "${WorkArea}"
```

Uncomment them \(remove the hash `#`\) and replace what is between the first quotation marks `"..."` with your the path to your workarea. In our case, it looks like this:

```bash
macro WorkArea "/besfs/users/$USER/boss/workarea"

path_remove CMTPATH "${WorkArea}"
path_prepend CMTPATH "${WorkArea}"
```

The `$CMTPATH` is an important variable for [CMT](intro.md#configuration-management-tool-cmt). It is comparable to `$PATH` in that it lists all directories that contain CMT packages. Note that, when CMT searches for packages that you listed in the `requirements` file, it will use the first occurrence in the `$CMTPATH`. This is why you `prepend` it.

{% hint style="info" %}
Note that `$CMTPATH` will actually be set to a subfolder called `workarea` **within** what we so far defined as the _BOSS workarea_. We will create this subfolder in [**Step 7**](setup.md#step-7-create-a-workarea-subfolder).
{% endhint %}

### **Step 4: Set references to BOSS**

Now you can use the scripts in `cmthome` to set all references to BOSS at once, using:

```bash
source setupCMT.sh  # starts connection to the CMT
cmt config          # initiates configuration
source setup.sh     # sets path variables
```

Just to be sure, you can check whether the path variables have been set correctly:

```bash
echo $CMTPATH
```

If everything went well, it should print something like:

```bash
/besfs/users/$USER/boss/workarea:/afs/ihep.ac.cn/bes3/offline/Boss/7.0.4:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/gaudi/GAUDI_v23r9:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/LCGCMT/LCGCMT_65a
```

The paths listed here \(separated by `:` columns\) will be used to look for packages required by the `requirements` files of packages \(see [Set up a BOSS package](setup-package.md)\). The first of these paths points to your _BOSS workarea_, the second to the BOSS version you use \(also called `$BesArea`\), and the rest to libraries of for instance [Gaudi](https://dayabay.bnl.gov/dox/GaudiKernel/html/annotated.html).

### **Step 5: Modify your** `bashrc`

Adapt your `bash` profile \(`.bash_profile`\) and _run commands_ file \(`.bashrc`\) so that BOSS is loaded automatically every time you log into the server.

First, add the following lines to your bash profile \(use `vi ~/.bash_profile`\):

{% code-tabs %}
{% code-tabs-item title=".bash\_profile" %}
```bash
if test -f .bashrc; then
    source .bashrc
fi
```
{% endcode-tabs-item %}
{% endcode-tabs %}

These lines force the server to load your `.bashrc` run commands file when you log in. In that file, you should add the following lines:

{% code-tabs %}
{% code-tabs-item title=".bashrc" %}
```bash
export BOSSWORKAREA="/besfs/users/${USER}/boss"
export BOSSVERSION="7.0.4"
CMTHOME="/afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-${BOSSVERSION}"

source "${BOSSWORKAREA}/cmthome/setupCMT.sh"
source "${BOSSWORKAREA}/cmthome/setup.sh"
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/
```
{% endcode-tabs-item %}
{% endcode-tabs %}

Notice that the commands we used in [**Step 4**](setup.md#step-4-set-references-to-boss) appear here again. There is also a reference to the `workarea` \(see [**Step 7**](setup.md#step-7-create-a-workarea-subfolder)\). The last line allows you to submit BOSS jobs to the 'queue' \(using the `hep_sub` command\) — for now, don't worry what this means.

You can now either log in again to the server or use `source ~/.bashrc` to reload the run commands.

### **Step 6: Test BOSS using** `boss.exe`

To test whether everything went correctly, you can try to run BOSS:

```text
boss.exe
```

It should result in a \(trivial\) error message like this:

```text
               BOSS version: 7.0.4
************** BESIII Collaboration **************

the jobOptions file is : jobOptions.txt
ERROR! the jobOptions file is empty!
```

If not, something went wrong and you should carefully recheck what you did in the above steps.

### **Step 7: Create a** `workarea` **subfolder**

It is convenient to place your BOSS packages in a `workarea` folder next to the `cmthome` folder we have been using so far. In our case it will be:

```bash
mkdir -p "$BOSSWORKAREA/workarea"
```

The option `-p` is used to avoid error messages when making this directory.

We'll get back to the `workarea` folder when we [set up a BOSS package](setup-package.md).

{% hint style="info" %}
Your _BOSS workarea_ typically contains three folders \(see [an example here](https://github.com/redeboer/BOSS_IniSelect/tree/master/workarea)\):

1. [`Analysis`](https://github.com/redeboer/BOSS_IniSelect/tree/master/workarea/Analysis), which contains CMT packages that you use for your analysis
2. [`InstallArea`](https://github.com/redeboer/BOSS_IniSelect/tree/master/workarea/InstallArea), which is created when you use `cmt confi`
3. [`TestRelease`](https://github.com/redeboer/BOSS_IniSelect/tree/master/workarea/TestRelease), which is used to run all packages

The file structure of your _workarea_ follows that of the packages in `$BesArea` \(use `ls $BesArea` to verify this\), because, [as explained before](setup.md#step-3-modify-requirements), packages in your _workarea_ have priority over those in `$BesArea`. As such, you can expand your _workarea_ by copying certain packages from the `$BesArea` and modifying those. In the _BOSS Afterburner_, there is for instance [a modification of the `BesEvtGen` Monte Carlo generator](https://github.com/redeboer/BOSS_IniSelect/tree/master/workarea/Generator).
{% endhint %}


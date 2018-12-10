# Setup of your BOSS environment

{% hint style="warning" %}
**Warning:** In it's current version, this tutorial assumes you use a `bash` terminal.

**@todo** Rewrite for TC shell.
{% endhint %}

I advise you to set up your environment in the same way I did. There are two main directories that you will be using: \(1\) the _BOSS Afterburner_ repository, which contains your analysis code, and \(2\) the _BOSS workarea_, which contains your run scripts for BOSS.

I placed the _workarea_ folder **within** the BOSS Afterburner repository \(see `BOSS_Afterburner/boss`\), so that it is available as an example, but another common procedure is to put it in the `ihepbatch` folder \(see [Organisation of the IHEP server](ihep-server.md) to decide what is best for you\). I think the BOSS Afterburner is best placed in the `besfs` folder. Data generated with **BOSS** through the _workarea_ scripts will then be written to the `BOSS_Afterburner/data` subdirectory and analysis plots to the `BOSS_Afterburner/plots` folder.

## \(1\) Set up the _BOSS Afterburner_

This is the simple part. Go to the _BES file system_ folder \(or whatever folder you prefer\):

```bash
cd /besfs/users/$USER
```

[Clone](https://help.github.com/articles/cloning-a-repository/) the _BOSS Afterburner_ repository:

```bash
git clone https://github.com/redeboer/BOSS_Afterburner
```

The _BOSS Afterburner_ will be used to analyse output from **BOSS**. We get back to this in the section [Data analysis](data-analysis.md).

## \(2\) Set up your _BOSS workarea_

{% hint style="info" %}
**The below procedure is** [**also available as a bash script**](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/setup_boss.sh)**.**
{% endhint %}

You will be running your **BOSS** analyses from this folder. Your _BOSS workarea_ will contain a _Configuration Management Tool_ folder \([`cmthome*`](https://github.com/redeboer/BOSS_Afterburner/tree/master/boss/cmthome)\), which is used to set up path variables for **BOSS**, and a [`workarea*`](https://github.com/redeboer/BOSS_Afterburner/tree/master/boss/workarea) folder where you develop your own **BOSS** packages \(including the `jobOptions*.txt` files\). Here, the \* stands for the version of BOSS you are using \(in the BOSS Afterburner it is left empty\).

We will learn more about what the Configuration Management Tool \(CMT\) actually is [Set up a BOSS package](setup-package.md).

### **Step 1:** Go to your workarea folder

For the sake of making this tutorial work in a general setting, we will first define a `bash` variable here:

```bash
BOSSWORKAREA="/besfs/users/$USER/BOSS_Afterburner/boss"
```

You can change what is between the quotation marks `"` by whatever folder you prefer, for instance by `/ihepbatch/bes/$USER` in case you want your _BOSS workarea_ to be placed in some other path. As you see, we'll use the `BOSS_Afterburner/boss` folder in this tutorial.

Now, move into that directory.

```bash
cd "$BOSSWORKAREA"
```

At this stage, you'll have to decide which version of BOSS you have to use. At the time of writing, **version 7.0.4** is the latest stable version, though it could be that for your analysis you have to use data sets that were reconstructed with older versions of **BOSS**. Here, I'll just use `7.0.4`, but you can replace this number with whatever version you need.

Again, we'll define the version number as a variable here.

```bash
BOSSVERSION="7.0.4"
```

### **Step 2: Import environment scripts**

We'll first have to obtain some scripts that can set up the necessary references to **BOSS**. This is done by copying the `cmthome*` folder from BOSS Software directory to your _BOSS workarea_:

```bash
mkdir "$BOSSWORKAREA/cmthome"
cp -Rf /afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-$BOSSVERSION/* "$BOSSWORKAREA/cmthome"
```

and navigate into that copy:

```bash
cd "$BOSSWORKAREA/cmthome"*
```

Note that we have omitted the version from the original folder name. You can choose to keep it as well, but in the BOSS Afterburner, the convention is that `cmthome` and `workarea` without a version number refer to the latest stable version of BOSS.

### **Step 3: Modify `requirements`**

You'll now have to modify the file called `requirements`, so that it handles your username properly. We'll use the `vi` editor here, but you can use whatever editor you prefer:

```bash
vi requirements
```

The file contains the following lines:

```bash
#macro WorkArea "/ihepbatch/bes/maqm/workarea"

#path_remove CMTPATH "${WorkArea}"
#path_prepend CMTPATH "${WorkArea}"
```

Uncomment them \(remove the hash `#`\) and replace what is between the first quotation marks `"..."` with your the path to your _BOSS workarea_. In the case of having the _BOSS workarea_ within the BOSS Afterburner, it looks like this:

```bash
macro WorkArea "/besfs/users/$USER/BOSS_Afterburner/boss/workarea"

path_remove CMTPATH "${WorkArea}"
path_prepend CMTPATH "${WorkArea}"
```

{% hint style="info" %}
Note that `$CMTPATH` will actually be set to a subfolder called `workarea` **within** what we so far defined as the _BOSS workarea_. We will create this subfolder in [**Step 7**](setup.md#step-7-create-your-workarea).
{% endhint %}

### **Step 4: Set references to BOSS**

Now you can use the scripts in `cmthome` to set all references to **BOSS** at once, using:

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
/besfs/users/$USER/BOSS_Afterburner/boss/workarea:/afs/ihep.ac.cn/bes3/offline/Boss/7.0.4:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/gaudi/GAUDI_v23r9:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/LCGCMT/LCGCMT_65a
```

The paths listed here \(separated by `:` columns\) will be used to look for packages required by the `requirements` files of packages \(see [Set up a BOSS package](setup-package.md)\). The first of these paths is your _workarea_, the second the BOSS version you use, the rest the paths of the Gaudi libraries.

### **Step 5: Modify your `bashrc`**

Adapt your `bash` profile \(`.bash_profile`\) and _run commands_ file \(`.bashrc`\) so that **BOSS** is loaded automatically every time you log into the server.

First, add the following lines to your bash profile \(`vi ~/.bash_profile`\):

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
source ~/.load_boss.sh
```
{% endcode-tabs-item %}
{% endcode-tabs %}

For this to work upon login, you will have to [download the `.load_boss.sh` script from the BOSS Afterburner](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/.load_boss.sh) and add it to your home folder \(`~`\).

Notice that the commands we used in [**Step 4**](setup.md#step-4-set-references-to-boss) are again used  in the [`.load_boss.sh` file](https://github.com/redeboer/BOSS_Afterburner/blob/master/boss/.load_boss.sh). There is also a reference to the `workarea` \(see [**Step 7**](setup.md#step-7-create-your-workarea)\). The last line allows you to submit **BOSS** jobs to the queue \(`hep_sub`\) — for now, don't worry what this means.

You can now either log in again to the server or use `source ~/.bashrc` to reload the run commands.

### **Step 6: Test it using `boss.exe`**

To test whether everything went correctly, you can try to run **BOSS**:

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

### **Step 7: Create a `workarea` subfolder**

It is convention to place your **BOSS** packages in a `workarea` folder next to the `cmthome` folder we have been using so far. In our case it will be:

```bash
mkdir -p "$BOSSWORKAREA/workarea"
```

The option `-p` is used to avoid error messages when making this directory: if you followed the above steps this folder already exists, because it is already available in the BOSS repository \(which you cloned in [the first part](setup.md#1-set-up-the-boss-afterburner)\).

We'll get back to the `workarea` folder when we [set up a BOSS package](setup-package.md).


# Setup of your BOSS environment

**\*Note:** In it's current version, this tutorial assumes you use a `bash` terminal.\*

I advise you to set up your environment in the same way I did. There are two main directories that you will be using: \(1\) the _work area_ that contains your run scripts for **BOSS** and \(2\) the _BOSS Afterburner_ repository. I placed the work area in my `ihepbatch` folder \(see [Organisation of the IHEP server](ihep-server.md)\) and the BOSS Afterburner in my `besfs` folder. Data generated with **BOSS** through the work area scripts is written then to the `BOSS_Afterburner/data` directory on `besfs` \(see later\).

## \(1\) Set up your work area

@todo Test the procedure described below! @todo Rewrite for TC shell.

You will be running your **BOSS** analyses from this folder. It contains a _Configuration Management Tool_ folder \(`cmthome`\), which is used to set up path variables for **BOSS**, and a `workarea` folder where you develop your own **BOSS** packages \(including the `jobOptions*.txt` files\).

### **Step 1:** Go to your _IHEP batch_ folder

```text
cd /ihepbatch/bes/$USER
```

At this stage, you'll have to decide which version of BOSS you have to use. At the time of writing, version 7.0.3 is the latest stable version, though it could be that for your analysis you have to use data sets that were reconstructed with older versions of **BOSS**. Here, I'll just use `7.0.3`, but you can replace this number with whatever version you need.

### **Step 2: Configure the environment**

We'll first set up the necessary references to **BOSS** using the _Configuration Management Tool_ \(CMT\). This is done by copying the `cmthome` folder from BOSS Software directory to the `ihepbatch` folder where you currently are:

```text
cp -R /afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-7.0.3 .
```

and navigate into that copy:

```text
cd cmthome-7.0.3
```

### **Step 3: Modify `requirements`**

You'll now have to modify the file called `requirements` so that it handles your username properly. We'll use the `vi` editor here, but you can use whatever editor you prefer:

```text
vi requirements
```

The file contains the following lines:

```text
#macro WorkArea "/ihepbatch/bes/maqm/workarea"

#path_remove CMTPATH "${WorkArea}"
#path_prepend CMTPATH "${WorkArea}"
```

Uncomment them \(remove the hash `#`\) and replace `maqm` with your own user name. Like this:

```text
macro WorkArea "/ihepbatch/bes/$USER/workarea"

path_remove CMTPATH "${WorkArea}"
path_prepend CMTPATH "${WorkArea}"
```

### **Step 4: Set references to BOSS**

Now you can use the scripts to set all references to **BOSS** at once using:

```text
source setupCMT.sh  # starts connection to the CMT
cmt config          # initiates configuration
source setup.sh     # sets path variables
```

Just to be sure, you can check whether the path variables have been set correctly:

```text
echo $CMTPATH
```

If everything went well, it should print:

```text
/ihepbatch/bes/$USER/workarea-7.0.3:/afs/ihep.ac.cn/bes3/offline/Boss/7.0.3:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/gaudi/GAUDI_v23r9:/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/ExternalLib/LCGCMT/LCGCMT_65a
```

### **Step 5: Modify your `bashrc`**

Adapt your `bash` profile \(`.bash_profile`\) and _run commands_ file \(`.bashrc`\) so that **BOSS** is loaded automatically every time you log into the server. The easy solution is simply copy-pasting and running these commands:

```text
echo -e "if test -f .bashrc; then\n\tsource .bashrc\nfi" >> ~/.bash_profile
echo "IHEPBATCH=\"/ihepbatch/bes/$USER\"" >> ~/.bashrc
echo "CMTHOME=\"/afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-7.0.3\"" >> ~/.bashrc
echo "source $IHEPBATCH/cmthome-7.0.3/setupCMT.sh" >> ~/.bashrc
echo "source $IHEPBATCH/cmthome-7.0.3/setup.sh" >> ~/.bashrc
echo "source $IHEPBATCH/workarea-7.0.3/TestRelease/TestRelease-00-00-86/cmt/setup.sh" >> ~/.bashrc
echo "export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/" >> ~/.bashrc
```

However, to avoid becoming a [copy ninja](https://pics.me.me/kakashi-went-from-the-copy-ninja-to-the-copy-paste-14969048.png), you'd better modify these files yourself. First, add these lines to your bash profile \(`vi ~/bash_profile`\):

```text
if test -f .bashrc; then
    source .bashrc
fi
```

These lines force the server to load your `.bashrc` run commands file. In that file, you should add the following lines:

```text
IHEPBATCH="/ihepbatch/bes/$USER"
CMTHOME="/afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-7.0.3"
source $IHEPBATCH/cmthome-7.0.3/setupCMT.sh
source $IHEPBATCH/cmthome-7.0.3/setup.sh
source $IHEPBATCH/workarea-7.0.3/TestRelease/TestRelease-00-00-86/cmt/setup.sh
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/
```

Notice that the commands we used in step 4 are used here again. There is also a reference to the `workarea` \(see step 7\). The last line allows you to submit **BOSS** jobs to the queue---for now, don't worry what this means.

You can now either log in again to the server or use `source ~/.bashrc` to reload the run commands.

### **Step 6: Test it `boss.exe`**

To test whether everything went correctly, you can try to run **BOSS**:

```text
boss.exe
```

It should result in a \(trivial\) error message like this:

```text
               BOSS version: 7.0.3
************** BESIII Collaboration **************

the jobOptions file is : jobOptions.txt
ERROR! the jobOptions file is empty!
```

### **Step 7: Create your** _**workarea**_

It is convention to place your **BOSS** packages in a _workarea_ folder next to the `cmthome` folder we have been using so far. Let's create it:

```text
cd /ihepbatch/bes/$USER
mkdir workarea-7.0.3
```

We'll get back to this folder when we [set up a BOSS package](../#set-up-a-boss-package).

## \(2\) Set up the BOSS Afterburner

This is the simple part. Go to the _BES file system_ folder:

```text
cd /besfs/users/$USER
```

[Clone](https://help.github.com/articles/cloning-a-repository/) the _BOSS Afterburner_ repository:

```text
git clone https://github.com/redeboer/BOSS_Afterburner
```

The _BOSS Aafterburner_ will be used to analyse output from **BOSS**. We get back to this in the section [Data analysis](data-analysis.md).


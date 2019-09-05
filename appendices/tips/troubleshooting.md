---
description: Known issues and some solutions.
---

# Troubleshooting

### **I lost read-write access in my`afs` home folder**

Formerly, this problem could be solved using the `klog` command. Since August 2019, this command has become:

```bash
kinit $USER
aklog -d
```

You should now be able to read-write in _all_ your sessions.

### I'm sure my job is set up correctly, but it keeps resulting in:

```text
JobOptionsSvc       ERROR # =======> <package>/share/jobOptions_<package>.txt
JobOptionsSvc       ERROR # (22,1): parse error
...
JobOptionsSvc       FATAL Job options errors.
ApplicationMgr      FATAL Error initializing JobOptionsSvc
```

Yep, this is a weird one... So far, the cause was usually that the `jobOptions_*.txt` ends in a comment. You can solve it by adding a new line to the file.

### I cannot run a bash script, but I'm sure it should work

It could be that you wrote the `.sh` script on Windows and the file wasn't stored with Linux line endings. You can change these line endings back to Linux using:

```bash
sed -i 's/\r$//' $fileName
```

{% hint style="info" %}
This command is also available is used `source` to load [`jobs/CommonFunctions.sh`](https://github.com/redeboer/BOSS_IniSelect/blob/master/jobs/CommonFunctions.sh), you can use the function `ChangeLineEndingsFromWindowsToUnix`.
{% endhint %}

### Some header files are not found when compiling my package

Check your `requirements` file. Packages that you need should be declared here as well. For instance, if you want to use `McTruth` packages such as `McParticle.h`, you should add the line:

{% code-tabs %}
{% code-tabs-item title="requirements" %}
```text
use McTruth     McTruth-*     Event
```
{% endcode-tabs-item %}
{% endcode-tabs %}

### I cannot submit a job through `boss.condor` or `hep_sub` but see `ERROR: Failed to create new proc id` instead

Two known causes:

1. In the case of `hep_sub`, you should submit an **executable** bash script. Make the `sh` script executable using `chmod +x`. Use `boss.condor` in exactly the same way as `boss.exe`, that is, feed it a job options file \(`txt`\), not a bash script.
2. You sourced a bash script that contained an `export -f`statement \(exporting a bash `function`\). While this is correct way of exporting a function, it somehow affects BOSS. Change this statement into `export` \(omit the `f` option\) and the issue is fixed.

### I cannot run `boss.exe` without jobs

It should be possible to run `boss.exe` without jobs \(see [here](../../tutorials/getting-started/setup.md#step-6-test-boss-using-boss-exe)\). Does it result in the following error message?

```text
boss.exe: error while loading shared libraries: libReflex.so: cannot open shared object file: No such file or directory
```

If so, you probably forgot to [source `TestRelease`](../../tutorials/getting-started/setup.md#step-5-implement-the-testrelease-package).

### I get a message about `sysInitialize()` when running a job

If you receive the following error message:

```text
**************************************************
               BOSS version: 7.0.4
************** BESIII Collaboration **************

the jobOptions file is : jobOptions_sim.txt
JobOptionsSvc       FATAL in sysInitialize(): standard std::exception is caught
JobOptionsSvc       ERROR locale::facet::_S_create_c_locale name not valid
ApplicationMgr      FATAL Error initializing JobOptionsS
```

it means you have encountered another nasty bug. It probably has to do with the environment of the terminal with which you log in \(in the case of Windows, it is known to appear when [using the `wsl` terminal](./#compiling-on-windows-10)\). Try a different terminal \(such as [`git bash`](https://gitforwindows.org/)\) and the problem will probably disappear. It's magic!


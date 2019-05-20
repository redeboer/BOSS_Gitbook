---
description: Known issues and some solutions.
---

# Troubleshooting

### **I lost read-write access**

One easy command to solve it: use `klog` and enter your password. You should now be able to read-write in _all_ your sessions.

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


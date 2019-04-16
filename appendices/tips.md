# Tips, Tricks, and Troubleshooting

## Tips & Tricks

### Recommended software

Although I recognise that any programmer will and should develop her or his own habits, there are a few simple recommendations I would like to make, in particular for beginners.

* [**Visual Studio Code**](https://code.visualstudio.com) **\(VS Code\).** Note the "Code" here—I am not referring to the infamous IDE version. Microsoft decided to strip Visual Studio of its compile capabilities and develop this editor in parallel. Since it has been made available _for free_ on all OS platforms, is regularly updated, is configurable with easy-to-access `json` files, offers a growing number of user-developed extensions, I consider VS Code to be one of the most powerful code editors available on the market. Even for users who prefer not to touch the mouse, VS Code has much to offer as well. \(OK, if your are really hardcore keyboard only, just go for [`vim`](https://www.vim.org/).\) **Note:** For working on the IHEP server, you will need to install this handy [SSH FS extension](https://marketplace.visualstudio.com/items?itemName=Kelvin.vscode-sshfs).
* Other access to the IHEP server through SSH/SFTP:
  * [**Xmanager**](https://www.netsarang.com/products/xmg_overview.html)
  * [**WinSCP**](https://winscp.net/eng/index.php)
  * [**PuTTY**](https://www.putty.org/)

### Key generation for SSH

If you do not like to keep having to enter your password, have a look at generating an ssh key [here](https://www.ssh.com/ssh/keygen/) and [here](https://help.github.com/en/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent).

1. Generate a key with the command `ssh-keygen`. You can choose to leave the password empty.
2. Add the SSH key to the `ssh-agent` and create a corresponding _public key_ with the commands: `eval $(ssh-agent -s) ssh-add ~/.ssh/id_rsa`
3. Copy the public key to the server using: `ssh-copy-id -i ~/.ssh/id_rsa <your user name>@lxslc6.ihep.ac.cn` You will be asked for your IHEP account password.
4. Try to log in to the server with: `ssh -Y <your user name>@lxslc7.ihep.ac.cn` If all went correctly, you don't have to enter your password anymore.

### Compiling

For compiling outside ROOT \(that is, _not_ using the ROOT interpreter\), you will need to use a compiler like `g++`. The compiler needs to be told where the libraries for included ROOT header files are located. You can do this using flags that ROOT set during its installation. In case of `g++`, use:

```text
g++ YourCode.C -I$(root-config --incdir) $(root-config --libs --evelibs --glibs) -o YourBinaryOutput.o
```

_Pro_ `bash` _tip:_ You might like to create an easy command for this. You can do this by adding the following lines to your `~/.bashrc`.

```text
function rtcompile () {
    g++ "$1" -I$(root-config --incdir) $(root-config --libs --evelibs --glibs) -lRooFit -lRooFitCore -lRooStats -lMinuit -o "${1/.*/.o}"
}
function rtcompilerun () {
    rtcompile "$1"
    if [ $? -eq 0 ]; then
        ./${1/.*/.o}
    fi
}
function rtdebugcompile () {
    g++ "$1" -I$(root-config --incdir) $(root-config --libs --evelibs --glibs) -lRooFit -lRooFitCore -lRooStats -lMinuit -fsanitize=address -g -o "${1/.*/}"
}
export rtcompile
export rtcompilerun
export rtdebugcompile
```

Note the flags added through `root-config`: there are includes \(preceded by option `-I`\) and linked libraries \(following that option, and preceding output option `-o`\). Note also that flags have been added for `RooFit`. For more information about ROOT flags, see [this page](https://root.cern.ch/root/HowtoCERNLIB.html).

I give three examples of commands here, one for compiling only \(`rtcompile`\), one for compiling and executing if successful \(`rtcompilerun`\), and one for compiling with `fsanitize` activated \([`rtdebugcompile`](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html)\). The latter is useful if you want to look for memory leaks etc --- only use if you are interested in this, because it will decrease runtime. In addition, there are many issues in root \(like `TString`\) that are identified by `fsanitize`.

### Compiling on Windows 10

Although I highly recommend working on a Linux OS such as Ubuntu or Scientific Linux, there are still -certain advantages of working on Windows. As a developer, that brings problems, however, if you want to start compiling your code.

Since Windows 10, there exists an easy solution: the [Linux Subsystem](https://docs.microsoft.com/en-us/windows/wsl/install-win10). In the newest versions can be easily installed from the Windows Store \(search for "Ubuntu"\). After installing, search for "Ubuntu" in the start menu. This is a bash terminal that has full access to your windows system, but entirely through bash commands.

As such, you have access to convenient commands like `apt install`, `vi`, and `g++`. Best of all is that you can use this to [install ROOT](https://root.cern.ch/build-prerequisites). If you are having trouble installing ROOT through bash, have a look [at this script](https://github.com/redeboer/NIKHEFProject2018/blob/master/docs/Install%20CERN%20ROOT6.sh) \(ROOT6\).

For your orientation: the root path of your Linux subsystem will be located in something like:

```text
C:\Users\<your user name>\AppData\Local\Packages\CanonicalGroupLimited.UbuntuonWindows_<some identifier>\LocalState\
```

## Troubleshooting

_For the moment, I use this section to collect issues I encounter and provide a solution where possible._

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


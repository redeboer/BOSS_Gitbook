# Tips, Tricks, and Troubleshooting {#tips}

# Tips & Tricks

## Recommended software
Although I recognise that any programmer will and should develop her or his own habits, there are a few simple recommendations I would like to make, in particular for beginners.

- **[Visual Studio Code](https://code.visualstudio.com) (VScode).** Note the "Code" here---I am not refering to the infamous IDE version. Microsoft decided to strip Visual Studio of its compile capabilities and develop this editor in parallel. Since it has been made available *for free* on all OS platforms, is regularly updated, is configurable with easy-to-access `json` files, offers a growing number of user-developed exgensions, I consider VScode to be one of the most powerful code editors available on the market. Even for users who prefer not to touch the mouse, VScode has much to offer as well. (Ok, if your are really hardcore keyboard only, just go for `vim`.) **Note:** For working on the IHEP server, you will need to install this handy [SSH FS extension](https://marketplace.visualstudio.com/items?itemName=Kelvin.vscode-sshfs).
- Other access to the IHEP server through SSH/SFTP:
	- **[Xmanager](https://www.netsarang.com/products/xmg_overview.html)**
	- **[WinSCP](https://winscp.net/eng/index.php)**
	- **[PuTTY](https://www.putty.org/)**

## Compiling
For compiling outside ROOT, you will need to use a compiler like `g++`. The compiler needs to be told where the libraries for included ROOT header files are located. You can do this using flags that ROOT set during its installation. In case of `g++`, use:

	g++ YourCode.C -I$(root-config --incdir) $(root-config --libs --evelibs --glibs) -o YourBinaryOutput.o

*Pro `bash` tip:* You might like to create an easy command for this. You can do this by adding the following lines to your `~/.bashrc`.

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

Note the flags added through `root-config`: there are includes (preceded by option `-I`) and linked libraries (following that option, and preceding output option `-o`). Note also that flags have been added for `RooFit`. For more information about ROOT flags, see [this page](https://root.cern.ch/root/HowtoCERNLIB.html).

I give three examples of commands here, one for compiling only (`rtcompile`), one for compiling and executing if successful (`rtcompilerun`), and one for compiling with `fsanitize` activated ([`rtdebugcompile`](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html)). The latter is useful if you want to look for memory leaks etc --- only use if you are interested in this, because it will decrease runtime. In addition, there are many issues in root (like `TString`) that are identified by `fsanitize`.

# Troubleshooting

*For the moment, I use this section to collect issues I encounter and provide a solution where possble*

- **I lost read-write access.** One easy command to solve it: use `klog` and enter your password. You should now be able to read-write in *all* your sessions.
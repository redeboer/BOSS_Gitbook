Tips, Tricks, and Troubleshooting {#tips}
=========================================

## Tips & Tricks
Although I recognise that any programmer will and should develop her or his own habits, there are a few simple recommendations I would like to make, in particular for beginners.

- **[Visual Studio Code](https://code.visualstudio.com) (VScode).** Note the "Code" here---I am not refering to the infamous IDE version. Microsoft decided to strip Visual Studio of its compile capabilities and develop this editor in parallel. Since it has been made available *for free* on all OS platforms, is regularly updated, is configurable with easy-to-access `json` files, offers a growing number of user-developed exgensions, I consider VScode to be one of the most powerful code editors available on the market. Even for users who prefer not to touch the mouse, VScode has much to offer as well. (Ok, if your are really hardcore keyboard only, just go for `vim`.) **Note:** For working on the IHEP server, you will need to install this handy [SSH FS extension](https://marketplace.visualstudio.com/items?itemName=Kelvin.vscode-sshfs).
- Other access to the IHEP server through SSH/SFTP:
	- **[Xmanager](https://www.netsarang.com/products/xmg_overview.html)**
	- **[WinSCP](https://winscp.net/eng/index.php)**
	- **[PuTTY](https://www.putty.org/)**


## Troubleshooting
*For the moment, I use this section to collect issues I encounter and provide a solution where possble*

- **I lost read-write access.** One easy command to solve it: use `klog` and enter your password. You should now be able to read-write in *all* your sessions.
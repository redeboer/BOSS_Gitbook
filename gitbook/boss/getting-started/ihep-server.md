# Organisation of the IHEP server

The IHEP server runs on [Scientific Linux CERN](https://linux.web.cern.ch/linux/scientific.shtml) \(SLC\). The server offers several versions. Usually, people use either SLC5, SLC6, or SLC7. The domain names for these are `lxslc7.ihep.ac.cn`, where the `7` in this case refers to SLC7. If you are running on Linux or a Linux terminal, the server can be easily accessed using:

```bash
ssh -Y <your user name>@lxslc7.ihep.ac.cn
```

Here, the option `-Y` ensures _X11 forwarding_, allowing you to open graphical applications.

{% hint style="info" %}
If you don't like having to enter your password every time you log in, have a look at the section [Key generation in Tips & Tricks](../../appendices/tips.md#key-generation-for-ssh).
{% endhint %}

In Windows, there are some nice tools that allow you to access the server. First of all, to be able to use SSH, use will either have to use [PuTTY](https://www.putty.org/) or more extensive software like [Xmanager](https://www.netsarang.com/products/xmg_overview.html). You can also just search for some Linux terminals for Windows. In addition, I would recommend you to work with the \(S\)FTP client [WinSCP](https://winscp.net/eng/index.php). It allows you to easily navigate the file structure of the IHEP server and to quickly transfer---even synchronise---files up and down to your own computer.

## Main directories

When you have logged into the server, you usually start in your home \(`~`\) folder. Move to the root of the server \(`cd /`\) and you'll see that is a large number of other directories. A few of these directories contain space that is assigned to your user account. Here is an overview:

| Path | Data quotum | Max. number of files | Remark |
| :--- | :---: | :---: | :---: |
| `/afs/ihep.ac.cn/users/<letter>/$USER` | 500 MB | NA | home \(`~`\) |
| `/besfs/users/$USER` | 50 GB | 300,000 |  |
| `/ihepbatch/bes/$USER` | 200 MB | NA |  |
| `/workfs/bes/$USER` | 5 GB | 50,000 | no `hep_sub` available |
| `/scratchfs/bes/$USER` | 500 GB | NA | max. 2 weeks |

{% hint style="danger" %}
**Do not exceed these data quotas!** If you do, the folder of which you are exceeding its quota will be locked by the Computing Center after a few weeks and it is quite a hassle to regain access.
{% endhint %}

Of course, here, and in what follows, `$USER` refers to your user name.

## Important data paths

Some other important directories are the following:

* [BOSS Software directory](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/How_to_setup_BOSS_environment_on_lxslc)
  * `/afs/ihep.ac.cn/bes3/offline/Boss` \(also referred to with `$BesArea`\)
* [Raw data files](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Raw_Data)
  * `/bes3fs/offline/data/raw`
  * `/besfs2/offline/data/raw` \(only rounds 3 and 6\)
  * `/besfs/offline/data/randomtrg` \(random trigger data\)
* [Reconstructed data sets](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Production)
  * `/besfs3/offline/data/`
  * `/besfs2/offline/` \(older versions\)
* [Reconstructed Monte Carlo sets](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Jpsi_data) \(for example\):
  *  `/besfs2/offline/data/664-1/jpsi/09mc/dst` \(2009; 225M\)
  *  `/besfs2/offline/data/664-1/jpsi/12mc/dst` \(2012; 200M\)
  *  `/besfs2/offline/data/664-1/jpsi/12mc/grid/dst` \(2012; 800M\)
  * \(no MC samples available yet for 2018\)

These directories will be important later in this 'tutorial'.

**For the latest data file locations, see** [**this page**](https://docbes3.ihep.ac.cn/~offlinesoftware/index.php/Production)**.**


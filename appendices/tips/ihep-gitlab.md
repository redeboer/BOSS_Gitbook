# IHEP GitLab

IHEP supplies a GitLab server, which allows you to put your analysis code in a `git` repository. You can then enjoy all the benefits of version control, different branches to collaborate as a team, a better overview through online access, etc. The IHEP GitLab server can be accessed through [gitlab.ihep.ac.cn](http://gitlab.ihep.ac.cn/), but you will first need to apply First, for an IHEP GitLab account. You can do this by sending an email to [fanrh@ihep.ac.cn](mailto:fanrh@ihep.ac.cn).

Have a look [here](https://guides.github.com/introduction/git-handbook/) at what `git` does, it's worth it!

{% hint style="info" %}
Unfortunately, the IHEP GitLab server is only available on-campus through the LAN network. In theory, it is possible to connect through the IHEP VPN \([ssl.ihep.ac.cn](http://ssl.ihep.ac.cn)\) using EasyConnect, though to set this up, you will first need to be in that LAN network. There are plans to make the server available through the standard SSO account.
{% endhint %}

## Pushing your code to a new repository

Imagine the situation where you have already developed some code for your analysis and you want to start tracking it using `git`. Here, we go through the steps required to put that code into a repository and push it to the IHEP GitLab server.

#### Step 1: Go to the files you want to track

Go to your the folder containing your code or, alternatively, make a directory \(`mkdir`\), and add some test files there.

#### Step 2: Initialize the repository

Initialize this folder as an empty `git` repository using:

```bash
git init
```

The name of the repository that you just initialized is the name of the folder.

#### Step 3: Add the files in the directory

Files in the directory are not tracked by `git` automatically. You have to add them manually. This can be done through the `git add` command, for instance:

```bash
git add temp.sh
git add config/testfile.txt
git add src/
git add inc/*.hpp
git add .
```

You now have _staged_ these files, that is, made them ready for a commit to the repository. Files that have been added, will be tracked from then onward: if you change such a file`git` allows you to compare the changes, move back to older version, compare the file to its counterpart in parallel branches, etc.

Note that the paths are relative and that you can use `git add` from any subdirectory in the repository.

{% hint style="info" %}
#### `.gitignore`

If there are certain files you never want to track \(such as input data files or output plots\), you 'shield' them by creating a file called `.gitignore` \(note the dot\) in the main directory of the repository. This is a text file contains relative paths of the files you want to ignore. Wildcards are allowed, see [here](https://help.github.com/en/articles/ignoring-files) for more information. Now, if you use `git add .`, all new or modified files in the folder will be staged, but for the ones excluded by `.gitignore`.
{% endhint %}

#### Step 4: Commit the changes

Once you have added the files, you can make `commit` the changes using:

```bash
git commit -m "<some description>"
```

This will basically create a new point in the history of your `git` repository to which you can move back any time.

#### Step 5: Check the status of the repository

Some commands that are useful from this stage onward:

* Use `git status` to check which files have been tracked, which ones are modified compared to the previous commit, which ones removed, etc. If you added all the files you wanted to add, you can `commit` or `push`.
* Use `git log` to see the history of all your commits.
* Use `git diff <relative path>` to compare the differences in a tracked directory or file with its previous commit.
* Use `git checkout <relative path>` to retrieve the previous version of the file or directory.
* See [here](https://git-scm.com/docs) for a full reference of `git` commands.

{% hint style="info" %}
The above 5 steps are all you need to know if you just want to track your files through `git` **locally**. You do not have to work with a GitLab server, though of course this does allow for team collaboration and is the best way to backup your work.
{% endhint %}

#### Step 6: Push your new repository to the IHEP GitLab server

If you have applied for an account \(see [introduction above](ihep-gitlab.md)\), you can push this new repository to [gitlab.ihep.ac.cn](http://gitlab.ihep.ac.cn/). Initially, the repository will be public within IHEP, but we'll see later how you can make this repository private or available only to certain people.

First, add the SSH location to which you want to write your repository:

```bash
git remote add origin git@gitlab.ihep.ac.cn:<username>/<repository name>
```

Here, `<user name>` should be the one you were given when you registered, while `<repository name>` cannot contain spaces.


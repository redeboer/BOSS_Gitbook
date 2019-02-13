Jobs folder
===========

This folder contains a `bash` script framework that allows you to automatically create `jobOptions*.txt` files. These files are in turn used to submit an event selection job to queue. The framework essentially consists only two scripts: one for generating exclusive Monte Carlo simulation jobs and one for analysing DST data files (can also be inclusive of exclusive Monte Carlo files). These two scripts are supported by the `CommonFunctions.sh` source file.

In the case of Monte Carlo, you only have to specify the number of jobs, the number of events per job, and the decay card you want to use. The job options are then generated based on a job option template for the `sim` and `rec` steps in BOSS.

In the case of data analysis, you only have to specify either a path to a folder of DST files or a file containing a selection of such paths. You then specify which package you want to run and how many DST files you want to analyse per job. The file selection is then automatically split, so that you won't have one job file with an incredible amounts of DST files to be analysed.

See see section [Running jobs](https://besiii.gitbook.io/boss/working-with-boss/running-jobs) in the BOSS Gitbook for more information.

Folder for header files of the BOSS Afterburner
===============================================

This folder contains the header files for objects of the *Boss Afterburner*, that is, the framework that processes output of the event selection. **The framework is currently still under development.**

This part of the repository can be compiled and run using `make` in the base folder of the BOSS Afterburner repository. This writes binaries to the `bin` folder for each source file in the `src` folder and writes executables to the `exe` folder for each script in the `scripts` folder. You can a script such as `AnalyseBOSSOutput.C` by executing `./exe/AnalyseBOSSOutput.exe configs/<some configuration file>.txt`. Of course, that means that you will also have to specify parameters such as input file in that configuration file.

See further information [here](https://besiii.gitbook.io/boss-gitbook/working-with-the-boss-afterburner/intro).

#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer <remco.de.boer@ihep.ac.cn>
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 22 October 2018
# *         USAGE: bash CreateJobFiles.sh <package name> <number of jobs> <number of events> <output level>
# *     ARGUMENTS:
# *       1) package name (default is "D0phi_KpiKK")
# *       2) number of job files to be created (default is 25)
# *       3) number of events per job (default is 10,000)
# *       4) terminal output level (default is 4)
# *          2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL
# * ===============================================================================

set -e # exit if a command or function exits with a non-zero status

# * ===================================================== * #
# * ------- Attempt to load script with functions ------- * #
# * ===================================================== * #
	commonFunctionsScriptName="CommonFunctions.sh"
	if [ -s "${commonFunctionsScriptName}" ]; then
		source "${commonFunctionsScriptName}"
	else
		echo -e "\e[91mFATAL ERROR: Source script \"${commonFunctionsScriptName}\" does not exist\e[0m"
		exit
	fi


# ! ================================ ! #
# ! ------- Script arguments ------- ! #
# ! ================================ ! #
	# * (1) Package name
	packageName="D0phi_KpiKK" # default argument
	if [ $# -ge 1 ]; then packageName="${1}"; fi
	# * (2) number of jobOption files and submit scripts that need to be generated
	nJobs=25 # default argument
	if [ $# -ge 2 ]; then nJobs=${2}; fi
	# * (3) number of events per job
	nEventsPerJob=10000 # default argument
	if [ $# -ge 3 ]; then nEventsPerJob=${3}; fi
	# * (4) Terminal message output level
	outputLevel=4 # default argument: 4 (MSG::WARNING)
	if [ $# -ge 4 ]; then outputLevel=${4}; fi


# * ================================= * #
# * ------- Script parameters ------- * #
# * ================================= * #
	afterburnerPath="${PWD/${PWD/*BOSS_Afterburner}}" # get path of BOSS Afterburner
	scriptFolder="${afterburnerPath}/jobs" # contains templates and will write scripts to its subfolders
	outputFolder="/scratchfs/bes/${USER}/data" # rtraw, dst, root, and log files will be written to this folder


# * =============================================== * #
# * ------- Check arguments and parameters -------  * #
# * =============================================== * #
	CheckIfFolderExists "${scriptFolder}"
	CheckIfFolderExists "${outputFolder}"


# * ============================= * #
# * ------- Main function ------- * #
# * ============================= * #

	# * User input
	echo "This will create ${nJobs} \"${packageName}\" simulation and reconstruction job option files with ${nEventsPerJob} events each in job."
	echo "  --> Total number of events: $(printf "%'d" $((${nJobs} * ${nEventsPerJob})))"
	AskForInput "\nTo write job files, press ENTER, else Ctrl+C ..."

	# * Create and EMPTY scripts directory
	CreateOrEmptyDirectory "${scriptFolder}" "sim"  "${packageName}"
	CreateOrEmptyDirectory "${scriptFolder}" "rec"  "${packageName}"
	CreateOrEmptyDirectory "${scriptFolder}" "sub"  "${packageName}_mc"
	# * Create and EMPTY output directory
	CreateOrEmptyDirectory "${outputFolder}" "raw"  "${packageName}"
	CreateOrEmptyDirectory "${outputFolder}" "dst"  "${packageName}"
	CreateOrEmptyDirectory "${outputFolder}" "log"  "${packageName}"

	# * Loop over jobs
	for jobNo in $(seq 0 $((${nJobs} - 1))); do

		echo -en "\e[0K\rCreating files for job $(($jobNo+1))/${nJobs}..." # overwrite previous line

		# * Generate the simulation files (sim)
		randomSeed=$(($(date +%s%N) % 1000000000)) # random seed based on system time
		templateName="${scriptFolder}/templates/simulation.txt"
		outputFile="${scriptFolder}/sim/sim_${packageName}_${jobNo}.txt"
		CheckIfFileExists "${templateName}"
		awk '{flag = 1}
			{sub(/__SCRIPTPATH__/,"'${scriptFolder}'")}
			{sub(/__RANDSEED__/,'${randomSeed}')}
			{sub(/__DECAYCARD__/,"dec/'${packageName}'.dec")}
			{sub(/__OUTPUTPATH__/,"'${outputFolder}'")}
			{sub(/__OUTPUTFILE__/,"raw/'${packageName}'_'${jobNo}'.rtraw")}
			{sub(/__OUTPUTLEVEL__/,'${outputLevel}')}
			{sub(/__NEVENTS__/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${outputFile}"
		ChangeLineEndingsFromWindowsToUnix "${outputFile}"
		chmod +x "${outputFile}"

		# * Generate the reconstruction files (rec)
		templateName="${scriptFolder}/templates/reconstruction.txt"
		outputFile="${scriptFolder}/rec/rec_${packageName}_${jobNo}.txt"
		CheckIfFileExists "${templateName}"
		awk '{flag = 1}
			{sub(/__RANDSEED__/,'${randomSeed}')}
			{sub(/__OUTPUTLEVEL__/,'${outputLevel}')}
			{sub(/__OUTPUTPATH__/,"'${outputFolder}'")}
			{sub(/__INPUTFILE__/,"raw/'${packageName}'_'${jobNo}'.rtraw")}
			{sub(/__OUTPUTFILE__/,"dst/'${packageName}'_'${jobNo}'.dst")}
			{sub(/__NEVENTS__/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		"${templateName}" > "${outputFile}"
		ChangeLineEndingsFromWindowsToUnix "${outputFile}"
		chmod +x "${outputFile}"

		# * Generate the submit files (sub)
		outputFile="${scriptFolder}/sub/sub_${packageName}_mc_${jobNo}.sh"
		echo "#!/bin/bash" > "${outputFile}" # empty file and write first line
		echo "{ boss.exe \"${scriptFolder}/sim/sim_${packageName}_${jobNo}.txt\"; } &> \"${outputFolder}/log/sim_${packageName}_${jobNo}.log\"" >> "${outputFile}"
		echo "{ boss.exe \"${scriptFolder}/rec/rec_${packageName}_${jobNo}.txt\"; } &> \"${outputFolder}/log/rec_${packageName}_${jobNo}.log\"" >> "${outputFile}"
		ChangeLineEndingsFromWindowsToUnix "${outputFile}"
		chmod +x "${outputFile}"

	done
	echo


# * ===================================== * #
# * ------- Final terminal output ------- * #
# * ===================================== * #
	PrintSuccessMessage \
		"Succesfully created ${nJobs} \"${packageName}\" job files with ${nEventsPerJob} events each\n"

set +e # exit if a command or function exits with a non-zero status
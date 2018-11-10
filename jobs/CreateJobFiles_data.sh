#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 8 November 2018
# *         USAGE: bash CreateJobFiles.sh <number of jobs> <number of events>
# *     ARGUMENTS: 1) number of job files to be created (default is 25)
# *                2) input file that will be used to create the list of dst files (check default value)
# *                3) number of events per job (default is 10,000)
# *                4) analysis type name (default is "rhopi_data")
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
	# * (1) number of jobOption files and submit scripts that need to be generated
	nJobs=25 # default argument
	if [ $# -ge 1 ]; then
		nJobs=${1}
	fi    
	# * (2) input file that will be used to create the list of dst files
	inputFile="filenames/SelectionByDate_jpsi_round02.txt" # default argument
	if [ $# -ge 2 ]; then
		inputFile="${2}"
	fi
	# * (3) number of events per job (optional -- default is 10,000)
	nEventsPerJob=10000 # default argument
	if [ $# -ge 3 ]; then
		nEventsPerJob=${3}
	fi
	# * (4) analysis type name (optional -- default is "rhopi_data")
	analysis_type="rhopi_data" # default argument
	if [ $# -ge 4 ]; then
		analysis_type="${4}"
	fi


# * ================================= * #
# * ------- Script parameters ------- * #
# * ================================= * #
	afterburnerPath="${PWD/${PWD/*BOSS_Afterburner}}" # get path of BOSS Afterburner
	scriptFolder="${afterburnerPath}/jobs" # contains templates and will write scripts to its subfolders
	outputFolder="${afterburnerPath}/data" # rtraw, dst, root, and log files will be written to this folder
	temporaryFileName="temporary.txt" # will be removed at the end


# * =============================================== * #
# * ------- Check arguments and parameters -------  * #
# * =============================================== * #
	CheckIfFileExists   "${inputFile}"
	CheckIfFolderExists "${scriptFolder}"
	CheckIfFolderExists "${outputFolder}"


# * ============================= * #
# * ------- Main function ------- * #
# * ============================= * #

	# * User input
	echo "This will create ${nJobs} \"ana_${analysis_type}.txt\" job files with ${nEventsPerJob} events each in folder:"
	echo "   \"${scriptFolder}\"."
	echo "DST files will be loaded from the file names listed in this file:"
	echo "   \"${inputFile}\" (has $(cat ${inputFile} | wc -l) newline characters)"

	AskForInput "To continue, press ENTER, else Ctrl+C ..."

	# * Create inventory of files
	CreateFilenameInventoryFromFileOfDirectories "${inputFile}" "${temporaryFileName}" "dst"
	FormatTextFileToCppVectorArguments "${temporaryFileName}"

	# * Create and EMPTY scripts directory (no need for sim and rec in data analysis)
	CreateOrEmptyDirectory "${scriptFolder}" "ana"  "${analysis_type}"
	CreateOrEmptyDirectory "${scriptFolder}" "sub"  "${analysis_type}"
	# * Create and EMPTY output directory
	CreateOrEmptyDirectory "${outputFolder}" "raw"  "${analysis_type}"
	CreateOrEmptyDirectory "${outputFolder}" "dst"  "${analysis_type}"
	CreateOrEmptyDirectory "${outputFolder}" "root" "${analysis_type}"
	CreateOrEmptyDirectory "${outputFolder}" "log"  "${analysis_type}"

	# * Loop over jobs
	for jobNo in $(seq 0 $((${nJobs} - 1))); do

		echo -en "\e[0K\rCreating files for job $(($jobNo+1))/${nJobs}..." # overwrite previous line

		# * Generate the analyse files (ana)
		templateName="${scriptFolder}/templates/jobOptions_ana_${analysis_type}.txt"
		outputJobOptionsFile="${scriptFolder}/ana/ana_${analysis_type}_${jobNo}.txt"
		CheckIfFileExists "${templateName}"
		awk '{flag = 1}
			{sub(/ROOTFILE/,"root/ana_'${analysis_type}'_'${jobNo}'.root")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		"${templateName}" > "${outputJobOptionsFile}"
		sed -i "/DSTFILES/{
			s/DSTFILES//g
			r ${temporaryFileName}
		}" "${outputJobOptionsFile}"

		# * Generate the submit files (sub)
		templateName="${scriptFolder}/templates/submit_data.sh"
		outputScriptFile="${scriptFolder}/sub/sub_${analysis_type}_${jobNo}.sh"
		CheckIfFileExists "${templateName}"
		awk '{flag = 1}
			{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/ANA_BOS/,"ana/ana_'${analysis_type}'_'${jobNo}'.txt")}
			{sub(/ANA_LOG/,"log/ana_'${analysis_type}'_'${jobNo}'.log")}
			{if(flag == 1) {print $0} else {next} }' \
		"${templateName}" > "${outputScriptFile}"

	done
	echo


# * ===================================== * #
# * ------- Final terminal output ------- * #
# * ===================================== * #
	PrintSuccessMessage \
		"Succesfully created ${nJobs} \"${analysis_type}\" job files with ${nEventsPerJob} events each\n"
	rm "${temporaryFileName}"

set +e # exit if a command or function exits with a non-zero status
#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption_ana files based on a template. Only one
# *                DST file is used as argument for "digiRootInputFile".
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 9 November 2018
# *         USAGE: bash CreateJobFiles.sh <full path of DST file> <analysis type>
# *     ARGUMENTS: 1) DST file to analyse
# *                2) analysis type name (default is "rhopi_data")
# *                3) print terminal output? (default true) -- set to false if called in a for loop
# * ===============================================================================

set -e # exit if a command or function exits with a non-zero status

# * ===================================================== * #
# * ------- Attempt to load script with functions ------- * #
# * ===================================================== * #
	commonFunctionsScriptName="CommonFunctions.sh"
	if [ -s "${commonFunctionsScriptName}" ]; then
		source "${commonFunctionsScriptName}"
	else
		exit
	fi


# ! ================================ ! #
# ! ------- Script arguments ------- ! #
# ! ================================ ! #
	# * (1) dst file to analyse
	dstFile=${1}
	# * (2) analysis type name (optional -- default is "rhopi_data")
	analysis_type="rhopi_data" # default argument
	if [ $# -ge 2 ]; then
		analysis_type="${2}"
	fi
	# * (3) print terminal output? (default true) -- set to false if called in a for loop
	printTerminalOutput=true # default argument
	if [ $# -ge 3 ]; then
		printTerminalOutput="${3}"
	fi


# * ================================= * #
# * ------- Script parameters ------- * #
# * ================================= * #
	nEventsPerJob=10000000 # ten million, but is a max only
	afterburnerPath="${PWD/${PWD/*BOSS_Afterburner}}" # get path of BOSS Afterburner
	scriptFolder="${afterburnerPath}/jobs" # contains templates and will write scripts to its subfolders
	outputFolder="${afterburnerPath}/data" # rtraw, dst, root, and log files will be written to this folder
	suffixForFilename=$(basename ${dstFile})


# * =============================================== * #
# * ------- Check arguments and parameters -------  * #
# * =============================================== * #
	CheckIfFileExists   "${dstFile}"
	CheckIfFolderExists "${scriptFolder}"
	CheckIfFolderExists "${outputFolder}"


# * ============================= * #
# * ------- Main function ------- * #
# * ============================= * #

	# * User input
	if [ "${printTerminalOutput}" == true ]; then
		echo "This will create ${nJobs} \"ana_${analysis_type}.txt\" job files with ${nEventsPerJob} events each in folder:"
		echo "   \"${scriptFolder}\"."
		echo "This DST file will be used as argument for \"digiRootInputFile\":"
		echo "   \"${dstFile}\""
		AskForInput "To continue, press ENTER, else Ctrl+C ..."
	fi

	# * Create and EMPTY scripts directory (no need for sim and rec in data analysis)
	CreateOrEmptyDirectory "${scriptFolder}" "ana"  "${analysis_type}"
	CreateOrEmptyDirectory "${scriptFolder}" "sub"  "${analysis_type}"
	# * Create and EMPTY output directory
	CreateOrEmptyDirectory "${outputFolder}" "raw"  "${analysis_type}"
	CreateOrEmptyDirectory "${outputFolder}" "dst"  "${analysis_type}"
	CreateOrEmptyDirectory "${outputFolder}" "root" "${analysis_type}"
	CreateOrEmptyDirectory "${outputFolder}" "log"  "${analysis_type}"

	# * Prepare for insertion in C++ vector
	# dstFile="\"${dstFile}\""

	# * Generate the analyse file (ana)
	templateName="${scriptFolder}/templates/jobOptions_ana_${analysis_type}.txt"
	outputJobOptionsFile="${scriptFolder}/ana/ana_${analysis_type}_${suffixForFilename}.txt"
	CheckIfFileExists "${templateName}"
	awk '{flag = 1}
		{sub(/DSTFILES/,"'${dstFile}'")}
		{sub(/ROOTFILE/,"root/ana_'${analysis_type}'_'${suffixForFilename}'.root")}
		{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
		{sub(/NEVENTS/,'${nEventsPerJob}')}
		{if(flag == 1) {print $0} else {next} }' \
	"${templateName}" > "${outputJobOptionsFile}"

	# * Generate the submit file (sub)
	templateName="${scriptFolder}/templates/submit_data.sh"
	outputScriptFile="${scriptFolder}/sub/sub_${analysis_type}_${suffixForFilename}.sh"
	CheckIfFileExists "${templateName}"
	awk '{flag = 1}
		{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
		{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
		{sub(/ANA_BOS/,"ana/ana_'${analysis_type}'_'${suffixForFilename}'.txt")}
		{sub(/ANA_LOG/,"log/ana_'${analysis_type}'_'${suffixForFilename}'.log")}
		{if(flag == 1) {print $0} else {next} }' \
	"${templateName}" > "${outputScriptFile}"


# * ===================================== * #
# * ------- Final terminal output ------- * #
# * ===================================== * #
	if [ "${printTerminalOutput}" == true ]; then
		PrintSuccessMessage "Succesfully created ${nJobs} \"${analysis_type}\" job files with ${nEventsPerJob} events each\n"
	fi

set +e # exit if a command or function exits with a non-zero status
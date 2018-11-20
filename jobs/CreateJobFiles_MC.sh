#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 22 October 2018
# *         USAGE: bash CreateJobFiles.sh <number of jobs> <number of events>
# *     ARGUMENTS: 1) number of job files to be created (default is 25)
# *                2) number of events per job (default is 10,000)
# *                3) analysis type name (default is "rhopi_mc")
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
	# * (2) number of events per job (optional -- default is 10,000)
	nEventsPerJob=10000 # default argument
	if [ $# -ge 3 ]; then
		nEventsPerJob=${3}
	fi
	# * (3) analysis type name (optional -- default is "rhopi_mc")
	analysis_type="rhopi_mc" # default argument
	if [ $# -ge 4 ]; then
		analysis_type="${4}"
	fi


# * ================================= * #
# * ------- Script parameters ------- * #
# * ================================= * #
	afterburnerPath="${PWD/${PWD/*BOSS_Afterburner}}" # get path of BOSS Afterburner
	scriptFolder="${afterburnerPath}/jobs" # contains templates and will write scripts to its subfolders
	outputFolder="${afterburnerPath}/data" # rtraw, dst, root, and log files will be written to this folder


# * =============================================== * #
# * ------- Check arguments and parameters -------  * #
# * =============================================== * #
	CheckIfFolderExists "${scriptFolder}"
	CheckIfFolderExists "${outputFolder}"


# * ============================= * #
# * ------- Main function ------- * #
# * ============================= * #

	# * User input
	echo "This will create ${nJobs} \"ana_${analysis_type}.txt\" job files with ${nEventsPerJob} events each in folder:"
	echo "   \"${scriptFolder}\"."

	AskForInput "To continue, press ENTER, else Ctrl+C ..."

	# * Create and EMPTY scripts directory
	CreateOrEmptyDirectory "${scriptFolder}" "ana"  "${analysis_type}"
	CreateOrEmptyDirectory "${scriptFolder}" "sub"  "${analysis_type}"
	CreateOrEmptyDirectory "${scriptFolder}" "sim"  "${analysis_type}"
	CreateOrEmptyDirectory "${scriptFolder}" "rec"  "${analysis_type}"
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
			{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
			{sub(/ROOTFILE/,"root/ana_'${analysis_type}'_'${jobNo}'.root")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		"${templateName}" > "${outputJobOptionsFile}"

		# * Generate the submit files (sub)
		templateName="${scriptFolder}/templates/submit_mc.sh"
		outputScriptFile="${scriptFolder}/sub/sub_${analysis_type}_${jobNo}.sh"
		CheckIfFileExists "${templateName}"
		awk '{flag = 1}
			{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/SIM_BOS/,"sim/sim_'${analysis_type}'_'${jobNo}'.txt")}
			{sub(/SIM_LOG/,"log/sim_'${analysis_type}'_'${jobNo}'.log")}
			{sub(/REC_BOS/,"rec/rec_'${analysis_type}'_'${jobNo}'.txt")}
			{sub(/REC_LOG/,"log/rec_'${analysis_type}'_'${jobNo}'.log")}
			{sub(/ANA_BOS/,"ana/ana_'${analysis_type}'_'${jobNo}'.txt")}
			{sub(/ANA_LOG/,"log/ana_'${analysis_type}'_'${jobNo}'.log")}
			{if(flag == 1) {print $0} else {next} }' \
		"${templateName}" > "${outputScriptFile}"

		# * Generate the simulation files (sim)
		templateName="${scriptFolder}/templates/jobOptions_sim_${analysis_type}.txt"
		outputJobOptionsFile="${scriptFolder}/sim/sim_${analysis_type}_${jobNo}.txt"
		CheckIfFileExists "${templateName}"
		awk '{flag = 1}
			{sub(/RDF/,"raw/sim_'${analysis_type}'_'${jobNo}'.rtraw")}
			{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/RAND/,'$(($(date +%s%N) % 1000000000))')}
			{sub(/DEC/,"dec/'${analysis_type}'.dec")}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${outputJobOptionsFile}"

		# * Generate the reconstruction files (rec)
		templateName="${scriptFolder}/templates/jobOptions_rec_${analysis_type}.txt"
		outputJobOptionsFile="${scriptFolder}/rec/rec_${analysis_type}_${jobNo}.txt"
		CheckIfFileExists "${templateName}"
		awk '{flag = 1}
			{sub(/RDF/,"raw/sim_'${analysis_type}'_'${jobNo}'.rtraw")}
			{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/RAND/,'$(($(date +%s%N) % 1000000000))')}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		"${templateName}" > "${outputJobOptionsFile}"

	done
	echo


# * ===================================== * #
# * ------- Final terminal output ------- * #
# * ===================================== * #
	PrintSuccessMessage \
		"Succesfully created ${nJobs} \"${analysis_type}\" job files with ${nEventsPerJob} events each\n"

set +e # exit if a command or function exits with a non-zero status
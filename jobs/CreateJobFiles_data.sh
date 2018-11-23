#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 8 November 2018
# *         USAGE: bash CreateJobFiles_data.sh <search term> <analysis name> <number of events>
# *     ARGUMENTS: 1) input file that will be used to create the list of dst files,
# *                   use wild cards to add more files (check default value below)
# *                2) analysis type name (default is "rhopi_data")
# *                3) number of events per job (default is -1, i.e. all events)
# * ===============================================================================

set -e # exit if a command or function exits with a non-zero status

# * ===================================================== * #
# * ------- Attempt to load script with functions ------- * #
# * ===================================================== * #
	commonFunctionsScriptName="CommonFunctions.sh"
	if [ -s "${commonFunctionsScriptName}" ]; then
		source "${commonFunctionsScriptName}"
	else
		echo -e "\e[91mERROR: Source script \"${commonFunctionsScriptName}\" does not exist\e[0m"
		exit
	fi


# ! ================================ ! #
# ! ------- Script arguments ------- ! #
# ! ================================ ! #
	# * (1) input files that will be used to create the list of dst files
	searchTerm="filenames/besfs3_offline_data_703-1_jpsi_round02_dst_090612_*.txt"
	if [ $# -ge 1 ]; then searchTerm="${1}"; fi
	# * (2) analysis type name (optional -- default is "rhopi_data")
	analysisType="rhopi_data" # default argument
	if [ $# -ge 2 ]; then analysisType="${2}"; fi
	# * (3) number of events per job (optional -- default is -1, i.e. all events)
	nEventsPerJob=-1 # default argument
	if [ $# -ge 3 ]; then nEventsPerJob=${3}; fi


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
	ls ${searchTerm} > /dev/null
	if [ $? != 0 ]; then
		PrintErrorMessage "\nERROR: Search string\n  \"${searchTerm}\"\nhas no matches"
		exit
	fi


# * ============================= * #
# * ------- Main function ------- * #
# * ============================= * #

	# * User input * #
		echo "This will create \"ana_${analysisType}\" job files with ${nEventsPerJob} events each."
		echo "Job option files will be written to folder:"
		echo "   \"${scriptFolder}/ana\""
		echo
		echo "DST files will be loaded from the $(ls ${searchTerm} | wc -l) files matching this search pattern:"
		echo "   \"${searchTerm}\""
		AskForInput "\nTo continue, press ENTER, else Ctrl+C ..."

	# * Create and EMPTY scripts directory (no need for sim and rec in data analysis) * #
		CreateOrEmptyDirectory "${scriptFolder}" "ana"  "${analysisType}"
		CreateOrEmptyDirectory "${scriptFolder}" "sub"  "${analysisType}"
	# * Create and EMPTY output directory * #
		CreateOrEmptyDirectory "${outputFolder}" "dst"  "${analysisType}"
		CreateOrEmptyDirectory "${outputFolder}" "log"  "${analysisType}"
		CreateOrEmptyDirectory "${outputFolder}" "raw"  "${analysisType}"
		CreateOrEmptyDirectory "${outputFolder}" "root" "${analysisType}"

	# * Loop over input files * #
		jobNo=0 # set counter
		# echo ${searchTerm}
		for file in $(ls ${searchTerm}); do
			echo "Parsing \"$(basename "${file}")\""

			# * Format file for implementation into vector
				FormatTextFileToCppVectorArguments "${file}"

			# * Generate the analyse files (ana)
				templateName="${scriptFolder}/templates/jobOptions_ana_${analysisType}.txt"
				CheckIfFileExists "${templateName}"
				outputJobOptionsFile="${scriptFolder}/ana/ana_${analysisType}_${jobNo}.txt"
				# Replace simple parameters in template
				awk '{flag = 1}
					{sub(/ROOTFILE/,"root/ana_'${analysisType}'_'${jobNo}'.root")}
					{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
					{sub(/NEVENTS/,'${nEventsPerJob}')}
					{if(flag == 1) {print $0} else {next} }' \
				"${templateName}" > "${outputJobOptionsFile}"
				# Fill in vector of input DST files
				sed -i "/DSTFILES/{
					s/DSTFILES//g
					r ${file}
				}" "${outputJobOptionsFile}"
				ChangeLineEndingsFromWindowsToUnix "${outputJobOptionsFile}"

			# * Generate the submit files (sub)
				templateName="${scriptFolder}/templates/submit_data.sh"
				outputScriptFile="${scriptFolder}/sub/sub_${analysisType}_${jobNo}.sh"
				CheckIfFileExists "${templateName}"
				awk '{flag = 1}
					{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
					{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
					{sub(/ANA_BOS/,"ana/ana_'${analysisType}'_'${jobNo}'.txt")}
					{sub(/ANA_LOG/,"log/ana_'${analysisType}'_'${jobNo}'.log")}
					{if(flag == 1) {print $0} else {next} }' \
				"${templateName}" > "${outputScriptFile}"
				ChangeLineEndingsFromWindowsToUnix "${outputJobOptionsFile}"

			# * Increase counter
				jobNo=$((jobNo + 1))

		done
	echo


# * ===================================== * #
# * ------- Final terminal output ------- * #
# * ===================================== * #
	PrintSuccessMessage \
		"Succesfully created ${jobNo} \"${analysisType}\" job files with ${nEventsPerJob} events each\n"

set +e # exit if a command or function exits with a non-zero status
#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 8 November 2018
# *         USAGE: bash CreateJobFiles_data.sh <search term> <package name> <number of events>
# *     ARGUMENTS: 1) input file that will be used to create the list of dst files,
# *                   use wild cards to add more files (check default value below)
# *                2) package name (default is "RhopiAlg_data")
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
	searchTerm="filenames/besfs3_offline_data_703-1_jpsi_round02_dst_selection_*.txt"
	if [ $# -ge 1 ]; then searchTerm="${1}"; fi
	# * (2) package name (optional -- default is "RhopiAlg_data")
	packageName="RhopiAlg_data" # default argument
	if [ $# -ge 2 ]; then packageName="${2}"; fi
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
		echo "This will create \"ana_${packageName}\" job files with ${nEventsPerJob} events each."
		echo "Job option files will be written to folder:"
		echo "   \"${scriptFolder}/ana\""
		echo
		echo "DST files will be loaded from the $(ls ${searchTerm} | wc -l) files matching this search pattern:"
		echo "   \"${searchTerm}\""
		AskForInput "\nTo continue, press ENTER, else Ctrl+C ..."

	# * Create and EMPTY scripts directory (no need for sim and rec in data analysis) * #
		CreateOrEmptyDirectory "${scriptFolder}" "ana"  "${packageName}"
		CreateOrEmptyDirectory "${scriptFolder}" "sub"  "${packageName}"
	# * Create and EMPTY output directory * #
		CreateOrEmptyDirectory "${outputFolder}" "dst"  "${packageName}"
		CreateOrEmptyDirectory "${outputFolder}" "log"  "${packageName}"
		CreateOrEmptyDirectory "${outputFolder}" "raw"  "${packageName}"
		CreateOrEmptyDirectory "${outputFolder}" "root" "${packageName}"

	# * Loop over input files * #
		jobNo=0 # set counter
		# echo ${searchTerm}
		for file in $(ls ${searchTerm}); do
			echo "Parsing \"$(basename "${file}")\""

			# * Format file for implementation into vector
				FormatTextFileToCppVectorArguments "${file}"

			# * Generate the analyse files (ana)
				templateName="${scriptFolder}/templates/jobOptions_ana_${packageName}.txt"
				CheckIfFileExists "${templateName}"
				outputFile="${scriptFolder}/ana/ana_${packageName}_${jobNo}.txt"
				# Replace simple parameters in template
				awk '{flag = 1}
					{sub(/ROOTFILE/,"root/ana_'${packageName}'_'${jobNo}'.root")}
					{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
					{sub(/NEVENTS/,'${nEventsPerJob}')}
					{if(flag == 1) {print $0} else {next} }' \
				"${templateName}" > "${outputFile}"
				# Fill in vector of input DST files
				sed -i "/DSTFILES/{
					s/DSTFILES//g
					r ${file}
				}" "${outputFile}"
				ChangeLineEndingsFromWindowsToUnix "${outputFile}"
				chmod +x "${outputFile}"

			# * Generate the submit files (sub)
				templateName="${scriptFolder}/templates/submit_data.sh"
				outputFile="${scriptFolder}/sub/sub_${packageName}_${jobNo}.sh"
				CheckIfFileExists "${templateName}"
				awk '{flag = 1}
					{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
					{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
					{sub(/ANA_BOS/,"ana/ana_'${packageName}'_'${jobNo}'.txt")}
					{sub(/ANA_LOG/,"log/ana_'${packageName}'_'${jobNo}'.log")}
					{if(flag == 1) {print $0} else {next} }' \
				"${templateName}" > "${outputFile}"
				ChangeLineEndingsFromWindowsToUnix "${outputFile}"
				chmod +x "${outputFile}"

			# * Increase counter
				jobNo=$((jobNo + 1))

		done
	echo


# * ===================================== * #
# * ------- Final terminal output ------- * #
# * ===================================== * #
	PrintSuccessMessage \
		"Succesfully created ${jobNo} \"${packageName}\" job files with ${nEventsPerJob} events each\n"

set +e # exit if a command or function exits with a non-zero status
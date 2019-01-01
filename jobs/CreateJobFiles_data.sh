#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 8 November 2018
# *         USAGE: bash CreateJobFiles_data.sh <package name> <search term> <number of events> <output level>
# *     ARGUMENTS:
# *       1) package name (default is "d0phi_KpiKK")
# *       2) input file that will be used to create the list of dst files,
# *          use wild cards to add a selection of files (check default value below)
# *       3) number of events per job (default is -1, i.e. all events)
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
		echo -e "\e[91mERROR: Source script \"${commonFunctionsScriptName}\" does not exist\e[0m"
		exit
	fi


# ! ================================ ! #
# ! ------- Script arguments ------- ! #
# ! ================================ ! #
	# * (1) Package name
	packageName="d0phi_KpiKK" # default argument
	if [ $# -ge 1 ]; then packageName="${1}"; fi
	# * (2) Input files that will be used to create the list of dst files
	searchTerm="filenames/besfs3_offline_data_703-1_jpsi_round02_dst_selection_*.txt" # default argument
	if [ $# -ge 2 ]; then searchTerm="${2}"; fi
	# * (3) Number of events per job
	nEventsPerJob=-1 # default argument
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
	ls ${searchTerm} > /dev/null
	if [ $? != 0 ]; then
		PrintErrorMessage "\nERROR: Search string\n  \"${searchTerm}\"\nhas no matches"
		exit
	fi


# * ============================= * #
# * ------- Main function ------- * #
# * ============================= * #

	# * User input * #
	nJobs=$(ls ${searchTerm} | wc -l)
	# * User input * #
		echo "This will create \"${packageName}_*.txt\" analysis job option files with ${nEventsPerJob} events each."
		echo "These files will be written to folder:"
		echo "   \"${scriptFolder}/ana\""
		echo
		echo "DST files will be loaded from the ${nJobs} files matching this search pattern:"
		echo "   \"${searchTerm}\""
		if [ ${nJobs} -lt 0 ]; then
			echo
			echo "  --> Total number of events: $(printf "%'d" $((${nJobs} * ${nEventsPerJob})))"
		fi
		AskForInput "\nTo write job files, press ENTER, else Ctrl+C ..."

	# * Create and EMPTY scripts directory * #
		CreateOrEmptyDirectory "${scriptFolder}" "ana"  "${packageName}"
		CreateOrEmptyDirectory "${scriptFolder}" "sub"  "${packageName}_data"
	# * Create and EMPTY output directory * #
		CreateOrEmptyDirectory "${outputFolder}" "log"  "${packageName}"
		CreateOrEmptyDirectory "${outputFolder}" "root" "${packageName}"

	# * Loop over input files * #
		jobNo=0 # set counter
		for file in $(ls ${searchTerm}); do
			echo "Parsing \"$(basename "${file}")\""

			# * Format file for implementation into vector
				FormatTextFileToCppVectorArguments "${file}"

			# * Generate the analysis files (ana)
				templateName="${scriptFolder}/templates/analysis.txt"
				CheckIfFileExists "${templateName}"
				outputFile="${scriptFolder}/ana/ana_${packageName}_${jobNo}.txt"
				packageNameCAP=$(echo ${packageName} | awk '{print toupper($0)}') # to upper case
				# Replace simple parameters in template
				awk '{flag = 1}
					{sub(/__PACKAGENAME_CAP__/,"'${packageNameCAP}'")}
					{sub(/__PACKAGENAME__/,"'${packageName}'")}
					{sub(/__OUTPUTLEVEL__/,'${outputLevel}')}
					{sub(/__NEVENTS__/,'${nEventsPerJob}')}
					{sub(/__OUTPUTPATH__/,"'${outputFolder}'")}
					{sub(/__OUTPUTFILE__/,"root/'${packageName}'_'${jobNo}'.root")}
					{if(flag == 1) {print $0} else {next} }' \
				"${templateName}" > "${outputFile}"
				# Fill in vector of input DST files
				sed -i "/__INPUTFILES__/{
					s/__INPUTFILES__//g
					r ${file}
				}" "${outputFile}"
				ChangeLineEndingsFromWindowsToUnix "${outputFile}"
				chmod +x "${outputFile}"

			# * Generate the submit files (sub)
				outputFile="${scriptFolder}/sub/sub_${packageName}_data_${jobNo}.sh"
				echo "#!/bin/bash" > "${outputFile}" # empty file and write first line
				echo "{ boss.exe \"${scriptFolder}/ana/ana_${packageName}_${jobNo}.txt\"; } &> \"${outputFolder}/log/ana_${packageName}_${jobNo}.log\"" >> "${outputFile}"
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
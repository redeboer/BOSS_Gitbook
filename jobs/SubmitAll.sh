#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch submit jobOption files created with CreateJobFiles.sh
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 22 October 2018
# *         USAGE: bash SubmitAll.sh <analysis name>
# *     ARGUMENTS: $1 analysis name (e.g. "rhopi")
# * ===============================================================================

set -o nounset # Treat unset variables as an error

# * ------- Attempt to load script with functions ------- * #
	commonFunctionsScriptName="CommonFunctions.sh"
	if [ -s "${commonFunctionsScriptName}" ]; then
		source "${commonFunctionsScriptName}"
	else
		PrintErrorMessage "ERROR: Source script \"${commonFunctionsScriptName}\" does not exist"
		exit
	fi

# * ------- Script parameters ------- *
	analysis_type="${1}" # will be used in file naming
	afterburnerPath="${PWD/${PWD/*BOSS_Afterburner}}" # get path of BOSS Afterburner
	scriptFolder="${afterburnerPath}/jobs"


# * ------- Functions ------- *
	function CheckFolder()
	{
		folderToCheck="${1}"
		if [ ! -d ${folderToCheck} ]; then
			PrintErrorMessage "FATAL ERROR: folder \"${folderToCheck}\" does not exist. Check this script..."
			exit
		fi
	}


# * ------- Check parameters ------- *
	CheckFolder ${scriptFolder}
	nJobs=$(ls ${scriptFolder}/sub/sub_${analysis_type}_*.sh | wc -l)
	if [ ${nJobs} == 0 ]; then
		PrintErrorMessage "ERROR: No jobs of type \"${analysis_type}\" available"
		exit
	fi


# * ------- Run over all job files ------- *
	AskForInput "Press ENTER to submit ${nJobs} \"${analysis_type}\" jobs..."
	for job in $(ls ${scriptFolder}/sub/sub_${analysis_type}_*.sh); do
		chmod +x ${job} # not sure if necessary
		# hep_sub -g physics ${job}
		if [ $? != 0 ]; then
			PrintErrorMessage "Aborted submitting jobs"
			exit
		fi
	done

# * ------- Final terminal output ------- * #
	PrintSuccessMessage "Succesfully submitted ${nJobs} \"${analysis_type}\" jobs"
	echo
	echo "These are your jobs:"
	hep_q -u $USER
#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 22 October 2018
# *         USAGE: bash CreateJobFiles.sh <number of jobs> <number of events>
# *     ARGUMENTS: 1) number of job files to be created
# *                2) number of events per job (optional -- default is 10,000)
# *                3) analysis type name (optional -- default is "rhopi")
# * ===============================================================================

set -o nounset # Treat unset variables as an error

# * ------- Script parameters ------- *
	nJobs=${1} # number of jobOption files and submit scripts that need to be generated
	nEventsPerJob=10000   # default number of events per job
	if [ $# -ge 2 ]; then
		nEventsPerJob=${2}
	fi
	analysis_type="rhopi" # default value: will be used in file naming
	if [ $# -ge 3 ]; then
		analysis_type="${3}"
	fi
	afterburnerPath="${PWD/${PWD/*BOSS_Afterburner}}" # get path of BOSS Afterburner
	scriptFolder="${afterburnerPath}/jobs" # contains templates and will write scripts to its subfolders
	outputFolder="${afterburnerPath}/data"
		# rtraw, dst, root, and log files will be written to this folder


# * ------- Functions ------- *
	function CheckFolder()
	{
		folderToCheck="${1}"
		if [ ! -d ${folderToCheck} ]; then
			echo -e "\e[91mFATAL ERROR: folder \"${folderToCheck}\" does not exist. Check this script...\e[0m"
			exit
		fi
	}
	function CheckTemplateFile()
	{
		templateName="${1}"
		if [ ! -s "${templateName}" ]; then
			echo -e "\e[91mFATAL ERROR: Template file \"${templateName}\" does not exist\e[0m"
			exit
		fi
	}
	function CreateOrEmptyOutputSubDir()
	{
		if [ ! -d "${1}/${2}" ]; then
			mkdir "${1}/${2}"
		else
			rm -rf "${1}/${2}/${2}_${analysis_type}_"*".sh"       # remove sumbit scripts
			rm -rf "${1}/${2}/${2}_${analysis_type}_"*".sh."*"."* # remove log files
			rm -rf "${1}/${2}/${2}_${analysis_type}_"*".txt"      # remove jobOptions
		fi
	}


# * ------- Check parameters ------- *
	CheckFolder ${scriptFolder}
	CheckFolder ${outputFolder}


# * ------- Main function ------- *

	# * User input
	echo "This will create ${nJobs} \"${analysis_type}\" job files with ${nEventsPerJob} events each of them in folder \"${scriptFolder}\"."
	read -p "To continue, press ENTER, else Ctrl+C ..."

	# * Create and EMPTY scripts directory
	CreateOrEmptyOutputSubDir "${scriptFolder}" "sim"
	CreateOrEmptyOutputSubDir "${scriptFolder}" "rec"
	CreateOrEmptyOutputSubDir "${scriptFolder}" "ana"
	CreateOrEmptyOutputSubDir "${scriptFolder}" "sub"
	# * Create and EMPTY sutput directory
	CreateOrEmptyOutputSubDir "${outputFolder}" "raw"
	CreateOrEmptyOutputSubDir "${outputFolder}" "dst"
	CreateOrEmptyOutputSubDir "${outputFolder}" "root"
	CreateOrEmptyOutputSubDir "${outputFolder}" "log"

	# * Loop over jobs
	for jobNo in $(seq 0 $((${nJobs} - 1))); do

		# * Generate the simulation files (sim)
		CheckTemplateFile "${scriptFolder}/templates/jobOptions_sim_${analysis_type}.txt"
		awk '{flag = 1}
			{sub(/RDF/,"raw/sim_'${analysis_type}'_'${jobNo}'.rtraw")}
			{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/RAND/,'$(($(date +%s%N) % 1000000000))')}
			{sub(/DEC/,"dec/'${analysis_type}'.dec")}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${scriptFolder}/sim/sim_${analysis_type}_${jobNo}.txt"

		# * Generate the reconstruction files (rec)
		CheckTemplateFile "${scriptFolder}/templates/jobOptions_rec_${analysis_type}.txt"
		awk '{flag = 1}
			{sub(/RDF/,"raw/sim_'${analysis_type}'_'${jobNo}'.rtraw")}
			{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/RAND/,'$(($(date +%s%N) % 1000000000))')}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${scriptFolder}/rec/rec_${analysis_type}_${jobNo}.txt"

		# * Generate the analyse files (ana)
		CheckTemplateFile "${scriptFolder}/templates/jobOptions_ana_${analysis_type}.txt"
		awk '{flag = 1}
			{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
			{sub(/ROOTFILE/,"root/ana_'${analysis_type}'_'${jobNo}'.root")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${scriptFolder}/ana/ana_${analysis_type}_${jobNo}.txt"

		# * Generate the submit files (sub)
		CheckTemplateFile "${scriptFolder}/templates/submit.sh"
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
		${templateName} > "${scriptFolder}/sub/sub_${analysis_type}_${jobNo}.sh"

	done


# * ------- Final terminal output ------- *
	echo -e "\e[92mSuccesfully created ${nJobs} \"${analysis_type}\" job files with ${nEventsPerJob} events each\e[0m"
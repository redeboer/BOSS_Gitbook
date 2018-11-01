#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: Batch create jobOption files based on a template
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 22 October 2018
# *         USAGE: bash CreateJobFiles.sh <number of jobs> <number of events>
# *     ARGUMENTS: $1 number of job files to be created
# *                $2 number of events per job (optional -- default is 10,000)
# * ===============================================================================

set -o nounset # Treat unset variables as an error

# * ------- Script parameters ------- * #
	nJobs=${1}
	analysis_type="rhopi" # will be used in file naming
	rand_sim=2000         # random number generator for sim option
	nEventsPerJob=10000   # default number of events per job
	scriptFolder="/ihepbatch/bes/$USER/workarea-7.0.3/TestRelease/TestRelease-00-00-86/run"
		# rtraw, dst, root, and log files will be written to this folder
	outputFolder="/besfs/users/$USER/BOSS_Afterburner/data"
		# rtraw, dst, root, and log files will be written to this folder
	if [ $# -ge 2 ]; then # optional user input for number of events per job
		nEventsPerJob=${2}
	fi

# * ------- Check parameters ------- * #
	if [ ! -d ${scriptFolder} ]; then
		echo -e "\e[91mERROR: output folder \"${scriptFolder}\" does not exist. Check this script...\e[0m"
		exit
	fi
	if [ ! -d ${outputFolder} ]; then
		echo -e "\e[91mERROR: output folder \"${outputFolder}\" does not exist. Check this script...\e[0m"
		exit
	fi

# * ------- Functions ------- * #
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
	function CheckTemplateFile()
	{
		templateName="${1}"
		if [ ! -s "${templateName}" ]
		then
			echo "Template file \"${templateName}\" does not exist"
			exit
		fi
	}



# * ------- Main function ------- * #

	# * User input
	echo "This will create ${nJobs} \"${analysis_type}\" job files with ${nEventsPerJob} events each of them."
	read -p "To continue, press ENTER, else Ctrl+C ..."

	# * Create and EMPTY scripts directory
	CreateOrEmptyOutputSubDir "${scriptFolder}" "sim"
	CreateOrEmptyOutputSubDir "${scriptFolder}" "rec"
	CreateOrEmptyOutputSubDir "${scriptFolder}" "ana"
	CreateOrEmptyOutputSubDir "${scriptFolder}" "submit"
	# * Create and EMPTY sutput directory
	CreateOrEmptyOutputSubDir "${outputFolder}" "rtraw"
	CreateOrEmptyOutputSubDir "${outputFolder}" "dst"
	CreateOrEmptyOutputSubDir "${outputFolder}" "dst"
	CreateOrEmptyOutputSubDir "${outputFolder}" "log"

	# * Loop over jobs
	for jobNo in $(seq 0 $((${nJobs} - 1))); do

		# * Generate the simulation files (sim) *
		CheckTemplateFile "${scriptFolder}/templates/jobOptions_sim_${analysis_type}.txt"
		awk '{flag = 1}
			{sub(/RDF/,"rtraw/sim_'${analysis_type}'_'${jobNo}'.rtraw")}
			{sub(/SCRIPT_PATH/,"'${scriptFolder}'")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/RAND/,'$(($(date +%s%N) % 1000000000))')}
			{sub(/DEC/,"dec/'${analysis_type}'.dec")}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${scriptFolder}/sim/sim_${analysis_type}_${jobNo}.txt"

		# * Generate the reconstruction files (rec) *
		CheckTemplateFile "${scriptFolder}/templates/jobOptions_rec_${analysis_type}.txt"
		awk '{flag = 1}
			{sub(/RDF/,"rtraw/sim_'${analysis_type}'_'${jobNo}'.rtraw")}
			{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/RAND/,'$(($(date +%s%N) % 1000000000))')}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${scriptFolder}/rec/rec_${analysis_type}_${jobNo}.txt"

		# * Generate the analyse files (ana) *
		CheckTemplateFile "${scriptFolder}/templates/jobOptions_ana_${analysis_type}.txt"
		awk '{flag = 1}
			{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
			{sub(/ROOTFILE/,"root/ana_'${analysis_type}'_'${jobNo}'.root")}
			{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
			{sub(/NEVENTS/,'${nEventsPerJob}')}
			{if(flag == 1) {print $0} else {next} }' \
		${templateName} > "${scriptFolder}/ana/ana_${analysis_type}_${jobNo}.txt"

		# * Generate the submit files *
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
		${templateName} > "${scriptFolder}/submit/submit_${analysis_type}_${jobNo}.sh"

	done

# * ------- Final terminal output ------- * #
	echo -e "\e[92mSuccesfully created ${nJobs} \"${analysis_type}\" job files with ${nEventsPerJob} events each\e[0m"
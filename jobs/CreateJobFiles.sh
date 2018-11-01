#!/bin/bash -
# * ===============================================================================
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 22 October 2018
# *         USAGE: bash CreateJobFiles.sh <number of jobs> <number of events>
# *     ARGUMENTS: $1 number of job files to be created
# *                $2 number of events per job (optional -- default is 10,000)
# * ===============================================================================

set -o nounset # Treat unset variables as an error

# * ------- Script parameters ------- *
nJobs=${1}
analysis_type="test1" # will be used in file naming
rand_sim=2000         # random number generator for sim option
nEventsPerJob=10000   # default number of events per job
scriptFolder="/ihepbatch/bes/$USER/workarea-7.0.3/TestRelease/TestRelease-00-00-86/run"
	# rtraw, dst, root, and log files will be written to this folder
outputFolder="/besfs/users/$USER/output"
	# rtraw, dst, root, and log files will be written to this folder
if [ $# -ge 2 ]
then
	nEventsPerJob=${2} # optional user input for number of events per job
fi
if [ ! -d ${outputFolder} ]
then
	echo "ERROR: output folder \"${outputFolder}\" does not exist. Check this script..."
	exit
fi

# * ------- Functions ------- *
function CreateOrEmptyOutputSubDir()
{
	if [ ! -d "${scriptFolder}/${1}" ]
	then
		mkdir "${scriptFolder}/${1}"
	else
		rm -rf "${scriptFolder}/${1}/${1}_${analysis_type}_"*".sh"
		rm -rf "${scriptFolder}/${1}/${1}_${analysis_type}_"*".sh."*"."*
		rm -rf "${scriptFolder}/${1}/${1}_${analysis_type}_"*".txt"
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

# * ------- Create directories ------- *
# * Scripts directory
CreateOrEmptyOutputSubDir "sim"
CreateOrEmptyOutputSubDir "rec"
CreateOrEmptyOutputSubDir "ana"
CreateOrEmptyOutputSubDir "submit"
# * Output directory
CreateOrEmptyOutputSubDir "${outputFolder}/rtraw"
CreateOrEmptyOutputSubDir "${outputFolder}/dst"
CreateOrEmptyOutputSubDir "${outputFolder}/dst"
CreateOrEmptyOutputSubDir "${outputFolder}/log"


# * ------- Loop over run numbers ------- *
for jobNo in $(seq 0 $((${nJobs} - 1)))
do

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
	${templateName} > sim/sim_${analysis_type}_${jobNo}.txt

	# * Generate the reconstruction files (rec) *
	CheckTemplateFile "${scriptFolder}/templates/jobOptions_rec_${analysis_type}.txt"
	awk '{flag = 1}
		{sub(/RDF/,"rtraw/sim_'${analysis_type}'_'${jobNo}'.rtraw")}
		{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
		{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
		{sub(/RAND/,'$(($(date +%s%N) % 1000000000))')}
		{sub(/NEVENTS/,'${nEventsPerJob}')}
		{if(flag == 1) {print $0} else {next} }' \
	${templateName} > rec/rec_${analysis_type}_${jobNo}.txt

	# * Generate the analyse files (ana) *
	CheckTemplateFile "${scriptFolder}/templates/jobOptions_ana_${analysis_type}.txt"
	awk '{flag = 1}
		{sub(/DSTFILE/,"dst/rec_'${analysis_type}'_'${jobNo}'.dst")}
		{sub(/ROOTFILE/,"root/ana_'${analysis_type}'_'${jobNo}'.root")}
		{sub(/OUTPUT_PATH/,"'${outputFolder}'")}
		{sub(/NEVENTS/,'${nEventsPerJob}')}
		{if(flag == 1) {print $0} else {next} }' \
	${templateName} > ana/ana_${analysis_type}_${jobNo}.txt

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
	${templateName} > submit/submit_${analysis_type}_${jobNo}.sh

done
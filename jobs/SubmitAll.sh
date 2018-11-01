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

# * ------- Script parameters ------- *
	analysis_type="${1}" # will be used in file naming
	scriptFolder="/ihepbatch/bes/$USER/workarea-7.0.3/TestRelease/TestRelease-00-00-86/run"

# * ------- Run over all job files ------- *
	nJobs=$(ls ${scriptFolder}/submit/submit_${analysis_type}_*.sh | wc -l)
	read -p "Submitting ${nJobs} \"${analysis_type}\" jobs. Continue...?"
	for job in $(ls ${scriptFolder}/submit/submit_${analysis_type}_*.sh); do
		chmod +x ${job} # not sure if necessary
		hep_sub -g physics ${job}
		if [ $? != 0 ]; then
			echo -e "\e[91mAborted submitting jobs\e[0m"
			exit
		fi
	done

# * ------- Final terminal output ------- * #
	echo -e "\e[92mSuccesfully submitted ${nJobs} \"${analysis_type}\" jobs\e[0m\n"
	echo "These are your jobs:"
	hep_q -u $USER
#!/bin/bash -
# * ===============================================================================
# *   DESCRIPTION: This example script shows (1) how create inventory files
# *                listing dst files and (2) how to use those to create a job
# *                files for data analysis. It is desinged to illustrate the use
# *                of functions in the CommonFunctions.sh script.
# *        AUTHOR: Remco de Boer <remco.de.boer@ihep.ac.cn>
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)
# *       CREATED: 23 November 2018
# *         USAGE: bash ExampleScript_CreateDataJoboptions.sh
# * ===============================================================================

source CommonFunctions.sh

# * Scripts parameters * #
packageName="RhopiAlg"
nFilesPerJob=100
nEventsPerJob=-1
gExampleFromFile=0 #! set to 1 if you want to load your dst files from "filenames/ExampleFile_DstFiles"

# * Create job from template and submit * #
if [ ${gExampleFromFile} == 1 ]; then
	# * This will create your job files based on a file listing dst files and directories
	CreateFilenameInventoryFromFile "filenames/ExampleFile_DstFiles" "filenames/besfs3_offline_data_703-1_jpsi_round02_dst_selection.txt" ${nFilesPerJob} "dst"
	bash CreateJobFiles_data.sh "filenames/besfs3_offline_data_703-1_jpsi_round02_dst_selection_???.txt" "${packageName}_data" ${nEventsPerJob}
else
	# * This will create your job files based on a directory containing dst files
	CreateFilenameInventoryFromDirectory "/besfs3/offline/data/703-1/jpsi/round02/dst/" "filenames/besfs3_offline_data_703-1_jpsi_round02_dst.txt" ${nFilesPerJob} "dst"
	bash CreateJobFiles_data.sh "filenames/besfs3_offline_data_703-1_jpsi_round02_dst_???.txt" "${packageName}_data" ${nEventsPerJob}
fi
bash SubmitAll.sh "${packageName}_data"
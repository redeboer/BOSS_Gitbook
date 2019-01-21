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
packageName="D0phi_KpiKK"
nFilesPerJob=100
nEventsPerJob=-1
outputLevel=4
identifier="besfs3_offline_data_703-1_jpsi_round02_dst" # DzeroPhi_mc
analyseMC=1 # set to 1 if you want to analyse the DST from your simulation

# * Parameters in case of file/directory reading * #
gExampleFromFile=0 # set to 1 if you want to load your dst files from "${fileToRead}"
directoryToRead="/besfs3/offline/data/703-1/jpsi/round02/dst/"
fileToRead="filenames/ExampleFile_DstFiles" # filenames/ExampleFile_DzeroPhi_mc

# * In case of analysing Monte Carlo output * #
if [ ${analyseMC} == 1 ]; then
	gExampleFromFile=0
	nEventsPerJob=1000
	directoryToRead="/scratchfs/bes/deboer/data/dst/${packageName}_MC_1e6"
	identifier="${packageName}"
fi

# * Create job from template and submit * #
if [ ${gExampleFromFile} == 1 ]; then
	# * This will create your job files based on a file listing dst files and directories
	CreateFilenameInventoryFromFile "${fileToRead}" "filenames/${identifier}_fromfile.txt" ${nFilesPerJob} "dst"
	bash CreateJobFiles_data.sh "${packageName}" "filenames/${identifier}_fromfile_???.txt" ${nEventsPerJob} ${outputLevel}
else
	# * This will create your job files based on a directory containing dst files
	CreateFilenameInventoryFromDirectory "${directoryToRead}" "filenames/${identifier}.txt" ${nFilesPerJob} "dst"
	bash CreateJobFiles_data.sh "${packageName}" "filenames/${identifier}_???.txt" ${nEventsPerJob} ${outputLevel}
fi
bash SubmitAll.sh "${packageName}_data"
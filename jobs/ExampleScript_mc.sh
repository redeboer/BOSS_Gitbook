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
nJobs=10
nEventsPerJob=10000
outputLevel=4

# * Create job from template and submit * #
bash CreateJobFiles_mc.sh "${packageName}" ${nJobs} ${nEventsPerJob} ${outputLevel}
bash SubmitAll.sh "${packageName}_mc"
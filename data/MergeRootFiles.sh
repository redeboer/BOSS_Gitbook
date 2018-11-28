# * Script parameters *
	analysisName="${1}"
	numberOfFiles=$(ls root/ana_${analysisName}_*.root | wc -l)
	outputFile="root/ana_${analysisName}.root"
	if [ -f "${outputFile}" ]; then
		read -p "WARNING: Press ENTER to overwrite \"${outputFile}\"..."
	fi

# * Check if files exist *
	if [ numberOfFiles == 0 ]; then
		echo "No ROOT files of analysis type \"${analysisName}\""
		exit
	fi

# * Merge ROOT files *
	hadd -f "${outputFile}" $(ls root/ana_${analysisName}_*.root)

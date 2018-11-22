# * Script parameters *
	analysisName=${1}
	numberOfFiles=$(ls root/ana_${analysisName}_*.root | wc -l)

# * Check if files exist *
if [ numberOfFiles == 0 ]; then
	echo "No ROOT files of analysis type \"${analysisName}\""
	exit
fi

# * Merge ROOT files *
	hadd "plots/ana_${analysisName}.root" $(ls root/ana_${analysisName}_*.root)
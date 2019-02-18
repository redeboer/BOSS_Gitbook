# * Check environment * #
if [ "${BesArea}" == "" ]; then
	echo "ERROR: \$BesArea has bot been defined."
	echo "       Please set up BOSS again!"
	exit
fi

# * Functions * #
	function AskToOverwrite()
	{
		local PackageToOverwrite="$1"
		local SourcePath="${BesArea}/${PackageToOverwrite}"
		# * Look for latest package
		SourcePath=${SourcePath}/$(ls ${SourcePath}* | tail -1)
		if [ $? != 0 ]; then
			exit
		fi
		local VersionToOverwrite=$(basename "${SourcePath}")
		# echo "SourcePath:         $SourcePath"
		# echo "PackageToOverwrite: $PackageToOverwrite"
		# echo "VersionToOverwrite: $VersionToOverwrite"
		# * Check if package already exists in your workarea
		local TargetPath="${BOSSWORKAREA}/workarea/${PackageToOverwrite}/${VersionToOverwrite}"
		if [ -d "${TargetPath}" ]; then
			echo "WARNING: Package \"${VersionToOverwrite}\" already exists in your workarea"
			read -p "Press ENTER to overwrite..."
			rm -rf "${TargetPath}"
		fi
		mkdir -p "${TargetPath}"
		echo "Importing \"${VersionToOverwrite}\"..."
		cp -fR "${SourcePath}"* $(dirname "${TargetPath}")
	}

# * Copy example packages * #
	# AskToOverwrite "Analysis/Physics/PsiPrime"
	# AskToOverwrite "Analysis/Physics/RhopiAlg"
	AskToOverwrite "Reconstruction/DTagAlg"
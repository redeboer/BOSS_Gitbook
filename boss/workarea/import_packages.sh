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
		ls ${SourcePath}/* | tail -1
		if [ $? != 0 ]; then
			exit
		fi
		SourcePath=$(ls ${SourcePath}* | tail -1)
		PackageToOverwrite=$(basename "${SourcePath}")
		# * Check if package already exists in your workarea
		local TargetPath="${BOSSWORKAREA}/workarea/${PackageToOverwrite}"
		if [ -d "${TargetPath}" ]; then
			echo "WARNING: Package \"${PackageToOverwrite}\" already exists in your workarea"
			read -p "Press ENTER to overwrite..."
			rm -rf "${TargetPath}"
		fi
		mkdir -p "${TargetPath}"
		echo "Overwriting \"${PackageToOverwrite}\"..."
		cp -fR "${SourcePath}"* $(dirname "${TargetPath}")
	}

# * Copy example packages * #
	AskToOverwrite "Analysis/Physics/RhopiAlg"
	AskToOverwrite "TestRelease"
	AskToOverwrite "Generator/BesEvtGen"

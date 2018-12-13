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
		# * Check if package exists in $BesArea
		if [ ! -d "${SourcePath}" ]; then
			echo "ERROR: package \"${PackageToOverwrite}\" does not exist in \$BesArea!"
			exit
		fi
		local TargetPath="${BOSSWORKAREA}/workarea/${PackageToOverwrite}"
		if [ -d "${TargetPath}" ]; then
			echo "WARNING: Package \"${PackageToOverwrite}\" already exists in your workarea"
			read -p "Press ENTER to overwrite..."
			rm -rf "${TargetPath}"
		fi
		mkdir -p "${TargetPath}"
		echo "Overwriting \"${PackageToOverwrite}\"..."
		cp -fR "${SourcePath}"* "${TargetPath}"
	}

# * Copy example packages * #
	AskToOverwrite "TestRelease/TestRelease-00-00-86"
	AskToOverwrite "Analysis/Physics/RhopiAlg/RhopiAlg-00-00-23"
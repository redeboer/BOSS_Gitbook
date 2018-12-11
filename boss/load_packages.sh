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
		# * Check if package exists in $BesArea
		if [ ! -d "${BesArea}/${PackageToOverwrite}" ]; then
			echo "ERROR: package \"${PackageToOverwrite}\" does not exist in \$BesArea!"
			exit
		fi
		if [ -d "${BOSSWORKAREA}/workarea/${PackageToOverwrite}" ]; then
			echo "WARNING: Package \"${PackageToOverwrite}\" already exists in your workarea"
			read -p "Press ENTER to overwrite..."
			rm -rf "${BOSSWORKAREA}/workarea/${PackageToOverwrite}"
		fi
		echo "Overwriting \"${PackageToOverwrite}\"..."
		echo $BOSSWORKAREA
		cp –r "${BesArea}/${PackageToOverwrite}" "${BOSSWORKAREA}/workarea/$(dirname ${PackageToOverwrite})"
	}

# * Copy example packages * #
	AskToOverwrite "TestRelease/TestRelease-00-00-86"
	AskToOverwrite "Analysis/Physics/RhopiAlg/RhopiAlg-00-00-23"
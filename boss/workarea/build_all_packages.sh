# * Definition of some functions * #
	function AttemptToExecute()
	{
		local commandToExecute="${1}"
		echo -e "\n\n--== EXECUTING \"${commandToExecute}\" ==--"
		${commandToExecute}
		if [ $? != 0 ]; then
			echo "ERROR: failed to execute \"${commandToExecute}\""
			return 1
		fi
	}
	function cmtbroadcast()
	{
		# * Connect your workarea to BOSS
		AttemptToExecute "cmt broadcast"
		# * Perform setup and cleanup scripts
		AttemptToExecute "cmt config"
		# * Build and connect executables to BOSS
		AttemptToExecute "cmt broadcast make"
		# * Set bash variables
		AttemptToExecute "source setup.sh"
	}
	function cmtconfig()
	{
		# * Create CMT scripts
		AttemptToExecute "cmt config"
		# * Build executables
		AttemptToExecute "make"
		# * Make package available to BOSS
		AttemptToExecute "source setup.sh"
	}
	function performBuild()
	{
		# * Local variables
		local package="${1}"
		local whatToPerform="${2}" # cmtbroadcast or cmtconfig?
		local currentPath="$(pwd)"
		# * Perform cmt config etc.
		cd "${package}"
		"${whatToPerform}"
		cd "${currentPath}"
	}

# * Build packages (i.e. not TestRelease) * #
	echo -e "\n\n\n----==== PERFORMING CMT CONFIG ON ALL PACKAGES ====----"
	for package in $(find -name cmt -type d | grep -v TestRelease); do
		performBuild "${package}" cmtconfig
	done

# * Build TestReleases * #
	echo -e "\n\n\n--==== PERFORMING CMT BROADCAST ON ALL TESTRELEASES ====----"
	for package in $(find -name cmt -type d | grep TestRelease)
		performBuild "${package}" cmtbroadcast
	done
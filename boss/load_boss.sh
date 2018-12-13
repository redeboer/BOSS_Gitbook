# * Some BOSS variables * #
	BESFS="/besfs/users/${USER}"
	BOSSVERSION="7.0.4"
	BOSS_Afterburner="${BESFS}/BOSS_Afterburner"
	BOSSWORKAREA="${BOSS_Afterburner}/boss"
	CMTHOME="/afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-${BOSSVERSION}"
	CMTHOMENAME="cmthome"
	IHEPBATCH="/ihepbatch/bes/${USER}"
	SCRATCHFS="/scratchfs/bes/${USER}"
	WORKAREANAME="workarea"
	export BESFS
	export BOSSVERSION
	export BOSS_Afterburner
	export BOSSWORKAREA
	export IHEPBATCH
	export SCRATCHFS
	# export CMTHOME # ! do not export, inconsistent with GaudiPolicy

# * Setup BOSS area * #
	source "${BOSSWORKAREA}/${CMTHOMENAME}/setupCMT.sh"
	source "${BOSSWORKAREA}/${CMTHOMENAME}/setup.sh"
	source "${BOSSWORKAREA}/${WORKAREANAME}/TestRelease/TestRelease-00-01-00/cmt/setup.sh"
	export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/


# * Navigation functions * #
	function cdafterburner()
	{
		local subfolder="${1}"
		if [ $# == 0 ] ; then
			subfolder=""
		fi
		cd "${BOSS_Afterburner}/${subfolder}"
	}
	export cdafterburner # navigate to BOSS Afterburner (optional argument: subfolder)
	function cdbesfs()
	{
		local subfolder="${1}"
		if [ $# == 0 ] ; then
			subfolder=""
		fi
		cd "${BESFS}/${subfolder}"
	}
	export cdbesfs # navigate to besfs folder (optional argument: subfolder)
	function cdihepbatch()
	{
		local subfolder="${1}"
		if [ $# == 0 ] ; then
			subfolder=""
		fi
		cd "${IHEPBATCH}/${subfolder}"
	}
	export cdihepbatch # navigate to ihepbatch folder (optional argument: subfolder)
	function cdscratchfs()
	{
		local subfolder="${1}"
		if [ $# == 0 ] ; then
			subfolder=""
		fi
		cd "${SCRATCHFS}/${subfolder}"
	}
	export cdscratchfs # navigate to scratchfs folder (optional argument: subfolder)
	function cdworkarea()
	{
		local subfolder="${1}"
		if [ $# == 0 ] ; then
			subfolder=""
		fi
		cd "${BOSSWORKAREA}/workarea/${subfolder}"
	}
	export cdworkarea # navigate to workarea (optional argument: subfolder)

# * Some BOSS functions * #
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
	export AttemptToExecute
	function cmtbroadcast()
	{
		# * Connect your workarea to BOSS
		AttemptToExecute "cmt broadcast"
		if [ $? != 0 ]; then return; fi
		# * Perform setup and cleanup scripts
		AttemptToExecute "cmt config"
		if [ $? != 0 ]; then return; fi
		# * Build and connect executables to BOSS
		AttemptToExecute "cmt broadcast make"
		if [ $? != 0 ]; then return; fi
		# * Set bash variables
		AttemptToExecute "source setup.sh"
		if [ $? != 0 ]; then return; fi
	}
	export cmtbroadcast
	function cmtconfig()
	{
		# * Create CMT scripts
		AttemptToExecute "cmt config"
		if [ $? != 0 ]; then return; fi
		# * Build executables
		AttemptToExecute "make"
		if [ $? != 0 ]; then return; fi
		# * Make package available to BOSS
		AttemptToExecute "source setup.sh"
		if [ $? != 0 ]; then return; fi
	}
	export cmtconfig
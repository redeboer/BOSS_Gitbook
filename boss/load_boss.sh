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
# export CMTHOME # ! do not export, inconsistent with GaudiPolicy
export IHEPBATCH
export SCRATCHFS

# * Setup BOSS area * #
source "${BOSSWORKAREA}/${CMTHOMENAME}/setupCMT.sh"
source "${BOSSWORKAREA}/${CMTHOMENAME}/setup.sh"
source "${BOSSWORKAREA}/${WORKAREANAME}/TestRelease/TestRelease-"*"/cmt/setup.sh"
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/
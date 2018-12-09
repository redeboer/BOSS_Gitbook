# echo "setup cmt_standalone v0 in /besfs/users/deboer/BOSS_Afterburner/boss/cmthome"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtcmt_standalonetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtcmt_standalonetempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=cmt_standalone -version=v0 -path=/besfs/users/deboer/BOSS_Afterburner/boss/cmthome  -no_cleanup $* >${cmtcmt_standalonetempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=cmt_standalone -version=v0 -path=/besfs/users/deboer/BOSS_Afterburner/boss/cmthome  -no_cleanup $* >${cmtcmt_standalonetempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtcmt_standalonetempfile}
  unset cmtcmt_standalonetempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtcmt_standalonetempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtcmt_standalonetempfile}
unset cmtcmt_standalonetempfile
exit $cmtsetupstatus


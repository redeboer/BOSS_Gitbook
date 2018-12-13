# echo "setup TestRelease TestRelease-00-00-86 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtTestReleasetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtTestReleasetempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=TestRelease -version=TestRelease-00-00-86 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea  -no_cleanup $* >${cmtTestReleasetempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=TestRelease -version=TestRelease-00-00-86 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea  -no_cleanup $* >${cmtTestReleasetempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtTestReleasetempfile}
  unset cmtTestReleasetempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtTestReleasetempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtTestReleasetempfile}
unset cmtTestReleasetempfile
exit $cmtsetupstatus


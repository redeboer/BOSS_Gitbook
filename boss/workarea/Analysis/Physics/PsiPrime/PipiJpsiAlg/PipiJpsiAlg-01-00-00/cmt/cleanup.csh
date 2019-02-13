# echo "cleanup PipiJpsiAlg PipiJpsiAlg-00-00-03 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics/PsiPrime"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtPipiJpsiAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtPipiJpsiAlgtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=PipiJpsiAlg -version=PipiJpsiAlg-00-00-03 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics/PsiPrime  $* >${cmtPipiJpsiAlgtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=PipiJpsiAlg -version=PipiJpsiAlg-00-00-03 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics/PsiPrime  $* >${cmtPipiJpsiAlgtempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtPipiJpsiAlgtempfile}
  unset cmtPipiJpsiAlgtempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtPipiJpsiAlgtempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtPipiJpsiAlgtempfile}
unset cmtPipiJpsiAlgtempfile
exit $cmtcleanupstatus


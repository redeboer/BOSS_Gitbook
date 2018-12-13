# echo "cleanup KGamma KGamma-00-00-01 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtKGammatempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtKGammatempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=KGamma -version=KGamma-00-00-01 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtKGammatempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=KGamma -version=KGamma-00-00-01 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtKGammatempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtKGammatempfile}
  unset cmtKGammatempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtKGammatempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtKGammatempfile}
unset cmtKGammatempfile
exit $cmtcleanupstatus


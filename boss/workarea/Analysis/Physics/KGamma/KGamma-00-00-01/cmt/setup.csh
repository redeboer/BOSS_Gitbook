# echo "setup KGamma KGamma-00-00-01 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtKGammatempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtKGammatempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=KGammaversion=KGamma-00-00-01 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtKGammatempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=KGammaversion=KGamma-00-00-01 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtKGammatempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtKGammatempfile}
  unset cmtKGammatempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtKGammatempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtKGammatempfile}
unset cmtKGammatempfile
exit $cmtsetupstatus


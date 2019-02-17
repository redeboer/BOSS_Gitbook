# echo "setup rhopi_pipigg rhopi_pipigg-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtrhopi_pipiggtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtrhopi_pipiggtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=rhopi_pipigg -version=rhopi_pipigg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtrhopi_pipiggtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=rhopi_pipigg -version=rhopi_pipigg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtrhopi_pipiggtempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtrhopi_pipiggtempfile}
  unset cmtrhopi_pipiggtempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtrhopi_pipiggtempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtrhopi_pipiggtempfile}
unset cmtrhopi_pipiggtempfile
exit $cmtsetupstatus


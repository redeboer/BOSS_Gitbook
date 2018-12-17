# echo "cleanup DzeroPhi DzeroPhi-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtDzeroPhitempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtDzeroPhitempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=DzeroPhi -version=DzeroPhi-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtDzeroPhitempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=DzeroPhi -version=DzeroPhi-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtDzeroPhitempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtDzeroPhitempfile}
  unset cmtDzeroPhitempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtDzeroPhitempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtDzeroPhitempfile}
unset cmtDzeroPhitempfile
exit $cmtcleanupstatus


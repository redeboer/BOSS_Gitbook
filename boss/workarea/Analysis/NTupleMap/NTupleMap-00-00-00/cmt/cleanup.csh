# echo "cleanup NTupleMap NTupleMap-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtNTupleMaptempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtNTupleMaptempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=NTupleMap -version=NTupleMap-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtNTupleMaptempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=NTupleMap -version=NTupleMap-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtNTupleMaptempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtNTupleMaptempfile}
  unset cmtNTupleMaptempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtNTupleMaptempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtNTupleMaptempfile}
unset cmtNTupleMaptempfile
exit $cmtcleanupstatus


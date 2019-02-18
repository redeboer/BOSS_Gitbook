# echo "cleanup DTagAlg DTagAlg-00-01-05 in /afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Reconstruction"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtDTagAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtDTagAlgtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=DTagAlg -version=DTagAlg-00-01-05 -path=/afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Reconstruction  $* >${cmtDTagAlgtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=DTagAlg -version=DTagAlg-00-01-05 -path=/afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Reconstruction  $* >${cmtDTagAlgtempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtDTagAlgtempfile}
  unset cmtDTagAlgtempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtDTagAlgtempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtDTagAlgtempfile}
unset cmtDTagAlgtempfile
exit $cmtcleanupstatus


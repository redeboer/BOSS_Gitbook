# echo "cleanup DTagAlg DTagAlg-00-01-05 in /afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Reconstruction"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDTagAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDTagAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=DTagAlg -version=DTagAlg-00-01-05 -path=/afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Reconstruction  $* >${cmtDTagAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=DTagAlg -version=DTagAlg-00-01-05 -path=/afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Reconstruction  $* >${cmtDTagAlgtempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtDTagAlgtempfile}
  unset cmtDTagAlgtempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtDTagAlgtempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtDTagAlgtempfile}
unset cmtDTagAlgtempfile
return $cmtcleanupstatus


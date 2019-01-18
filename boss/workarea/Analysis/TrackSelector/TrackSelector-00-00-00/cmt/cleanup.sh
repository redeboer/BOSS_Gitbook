# echo "cleanup TrackSelector TrackSelector-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtTrackSelectortempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtTrackSelectortempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=TrackSelector -version=TrackSelector-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  $* >${cmtTrackSelectortempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=TrackSelector -version=TrackSelector-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  $* >${cmtTrackSelectortempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtTrackSelectortempfile}
  unset cmtTrackSelectortempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtTrackSelectortempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtTrackSelectortempfile}
unset cmtTrackSelectortempfile
return $cmtcleanupstatus


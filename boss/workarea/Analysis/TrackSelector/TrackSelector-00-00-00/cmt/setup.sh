# echo "setup TrackSelector TrackSelector-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtTrackSelectortempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtTrackSelectortempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=TrackSelector -version=TrackSelector-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  -no_cleanup $* >${cmtTrackSelectortempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=TrackSelector -version=TrackSelector-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  -no_cleanup $* >${cmtTrackSelectortempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtTrackSelectortempfile}
  unset cmtTrackSelectortempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtTrackSelectortempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtTrackSelectortempfile}
unset cmtTrackSelectortempfile
return $cmtsetupstatus


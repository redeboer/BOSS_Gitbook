# echo "cleanup TrackSelector TrackSelector-02-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtTrackSelectortempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtTrackSelectortempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=TrackSelector -version=TrackSelector-02-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  $* >${cmtTrackSelectortempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=TrackSelector -version=TrackSelector-02-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  $* >${cmtTrackSelectortempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtTrackSelectortempfile}
  unset cmtTrackSelectortempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtTrackSelectortempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtTrackSelectortempfile}
unset cmtTrackSelectortempfile
exit $cmtcleanupstatus


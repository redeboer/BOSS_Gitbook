# echo "setup TrackSelector TrackSelector-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtTrackSelectortempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtTrackSelectortempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=TrackSelector -version=TrackSelector-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  -no_cleanup $* >${cmtTrackSelectortempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=TrackSelector -version=TrackSelector-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis  -no_cleanup $* >${cmtTrackSelectortempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtTrackSelectortempfile}
  unset cmtTrackSelectortempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtTrackSelectortempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtTrackSelectortempfile}
unset cmtTrackSelectortempfile
exit $cmtsetupstatus


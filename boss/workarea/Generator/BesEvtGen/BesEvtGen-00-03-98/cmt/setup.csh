# echo "setup BesEvtGen BesEvtGen-00-03-98 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Generator"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtBesEvtGentempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtBesEvtGentempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=BesEvtGen -version=BesEvtGen-00-03-98 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Generator  -no_cleanup $* >${cmtBesEvtGentempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=BesEvtGen -version=BesEvtGen-00-03-98 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Generator  -no_cleanup $* >${cmtBesEvtGentempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtBesEvtGentempfile}
  unset cmtBesEvtGentempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtBesEvtGentempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtBesEvtGentempfile}
unset cmtBesEvtGentempfile
exit $cmtsetupstatus


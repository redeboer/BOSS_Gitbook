# echo "setup DzeroPhi DzeroPhi-01-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtDzeroPhitempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtDzeroPhitempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=DzeroPhi -version=DzeroPhi-01-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtDzeroPhitempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=DzeroPhi -version=DzeroPhi-01-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtDzeroPhitempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtDzeroPhitempfile}
  unset cmtDzeroPhitempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtDzeroPhitempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtDzeroPhitempfile}
unset cmtDzeroPhitempfile
exit $cmtsetupstatus


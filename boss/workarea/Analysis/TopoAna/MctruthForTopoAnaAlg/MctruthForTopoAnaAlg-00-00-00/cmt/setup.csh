# echo "setup MctruthForTopoAnaAlg MctruthForTopoAnaAlg-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtMctruthForTopoAnaAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtMctruthForTopoAnaAlgtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=MctruthForTopoAnaAlg -version=MctruthForTopoAnaAlg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna  -no_cleanup $* >${cmtMctruthForTopoAnaAlgtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=MctruthForTopoAnaAlg -version=MctruthForTopoAnaAlg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna  -no_cleanup $* >${cmtMctruthForTopoAnaAlgtempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtMctruthForTopoAnaAlgtempfile}
  unset cmtMctruthForTopoAnaAlgtempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtMctruthForTopoAnaAlgtempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtMctruthForTopoAnaAlgtempfile}
unset cmtMctruthForTopoAnaAlgtempfile
exit $cmtsetupstatus


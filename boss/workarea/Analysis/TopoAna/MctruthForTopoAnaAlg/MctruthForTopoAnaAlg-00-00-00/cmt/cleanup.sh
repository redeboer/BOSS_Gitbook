# echo "cleanup MctruthForTopoAnaAlg MctruthForTopoAnaAlg-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtMctruthForTopoAnaAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtMctruthForTopoAnaAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=MctruthForTopoAnaAlg -version=MctruthForTopoAnaAlg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna  $* >${cmtMctruthForTopoAnaAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=MctruthForTopoAnaAlg -version=MctruthForTopoAnaAlg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna  $* >${cmtMctruthForTopoAnaAlgtempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtMctruthForTopoAnaAlgtempfile}
  unset cmtMctruthForTopoAnaAlgtempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtMctruthForTopoAnaAlgtempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtMctruthForTopoAnaAlgtempfile}
unset cmtMctruthForTopoAnaAlgtempfile
return $cmtcleanupstatus


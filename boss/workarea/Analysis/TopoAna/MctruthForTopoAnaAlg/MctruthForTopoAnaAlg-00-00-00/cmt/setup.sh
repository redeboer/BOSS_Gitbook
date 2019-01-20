# echo "setup MctruthForTopoAnaAlg MctruthForTopoAnaAlg-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtMctruthForTopoAnaAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtMctruthForTopoAnaAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=MctruthForTopoAnaAlg -version=MctruthForTopoAnaAlg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna  -no_cleanup $* >${cmtMctruthForTopoAnaAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=MctruthForTopoAnaAlg -version=MctruthForTopoAnaAlg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/TopoAna  -no_cleanup $* >${cmtMctruthForTopoAnaAlgtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtMctruthForTopoAnaAlgtempfile}
  unset cmtMctruthForTopoAnaAlgtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtMctruthForTopoAnaAlgtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtMctruthForTopoAnaAlgtempfile}
unset cmtMctruthForTopoAnaAlgtempfile
return $cmtsetupstatus


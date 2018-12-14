# echo "setup KGamma KGamma-00-00-01 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtKGammatempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtKGammatempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=KGamma -version=KGamma-00-00-01 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtKGammatempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=KGamma -version=KGamma-00-00-01 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtKGammatempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtKGammatempfile}
  unset cmtKGammatempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtKGammatempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtKGammatempfile}
unset cmtKGammatempfile
return $cmtsetupstatus


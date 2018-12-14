# echo "cleanup JpsiDzeroOmega JpsiDzeroOmega-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtJpsiDzeroOmegatempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtJpsiDzeroOmegatempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=JpsiDzeroOmega -version=JpsiDzeroOmega-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtJpsiDzeroOmegatempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=JpsiDzeroOmega -version=JpsiDzeroOmega-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  $* >${cmtJpsiDzeroOmegatempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtJpsiDzeroOmegatempfile}
  unset cmtJpsiDzeroOmegatempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtJpsiDzeroOmegatempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtJpsiDzeroOmegatempfile}
unset cmtJpsiDzeroOmegatempfile
return $cmtcleanupstatus


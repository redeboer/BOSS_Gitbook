# echo "setup DzeroOmega DzeroOmega-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDzeroOmegatempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDzeroOmegatempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=DzeroOmega -version=DzeroOmega-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtDzeroOmegatempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=DzeroOmega -version=DzeroOmega-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtDzeroOmegatempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtDzeroOmegatempfile}
  unset cmtDzeroOmegatempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtDzeroOmegatempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtDzeroOmegatempfile}
unset cmtDzeroOmegatempfile
return $cmtsetupstatus


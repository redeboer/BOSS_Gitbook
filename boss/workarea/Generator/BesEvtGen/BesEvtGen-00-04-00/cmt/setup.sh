# echo "setup BesEvtGen BesEvtGen-00-04-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Generator"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtBesEvtGentempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtBesEvtGentempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=BesEvtGen -version=BesEvtGen-00-04-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Generator  -no_cleanup $* >${cmtBesEvtGentempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=BesEvtGen -version=BesEvtGen-00-04-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Generator  -no_cleanup $* >${cmtBesEvtGentempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtBesEvtGentempfile}
  unset cmtBesEvtGentempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtBesEvtGentempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtBesEvtGentempfile}
unset cmtBesEvtGentempfile
return $cmtsetupstatus


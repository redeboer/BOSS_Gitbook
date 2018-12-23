# echo "setup DzeroPhi DzeroPhi-01-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDzeroPhitempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDzeroPhitempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=DzeroPhi -version=DzeroPhi-01-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtDzeroPhitempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=DzeroPhi -version=DzeroPhi-01-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtDzeroPhitempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtDzeroPhitempfile}
  unset cmtDzeroPhitempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtDzeroPhitempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtDzeroPhitempfile}
unset cmtDzeroPhitempfile
return $cmtsetupstatus


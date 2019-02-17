# echo "setup rhopi_pipigg rhopi_pipigg-00-00-00 in /besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtrhopi_pipiggtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtrhopi_pipiggtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=rhopi_pipigg -version=rhopi_pipigg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtrhopi_pipiggtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=rhopi_pipigg -version=rhopi_pipigg-00-00-00 -path=/besfs/users/deboer/BOSS_Afterburner/boss/workarea/Analysis/Physics  -no_cleanup $* >${cmtrhopi_pipiggtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtrhopi_pipiggtempfile}
  unset cmtrhopi_pipiggtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtrhopi_pipiggtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtrhopi_pipiggtempfile}
unset cmtrhopi_pipiggtempfile
return $cmtsetupstatus


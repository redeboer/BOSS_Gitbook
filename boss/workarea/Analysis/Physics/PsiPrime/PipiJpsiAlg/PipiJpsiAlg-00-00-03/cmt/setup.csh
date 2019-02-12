# echo "setup PipiJpsiAlg PipiJpsiAlg-00-00-03 in /afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Analysis/Physics/PsiPrime"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtPipiJpsiAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtPipiJpsiAlgtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=PipiJpsiAlg -version=PipiJpsiAlg-00-00-03 -path=/afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Analysis/Physics/PsiPrime  -no_cleanup $* >${cmtPipiJpsiAlgtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=PipiJpsiAlg -version=PipiJpsiAlg-00-00-03 -path=/afs/.ihep.ac.cn/bes3/offline/Boss/7.0.4/Analysis/Physics/PsiPrime  -no_cleanup $* >${cmtPipiJpsiAlgtempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtPipiJpsiAlgtempfile}
  unset cmtPipiJpsiAlgtempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtPipiJpsiAlgtempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtPipiJpsiAlgtempfile}
unset cmtPipiJpsiAlgtempfile
exit $cmtsetupstatus


# echo "cleanup BesEvtGen BesEvtGen-00-03-90 in /afs/ihep.ac.cn/users/m/marq/ihepbatch/workarea703/physics/Generator"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtBesEvtGentempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtBesEvtGentempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=BesEvtGen -version=BesEvtGen-00-03-90 -path=/afs/ihep.ac.cn/users/m/marq/ihepbatch/workarea703/physics/Generator  $* >${cmtBesEvtGentempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=BesEvtGen -version=BesEvtGen-00-03-90 -path=/afs/ihep.ac.cn/users/m/marq/ihepbatch/workarea703/physics/Generator  $* >${cmtBesEvtGentempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtBesEvtGentempfile}
  unset cmtBesEvtGentempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtBesEvtGentempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtBesEvtGentempfile}
unset cmtBesEvtGentempfile
exit $cmtcleanupstatus


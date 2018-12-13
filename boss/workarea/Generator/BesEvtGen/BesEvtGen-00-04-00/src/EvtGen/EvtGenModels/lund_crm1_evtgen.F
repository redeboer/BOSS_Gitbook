C//--------------------------------------------------------------------------
C//
C// Environment:
C//      This software is part of models developed at BES collaboration
C//      based on the EvtGen framework.  If you use all or part
C//      of it, please give an appropriate acknowledgement.
C//
C// Copyright Information: See EvtGen/BesCopyright
C//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP
C//
C// Module: lund_crm1_evtgen.F
C//
C// Description: Modified Lund model at tau-charm energy level, see 
C//               PHYSICAL REVIEW D, VOLUME 62, 034003
C// Modification history:
C//
C//   Ping R.-G. Chen J.-C.   Octo., 2007       Module created from BESII
C//
C//------------------------------------------------------------------------
       

        SUBROUTINE LUNDCRM(numevt,pid,sqrts,ndaug,kf,km,px,py,pz,e,flag)
C***********************************************************************
C  lund_crm.F 08/21/2001  Chen J.C. 
C  EVT_TYPE = 31    (for SIMBES)
c (A) Generator for BOTH Charmonium and continuum energy hadron decay
c      J/psi-->Hadrons       Ecm=3.0969+-0.0002GeV, or Ecm=3.096+-0.0001GeV
c      psi(2S) --> hadrons   Ecm=3.686+-0.0002 GeV)
c (B)  for e+e- --> gamma* --> hadrons
c      (Ecm=other energy), call LUND and mixing D meson with Eichten model
c (C)  or MPARAM(1) = 1000,
c      only call LUND for continuum energy hadron decay
c (D)  or MPARAM(1) = 2000,
c      only call EICHTEN for Charm mesons(DD~,DD*~...) decay
c    Characters in this Generator are as follows:
C     (1) make particle ID match between SOBER and jetset74.
C     (2) Let particle decay in the beam pipe.
C     (3) check if a particle which SOBER unincluded, run again. 
C    *not use(4) LULIST1, modified from jetset7.4's LULIST for there
C    *     is a same name subroutine in the BESLIB  kroalb.f.
C     (5) Wiggle EBEAM is by a Sober card 'sig_bm SIGEBM'.
C     (6) Related subroutines are as floows
C           lucharm.f    for J/psi and psi(2S) decay
C              |-->luexec1.f-->ludecy1-->luonia1(rgg/ggg decay)
C     (7) J/psi decay channel input by PDG(98)
C     (8) psi(2S) decay cheannel input by PDG(96)
C     (9) Input parameters by SOBER card to control final channel as follows
c        MPARAM(1) = +-1, N,part1_ID,part1_Charge,part2_ID,part2_charge,....
C            J/psi(psi(2S))--> P1,P2...
c            Decay modes MPARAM(1)= +1/-1 = out only/cancel stable
c            N=Number. of final particles, 
C            part_ID=ID of final particle in xmchty.inc
C                    for Example: gamma=1,e=2,mu=3,pi=4,K=5,p=6...
c            part_Charge= Charge of final particle.
c        MPARAM(1) = +-2, N,part1_ID,part2_ID,,....
C            J/psi(psi(2S))--> P1(resonance),P2(resonance)...
c            Decay modes MPARAM(1) = +2 = out only resonance of 1st decay  level
c            Decay modes MPARAM(1) = -2 = cancel resonance of 1st decay level
c            Decay modes 2=resonance of 1st decay level
c            N=Number. of final particles, 
C            part_ID=ID of final particle in Jetset7.4
C                    for Example:gamma=22,e+-=+-11,mu+-=+-133,
c                                pi+-=+-211,pi0=111,
c                                K+-=321,K0/K0~=311/-311 ...
c        MPARAM(1) = +-3, N,part1_ID,part2_ID,,....
C            J/psi(psi(2S))--> P1(resonance),P2(resonance)...+X
c            Decay modes MPARAM(1) = +3 = out resonance of 1st decay level
c            Decay modes MPARAM(1) = -3 = cancel resonance of 1st decay level
c            N=Number. of final particles, 
C            part_ID=ID of final particle in Jetset7.4
c        MPARAM(1) = +-4,N
C            J/psi(psi(2S))--> Nch  stable partcles
c            Decay modes MPARAM(1)= +1/-1 = out only/cancel stable
c            N=Number of final charged particles, 
c (E)  lepton pair channels are closed in psi(2S).
c                          e+e-    mu+mu-  tau+tau- (1/0 = on/off)
c         MPARAM(1) = 3000  1       1        1
c    
c (F)  lepton pair channels are closed in jpsi
c                          e+e-    mu+mu- (1/0 = on/off)
c         MPARAM(1) = 4000  1       1
c     (10) Defult MPARAM(1)=0 and decay to all of possible channels
c
C                                       Chen J.C. 99/01/26
c  
C   Modified:
C   (1)  Random seed input from SOBER card works to JETSET's random seed 
C  MRLU(1) too.       
C                     Chen J.C. 01/03/08 
C   (2)  Add MPARAM(1)=-1,-2,-3 selection to cancel the output modes
c   (3) Add mass_width = 0.08GeV to f0,  f0(mass<1GeV)->pipi f0(m>1GeV)->KK
c       (luexec1.F->ludecy1,charm_updat)
C                              Chen J.C. 07/11/2002
c   (4) Add angular distribution for J/psi->e+e-/mu+mu-            
c          1+A*cos^2(theta), where A(Def=1.0 is inputed by XPARAM(2).
c                                         Chen Jiangchuan 2003.10.10 
c   (5)  Add sign on IDTRK to identify Particle and Antiparticle
c        For example: Lambda0/Lambda0~ = LUND(3122,-3122) -> SOBER(7,-7)
c        Modify:   idtrk=J -> IDTRK=J*ISIGN(1,K(I,2))
c           This bug made Lambda0~  to be sent to GEANT as Lambda0.
c                            Chen Jiangchuan March 23, 2004       
c
C ----------------------------------------------------------
C     P(5)---- MOMUMTUM, ENERGY, MASS OF PARTICLE.
C                  P(1)  P(2)  P(3)  P(4)  P(5)
C                  PX    PY    PZ     E      M
C----------------- Pythia parameters optimzed with J/psi data, see talk by RangRuiling at BESIII Collaboration 2013 Spring Meeting
c  http://indico.ihep.ac.cn/getFile.py/access?contribId=36&sessionId=9&resId=0&materialId=slides&confId=3077
c
      implicit none

      integer numevt,status,flag,flag_temp
      real sigebm/0.001/

      integer ndmax,pid,ip
      parameter (ndmax=100)

      integer kf(ndmax),ndaug,km(ndmax)
      integer kp,kid,ipart1,ipart2,ICKF,KFstring,KFmother
      real*8  m,px(ndmax),py(ndmax),pz(ndmax),e(ndmax)

      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)   !jetset V7.4
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) !jetset v7.4
        integer N,K,mstu,mstj
        real p,v,paru,parj, Rdchannel(10),Rlund,Rtot,RR,R_d
        real ddn,ddsn,dsdsn,sqrts0
      integer new_charm,i,j,ij,idsign,Nmpa,nds,mli, makeline(2),
     &  ipart(2,20),nloop,ij0
      save new_charm, Rdchannel,Rlund,ddn,ddsn,dsdsn
      data new_charm/0/
      data Rdchannel/10*0./
      LOGICAL FIRST /.TRUE./    !MHK  Initialization flag
      COMMON/LUDAT3/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5)
      common/charm/mdcy_charm(2),mdme_charm(100,2), brat_charm(100),
     &  kfdp_charm(100,5)
       integer mdcy,mdme,kfdp,mdcy_charm,mdme_charm,kfdp_charm
       real brat,brat_charm
       SAVE /LUJETS/,/LUDAT1/,/LUDAT3/,/ludatr/      
       real RLU
        external  RLU
       common/ludatr/mrlu(6),rrlu(100) !seed of LUND random number generator
       integer nevtlist/10/,mrlu
cjc       INTEGER PIDTR, PID1  !PID transform from lundcharm to PDG2006
	  REAL tecm,ecm,sqrts,rrlu,totalE
        common/lundseed/ilundseed
        integer ilundseed
        external ranlux 
        real r1
        logical flagmother, PDGmode, non_KF, flagpsi

        common /mc_mode/mc_mode
        integer mc_mode(10),mcmode(10)
        equivalence(mcmode(1),mc_mode(1))
chenjc/2010-01-17 added for protection
        integer Iabort,mytest
        common /istat/Iabort
CC--PINGRG--2011-10-20
        COMMON/CHARMONIUMKF/ICKF

         Iabort=0

        tecm = sqrts
        ecm = sqrts
        flag = 0;

        if(pid.eq.10441.or.pid.eq.20443.or.pid.eq.445.or.
     &     pid.eq.10443.or.pid.eq.441.or.pid.eq.443.or.
     &     pid.eq.100443) then 
           flagmother = .true.
           else
           flagmother = .false.
           endif

cc       print*,'Decaying Particle: numevt,pid, sqrts=, ',numevt,pid,sqrts 

      if(numevt.lt.0) return
      if(numevt.eq.0) then
        sqrts0=sqrts
        PARJ(135)=0.01  !(D=0.01)minimum  photon E in initial-state radiation
C... particle decay in the beam pipe 
      MSTJ(22)=4      ! decay when the vertex within a cylindrical volume
      PARJ(73)=5.    !Let most resonace decay in Rin=5 mm ?
      PARJ(74)=30.    ! Z(mm) for end of CDC in BES, Z(MDC)=1100 mm       ?

C...New tuning parameters MSTJ(101)=(5 or 2)::(parton-shower or 2nd-ME)
      PARJ(21)=0.427     ! xgm(D=0.36), width in the Px,Py Gaussian dist.
      PARJ(33)=0.338     ! mass-2q(D=0.8),below which 2 -hadrons prodced,but no much diff by p80
      parj(126)=0.813     ! (D=2.)GeV Mass_min(gg) (merge g-g to g)

C...baryon production parameters
      parj(1)=0.071       !(D=0.1)p(qq)/p(q)
      parj(2)=0.49        !(D=0.3)p(ss)/p(uu),(uu:dd:ss=1:1:parj(2))
c..Kinds of mesons parameters(pseudoscalar spin=0(pi(ud~),k(s),eta(uu~+dd~+ss~)
c   spin=1,J=1(Vector, rho,omiga,phi,h1..)
c      V/p=<3/1 suggested by spin and overlapper of wave function p227
C                              ...i=11:u&d,12:s,13:C,
c      L=0,S=0,J=0, ratio=(1-parj(i))(1-parj(14))  pseudoscalar meson
c      L=0,S=1,J=1, ratio=parj(i)(1-parj(15)-parj(16)-parj(17))  vector
c      L=1,S=0,J=1, ratio=(1-parj(i))parj(14)      axial vector meson
C      L=1,S=1,J=0, ratio=parj(i)parj(15)          scalar meson
c      L=1,S=1,J=1, ratio=parj(i)parj(16)          another axial vector meson
c      L=1,S=1,J=2, ratio=parj(i)parj(17)          tensor meson
C...marked * means effection on reducsion scalar production
      parj(11)=0.1     !*(D=0.5) V/p ratio of u&d
      parj(12)=0.928    !(D=0.6)  V/p ratio of strange(s)
c      parj(13)=0.8    !(D=0.75) V/p ratio of Charm
      parj(14)=0.645    !*(D=0.)  spin=0,L=1,J=1   axial Vector meson
      parj(15)=0.016    !(D=0.)   spin=1,L=1,J=0   scalar meson
      parj(16)=0.026    !(D=0.)   spin=1,L=1,J=1   anothe axial Vector meson
      parj(17)=0.156    !(D=0.)   spin=1,L=1,J=2   tensor meson

      parj(25)=0.336     !(D=1.) extra supression for eta production
c... calculate frection of Charm meson by Eichten model
      if(sqrts0.gt.3.726.and.sqrts0.lt.4.5) then
        mstj(104)=3      ! only produce u,d,s quark
        MSTJ(107)=0  !(D=0)initial-state radiatic correction;0:no; 1:do
        call eichten(SQRTS,Rdchannel)
      endif
      if(rdchannel(1)+rdchannel(4).gt.0.)
     &  ddn=rdchannel(1)/(rdchannel(1)+rdchannel(4))
      if(rdchannel(2)+rdchannel(5).gt.0.)
     &  ddsn=rdchannel(2)/(rdchannel(2)+rdchannel(5))
      if(rdchannel(3)+rdchannel(6).gt.0.)
     &  dsdsn=rdchannel(3)/(rdchannel(3)+rdchannel(6))
        Rlund=2.5    ! R fraction of continuum events
      R_d=Rlund
      Rtot=Rdchannel(10)+Rlund
      Rlund=Rlund/Rtot             ! fraction of continuum events
      do j=1,9
        R_d=R_d+Rdchannel(j)
        Rdchannel(j)=R_d/Rtot
      enddo
c      write(*,*) 'Rlund,Rtot=',Rlund,Rtot,' R_D_channel=',Rdchannel
      if(new_charm.eq.0) then
        call charm_updat
        new_charm=1
       endif

        if(MSTJ(107).eq.1)then
           write(*,*) 'lund_crm> do initial-state radiatic correction!'
        else if(MSTJ(107).eq.0) then
           write(*,*) 'lund_crm> no initial-state radiatic correction!'
        endif

        mrlu(1)=ilundseed !!lundcharm random seed
        mrlu(2)=0
        call flush()
      return
      endif
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
cccccc   begin to loop over events

 99   continue !! begin to generate events
        N=0
        flag = 0;
       nloop=0
10     continue       ! begain Lund jetset particle generator
       nloop=nloop+1
       if(nloop.gt.1000000) then
        write(*,*) 'Branching ratio of final channel is too
     &small(<0.000001)!'
        status=3
        call flush()
        return
       endif

        do i=1,10
            mcmode(i)=0
         enddo
C     -- Wiggle beam energy
cjc      CALL MCBEAM
cjc         call rnormx(r1,1,ranlux)
cjc         sqrts = tecm + r1*sigebm
cjc      if(numevt.eq.1)  mrlu(1)=IRANDM(1)  ! seed of LUND random from sobcard
C.... select J/psi or psi' by ECM-mass(J/psi or psi')<width_channel/4
c         print*,'pid= ',pid
C....... pingrg 2007/10/7
       ICKF = 0
       if(pid.eq.100443.or.pid.eq.443 .or.pid.eq.10441.or.pid.eq.20443     !! decay psi(2S),chic_J,J/psi,eta_c 
     &   .or. pid .eq. 445 .or. pid.eq.441 .or. pid.eq.10443) then   !! not implement eta_c(2S), h_c decays
       if(pid.eq.443)    mcmode(10)=7     !J/psi
       if(pid.eq.100443) mcmode(10)=8     !psi(2S)
       if(pid.eq.100443) then             !! in JetSet74, psi(2S) id=30443, but in EvtGen,id=100443
         ICKF = 30443 
        else
         ICKF = PID  !!TRANSFER TO LUcharm
        endif
c...........
       CALL LUcharm(4,SQRTS)
          if(Iabort.eq.1) then
              iabort=0
              write(*,*) 'Abort and reproduce this event!'
              goto 10
          endif
       else  !!! decay psi(3770)
C...select fraction of Charm meson paires and continuum hadrons production
          RR=RLU(0)
          IF(RR.LE.Rlund) then
             mcmode(10)=4    !qed
            CALL LUEEVT1(0,SQRTS)   ! continuum energy generator
          else if((RR.gt.Rlund.and.RR.le.Rdchannel(1)).or.
     &        (RR.gt.Rdchannel(3).and.RR.le.Rdchannel(4))) then
          mcmode(10)=9
          if(RLU(0).le.ddn) then
            call luddgen(421,421,SQRTS)    ! D0D0~ 
          else
            call luddgen(411,411,SQRTS)    ! D+D- 
          endif
        else if((RR.gt.Rdchannel(1).and.RR.le.Rdchannel(2)).or.
     &      (RR.gt.Rdchannel(4).and.RR.le.Rdchannel(5))) then
           mcmode(10)=10
          if(RLU(0).le.ddsn) then
            call luddgen(421,423,SQRTS)    ! D0D*0~ 
          else
            call luddgen(411,413,SQRTS)    ! D+-D*+- 
          endif
        else if((RR.gt.Rdchannel(2).and.RR.le.Rdchannel(3)).or.
     &      (RR.gt.Rdchannel(5).and.RR.le.Rdchannel(6))) then
           mcmode(10)=11
          if(RLU(0).le.dsdsn) then
            call luddgen(423,423,SQRTS)    ! D*0D*0~ 
          else
            call luddgen(421,421,SQRTS)    ! D*+-D*+- 
          endif
        else if(RR.gt.Rdchannel(6).and.RR.le.Rdchannel(7)) then
            mcmode(10)=12
            call luddgen(431,431,SQRTS)   !DsDs
        else if(RR.gt.Rdchannel(7).and.RR.le.Rdchannel(8)) then
             mcmode(10)=13
            call luddgen(431,433,SQRTS)   !DsDs*
        else if(RR.gt.Rdchannel(8).and.RR.le.Rdchannel(9)) then
             mcmode(10)=14
             call luddgen(433,433,SQRTS) ! Ds*Ds*
         endif
       endif

c....output
cjc/2007/EvtGen      CALL LUEDIT(2)
      IF (FIRST) THEN
cjc        CALL LULIST(11)        ! LULIST(11) in the jetset7.4
        FIRST = .FALSE.
      ENDIF
C-----------------------------------------------------------------------
c find partons, delete secondary partons, set mother pointers
      KFstring=0
      ndaug = 0
      pdgmode = .false.
      flagpsi = ickf.eq.30443 .or. ickf.eq.443
cc----- debugging
c      CALL LULIST(1)

      do 1010 i=1,n
        kp = k(i,3)   !line number of parent particle or jet for i-th track
        kid = k(i,2)  !parton/particle KF code

        non_KF = kid.eq.21 .or. kid.ge.23.and.kid.le.37.or.kid
     &   .eq.91.or.kid.eq.92   !! tag gluon,W,and string,cluster
         if(kid.eq.91.or.kid.eq.92) KFstring=i  !! tag the string, assuming charmonium decays into string

c        print*,'i= ',i,' ',pdgmode,' kp= ',kp,' KFstring= ', KFstring
c        print*, 'not(non_KF)=',.not.non_KF

        if(.not.pdgmode.and.kid.eq.22.and.abs(k(kp,2)).le.4 )then   !!fill the FSR photon for ccbar anahilation into qqbar(q=u,d,s)  
           mytest = 1
           goto 1009                                
        else if(pid.eq.30443.and.(abs(kid).eq.411.or.abs(kid).eq.421))
     &                                                      then      !! selec psi(3770)open charm decay modes
           pdgmode = .true.
           mytest = 2
           goto 1009   
        else if(flagmother .and. kp.eq.1.and..not.non_KF.and.flagpsi) then  !!selection Jpsi,psi(2S) decay mode in PDG mode
           if(k(i+1,2).ne.21) pdgmode = .true.
           mytest =3
           goto 1009
        else if(.not.pdgmode.and.kp.eq.KFstring.and.KFstring.ne.0) then  !! selection string and cluster fragmentation events
           mytest = 4
           goto 1009
        else if (KFstring .gt. 0 .and. k(i,3).eq.KFstring.and..not.flagpsi) then !! selection etac,chicj, to qqbar
           mytest = 5
           goto 1009
        else
           mytest = 6
           goto 1010
        endif      


 1009   if(kid.eq.30443) kid=100443 !change jetset psi' id to pdg06      
        ndaug = ndaug + 1
        km(ndaug)=kp
        kf(ndaug)=kid
        px(ndaug)=p(i,1)
        py(ndaug)=p(i,2)
        pz(ndaug)=p(i,3)
        e(ndaug)=p(i,4)
c
c for debugging:
c        print '( 2I5,I12,4F12.4,I4 )',ndaug,km(ndaug),kf(ndaug),
c     1    px(ndaug),py(ndaug),pz(ndaug),e(ndaug),mytest
c-----------------------

1010  continue

C.....  for debugging !pingrg  
c       PRINT*,'N= ',N
c       CALL LULIST(1)
c-- test SetLundPar command
c      print*,'parj(12)=========0.66, ',parj(12)
cc check the energy conservation
      totalE=0.
      do i=1,ndaug
      totalE=totalE+e(i)
      enddo
chenjc/write mcmode into evt_header flag
         call mvbits(mcmode(2),0,10,flag,0)                ! DMODE1_max <= 1024
         call mvbits(mcmode(3),0,10,flag,10)              ! DMODE2_max <= 1024
         if(mcmode(4).eq.-1) mcmode(4)=0
         call mvbits(mcmode(4),0,1,flag,20)               ! DCHARM1 1/0 = D/D~
         if (mcmode(5).eq.-1) mcmode(5)=0
         call mvbits(mcmode(5),0,1,flag,21)               ! DCHARM2
         call mvbits(mcmode(6),0,1,flag,22)               ! not used
         call mvbits(mcmode(7),0,1,flag,23)               ! not used
         call mvbits(mcmode(8),0,1,flag,24)               ! not used
         call mvbits(mcmode(9),0,3,flag,25)               ! nx_max <= 8
         call mvbits(mcmode(10),0,4,flag,28)              ! isr_resonances_max <= 16

      if(abs(ecm-totalE).ge.0.001) goto 99
c         if(abs(ecm-totalE).ge.0.001) print*,'|ecm-totalE|= ',
c     &  ecm,' ',totalE,' ',abs(ecm-totalE)
      call flush()
      RETURN
      END
C********************************************************************* 
 
      SUBROUTINE decay1(kfa)
      
C...Purpose: Statistics 1st decay products of unstable particles in the code. 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT3/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      common/charm/mdcy_charm(2),mdme_charm(100,2), brat_charm(100),
     &  kfdp_charm(100,5)
      CHARACTER CHAP*16,CHAc(10)*16
      
C...Initial values. 
      NTRY=0 
      KFS=ISIGN(1,kfa)
      KC=LUCOMP(KFA) 
      do i=1,10
       CHAc(i)='                '
      enddo  

      WRITE(MSTU(11),*) '     LUDECY1 list'
      WRITE(MSTU(11),*) ' Probability          Complete final state'
      
C...Check existence of decay channels. Particle/antiparticle rules.
      KCA=KC
      IF(MDCY(KC,2).GT.0) THEN
        MDMDCY=MDME(MDCY(KC,2),2)
        IF(MDMDCY.GT.80.AND.MDMDCY.LE.90) KCA=MDMDCY
      ENDIF
      IF(MDCY(KCA,2).LE.0.OR.MDCY(KCA,3).LE.0) THEN
        CALL LUERRM(9,'(LUDECY:) no decay channel defined')
        RETURN
      ENDIF
      IF(MOD(KFA/1000,10).EQ.0.AND.(KCA.EQ.85.OR.KCA.EQ.87)) KFS=-KFS
      IF(KCHG(KC,3).EQ.0) THEN
        KFSP=1
        KFSN=0
        IF(RLU(0).GT.0.5) KFS=-KFS
      ELSEIF(KFS.GT.0) THEN
        KFSP=1
        KFSN=0
      ELSE
        KFSP=0
        KFSN=1
      ENDIF
      
 
C...Sum branching ratios of allowed decay channels. 
  220 NOPE=0 
      BRSU=0. 
      DO 230 IDL=MDCY(KCA,2),MDCY(KCA,2)+MDCY(KCA,3)-1 
        IF(MDME(IDL,1).NE.1.AND.KFSP*MDME(IDL,1).NE.2.AND.
     &    KFSN*MDME(IDL,1).NE.3) GOTO 230
        
      IF(MDME(IDL,2).GT.100) GOTO 230 
      NOPE=NOPE+1 
      BRSU=BRSU+BRAT(IDL) 
  230 CONTINUE 

      IF(NOPE.EQ.0) THEN 
        CALL LUERRM(2,'(LUDECY:) all decay channels closed by user') 
        RETURN 
      ENDIF 
      
      DO 290 IDC=MDCY(KCA,2),MDCY(KCA,2)+MDCY(KCA,3)-1 
        IF(MDME(IDc,1).NE.1) goto 290
        IF(MDME(IDc,2).GT.100) GOTO 290 
      BRidc=BRAT(IDc)/BRSU 
 
C...Read out decay products. Convert to standard flavour code. 
      njt=0
      JTMAX=5 
      IF(MDME(IDC+1,2).EQ.101) JTMAX=10 
      DO 280 JT=1,JTMAX 
      IF(JT.LE.5) KP=KFDP(IDC,JT) 
      IF(JT.GE.6) KP=KFDP(IDC+1,JT-5) 
      IF(KP.EQ.0) GOTO 280 
      KPA=IABS(KP) 
      KCP=LUCOMP(KPA) 
      IF(KPA.GT.80) MHADDY=1 
      IF(KCHG(KCP,3).EQ.0.AND.KPA.NE.81.AND.KPA.NE.82) THEN 
        KFP=KP 
      ELSEIF(KPA.NE.81.AND.KPA.NE.82) THEN 
        KFP=KFS*KP 
      ELSEIF(KPA.EQ.81.AND.MOD(KFA/1000,10).EQ.0) THEN 
        KFP=-KFS*MOD(KFA/10,10) 
      ELSEIF(KPA.EQ.81.AND.MOD(KFA/100,10).GE.MOD(KFA/10,10)) THEN 
        KFP=KFS*(100*MOD(KFA/10,100)+3) 
      ELSEIF(KPA.EQ.81) THEN 
        KFP=KFS*(1000*MOD(KFA/10,10)+100*MOD(KFA/100,10)+1) 
      ELSEIF(KP.EQ.82) THEN 
         idc_q=idc
        CALL LUKFDI(-KFS*INT(1.+(2.+PARJ(2))*RLU(0)),0,KFP,KDUMP) 
        IF(KFP.EQ.0) GOTO 280 
c        MSTJ(93)=1 
c        IF(PV(1,5).LT.PARJ(32)+2.*ULMASS(KFP)) GOTO 260 
      ELSEIF(KP.EQ.-82) THEN 
        KFP=-KFP 
        IF(IABS(KFP).GT.10) KFP=KFP+ISIGN(10000,KFP) 
      ENDIF 
      njt=njt+1
      IF(KPA.EQ.81.OR.KPA.EQ.82) KCP=LUCOMP(KFP) 
C...Get particle name, pad it and check it is not too long.
      CALL LUNAME(kfp,CHAP)
      LEN=0
      DO 100 LEM=1,16
        IF(CHAP(LEM:LEM).NE.' ') LEN=LEM
 100  CONTINUE
      chac(njt)=chap(1:len)
280   continue

      WRITE(MSTU(11),1100) bridc,(chac(i),i=1,njt)

290   CONTINUE

      WRITE(MSTU(11),*) 'Sum_bratch=',brsu 
      WRITE(MSTU(11),*) 'New added channels(include in old qq~brat'
      WRITE(MSTU(11),*) 'old qq~brat=',brat(idc_q)

      idc_min=0
      idc_max=0
      if(kfa.eq.443) then
        idc_min=1
        idc_max=mdcy_charm(1)
      elseif(kfa.eq.30443) then
        idc_min=mdcy_charm(1)+1
        idc_max=mdcy_charm(2)
      endif 
      bratsum=0.
      DO 1290 IDC=idc_min, idc_max
        IF(MDME_charm(IDc,1).NE.1) goto 1290
      BRatsum=bratsum+BRAT_charm(IDc) 
      BRidc=BRAT_charm(IDc) 
 
C...Read out decay products. Convert to standard flavour code. 
      njt=0
      JTMAX=5 
      DO 1280 JT=1,JTMAX 
      IF(JT.LE.5) KP=KFDP_charm(IDC,JT) 
      IF(JT.GE.6) KP=KFDP_charm(IDC+1,JT-5) 
      IF(KP.EQ.0) GOTO 1280 
      KPA=IABS(KP) 
      KCP=LUCOMP(KPA) 
      IF(KPA.GT.80) MHADDY=1 
      IF(KCHG(KCP,3).EQ.0.AND.KPA.NE.81.AND.KPA.NE.82) THEN 
        KFP=KP 
      ELSEIF(KPA.NE.81.AND.KPA.NE.82) THEN 
        KFP=KFS*KP 
      ELSEIF(KPA.EQ.81.AND.MOD(KFA/1000,10).EQ.0) THEN 
        KFP=-KFS*MOD(KFA/10,10) 
      ELSEIF(KPA.EQ.81.AND.MOD(KFA/100,10).GE.MOD(KFA/10,10)) THEN 
        KFP=KFS*(100*MOD(KFA/10,100)+3) 
      ELSEIF(KPA.EQ.81) THEN 
        KFP=KFS*(1000*MOD(KFA/10,10)+100*MOD(KFA/100,10)+1) 
      ELSEIF(KP.EQ.82) THEN 
        CALL LUKFDI(-KFS*INT(1.+(2.+PARJ(2))*RLU(0)),0,KFP,KDUMP) 
        IF(KFP.EQ.0) GOTO 1280 
      ELSEIF(KP.EQ.-82) THEN 
        KFP=-KFP 
        IF(IABS(KFP).GT.10) KFP=KFP+ISIGN(10000,KFP) 
      ENDIF 
      njt=njt+1
      IF(KPA.EQ.81.OR.KPA.EQ.82) KCP=LUCOMP(KFP) 
C...Get particle name, pad it and check it is not too long.
      CALL LUNAME(kfp,CHAP)
      LEN=0
      DO 500 LEM=1,16
        IF(CHAP(LEM:LEM).NE.' ') LEN=LEM
 500  CONTINUE
      chac(njt)=chap(1:len)
1280   continue

      WRITE(MSTU(11),1100) bridc,(chac(i),i=1,njt)

1290   CONTINUE 
      WRITE(MSTU(11),*) 'Sum of new channel=',bratsum

1100   format(1x,f10.5,5x,10a10)

      RETURN 
      END 
********************************************************************* 
 
      SUBROUTINE LUcharm(KFL0,ECM) 
 
C...Purpose: to handle the generation of an e+e- annihilation jet event. 
C...Purpose: to generate charm-onium decays (call luexec1->ludecy1->luonia1)
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT1/,/LUDAT2/ , ldeclst
      data ldeclst/0 /
      COMMON/CHARMONIUMKF/ICKF
      
cc---  decay etac, etac',chicj to qqbar// pingrg,2012-6-19
      if(ickf .ne. 443 .and. ickf.ne.30443) then
      CALL LUEEVT1(0,ECM)
c      print*, 'Particle with KF= ',ickf, ' is  deccayed.' 
c      CALL LULIST(1)
      return
      endif
CCCCCCCCCCCCCCCCCCCC

      kfl=kfl0
C...Check input parameters. 
cjc      IF(MSTU(12).GE.1) CALL LULIST(0) 
      if(kfl.ne.4) then       !check if charm flavour
      CALL LUERRM(16,'(LUcharm1:) called with not charm(4) flavour')
        IF(MSTU(21).GE.1) RETURN
      endif
      IF(ECM.LT.PARJ(127)+2.02*PARF(101)) THEN
        CALL LUERRM(16,'(LUcharm1:) called with too small CM energy')
        IF(MSTU(21).GE.1) RETURN
      ENDIF
       KFL=0
C...Check consistency of MSTJ options set. 
      IF(MSTJ(109).EQ.2.AND.MSTJ(110).NE.1) THEN 
        CALL LUERRM(6, 
     &  '(LUEEVT:) MSTJ(109) value requires MSTJ(110) = 1') 
        MSTJ(110)=1 
      ENDIF 
      IF(MSTJ(109).EQ.2.AND.MSTJ(111).NE.0) THEN 
        CALL LUERRM(6, 
     &  '(LUEEVT:) MSTJ(109) value requires MSTJ(111) = 0') 
        MSTJ(111)=0 
      ENDIF 

C...Initialize alpha_strong and total cross-section. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      IF(MSTJ(116).GT.0.AND.(MSTJ(116).GE.2.OR.ABS(ECM-PARJ(151)).GE. 
     &PARJ(139).OR.10*MSTJ(102)+KFL.NE.MSTJ(119))) CALL LUXTOT(KFL,ECM, 
     &XTOT) 
      IF(MSTJ(116).GE.3) MSTJ(116)=1 
      PARJ(171)=0. 
 
C...Add initial e+e- to event record (documentation only). 
      NTRY=0 
  100 NTRY=NTRY+1 
      IF(NTRY.GT.100) THEN 
        CALL LUERRM(14,'(LUEEVT:) caught in an infinite loop') 
        RETURN 
      ENDIF 
      MSTU(24)=0 
      NC=0 
      IF(MSTJ(115).GE.2) THEN 
        NC=NC+2 
        CALL LU1ENT(NC-1,11,0.5*ECM,0.,0.) 
        K(NC-1,1)=21 
        CALL LU1ENT(NC,-11,0.5*ECM,PARU(1),0.) 
        K(NC,1)=21 
      ENDIF 
 
C...Radiative photon (in initial state). 
      MK=0 
      ECMC=ECM 
C... trun off ISR correction ==pingrg
c      IF(MSTJ(107).GE.1.AND.MSTJ(116).GE.1) CALL LURADK1(ECM,MK,PAK, 
c     &THEK,PHIK,ALPK) 
c      IF(MK.EQ.1) ECMC=SQRT(ECM*(ECM-2.*PAK)) 
c      IF(MSTJ(115).GE.1.AND.MK.EQ.1) THEN 
c        NC=NC+1 
c        CALL LU1ENT(NC,22,PAK,THEK,PHIK) 
c        K(NC,3)=MIN(MSTJ(115)/2,1) 
c      ENDIF 
 
C...Virtual exchange boson (gamma or Z0). 
      IF(MSTJ(115).GE.3) THEN 
        NC=NC+1 
        KF=22 
        IF(MSTJ(102).EQ.2) KF=23 
        MSTU10=MSTU(10) 
        MSTU(10)=1 
        P(NC,5)=ECMC 
        CALL LU1ENT(NC,KF,ECMC,0.,0.) 
        K(NC,1)=21 
        K(NC,3)=1 
        MSTU(10)=MSTU10 
      ENDIF
      
cjc/99 
C...Choose x1 and x2 according to matrix element.
       KFLC=IABS(KFL0)
      KF1=110*KFLC+3      !J/psi
      kc1=lucomp(kf1)
      KF2=30000+110*KFLC+3   ! psi'
      kc2=lucomp(kf2)

C.... select J/psi or psi' by ECM-mass(J/psi or psi')<width_channel/2
cc..... coment by pingrg,2011-10-20, using common/charmoniumkf/ to transfer the id
c       print*, 'LUcharm:ICKF= ',ickf,', mass= ',pmas(lucomp(ickf),1)
c      if(abs(Ecmc-pmas(KC1,1)).lt.0.004) then !Ecm smear 0.001GeV
c        kf=kf1
c      else if(abs(Ecmc-3.096).lt.0.004) then !Ecm smear 0.001GeV
c         kf=kf1
c       else if(abs(Ecmc-pmas(KC2,1)).lt.0.004) then !Ecm smear 0.001GeV
c        kf=kf2
c      else
      if(abs(ICKF).gt.0) then 
        kf = ickf
      else
        goto 105
      endif

c.... list decay branching
      if(ldeclst.eq.0) then
	call decay1(kf)
        ldeclst=1
      endif
C....Generate a charm-oniam
      MSTU10=MSTU(10)
      MSTU(10)=1
      ip=Nc+1
      P(ip,5)=ECMc
c      print*, 'KF= ',kf
      CALL LU1ENT(ip,KF,ECMC,0.,0.)
      K(IP,3)=K(IP,3)+(MSTJ(115)/2)+(KFLC/5)*(MSTJ(115)/3)*(NC-1)
      MSTU(10)=MSTU10

C...Rotation and boost from radiative photon. 
      IF(MK.EQ.1) THEN 
        DBEK=-PAK/(ECM-PAK) 
        NMIN=NC+1-MSTJ(115)/3 
        CALL LUDBRB(NMIN,N,0.,-PHIK,0D0,0D0,0D0) 
        CALL LUDBRB(NMIN,N,ALPK,0.,DBEK*SIN(THEK),0D0,DBEK*COS(THEK)) 
        CALL LUDBRB(NMIN,N,0.,PHIK,0D0,0D0,0D0) 
      ENDIF 
 
C...Generate fragmentation. Information for LUTABU:
      
      IF(MSTJ(105).EQ.1) CALL LUEXEC1

      MSTU(161)=110*KFLC+3
      MSTU(162)=0

      RETURN
105   continue
cjc--
C...Choice of flavour and jet configuration. 
      CALL LUXKFL(KFL,ECM,ECMC,KFLC) 
      IF(KFLC.EQ.0) GOTO 100 
      CALL LUXJET(ECMC,NJET,CUT) 
      KFLN=21 
      IF(NJET.EQ.4) CALL LUX4JT(NJET,CUT,KFLC,ECMC,KFLN,X1,X2,X4, 
     &X12,X14) 
      IF(NJET.EQ.3) CALL LUX3JT(NJET,CUT,KFLC,ECMC,X1,X3) 
      IF(NJET.EQ.2) MSTJ(120)=1 
 
C...Fill jet configuration and origin. 
      IF(NJET.EQ.2.AND.MSTJ(101).NE.5) CALL LU2ENT(NC+1,KFLC,-KFLC,ECMC) 
      IF(NJET.EQ.2.AND.MSTJ(101).EQ.5) CALL LU2ENT(-(NC+1),KFLC,-KFLC, 
     & ECMC) 
      IF(NJET.EQ.3) CALL LU3ENT(NC+1,KFLC,21,-KFLC,ECMC,X1,X3) 
      IF(NJET.EQ.4.AND.KFLN.EQ.21) CALL LU4ENT(NC+1,KFLC,KFLN,KFLN, 
     & -KFLC,ECMC,X1,X2,X4,X12,X14) 
      IF(NJET.EQ.4.AND.KFLN.NE.21) CALL LU4ENT(NC+1,KFLC,-KFLN,KFLN, 
     & -KFLC,ECMC,X1,X2,X4,X12,X14) 
      IF(MSTU(24).NE.0) GOTO 100 
      DO 110 IP=NC+1,N 
      K(IP,3)=K(IP,3)+MIN(MSTJ(115)/2,1)+(MSTJ(115)/3)*(NC-1) 
  110 CONTINUE 
 
C...Angular orientation according to matrix element. 
      IF(MSTJ(106).EQ.1) THEN 
        CALL LUXDIF(NC,NJET,KFLC,ECMC,CHI,THE,PHI) 
        CALL LUDBRB(NC+1,N,0.,CHI,0D0,0D0,0D0) 
        CALL LUDBRB(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 
 
C...Rotation and boost from radiative photon. 
      IF(MK.EQ.1) THEN 
        DBEK=-PAK/(ECM-PAK) 
        NMIN=NC+1-MSTJ(115)/3 
        CALL LUDBRB(NMIN,N,0.,-PHIK,0D0,0D0,0D0) 
        CALL LUDBRB(NMIN,N,ALPK,0.,DBEK*SIN(THEK),0D0,DBEK*COS(THEK)) 
        CALL LUDBRB(NMIN,N,0.,PHIK,0D0,0D0,0D0) 
      ENDIF 
 
C...Generate parton shower. Rearrange along strings and check. 
      IF(MSTJ(101).EQ.5) THEN 
        CALL LUSHOW(N-1,N,ECMC) 
        MSTJ14=MSTJ(14) 
        IF(MSTJ(105).EQ.-1) MSTJ(14)=-1 
        IF(MSTJ(105).GE.0) MSTU(28)=0 
        CALL LUPREP(0) 
        MSTJ(14)=MSTJ14 
        IF(MSTJ(105).GE.0.AND.MSTU(28).NE.0) GOTO 100 
      ENDIF 
 
C...Fragmentation/decay generation. Information for LUTABU. 
      IF(MSTJ(105).EQ.1) CALL LUEXEC1 
      MSTU(161)=KFLC 
      MSTU(162)=-KFLC 
 
      RETURN 
      END 
C********************************************************************* 
 
      SUBROUTINE luddgen(KFL1,KFL2,ECM) 
 
C...Purpose: to handle the generation of DD~,D*D*~,Ds*Ds*~ events. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT1/,/LUDAT2/ 
          common /mc_mode/mc_mode
          integer mc_mode(10),mcmode(10)
          equivalence(mcmode(1),mc_mode(1))

C...Check input parameters. 
      IF(MSTU(12).GE.1) CALL lulist(0) 
      IF(KFL1.LT.411.and.KFL1.GT.433) THEN    !DD~,D*D*~,Ds*Ds*~
        CALL LUERRM(16,'(LUDDGEN:) called with unknown DD~ code') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(KFL2.LT.411.and.KFL2.GT.433) THEN    !DD~,D*D*~,Ds*Ds*~
        CALL LUERRM(16,'(LUDDGEN:) called with unknown DD~ code') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      ECMMIN=PMAS(KFL1,1)+PMAS(KFL2,1) 
      IF(ECM.LT.ECMMIN) THEN 
        CALL LUERRM(16,'(LUDDGEN:) called with too small CM energy') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Check consistency of MSTJ options set. 
      IF(MSTJ(109).EQ.2.AND.MSTJ(110).NE.1) THEN 
        CALL LUERRM(6, 
     &  '(LUDDGEN:) MSTJ(109) value requires MSTJ(110) = 1') 
        MSTJ(110)=1 
      ENDIF 
      IF(MSTJ(109).EQ.2.AND.MSTJ(111).NE.0) THEN 
        CALL LUERRM(6, 
     &  '(LUDDGEN:) MSTJ(109) value requires MSTJ(111) = 0') 
        MSTJ(111)=0 
      ENDIF 
 
C...Initialize alpha_strong and total cross-section. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      IF(MSTJ(116).GT.0.AND.(MSTJ(116).GE.2.OR.ABS(ECM-PARJ(151)).GE. 
     &PARJ(139).OR.10*MSTJ(102)+KFL1.NE.MSTJ(119))) 
     &CALL LUXTOT(KFL1,ECM,XTOT)
      IF(MSTJ(116).GE.3) MSTJ(116)=1 
       PARJ(171)=0. 
C...Add initial e+e- to event record (documentation only). 
      NTRY=0 
  100 NTRY=NTRY+1 
      IF(NTRY.GT.100) THEN 
        CALL LUERRM(14,'(LUDDGEN:) caught in an infinite loop') 
        RETURN 
      ENDIF 
      MSTU(24)=0 
      NC=0 
      IF(MSTJ(115).GE.2) THEN 
        NC=NC+2 
        CALL LU1ENT(NC-1,11,0.5*ECM,0.,0.) 
        K(NC-1,1)=21 
        CALL LU1ENT(NC,-11,0.5*ECM,PARU(1),0.) 
        K(NC,1)=21 
      ENDIF 
 
C...Radiative photon (in initial state). 
      MK=0 
      ECMC=ECM 
      IF(MSTJ(107).GE.1.AND.MSTJ(116).GE.1) CALL LURADK1(ECM,MK,PAK, 
     &THEK,PHIK,ALPK) 
      IF(MK.EQ.1) ECMC=SQRT(ECM*(ECM-2.*PAK)) 
      IF(MSTJ(115).GE.1.AND.MK.EQ.1) THEN 
        NC=NC+1 
        CALL LU1ENT(NC,22,PAK,THEK,PHIK) 
        K(NC,3)=MIN(MSTJ(115)/2,1) 
      ENDIF 
  
C...Fill DD~ configuration and origin. 
      if(RLU(0).le.0.5) then
        kfl10 = kfl1
        kfl20 = -kfl2
      else
        kfl10 = -kfl1
        kfl20 = kfl2
      endif
      CALL LU2ENT(NC+1,KFL10,KFL20,ECMC) 

C...Angular orientation
        if(KFL1.eq.411.and.KFL2.eq.411) then          !D+D-
C Generate the Psi'' decay angles: sin**2=1-cos**2 for theta and flat for phi.
150     COSTHE = 2.0*RLU(0) - 1.0
        IF(RLU(0) .GT. (1.0 - cosTHE**2)) GO TO 150
        else if(KFL1.eq.421.and.KFL2.eq.421) then     !D0D0~ 
152     COSTHE = 2.0*RLU(0) - 1.0
        IF(RLU(0) .GT. (1.0 - cosTHE**2)) GO TO 152
        else if(KFL1.eq.411.and.KFL2.eq.413) then     !D*D+-
154     COSTHE = 2.0*RLU(0) - 1.0
        IF(RLU(0) .GT. (1.0 + cosTHE**2)) GO TO 154
        else if(KFL1.eq.421.and.KFL2.eq.423) then     !D*D0
156     COSTHE = 2.0*RLU(0) - 1.0
        IF(RLU(0) .GT. (1.0 + cosTHE**2)) GO TO 156
        else if(KFL1.eq.413.and.KFL2.eq.413) then     !D*D*
        A= 0.0
        IF(A.LE.0.) OLIOM= 1.0
COB   IF(A.GT.0.) OLIOM= 1.0+A
158     COSTHE = 2.0*RLU(0) - 1.0
        IF(OLIOM*RLU(0) .GT. (1.0 + A*cosTHE**2)) GO TO 158
        else if(KFL1.eq.423.and.KFL2.eq.423) then     !D*0D*0
        A= 0.0
        IF(A.LE.0.) OLIOM= 1.0
COB   IF(A.GT.0.) OLIOM= 1.0+A
160     COSTHE = 2.0*RLU(0) - 1.0
        IF(OLIOM*RLU(0) .GT. (1.0 + A*cosTHE**2)) GO TO 160
        else if(KFL1.eq.431.and.KFL2.eq.431) then     !DsDs~
162     COSTHE = 2.0*RLU(0) - 1.0
        IF(RLU(0) .GT. (1.0 - cosTHE**2)) GO TO 162
        else if(KFL1.eq.431.and.KFL2.eq.433) then     !DsDs*
164     COSTHE = 2.0*RLU(0) - 1.0
        IF(RLU(0) .GT. (1.0 + cosTHE**2)) GO TO 164
        else if(KFL1.eq.433.and.KFL2.eq.433) then     !Ds*Ds*
cjc        A= -1.0           
        A= 0.0                  !same as D*D*(suggested by Zhang D.H.)
        IF(A.LE.0.) OLIOM= 1.0
166     COSTHE = 2.0*RLU(0) - 1.0
        IF(OLIOM*RLU(0) .GT. (1.0 + A*cosTHE**2)) GO TO 166
        endif

        THE=acos(cosTHE)
        PHI = RLU(0) * 6.28319

        CALL LUDBRB(NC+1,N,THE,PHI,0D0,0D0,0D0) 
 
C...Rotation and boost from radiative photon. 
      IF(MK.EQ.1) THEN 
        DBEK=-PAK/(ECM-PAK) 
        NMIN=NC+1-MSTJ(115)/3 
        CALL LUDBRB(NMIN,N,0.,-PHIK,0D0,0D0,0D0) 
        CALL LUDBRB(NMIN,N,ALPK,0.,DBEK*SIN(THEK),0D0,DBEK*COS(THEK)) 
        CALL LUDBRB(NMIN,N,0.,PHIK,0D0,0D0,0D0) 
      ENDIF 
  
C...Fragmentation/decay generation. Information for LUTABU. 
      IF(MSTJ(105).EQ.1) CALL LUEXEC1
      MSTU(161)=KFL10
      MSTU(162)=KFL20

      MCMODE(2) = 1000
      MCMODE(3) = 1000
      if(KFL10.gt.0) then
        MCMODE(4)=1
        MCMODE(5)=0
      else
        MCMODE(4)=0
        MCMODE(5)=1
      endif

      RETURN 
      END 
 
C********************************************************************* 
C********************************************************************* 
 
      SUBROUTINE LUEEVT1(KFL,ECM) 
 
C...Purpose: to handle the generation of an e+e- annihilation jet event. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT1/,/LUDAT2/ 
 
C...Check input parameters. 
      IF(MSTU(12).GE.1) CALL lulist(0) 
      IF(KFL.LT.0.OR.KFL.GT.8) THEN 
        CALL LUERRM(16,'(LUEEVT:) called with unknown flavour code') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(KFL.LE.5) ECMMIN=PARJ(127)+2.02*PARF(100+MAX(1,KFL)) 
      IF(KFL.GE.6) ECMMIN=PARJ(127)+2.02*PMAS(KFL,1) 
      IF(ECM.LT.ECMMIN) THEN 
        CALL LUERRM(16,'(LUEEVT:) called with too small CM energy') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Check consistency of MSTJ options set. 
      IF(MSTJ(109).EQ.2.AND.MSTJ(110).NE.1) THEN 
        CALL LUERRM(6, 
     &  '(LUEEVT:) MSTJ(109) value requires MSTJ(110) = 1') 
        MSTJ(110)=1 
      ENDIF 
      IF(MSTJ(109).EQ.2.AND.MSTJ(111).NE.0) THEN 
        CALL LUERRM(6, 
     &  '(LUEEVT:) MSTJ(109) value requires MSTJ(111) = 0') 
        MSTJ(111)=0 
      ENDIF 
 
C...Initialize alpha_strong and total cross-section. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      IF(MSTJ(116).GT.0.AND.(MSTJ(116).GE.2.OR.ABS(ECM-PARJ(151)).GE. 
     &PARJ(139).OR.10*MSTJ(102)+KFL.NE.MSTJ(119))) CALL LUXTOT(KFL,ECM, 
     &XTOT) 
      IF(MSTJ(116).GE.3) MSTJ(116)=1 
      PARJ(171)=0. 
 
C...Add initial e+e- to event record (documentation only). 
      NTRY=0 
  100 NTRY=NTRY+1 
      IF(NTRY.GT.100) THEN 
        CALL LUERRM(14,'(LUEEVT:) caught in an infinite loop') 
        RETURN 
      ENDIF 
      MSTU(24)=0 
      NC=0 
      IF(MSTJ(115).GE.2) THEN 
        NC=NC+2 
        CALL LU1ENT(NC-1,11,0.5*ECM,0.,0.) 
        K(NC-1,1)=21 
        CALL LU1ENT(NC,-11,0.5*ECM,PARU(1),0.) 
        K(NC,1)=21 
      ENDIF 
 
C...Radiative photon (in initial state). 
      MK=0 
      ECMC=ECM 
      IF(MSTJ(107).GE.1.AND.MSTJ(116).GE.1) CALL LURADK1(ECM,MK,PAK, 
     &THEK,PHIK,ALPK) 
      IF(MK.EQ.1) ECMC=SQRT(ECM*(ECM-2.*PAK)) 
      IF(MSTJ(115).GE.1.AND.MK.EQ.1) THEN 
        NC=NC+1 
        CALL LU1ENT(NC,22,PAK,THEK,PHIK) 
        K(NC,3)=MIN(MSTJ(115)/2,1) 
      ENDIF 
 
C...Virtual exchange boson (gamma or Z0). 
      IF(MSTJ(115).GE.3) THEN 
        NC=NC+1 
        KF=22 
        IF(MSTJ(102).EQ.2) KF=23 
        MSTU10=MSTU(10) 
        MSTU(10)=1 
        P(NC,5)=ECMC 
        CALL LU1ENT(NC,KF,ECMC,0.,0.) 
        K(NC,1)=21 
        K(NC,3)=1 
        MSTU(10)=MSTU10 
      ENDIF 
 
C...Choice of flavour and jet configuration. 
      CALL LUXKFL(KFL,ECM,ECMC,KFLC) 
      IF(KFLC.EQ.0) GOTO 100 
      CALL LUXJET(ECMC,NJET,CUT) 
      KFLN=21 
      IF(NJET.EQ.4) CALL LUX4JT(NJET,CUT,KFLC,ECMC,KFLN,X1,X2,X4, 
     &X12,X14) 
      IF(NJET.EQ.3) CALL LUX3JT(NJET,CUT,KFLC,ECMC,X1,X3) 
      IF(NJET.EQ.2) MSTJ(120)=1 
 
C...Fill jet configuration and origin. 
      IF(NJET.EQ.2.AND.MSTJ(101).NE.5) CALL LU2ENT(NC+1,KFLC,-KFLC,ECMC) 
      IF(NJET.EQ.2.AND.MSTJ(101).EQ.5) CALL LU2ENT(-(NC+1),KFLC,-KFLC, 
     &ECMC) 
      IF(NJET.EQ.3) CALL LU3ENT(NC+1,KFLC,21,-KFLC,ECMC,X1,X3) 
      IF(NJET.EQ.4.AND.KFLN.EQ.21) CALL LU4ENT(NC+1,KFLC,KFLN,KFLN, 
     &-KFLC,ECMC,X1,X2,X4,X12,X14) 
      IF(NJET.EQ.4.AND.KFLN.NE.21) CALL LU4ENT(NC+1,KFLC,-KFLN,KFLN, 
     &-KFLC,ECMC,X1,X2,X4,X12,X14) 
      IF(MSTU(24).NE.0) GOTO 100 
      DO 110 IP=NC+1,N 
      K(IP,3)=K(IP,3)+MIN(MSTJ(115)/2,1)+(MSTJ(115)/3)*(NC-1) 
  110 CONTINUE 
 
C...Angular orientation according to matrix element. 
      IF(MSTJ(106).EQ.1) THEN 
        CALL LUXDIF(NC,NJET,KFLC,ECMC,CHI,THE,PHI) 
        CALL LUDBRB(NC+1,N,0.,CHI,0D0,0D0,0D0) 
        CALL LUDBRB(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 
 
C...Rotation and boost from radiative photon. 
      IF(MK.EQ.1) THEN 
        DBEK=-PAK/(ECM-PAK) 
        NMIN=NC+1-MSTJ(115)/3 
        CALL LUDBRB(NMIN,N,0.,-PHIK,0D0,0D0,0D0) 
        CALL LUDBRB(NMIN,N,ALPK,0.,DBEK*SIN(THEK),0D0,DBEK*COS(THEK)) 
        CALL LUDBRB(NMIN,N,0.,PHIK,0D0,0D0,0D0) 
      ENDIF 
 
C...Generate parton shower. Rearrange along strings and check. 
      IF(MSTJ(101).EQ.5) THEN 
        CALL LUSHOW(N-1,N,ECMC) 
        MSTJ14=MSTJ(14) 
        IF(MSTJ(105).EQ.-1) MSTJ(14)=-1 
        IF(MSTJ(105).GE.0) MSTU(28)=0 
        CALL LUPREP(0) 
        MSTJ(14)=MSTJ14 
        IF(MSTJ(105).GE.0.AND.MSTU(28).NE.0) GOTO 100 
      ENDIF 
 
C...Fragmentation/decay generation. Information for LUTABU. 
      IF(MSTJ(105).EQ.1) CALL LUEXEC1 
      MSTU(161)=KFLC 
      MSTU(162)=-KFLC 
 
      RETURN 
      END 
 
C********************************************************************* 
*********************************************************************
*surbroutine list:                                                  *
*   (1) LUEXEC1, (2) LUDECY1 (3) cgparity, (4) psupress             *
*   (5) charm_updat                                                 *
********************************************************************* 
      SUBROUTINE LUEXEC1 
*********************************************************************
* This subroutine is modified from LUEXEC in the jetset74.f         *
*   (1) J/psi,psi' decay to rgg/ggg, need to check C&G parity       *
*       conservation.                                               *
*   (2) CALL LUDECY1.                                               *
*                         Chen Jiangchuan  1998.5.11                *
* Modified:                                                         *
*     *notuse(1)   ULANGL -> ULANGL1   (same to jetset_741.f)              *
*     *notuse(2)   LULIST -> LULIST1   
*                   Chen Jiangchuan     2001 March 13               *
*                                                                   *
*********************************************************************

C...Purpose: to administrate the fragmentation and decay chain. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT3/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT1/,/LUDAT2/,/LUDAT3/
      DIMENSION PS(2,6) 
      dimension Kg1(4000,5),Pg1(4000,5),Vg1(4000,5) 
      common /istat/Iabort

C...Initialize and reset. 
      ngg=0
      MSTU(24)=0 
      IF(MSTU(12).GE.1) CALL LULIST(0) 
      MSTU(31)=MSTU(31)+1 
      MSTU(1)=0 
      MSTU(2)=0 
      MSTU(3)=0 
      IF(MSTU(17).LE.0) MSTU(90)=0 
      MCONS=1 
      
C...Sum up momentum, energy and charge for starting entries. 

      NSAV=N 
      DO 110 I=1,2 
      DO 100 J=1,6 
      PS(I,J)=0. 
  100 CONTINUE 
  110 CONTINUE 
      DO 130 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 130 
      DO 120 J=1,4 
      PS(1,J)=PS(1,J)+P(I,J) 
  120 CONTINUE 
      PS(1,6)=PS(1,6)+LUCHGE(K(I,2)) 
  130 CONTINUE 
      PARU(21)=PS(1,4) 
 
C...Prepare system for subsequent fragmentation/decay. 

      CALL LUPREP(0) 
 
C...Loop through jet fragmentation and particle decays. 

      MBE=0 
  140 MBE=MBE+1 
      IP=0 
  150 IP=IP+1 
      KC=0 
      IF(K(IP,1).GT.0.AND.K(IP,1).LE.10) KC=LUCOMP(K(IP,2)) 
      IF(KC.EQ.0) THEN 
 
C...Particle decay if unstable and allowed. Save long-lived particle 
C...decays until second pass after Bose-Einstein effects. 

      ELSEIF(KCHG(KC,2).EQ.0) THEN 
        IF(MSTJ(21).GE.1.AND.MDCY(KC,1).GE.1.AND.(MSTJ(51).LE.0.OR.MBE 
     &  .EQ.2.OR.PMAS(KC,2).GE.PARJ(91).OR.IABS(K(IP,2)).EQ.311)) 
     &    CALL LUDECY1(IP,ngg,ipg)   

C...Decay products may develop a shower. 

        IF(MSTJ(92).GT.0) THEN 
          IP1=MSTJ(92) 
          QMAX=SQRT(MAX(0.,(P(IP1,4)+P(IP1+1,4))**2-(P(IP1,1)+P(IP1+1, 
     &    1))**2-(P(IP1,2)+P(IP1+1,2))**2-(P(IP1,3)+P(IP1+1,3))**2)) 
          CALL LUSHOW(IP1,IP1+1,QMAX) 
          CALL LUPREP(IP1) 
          MSTJ(92)=0 
        ELSEIF(MSTJ(92).LT.0) THEN 
          IP1=-MSTJ(92) 
          CALL LUSHOW(IP1,-3,P(IP,5)) 
          CALL LUPREP(IP1) 
          MSTJ(92)=0 
        ENDIF 
 
C...Jet fragmentation: string or independent fragmentation. 
      ELSEIF(K(IP,1).EQ.1.OR.K(IP,1).EQ.2) THEN 
        MFRAG=MSTJ(1) 
        IF(MFRAG.GE.1.AND.K(IP,1).EQ.1) MFRAG=2 
        IF(MSTJ(21).GE.2.AND.K(IP,1).EQ.2.AND.N.GT.IP) THEN 
          IF(K(IP+1,1).EQ.1.AND.K(IP+1,3).EQ.K(IP,3).AND. 
     &    K(IP,3).GT.0.AND.K(IP,3).LT.IP) THEN 
            IF(KCHG(LUCOMP(K(K(IP,3),2)),2).EQ.0) MFRAG=MIN(1,MFRAG) 
          ENDIF 
        ENDIF

cjc... J/psi,psi' decay to rgg/ggg, need to check C&G parity conservation
       if(ngg.ge.1) then 
         ntmp=n
         ngp=0
           igk=0
           do igg=1,n
             do jgk=1,5
               kg1(igg,jgk)=k(igg,jgk)
               pg1(igg,jgk)=p(igg,jgk)
               vg1(igg,jgk)=v(igg,jgk)
             enddo
            enddo 
          ngentry=0
154       ngentry=ngentry+1
          if(ngentry.gt.1000) then
            write(*,*) '(LUEXEC1):infinit loop for G conservation!'
            return
            endif 
cjc... reset g string initial value
            n=ntmp
            igk=0
            do igg=1,n
             do jgk=1,5
               k(igg,jgk)=kg1(igg,jgk)
               p(igg,jgk)=pg1(igg,jgk)
               v(igg,jgk)=vg1(igg,jgk)
             enddo
           enddo
          IF(MFRAG.EQ.1) CALL LUSTRF(IP) 
          IF(MFRAG.EQ.2) CALL LUINDF(IP) 
          IF(MFRAG.EQ.2.AND.K(IP,1).EQ.1) MCONS=0 
        IF(MFRAG.EQ.2.AND.(MSTJ(3).LE.0.OR.MOD(MSTJ(3),5).EQ.0)) MCONS=0 

cjc... check C&G parity conservation        
        if((k(ntmp+1,2).gt.80).and.(k(ntmp+1,2).lt.100)) then
             ip1=ipg
             ip2=ntmp+2
             ip3=n
	     icg=1
             call cgparity(ip1,ip2,ip3,kcok,ngg,icg)
             if(kcok.eq.-1) goto 154
             call psupress(ip1,ip2,ip3,ksok)
             if(ksok.eq.-1) goto 154
cjc           if(ngg.eq.1) then      ! G conservation in J/psi->ggg
           if(ngg.ge.1) then      ! G conservation in J/psi->ggg/rgg
            icg=2
	    call cgparity(ip1,ip2,ip3,kgok,ngg,icg)
            if(kgok.eq.-1) goto 154
          endif   
         else
            write(*,*) 'LUEXEC!:Wrong, no string!'
         endif 
        else
cjc....nomoral fragmentation of string
        IF(MFRAG.EQ.1) CALL LUSTRF(IP) 
        IF(MFRAG.EQ.2) CALL LUINDF(IP) 
        IF(MFRAG.EQ.2.AND.K(IP,1).EQ.1) MCONS=0 
        IF(MFRAG.EQ.2.AND.(MSTJ(3).LE.0.OR.MOD(MSTJ(3),5).EQ.0)) MCONS=0 
       endif
      ENDIF 
 
C...Loop back if enough space left in LUJETS and no error abort. 
      IF(MSTU(24).NE.0.AND.MSTU(21).GE.2) THEN 
      ELSEIF(IP.LT.N.AND.N.LT.MSTU(4)-20-MSTU(32)) THEN 
        GOTO 150 
      ELSEIF(IP.LT.N) THEN 
        CALL LUERRM(11,'(LUEXEC:) no more memory left in LUJETS') 
      ENDIF 
 
C...Include simple Bose-Einstein effect parametrization if desired. 
      IF(MBE.EQ.1.AND.MSTJ(51).GE.1) THEN 
        CALL LUBOEI(NSAV) 
        GOTO 140 
      ENDIF 
 
C...Check that momentum, energy and charge were conserved. 
      DO 170 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 170 
      DO 160 J=1,4 
      PS(2,J)=PS(2,J)+P(I,J) 
  160 CONTINUE 
      PS(2,6)=PS(2,6)+LUCHGE(K(I,2)) 
  170 CONTINUE 
      PDEV=(ABS(PS(2,1)-PS(1,1))+ABS(PS(2,2)-PS(1,2))+ABS(PS(2,3)- 
     &PS(1,3))+ABS(PS(2,4)-PS(1,4)))/(1.+ABS(PS(2,4))+ABS(PS(1,4))) 
      IF(MCONS.EQ.1.AND.PDEV.GT.PARU(11)) CALL LUERRM(15, 
     &'(LUEXEC:) four-momentum was not conserved')
chenjc/2010-01-17 added for protection
      IF(MCONS.EQ.1.AND.PDEV.GT.PARU(11)) Iabort=1         
      IF(MCONS.EQ.1.AND.ABS(PS(2,6)-PS(1,6)).GT.0.1) CALL LUERRM(15, 
     &'(LUEXEC:) charge was not conserved') 

      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUDECY1(IP,ngg,ipg) 
**********************************************************************
* This subroutine is modified from LUDECY in the jetset74.F          *
* (J/psi & psi(2S) decay channel and branch ratio are modified.)     *
*      IP: input, partile to decay.                                  *
*     ngg: ouput, J/psi,psi' decay into ggg; need to check G parity. *
*     ipg: ouput, ip of decayed J/psi or psi' to ggg.                *
*                                         Chen Jiangchuan 1998.5.31  *
*     Modified:                                                      *
*         f0(mass<1GeV))->pipi,f0(mass>1GeV)->KK                     *
*                                         Chen Jiangchuan 2002.7.18  *
*         Add angular distribution for J/psi->e+e-/mu+mu-            *
*         1+A*cos^2(theta), where A(Def=1.0) is input by XPARAM(2).  *
*                                         Chen Jiangchuan 2003.10.10 *
**********************************************************************
C...Purpose: to handle the decay of unstable particles. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT3/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT1/,/LUDAT2/,/LUDAT3/ ,wtcor
      common/charm/mdcy_charm(2),mdme_charm(100,2), brat_charm(100),
     &  kfdp_charm(100,5)
      DIMENSION VDCY(4),KFLO(4),KFL1(4),PV(10,5),RORD(10),UE(3),BE(3), 
     &WTCOR(10),PTAU(4),PCMTAU(4) 
      DOUBLE PRECISION DBETAU(3) 
      DATA WTCOR/2.,5.,15.,60.,250.,1500.,1.2E4,1.2E5,150.,16./ 
cjc       INCLUDE 'cmodel.inc'    
       data A_angl/0.0/

C...Functions: momentum in two-particle decays, four-product and 
C...matrix element times phase space in weak decays. 
      PAWT(A,B,C)=SQRT((A**2-(B+C)**2)*(A**2-(B-C)**2))/(2.*A) 
      FOUR(I,J)=P(I,4)*P(J,4)-P(I,1)*P(J,1)-P(I,2)*P(J,2)-P(I,3)*P(J,3) 
      HMEPS(HA)=((1.-HRQ-HA)**2+3.*HA*(1.+HRQ-HA))* 
     &SQRT((1.-HRQ-HA)**2-4.*HRQ*HA) 

C...Initial values. 
      NTRY=0 
      NSAV=N 
      KFA=IABS(K(IP,2)) 
      KFS=ISIGN(1,K(IP,2)) 
      KC=LUCOMP(KFA) 
      MSTJ(92)=0 
C...Choose lifetime and determine decay vertex. 
      IF(K(IP,1).EQ.5) THEN 
        V(IP,5)=0. 
      ELSEIF(K(IP,1).NE.4) THEN 
        V(IP,5)=-PMAS(KC,4)*LOG(RLU(0)) 
      ENDIF 
      DO 100 J=1,4 
      VDCY(J)=V(IP,J)+V(IP,5)*P(IP,J)/P(IP,5) 
  100 CONTINUE 
C...Determine whether decay allowed or not. 
      MOUT=0 
      IF(MSTJ(22).EQ.2) THEN 
        IF(PMAS(KC,4).GT.PARJ(71)) MOUT=1 
      ELSEIF(MSTJ(22).EQ.3) THEN 
        IF(VDCY(1)**2+VDCY(2)**2+VDCY(3)**2.GT.PARJ(72)**2) MOUT=1 
      ELSEIF(MSTJ(22).EQ.4) THEN 
        IF(VDCY(1)**2+VDCY(2)**2.GT.PARJ(73)**2) MOUT=1 
        IF(ABS(VDCY(3)).GT.PARJ(74)) MOUT=1 
      ENDIF 
      IF(MOUT.EQ.1.AND.K(IP,1).NE.5) THEN 
        K(IP,1)=4 
        RETURN 
      ENDIF 
 
C...Interface to external tau decay library (for tau polarization). 
      IF(KFA.EQ.15.AND.MSTJ(28).GE.1) THEN 
 
C...Starting values for pointers and momenta. 
        ITAU=IP 
        DO 110 J=1,4 
        PTAU(J)=P(ITAU,J) 
        PCMTAU(J)=P(ITAU,J) 
  110   CONTINUE 
 
C...Iterate to find position and code of mother of tau. 
        IMTAU=ITAU 
  120   IMTAU=K(IMTAU,3) 
 
        IF(IMTAU.EQ.0) THEN 
C...If no known origin then impossible to do anything further. 
          KFORIG=0 
          IORIG=0 
 
        ELSEIF(K(IMTAU,2).EQ.K(ITAU,2)) THEN 
C...If tau -> tau + gamma then add gamma energy and loop. 
          IF(K(K(IMTAU,4),2).EQ.22) THEN 
            DO 130 J=1,4 
            PCMTAU(J)=PCMTAU(J)+P(K(IMTAU,4),J) 
  130       CONTINUE 
          ELSEIF(K(K(IMTAU,5),2).EQ.22) THEN 
            DO 140 J=1,4 
            PCMTAU(J)=PCMTAU(J)+P(K(IMTAU,5),J) 
  140       CONTINUE 
          ENDIF 
          GOTO 120 
 
        ELSEIF(IABS(K(IMTAU,2)).GT.100) THEN 
C...If coming from weak decay of hadron then W is not stored in record, 
C...but can be reconstructed by adding neutrino momentum. 
          KFORIG=-ISIGN(24,K(ITAU,2)) 
          IORIG=0 
          DO 160 II=K(IMTAU,4),K(IMTAU,5) 
          IF(K(II,2)*ISIGN(1,K(ITAU,2)).EQ.-16) THEN 
            DO 150 J=1,4 
            PCMTAU(J)=PCMTAU(J)+P(II,J) 
  150       CONTINUE 
          ENDIF 
  160     CONTINUE 
 
        ELSE 
C...If coming from resonance decay then find latest copy of this 
C...resonance (may not completely agree). 
          KFORIG=K(IMTAU,2) 
          IORIG=IMTAU 
          DO 170 II=IMTAU+1,IP-1 
          IF(K(II,2).EQ.KFORIG.AND.K(II,3).EQ.IORIG.AND. 
     &    ABS(P(II,5)-P(IORIG,5)).LT.1E-5*P(IORIG,5)) IORIG=II 
  170     CONTINUE 
          DO 180 J=1,4 
          PCMTAU(J)=P(IORIG,J) 
  180     CONTINUE 
        ENDIF 
 
C...Boost tau to rest frame of production process (where known) 
C...and rotate it to sit along +z axis. 
        DO 190 J=1,3 
        DBETAU(J)=PCMTAU(J)/PCMTAU(4) 
  190   CONTINUE 
        IF(KFORIG.NE.0) CALL LUDBRB(ITAU,ITAU,0.,0.,-DBETAU(1), 
     &  -DBETAU(2),-DBETAU(3)) 
        PHITAU=ULANGL(P(ITAU,1),P(ITAU,2)) 
        CALL LUDBRB(ITAU,ITAU,0.,-PHITAU,0D0,0D0,0D0) 
        THETAU=ULANGL(P(ITAU,3),P(ITAU,1)) 
        CALL LUDBRB(ITAU,ITAU,-THETAU,0.,0D0,0D0,0D0) 
 
C...Call tau decay routine (if meaningful) and fill extra info. 
        IF(KFORIG.NE.0.OR.MSTJ(28).EQ.2) THEN 
          CALL LUTAUD(ITAU,IORIG,KFORIG,NDECAY) 
          DO 200 II=NSAV+1,NSAV+NDECAY 
          K(II,1)=1 
          K(II,3)=IP 
          K(II,4)=0 
          K(II,5)=0 
  200     CONTINUE 
          N=NSAV+NDECAY 
        ENDIF 
 
C...Boost back decay tau and decay products. 
        DO 210 J=1,4 
        P(ITAU,J)=PTAU(J) 
  210   CONTINUE 
        IF(KFORIG.NE.0.OR.MSTJ(28).EQ.2) THEN 
          CALL LUDBRB(NSAV+1,N,THETAU,PHITAU,0D0,0D0,0D0) 
          IF(KFORIG.NE.0) CALL LUDBRB(NSAV+1,N,0.,0.,DBETAU(1), 
     &    DBETAU(2),DBETAU(3)) 
 
C...Skip past ordinary tau decay treatment. 
          MMAT=0 
          MBST=0 
          ND=0 
          GOTO 660 
        ENDIF 
      ENDIF 
 
C...B-B~ mixing: flip sign of meson appropriately. 
      MMIX=0 
      IF((KFA.EQ.511.OR.KFA.EQ.531).AND.MSTJ(26).GE.1) THEN 
        XBBMIX=PARJ(76) 
        IF(KFA.EQ.531) XBBMIX=PARJ(77) 
        IF(SIN(0.5*XBBMIX*V(IP,5)/PMAS(KC,4))**2.GT.RLU(0)) MMIX=1 
        IF(MMIX.EQ.1) KFS=-KFS 
      ENDIF 
 
C...Check existence of decay channels. Particle/antiparticle rules. 
      KCA=KC 
      IF(MDCY(KC,2).GT.0) THEN 
        MDMDCY=MDME(MDCY(KC,2),2) 
        IF(MDMDCY.GT.80.AND.MDMDCY.LE.90) KCA=MDMDCY 
      ENDIF 
      IF(MDCY(KCA,2).LE.0.OR.MDCY(KCA,3).LE.0) THEN 
        CALL LUERRM(9,'(LUDECY:) no decay channel defined') 
        RETURN 
      ENDIF 
      IF(MOD(KFA/1000,10).EQ.0.AND.(KCA.EQ.85.OR.KCA.EQ.87)) KFS=-KFS 
      IF(KCHG(KC,3).EQ.0) THEN 
        KFSP=1 
        KFSN=0 
        IF(RLU(0).GT.0.5) KFS=-KFS 
      ELSEIF(KFS.GT.0) THEN 
        KFSP=1 
        KFSN=0 
      ELSE 
        KFSP=0 
        KFSN=1 
      ENDIF 
 
C...Sum branching ratios of allowed decay channels. 
  220 NOPE=0 
      BRSU=0. 
      DO 230 IDL=MDCY(KCA,2),MDCY(KCA,2)+MDCY(KCA,3)-1 
      IF(MDME(IDL,1).NE.1.AND.KFSP*MDME(IDL,1).NE.2.AND. 
     &KFSN*MDME(IDL,1).NE.3) GOTO 230 
      IF(MDME(IDL,2).GT.100) GOTO 230 
      NOPE=NOPE+1 
      BRSU=BRSU+BRAT(IDL) 
  230 CONTINUE 


      IF(NOPE.EQ.0) THEN 
        CALL LUERRM(2,'(LUDECY:) all decay channels closed by user') 
        RETURN 
      ENDIF 
 
C...Select decay channel among allowed ones. 
  240 RBR=BRSU*RLU(0) 
      IDL=MDCY(KCA,2)-1 
  250 IDL=IDL+1 
      IF(MDME(IDL,1).NE.1.AND.KFSP*MDME(IDL,1).NE.2.AND. 
     &KFSN*MDME(IDL,1).NE.3) THEN 
        IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1) GOTO 250 
      ELSEIF(MDME(IDL,2).GT.100) THEN 
        IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1) GOTO 250 
      ELSE 
        IDC=IDL 
        RBR=RBR-BRAT(IDL) 
        IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1.AND.RBR.GT.0.) GOTO 250 
      ENDIF 
cjc+++ f0(mass<1GeV)->pipi, f0(mass>1GeV) -> KK
      if(kfa.eq.10221) Then
         if(P(ip,5).lt.1.0) then !(f0->pipi)
C...Sum branching ratios of allowed decay channels. (pi pi)
           NOPE=0 
           BRSU=0. 
           DO 231 IDL=MDCY(KCA,2),MDCY(KCA,2)+1 
             IF(MDME(IDL,1).NE.1.AND.KFSP*MDME(IDL,1).NE.2.AND. 
     &         KFSN*MDME(IDL,1).NE.3) GOTO 231 
               IF(MDME(IDL,2).GT.100) GOTO 231 
               NOPE=NOPE+1 
               BRSU=BRSU+BRAT(IDL) 
 231       CONTINUE 
C...Select decay channel among allowed ones. (pipi)
           RBR=BRSU*RLU(0) 
           IDL=MDCY(KCA,2)-1 
 251       IDL=IDL+1 
           IF(MDME(IDL,1).NE.1.AND.KFSP*MDME(IDL,1).NE.2.AND. 
     &       KFSN*MDME(IDL,1).NE.3) THEN 
             IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1) GOTO 251 
           ELSEIF(MDME(IDL,2).GT.100) THEN 
             IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1) GOTO 251
           ELSE 
             IDC=IDL 
             RBR=RBR-BRAT(IDL) 
        IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1.AND.RBR.GT.0.) GOTO 251
           ENDIF 

        else           !(f0->KK)

C...Sum branching ratios of allowed decay channels. (KK)
         NOPE=0 
         BRSU=0. 
         DO 232 IDL=MDCY(KCA,2)+2,MDCY(KCA,2)+MDCY(KCA,3)-1 
            IF(MDME(IDL,1).NE.1.AND.KFSP*MDME(IDL,1).NE.2.AND. 
     &         KFSN*MDME(IDL,1).NE.3) GOTO 232 
            IF(MDME(IDL,2).GT.100) GOTO 232 
            NOPE=NOPE+1 
            BRSU=BRSU+BRAT(IDL) 
 232     CONTINUE 
C...Select decay channel among allowed ones. (KK)
         RBR=BRSU*RLU(0) 
         IDL=MDCY(KCA,2)+1 
 252     IDL=IDL+1 
         IF(MDME(IDL,1).NE.1.AND.KFSP*MDME(IDL,1).NE.2.AND. 
     &     KFSN*MDME(IDL,1).NE.3) THEN 
             IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1) GOTO 252
          ELSEIF(MDME(IDL,2).GT.100) THEN 
             IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1) GOTO 252 
          ELSE 
            IDC=IDL 
            RBR=RBR-BRAT(IDL) 
        IF(IDL.LT.MDCY(KCA,2)+MDCY(KCA,3)-1.AND.RBR.GT.0.) GOTO 252 
          ENDIF 

       endif
       endif
cjc--------------------            

C...Start readout of decay channel: matrix element, reset counters. 
      MMAT=MDME(IDC,2) 
  260 NTRY=NTRY+1 
      IF(NTRY.GT.100000) THEN 
        CALL LUERRM(14,'(LUDECY1:) caught in infinite loop') 
        call lulist(1)
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      I=N 
      NP=0 
      NQ=0 
      MBST=0 
      IF(MMAT.GE.11.AND.MMAT.NE.46.AND.P(IP,4).GT.20.*P(IP,5)) MBST=1 
      DO 270 J=1,4 
      PV(1,J)=0. 
      IF(MBST.EQ.0) PV(1,J)=P(IP,J) 
  270 CONTINUE 
      IF(MBST.EQ.1) PV(1,4)=P(IP,5) 
      PV(1,5)=P(IP,5) 
      PS=0. 
      PSQ=0. 
      MREM=0 
      MHADDY=0 
      IF(KFA.GT.80) MHADDY=1 
 
C...Read out decay products. Convert to standard flavour code. 
      JTMAX=5 
      IF(MDME(IDC+1,2).EQ.101) JTMAX=10 
      lonid=0
      idccharm=0
      IDL_charm0=0
      DO 280 JT=1,JTMAX 
      IF(JT.LE.5) KP=KFDP(IDC,JT) 
      IF(JT.GE.6) KP=KFDP(IDC+1,JT-5) 
cjc... J/psi or psi(2S)(for resonace's random quark)
      if(((kfa.eq.443).or.(kfa.eq.30443)).and.(lonid.ne.1)) then

C...Select decay channel among allowed ones(adding for charm in charm_updat.f).
        if((abs(kp).eq.82).and.(jt.eq.1)) then
  272     RBR=brat(idl)*RLU(0) 
            if(kfa.eq.443) then
              IDL_charm0=0                          !J/psi   IDCmin=1 
              idl_max=mdcy_charm(1)
             else  
               IDL_charm0=mdcy_charm(1)             !psi(2S)  IDCmin=51
              idl_max=mdcy_charm(2)
            endif  
            IDL_charm=IDL_charm0 
  274     IDL_charm=IDL_charm+1 
          IF(MDME_charm(IDL_charm,1).NE.1) THEN 
              IF(IDL_charm.LT.idl_max) GOTO 274
            ELSE 
              IDCcharm=IDL_charm 
              RBR=RBR-BRAT_charm(IDL_charm) 
              IF((IDL_charm.LT.idl_max).AND.(RBR.GT.0.)) GOTO 274
            ENDIF 
        endif

        if(idccharm.eq.(idl_charm0+1))then
            lonid=1                                             !qq~ decay
         elseif(idccharm.eq.(idl_charm0+2))then       
cjc... J/psi --> rgg/ggg    (for J/psi resonace's random quark)
            MMAT=MDME_charm(IDCcharm,2) 
            K(IP,1)=11                                !Mark decayed particle
            ipp=ip
            ngg=0
            call luonia1(ipp,4,ngg)                   ! rgg/ggg decay
            if(ngg.ge.1) ipg=ipp               !decay meson ip(used for G)
           return
         elseif(idccharm.ge.(idl_charm0+3)) then
            MMAT=MDME_charm(IDCcharm,2) 
           if(jt.le.5) then
              kp=kfdp_charm(idccharm,jt)                 ! adding particle channel
            else
              kp=0
           endif
         endif

      endif
cjc...
      IF(KP.EQ.0) GOTO 280 
      KPA=IABS(KP) 
      KCP=LUCOMP(KPA) 
      IF(KPA.GT.80) MHADDY=1 
      IF(KCHG(KCP,3).EQ.0.AND.KPA.NE.81.AND.KPA.NE.82) THEN 
        KFP=KP 
      ELSEIF(KPA.NE.81.AND.KPA.NE.82) THEN 
        KFP=KFS*KP 
      ELSEIF(KPA.EQ.81.AND.MOD(KFA/1000,10).EQ.0) THEN 
        KFP=-KFS*MOD(KFA/10,10) 
      ELSEIF(KPA.EQ.81.AND.MOD(KFA/100,10).GE.MOD(KFA/10,10)) THEN 
        KFP=KFS*(100*MOD(KFA/10,100)+3) 
      ELSEIF(KPA.EQ.81) THEN 
        KFP=KFS*(1000*MOD(KFA/10,10)+100*MOD(KFA/100,10)+1) 
      ELSEIF(KP.EQ.82) THEN 
        CALL LUKFDI(-KFS*INT(1.+(2.+PARJ(2))*RLU(0)),0,KFP,KDUMP) 
        IF(KFP.EQ.0) GOTO 260 
        MSTJ(93)=1 
        IF(PV(1,5).LT.PARJ(32)+2.*ULMASS(KFP)) GOTO 260 
      ELSEIF(KP.EQ.-82) THEN 
        KFP=-KFP 
        IF(IABS(KFP).GT.10) KFP=KFP+ISIGN(10000,KFP) 
      ENDIF 
      IF(KPA.EQ.81.OR.KPA.EQ.82) KCP=LUCOMP(KFP) 

C...Add decay product to event record or to quark flavour list. 
      KFPA=IABS(KFP) 
      KQP=KCHG(KCP,2) 
      IF(MMAT.GE.11.AND.MMAT.LE.30.AND.KQP.NE.0) THEN 
        NQ=NQ+1 
        KFLO(NQ)=KFP 
        MSTJ(93)=2 
        PSQ=PSQ+ULMASS(KFLO(NQ)) 
      ELSEIF((MMAT.EQ.42.OR.MMAT.EQ.43.OR.MMAT.EQ.48).AND.NP.EQ.3.AND. 
     &MOD(NQ,2).EQ.1) THEN 
        NQ=NQ-1 
        PS=PS-P(I,5) 
        K(I,1)=1 
        KFI=K(I,2) 
        CALL LUKFDI(KFP,KFI,KFLDMP,K(I,2)) 
        IF(K(I,2).EQ.0) GOTO 260 
        MSTJ(93)=1 
        P(I,5)=ULMASS(K(I,2)) 
        PS=PS+P(I,5) 
      ELSE 
        I=I+1 
        NP=NP+1 
        IF(MMAT.NE.33.AND.KQP.NE.0) NQ=NQ+1 
        IF(MMAT.EQ.33.AND.KQP.NE.0.AND.KQP.NE.2) NQ=NQ+1 
        K(I,1)=1+MOD(NQ,2) 
        IF(MMAT.EQ.4.AND.JT.LE.2.AND.KFP.EQ.21) K(I,1)=2 
        IF(MMAT.EQ.4.AND.JT.EQ.3) K(I,1)=1 
        K(I,2)=KFP 
        K(I,3)=IP 
        K(I,4)=0 
        K(I,5)=0 
        P(I,5)=ULMASS(KFP) 
        IF(MMAT.EQ.45.AND.KFPA.EQ.89) P(I,5)=PARJ(32) 
        PS=PS+P(I,5) 
      ENDIF 
  280 CONTINUE 
   
C...Check masses for resonance decays. 
      IF(MHADDY.EQ.0) THEN 
        IF(PS+PARJ(64).GT.PV(1,5)) GOTO 240 
      ENDIF 
 
C...Choose decay multiplicity in phase space model.  (cjc/99 qq~ decay) 
  290 IF(MMAT.GE.11.AND.MMAT.LE.30) THEN          
        PSP=PS 
        CNDE=PARJ(61)*LOG(MAX((PV(1,5)-PS-PSQ)/PARJ(62),1.1)) 
        IF(MMAT.EQ.12) CNDE=CNDE+PARJ(63) 
  300   NTRY=NTRY+1 
        IF(NTRY.GT.1000) THEN 
          CALL LUERRM(14,'(LUDECY2:) caught in infinite loop') 
        call lulist(1)
          IF(MSTU(21).GE.1) RETURN 
        ENDIF 
        IF(MMAT.LE.20) THEN 
          GAUSS=SQRT(-2.*CNDE*LOG(MAX(1E-10,RLU(0))))* 
     &    SIN(PARU(2)*RLU(0)) 
          ND=0.5+0.5*NP+0.25*NQ+CNDE+GAUSS 
          IF(ND.LT.NP+NQ/2.OR.ND.LT.2.OR.ND.GT.10) GOTO 300 
          IF(MMAT.EQ.13.AND.ND.EQ.2) GOTO 300 
          IF(MMAT.EQ.14.AND.ND.LE.3) GOTO 300 
          IF(MMAT.EQ.15.AND.ND.LE.4) GOTO 300 
        ELSE 
          ND=MMAT-20 
        ENDIF 
 
C...Form hadrons from flavour content. 
          DO 310 JT=1,4 
        KFL1(JT)=KFLO(JT) 
  310   CONTINUE 
        IF(ND.EQ.NP+NQ/2) GOTO 330 
        DO 320 I=N+NP+1,N+ND-NQ/2 
        JT=1+INT((NQ-1)*RLU(0)) 
        CALL LUKFDI(KFL1(JT),0,KFL2,K(I,2)) 
        IF(K(I,2).EQ.0) GOTO 300 
        KFL1(JT)=-KFL2 
  320   CONTINUE 
  330   JT=2 
        JT2=3 
        JT3=4 
        IF(NQ.EQ.4.AND.RLU(0).LT.PARJ(66)) JT=4 
        IF(JT.EQ.4.AND.ISIGN(1,KFL1(1)*(10-IABS(KFL1(1))))* 
     &  ISIGN(1,KFL1(JT)*(10-IABS(KFL1(JT)))).GT.0) JT=3 
        IF(JT.EQ.3) JT2=2 
        IF(JT.EQ.4) JT3=2 
        CALL LUKFDI(KFL1(1),KFL1(JT),KFLDMP,K(N+ND-NQ/2+1,2)) 
        IF(K(N+ND-NQ/2+1,2).EQ.0) GOTO 300 
        IF(NQ.EQ.4) CALL LUKFDI(KFL1(JT2),KFL1(JT3),KFLDMP,K(N+ND,2)) 
        IF(NQ.EQ.4.AND.K(N+ND,2).EQ.0) GOTO 300 
        
C...Check that sum of decay product masses not too large. 
        PS=PSP 
        DO 340 I=N+NP+1,N+ND 
        K(I,1)=1 
        K(I,3)=IP 
        K(I,4)=0 
        K(I,5)=0 
        P(I,5)=ULMASS(K(I,2)) 
        PS=PS+P(I,5) 
  340   CONTINUE 
        IF(PS+PARJ(64).GT.PV(1,5)) GOTO 300 
c... check C parity conservation(in gamma*-->qq~)
        ip1=ip
        ip2=n+1     ! np=0 usually when mmat=11-30,except g,q in products
        ip3=n+nd
	icg=1
        call cgparity(ip1,ip2,ip3,kcok,0,icg)
        if(kcok.eq.-1) goto 300
        call psupress(ip1,ip2,ip3,ksok)
        if(ksok.eq.-1) goto 300

C...Rescale energy to subtract off spectator quark mass. 
      ELSEIF((MMAT.EQ.31.OR.MMAT.EQ.33.OR.MMAT.EQ.44.OR.MMAT.EQ.45) 
     &.AND.NP.GE.3) THEN 
        PS=PS-P(N+NP,5) 
        PQT=(P(N+NP,5)+PARJ(65))/PV(1,5) 
        DO 350 J=1,5 
        P(N+NP,J)=PQT*PV(1,J) 
        PV(1,J)=(1.-PQT)*PV(1,J) 
  350   CONTINUE 
        IF(PS+PARJ(64).GT.PV(1,5)) GOTO 260 
        ND=NP-1 
        MREM=1 
 
C...Phase space factors imposed in W decay. 
      ELSEIF(MMAT.EQ.46) THEN 
        MSTJ(93)=1 
        PSMC=ULMASS(K(N+1,2)) 
        MSTJ(93)=1 
        PSMC=PSMC+ULMASS(K(N+2,2)) 
        IF(MAX(PS,PSMC)+PARJ(32).GT.PV(1,5)) GOTO 240 
        HR1=(P(N+1,5)/PV(1,5))**2 
        HR2=(P(N+2,5)/PV(1,5))**2 
        IF((1.-HR1-HR2)*(2.+HR1+HR2)*SQRT((1.-HR1-HR2)**2-4.*HR1*HR2) 
     &  .LT.2.*RLU(0)) GOTO 240 
        ND=NP 
 
C...Fully specified final state: check mass broadening effects. 
      ELSE 
        IF(NP.GE.2.AND.PS+PARJ(64).GT.PV(1,5)) GOTO 260 
        ND=NP 
      ENDIF 
 
C...Select W mass in decay Q -> W + q, without W propagator. 
      IF(MMAT.EQ.45.AND.MSTJ(25).LE.0) THEN 
        HLQ=(PARJ(32)/PV(1,5))**2 
        HUQ=(1.-(P(N+2,5)+PARJ(64))/PV(1,5))**2 
        HRQ=(P(N+2,5)/PV(1,5))**2 
  360   HW=HLQ+RLU(0)*(HUQ-HLQ) 
        IF(HMEPS(HW).LT.RLU(0)) GOTO 360 
        P(N+1,5)=PV(1,5)*SQRT(HW) 
 
C...Ditto, including W propagator. Divide mass range into three regions. 
      ELSEIF(MMAT.EQ.45) THEN 
        HQW=(PV(1,5)/PMAS(24,1))**2 
        HLW=(PARJ(32)/PMAS(24,1))**2 
        HUW=((PV(1,5)-P(N+2,5)-PARJ(64))/PMAS(24,1))**2 
        HRQ=(P(N+2,5)/PV(1,5))**2 
        HG=PMAS(24,2)/PMAS(24,1) 
        HATL=ATAN((HLW-1.)/HG) 
        HM=MIN(1.,HUW-0.001) 
        HMV1=HMEPS(HM/HQW)/((HM-1.)**2+HG**2) 
  370   HM=HM-HG 
        HMV2=HMEPS(HM/HQW)/((HM-1.)**2+HG**2) 
        IF(HMV2.GT.HMV1.AND.HM-HG.GT.HLW) THEN 
          HMV1=HMV2 
          GOTO 370 
        ENDIF 
        HMV=MIN(2.*HMV1,HMEPS(HM/HQW)/HG**2) 
        HM1=1.-SQRT(1./HMV-HG**2) 
        IF(HM1.GT.HLW.AND.HM1.LT.HM) THEN 
          HM=HM1 
        ELSEIF(HMV2.LE.HMV1) THEN 
          HM=MAX(HLW,HM-MIN(0.1,1.-HM)) 
        ENDIF 
        HATM=ATAN((HM-1.)/HG) 
        HWT1=(HATM-HATL)/HG 
        HWT2=HMV*(MIN(1.,HUW)-HM) 
        HWT3=0. 
        IF(HUW.GT.1.) THEN 
          HATU=ATAN((HUW-1.)/HG) 
          HMP1=HMEPS(1./HQW) 
          HWT3=HMP1*HATU/HG 
        ENDIF 
 
C...Select mass region and W mass there. Accept according to weight. 
  380   HREG=RLU(0)*(HWT1+HWT2+HWT3) 
        IF(HREG.LE.HWT1) THEN 
          HW=1.+HG*TAN(HATL+RLU(0)*(HATM-HATL)) 
          HACC=HMEPS(HW/HQW) 
        ELSEIF(HREG.LE.HWT1+HWT2) THEN 
          HW=HM+RLU(0)*(MIN(1.,HUW)-HM) 
          HACC=HMEPS(HW/HQW)/((HW-1.)**2+HG**2)/HMV 
        ELSE 
          HW=1.+HG*TAN(RLU(0)*HATU) 
          HACC=HMEPS(HW/HQW)/HMP1 
        ENDIF 
        IF(HACC.LT.RLU(0)) GOTO 380 
        P(N+1,5)=PMAS(24,1)*SQRT(HW) 
      ENDIF 
 
C...Determine position of grandmother, number of sisters, Q -> W sign. 
      NM=0 
      KFAS=0 
      MSGN=0 
      IF(MMAT.EQ.3.OR.MMAT.EQ.46) THEN 
        IM=K(IP,3) 
        IF(IM.LT.0.OR.IM.GE.IP) IM=0 
        IF(MMAT.EQ.46.AND.MSTJ(27).EQ.1) THEN 
          IM=0 
        ELSEIF(MMAT.EQ.46.AND.MSTJ(27).GE.2.AND.IM.NE.0) THEN 
          IF(K(IM,2).EQ.94) THEN 
            IM=K(K(IM,3),3) 
            IF(IM.LT.0.OR.IM.GE.IP) IM=0 
          ENDIF 
        ENDIF 
        IF(IM.NE.0) KFAM=IABS(K(IM,2)) 
        IF(IM.NE.0.AND.MMAT.EQ.3) THEN 
          DO 390 IL=MAX(IP-2,IM+1),MIN(IP+2,N) 
          IF(K(IL,3).EQ.IM) NM=NM+1 
          IF(K(IL,3).EQ.IM.AND.IL.NE.IP) ISIS=IL 
  390     CONTINUE 
          IF(NM.NE.2.OR.KFAM.LE.100.OR.MOD(KFAM,10).NE.1.OR. 
     &    MOD(KFAM/1000,10).NE.0) NM=0 
          IF(NM.EQ.2) THEN 
            KFAS=IABS(K(ISIS,2)) 
            IF((KFAS.LE.100.OR.MOD(KFAS,10).NE.1.OR. 
     &      MOD(KFAS/1000,10).NE.0).AND.KFAS.NE.22) NM=0 
          ENDIF 
        ELSEIF(IM.NE.0.AND.MMAT.EQ.46) THEN 
          MSGN=ISIGN(1,K(IM,2)*K(IP,2)) 
          IF(KFAM.GT.100.AND.MOD(KFAM/1000,10).EQ.0) MSGN= 
     &    MSGN*(-1)**MOD(KFAM/100,10) 
        ENDIF 
      ENDIF 
 
C...Kinematics of one-particle decays. 
      IF(ND.EQ.1) THEN 
        DO 400 J=1,4 
        P(N+1,J)=P(IP,J) 
  400   CONTINUE 
        GOTO 660 
      ENDIF 
 
C...Calculate maximum weight ND-particle decay. 
      PV(ND,5)=P(N+ND,5) 
      IF(ND.GE.3) THEN 
        WTMAX=1./WTCOR(ND-2) 
        PMAX=PV(1,5)-PS+P(N+ND,5) 
        PMIN=0. 
        DO 410 IL=ND-1,1,-1 
        PMAX=PMAX+P(N+IL,5) 
        PMIN=PMIN+P(N+IL+1,5) 
        WTMAX=WTMAX*PAWT(PMAX,PMIN,P(N+IL,5)) 
  410   CONTINUE 
      ENDIF 
 
C...Find virtual gamma mass in Dalitz decay. 
  420 IF(ND.EQ.2) THEN 
      ELSEIF(MMAT.EQ.2) THEN 
        PMES=4.*PMAS(11,1)**2 
        PMRHO2=PMAS(131,1)**2 
        PGRHO2=PMAS(131,2)**2 
  430   PMST=PMES*(P(IP,5)**2/PMES)**RLU(0) 
        WT=(1+0.5*PMES/PMST)*SQRT(MAX(0.,1.-PMES/PMST))* 
     &  (1.-PMST/P(IP,5)**2)**3*(1.+PGRHO2/PMRHO2)/ 
     &  ((1.-PMST/PMRHO2)**2+PGRHO2/PMRHO2) 
        IF(WT.LT.RLU(0)) GOTO 430 
        PV(2,5)=MAX(2.00001*PMAS(11,1),SQRT(PMST)) 
 
C...M-generator gives weight. If rejected, try again. 
      ELSE 
  440   RORD(1)=1. 
        DO 470 IL1=2,ND-1 
        RSAV=RLU(0) 
        DO 450 IL2=IL1-1,1,-1 
        IF(RSAV.LE.RORD(IL2)) GOTO 460 
        RORD(IL2+1)=RORD(IL2) 
  450   CONTINUE 
  460   RORD(IL2+1)=RSAV 
  470   CONTINUE 
        RORD(ND)=0. 
        WT=1. 
        DO 480 IL=ND-1,1,-1 
        PV(IL,5)=PV(IL+1,5)+P(N+IL,5)+(RORD(IL)-RORD(IL+1))*(PV(1,5)-PS) 
        WT=WT*PAWT(PV(IL,5),PV(IL+1,5),P(N+IL,5)) 
  480   CONTINUE 
        IF(WT.LT.RLU(0)*WTMAX) GOTO 440 
      ENDIF 

C...Perform two-particle decays in respective CM frame. 
  490 DO 510 IL=1,ND-1 
      PA=PAWT(PV(IL,5),PV(IL+1,5),P(N+IL,5)) 
      UE(3)=2.*RLU(0)-1. 
      PHI=PARU(2)*RLU(0) 
      UE(1)=SQRT(1.-UE(3)**2)*COS(PHI) 
      UE(2)=SQRT(1.-UE(3)**2)*SIN(PHI) 
      DO 500 J=1,3 
      P(N+IL,J)=PA*UE(J) 
      PV(IL+1,J)=-PA*UE(J) 
  500 CONTINUE 
      P(N+IL,4)=SQRT(PA**2+P(N+IL,5)**2) 
      PV(IL+1,4)=SQRT(PA**2+PV(IL+1,5)**2) 
  510 CONTINUE 

C...Angular distribution for J/psi -> e+e- or mu+mu- (1+1.0*cos^2(theta)) 
      IF(MMAT.EQ.3.AND.(idl.eq.721.or.idl.eq.722)) THEN 
      pn1=sqrt(p(n+1,1)*p(n+1,1)+p(n+1,2)*p(n+1,2)+p(n+1,3)*p(n+1,3))
c      if(xparam(2).ne.0.) A_angl=xparam(2)
         if(pn1.gt.0.) then
           costh=p(n+1,3)/pn1
          else
           costh=10.
         endif
         if(A_angl.ge.0) then         
           ANGDIS=(1.0+A_angl*costh*costh)/(1.0+A_angl)
         else
           ANGDIS=(1.0+A_angl*costh*costh)
         endif
        IF(RLU(0).GT.ANGDIS) GOTO 490
       endif

C...Lorentz transform decay products to lab frame. 
      DO 520 J=1,4 
      P(N+ND,J)=PV(ND,J) 
  520 CONTINUE 
      DO 560 IL=ND-1,1,-1 
      DO 530 J=1,3 
      BE(J)=PV(IL,J)/PV(IL,4) 
  530 CONTINUE 
      GA=PV(IL,4)/PV(IL,5) 
      DO 550 I=N+IL,N+ND 
      BEP=BE(1)*P(I,1)+BE(2)*P(I,2)+BE(3)*P(I,3) 
      DO 540 J=1,3 
      P(I,J)=P(I,J)+GA*(GA*BEP/(1.+GA)+P(I,4))*BE(J) 
  540 CONTINUE 
      P(I,4)=GA*(P(I,4)+BEP) 
  550 CONTINUE 
  560 CONTINUE 
 
C...Check that no infinite loop in matrix element weight. 
      NTRY=NTRY+1 
      IF(NTRY.GT.800) GOTO 590 
 
C...Matrix elements for omega and phi decays. 
      IF(MMAT.EQ.1) THEN 
        WT=(P(N+1,5)*P(N+2,5)*P(N+3,5))**2-(P(N+1,5)*FOUR(N+2,N+3))**2 
     &  -(P(N+2,5)*FOUR(N+1,N+3))**2-(P(N+3,5)*FOUR(N+1,N+2))**2 
     &  +2.*FOUR(N+1,N+2)*FOUR(N+1,N+3)*FOUR(N+2,N+3) 
        IF(MAX(WT*WTCOR(9)/P(IP,5)**6,0.001).LT.RLU(0)) GOTO 420 
 
C...Matrix elements for pi0 or eta Dalitz decay to gamma e+ e-. 
      ELSEIF(MMAT.EQ.2) THEN 
        FOUR12=FOUR(N+1,N+2) 
        FOUR13=FOUR(N+1,N+3) 
        WT=(PMST-0.5*PMES)*(FOUR12**2+FOUR13**2)+ 
     &  PMES*(FOUR12*FOUR13+FOUR12**2+FOUR13**2) 
        IF(WT.LT.RLU(0)*0.25*PMST*(P(IP,5)**2-PMST)**2) GOTO 490 
 
C...Matrix element for S0 -> S1 + V1 -> S1 + S2 + S3 (S scalar, 
C...V vector), of form cos**2(theta02) in V1 rest frame, and for 
C...S0 -> gamma + V1 -> gamma + S2 + S3, of form sin**2(theta02). 
      ELSEIF(MMAT.EQ.3.AND.NM.EQ.2) THEN 
        FOUR10=FOUR(IP,IM) 
        FOUR12=FOUR(IP,N+1) 
        FOUR02=FOUR(IM,N+1) 
        PMS1=P(IP,5)**2 
        PMS0=P(IM,5)**2 
        PMS2=P(N+1,5)**2 
        IF(KFAS.NE.22) HNUM=(FOUR10*FOUR12-PMS1*FOUR02)**2 
        IF(KFAS.EQ.22) HNUM=PMS1*(2.*FOUR10*FOUR12*FOUR02- 
     &  PMS1*FOUR02**2-PMS0*FOUR12**2-PMS2*FOUR10**2+PMS1*PMS0*PMS2) 
        HNUM=MAX(1E-6*PMS1**2*PMS0*PMS2,HNUM) 
        HDEN=(FOUR10**2-PMS1*PMS0)*(FOUR12**2-PMS1*PMS2) 
        IF(HNUM.LT.RLU(0)*HDEN) GOTO 490  
C...Matrix element for "onium" -> g + g + g or gamma + g + g. 
      ELSEIF(MMAT.EQ.4) THEN 
        HX1=2.*FOUR(IP,N+1)/P(IP,5)**2 
        HX2=2.*FOUR(IP,N+2)/P(IP,5)**2 
        HX3=2.*FOUR(IP,N+3)/P(IP,5)**2 
        WT=((1.-HX1)/(HX2*HX3))**2+((1.-HX2)/(HX1*HX3))**2+ 
     &  ((1.-HX3)/(HX1*HX2))**2 
        IF(WT.LT.2.*RLU(0)) GOTO 420 
        IF(K(IP+1,2).EQ.22.AND.(1.-HX1)*P(IP,5)**2.LT.4.*PARJ(32)**2) 
     &  GOTO 420 
 
C...Effective matrix element for nu spectrum in tau -> nu + hadrons. 
      ELSEIF(MMAT.EQ.41) THEN 
        HX1=2.*FOUR(IP,N+1)/P(IP,5)**2 
        HXM=MIN(0.75,2.*(1.-PS/P(IP,5))) 
        IF(HX1*(3.-2.*HX1).LT.RLU(0)*HXM*(3.-2.*HXM)) GOTO 420 
 
C...Matrix elements for weak decays (only semileptonic for c and b) 
      ELSEIF((MMAT.EQ.42.OR.MMAT.EQ.43.OR.MMAT.EQ.44.OR.MMAT.EQ.48) 
     &.AND.ND.EQ.3) THEN 
        IF(MBST.EQ.0) WT=FOUR(IP,N+1)*FOUR(N+2,N+3) 
        IF(MBST.EQ.1) WT=P(IP,5)*P(N+1,4)*FOUR(N+2,N+3) 
        IF(WT.LT.RLU(0)*P(IP,5)*PV(1,5)**3/WTCOR(10)) GOTO 420 
      ELSEIF(MMAT.EQ.42.OR.MMAT.EQ.43.OR.MMAT.EQ.44.OR.MMAT.EQ.48) THEN 
        DO 580 J=1,4 
        P(N+NP+1,J)=0. 
        DO 570 IS=N+3,N+NP 
        P(N+NP+1,J)=P(N+NP+1,J)+P(IS,J) 
  570   CONTINUE 
  580   CONTINUE 
        IF(MBST.EQ.0) WT=FOUR(IP,N+1)*FOUR(N+2,N+NP+1) 
        IF(MBST.EQ.1) WT=P(IP,5)*P(N+1,4)*FOUR(N+2,N+NP+1) 
        IF(WT.LT.RLU(0)*P(IP,5)*PV(1,5)**3/WTCOR(10)) GOTO 420 
 
C...Angular distribution in W decay. 
      ELSEIF(MMAT.EQ.46.AND.MSGN.NE.0) THEN 
        IF(MSGN.GT.0) WT=FOUR(IM,N+1)*FOUR(N+2,IP+1) 
        IF(MSGN.LT.0) WT=FOUR(IM,N+2)*FOUR(N+1,IP+1) 
        IF(WT.LT.RLU(0)*P(IM,5)**4/WTCOR(10)) GOTO 490 
      ENDIF 
 
C...Scale back energy and reattach spectator. 
  590 IF(MREM.EQ.1) THEN 
        DO 600 J=1,5 
        PV(1,J)=PV(1,J)/(1.-PQT) 
  600   CONTINUE 
        ND=ND+1 
        MREM=0 
      ENDIF 
 
C...Low invariant mass for system with spectator quark gives particle, 
C...not two jets. Readjust momenta accordingly. 
      IF((MMAT.EQ.31.OR.MMAT.EQ.45).AND.ND.EQ.3) THEN 
        MSTJ(93)=1 
        PM2=ULMASS(K(N+2,2)) 
        MSTJ(93)=1 
        PM3=ULMASS(K(N+3,2)) 
        IF(P(N+2,5)**2+P(N+3,5)**2+2.*FOUR(N+2,N+3).GE. 
     &  (PARJ(32)+PM2+PM3)**2) GOTO 660 
        K(N+2,1)=1 
        KFTEMP=K(N+2,2) 
        CALL LUKFDI(KFTEMP,K(N+3,2),KFLDMP,K(N+2,2)) 
        IF(K(N+2,2).EQ.0) GOTO 260 
        P(N+2,5)=ULMASS(K(N+2,2)) 
        PS=P(N+1,5)+P(N+2,5) 
        PV(2,5)=P(N+2,5) 
        MMAT=0 
        ND=2 
        GOTO 490 
      ELSEIF(MMAT.EQ.44) THEN 
        MSTJ(93)=1 
        PM3=ULMASS(K(N+3,2)) 
        MSTJ(93)=1 
        PM4=ULMASS(K(N+4,2)) 
        IF(P(N+3,5)**2+P(N+4,5)**2+2.*FOUR(N+3,N+4).GE. 
     &  (PARJ(32)+PM3+PM4)**2) GOTO 630 
        K(N+3,1)=1 
        KFTEMP=K(N+3,2) 
        CALL LUKFDI(KFTEMP,K(N+4,2),KFLDMP,K(N+3,2)) 
        IF(K(N+3,2).EQ.0) GOTO 260 
        P(N+3,5)=ULMASS(K(N+3,2)) 
        DO 610 J=1,3 
        P(N+3,J)=P(N+3,J)+P(N+4,J) 
  610   CONTINUE 
        P(N+3,4)=SQRT(P(N+3,1)**2+P(N+3,2)**2+P(N+3,3)**2+P(N+3,5)**2) 
        HA=P(N+1,4)**2-P(N+2,4)**2 
        HB=HA-(P(N+1,5)**2-P(N+2,5)**2) 
        HC=(P(N+1,1)-P(N+2,1))**2+(P(N+1,2)-P(N+2,2))**2+ 
     &  (P(N+1,3)-P(N+2,3))**2 
        HD=(PV(1,4)-P(N+3,4))**2 
        HE=HA**2-2.*HD*(P(N+1,4)**2+P(N+2,4)**2)+HD**2 
        HF=HD*HC-HB**2 
        HG=HD*HC-HA*HB 
        HH=(SQRT(HG**2+HE*HF)-HG)/(2.*HF) 
        DO 620 J=1,3 
        PCOR=HH*(P(N+1,J)-P(N+2,J)) 
        P(N+1,J)=P(N+1,J)+PCOR 
        P(N+2,J)=P(N+2,J)-PCOR 
  620   CONTINUE 
        P(N+1,4)=SQRT(P(N+1,1)**2+P(N+1,2)**2+P(N+1,3)**2+P(N+1,5)**2) 
        P(N+2,4)=SQRT(P(N+2,1)**2+P(N+2,2)**2+P(N+2,3)**2+P(N+2,5)**2) 
        ND=ND-1 
      ENDIF 

C...Check invariant mass of W jets. May give one particle or start over. 
  630 IF((MMAT.EQ.42.OR.MMAT.EQ.43.OR.MMAT.EQ.44.OR.MMAT.EQ.48) 
     &.AND.IABS(K(N+1,2)).LT.10) THEN 
        PMR=SQRT(MAX(0.,P(N+1,5)**2+P(N+2,5)**2+2.*FOUR(N+1,N+2))) 
        MSTJ(93)=1 
        PM1=ULMASS(K(N+1,2)) 
        MSTJ(93)=1 
        PM2=ULMASS(K(N+2,2)) 
        IF(PMR.GT.PARJ(32)+PM1+PM2) GOTO 640 
        KFLDUM=INT(1.5+RLU(0)) 
        CALL LUKFDI(K(N+1,2),-ISIGN(KFLDUM,K(N+1,2)),KFLDMP,KF1) 
        CALL LUKFDI(K(N+2,2),-ISIGN(KFLDUM,K(N+2,2)),KFLDMP,KF2) 
        IF(KF1.EQ.0.OR.KF2.EQ.0) GOTO 260 
        PSM=ULMASS(KF1)+ULMASS(KF2) 
        IF((MMAT.EQ.42.OR.MMAT.EQ.48).AND.PMR.GT.PARJ(64)+PSM) GOTO 640 
        IF(MMAT.GE.43.AND.PMR.GT.0.2*PARJ(32)+PSM) GOTO 640 
        IF(MMAT.EQ.48) GOTO 420 
        IF(ND.EQ.4.OR.KFA.EQ.15) GOTO 260 
        K(N+1,1)=1 
        KFTEMP=K(N+1,2) 
        CALL LUKFDI(KFTEMP,K(N+2,2),KFLDMP,K(N+1,2)) 
        IF(K(N+1,2).EQ.0) GOTO 260 
        P(N+1,5)=ULMASS(K(N+1,2)) 
        K(N+2,2)=K(N+3,2) 
        P(N+2,5)=P(N+3,5) 
        PS=P(N+1,5)+P(N+2,5) 
        IF(PS+PARJ(64).GT.PV(1,5)) GOTO 260 
        PV(2,5)=P(N+3,5) 
        MMAT=0 
        ND=2 
        GOTO 490 
      ENDIF 
 
C...Phase space decay of partons from W decay. 
  640 IF((MMAT.EQ.42.OR.MMAT.EQ.48).AND.IABS(K(N+1,2)).LT.10) THEN 
        KFLO(1)=K(N+1,2) 
        KFLO(2)=K(N+2,2) 
        K(N+1,1)=K(N+3,1) 
        K(N+1,2)=K(N+3,2) 
        DO 650 J=1,5 
        PV(1,J)=P(N+1,J)+P(N+2,J) 
        P(N+1,J)=P(N+3,J) 
  650   CONTINUE 
        PV(1,5)=PMR 
        N=N+1 
        NP=0 
        NQ=2 
        PS=0. 
        MSTJ(93)=2 
        PSQ=ULMASS(KFLO(1)) 
        MSTJ(93)=2 
        PSQ=PSQ+ULMASS(KFLO(2)) 
        MMAT=11 
        GOTO 290 
      ENDIF 
 
C...Boost back for rapidly moving particle. 
  660 N=N+ND 
      IF(MBST.EQ.1) THEN 
        DO 670 J=1,3 
        BE(J)=P(IP,J)/P(IP,4) 
  670   CONTINUE 
        GA=P(IP,4)/P(IP,5) 
        DO 690 I=NSAV+1,N 
        BEP=BE(1)*P(I,1)+BE(2)*P(I,2)+BE(3)*P(I,3) 
        DO 680 J=1,3 
        P(I,J)=P(I,J)+GA*(GA*BEP/(1.+GA)+P(I,4))*BE(J) 
  680   CONTINUE 
        P(I,4)=GA*(P(I,4)+BEP) 
  690   CONTINUE 
      ENDIF 
 
C...Fill in position of decay vertex. 
      DO 710 I=NSAV+1,N 
      DO 700 J=1,4 
      V(I,J)=VDCY(J) 
  700 CONTINUE 
      V(I,5)=0. 
  710 CONTINUE 
 
C...Set up for parton shower evolution from jets. 
      IF(MSTJ(23).GE.1.AND.MMAT.EQ.4.AND.K(NSAV+1,2).EQ.21) THEN 
        K(NSAV+1,1)=3 
        K(NSAV+2,1)=3 
        K(NSAV+3,1)=3 
        K(NSAV+1,4)=MSTU(5)*(NSAV+2) 
        K(NSAV+1,5)=MSTU(5)*(NSAV+3) 
        K(NSAV+2,4)=MSTU(5)*(NSAV+3) 
        K(NSAV+2,5)=MSTU(5)*(NSAV+1) 
        K(NSAV+3,4)=MSTU(5)*(NSAV+1) 
        K(NSAV+3,5)=MSTU(5)*(NSAV+2) 
        MSTJ(92)=-(NSAV+1) 
      ELSEIF(MSTJ(23).GE.1.AND.MMAT.EQ.4) THEN 
        K(NSAV+2,1)=3 
        K(NSAV+3,1)=3 
        K(NSAV+2,4)=MSTU(5)*(NSAV+3) 
        K(NSAV+2,5)=MSTU(5)*(NSAV+3) 
        K(NSAV+3,4)=MSTU(5)*(NSAV+2) 
        K(NSAV+3,5)=MSTU(5)*(NSAV+2) 
        MSTJ(92)=NSAV+2 
      ELSEIF(MSTJ(23).GE.1.AND.(MMAT.EQ.32.OR.MMAT.EQ.44.OR.MMAT.EQ.46) 
     &.AND.IABS(K(NSAV+1,2)).LE.10.AND.IABS(K(NSAV+2,2)).LE.10) THEN 
        K(NSAV+1,1)=3 
        K(NSAV+2,1)=3 
        K(NSAV+1,4)=MSTU(5)*(NSAV+2) 
        K(NSAV+1,5)=MSTU(5)*(NSAV+2) 
        K(NSAV+2,4)=MSTU(5)*(NSAV+1) 
        K(NSAV+2,5)=MSTU(5)*(NSAV+1) 
        MSTJ(92)=NSAV+1 
      ELSEIF(MSTJ(23).GE.1.AND.(MMAT.EQ.32.OR.MMAT.EQ.44.OR.MMAT.EQ.46) 
     &.AND.IABS(K(NSAV+1,2)).LE.20.AND.IABS(K(NSAV+2,2)).LE.20) THEN 
        MSTJ(92)=NSAV+1 
      ELSEIF(MSTJ(23).GE.1.AND.MMAT.EQ.33.AND.IABS(K(NSAV+2,2)).EQ.21) 
     &THEN 
        K(NSAV+1,1)=3 
        K(NSAV+2,1)=3 
        K(NSAV+3,1)=3 
        KCP=LUCOMP(K(NSAV+1,2)) 
        KQP=KCHG(KCP,2)*ISIGN(1,K(NSAV+1,2)) 
        JCON=4 
        IF(KQP.LT.0) JCON=5 
        K(NSAV+1,JCON)=MSTU(5)*(NSAV+2) 
        K(NSAV+2,9-JCON)=MSTU(5)*(NSAV+1) 
        K(NSAV+2,JCON)=MSTU(5)*(NSAV+3) 
        K(NSAV+3,9-JCON)=MSTU(5)*(NSAV+2) 
        MSTJ(92)=NSAV+1 
      ELSEIF(MSTJ(23).GE.1.AND.MMAT.EQ.33) THEN 
        K(NSAV+1,1)=3 
        K(NSAV+3,1)=3 
        K(NSAV+1,4)=MSTU(5)*(NSAV+3) 
        K(NSAV+1,5)=MSTU(5)*(NSAV+3) 
        K(NSAV+3,4)=MSTU(5)*(NSAV+1) 
        K(NSAV+3,5)=MSTU(5)*(NSAV+1) 
        MSTJ(92)=NSAV+1 
 
C...Set up for parton shower evolution in t -> W + b. 
      ELSEIF(MSTJ(27).GE.1.AND.MMAT.EQ.45.AND.ND.EQ.3) THEN 
        K(NSAV+2,1)=3 
        K(NSAV+3,1)=3 
        K(NSAV+2,4)=MSTU(5)*(NSAV+3) 
        K(NSAV+2,5)=MSTU(5)*(NSAV+3) 
        K(NSAV+3,4)=MSTU(5)*(NSAV+2) 
        K(NSAV+3,5)=MSTU(5)*(NSAV+2) 
        MSTJ(92)=NSAV+1 
      ENDIF 
 
C...Mark decayed particle; special option for B-B~ mixing. 
      IF(K(IP,1).EQ.5) K(IP,1)=15 
      IF(K(IP,1).LE.10) K(IP,1)=11 
      IF(MMIX.EQ.1.AND.MSTJ(26).EQ.2.AND.K(IP,1).EQ.11) K(IP,1)=12 
      K(IP,4)=NSAV+1 
      K(IP,5)=N 
 
      RETURN 
      END 

************************************************************************* 
       SUBROUTINE cgparity(ip1,ip2,ip3,kgok,ngg,icg) 
***************************************************************************** 
*   Purpose:(1) Initial C&G parities of mesons(PDG98).                      *
*           (2) Compare C&G parities between decay meson and its decayed    *
*               products.                                                   *
*                   C=(-1)^J, G=(-1)^I*C                                    *
*    called:  luexec1.f, ludecy1.f                                          *
*   used in:  luexec1.f, ludecy1.f                                          *
*   lgp1(i):  KF of meson ith meson, 1<i<ndim number of mesons.             *
*   lgp2C(i):  C parity of ith meson.                                        *
*   lgp2G(i):  G parity of ith meson.                                        *
*   lgpJ(i):  2*J of ith meson.                                             *
*   lgpI(i):  2*I of ith meson.                                             *
*    pgp(i):  The ith meson takes I+J or I+J+1.                             *
*     input:  ip1, decay meson position in the /LUJETS/;                    *
*             ip2-ip3, decayed products position.                           *
*             ngg, 1/2 ggg/gamma*gg                                         *
*             icg 1=Cparity/2=Gparity                                       *
*    output:  kgok,1:C/Gparity ok; -1:bad;                                  *
*                                      Chen Jiangchuan  1999.11.28          *
*****************************************************************************
      implicit none
      integer ndim,npart
      parameter (ndim=100,npart=62)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      integer N,K,P,V
      integer kgok,ip,ip1,ip2,ip3,initg,lastg,istatus,i,ndecy,orbitj
      integer lgp1(ndim),lgp2c(ndim),lgp3(20),lgp2g(ndim),lgpJ(ndim),
     &  lgpI(ndim), ngg,icg,orbiti
      real pgj(ndim),pgi(ndim)
      save lgp1,lgp2c,lgp2g,lgpJ,lgpI,pgj,pgi
      real RLU
C....initial Gparities of mesons
*************************************************************************
c...KF of mesons in the jetset7.4
c               pi+ pi0 eta  eta'  f0    f0'    f1   f1'   f2 f2'  rho+
      data lgp1/211,111,221, 331, 10221,10331,20223,20333,225,335,213,
c     rho0 omega a0+  a00  a1+  a10   a2+ a20 phi   h1    h1'  h1c
     &113,223,10211,10111,20213,20113,215,115,333,10223,10333,10443,
c     b1+   b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon' etab
     &10213,10113,441, 443,10441,20443,445,30443,  553,  30553, 551,
c     gamma D0 XGM- XGM+ XGM0 XGM*0 XGM*- XGM*+ Xi- Xi0
     & 22,  3122,3112,3222,3212,3214,3114, 3224,3312,3322,
c      Delta+ Delta- Delta0 Delta++ K+ K0 K*+ K*0  K1+  K10   K0*+ 
     & 2214,  1114,  2114,  2224, 321,311,323,313,10323,10313,10321,
c      K0*0  K1*+  K1*0   K2*+ K2*0 n0  p+ 
     &10311,20323,20313, 325, 315,2112,2212,38*0/

c...C parities of mesons for single meson
c               pi+pi0 eta eta' f0   f0'   f1   f1'  f2  f2' rho+
      data lgp2c/0, +1, +1, +1, +1,  +1,   +1,  +1,  +1, +1, 0,
c      rho0 omega a0+  a00  a1+  a10  a2+ a20 phi   h1    h1' h1c
     &  -1,  -1,  0,   +1,   0,   +1,  0, +1,  -1,  -1,   -1,  -1,
c      b1+ b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon'etab
     &  0,  -1,   +1,   -1, +1,  +1,  +1,   -1,     -1,    -1,  +1,
c     gamma 
     &  -1, 65*0/

c...G parities of mesons
c               pi+-pi0 eta  eta' f0   f0'   f1   f1'   f2 f2' rho+-
      data lgp2g/-1, -1, +1, +1,  +1,  +1,   +1,  +1,  +1, +1,  +1,
c      rho0 omega a0+  a00  a1+   a10   a2+ a20 phi   h1    h1'  h1c
     &  +1,  -1, -1,   -1,  -1,   -1,   -1, -1, -1,  -1,   -1,   -1,
c      b1+   b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon' etab
     &  +1,  +1,   +1,   -1, +1,  +1,  +1,   -1,     -1,     -1,   +1,
     &  66*0/

c...2*J of particls for cluster particls
c               pi+pi0 eta eta' f0   f0' f1   f1'  f2  f2' rho+
      data lgpJ/0 ,0 , 0 , 0 ,  0 ,  0 , 2 ,  2 ,  4 , 4 ,  2,
c      rho0 omega a0+  a00  a1+   a10  a2+ a20 phi h1   h1' h1c
     &  2 ,  2,   0 ,  0 ,  2 ,   2 ,  4 ,  4 ,2,  2,  2,  2,
c      b1+ b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon'etab
     &  2,  2,   0,   2,    0,   2,   4,   2,      2,    2,  0,
c     gamma LMD0 XGM- XGM+ XGM0 XGM*0 XGM*- XGM*+ Xi- Xi0  
     &  2,   1,   1,    1,  1,   3,   3,     3,   1,   1,
c      Delta+ Delta- Delta0 Delta++ K+ K0 K*+ K*0  K1+  K10   K0*+ 
     &  3,      3,     3,     3,    0, 0, 2,   2,  2,   2,   0,
c      K0*0  K1*+  K1*0  K2*+ K2*0 n0   p+ 
     &  0,    2,    2,    4, 4,   1,   1,38*0/
      
c...2*I of particls for cluster particls
c               pi+pi0 eta eta' f0   f0' f1   f1'  f2  f2' rho+
      data lgpI/2 ,2 , 0 , 0 ,  0 ,  0 , 0 ,  0 ,  0 , 0 ,  2,
c      rho0 omega a0+  a00  a1+   a10  a2+ a20 phi h1   h1' h1c
     &  2 ,  0,   2 ,  2 ,  2 ,   2 ,  2 ,  2 ,0,  0,  0,  0,
c      b1+ b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon'etab
     &  2,  2,   0,   0,    0,   0,   0,   0,      0,    0,  0,
c     gamma LMD0 XGM- XGM+ XGM0 XGM*0 XGM*- XGM*+ Xi- Xi0  
     &  0,   0,   2,   2,  2,   2,     2,    2,   1,   1,
c      Delta+ Delta- Delta0 Delta++ K+ K0 K*+ K*0  K1+  K10  K0*+ 
     &  3,      3,     3,     3,    1, 1, 1,   1,  1,   1,   1,
c      K0*0  K1*+  K1*0   K2*+ K2*0 n0   p+ 
     &  1,    1,    1,    1,   1,   1,   1,38*0/

c... ratio of 2*J of particls for cluster particls
c               pi+pi0 eta eta' f0   f0' f1   f1'  f2  f2' rho+
      data pgj/.0 , 1., 1., 1. , 1.,  1., 0.,  0.,  1., 1., 1.,
c      rho0 omega a0+  a00  a1+   a10  a2+ a20 phi h1   h1' h1c
     &  1.,  1.,  1.,  1.,  1.,   0.,  .0, 1.,1., 1.,  1., 1.,
c      b1+ b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon'etab
     & 1., 1.,   1.,   1.,  1.,  0.,  1.,  1.,     1.,  1., 1.,
c     gamma LMD0 XGM- XGM+ XGM0 XGM*0 XGM*- XGM*+ Xi- Xi0  
     &1. , .07,  .15, .15, .15,  .2 ,  .2 , .2 ,   .7, .7,
c      Delta+ Delta- Delta0 Delta++ K+ k0 K*+ K*0  K1+  K10  K0*+ 
     &  .7,    .7,    .7,    .7,   .3, .3,.6, .6,  .7,  .7,  .7,
c      K0*0  K1*+  K1*0   K2*+ K2*0 n0   p+ 
     & .7,   .7,   .7,    0.,  0., .9 , .9 , 38*0/

c... ratio of 2*I of particls for cluster particls
c               pi+pi0 eta eta' f0   f0' f1   f1'  f2  f2' rho+
      data pgi/1. , 1., 1., 1. , 1.,  1., 1.,  1.,  1., 1., 1.,
c      rho0 omega a0+  a00  a1+   a10  a2+ a20 phi h1   h1' h1c
     &  1.,  1.,  1.,  1.,  1.,   1.,  1., 1.,1., 1.,  1., 1.,
c      b1+ b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon'etab
     & 1., 1.,   1.,   1.,  1.,  1.,  1.,  1.,     1.,  1., 1.,
c     gamma LMD0 XGM- XGM+ XGM0 XGM*0 XGM*- XGM*+ Xi- Xi0  
     &.5 ,  1.,  1.,  1. , 1. ,  1.,   1.,   1.,  1. ,1. ,
c      Delta+ Delta- Delta0 Delta++ K+ K0 K*+ K*0  K1+  K10  K0*+ 
     &  1.,    1.,    1.,    1.,  1. ,1. ,1., 1.,  1.,  1.,  1.,
c      K0*0  K1*+  K1*0   K2*+ K2*0 n0   p+ 
     & 1.,   1.,   1.,    1.,  1., .02, .02, 38*0/

*************************************************************************
      kgok=-1
      initg=0
      lastg=0
      if(icg.eq.0) then
        write(*,*) 'icg=0!'
       return
      endif

c... find decay meson C/G parity
      do 10 i=1,npart
        if(abs(k(ip1,2)).eq.lgp1(i)) then
          if(icg.eq.1) initG=lgp2c(i)
          if(icg.eq.2) initG=lgp2g(i)
          goto 20
        endif
10    continue 
20    continue 
      if(initg.eq.0) then    !decay meson no good C/G parity
        kgok=1
        return
      endif 

c...find C/G parity of its decayed products
      if((ip2.eq.0).or.(ip3.eq.0).or.(ip3.lt.ip2)) then
        write(*,*) 'Wrong in CGparity.f (ip3<ip2)!'
        return
      endif  

       istatus=0         !check kind of sum quantum in the products
       orbitj=0.
       orbiti=0.
       ndecy=0

      do 40 ip=ip2,ip3
        ndecy=ndecy+1
        lgp3(ndecy)=0
        if(ndecy.gt.20) then
          write(*,*) 'in CGparity.f:too more decayed products!'
          kgok=1
          return
        endif  
	do 30 i=1,npart
          if(abs(k(ip,2)).eq.lgp1(i)) then
c..C=C'(for normal neutral meson) or 1*(-1)**J(for charge,strang and baryon)
	      if(icg.eq.1) then
	        if(lgp2c(i).eq.0) then
                  lgp3(ndecy)=1
                  orbitj=orbitj+lgpJ(i)
                  if(RLU(0).gt.pgj(i)) orbitj=orbitj+1
                else 
                 lgp3(ndecy)=lgp2c(i)
	        endif 
              endif

c..G=G'(for normal neutral meson) of C'(-1)**I (for strang meson and baryon)
	      if(icg.eq.2) then
               if(lgp2g(i).ne.0) then
		lgp3(ndecy)=lgp2g(i)
		else
		 if(lgp2c(i).ne.0) then
		  lgp3(ndecy)=lgp2c(i)
	         else
		  lgp3(ndecy)=1
                  orbitj=orbitj+lgpJ(i)
	          if(RLU(0).gt.pgj(i)) orbitj=orbitj+1
		 endif 
        	orbiti=orbiti+lgpI(i)
                if(RLU(0).gt.pgi(i)) orbiti=orbiti+1
               endif
	      endif
           goto 32
          endif
30    continue
32    continue
          if(lgp3(ndecy).eq.0) then
                kgok=1
            return
          endif 
40    continue

c...compare G parity between decay meson and its decayed products 
      lastG=1
      do 50 i=1,ndecy
        lastG=lastG*lgp3(i)
50    continue
        if(ngg.eq.2) then 
           orbitj=orbitj+lgpJ(35)
           if(icg.eq.2) then
	    orbiti=orbiti+lgpI(35)
	    if(RLU(0).gt.pgi(35)) orbiti=orbiti+2
           endif
        endif
 
      if(orbitj.ge.2) lastg=lastg*(-1)**(orbitj/2) 
      if(orbiti.ge.2) lastg=lastg*(-1)**(orbiti/2)
      
      if(lastG.eq.initG) then
        kgok=1
      else
        kgok=-1
      endif

      return
      end

*****************************************************************************
      SUBROUTINE psupress(ip1,ip2,ip3,ksok) 
***************************************************************************** 
*   Purpose:(1) Supression particle production(for rho pi puzzle).          *
*    called:  luexec1.f, ludecy1.f                                          *
*   used in:  luexec1.f, ludecy1.f                                          *
*   lgp1(i):  KF of meson which be supressed.                               *
*    pgp(i):  suppress ratio of ith meson.                                  *
*     input:  ip1, decay meson position in the /LUJETS/;                    *
*             ip2-ip3, decayed products position.                           *
*    output:  ksok,1: no supress; -1:upress                                 *
*                                      Chen Jiangchuan  1998.5.31           *
*****************************************************************************
      implicit none
      integer ndim,npart
      parameter (ndim=20,npart=4)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      integer N,K,P,V
      integer lgp1(ndim),lgp2(ndim)
      real pgp(ndim)
      integer ksok,ip,ip1,ip2,ip3,i,istatus
      real  RLU
      save lgp1,pgp
*************************************************************************
c...KF of mesons in the jetset7.4
c               pi+ pi0 eta  eta'  f0    f0'    f1   f1'   f2 f2'  rho+
c     data lgp1/211,111,221, 331, 10221,10331,20223,20333,225,335,213,
c     rho0 omega a0+  a00  a1+   a10   a2+ a20 phi   h1    h1'  h1c
c    &113,223,10211,10111,20213,20113,215,115,333,10223,10333,10443,
c     b1+   b10  etac J/psi xc0  xc1  xc2 psi(2s) upslon upsilon' etab
c    &10213,10113,441, 443,10441,20443,445,30443,  553,  30553, 551,
c     gamma LMD0 XGM- XGM+ XGM0 XGM*0 XGM*- XGM*+ Xi- Xi0  
c    & 22,  3122,3112,3222,3212,3214,3114, 3224,3312,3322,
c      Delta+ Delta- Delta0 Delta++ K+ K0 K*+ K*0  K1+  K10   K0*+ 
c    & 2214,  1114,  2114,  2224, 321,311,323,313,10323,10313,10321,
c      K0*0  K1*+  K1*0   K2*+ K2*0 n0    p+ 
c    &10311,20323,20313, 325, 315,2112,2212,88*0/
c                pi+  rho+ pi0 rho0  
       data lgp1/211, 213, 111, 113, 16*0/ 
c                pi+ rho0 pi0 rho0 
       data lgp2/1,  9,   1,   9, 16*0/ 

c... suppress ratio 
c               pi+ rho0 pi0 rho0
      data  pgp/1., .9, 1.,  .9, 16*0/
***********************************************************************
      ksok=1
      if(k(ip1,2).ne.443.and.k(ip1,2).ne.30443) return
c...find  meson whose decay being supressed  
        istatus=0
      if(ip3-ip2.eq.1) then
      do 40 ip=ip2,ip3
        do 30 i=1,npart
          if(abs(k(ip,2)).eq.lgp1(i)) then
            if(RLU(0).lt.pgp(i)) then
                istatus=istatus+lgp2(i)
                goto 32 
            endif
          endif
30    continue
32    continue
40    continue
      endif
      if(istatus.eq.10) then 
       ksok=-1
      endif 
      return
      end

*******************************************************************************
      SUBROUTINE charm_updat 
***************************************************************************** 
*   Purpose:(1) update some parameters in the jetset7.4                     *
*           (2) modify/reset J/psi & psi(2s) decay channel in the jetset_74 *
*           by databook(98).                                                *
*    called:  lund1.f                                                       *
*   used in:  ludecy1.f                                                     *
*    New added channels are put in the data common/charm/:                  *
*    (1) IDC region of new data: J/psi(1-50), psi(2s)(51-100):              *
*     (mdcy_charm(1)=50 for IDCmax of J/psi, mdcy_charm(2)=100 for psi(2s). *
*    (2)mdme_charm(IDC,1)=1/0 on/off decay channel;                         *
*    (3) brat_charm(IDC) branch ratio of the channel;                       *
*    (4) kfdp_charm(IDC,5)=KF of produced particles(maximum=5 particls).    *
*                                      Chen Jiangchuan  1998.5.11           *
c  Modified:                                                                *
c     (1) update eta_c mass and width;                                      *
c     (2) update f0 mass and width                                          *
c                                      Chen J.C. 2002.07.11
c     (3) update mass,width and Br according PDG2010                        *
*****************************************************************************
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT3/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      common/charm/mdcy_charm(2),mdme_charm(100,2), brat_charm(100),
     &  kfdp_charm(100,5)
      data mdcy_charm,mdme_charm,brat_charm,kfdp_charm/50,100,800*0/
*****************************************************************************
*    Update some parameters in the jetset7.4                                *
*****************************************************************************
c      parj(127)=0.7              ! (D=1.)GeV Mass_min(g)(safe margin)
c      parj(126)=1.               ! (D=2.)GeV Mass_min(gg) in 3-4 Jet
c      parj(32)=parj(127)         ! GeV Mass_min(q)
c       parj(126)=1.               ! (D=2.)GeV Mass_min(gg) (merge g-g to g)
chenjc 2002.06.25 update eta_c mass and width (PDG 2000)
cjc+++++ eta_c
        pmas(lucomp(441),1)=2.980           !eta_c mass(GeV)
        pmas(lucomp(441),2)=0.0286            !etac width (GeV)
        pmas(lucomp(441),3)=0.0500            !maximum deviation(GeV)
c+++++ f_0
         IDC_f0 = lucomp(10221)
        pmas(IDC_f0,1)=0.980
        pmas(IDC_f0,2)=0.080
        pmas(IDC_f0,3)=0.350
c       do idc_f0i = mdcy(IDC_f0,2)+2,mdcy(IDC_f0,2)+mdcy(IDC_f0,3)-1
c                 mdme(idc_f0i,1) = 0          ! switch off f0 -> KK
c        enddo
***********************************************************************
*           J/psi decay channel by databook(98)                       *
***********************************************************************
        brat(721)=0.0594       !J/psi-->e+e-
        mdme(721,2)=3          ! for 1+A*cos^2(theta) angular distribution
        brat(722)=0.0593       !J/psi-->mu+mu-
        mdme(722,2)=3          ! for 1+A*cos^2(theta) angular distribution
c...hadron(bran(qq~))-->qq~(0.17)+rgg/ggg+retac(0.013)+other_news
        brat(723)=1-brat(721)-brat(722)   !J/psi-->hadron     !ran(qq~)=1-(e+e-)-(mu+mu-)
c...adding decay channels(included in brat(J/psi-->hadron)),used in ludecy1.f:
        brat_charm(1)=0.17           !J/psi-->qq~
        mdme_charm(1,1)=mdme(723,1)
        mdme_charm(1,2)=mdme(723,2)
        kfdp_charm(1,1)=kfdp(723,1)
        kfdp_charm(1,2)=kfdp(723,2)
        
cjc        brat_charm(2)=0.64163 !J/psi-->rgg/ggg=brat(723)-qq~-charmonia-news
        brat_charm(2)=0.63732 !J/psi-->rgg/ggg=brat(723)-qq~-charmonia-news
        mdme_charm(2,1)=1
        mdme_charm(2,2)=0
        Kfdp_charm(2,1)=21
        kfdp_charm(2,2)=21
        kfdp_charm(2,3)=21

        brat_charm(3)=0.017          !(charmonia) J/psi-->gamma etac
        mdme_charm(3,1)=1
        mdme_charm(3,2)=0
        kfdp_charm(3,1)=441
        kfdp_charm(3,2)=22

        brat_charm(4)=0.004          !other more J/psi-->rho+ pi-
        mdme_charm(4,1)=1
        mdme_charm(4,2)=0
        kfdp_charm(4,1)=213
        kfdp_charm(4,2)=-211

        brat_charm(5)=0.004          !other more J/psi-->rho- pi+
        mdme_charm(5,1)=1
        mdme_charm(5,2)=0
        kfdp_charm(5,1)=-213
        kfdp_charm(5,2)=211

        brat_charm(6)=0.0045          !other more J/psi-->rho0 pi0
        mdme_charm(6,1)=1
        mdme_charm(6,2)=0
        kfdp_charm(6,1)=113
        kfdp_charm(6,2)=111

        brat_charm(7)=0.0019          !other more J/psi-->a_2- pi+
        mdme_charm(7,1)=1
        mdme_charm(7,2)=0
        kfdp_charm(7,1)=-215
        kfdp_charm(7,2)=211

        brat_charm(8)=0.0019          !other more J/psi-->a_2+ pi-
        mdme_charm(8,1)=1
        mdme_charm(8,2)=0
        kfdp_charm(8,1)=215
        kfdp_charm(8,2)=-211

        brat_charm(9)=0.0019          !other more J/psi-->a_20 pi0 
        mdme_charm(9,1)=1
        mdme_charm(9,2)=0
        kfdp_charm(9,1)=115
        kfdp_charm(9,2)=111

        brat_charm(10)=0.0031          !other more J/psi-->a_2- rho+
        mdme_charm(10,1)=1
        mdme_charm(10,2)=0
        kfdp_charm(10,1)=-215
        kfdp_charm(10,2)=213

        brat_charm(11)=0.0031          !other more J/psi-->a_2+ rho-
        mdme_charm(11,1)=1
        mdme_charm(11,2)=0
        kfdp_charm(11,1)=215
        kfdp_charm(11,2)=-213

        brat_charm(12)=0.0032          !other more J/psi-->a_20 rho0 
        mdme_charm(12,1)=1
        mdme_charm(12,2)=0
        kfdp_charm(12,1)=115
        kfdp_charm(12,2)=113

        brat_charm(13)=0.0013          !other more J/psi-->omega eta 
        mdme_charm(13,1)=1
        mdme_charm(13,2)=0
        kfdp_charm(13,1)=223
        kfdp_charm(13,2)=221

        brat_charm(14)=0.0032          !other more J/psi-->omega f_2 
        mdme_charm(14,1)=1
        mdme_charm(14,2)=0
        kfdp_charm(14,1)=223
        kfdp_charm(14,2)=225

        brat_charm(15)=0.0009          !other more J/psi-->pp~ omega 
        mdme_charm(15,1)=1
        mdme_charm(15,2)=0
        kfdp_charm(15,1)=223
        kfdp_charm(15,2)=2212
        kfdp_charm(15,3)=-2212

        brat_charm(16)=0.0005          !other more J/psi-->b_1- pi+
        mdme_charm(16,1)=1
        mdme_charm(16,2)=0
        kfdp_charm(16,1)=-10213
        kfdp_charm(16,2)=211

        brat_charm(17)=0.0005          !other more J/psi-->b_1+ pi-
        mdme_charm(17,1)=1
        mdme_charm(17,2)=0
        kfdp_charm(17,1)=10213
        kfdp_charm(17,2)=-211

         brat_charm(18)=0.0008          !other more J/psi-->b_10 pi0 
         mdme_charm(18,1)=1
         mdme_charm(18,2)=0
         kfdp_charm(18,1)=10113
         kfdp_charm(18,2)=111

        brat_charm(19)=0.0024          !other more J/psi-->K+K*-~ 
        mdme_charm(19,1)=1
        mdme_charm(19,2)=0
        kfdp_charm(19,1)=321
        kfdp_charm(19,2)=-323

        brat_charm(20)=0.0024          !other more J/psi-->K-K*+ 
        mdme_charm(20,1)=1
        mdme_charm(20,2)=0
        kfdp_charm(20,1)=-321
        kfdp_charm(20,2)=323

        brat_charm(21)=0.0018          !other more J/psi-->K0K*0 
        mdme_charm(21,1)=1
        mdme_charm(21,2)=0
        kfdp_charm(21,1)=311
        kfdp_charm(21,2)=-313

        brat_charm(22)=0.0018          !other more J/psi-->K0K*0 
        mdme_charm(22,1)=1
        mdme_charm(22,2)=0
        kfdp_charm(22,1)=-311
        kfdp_charm(22,2)=313

        brat_charm(23)=0.0028          !other more J/psi-->K*+K*2-~ 
        mdme_charm(23,1)=1
        mdme_charm(23,2)=0
        kfdp_charm(23,1)=323
        kfdp_charm(23,2)=-325

        brat_charm(24)=0.0028          !other more J/psi-->K*-K*2+ 
        mdme_charm(24,1)=1
        mdme_charm(24,2)=0
        kfdp_charm(24,1)=-323
        kfdp_charm(24,2)=325

        brat_charm(25)=0.0028          !other more J/psi-->K*0K2*0-~ 
        mdme_charm(25,1)=1
        mdme_charm(25,2)=0
        kfdp_charm(25,1)=313
        kfdp_charm(25,2)=-315

        brat_charm(26)=0.0028          !other more J/psi-->K*0~K2*0+ 
        mdme_charm(26,1)=1
        mdme_charm(26,2)=0
        kfdp_charm(26,1)=-313
        kfdp_charm(26,2)=315

        brat_charm(27)=0.00053          !other more J/psi-->omega f_2' 
        mdme_charm(27,1)=1
        mdme_charm(27,2)=0
        kfdp_charm(27,1)=333
        kfdp_charm(27,2)=335

        brat_charm(28)=0.00114          !other more J/psi-->phi K+K- 
        mdme_charm(28,1)=1
        mdme_charm(28,2)=0                
        kfdp_charm(28,1)=333
        kfdp_charm(28,2)=321
        kfdp_charm(28,3)=-321

        brat_charm(29)=0.00431          !gamma eta' 
        mdme_charm(29,1)=1
        mdme_charm(29,2)=0                
        kfdp_charm(29,1)=331
        kfdp_charm(29,2)=22

************************************************************************
*            psi(2s) decay channel by databook(98)                     *
************************************************************************
c        brat(968)=0.0085       !psi(2s)-->e+e-
c        brat(969)=0.0077       !psi(2s)-->mu+mu-
cc...ran(qq~)-->qq~(0.029)+rgg/ggg
c        brat(970)=0.21403      !psi(2s)-->ran(qq~) !1-e-mu-(J/psi)-(rad)
c        brat(971)=0.302        !psi(2s)-->J/pi pi+pi-
c        brat(972)=0.179        !psi(2s)-->J/pi pi0pi0
c        brat(973)=0.027        !psi(2s)-->J/pi eta
c        brat(974)=0.00097      !psi(2s)-->J/pi pi0
c        brat(975)=0.093        !psi(2s)-->xc0 gamma
c        brat(976)=0.087        !psi(2s)-->xc1 gamma
c        brat(977)=0.078        !psi(2s)-->xc2 gamma
c        brat(978)=0.0028       !psi(2s)-->etac gamma

cCJC/98... on/off(1/0) for Charm-onia decay channel
cc        MDME(971,1)=0            !psi(2s)-->J/pi pi+pi-
cC        MDME(972,1)=0            !psi(2s)-->J/pi pi0pi0
cc        MDME(973,1)=0            !psi(2s)-->J/pi eta
cc        MDME(974,1)=0            !psi(2s)-->J/pi pi0
cc        MDME(975,1)=0            !psi(2s)-->xc0 gamma
cc        MDME(976,1)=0            !psi(2s)-->xc1 gamma
cc        MDME(977,1)=0            !psi(2s)-->xc2 gamma
cc        MDME(978,1)=0            !psi(2s)-->etac gamma

cc... adding decay channels(included in brat(psi-->rad(qq~)), used in ludecy1.f:
c        brat_charm(51)=0.029           !psi(2s)-->qq~
c        mdme_charm(51,1)=mdme(970,1)
c        mdme_charm(51,2)=mdme(970,2)
c        kfdp_charm(51,1)=kfdp(970,1)
c        kfdp_charm(51,2)=kfdp(970,2)

c        brat_charm(52)=0.17188 !psi(2s)-->rgg/ggg=brat(970)-brat(51)-53-54
c        mdme_charm(52,1)=1
c        mdme_charm(52,2)=0
c        kfdp_charm(52,1)=21
c        kfdp_charm(52,2)=21
c        kfdp_charm(52,3)=21
c
c        brat_charm(53)=0.00315 !psi(2s)-->tau+tau-=0.3885*(Bee+Buu)/2
c        mdme_charm(53,1)=1
c        mdme_charm(53,2)=0
c        kfdp_charm(53,1)=15
c        kfdp_charm(53,2)=-15

c        brat_charm(54)=0.01    !psi(2s)-->J/psi+mu+mu-
c        mdme_charm(54,1)=1
c        mdme_charm(54,2)=0
c        kfdp_charm(54,1)=443
c        kfdp_charm(54,2)=13
c        kfdp_charm(54,3)=-13

************************************************************************
*            psi(2s) decay channel by databook(96)&(98)                *
************************************************************************
        brat(968)=0.00772       !psi(2s)-->e+e- (PDG(98))
        brat(969)=0.0077       !psi(2s)-->mu+mu-
c...ran(qq~)-->qq~(0.029)+rgg/ggg
        brat(970)=0.18703      !psi(2s)-->ran(qq~)   ! 1-(e)-(mu)-(J/psi)-(rad)
        brat(971)=0.336        !psi(2s)-->J/pi pi+pi-   (PDG(96))
        brat(972)=0.1773       !psi(2s)-->J/pi pi0pi0   (PDG(96))
        brat(973)=0.0328       !psi(2s)-->J/pi eta
        brat(974)=0.0013       !psi(2s)-->J/pi pi0
        brat(975)=0.0962       !psi(2s)-->xc0 gamma
        brat(976)=0.092        !psi(2s)-->xc1 gamma
        brat(977)=0.0874       !psi(2s)-->xc2 gamma
        brat(978)=0.0034       !psi(2s)-->etac gamma

c... adding decay channels(included in brat(psi-->rad(qq~)), used in ludecy1.f:
        brat_charm(51)=0.029           !psi(2s)-->qq~
        mdme_charm(51,1)=mdme(970,1)
        mdme_charm(51,2)=mdme(970,2)
        kfdp_charm(51,1)=kfdp(970,1)
        kfdp_charm(51,2)=kfdp(970,2)

c        brat_charm(52)=0.14488 !psi(2s)-->rgg/ggg=brat(970)-brat(51)-53-54
        brat_charm(52)=0.15488 !psi(2s)-->rgg/ggg=brat(970)-brat(51)-53-54
        mdme_charm(52,1)=1
        mdme_charm(52,2)=0
        kfdp_charm(52,1)=21
        kfdp_charm(52,2)=21
        kfdp_charm(52,3)=21

        brat_charm(53)=0.00315  !psi(2s)-->tau+tau-=0.3885*(Bee+Buu)/2 (PDG(98))
        mdme_charm(53,1)=1
        mdme_charm(53,2)=0
        kfdp_charm(53,1)=15
        kfdp_charm(53,2)=-15

c        brat_charm(54)=0.01    !psi(2s)-->J/psi+mu+mu- (PDG(98))
c        mdme_charm(54,1)=1 
c        mdme_charm(54,2)=0 
c        kfdp_charm(54,1)=443
c        kfdp_charm(54,2)=13
c        kfdp_charm(54,3)=-13
**************************************************************************
* Update mass of e,mu,tau,pi0,pi+,eta,Ks,KL,K0,K+,n,p
**************************************************************************
              pmas(lucomp(11),1)=0.000510999
              pmas(lucomp(13),1)=0.1056584
              pmas(lucomp(15),1)=1.7770
              pmas(lucomp(111),1)=0.134976
              pmas(lucomp(211),1)=0.139570
              pmas(lucomp(221),1)=0.54784
              pmas(lucomp(310),1)=0.49767
              pmas(lucomp(130),1)=0.49767
              pmas(lucomp(311),1)=0.49767
              pmas(lucomp(321),1)=0.49368
              pmas(lucomp(2112),1)=0.9395656
              pmas(lucomp(2212),1)=0.9382723
              pmas(lucomp(113),1)=0.77549
              pmas(lucomp(213),1)=0.77549

c        write(*,*) 'rho0(113): KC, mass,width,maxdeviation,life(mm)',
c     >  lucomp(113),pmas(lucomp(113),1),pmas(lucomp(113),2),
c     >   pmas(lucomp(113),3),pmas(lucomp(113),4)
c        write(*,*) 'rho+(213): KC, mass,width,maxdeviation,life(mm)',
c     >  lucomp(213),pmas(lucomp(213),1),pmas(lucomp(213),2),
c     >   pmas(lucomp(213),3),pmas(lucomp(213),4)


      RETURN 
      END


C********************************************************************* 
      SUBROUTINE LUONIA1(ipp,KFL,ngg) 
*****************************************************************************
*     Modified from LUONIA.f in the jeset7.4                                * 
*     onia-->rgg/ggg in CM system and Lorentz boost to Exp. system          *
*      ipp: onia position in /LUJETS/                                       *
*      KFL: KF of onia                                                      *
*      ngg: 1-decay to ggg, 2-decay to rgg                                  *
*                                 Chen Jiangchuan 1998.5.31                 *
*****************************************************************************
C...Purpose: to generate onium decays into three 
C...gluons or two gluons and a photon. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT1/,/LUDAT2/ 
      DIMENSION PV(5)
      DOUBLE PRECISION dps(5)

c.... parent particle(onium) parameters
      do i=1,5
        pv(i)=p(ipp,i)
        DPS(i)=0D0
      enddo
c...decay in the CM system of onium  now
      Ecm=p(ipp,5)    ! Ecm=mass_onia

C...Printout. Check input parameters. 
cjc      IF(MSTU(12).GE.1) CALL lulist(1) 
      IF(KFL.LT.0.OR.KFL.GT.8) THEN 
        CALL LUERRM(16,'(LUONIA:) called with unknown flavour code') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(ECM.LT.PARJ(127)+2.02*PARF(101)) THEN 
        CALL LUERRM(16,'(LUONIA:) called with too small CM energy') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Initial e+e- and onium state (optional). 
      NC=N 
      KFLC=IABS(KFL) 
 
C...Choose x1 and x2 according to matrix element. 
      Irgg=0       ! for keeping input B(rgg)/B(ggg)
      NTRY=0 
  100 X1=RLU(0) 
      X2=RLU(0) 
      X3=2.-X1-X2 
      IF(X3.GE.1..OR.((1.-X1)/(X2*X3))**2+((1.-X2)/(X1*X3))**2+ 
     &((1.-X3)/(X1*X2))**2.LE.2.*RLU(0)) GOTO 100 
      NTRY=NTRY+1 
      NJET=3 
      IF(MSTJ(101).LE.4) CALL LU3ENT(NC+1,21,21,21,ECM,X1,X3) 
      IF(MSTJ(101).GE.5) CALL LU3ENT(-(NC+1),21,21,21,ECM,X1,X3) 
 
C...Photon-gluon-gluon events. Small system modifications. Jet origin. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      QF=0. 
      IF(KFLC.NE.0) QF=KCHG(KFLC,1)/3. 
      RGAM=7.2*QF**2*PARU(101)/ULALPS(ECM**2) 
cjc... correction for b(rgg)/b(ggg)=36/5*eq**2*aem/as(Q)*(1+2.2*as(q)/pi)
      rgam=rgam*(1.+2.2*ULALPS(ECM**2)/3.14159265)   
      MK=0 
      ECMC=ECM 
      ngg=0
      IF((RLU(0).GT.RGAM/(1.+RGAM)).and.(irgg.eq.0)) THEN  !parj(126)=mass_min(gg) (d=2GeV)
cjc      IF(RLU(0).GT.RGAM/(1.+RGAM)) THEN  !parj(126)=mass_min(gg) (d=2GeV)
        IF((1.-MAX(X1,X2,X3)).LE.MAX((PARJ(126)/ECM)**2,PARJ(125))) 
     &  NJET=2 
        IF(NJET.EQ.2.AND.MSTJ(101).LE.4) CALL LU2ENT(NC+1,21,21,ECM) 
        IF(NJET.EQ.2.AND.MSTJ(101).GE.5) CALL LU2ENT(-(NC+1),21,21,ECM) 
        ngg=1       ! decay to ggg
      ELSE 
        MK=1 
        ECMC=SQRT(1.-X1)*ECM 

cjc       IF(ECMC.LT.2.*PARJ(127))  GOTO 100       !2mg>2*parj(127)
       IF(ECMC.LT.2.*PARJ(127)) then
         irgg=1
         GOTO 100       !2mg>2*parj(127)
       endif 

        ngg=2

        K(NC+1,1)=1 
        K(NC+1,2)=22 
        K(NC+1,4)=0 
        K(NC+1,5)=0 
        IF(MSTJ(101).GE.5) K(NC+2,4)=MSTU(5)*(NC+3) 
        IF(MSTJ(101).GE.5) K(NC+2,5)=MSTU(5)*(NC+3) 
        IF(MSTJ(101).GE.5) K(NC+3,4)=MSTU(5)*(NC+2) 
        IF(MSTJ(101).GE.5) K(NC+3,5)=MSTU(5)*(NC+2) 
        NJET=2 
cjc/98.... cancel gg->phsspace
c        IF(ECMC.LT.4.*PARJ(127)) THEN   !parj(127)=mass_min(g)(d=1.0 GeV)
c          MSTU10=MSTU(10)                !phase_space (decay to qq~)
c          MSTU(10)=1 
c          P(NC+2,5)=ECMC 
c          CALL LU1ENT(NC+2,83,0.5*(X2+X3)*ECM,PARU(1),0.) 
c          MSTU(10)=MSTU10 
c          NJET=0 
c        ENDIF 
cjc/98..... 
      ENDIF 
      DO 110 IP=NC+1,N 
      K(IP,3)=IPP+(MSTJ(115)/2)+(KFLC/5)*(MSTJ(115)/3)*(NC-1) 
  110 CONTINUE 
 
C...Differential cross-sections. Upper limit for cross-section. 
      IF(MSTJ(106).EQ.1) THEN 
        SQ2=SQRT(2.) 
        HF1=1.-PARJ(131)*PARJ(132) 
        HF3=PARJ(133)**2 
        CT13=(X1*X3-2.*X1-2.*X3+2.)/(X1*X3) 
cjc...keep st13=/=0.
        if(abs(ct13).ge.1.) goto 100

        ST13=SQRT(1.-CT13**2) 
        SIGL=0.5*X3**2*((1.-X2)**2+(1.-X3)**2)*ST13**2 
        SIGU=(X1*(1.-X1))**2+(X2*(1.-X2))**2+(X3*(1.-X3))**2-SIGL 
        SIGT=0.5*SIGL 
cjc...keep st13=/=0.
	if(abs(st13).lt.0.000001) goto 100
        SIGI=(SIGL*CT13/ST13+0.5*X1*X3*(1.-X2)**2*ST13)/SQ2 
        SIGMAX=(2.*HF1+HF3)*ABS(SIGU)+2.*(HF1+HF3)*ABS(SIGL)+2.*(HF1+ 
     &  2.*HF3)*ABS(SIGT)+2.*SQ2*(HF1+2.*HF3)*ABS(SIGI) 
 
C...Angular orientation of event. 
  120   CHI=PARU(2)*RLU(0) 
        CTHE=2.*RLU(0)-1. 
        PHI=PARU(2)*RLU(0) 
        CCHI=COS(CHI) 
        SCHI=SIN(CHI) 
        C2CHI=COS(2.*CHI) 
        S2CHI=SIN(2.*CHI) 
        THE=ACOS(CTHE) 
        STHE=SIN(THE) 
        C2PHI=COS(2.*(PHI-PARJ(134))) 
        S2PHI=SIN(2.*(PHI-PARJ(134))) 
        SIG=((1.+CTHE**2)*HF1+STHE**2*C2PHI*HF3)*SIGU+2.*(STHE**2*HF1- 
     &  STHE**2*C2PHI*HF3)*SIGL+2.*(STHE**2*C2CHI*HF1+((1.+CTHE**2)* 
     &  C2CHI*C2PHI-2.*CTHE*S2CHI*S2PHI)*HF3)*SIGT-2.*SQ2*(2.*STHE*CTHE* 
     &  CCHI*HF1-2.*STHE*(CTHE*CCHI*C2PHI-SCHI*S2PHI)*HF3)*SIGI 
        IF(SIG.LT.SIGMAX*RLU(0)) GOTO 120 
        CALL LUDBRB(NC+1,N,0.,CHI,0D0,0D0,0D0) 
        CALL LUDBRB(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 

cjc..Lorentz transform decay products(ggg) from onium-Ecm system to lab system 
      dtmp=0.0000001
      if((abs(pv(1)).gt.dtmp).and.(abs(pv(2)).gt.dtmp).and.
     & (abs(pv(3)).gt.dtmp)) then
      DO 130 J=1,5
        DPS(J)=DPS(J)+pv(J)
 130  CONTINUE
      DPS(4)=SQRT(DBLE(Pv(1))**2+DBLE(pv(2))**2+
     &  DBLE(Pv(3))**2+DBLE(Pv(5))**2)
      CALL LUDBRB(NC+1,N,0.,0.,DPS(1)/DPS(4),
     &  DPS(2)/DPS(4),DPS(3)/DPS(4))
      endif

cjc... take onia's vertex position as ggg's vertex position  
      do 150 i=nc+1,n
        do 140 j=1,5
          v(i,j)=v(ipp,j)
140    continue          
150    continue          
      
C...Generate parton shower. Rearrange along strings and check. 
      IF(MSTJ(101).GE.5.AND.NJET.GE.2) THEN 
        CALL LUSHOW(NC+MK+1,-NJET,ECMC) 
       MSTJ14=MSTJ(14) 
        IF(MSTJ(105).EQ.-1) MSTJ(14)=-1 
        IF(MSTJ(105).GE.0) MSTU(28)=0 
        CALL LUPREP(0) 
       MSTJ(14)=MSTJ14 
        IF(MSTJ(105).GE.0.AND.MSTU(28).NE.0) GOTO 100 
      ENDIF 
 
      RETURN 
      END 
C********************************************************************* 
 
      SUBROUTINE LURADK1(ECM,MK,PAK,THEK,PHIK,ALPK) 
 
C...Purpose: to generate initial state photon radiation. 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT1/ 
 
C...Function: cumulative hard photon spectrum in QFD case. 
      FXK(XX)=2.*LOG(XX)+PARJ(161)*LOG(1.-XX)+PARJ(162)*XX+ 
     &PARJ(163)*LOG((XX-SZM)**2+SZW**2)+PARJ(164)*ATAN((XX-SZM)/SZW) 
 
C...Determine whether radiative photon or not. 
      MK=0 
      PAK=0. 
      IF(PARJ(160).LT.RLU(0)) RETURN 
      MK=1 
 
C...Photon energy range. Find photon momentum in QED case. 
      XKL=PARJ(135) 
      XKU=MIN(PARJ(136),1.-(2.*PARJ(127)/ECM)**2) 
      IF(MSTJ(102).LE.1) THEN 
  100   XK=1./(1.+(1./XKL-1.)*((1./XKU-1.)/(1./XKL-1.))**RLU(0)) 
        IF(1.+(1.-XK)**2.LT.2.*RLU(0)) GOTO 100 
 
C...Ditto in QFD case, by numerical inversion of integrated spectrum. 
      ELSE 
        SZM=1.-(PARJ(123)/ECM)**2 
        SZW=PARJ(123)*PARJ(124)/ECM**2 
        FXKL=FXK(XKL) 
        FXKU=FXK(XKU) 
        FXKD=1E-4*(FXKU-FXKL) 
        FXKR=FXKL+RLU(0)*(FXKU-FXKL) 
        NXK=0 
  110   NXK=NXK+1 
        XK=0.5*(XKL+XKU) 
        FXKV=FXK(XK) 
        IF(FXKV.GT.FXKR) THEN 
          XKU=XK 
          FXKU=FXKV 
        ELSE 
          XKL=XK 
          FXKL=FXKV 
        ENDIF 
C === Chenjc 07/23/2001 for protection the case of '(FXKU-FXKL)=0'
        if(abs(FXKU-FXKL).le.1.e-6) then
         mk=0 
         return 
        endif 
C----Chenjc 

        IF(NXK.LT.15.AND.FXKU-FXKL.GT.FXKD) GOTO 110 
        XK=XKL+(XKU-XKL)*(FXKR-FXKL)/(FXKU-FXKL) 
      ENDIF 
      PAK=0.5*ECM*XK 

C...Photon polar and azimuthal angle. 
      PME=2.*(ULMASS(11)/ECM)**2 
  120 CTHM=PME*(2./PME)**RLU(0) 
      IF(1.-(XK**2*CTHM*(1.-0.5*CTHM)+2.*(1.-XK)*PME/MAX(PME, 
     &CTHM*(1.-0.5*CTHM)))/(1.+(1.-XK)**2).LT.RLU(0)) GOTO 120 
      CTHE=1.-CTHM 
      IF(RLU(0).GT.0.5) CTHE=-CTHE 
      STHE=SQRT(MAX(0.,(CTHM-PME)*(2.-CTHM))) 
      THEK=ULANGL(CTHE,STHE) 
      PHIK=PARU(2)*RLU(0) 
 
C...Rotation angle for hadronic system. 
      SGN=1. 
      IF(0.5*(2.-XK*(1.-CTHE))**2/((2.-XK)**2+(XK*CTHE)**2).GT. 
     &RLU(0)) SGN=-1. 
      ALPK=ASIN(SGN*STHE*(XK-SGN*(2.*SQRT(1.-XK)-2.+XK)*CTHE)/ 
     &(2.-XK*(1.-SGN*CTHE))) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUTAB1(MTABU) 
 
C...Purpose: to evaluate various properties of an event, with 
C...statistics accumulated during the course of the run and 
C...printed at the end. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT2/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT3/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT1/,/LUDAT2/,/LUDAT3/ 
      DIMENSION KFDM(8),KFDC(2000,0:8),NPDC(2000),kftemp(8)
      SAVE NEVDC,NKFDC,NREDC,KFDC,NPDC 
      CHARACTER CHAU*16,CHDC(8)*12 
      dimension makeline(3)
      DATA NEVDC/0/,NKFDC/0/,NREDC/0/ 

C...Reset statistics on decay channels. 
      IF(MTABU.EQ.50.or.MTABU.EQ.60) THEN 
        NEVDC=0 
        NKFDC=0 
        NREDC=0 
 
C...Identify and order flavour content of final state. 
      ELSEIF(MTABU.EQ.51.or.MTABU.EQ.61) THEN 
        NEVDC=NEVDC+1 
        NDS=0 
cjc...check if particle produced by 2nd-string or onia-direct        
        makeline(1)=1
        makeline(2)=1
        mli=0
        DO 645 I=1,N 
          IF((K(I,2).EQ.92).or.(K(I,2).EQ.91)) THEN 
            mli=mli+1
            if(mli.gt.2) then
         write(*,*) 'mli=',mli,'more than 2 cluster(lutab2.f)'
               mli=2
            endif
          makeline(mli)=i              !(g,q)string input
        endif
645   continue
        DO 670 I=1,N 
      IF(MTABU.EQ.51) THEN 
c... pi0-->2gamma
        if(k(i,2).eq.111) then
         else
           if(K(K(I,3),2).eq.111) goto 670
          IF(K(I,1).LE.0.OR.K(I,1).GE.6) GOTO 670    ! final undecyed particles
        endif
c----
      elseif(MTABU.EQ.61) THEN
c        IF(K(I,3).ne.1) goto 670  !1st partile decay
        if(k(i,2).eq.21.or.k(i,3).le.0) goto 670
       IF(K(I,3).ne.1.and.K(I,3).NE.makeline(1).and.
     &K(I,3).NE.makeline(2))goto 670 !1st partile decay
      endif
        NDS=NDS+1 
        IF(NDS.GT.8) THEN 
          NREDC=NREDC+1 
          RETURN 
        ENDIF 
        KFM=2*IABS(K(I,2)) 
        IF(K(I,2).LT.0) KFM=KFM-1 
        DO 650 IDS=NDS-1,1,-1 
        IIN=IDS+1 
        IF(KFM.LT.KFDM(IDS)) GOTO 660 
        KFDM(IDS+1)=KFDM(IDS) 
  650   CONTINUE 
        IIN=1 
  660   KFDM(IIN)=KFM 
        kftemp(nds)=K(I,2)
  670   CONTINUE 
cjc/check final state
c        isi=0        
c        if(nds.eq.2) then
c          do itemp=1,nds
cc            if(abs(kftemp(itemp)).eq.213.and.abs(kftemp(itemp)).ne.213.) isi=1
c           if(abs(kftemp(1)).eq.213.and.abs(kftemp(2)).eq.215) isi=1
c           if(abs(kftemp(1)).eq.215.and.abs(kftemp(2)).eq.213) isi=1
c          enddo  
c          if(isi.eq.1) then
c            call lulist(1)
c          endif
c         endif
cjc/check   
C...Find whether old or new final state. 
        DO 690 IDC=1,NKFDC 
        IF(NDS.LT.KFDC(IDC,0)) THEN 
          IKFDC=IDC 
          GOTO 700 
        ELSEIF(NDS.EQ.KFDC(IDC,0)) THEN 
          DO 680 I=1,NDS 
          IF(KFDM(I).LT.KFDC(IDC,I)) THEN 
            IKFDC=IDC 
            GOTO 700 
          ELSEIF(KFDM(I).GT.KFDC(IDC,I)) THEN 
            GOTO 690 
          ENDIF 
  680     CONTINUE 
          IKFDC=-IDC 
          GOTO 700 
        ENDIF 
  690   CONTINUE 
        IKFDC=NKFDC+1 
  700   IF(IKFDC.LT.0) THEN 
          IKFDC=-IKFDC 
        ELSEIF(NKFDC.GE.2000) THEN 
          NREDC=NREDC+1 
          RETURN 
        ELSE 
          DO 720 IDC=NKFDC,IKFDC,-1 
          NPDC(IDC+1)=NPDC(IDC) 
          DO 710 I=0,8 
          KFDC(IDC+1,I)=KFDC(IDC,I) 
  710     CONTINUE 
  720     CONTINUE 
          NKFDC=NKFDC+1 
          KFDC(IKFDC,0)=NDS 
          DO 730 I=1,NDS 
          KFDC(IKFDC,I)=KFDM(I) 
  730     CONTINUE 
          NPDC(IKFDC)=0 
        ENDIF 
        NPDC(IKFDC)=NPDC(IKFDC)+1 
 
C...Write statistics on decay channels. 
      ELSEIF(MTABU.EQ.52.or.MTABU.EQ.62) THEN 
        FAC=1./MAX(1,NEVDC) 
        WRITE(MSTU(11),5900) NEVDC 
cjc        WRITE(3,5900) NEVDC 
        DO 750 IDC=1,NKFDC 
        DO 740 I=1,KFDC(IDC,0) 
        KFM=KFDC(IDC,I) 
        KF=(KFM+1)/2 
        IF(2*KF.NE.KFM) KF=-KF 
        CALL LUNAME(KF,CHAU) 
        CHDC(I)=CHAU(1:12) 
        IF(CHAU(13:13).NE.' ') CHDC(I)(12:12)='?' 
  740   CONTINUE 
        WRITE(MSTU(11),6000) FAC*NPDC(IDC),(CHDC(I),I=1,KFDC(IDC,0)) 
cjc        WRITE(3,6000) FAC*NPDC(IDC),(CHDC(I),I=1,KFDC(IDC,0)) 
  750   CONTINUE 
        IF(NREDC.NE.0) WRITE(MSTU(11),6100) FAC*NREDC 
cjc        IF(NREDC.NE.0) WRITE(3,6100) FAC*NREDC 
      endif
 
C...Format statements for output on unit MSTU(11) (default 6). 
 5900 FORMAT(///20X,'Decay channel analysis - final state'/ 
     &  20X,'based on an analysis of ',I10,' events'// 
     &2X,'Probability',10X,'Complete final state'/) 
cjc 6000 FORMAT(2X,F9.5,5X,8(A12,1X)) 
 6000 FORMAT(1X,F7.5,1X,8(A9,1X)) 
 6100 FORMAT(2X,F9.5,5X,'into other channels (more than 8 particles ', 
     &'or table overflow)') 
 
      RETURN 
      END 
 
c      PROGRAM EICHTEN
      subroutine EICHTEN(E,Rdchannel)
      IMPLICIT NONE
C------------------------------------------------------------------------
C  FCALC.FORTRAN -- Original Eichten Model calculations, obtained from
C                   Alan Weinstein on SLACVM, June 15, 1995.
C
C  950720  Michael Kelsey -- Converted to AIX 3.2.5
C          1)  Explicit PROGRAM statement included
C          2)  All HANDYPAK functions converted to HBOOK
C          3)  Clean up code by removing all commented out WRITE/FORMATs
C          4)  Remove extra "///" delimiters in FORMAT statements
C          5)  Use explicit Fortran OPEN statements for data files
C
C  950721  Major structural revisions
C          1)  All subroutines and functions extracted to separate files
C          2)  All common blocks extracted into INCLUDE files
C          3)  Added IMPLICIT NONE and declared all variables explicitly
C
C  950722  4)  Extracted file-reading to separate routines
C          5)  Consolidated local variables, reduced unneeded duplicates
C          6)  Extracted all calculational work to new FCALC.F
C
C  950725  Move data logging actions to FCALC.F
C  950803  Support command-line argument of energy to compute
C  950926  Add printout of sigma(mu mu) at input energy
C
C  990518  Modified for BES by Chen Jiangchuan
C          1) Make it as a subroutine
C          2) Input Ecm energy E
c          3) Ouput fraction of D mesons by R unit(Sig(nb)/Sigma(mu mu)),
C              Rdchannel(J) J=1-9 
C        1     2      3     4     5     6     7       8        9      10 
C      D0~D0 D0D*0 D*0~D*0 D+D- D+D*- D*+D*- Ds+Ds- Ds+Ds*-  Ds*+D*- R-charmtot
C------------------------------------------------------------------------
      INCLUDE 'fist.inc'
      INCLUDE 'gen.inc'

C  Useful parameters
      DOUBLE PRECISION PI, ALPHA2, HCSQ
      PARAMETER (PI = 3.1415926535)
      PARAMETER (ALPHA2 = 1D0/137.0359895**2)
      PARAMETER (HCSQ = 0.38937966D+6)             !Gev^2 nb
      real Rdchannel(10)
C  External Functions
      REAL RCHANNEL, RCHARM
      EXTERNAL RCHANNEL, RCHARM

C  Local Variables
      DOUBLE PRECISION EMIN,ESHIFT,ESTEP,WE(500),R(500)
      INTEGER J,NSTEPS
      REAL E,ANS(FIST_NFS,500),SIGFAC
      CHARACTER*80 CMDARG


C  Load data from files into common blocks
        call loaddata
        CALL LOADGEN
        CALL LOADMIX
        CALL LOADFIST

C  Check for command line argument (single calculation)
        CALL GETARG(1,CMDARG)
cjc        IF (CMDARG.NE.' ') THEN
cjc          READ(CMDARG,*) E              ! Get energy from command line
        if(E.gt.0.) then              

          SIGFAC = HCSQ * 4.*PI*ALPHA2/(3.*E**2)    !Convert R to sigma

          WRITE(6,100)'0 D-D','D-D*','D*-D*','+ D-D','D-D*','D*-D*'
     1               ,'F-F','F-F*','F*-F*','RTOT'
          WRITE(6,110) E, (RCHANNEL(E,REAL(J)),J=1,9), RCHARM(E)
          WRITE(6,120) (SIGFAC*RCHANNEL(E,REAL(J)),J=1,9),
     1                 SIGFAC*RCHARM(E)
          WRITE(6,130) E, SIGFAC

 100      FORMAT('ENERGY  ',9A7,1X,A7)
 110      FORMAT(F6.3,2X,9F7.4,1X,F7.4)
 120      FORMAT('Sig(nb)=',9F7.4,1X,F7.4)
 130      FORMAT('Sigma(mu mu) at',F6.3,' =',F6.3)
cjc -----output fraction of D/D*/Ds pairs by R unit(sigma(nb)/sigma(mu mu))
          do j=1,9
            rdchannel(j)=RCHANNEL(E,REAL(J))
          enddo
          rdchannel(10)=RCHARM(E)
          return
        else
          write(6,*)  'E<0 in the eichten_sub! E=',E
        ENDIF

C  Energy steps for actual calculations
        NSTEPS=380
        ESTEP=2.0D-3
        EMIN=2.*MASS(1,1)
        ESHIFT=0.0              !*** ESHIFT=1.80D-1

C  Compute charm production physics (rates and phases)
        CALL FCALC(NSTEPS,EMIN+ESHIFT,ESTEP,WE,R,ANS)

C  Write results to HBOOK histograms
cjc        CALL PLOTS(NSTEPS,EMIN+ESHIFT,ESTEP,WE,R,ANS)
      return
      END
      DOUBLE PRECISION FUNCTION EIGEN(X,I)
      IMPLICIT NONE
      DOUBLE PRECISION X
      INTEGER I

C  Local Variables
      DOUBLE PRECISION EGY(16,5),EI(5),EGY0(16),EO
      INTEGER J


C  Initialization
      EGY(1,1)=1.39787713D+0
      EGY(2,1)=3.47508762D+0
      EGY(3,1)=5.03291532D+0
      EGY(4,1)=6.37015002D+0
      EGY(5,1)=7.57493350D+0
      EGY(6,1)=8.68791542D+0
      EGY(7,1)=9.73197481D+0
      EGY(8,1)=1.07215739D+1
      EGY(9,1)=1.16666312D+1
      EGY(10,1)=2.82564665D+0
      EGY(11,1)=4.46186360D+0
      EGY(12,1)=5.84763424D+0
      EGY(13,1)=3.85058001D+0
      EGY(14,1)=5.29298414D+0
      EGY(15,1)=6.57158774D+0
      EGY(16,1)=7.74061620D+0
      EGY(1,2)=1.94848983D-1
      EGY(2,2)=2.80250670D+0
      EGY(3,2)=4.51799895D+0
      EGY(4,2)=5.93770192D+0
      EGY(5,2)=7.19524067D+0
      EGY(6,2)=8.34570043D+0
      EGY(7,2)=9.41816042D+0
      EGY(8,2)=1.04302456D+1
      EGY(9,2)=1.13936551D+1
      EGY(10,2)=2.23815690D+0
      EGY(11,2)=4.01510843D+0
      EGY(12,2)=5.47323073D+0
      EGY(13,2)=3.43174049D+0
      EGY(14,2)=4.94366366D+0
      EGY(15,2)=6.26579194D+0
      EGY(16,2)=7.46532890D+0
      EGY(1,3)=-1.34685458D+0
      EGY(2,3)=2.07327154D+0
      EGY(3,3)=3.98088453D+0
      EGY(4,3)=5.49359982D+0
      EGY(5,3)=6.80848165D+0
      EGY(6,3)=7.99882071D+0
      EGY(7,3)=9.10109086D+0
      EGY(8,3)=1.01365569D+1
      EGY(9,3)=1.11189323D+1
      EGY(10,3)=1.59060768D+0
      EGY(11,3)=3.54276033D+0
      EGY(12,3)=5.08423240D+0
      EGY(13,3)=2.98962775D+0
      EGY(14,3)=4.58105720D+0
      EGY(15,3)=5.95123186D+0
      EGY(16,3)=7.18374555D+0
      EGY(1,4)=-3.28750348D+0
      EGY(2,4)=1.28629940D+0
      EGY(3,4)=3.42447702D+0
      EGY(4,4)=5.04067177D+0
      EGY(5,4)=6.41707487D+0
      EGY(6,4)=7.64932760D+0
      EGY(7,4)=8.78252475D+0
      EGY(8,4)=9.84203559D+0
      EGY(9,4)=1.08438005D+1
      EGY(10,4)=8.74448694D-1
      EGY(11,4)=3.04350596D+0
      EGY(12,4)=4.68055623D+0
      EGY(13,4)=2.52208893D+0
      EGY(14,4)=4.20447739D+0
      EGY(15,4)=5.62766179D+0
      EGY(16,4)=6.89578767D+0
      EGY(1,5)=-5.66676708D+0
      EGY(2,5)=4.35051763D-1
      EGY(3,5)=2.84881318D+0
      EGY(4,5)=4.57992289D+0
      EGY(5,5)=6.02214651D+0
      EGY(6,5)=7.29829604D+0
      EGY(7,5)=8.46345121D+0
      EGY(8,5)=9.54758438D+0
      EGY(9,5)=1.05690809D+1
      EGY(10,5)=8.11854009D-2
      EGY(11,5)=2.51605798D+0
      EGY(12,5)=4.26214687D+0
      EGY(13,5)=2.02688445D+0
      EGY(14,5)=3.81324108D+0
      EGY(15,5)=5.29485147D+0
      EGY(16,5)=6.60139089D+0

      IF(X.LT.1.D-3) GO TO 12
      DO 11 J=1,5
        EI(J)=EGY(I,J)
   11 CONTINUE
      CALL INTPL(1.0D+0,EI,X,EO,1.0D+0,0)
      EIGEN=EO
      RETURN

   12 EGY0(1)=2.33810741D+0
      EGY0(2)=4.08794944D+0
      EGY0(3)=5.52055983D+0
      EGY0(4)=6.78670809D+0
      EGY0(5)=7.94413359D+0
      EGY0(6)=9.02265085D+0
      EGY0(7)=1.00401743D+1
      EGY0(8)=1.10085243D+1
      EGY0(9)=1.19360155D+1
      EGY0(10)=3.36125453D+0
      EGY0(11)=4.88445185D+0
      EGY0(12)=6.20762330D+0
      EGY0(13)=4.24818226D+0
      EGY0(14)=5.62970838D+0
      EGY0(15)=6.86888269D+0
      EGY0(16)=8.00970292D+0
      EIGEN=EGY0(I)
      RETURN
      END
      SUBROUTINE FCALC(NSTEPS,EMIN,ESTEP,WE,R,ANS)
      IMPLICIT NONE
      INTEGER NSTEPS            !Number of energy steps
      DOUBLE PRECISION EMIN     !Minimum energy (typically 2*m(D0))
      DOUBLE PRECISION ESTEP    !Energy step size (both in GeV)
      DOUBLE PRECISION WE(*)    !Array of c.m. energy values
      DOUBLE PRECISION R(*)     !Total R(charm) at each energy
C***  ANS(*) declared below (after FIST.INC)
C---------------------------------------------------------------------
C  Actual calculations of Eichten coupled-channel model.  Absolute
C  production rates, in units of R, for charmed mesons are computed
C  for the e+ e- collision energies specified by EMIN, ESTEP, NSTEPS.
C  Results are returned in the arrays WE (energies), R (total Rcharm),
C  and ANS (production rates for up to 16 meson species).
C
C  950722  Michael Kelsey -- Extracted from EICHTEN.F, main loop
C                            moved to FMATRIX.F (OMEGA, HEX, DPW1,2)
C  950723  Rearrange AIR and CORR into single loop, different logging
C  950724  Extract calculation of R, ANS vs. W to separate routine
C---------------------------------------------------------------------
      INCLUDE 'fist.inc'
      INCLUDE 'gen.inc'

C  Additional Arguments
      REAL ANS(FIST_NFS,*)      !Charm production rates

C  Useful Parameters
      DOUBLE PRECISION PI
      PARAMETER (PI = 3.14159265358979D+00)

C  Local Variables
      COMPLEX DET,HAW,PSA1,PSA2,PSB,QED1,QED2
      COMPLEX QCD1(FIST_MIX),QCD2(FIST_MIX)
      DOUBLE PRECISION DA(500),DZ(500),HA(500),AIR(500)
      DOUBLE PRECISION AREA,W
      INTEGER I,J,K
      REAL XYZ,PS(2,2,500)


C  Print headings for logging results
        WRITE(6,56) '0 D-D','D-D*','D*-D*','+ D-D','D-D*','D*-D*'
     1             ,'F-F','F-F*','F*-F*','D-DP1S','D-DP1T','D-DP2'
     2             ,'D*DP0','D*DPS1','D*DPT1','RTOT'
 56     FORMAT('ENERGY',2X,15A6,2X,A7)

C  Set up external results file
        OPEN(3,FILE='eichten.results',STATUS='UNKNOWN')
        WRITE(3,556)'0 D-D','D-D*','D*-D*','+ D-D','D-D*','D*-D*'
     1             ,'F-F','F-F*','F*-F*','RTOT'
C joe  556    FORMAT('ENERGY',2X,9A6,2X,A7)
 556    FORMAT('ENERGY',2X,9A9,2X,A9)

C  Loop over energy steps
        DO 47 K=1,NSTEPS
          W = EMIN + ESTEP*(K-1)
          WE(K) = W

          CALL RCALC(W,R(K),ANS(1,K),DET,HAW)
          DA(K)=DBLE(CABS(DET))
          DZ(K)=DBLE(REAL(DET))
          HA(K)=DBLE(CABS(HAW))

C  Record results in external results file and output log
           WRITE(3,558) W,(ANS(J,K),J=1,9),R(K)
C joe  558       FORMAT(F6.3,2X,9F6.3,2X,F7.4)
 558       FORMAT(F6.3,2X,9F9.6,2X,F9.6)

           WRITE(6,58) W,(ANS(J,K),J=1,15),R(K)
 58        FORMAT(F6.3,2X,15F6.3,2X,F7.4)

C  Integrate current step
          IF (K.EQ.1) AIR(1) = 0D0
          IF (K.GT.1) AIR(K)= ESTEP*(R(K-1)+R(K))/2D0 + AIR(K-1)

C  Compute phase shifts due to mixing effects
          CALL MINV(OMEGA,FIST_MIX,DPW1,QCD1,PSA1,XYZ,PSB)
          CALL MINV(OMEGA,FIST_MIX,DPW2,QCD2,PSA2,XYZ,PSB)

          QED1=(0.0,0.0)
          QED2=(0.0,0.0)
          DO I=1,FIST_MIX
             QED1=QED1+DPW1(I)*QCD1(I)
             QED2=QED2+DPW1(I)*QCD2(I)
          ENDDO
          
          PS(1,1,K)=REAL(QED1)
          PS(1,2,K)=AIMAG(QED1)
          PS(2,1,K)=REAL(QED2)
          PS(2,2,K)=AIMAG(QED2)
 47     CONTINUE

C  Close results file
        CLOSE(3)

C  Compute and report phase shifts
        OPEN(55,FILE='eichten.phases',STATUS='UNKNOWN')

        CALL PHASE1(WE,PS,NSTEPS,1,IPHFL)

        WRITE(55,49)
 49     FORMAT(/,T20,'RADIAL QUANTUM NUMBER',T50,
     1       'AREA IN NO COUPLING LIMIT')

        DO J=1,6
           AREA=(8.*PI*MC/A**2)/CM(J)**2
           WRITE(55,51) J,AREA
 51        FORMAT(T25,I3,T55,F10.7)
        ENDDO
        
        WRITE(55,53)
 53     FORMAT(/,T10,'ENERGY',T20,'RATIO',T43,'AREA',T60,
     1       'ABSOLUTE VALUE OF DETERMINANT')

        DO K=1,NSTEPS
           WRITE(55,55) WE(K),R(K),AIR(K),DA(K)
 55        FORMAT(T7,F10.7,T18,F10.7,T35,F14.7,T63,F14.7)
        ENDDO

        CLOSE(55)
      RETURN
      END
      SUBROUTINE FMATRIX(N,M,W)
      IMPLICIT NONE
      INTEGER N,M               !Matrix element to be computed
      DOUBLE PRECISION W        !Center-of-mass energy
C-----------------------------------------------------------------------
C  Computes matrix element term (N,M) for Eichten coupled-channel model,
C  for specified energy.  The OMEGA and HEX matrices are symmetric, so
C  both (N,M) and (M,N) terms are filled.
C
C  950722  Michael Kelsey -- Extracted from loop of FCALC.F
C  950724  Matrices moved into FIST.INC (common /DFMIX/)
C-----------------------------------------------------------------------
      INCLUDE 'fist.inc'

C  Useful Parameters
      DOUBLE PRECISION PI
      PARAMETER (PI = 3.14159265358979D+00)

C  Local Variables
      DOUBLE PRECISION ACORN(3,3),MFS(2)
      DOUBLE PRECISION OMEGA1,OMEGA2,OMEGAR,OMEGAI
      DOUBLE PRECISION CONST,DW,EGY,ES1,ES2,PMAX,PW,RHOW
      INTEGER I,IB,IFS,J,JMIX(3,2),JFLAG(6)
      REAL SMEGA1

C  Inline Functions
      DOUBLE PRECISION FUN,X,M1,M2,E0,E1
      FUN(X,M1,M2,E0,E1)=(1./4.)*DLOG((E1-X)/(X-E0))*((M2**2-M1**2
     1   )**2/X**3-X)-(1./(4.*X**3))*DLOG(E1/E0)*(M2**2-M1**2)**2+
     2   ((E0-E1)/4.)*(1+((M2**2-M1**2)**2/(X*E0*E1))*((1./X)+(E0+
     3   E1)/(2.*E0*E1)))


C  Initialization
        CONST=1./(2.*PI**2)
        PMAX=H*(FIST_NP-1)

        OMEGAR=0.D+0
        OMEGAI=0.D+0

C  Loop over states to be calculated
        DO IFS=1,FIST_NFS
          CALL FSTATE(W,N,M,IFS,ACORN,JMIX,JFLAG,MFS,PW,RHOW,DW)
          IF(IFS.EQ.1) DPW1(N)=CMPLX(REAL(DW),0.0)
          IF(IPHFL.NE.1) DPW2(N)=CMPLX(0.0,0.0)
          IF(IFS.EQ.4.AND.IPHFL.EQ.1) DPW2(N)=CMPLX(REAL(DW),0.0)

C  Set useful mass and energy parameters
          M1=MFS(1)
          M2=MFS(2)
          E0=M1+M2
          E1=DSQRT(PMAX**2+M1**2)+DSQRT(PMAX**2+M2**2)

C  Integrate form factors
          OMEGA1=0.D+0
          OMEGA2=0.D+0
          DO I=1,FIST_NP
            CALL FORM(W,I,IFS,ACORN,JMIX,JFLAG,MFS,PW,RHOW,DW,OMEGA1)
          ENDDO

          IF(W.LE.E0+1.D-5) THEN
            OMEGA1=CONST*OMEGA1
          ELSE
            OMEGA1=CONST*(OMEGA1+RHOW*FUN(W,M1,M2,E0,E1))
          ENDIF

C  Iaginary contribution to matrix element
          OMEGA2 = OMEGA2-PI*CONST*RHOW*(W**4-(M1**2-M2**2)**2)
     >             /(4.*W**3)

          HEX(N,M,IFS)=(-1.)*CMPLX(0.0,REAL(OMEGA2))
          HEX(M,N,IFS)=HEX(N,M,IFS)

          OMEGAR=OMEGAR+OMEGA1
          OMEGAI=OMEGAI+OMEGA2
        ENDDO                    !IFS

        SMEGA1=REAL(OMEGAR+MSD(N,M))
        IF(M.EQ.N) SMEGA1=REAL(OMEGAR+MSD(N,M)-W)

C  Fill in final result for OMEGA and return
        OMEGA(N,M)=CMPLX(SMEGA1,REAL(OMEGAI))
        OMEGA(M,N)=OMEGA(N,M)
      RETURN
      END
      SUBROUTINE FORM(W,I,IFS,ACORN,JMIX,JFLAG,MFS,PW,RHOW,DW,OMEGA1)
      IMPLICIT NONE
      DOUBLE PRECISION W
      INTEGER I, IFS
      DOUBLE PRECISION ACORN(3,3),MFS(*),PW,RHOW,DW
      INTEGER JMIX(3,2),JFLAG(*)
      DOUBLE PRECISION OMEGA1          !INPUT and OUTPUT
C-----------------------------------------------------------------------
C  Computes contribution to the OMEGA1 matrix element for a single step
C  in momentum (I) for a specified final state (IFS).  OMEGA1 is both
C  INPUT and OUTPUT.
C
C  950723  Michael Kelsey -- Extracted from loop of FMATRIX.F
C-----------------------------------------------------------------------
      INCLUDE 'fist.inc'

C  Local Variables
      DOUBLE PRECISION EGY,ES1,ES2,E0,RHOX,RHOXD,X
      INTEGER IF1,IF1MAX,IF2,IF2MAX,LI


C  Initialization
        X=H*(I-1)
        ES1=DSQRT(X**2+MFS(1)**2)
        ES2=DSQRT(X**2+MFS(2)**2)
        EGY=ES1+ES2
        E0 =MFS(1)+MFS(2)

C  Loop over mixing states
        RHOX=0.D+0
        RHOXD=0.D+0
        IF1MAX=JFLAG(5)
        IF (IF1MAX.LE.0) IF1MAX=0
        DO IF1=1,IF1MAX
           IF2MAX=JFLAG(6)
           IF (IF2MAX.LE.0) IF2MAX=0
           DO IF2=1,IF2MAX
              IF (JFLAG(1).EQ.2) THEN
                 RHOX=X*ACORN(IF1,IF2)*F(JMIX(IF1,1),I) 
     1                *F(JMIX(IF2,2),I)+RHOX
                 IF (I.EQ.1) THEN
                    RHOXD=ACORN(IF1,IF2)*(X+H)*F(JMIX(IF1,1),2)
     1                   *F(JMIX(IF2,2),2)+RHOXD
                 ELSEIF (I.LT.FIST_NP) THEN
                    RHOXD=ACORN(IF1,IF2)*((X+H)*F(JMIX(IF1,1),I+1)
     1                   *F(JMIX(IF2,2),I+1)-(X-H)*F(JMIX(IF1,1),I-1)
     2                   *F(JMIX(IF2,2),I-1))+RHOXD
                 ENDIF
              ELSE
                 RHOX=X*ACORN(IF1,IF2)*D(JMIX(IF1,1),I) 
     1                *D(JMIX(IF2,2),I)+RHOX
                 IF (I.EQ.1) THEN
                    RHOXD=ACORN(IF1,IF2)*(X+H)*D(JMIX(IF1,1),2)
     1                   *D(JMIX(IF2,2),2)+RHOXD
                 ELSEIF (I.LT.FIST_NP) THEN
                    RHOXD=ACORN(IF1,IF2)*((X+H)*D(JMIX(IF1,1),I+1)
     1                   *D(JMIX(IF2,2),I+1)-(X-H)*D(JMIX(IF1,1),I-1)
     2                   *D(JMIX(IF2,2),I-1))+RHOXD
                 ENDIF
              ENDIF
           ENDDO                !IF2
        ENDDO                   !IF1
        
        LI=MOD(I,2)
        IF (DABS(X-PW).LT.1.D-5.AND.W.GT.E0-1.D-5) THEN
           IF (I.EQ.1) THEN
              OMEGA1=OMEGA1-1./6.*RHOXD*ES1*ES2/EGY
           ELSEIF (I.NE.FIST_NP) THEN
              OMEGA1=OMEGA1-(2-LI)/3.*RHOXD*ES1*ES2/EGY
           ENDIF
        ELSE
           IF (I.EQ.1.OR.I.EQ.FIST_NP) THEN
              OMEGA1=OMEGA1+(1./3.)*(RHOX-RHOW)*X*H/(W-EGY)
           ELSE
              OMEGA1=OMEGA1+2*(2-LI)*(X/3.)*(RHOX-RHOW)*H/(W-EGY)
           ENDIF
        ENDIF

C  Return values of OMEGA1
      RETURN
      END
      SUBROUTINE FSTATE(W,NDY,MDY,IFS,ACORN,JMIX,JFLAG,MFS,PW,RHOW,DW)
      IMPLICIT NONE
      DOUBLE PRECISION W,ACORN(3,3),MFS(*),PW,RHOW,DW
      INTEGER NDY,MDY,IFS,JMIX(3,2),JFLAG(*)

      INCLUDE 'fist.inc'
      INCLUDE 'gen.inc'

C  External Functions
      DOUBLE PRECISION SIXJ
      EXTERNAL SIXJ

C  Local Variables
      DOUBLE PRECISION AMSQ(5),DPH(5),FEM(2),AMPS,DWP,E0,XI
      INTEGER J2,L1,L2,S1,S2,K1AMAX,K2AMAX,K1C,ILZ,IY,ILY,IZ,K2C
      INTEGER JA,JBMAX,JB,JAMAX,K,KK,JJ,ILX,IK,KFS,IDY,IJ
      INTEGER JDY,KMFS,JINDEX,IX,K2AC,JFL1,JFL2,K1AC


C  Initialize ACORN
        DO IJ=1,3
           DO IK=1,3
              ACORN(IJ,IK)=0.0D+00
           ENDDO
        ENDDO

C  Initialize JMIX
        DO IJ=1,3
           DO IK=1,2
              JMIX(IJ,IK)=0
           ENDDO
        ENDDO

C  Initialize JFLAG
        FEM(1)=5.0D-3
        FEM(2)=2.6D-3

        JFLAG(4)=0
        IF(FIST_NFS.GT.13) JFLAG(4)=1
        KFS=IFS
        IF(IFS.GT.3) KFS=IFS-3*JFLAG(4)
        IDY=NDY-NS1
        JDY=MDY-NS1
        JFLAG(1)=1
        IF(KFS.GT.3.AND.KFS.LT.7) JFLAG(1)=2
        JFLAG(2)=2
        IF(KFS.LT.7) JFLAG(2)=1
        JFLAG(3)=3
        IF(NDY.LE.NS1) THEN
           JFLAG(3)=2
           IDY=IDY+4
        ENDIF
        IF(MDY.LE.NS1) THEN
           JFLAG(3)=1
           JDY=JDY+4
        ENDIF
        JFLAG(5)=JFLAG(2)
        IF(JFLAG(3).EQ.3) JFLAG(5)=JFLAG(5)+1
        JFLAG(6)=JFLAG(5)
        IF(JFLAG(2).EQ.2.AND.JFLAG(3).EQ.2) JFLAG(6)=3

        J2=1
        IF(KFS.EQ.10) J2=0
        IF(KFS.EQ.9.OR.KFS.EQ.13) J2=2

        S1=1
        S2=1
        IF(KFS.GE.7.AND.KFS.LE.9) S1=0
        IF(KFS.EQ.7.OR.KFS.EQ.11) S2=0

        JFL1=1
        IF(KFS.EQ.3.OR.KFS.GT.9) JFL1=2
        IF(KFS.GT.3.AND.KFS.LT.6) JFL1=3
        IF(KFS.EQ.6) JFL1=4

        MFS(1)=MASS(JFL1,1)
        IF(JFLAG(4).NE.0) THEN
           IF(IFS.EQ.4.OR.IFS.EQ.5) MFS(1)=MFS(1)+FEM(1)
           IF(IFS.EQ.6) MFS(1)=MFS(1)+FEM(2)
        ENDIF

        JFL2=KFS-9
        IF(KFS.EQ.1) JFL2=1
        IF(JFL2.LT.0) JFL2=2
        IF(KFS.EQ.4.OR.KFS.EQ.8) JFL2=3
        IF(KFS.EQ.5.OR.KFS.EQ.6.OR.KFS.EQ.9) JFL2=4

        MFS(2)=MASS(JFL2,JFLAG(2))
        IF(JFLAG(4).NE.0) THEN
           IF(IFS.EQ.4) MFS(2)=MFS(2)+FEM(1)
           IF(IFS.EQ.5.OR.IFS.EQ.6) MFS(2)=MFS(2)+FEM(2)
        ENDIF

        DO K1AC=1,3
           DO K2AC=1,3
              ACORN(K1AC,K2AC)=0.D+0
           ENDDO
        ENDDO

        KMFS=MOD(KFS-1,3)

        JINDEX=JFLAG(3)
        GO TO (10,20,30), JINDEX

C  JINDEX = 1
 10   CONTINUE
        IF(JFLAG(2).EQ.1) THEN
           JMIX(1,1)=IDY
           JMIX(2,1)=0
           JMIX(3,1)=0
           JMIX(1,2)=JDY
           JMIX(2,2)=0
           JMIX(3,2)=0
           ACORN(1,1)=1./3.*(1.+3.*KMFS)
        ELSE
           JMIX(1,1)=2*IDY+7
           JMIX(2,1)=2*IDY+8
           JMIX(3,1)=0
           JMIX(1,2)=2*JDY+7
           JMIX(2,2)=2*JDY+8
           JMIX(3,2)=0
           L1=0
           L2=0
           DO ILX=0,2
              ACORN(1,1)=ACORN(1,1)
     1             +SIXJ(J2,S1,S2,L1,0,0,ILX)*SIXJ(J2,S1,S2,L2,0,0,ILX)
              ACORN(2,2)=ACORN(2,2)
     1             +SIXJ(J2,S1,S2,L1,2,2,ILX)*SIXJ(J2,S1,S2,L2,2,2,ILX)
           ENDDO
        ENDIF
      GO TO 40
        
C  JINDEX = 2
 20   CONTINUE
        IF(JFLAG(2).EQ.1) THEN
           JMIX(1,1)=IDY
           JMIX(2,1)=0
           JMIX(3,1)=0
           JMIX(1,2)=2*JDY+3
           JMIX(2,2)=2*JDY+4
           JMIX(3,2)=0
           ACORN(1,1)=DSQRT(2.D+0)/3.*(2.-3.*(KMFS-1.)**2)
        ELSE
 25        JMIX(1,1)=2*IDY+7
           JMIX(2,1)=2*IDY+8
           JMIX(3,1)=0
           JMIX(1,2)=3*JDY+14
           JMIX(2,2)=3*JDY+15
           JMIX(3,2)=3*JDY+16
           L1=0
           L2=2
           DO ILY=0,2
              ACORN(1,1)=ACORN(1,1)
     1             +SIXJ(J2,S1,S2,L1,0,0,ILY)*SIXJ(J2,S1,S2,L2,0,2,ILY)
              ACORN(2,2)=ACORN(2,2)
     1             +SIXJ(J2,S1,S2,L1,2,2,ILY)*SIXJ(J2,S1,S2,L2,2,0,ILY)
              ACORN(2,3)=ACORN(2,3)
     1             +SIXJ(J2,S1,S2,L1,2,2,ILY)*SIXJ(J2,S1,S2,L2,2,2,ILY)
           ENDDO
        ENDIF
      GO TO 40

C  JINDEX = 3
 30   CONTINUE
        IF(JFLAG(2).EQ.1) THEN
           JMIX(1,1)=2*IDY+3
           JMIX(2,1)=2*IDY+4
           JMIX(3,1)=0
           JMIX(1,2)=2*JDY+3
           JMIX(2,2)=2*JDY+4
           JMIX(3,2)=0
           ACORN(1,1)=2./3.
           IF(KMFS.EQ.2) ACORN(1,1)=ACORN(1,1)-6./15.
           IF(KMFS.EQ.2) ACORN(2,2)=12./5.
        ELSE
           JMIX(1,1)=3*IDY+14
           JMIX(2,1)=3*IDY+15
           JMIX(3,1)=3*IDY+16
           JMIX(1,2)=3*IDY+14
           JMIX(2,2)=3*IDY+15
           JMIX(3,2)=3*IDY+16
           L1=2
           L2=2
           DO ILZ=0,2
              ACORN(1,1)=ACORN(1,1)
     1             +SIXJ(J2,S1,S2,L1,0,2,ILZ)*SIXJ(J2,S1,S2,L2,0,2,ILZ)
              ACORN(2,2)=ACORN(2,2)
     1             +SIXJ(J2,S1,S2,L1,2,0,ILZ)*SIXJ(J2,S1,S2,L2,2,0,ILZ)
              ACORN(2,3)=ACORN(2,3)
     1             +SIXJ(J2,S1,S2,L1,2,0,ILZ)*SIXJ(J2,S1,S2,L2,2,2,ILZ)
              ACORN(3,3)=ACORN(3,3)
     1             +SIXJ(J2,S1,S2,L1,2,2,ILZ)*SIXJ(J2,S1,S2,L2,2,2,ILZ)
           ENDDO
           ACORN(3,2)=ACORN(2,3)
        ENDIF
      GO TO 40

C  Processing after JINDEX tests
 40   CONTINUE
        IF(JFLAG(1).EQ.1) THEN
          IF(JFLAG(2).NE.1.OR.JFLAG(4).EQ.0) THEN
             DO K1AC=1,JFLAG(5)
                DO K2AC=1,JFLAG(6)
                   ACORN(K1AC,K2AC)=2.*ACORN(K1AC,K2AC)
                ENDDO
             ENDDO
          ENDIF
        ENDIF

        IF(JFLAG(2).NE.1) THEN
           DO K1C=1,3
              DO K2C=1,3
                 ACORN(K1C,K2C)=0.D+0
              ENDDO
           ENDDO
        ENDIF

        E0=MFS(1)+MFS(2)
        PW=0.D+0
        RHOW=0.D+0
        IF(W.LE.E0+1.D-5) RETURN
        PW=DSQRT(W**2-2.*(MFS(1)**2+MFS(2)**2) +W**(-2)*(MFS(1)**2-
     1       MFS(2)**2)**2)/2.
        K=IDINT((PW+.5*H)/H)+1
        KK=K
        IF(KK.LT.3) KK=3
        DO JJ=1,5
           AMSQ(JJ)=0.D+0
           DPH(JJ)=D(JMIX(1,1),KK-3+JJ)

           DO JA=1,JFLAG(5)
              DO JB=1,JFLAG(6)
                 IF(JFLAG(1).EQ.1) THEN
                    AMSQ(JJ)=AMSQ(JJ)
     1                   +ACORN(JA,JB)*D(JMIX(JA,1),KK-3+JJ)
     2                   *D(JMIX(JB,2),KK-3+JJ)
                 ELSE
                    AMSQ(JJ)=AMSQ(JJ)
     1                   +ACORN(JA,JB)*F(JMIX(JA,1),KK-3+JJ)
     2                   *F(JMIX(JB,2),KK-3+JJ)
                 ENDIF
              ENDDO
           ENDDO
        ENDDO                   !JJ loop

        XI=(KK-3)*H
        CALL INTPL(XI,AMSQ,PW,AMPS,H,0)
        CALL INTPL(XI,DPH,PW,DWP,H,0)
        DW=DWP
        IF(MDY.GT.NS1) DW=-DSQRT(2.D+0)*DWP
        RHOW=PW*AMPS
      RETURN
      END
      SUBROUTINE GRNEX(W,ANSW,KFLAG)
      IMPLICIT NONE
      DOUBLE PRECISION W
      INTEGER KFLAG
      REAL ANSW(*)
C------------------------------------------------------------------------
C  Apply mixing matrices to charm production states.  Results returned
C  in ANSW and B arrays.
C
C  950718  Michael Kelsey -- Extracted from FCALC.FORTRAN
C  950724  Code revised to Fortran 77, redundancies eliminated.
C          Matrices OMEGA, HEX and B moved into FIST.INC
C------------------------------------------------------------------------
      INCLUDE 'fist.inc'
      INCLUDE 'gen.inc'

C  External Functions
      DOUBLE PRECISION WFLAM
      EXTERNAL WFLAM

C  Local Variables
      COMPLEX GRN(FIST_MIX,FIST_MIX),U(FIST_MIX,FIST_MIX)
      COMPLEX WGRN(FIST_MIX),DET,HAT
      DOUBLE PRECISION DARG
      INTEGER I,J,K
      REAL SYMM,ZIP


C  Initialization
        DO I=1,NS1
           DARG=((-1.D+0)**(I+1))*WFLAM(ALAM,I)
           B(I)=CMPLX(REAL(DARG),0.0)
        ENDDO

        DO I=NS1+1,FIST_MIX
           B(I)=CMPLX(0.0,0.0)
        ENDDO

        DO I=1,FIST_MIX
           DO J=1,FIST_MIX
              U(I,J)=(0.0,0.0)
              IF(I.EQ.J) U(I,J)=(1.0,0.0)
           ENDDO
        ENDDO

        DO I=1,FIST_MIX
           CALL MINV(OMEGA,FIST_MIX,U(1,I),GRN(1,I),DET,ZIP,HAT)
        ENDDO

        DO I=1,FIST_MIX
           WGRN(I)=(0.0,0.0)
           DO J=1,FIST_MIX
              WGRN(I)=GRN(I,J)*B(J)+WGRN(I)
           ENDDO
        ENDDO

        DO K=1,FIST_NFS
           SYMM=1.
           IF(FIST_NFS.EQ.13.AND.K.LE.6) SYMM=1./2.
           
           ANSW(K)=0.0
           DO I=1,FIST_MIX
              DO J=1,FIST_MIX
                 ANSW(K)=ANSW(K)
     1                +AIMAG(CONJG(WGRN(I))*SYMM*HEX(I,J,K)
     2                *WGRN(J))*(4./3.)*(6.*MC/A**2)/(W**2)
              ENDDO
           ENDDO
        ENDDO
      RETURN
      END
      SUBROUTINE INTPL(XI,FI,XO,FO,H,J)
      IMPLICIT NONE
      DOUBLE PRECISION XI,FI(5),XO,FO,H
      INTEGER J


C  Local Variables
      DOUBLE PRECISION A,B,DT,HA,HB,HN,TEST,WA,Y0,Y1,YNEAR,YT0,YT1
      INTEGER I,INT1,ISG1,ISG2,ISGR,ITEST

C  Inline Functions
      DOUBLE PRECISION G, HX, Y
      G(Y)=(Y**2-1.)*Y*(Y-2.)*FI(1)/24.-(Y-1.)*Y*(Y**2-4.)*FI(2)/6.
     1  +(Y**2-1.)*(Y**2-4.)*FI(3)/4.-(Y+1.)*Y*(Y**2-4.)*FI(4)/6.
     2  +(Y**2-1.)*Y*(Y+2.)*FI(5)/24.
      HX(Y)=G(Y)-FO


C  Initialization
      IF(J.EQ.1.OR.J.EQ.3) GO TO 10
      IF(J.EQ.2) GO TO 50
      Y=-2.+(XO-XI)/H
      FO=G(Y)
      RETURN
   10 ISG1=FI(1)/DABS(FI(1))
      ISG2=FI(5)/DABS(FI(5))
      ISGR=ISG1*ISG2
      IF(ISGR.GT.0) WRITE(6,11)
   11 FORMAT(20X,'INTERPOLATION FINDS TWO ZEROS IN REGION'
     1   ,' - RETURNING ZERO AT LOWEST VALUE OF ARGUMENT')
      DO 15 I=1,5
      WA=DFLOAT(I-3)
      TEST=ISG1*HX(WA)
      IF(TEST.LT.0.D+0) GO TO 12
      GO TO 13
   12 INT1=I
      GO TO 16
   13 IF(TEST.GT.0.D+0.AND.I.EQ.5) WRITE(6,14)
   14 FORMAT(20X,'NO ZERO FOUND IN THIS REGION-EXTRAPOLATING')
   15 INT1=137
   16 YNEAR=0.D+0
      IF(INT1.EQ.137) GO TO 20
      YT1=INT1-3
      YT0=YT1-1.D+0
      IF(ISGR.GT.0) YT1=YT1-.5
      HA=HX(YT1)
      HB=HX(YT0)
      YNEAR=(HA*YT0-HB*YT1)/(HA-HB)
   20 Y0=YNEAR
      IF(J.EQ.3) Y0=2.0D+0
      Y1=Y0+.2D+0
      DO 35 ITEST=1,50
      A=HX(Y1)
      B=HX(Y0)
      Y=(A*Y0-B*Y1)/(A-B)
      HN=HX(Y)
      IF(DABS(HN).LT.1.D-5) GO TO 40
      Y0=Y1
      Y1=Y
      IF(ITEST.EQ.50) WRITE(6,30)
   30 FORMAT(20X,'COULD NOT FIND ANY SOLUTION FOR THIS VALUE OF FO')
   35 CONTINUE
   40 XO=XI+(2+Y)*H
      RETURN
   50 Y=-2.+(XO-XI)/H
      DT=.02
      FO=(G(Y+DT)-G(Y-DT))/(2*DT*H)
      RETURN
      END
      SUBROUTINE LOADdata
      IMPLICIT NONE
C---------------------------------------------------------------------
C  Read data from *.eichten for loadgen.f loadmix.f lodgen.f
C
C  990520  Chen Jiangchuan -- Extracted from above codes
C---------------------------------------------------------------------
      INCLUDE 'gen.inc'
      INCLUDE 'fist.inc'
      INCLUDE 'mix.inc'
      integer i
********loadgen ******
        data cm/.31430839D+01,  .38020409D+01,  .42802358D+01
     &,  .46867534D+01,  .50513225D+01,  .53872132D+01
     &,  .57017567D+01,  .59995308D+01,  .62836449D+01/
        data mass/.18646000D+01,  .20067000D+01,  .19685000D+01
     &,  .21124000D+01,  .24228000D+01,  .24577000D+01
     &,  .25353500D+01,  .25732000D+01/
        data mp,ms,mc,a,alps/.33500000D+00,
     &   .45000000D+00,  .18418364D+01
     &,  .21228942D+01,  .38812000D+00/
********loadmix*****
        data SDM/
     & .000D+00, .000D+00, .000D+00, .000D+00, .000D+00
     &, .000D+00, .000D+00, .000D+00, .000D+00, .000D+00
     &, .000D+00,-.800D-01, .000D+00, .000D+00, .000D+00
     &,-.230D+00, .000D+00, .000D+00, .500D-01, .000D+00
     &,-.750D-01/
************loadfist*****
         data p/.0000000D+00,   .3000000D-01,   .6000000D-01
     &,   .9000000D-01,   .1200000D+00,   .1500000D+00
     &,   .1800000D+00,   .2100000D+00,   .2400000D+00
     &,   .2700000D+00,   .3000000D+00,   .3300000D+00
     &,   .3600000D+00,   .3900000D+00,   .4200000D+00
     &,   .4500000D+00,   .4800000D+00,   .5100000D+00
     &,   .5400000D+00,   .5700000D+00,   .6000000D+00
     &,   .6300000D+00,   .6600000D+00,   .6900000D+00
     &,   .7200000D+00,   .7500000D+00,   .7800000D+00
     &,   .8100000D+00,   .8400000D+00,   .8700000D+00
     &,   .9000000D+00,   .9300000D+00,   .9600000D+00
     &,   .9900000D+00,   .1020000D+01,   .1050000D+01
     &,   .1080000D+01,   .1110000D+01,   .1140000D+01
     &,   .1170000D+01,   .1200000D+01,   .1230000D+01
     &,   .1260000D+01,   .1290000D+01,   .1320000D+01
     &,   .1350000D+01,   .1380000D+01,   .1410000D+01
     &,   .1440000D+01,   .1470000D+01,   .1500000D+01
     &,   .1530000D+01,   .1560000D+01,   .1590000D+01
     &,   .1620000D+01,   .1650000D+01,   .1680000D+01
     &,   .1710000D+01,   .1740000D+01,   .1770000D+01
     &,   .1800000D+01,   .1830000D+01,   .1860000D+01
     &,   .1890000D+01,   .1920000D+01,   .1950000D+01
     &,   .1980000D+01,   .2010000D+01,   .2040000D+01
     &,   .2070000D+01,   .2100000D+01,   .2130000D+01
     &,   .2160000D+01,   .2190000D+01,   .2220000D+01
     &,   .2250000D+01,   .2280000D+01,   .2310000D+01
     &,   .2340000D+01,   .2370000D+01,   .2400000D+01
     &,   .2430000D+01,   .2460000D+01,   .2490000D+01
     &,   .2520000D+01,   .2550000D+01,   .2580000D+01
     &,   .2610000D+01,   .2640000D+01,   .2670000D+01
     &,   .2700000D+01,   .2730000D+01,   .2760000D+01
     &,   .2790000D+01,   .2820000D+01,   .2850000D+01
     &,   .2880000D+01,   .2910000D+01,   .2940000D+01
     &,   .2970000D+01,   .3000000D+01,   .3030000D+01
     &,   .3060000D+01/
         data (D1(i),i=1,290)/
     &   .0000000D+00,   .0000000D+00,   .0000000D+00
     &,   .0000000D+00,   .0000000D+00,   .0000000D+00
     &,   .0000000D+00,   .0000000D+00,  -.1523062D+01
     &,   .0000000D+00,  -.3453789D+01,   .0000000D+00
     &,  -.3119340D+01,   .0000000D+00,  -.1717514D+01
     &,   .0000000D+00,   .0000000D+00,   .0000000D+00
     &,  -.6824309D+01,   .0000000D+00,   .0000000D+00
     &,   .1007511D+02,   .3320476D-01,   .7906410D-01
     &,   .7797458D-01,   .4893969D-01,   .8094530D-01
     &,   .3702797D-04,  -.8148525D-01,  -.7315925D-04
     &,  -.1521046D+01,  -.8431857D-03,  -.3445473D+01
     &,  -.3436765D-02,  -.3105588D+01,  -.5659704D-02
     &,  -.1704828D+01,  -.5197049D-02,  -.5622559D-02
     &,  -.5834206D-02,  -.6809740D+01,   .1493540D-01
     &,   .1545466D-01,   .1003648D+02,   .6626733D-01
     &,   .1572700D+00,   .1543996D+00,   .9629200D-01
     &,   .1611155D+00,   .2951858D-03,  -.1614462D+00
     &,  -.5822560D-03,  -.1514963D+01,  -.3364659D-02
     &,  -.3420614D+01,  -.1371837D-01,  -.3064555D+01
     &,  -.2249594D-01,  -.1667079D+01,  -.2063975D-01
     &,  -.2242018D-01,  -.2326425D-01,  -.6766139D+01
     &,   .5941852D-01,   .6148465D-01,   .9921189D+01
     &,   .9885595D-01,   .2337718D+00,   .2278147D+00
     &,   .1406037D+00,   .2396996D+00,   .9902876D-03
     &,  -.2383554D+00,  -.1947356D-02,  -.1504872D+01
     &,  -.7542638D-02,  -.3379475D+01,  -.3067046D-01
     &,  -.2996901D+01,  -.5019439D-01,  -.1605190D+01
     &,  -.4570479D-01,  -.5011913D-01,  -.5200648D-01
     &,  -.6693965D+01,   .1325305D+00,   .1371400D+00
     &,   .9730976D+01,   .1308254D+00,   .3076298D+00
     &,   .2968670D+00,   .1805029D+00,   .3159661D+00
     &,   .2327772D-02,  -.3108328D+00,  -.4556778D-02
     &,  -.1490843D+01,  -.1332775D-01,  -.3322491D+01
     &,  -.5404519D-01,  -.2903713D+01,  -.8799332D-01
     &,  -.1520667D+01,  -.7955634D-01,  -.8827869D-01
     &,  -.9160419D-01,  -.6593951D+01,   .2326260D+00
     &,   .2407204D+00,   .9468719D+01,   .1620553D+00
     &,   .3782806D+00,   .3602136D+00,   .2146951D+00
     &,   .3892986D+00,   .4498542D-02,  -.3775359D+00
     &,  -.8756013D-02,  -.1472975D+01,  -.2066908D-01
     &,  -.3250261D+01,  -.8349039D-01,  -.2786479D+01
     &,  -.1350874D+00,  -.1415559D+01,  -.1209878D+00
     &,  -.1364287D+00,  -.1416386D+00,  -.6467108D+01
     &,   .3570704D+00,   .3695041D+00,   .9138356D+01
     &,   .1923235D+00,   .4449060D+00,   .4166237D+00
     &,   .2421751D+00,   .4589217D+00,   .7672888D-02
     &,  -.4372755D+00,  -.1483140D-01,  -.1451389D+01
     &,  -.2948748D-01,  -.3163630D+01,  -.1185650D+00
     &,  -.2647059D+01,  -.1904190D+00,  -.1292463D+01
     &,  -.1685855D+00,  -.1939240D+00,  -.2012364D+00
     &,  -.6314625D+01,   .5039584D+00,   .5215186D+00
     &,   .8744813D+01,   .2214131D+00,   .5067775D+00
     &,   .4651284D+00,   .2621172D+00,   .5242846D+00
     &,   .1199864D-01,  -.4890183D+00,  -.2300173D-01
     &,  -.1426236D+01,  -.3970385D-01,  -.3063309D+01
     &,  -.1587460D+00,  -.2487643D+01,  -.2526525D+00
     &,  -.1154176D+01,  -.2206323D+00,  -.2597740D+00
     &,  -.2695757D+00,  -.6138195D+01,   .6697851D+00
     &,   .6931422D+00,   .8293901D+01,   .2491957D+00
     &,   .5633645D+00,   .5049026D+00,   .2741013D+00
     &,   .5848951D+00,   .1759751D-01,  -.5319382D+00
     &,  -.3341018D-01,  -.1397731D+01,  -.5121502D-01
     &,  -.2950423D+01,  -.2034045D+00,  -.2310749D+01
     &,  -.3204620D+00,  -.1004032D+01,  -.2752908D+00
     &,  -.3331113D+00,  -.3456893D+00,  -.5939464D+01
     &,   .8510679D+00,   .8807738D+00,   .7792195D+01
     &,   .2755942D+00,   .6142151D+00,   .5353218D+00
     &,   .2777687D+00,   .6402276D+00,   .2456041D-01
     &,  -.5653239D+00,  -.4611644D-01,  -.1365974D+01
     &,  -.6390924D-01,  -.2826116D+01,  -.2519425D+00
     &,  -.2119005D+01,  -.3922700D+00,  -.8455331D+00
     &,  -.3306996D+00,  -.4129238D+00,  -.4285287D+00
     &,  -.5720285D+01,   .1043939D+01,   .1080415D+01
     &,   .7246909D+01,   .3004263D+00,   .6588513D+00
     &,   .5559732D+00,   .2731350D+00,   .6898802D+00
     &,   .3294732D-01,  -.5887492D+00,  -.6109693D-01
     &,  -.1331225D+01,  -.7761880D-01,  -.2691630D+01
     &,  -.3036249D+00,  -.1915408D+01,  -.4664558D+00
     &,  -.6822778D+00,  -.3848392D+00,  -.4980736D+00
     &,  -.5169143D+00,  -.5483017D+01,   .1244340D+01
     &,   .1287869D+01,   .6665778D+01,   .3235761D+00
     &,   .6969408D+00,   .5667750D+00,   .2605329D+00
     &,   .7334631D+00,   .4278295D-01,  -.6019869D+00
     &,  -.7825501D-01,  -.1293711D+01,  -.9229452D-01
     &,  -.2548289D+01,  -.3570935D+00,  -.1702887D+01
     &,  -.5412550D+00,  -.5180027D+00,  -.4357658D+00
     &,  -.5873799D+00,  -.6096220D+00,  -.5229549D+01
     &,   .1448140D+01,   .1498863D+01,   .6056789D+01
     &,   .3449443D+00,   .7281808D+00,   .5675227D+00
     &,   .2405844D+00,   .7708654D+00,   .5406594D-01
     &,  -.6049848D+00,  -.9737704D-01,  -.1253674D+01
     &,  -.1077607D+00,  -.2397477D+01,  -.4126847D+00
     &,  -.1484526D+01,  -.6151800D+00,  -.3562339D+00
     &,  -.4815646D+00,  -.6796193D+00,  -.7053837D+00
     &,  -.4962550D+01,   .1651039D+01,   .1708951D+01
     &,   .5428223D+01,   .3644482D+00,   .7525564D+00
     &,   .5585601D+00,   .2139598D+00/
         data (D1(i),i=291,580)/
     &   .8018670D+00,   .6675570D-01,  -.5979423D+00
     &,  -.1182062D+00,  -.1211372D+01,  -.1238733D+00
     &,  -.2240654D+01,  -.4691039D+00,  -.1263500D+01
     &,  -.6865044D+00,  -.2003855D+00,  -.5206041D+00
     &,  -.7735466D+00,  -.8029085D+00,  -.4684102D+01
     &,   .1848935D+01,   .1913889D+01,   .4788370D+01
     &,   .3820225D+00,   .7699597D+00,   .5403189D+00
     &,   .1817673D+00,   .8263933D+00,   .8078191D-01
     &,  -.5812407D+00,  -.1404063D+00,  -.1167066D+01
     &,  -.1404839D+00,  -.2079193D+01,  -.5255635D+00
     &,  -.1042748D+01,  -.7536563D+00,  -.5370544D-01
     &,  -.5514320D+00,  -.8679144D+00,  -.9009028D+00
     &,  -.4396998D+01,   .2037839D+01,   .2109551D+01
     &,   .4145361D+01,   .3976193D+00,   .7804371D+00
     &,   .5134143D+00,   .1451325D+00,   .8443900D+00
     &,   .9603995D-01,  -.5554445D+00,  -.1635829D+00
     &,  -.1121031D+01,  -.1574423D+00,  -.1914588D+01
     &,  -.5812877D+00,  -.8253058D+00,  -.8151663D+00
     &,   .8093102D-01,  -.5727602D+00,  -.9614928D+00
     &,  -.9980910D+00,  -.4103408D+01,   .2214003D+01
     &,   .2292056D+01,   .3507207D+01,   .4112082D+00
     &,   .7841239D+00,   .4786240D+00,   .1053403D+00
     &,   .8560614D+00,   .1124098D+00,  -.5212821D+00
     &,  -.1872943D+00,  -.1073540D+01,  -.1745710D+00
     &,  -.1748278D+01,  -.6355244D+00,  -.6138958D+00
     &,  -.8696987D+00,   .2011757D+00,  -.5836482D+00
     &,  -.1053088D+01,  -.1093235D+01,  -.3806047D+01
     &,   .2373990D+01,   .2457874D+01,   .2881330D+01
     &,   .4227760D+00,   .7811776D+00,   .4368650D+00
     &,   .6378531D-01,   .8615075D+00,   .1297354D+00
     &,  -.4796239D+00,  -.2110651D+00,  -.1024872D+01
     &,  -.1917701D+00,  -.1581665D+01,  -.6875565D+00
     &,  -.4110686D+00,  -.9160791D+00,   .3049757D+00
     &,  -.5834308D+00,  -.1141559D+01,  -.1185111D+01
     &,  -.3507011D+01,   .2514776D+01,   .2603795D+01
     &,   .2274810D+01,   .4323262D+00,   .7720159D+00
     &,   .3891662D+00,   .2181802D-01,   .8609552D+00
     &,   .1478433D+00,  -.4314568D+00,  -.2344000D+00
     &,  -.9753015D+00,  -.2088688D+00,  -.1416096D+01
     &,  -.7367123D+00,  -.2191326D+00,  -.9533165D+00
     &,   .3907883D+00,  -.5718222D+00,  -.1225794D+01
     &,  -.1272685D+01,  -.3208809D+01,   .2633694D+01
     &,   .2727133D+01,   .1694037D+01,   .4398781D+00
     &,   .7569440D+00,   .3366411D+00,  -.1919947D-01
     &,   .8546937D+00,   .1665426D+00,  -.3778575D+00
     &,  -.2567980D+00,  -.9251065D+00,  -.2257242D+00
     &,  -.1252891D+01,  -.7823748D+00,  -.4023286D-01
     &,  -.9806215D+00,   .4575276D+00,  -.5488579D+00
     &,  -.1304887D+01,  -.1354710D+01,  -.2913522D+01
     &,   .2728632D+01,   .2825673D+01,   .1144825D+01
     &,   .4454058D+00,   .7363873D+00,   .2804575D+00
     &,  -.5798336D-01,   .8430669D+00,   .1856294D+00
     &,  -.3199646D+00,  -.2777669D+00,  -.8745506D+00
     &,  -.2421991D+00,  -.1093231D+01,  -.8239896D+00
     &,   .1238950D+00,  -.9974183D+00,   .5046641D+00
     &,  -.5149057D+00,  -.1377720D+01,  -.1430624D+01
     &,  -.2623280D+01,   .2797968D+01,   .2897733D+01
     &,   .6320722D+00,   .4490793D+00,   .7108225D+00
     &,   .2218089D+00,  -.9340257D-01,   .8264660D+00
     &,   .2048907D+00,  -.2589506D+00,  -.2968366D+00
     &,  -.8238950D+00,  -.2581635D+00,  -.9382096D+00
     &,  -.8610715D+00,   .2719087D+00,  -.1003352D+01
     &,   .5322689D+00,  -.4706497D+00,  -.1443879D+01
     &,  -.1499439D+01,  -.2340015D+01,   .2840612D+01
     &,   .2942175D+01,   .1599061D+00,   .4509004D+00
     &,   .6807674D+00,   .1618855D+00,  -.1243485D+00
     &,   .8053210D+00,   .2241090D+00,  -.1959939D+00
     &,  -.3135722D+00,  -.7733903D+00,  -.2734953D+00
     &,  -.7888630D+00,  -.8932101D+00,   .4026223D+00
     &,  -.9982898D+00,   .5408491D+00,  -.4170642D+00
     &,  -.1502523D+01,  -.1560466D+01,  -.2065435D+01
     &,   .2856005D+01,   .2958419D+01,  -.2683976D+00
     &,   .4509428D+00,   .6467711D+00,   .1018472D+00
     &,  -.1500140D+00,   .7800926D+00,   .2430665D+00
     &,  -.1322531D+00,  -.3275850D+00,  -.7232750D+00
     &,  -.2880818D+00,  -.6460933D+00,  -.9200732D+00
     &,   .5153808D+00,  -.9823182D+00,   .5313129D+00
     &,  -.3553793D+00,  -.1553105D+01,  -.1613135D+01
     &,  -.1801122D+01,   .2844134D+01,   .2946441D+01
     &,  -.6504534D+00,   .4493500D+00,   .6094036D+00
     &,   .4279876D-01,  -.1697369D+00,   .7512634D+00
     &,   .2615493D+00,  -.6884220D-01,  -.3385426D+00
     &,  -.6737749D+00,  -.3017950D+00,  -.5106866D+00
     &,  -.9414094D+00,   .6097253D+00,  -.9557333D+00
     &,   .5049608D+00,  -.2870392D+00,  -.1595137D+01
     &,  -.1656941D+01,  -.1548466D+01,   .2805443D+01
     &,   .2906694D+01,  -.9847060D+00,   .4460960D+00
     &,   .5692463D+00,  -.1423239D-01,  -.1830703D+00
     &,   .7193300D+00,   .2793511D+00,  -.6808776D-02
     &,  -.3461767D+00,  -.6251007D+00,  -.3145924D+00
     &,  -.3833080D+00,  -.9570488D+00,   .6855657D+00
     &,  -.9190673D+00,   .4636215D+00,  -.2136555D+00
     &,  -.1628445D+01,  -.1691697D+01,  -.1308584D+01
     &,   .2740915D+01,   .2840190D+01,  -.1270382D+01
     &,   .4413419D+00,   .5268819D+00,  -.6831512D-01
     &,  -.1898846D+00,   .6847943D+00,   .2962772D+00
     &,   .5288483D-01,  -.3502886D+00/
         data (D1(i),i=581,870)/
     &  -.5774478D+00,  -.3263700D+00,  -.2644997D+00
     &,  -.9669022D+00,   .7431204D+00,  -.8729085D+00
     &,   .4092374D+00,  -.1370108D+00,  -.1652761D+01
     &,  -.1717127D+01,  -.1082603D+01,   .2651948D+01
     &,   .2748368D+01,  -.1507595D+01,   .4351951D+00
     &,   .4829441D+00,  -.1186300D+00,  -.1900521D+00
     &,   .6481565D+00,   .3121471D+00,   .1093773D+00
     &,  -.3507533D+00,  -.5309946D+00,  -.3370595D+00
     &,  -.1546808D+00,  -.9709596D+00,   .7828956D+00
     &,  -.8181805D+00,   .3440286D+00,  -.5881883D-01
     &,  -.1668065D+01,  -.1733204D+01,  -.8711428D+00
     &,   .2540336D+01,   .2633076D+01,  -.1697175D+01
     &,   .4277680D+00,   .4378749D+00,  -.1644809D+00
     &,  -.1838095D+00,   .6099074D+00,   .3267979D+00
     &,   .1619208D+00,  -.3475209D+00,  -.4859021D+00
     &,  -.3466046D+00,  -.5414896D-01,  -.9692865D+00
     &,   .8057069D+00,  -.7557509D+00,   .2704182D+00
     &,   .1914141D-01,  -.1674343D+01,  -.1739984D+01
     &,  -.6748484D+00,   .2408205D+01,   .2496511D+01
     &,  -.1840631D+01,   .4191763D+00,   .3922709D+00
     &,  -.2053030D+00,  -.1715379D+00,   .5705784D+00
     &,   .3401079D+00,   .2098893D+00,  -.3406159D+00
     &,  -.4423133D+00,  -.3549612D+00,   .3691594D-01
     &,  -.9620203D+00,   .8124507D+00,  -.6867531D+00
     &,   .1907444D+00,   .9501323D-01,  -.1671911D+01
     &,  -.1737582D+01,  -.4940909D+00,   .2257971D+01
     &,   .2341165D+01,  -.1940076D+01,   .4095379D+00
     &,   .3466396D+00,  -.2406667D+00,  -.1538255D+00
     &,   .5305084D+00,   .3519132D+00,   .2527840D+00
     &,  -.3301345D+00,  -.4003530D+00,  -.3621019D+00
     &,   .1184471D+00,  -.9494582D+00,   .8044471D+00
     &,  -.6123827D+00,   .1075232D+00,   .1672375D+00
     &,  -.1660973D+01,  -.1726416D+01,  -.3289963D+00
     &,   .2091911D+01,   .2169392D+01,  -.1998226D+01
     &,   .3989712D+00,   .3014530D+00,  -.2702784D+00
     &,  -.1312720D+00,   .4901838D+00,   .3621347D+00
     &,   .2902351D+00,  -.3162407D+00,  -.3601276D+00
     &,  -.3680195D+00,   .1904843D+00,  -.9316126D+00
     &,   .7828969D+00,  -.5337787D+00,   .2314353D-01
     &,   .2342325D+00,  -.1641818D+01,  -.1706797D+01
     &,  -.1797194D+00,   .1913258D+01,   .1984535D+01
     &,  -.2018066D+01,   .3875946D+00,   .2572024D+00
     &,  -.2939771D+00,  -.1047172D+00,   .4500014D+00
     &,   .3706965D+00,   .3220002D+00,  -.2991600D+00
     &,  -.3217254D+00,  -.3726396D+00,   .2531915D+00
     &,  -.9090080D+00,   .7493649D+00,  -.4522568D+00
     &,  -.4331962D-01,   .2945554D+00,  -.1615147D+01
     &,  -.1679114D+01,  -.4597598D-01,   .1725566D+01
     &,   .1790264D+01,  -.2003075D+01,   .3755245D+00
     &,   .2141552D+00,  -.3118200D+00,  -.7503592D-01
     &,   .4103811D+00,   .3775836D+00,   .3479600D+00
     &,  -.2791720D+00,  -.2851517D+00,  -.3760373D+00
     &,   .3067392D+00,  -.8819564D+00,   .7054568D+00
     &,  -.3690253D+00,  -.1400702D+00,   .3471249D+00
     &,  -.1581146D+01,  -.1644089D+01,   .7238365D-01
     &,   .1530350D+01,   .1588152D+01,  -.1956946D+01
     &,   .3628749D+00,   .1727175D+00,  -.3237028D+00
     &,  -.4316283D-01,   .3715326D+00,   .3826948D+00
     &,   .3681113D+00,  -.2566024D+00,  -.2506802D+00
     &,  -.3782048D+00,   .3514714D+00,  -.8508589D+00
     &,   .6527165D+00,  -.2853359D+00,  -.2148703D+00
     &,   .3909537D+00,  -.1540648D+01,  -.1602114D+01
     &,   .1758895D+00,   .1331056D+01,   .1381763D+01
     &,  -.1883582D+01,   .3497563D+00,   .1331855D+00
     &,  -.3299153D+00,  -.1003650D-01,   .3338059D+00
     &,   .3860596D+00,   .3825578D+00,  -.2318138D+00
     &,  -.2181148D+00,  -.3791600D+00,   .3877748D+00
     &,  -.8161467D+00,   .5928396D+00,  -.2024144D+00
     &,  -.2828542D+00,   .4253406D+00,  -.1494104D+01
     &,  -.1554056D+01,   .2650287D+00,   .1130449D+01
     &,   .1173958D+01,  -.1786950D+01,   .3362748D+00
     &,   .9586030D-01,  -.3307447D+00,   .2343686D-01
     &,   .2974516D+00,   .3876941D+00,   .3914992D+00
     &,  -.2051961D+00,  -.1875382D+00,  -.3789365D+00
     &,   .4160769D+00,  -.7782736D+00,   .5274595D+00
     &,  -.1213480D+00,  -.3427281D+00,   .4498398D+00
     &,  -.1442345D+01,  -.1500350D+01,   .3405487D+00
     &,   .9312224D+00,   .9675302D+00,  -.1671004D+01
     &,   .3226171D+00,   .6088829D-01,  -.3265596D+00
     &,   .5645258D-01,   .2626814D+00,   .3876345D+00
     &,   .3952192D+00,  -.1771568D+00,  -.1589667D+00
     &,  -.3775743D+00,   .4368867D+00,  -.7377399D+00
     &,   .4581897D+00,  -.4311370D-01,  -.3933069D+00
     &,   .4641943D+00,  -.1385940D+01,  -.1441982D+01
     &,   .4030543D+00,   .7357896D+00,   .7649777D+00
     &,  -.1539706D+01,   .3086960D+00,   .2836123D-01
     &,  -.3177952D+00,   .8800871D-01,   .2296685D+00
     &,   .3859361D+00,   .3940676D+00,  -.1480828D+00
     &,  -.1323961D+00,  -.3751210D+00,   .4507326D+00
     &,  -.6949612D+00,   .3865006D+00,   .3131818D-01
     &,  -.4338998D+00,   .4685565D+00,  -.1325684D+01
     &,  -.1379460D+01,   .4534559D+00,   .5465352D+00
     &,   .5687731D+00,  -.1396795D+01,   .2946974D+00
     &,  -.1546769D-02,  -.3049387D+00,   .1175145D+00
     &,   .1985492D+00,   .3826707D+00,   .3884604D+00
     &,  -.1184463D+00,  -.1078347D+00,  -.3715912D+00
     &,   .4581784D+00,  -.6504403D+00/
         data (D1(i),i=871,1160)/
     &   .3139053D+00,   .1011281D+00,  -.4640151D+00
     &,   .4631572D+00,  -.1262107D+01,  -.1313569D+01
     &,   .4925382D+00,   .3653648D+00,   .3808943D+00
     &,  -.1245890D+01,   .2807041D+00,  -.2876795D-01
     &,  -.2885521D+00,   .1442913D+00,   .1694240D+00
     &,   .3779250D+00,   .3788518D+00,  -.8862457D-01
     &,  -.8518281D-01,  -.3671225D+00,   .4598380D+00
     &,  -.6046463D+00,   .2416355D+00,   .1656176D+00
     &,  -.4834270D+00,   .4484892D+00,  -.1196064D+01
     &,  -.1245049D+01,   .5211974D+00,   .1940830D+00
     &,   .2032149D+00,  -.1090334D+01,   .2667921D+00
     &,  -.5323204D-01,  -.2690978D+00,   .1677915D+00
     &,   .1423599D+00,   .3717981D+00,   .3657269D+00
     &,  -.5900583D-01,  -.6443027D-01,  -.3617408D+00
     &,   .4562501D+00,  -.5580355D+00,   .1711969D+00
     &,   .2242150D+00,  -.4923388D+00,   .4252349D+00
     &,  -.1128184D+01,  -.1174623D+01,   .5404318D+00
     &,   .3417950D-01,   .3728283D-01,  -.9332928D+00
     &,   .2530932D+00,  -.7500566D-01,  -.2471991D+00
     &,   .1876652D+00,   .1173928D+00,   .3643995D+00
     &,   .3495875D+00,  -.2995438D-01,  -.4552317D-01
     &,  -.3555142D+00,   .4480377D+00,  -.5110462D+00
     &,   .1027836D+00,   .2764778D+00,  -.4910615D+00
     &,   .3942399D+00,  -.1059136D+01,  -.1102913D+01
     &,   .5510824D+00,  -.1131391D+00,  -.1156461D+00
     &,  -.7774844D+00,   .2395233D+00,  -.9415825D-01
     &,  -.2233572D+00,   .2035717D+00,   .9453045D-01
     &,   .3558465D+00,   .3309413D+00,  -.1804763D-02
     &,  -.2839927D-01,  -.3485134D+00,   .4357782D+00
     &,  -.4640936D+00,   .3819922D-01,   .3220931D+00
     &,  -.4801759D+00,   .3564821D+00,  -.9894684D+00
     &,  -.1030654D+01,   .5541154D+00,  -.2469497D+00
     &,  -.2546091D+00,  -.6254175D+00,   .2262218D+00
     &,  -.1106906D+00,  -.1981389D+00,   .2154212D+00
     &,   .7375480D-01,   .3462621D+00,   .3102758D+00
     &,   .2517283D-01,  -.1298928D-01,  -.3408114D+00
     &,   .4200874D+00,  -.4175661D+00,  -.2203901D-01
     &,   .3608739D+00,  -.4604552D+00,   .3130398D+00
     &,  -.9199005D+00,  -.9583060D+00,   .5504188D+00
     &,  -.3666073D+00,  -.3789318D+00,  -.4792144D+00
     &,   .2132446D+00,  -.1247536D+00,  -.1720633D+00
     &,   .2231235D+00,   .5502546D-01,   .3357729D+00
     &,   .2881108D+00,   .5064944D-01,   .7483371D-03
     &,  -.3324821D+00,   .4014973D+00,  -.3718215D+00
     &,  -.7741879D-01,   .3927542D+00,  -.4328302D+00
     &,   .2650593D+00,  -.8507796D+00,  -.8866318D+00
     &,   .5408407D+00,  -.4717354D+00,  -.4882168D+00
     &,  -.3405863D+00,   .2005989D+00,  -.1364731D+00
     &,  -.1455840D+00,   .2267198D+00,   .3828254D-01
     &,   .3245068D+00,   .2648924D+00,   .7443963D-01
     &,   .1296575D-01,  -.3235996D+00,   .3805501D+00
     &,  -.3271847D+00,  -.1274205D+00,   .4177815D+00
     &,  -.3983532D+00,   .2137226D+00,  -.7828501D+00
     &,  -.8158994D+00,   .5263170D+00,  -.5622242D+00
     &,  -.5823441D+00,  -.2111710D+00,   .1883482D+00
     &,  -.1450000D+00,  -.1191637D+00,   .2262867D+00
     &,   .2344972D-01,   .3125915D+00,   .2410542D+00
     &,   .9636281D-01,   .2370800D-01,  -.3142330D+00
     &,   .3577525D+00,  -.2839252D+00,  -.1717153D+00
     &,   .4361073D+00,  -.3581611D+00,   .1602495D+00
     &,  -.7162621D+00,  -.7468604D+00,   .5075998D+00
     &,  -.6381399D+00,  -.6613747D+00,  -.9198617D-01
     &,   .1765711D+00,  -.1530800D+00,  -.9319937D-01
     &,   .2221978D+00,   .1043720D-01,   .3001524D+00
     &,   .2169945D+00,   .1162799D+00,   .3306135D-01
     &,  -.3044132D+00,   .3335550D+00,  -.2423439D+00
     &,  -.2101333D+00,   .4479766D+00,  -.3134811D+00
     &,   .1057296D+00,  -.6517330D+00,  -.6795972D+00
     &,   .4854860D+00,  -.6998470D+00,  -.7256805D+00
     &,   .1615068D-01,   .1652452D+00,  -.1583935D+00
     &,  -.6808888D-01,   .2147000D+00,  -.8554870D-03
     &,   .2873110D+00,   .1930724D+00,   .1340928D+00
     &,   .4110919D-01,  -.2943470D+00,   .3084477D+00
     &,  -.2026324D+00,  -.2425831D+00,   .4537351D+00
     &,  -.2654720D+00,   .5131145D-01,  -.5893007D+00
     &,  -.6148493D+00,   .4606760D+00,  -.7478393D+00
     &,  -.7757666D+00,   .1127123D+00,   .1543556D+00
     &,  -.1619054D+00,  -.4404691D-01,   .2041547D+00
     &,  -.1053676D-01,   .2741842D+00,   .1695966D+00
     &,   .1497423D+00,   .4789313D-01,  -.2839954D+00
     &,   .2827887D+00,  -.1649439D+00,  -.2690502D+00
     &,   .4537825D+00,  -.2152566D+00,  -.1949486D-02
     &,  -.5294213D+00,  -.5524905D+00,   .4337870D+00
     &,  -.7827858D+00,  -.8123206D+00,   .1974379D+00
     &,   .1439428D+00,  -.1637313D+00,  -.2135886D-01
     &,   .1909156D+00,  -.1872056D-01,   .2608822D+00
     &,   .1468565D+00,   .1632079D+00,   .5365556D-01
     &,  -.2734266D+00,   .2569792D+00,  -.1294761D+00
     &,  -.2897218D+00,   .4484658D+00,  -.1640762D+00
     &,  -.5316491D-01,  -.4723117D+00,  -.4930750D+00
     &,   .4054531D+00,  -.8054899D+00,  -.8361698D+00
     &,   .2703068D+00,   .1340656D+00,  -.1640703D+00
     &,  -.2635014D-03,   .1755033D+00,  -.2552402D-01
     &,   .2475084D+00,   .1250772D+00,   .1744986D+00
     &,   .5834073D-01,  -.2627206D+00,   .2313102D+00
     &,  -.9631451D-01,  -.3048404D+00,   .4383995D+00
     &,  -.1129359D+00,  -.1013846D+00/
         data (D1(i),i=1161,1450)/
     &  -.4182599D+00,  -.4367749D+00,   .3761469D+00
     &,  -.8168620D+00,  -.8482532D+00,   .3315103D+00
     &,   .1246318D+00,  -.1631535D+00,   .1916152D-01
     &,   .1582829D+00,  -.3106548D-01,   .2341582D+00
     &,   .1044468D+00,   .1836570D+00,   .6208532D-01
     &,  -.2519346D+00,   .2060708D+00,  -.6553622D-01
     &,  -.3146510D+00,   .4240716D+00,  -.6282182D-01
     &,  -.1460023D+00,  -.3672354D+00,  -.3837111D+00
     &,   .3465254D+00,  -.8178938D+00,  -.8495945D+00
     &,   .3814025D+00,   .1156863D+00,  -.1610894D+00
     &,   .3677516D-01,   .1397521D+00,  -.3549918D-01
     &,   .2209705D+00,   .8512172D-01,   .1907532D+00
     &,   .6496041D-01,  -.2411282D+00,   .1815049D+00
     &,  -.3718715D-01,  -.3195907D+00,   .4060368D+00
     &,  -.1469127D-01,  -.1863226D+00,  -.3194129D+00
     &,  -.3339161D+00,   .3168028D+00,  -.8096322D+00
     &,  -.8412754D+00,   .4205304D+00,   .1072255D+00
     &,  -.1580325D+00,   .5246696D-01,   .1203164D+00
     &,  -.3886976D-01,   .2079179D+00,   .6718865D-01
     &,   .1958819D+00,   .6704836D-01,  -.2303523D+00
     &,   .1578190D+00,  -.1128415D-01,  -.3200199D+00
     &,   .3848508D+00,   .3073882D-01,  -.2219821D+00
     &,  -.2748497D+00,  -.2875012D+00,   .2875084D+00
     &,  -.7931558D+00,  -.8244115D+00,   .4495488D+00
     &,   .9924295D-01,  -.1541299D+00,   .6630221D-01
     &,   .1004014D+00,  -.4137014D-01,   .1951792D+00
     &,   .5073095D-01,   .1991539D+00,   .6843160D-01
     &,  -.2196546D+00,   .1351836D+00,   .1218261D-01
     &,  -.3163893D+00,   .3611122D+00,   .7275578D-01
     &,  -.2526078D+00,  -.2335651D+00,  -.2444877D+00
     &,   .2588219D+00,  -.7695532D+00,  -.8001290D+00
     &,   .4692089D+00,   .9172961D-01,  -.1495209D+00
     &,   .7824603D-01,   .8046612D-01,  -.4301553D-01
     &,   .1827008D+00,   .3578836D-01,   .2006963D+00
     &,   .6915205D-01,  -.2090790D+00,   .1137350D+00
     &,   .3324698D-01,  -.3091844D+00,   .3353006D+00
     &,   .1109422D+00,  -.2779945D+00,  -.1955433D+00
     &,  -.2048611D+00,   .2311376D+00,  -.7399024D+00
     &,  -.7695443D+00,   .4803315D+00,   .8467409D-01
     &,  -.1443356D+00,   .8833733D-01,   .6072381D-01
     &,  -.4395290D-01,   .1705933D+00,   .2237150D-01
     &,   .2006481D+00,   .6933811D-01,  -.1986649D+00
     &,   .9357801D-01,   .5196418D-01,  -.2988765D+00
     &,   .3079261D+00,   .1448247D+00,  -.2980577D+00
     &,  -.1607379D+00,  -.1685743D+00,   .2045429D+00
     &,  -.7052543D+00,  -.7337461D+00,   .4837823D+00
     &,   .7806310D-01,  -.1386942D+00,   .9664057D-01
     &,   .4154327D-01,  -.4427822D-01,   .1589000D+00
     &,   .1046550D-01,   .1991561D+00,   .6903214D-01
     &,  -.1884478D+00,   .7478797D-01,   .6843499D-01
     &,  -.2858749D+00,   .2795122D+00,   .1742167D+00
     &,  -.3128248D+00,  -.1290753D+00,  -.1355521D+00
     &,   .1793268D+00,  -.6666169D+00,  -.6937796D+00
     &,   .4804497D+00,   .7188180D-01,  -.1327071D+00
     &,   .1032413D+00,   .2319265D-01,  -.4408088D-01
     &,   .1476563D+00,   .5042134D-04,   .1963499D+00
     &,   .6827238D-01,  -.1784589D+00,   .5741350D-01
     &,   .8269495D-01,  -.2706826D+00,   .2505210D+00
     &,   .1989330D+00,  -.3224252D+00,  -.1004583D+00
     &,  -.1056950D+00,   .1555608D+00,  -.6249434D+00
     &,  -.6506333D+00,   .4712249D+00,   .6611407D-01
     &,  -.1264742D+00,   .1082419D+00,   .5896038D-02
     &,  -.4344344D-01,   .1368900D+00,  -.8956916D-02
     &,   .1924234D+00,   .6716562D-01,  -.1687253D+00
     &,   .4147920D-01,   .9484763D-01,  -.2537270D+00
     &,   .2213776D+00,   .2189339D+00,  -.3270773D+00
     &,  -.7477028D-01,  -.7888262D-01,   .1332740D+00
     &,  -.5811395D+00,  -.6052476D+00,   .4569916D+00
     &,   .6074285D-01,  -.1200853D+00,   .1117576D+00
     &,  -.1016667D-01,  -.4244142D-01,   .1266214D+00
     &,  -.1661487D-01,   .1875106D+00,   .6574446D-01
     &,  -.1592703D+00,   .2698827D-01,   .1050597D+00
     &,  -.2354818D+00,   .1924414D+00,   .2342787D+00
     &,  -.3270765D+00,  -.5189745D-01,  -.5496185D-01
     &,   .1126360D+00,  -.5360645D+00,  -.5585145D+00
     &,   .4385524D+00,   .5575034D-01,  -.1136201D+00
     &,   .1139124D+00,  -.2485890D-01,  -.4114337D-01
     &,   .1168645D+00,  -.2300461D-01,   .1817604D+00
     &,   .6405964D-01,  -.1501134D+00,   .1392529D-01
     &,   .1134261D+00,  -.2163218D+00,   .1640998D+00
     &,   .2451139D+00,  -.3227810D+00,  -.3171324D-01
     &,  -.3386458D-01,   .9356963D-01,  -.4903914D+00
     &,  -.5111342D+00,   .4168148D+00,   .5111830D-01
     &,  -.1071483D+00,   .1148358D+00,  -.3807015D-01
     &,  -.3961090D-01,   .1076271D+00,  -.2821563D-01
     &,   .1753162D+00,   .6215771D-01,  -.1412749D+00
     &,   .2258783D-02,   .1200740D+00,  -.1966146D+00
     &,   .1365927D+00,   .2516846D+00,  -.3145982D+00
     &,  -.1394406D-01,  -.1533611D-01,   .7619538D-01
     &,  -.4448539D+00,  -.4638681D+00,   .3924922D+00
     &,   .4682823D-01,  -.1007305D+00,   .1146601D+00
     &,  -.4975050D-01,  -.3789891D-01,   .9891155D-01
     &,  -.3234305D-01,   .1683144D+00,   .6008563D-01
     &,  -.1328095D+00,  -.8056240D-02,   .1251345D+00
     &,  -.1766936D+00,   .1102537D+00,   .2542029D+00
     &,  -.3029715D+00,   .1492248D-02,   .7067298D-03
     &,   .6040623D-01,  -.3998974D+00/
         data (D1(i),i=1451,1740)/
     &  -.4171827D+00,   .3661661D+00,   .4286154D-01
     &,  -.9441807D-01,   .1135169D+00,  -.5989064D-01
     &,  -.3605590D-01,   .9071555D-01,  -.3548514D-01
     &,   .1608831D+00,   .5793121D-01,  -.1245972D+00
     &,  -.1701419D-01,   .1287403D+00,  -.1568255D+00
     &,   .8525071D-01,   .2530478D+00,  -.2884054D+00
     &,   .1469256D-01,   .1462546D-01,   .4619275D-01
     &,  -.3555678D+00,  -.3711306D+00,   .3385839D+00
     &,   .3919977D-01,  -.8825443D-01,   .1115764D+00
     &,  -.6846833D-01,  -.3412422D-01,   .8303244D-01
     &,  -.3774119D-01,   .1531411D+00,   .5558588D-01
     &,  -.1167269D+00,  -.2380800D-01,   .1310239D+00
     &,  -.1373391D+00,   .6175196D-01,   .2486117D+00
     &,  -.2712861D+00,   .2591306D-01,   .2641324D-01
     &,   .3342426D-01,  -.3132193D+00,  -.3270889D+00
     &,   .3102775D+00,   .3582469D-01,  -.8227509D-01
     &,   .1088784D+00,  -.7567230D-01,  -.3214056D-01
     &,   .7585199D-01,  -.3920968D-01,   .1451976D+00
     &,   .5316943D-01,  -.1092453D+00,  -.3149546D-01
     &,   .1321161D+00,  -.1183650D+00,   .3988565D-01
     &,   .2413236D+00,  -.2521746D+00,   .3533754D-01
     &,   .3626063D-01,   .2214227D-01,  -.2727273D+00
     &,  -.2850063D+00,   .2817522D+00,   .3271843D-01
     &,  -.7650855D-01,   .1055854D+00,  -.8136569D-01
     &,  -.3013626D-01,   .6916091D-01,  -.3998678D-01
     &,   .1371511D+00,   .5074388D-01,  -.1021002D+00
     &,  -.3704372D-01,   .1321448D+00,  -.1001697D+00
     &,   .1974723D-01,   .2314514D+00,  -.2314587D+00
     &,   .4314331D-01,   .4445404D-01,   .1221151D-01
     &,  -.2343854D+00,  -.2451172D+00,   .2534518D+00
     &,   .2982082D-01,  -.7102498D-01,   .1018390D+00
     &,  -.8569968D-01,  -.2813783D-01,   .6294339D-01
     &,  -.4019122D-01,   .1291013D+00,   .4829703D-01
     &,  -.9530150D-01,  -.4158953D-01,   .1312110D+00
     &,  -.8292280D-01,   .1402290D-02,   .2194821D+00
     &,  -.2096519D+00,   .4937831D-01,   .5100880D-01
     &,   .3620280D-02,  -.1983934D+00,  -.2076559D+00
     &,   .2257588D+00,   .2720566D-01,  -.6577623D-01
     &,   .9767847D-01,  -.8876692D-01,  -.2625029D-01
     &,   .5717177D-01,  -.3985397D-01,   .1210977D+00
     &,   .4585276D-01,  -.8884821D-01,  -.4521421D-01
     &,   .1294808D+00,  -.6670238D-01,  -.1518224D-01
     &,   .2057899D+00,  -.1871518D+00,   .5425646D-01
     &,   .5614730D-01,  -.3836053D-02,  -.1648904D+00
     &,  -.1727692D+00,   .1989621D+00,   .2480905D-01
     &,  -.6074660D-01,   .9323244D-01,  -.9067058D-01
     &,  -.2431386D-01,   .5183424D-01,  -.3908808D-01
     &,   .1132250D+00,   .4343082D-01,  -.8273280D-01
     &,  -.4799940D-01,   .1270417D+00,  -.5156204D-01
     &,  -.2994255D-01,   .1908520D+00,  -.1644013D+00
     &,   .5790670D-01,   .6002258D-01,  -.1010507D-01
     &,  -.1339607D+00,  -.1405470D+00,   .1733085D+00
     &,   .2258963D-01,  -.5598594D-01,   .8858667D-01
     &,  -.9151119D-01,  -.2243756D-01,   .4691301D-01
     &,  -.3797067D-01,   .1055412D+00,   .4104481D-01
     &,  -.7694991D-01,  -.5002581D-01,   .1239998D+00
     &,  -.3761431D-01,  -.4293077D-01,   .1749644D+00
     &,  -.1416882D+00,   .6051788D-01,   .6277391D-01
     &,  -.1540998D-01,  -.1056401D+00,  -.1110286D+00
     &,   .1489907D+00,   .2058998D-01,  -.5149724D-01
     &,   .8381773D-01,  -.9144686D-01,  -.2063284D-01
     &,   .4238685D-01,  -.3657269D-01,   .9809533D-01
     &,   .3870846D-01,  -.7149191D-01,  -.5137234D-01
     &,   .1204541D+00,  -.2487733D-01,  -.5419608D-01
     &,   .1585081D+00,  -.1193379D+00,   .6216076D-01
     &,   .6453257D-01,  -.1973095D-01,  -.7992221D-01
     &,  -.8420957D-01,   .1261518D+00,   .1876419D-01
     &,  -.4731273D-01,   .7899317D-01,  -.9055568D-01
     &,  -.1890838D-01,   .3823409D-01,  -.3490527D-01
     &,   .9083043D-01,   .3643274D-01,  -.6638607D-01
     &,  -.5211520D-01,   .1164965D+00,  -.1336326D-01
     &,  -.6380538D-01,   .1417380D+00,  -.9764620D-01
     &,   .6303651D-01,   .6548755D-01,  -.2329635D-01
     &,  -.5676491D-01,  -.6004801D-01,   .1049036D+00
     &,   .1709876D-01,  -.4336871D-01,   .7418978D-01
     &,  -.8894810D-01,  -.1727046D-01,   .3443294D-01
     &,  -.3319355D-01,   .8408995D-01,   .3422653D-01
     &,  -.6154914D-01,  -.5232728D-01,   .1122115D+00
     &,  -.3064412D-02,  -.7183995D-01,   .1249735D+00
     &,  -.7684926D-01,   .6318044D-01,   .6567642D-01
     &,  -.2604723D-01,  -.3610404D-01,  -.3848008D-01
     &,   .8527625D-01,   .1556902D-01,  -.3965887D-01
     &,   .6941254D-01,  -.8673124D-01,  -.1572329D-01
     &,   .3096172D-01,  -.3132020D-01,   .7756950D-01
     &,   .3209691D-01,  -.5700808D-01,  -.5207766D-01
     &,   .1076760D+00,   .6043820D-02,  -.7839241D-01
     &,   .1084631D+00,  -.5714216D-01,   .6274472D-01
     &,   .6525777D-01,  -.2824049D-01,  -.1781922D-01
     &,  -.1937925D-01,   .6731197D-01,   .1419877D-01
     &,  -.3620489D-01,   .6473093D-01,  -.8400742D-01
     &,  -.1426936D-01,   .2779904D-01,  -.2938526D-01
     &,   .7139961D-01,   .3004928D-01,  -.5275180D-01
     &,  -.5143123D-01,   .1029597D+00,   .1400023D-01
     &,  -.8356372D-01,   .9242092D-01,  -.3868022D-01
     &,   .6180547D-01,   .6431096D-01,  -.2973883D-01
     &,  -.1810780D-02,  -.2646437D-02,   .5100378D-01
     &,   .1295262D-01,  -.3297473D-01/
         data (D1(i),i=1741,2030)/
     &   .6017975D-01,  -.8079325D-01,  -.1290965D-01
     &,   .2492403D-01,  -.2742793D-01,   .6559135D-01
     &,   .2808759D-01,  -.4876874D-01,  -.5044838D-01
     &,   .9812516D-01,   .2085515D-01,  -.8746058D-01
     &,   .7702658D-01,  -.2158080D-01,   .6038817D-01
     &,   .6286361D-01,  -.3078330D-01,   .1220080D-01
     &,   .1200736D-01,   .3627446D-01,   .1181996D-01
     &,  -.3001542D-01,   .5580370D-01,  -.7734707D-01
     &,  -.1165938D-01,   .2236439D-01,  -.2548164D-01
     &,   .6015019D-01,   .2621448D-01,  -.4504274D-01
     &,  -.4918495D-01,   .9322789D-01,   .2666798D-01
     &,  -.9017106D-01,   .6242643D-01,  -.5925731D-02
     &,   .5868004D-01,   .6110919D-01,  -.3131228D-01
     &,   .2413427D-01,   .2449853D-01,   .2308862D-01
     &,   .1079090D-01,  -.2733249D-01,   .5159892D-01
     &,  -.7366971D-01,  -.1049173D-01,   .2000737D-01
     &,  -.2357441D-01,   .5507674D-01,   .2442734D-01
     &,  -.4157071D-01,  -.4769204D-01,   .8831675D-01
     &,   .3150489D-01,  -.9146771D-01,   .4873488D-01
     &,   .8213944D-02,   .5670476D-01,   .5907323D-01
     &,  -.3155966D-01,   .3419281D-01,   .3503692D-01
     &,   .1143543D-01,   .9812541D-02,  -.2481709D-01
     &,   .4758982D-01,  -.6983460D-01,  -.9414491D-02
     &,   .1787913D-01,  -.2170723D-01,   .5034079D-01
     &,   .2273531D-01,  -.3829396D-01,  -.4601615D-01
     &,   .8343425D-01,   .3543678D-01,  -.9258926D-01
     &,   .3603643D-01,   .2086389D-01,   .5452404D-01
     &,   .5682021D-01,  -.3150489D-01,   .4249758D-01
     &,   .4374775D-01,   .1576681D-02,   .8954256D-02
     &,  -.2255810D-01,   .4378746D-01,  -.6591610D-01
     &,  -.8453066D-02,   .1601818D-01,  -.1994318D-01
     &,   .4599026D-01,   .2109251D-01,  -.3531342D-01
     &,  -.4419916D-01,   .7861698D-01,   .3853741D-01
     &,  -.9249251D-01,   .2438805D-01,   .3200739D-01
     &,   .5222414D-01,   .5444010D-01,  -.3101534D-01
     &,   .4923950D-01,   .5037078D-01,  -.7167567D-02
     &,   .8174681D-02,  -.2044591D-01,   .4019855D-01
     &,  -.6192694D-01,  -.7552908D-02,   .1429679D-01
     &,  -.1827422D-01,   .4198644D-01,   .1960896D-01
     &,  -.3254614D-01,  -.4227854D-01,   .7393027D-01
     &,   .4085844D-01,  -.9166591D-01,   .1378376D-01
     &,   .4170182D-01,   .4981517D-01,   .5194358D-01
     &,  -.3046741D-01,   .5454048D-01,   .5591868D-01
     &,  -.1463188D-01,   .7429408D-02,  -.1851264D-01
     &,   .3682609D-01,  -.5798850D-01,  -.6733292D-02
     &,   .1275306D-01,  -.1671028D-01,   .3831603D-01
     &,   .1821331D-01,  -.2993395D-01,  -.4028746D-01
     &,   .6929708D-01,   .4252217D-01,  -.9020947D-01
     &,   .4319648D-02,   .4992849D-01,   .4728633D-01
     &,   .4932019D-01,  -.2960553D-01,   .5854029D-01
     &,   .6012740D-01,  -.2090704D-01,   .6782993D-02
     &,  -.1674655D-01,   .3366994D-01,  -.5407663D-01
     &,  -.5990340D-02,   .1137197D-01,  -.1525779D-01
     &,   .3496357D-01,   .1685752D-01,  -.2755089D-01
     &,  -.3825509D-01,   .6484151D-01,   .4357807D-01
     &,  -.8820870D-01,  -.4067526D-02,   .5681687D-01
     &,   .4472887D-01,   .4666479D-01,  -.2873908D-01
     &,   .6137359D-01,   .6313677D-01,  -.2608483D-01
     &,   .6195799D-02,  -.1513616D-01,   .3072737D-01
     &,  -.5025995D-01,  -.5319989D-02,   .1013942D-01
     &,  -.1392004D-01,   .3191197D-01,   .1560122D-01
     &,  -.2532070D-01,  -.3620673D-01,   .6054612D-01
     &,   .4409735D-01,  -.8578574D-01,  -.1141927D-01
     &,   .6238970D-01,   .4226604D-01,   .4410542D-01
     &,  -.2771547D-01,   .6316882D-01,   .6507959D-01
     &,  -.3025641D-01,   .5662254D-02,  -.1367034D-01
     &,   .2799350D-01,  -.4652351D-01,  -.4718064D-02
     &,   .9042191D-02,  -.1266093D-01,   .2916274D-01
     &,   .1442489D-01,  -.2325685D-01,  -.3416416D-01
     &,   .5642393D-01,   .4415358D-01,  -.8298568D-01
     &,  -.1770920D-01,   .6678606D-01,   .3974753D-01
     &,   .4148698D-01,  -.2655520D-01,   .6404752D-01
     &,   .6608089D-01,  -.3351100D-01,   .5177261D-02
     &,  -.1238466D-01,   .2546183D-01,  -.4305873D-01
     &,  -.4180334D-02,   .8067965D-02,  -.1155240D-01
     &,   .2665687D-01,   .1332465D-01,  -.2134969D-01
     &,  -.3214585D-01,   .5248444D-01,   .4381516D-01
     &,  -.7987853D-01,  -.2306778D-01,   .7006362D-01
     &,   .3727316D-01,   .3891298D-01,  -.2533441D-01
     &,   .6423658D-01,   .6637398D-01,  -.3589783D-01
     &,   .4736168D-02,  -.1117766D-01,   .2312456D-01
     &,  -.3967034D-01,  -.3702552D-02,   .7205248D-02
     &,  -.1055368D-01,   .2439470D-01,   .1229800D-01
     &,  -.1958893D-01,  -.3016725D-01,   .4873411D-01
     &,   .4307861D-01,  -.7657094D-01,  -.2750799D-01
     &,   .7233199D-01,   .3485993D-01,   .3640128D-01
     &,  -.2407534D-01,   .6350392D-01,   .6571785D-01
     &,  -.3761099D-01,   .4334737D-02,  -.1008407D-01
     &,   .2097294D-01,  -.3645273D-01,  -.3280496D-02
     &,   .6443348D-02,  -.9659540D-02,   .2235648D-01
     &,   .1134148D-01,  -.1796483D-01,  -.2824103D-01
     &,   .4517666D-01,   .4210135D-01,  -.7310204D-01
     &,  -.3110411D-01,   .7373436D-01,   .3252127D-01
     &,   .3396606D-01,  -.2273541D-01,   .6231154D-01
     &,   .6456209D-01,  -.3861791D-01,   .3922874D-02
     &,  -.9094691D-02,   .1899760D-01,  -.3338037D-01
     &,  -.2909993D-02,   .5772333D-02/
         data (D1(i),i=2031,22*fist_np)/
     &  -.8863459D-02,   .2052264D-01,   .1045159D-01
     &,  -.1648820D-01,  -.2637741D-01,   .4181346D-01
     &,   .4088701D-01,  -.6952635D-01,  -.3392491D-01
     &,   .7428930D-01,   .3026744D-01,   .3161816D-01
     &,  -.2146068D-01,   .6056565D-01,   .6288227D-01
     &,  -.3903023D-01,   .3589473D-02,  -.8200843D-02
     &,   .1718876D-01,  -.3055117D-01,  -.2586945D-02
     &,   .5182991D-02,  -.8216367D-02,   .1892776D-01
     &,   .9624840D-02,  -.1510545D-01,  -.2458433D-01
     &,   .3864384D-01,   .3948218D-01,  -.6589199D-01
     &,  -.3608902D-01,   .7413019D-01,   .2817494D-01
     &,   .2943629D-01,  -.2026167D-01,   .5842191D-01
     &,   .6076695D-01,  -.3888230D-01,   .3285664D-02
     &,  -.7394402D-02,   .1553644D-01,  -.2790879D-01
     &,  -.2307346D-02,   .4666788D-02,  -.7535053D-02
     &,   .1739194D-01,   .8879796D-02,  -.1383281D-01
     &,  -.2286776D-01,   .3566537D-01,   .3792895D-01
     &,  -.6221232D-01,  -.3757697D-01,   .7335141D-01
     &,   .2611542D-01,   .2728905D-01,  -.1902302D-01
     &,   .5593361D-01,   .5824357D-01,  -.3831375D-01
     &,   .3008574D-02,  -.6667775D-02,   .1403064D-01
     &,  -.2544962D-01,  -.2067297D-02,   .4215829D-02
     &,  -.6986146D-02,   .1605874D-01,   .8166863D-02
     &,  -.1266232D-01,  -.2123193D-01,   .3287421D-01
     &,   .3626491D-01,  -.5857775D-01,  -.3850081D-01
     &,   .7204299D-01,   .2415165D-01,   .2524099D-01
     &,  -.1780760D-01,   .5328666D-01,   .5550566D-01
     &,  -.3728658D-01,   .2799620D-02,  -.6013893D-02
     &,   .1266143D-01,  -.2316788D-01,  -.1863016D-02
     &,   .3822823D-02,  -.6541060D-02,   .1484254D-01
     &,   .7506699D-02,  -.1158651D-01,  -.1967951D-01
     &,   .3026529D-01,   .3452339D-01,  -.5499462D-01
     &,  -.3892906D-01,   .7028974D-01,   .2228473D-01
     &,   .2329339D-01,  -.1662143D-01,   .5038667D-01
     &,   .5250241D-01,  -.3603674D-01,   .2566850D-02
     &,  -.5382609D-02,   .1141909D-01,  -.2105623D-01
     &,  -.1638112D-02,   .3442072D-02,  -.6115171D-02
     &,   .1375868D-01,   .6899662D-02,  -.1060199D-01
     &,  -.1821186D-01,   .2783257D-01,   .3273365D-01
     &,  -.5148875D-01,  -.3892729D-01,   .6817057D-01
     &,   .2051450D-01,   .2144618D-01,  -.1546906D-01
     &,   .4732540D-01,   .4932892D-01,  -.3444659D-01
     &,   .2353384D-02,  -.4857294D-02,   .1022822D-01
     &,  -.1910633D-01,  -.1453138D-02,   .3121810D-02
     &,  -.5737913D-02,   .1277664D-01,   .6335178D-02
     &,  -.9749754D-02,  -.1682920D-01,   .2556927D-01
     &,   .3092112D-01,  -.4815032D-01,  -.3855749D-01
     &,   .6575806D-01,   .1883981D-01,   .1969826D-01
     &,  -.1435381D-01,   .4415352D-01,   .4603821D-01
     &,  -.3262340D-01,   .2157355D-02,  -.4386838D-02
     &,   .9215069D-02,  -.1730934D-01,  -.1340227D-02
     &,   .2873130D-02,  -.5401385D-02,   .1188314D-01
     &,   .5863385D-02,  -.8889877D-02,  -.1553079D-01
     &,   .2346800D-01,   .2910764D-01,  -.4478949D-01
     &,  -.3787749D-01,   .6311820D-01,   .1722472D-01
     &,   .1801288D-01,  -.1327802D-01,   .4091712D-01
     &,   .4267849D-01,  -.3061491D-01,   .1977090D-02
     &,  -.3966050D-02,   .8360176D-02,  -.1558422D-01
     &,  -.1250131D-02,   .2658936D-02,  -.5098242D-02
     &,   .1106617D-01,   .5355382D-02,  -.8098092D-02
     &,  -.1431509D-01,   .2152101D-01,   .2731181D-01
     &,  -.4162652D-01,  -.3694063D-01,   .6031031D-01
     &,   .1575168D-01,   .1647452D-01,  -.1221709D-01
     &,   .3765811D-01,   .3929361D-01,  -.2846590D-01
     &,   .1843710D-02,  -.3590151D-02,   .7534000D-02
     &,  -.1408261D-01,  -.1179731D-02,   .2474389D-02
     &,  -.4821809D-02,   .1031507D-01,   .4901349D-02
     &,  -.7370029D-02,  -.1317993D-01,   .1972024D-01
     &,   .2554918D-01,  -.3860246D-01,  -.3576012D-01
     &,   .5735781D-01,   .1436777D-01,   .1502898D-01
     &,  -.1124224D-01,   .3441455D-01,   .3592337D-01
     &,  -.2621832D-01,   .1687094D-02,  -.3254754D-02
     &,   .6790922D-02,  -.1274764D-01,  -.1126072D-02
     &,   .2315081D-02,  -.4566168D-02,   .9620525D-02
     &,   .4461206D-02,  -.6756165D-02,  -.1212258D-01
     &,   .1805752D-01,   .2383257D-01,  -.3574865D-01
     &,  -.3445863D-01,   .5436802D-01,   .1306879D-01
     &,   .1367194D-01,  -.1031009D-01,   .3101173D-01
     &,   .3238866D-01,  -.2394499D-01/
         data (f1(i),i=1,290)/.0D+00,.000D+00,.000D+00
     &,   .0000000D+00,   .0000000D+00,   .0000000D+00
     &,   .0000000D+00,   .0000000D+00,   .2299047D-01
     &,   .5052282D-01,   .4310491D-01,   .2202982D-01
     &,   .5287433D-01,   .2074739D-04,  -.4557281D-01
     &,  -.3674058D-04,   .4590143D-01,   .1006349D+00
     &,   .8541701D-01,   .4340650D-01,   .1053154D+00
     &,   .1654922D-03,  -.9033733D-01,  -.2924430D-03
     &,   .6855378D-01,   .1497193D+00,   .1262548D+00
     &,   .6350092D-01,   .1568919D+00,   .5557260D-03
     &,  -.1335999D+00,  -.9792640D-03,   .9080988D-01
     &,   .1973755D+00,   .1649307D+00,   .8166517D-01
     &,   .2071864D+00,   .1308009D-02,  -.1746183D+00
     &,  -.2295782D-02,   .1125813D+00,   .2432378D+00
     &,   .2007143D+00,   .9743601D-01,   .2557323D+00
     &,   .2531039D-02,  -.2127147D+00,  -.4420805D-02
     &,   .1338140D+00,   .2868489D+00,   .2330866D+00
     &,   .1103303D+00,   .3022168D+00,   .4324992D-02
     &,  -.2473089D+00,  -.7507988D-02,   .1543657D+00
     &,   .3277195D+00,   .2614176D+00,   .1199830D+00
     &,   .3463280D+00,   .6778525D-02,  -.2777794D+00
     &,  -.1167992D-01,   .1741384D+00,   .3657032D+00
     &,   .2853031D+00,   .1261317D+00,   .3876996D+00
     &,   .9966612D-02,  -.3036899D+00,  -.1702542D-01
     &,   .1930410D+00,   .4003996D+00,   .3043839D+00
     &,   .1286239D+00,   .4260502D+00,   .1395267D-01
     &,  -.3246597D+00,  -.2359580D-01,   .2109898D+00
     &,   .4315441D+00,   .3183970D+00,   .1274207D+00
     &,   .4611104D+00,   .1877682D-01,  -.3404030D+00
     &,  -.3140312D-01,   .2278610D+00,   .4589177D+00
     &,   .3271789D+00,   .1225972D+00,   .4926767D+00
     &,   .2446937D-01,  -.3507325D+00,  -.4041942D-01
     &,   .2436247D+00,   .4823491D+00,   .3307172D+00
     &,   .1143384D+00,   .5205783D+00,   .3104151D-01
     &,  -.3555614D+00,  -.5057716D-01,   .2582879D+00
     &,   .5017160D+00,   .3290119D+00,   .1029040D+00
     &,   .5446327D+00,   .3848511D-01,  -.3549029D+00
     &,  -.6177066D-01,   .2717495D+00,   .5169454D+00
     &,   .3221341D+00,   .8875223D-01,   .5648609D+00
     &,   .4678095D-01,  -.3488675D+00,  -.7385878D-01
     &,   .2839708D+00,   .5279506D+00,   .3103727D+00
     &,   .7232102D-01,   .5811698D+00,   .5588789D-01
     &,  -.3377883D+00,  -.8668169D-01,   .2949227D+00
     &,   .5348236D+00,   .2940154D+00,   .5402303D-01
     &,   .5935596D+00,   .6575090D-01,  -.3216972D+00
     &,  -.1000141D+00,   .3045859D+00,   .5376817D+00
     &,   .2734716D+00,   .3450682D-01,   .6020716D+00
     &,   .7630046D-01,  -.3010875D+00,  -.1136443D+00
     &,   .3129504D+00,   .5365858D+00,   .2491973D+00
     &,   .1437229D-01,   .6067857D+00,   .8745400D-01
     &,  -.2763943D+00,  -.1273325D+00,   .3200157D+00
     &,   .5316900D+00,   .2217308D+00,  -.5765121D-02
     &,   .6078182D+00,   .9911748D-01,  -.2480667D+00
     &,  -.1408375D+00,   .3257903D+00,   .5231850D+00
     &,   .1915717D+00,  -.2532722D-01,   .6053188D+00
     &,   .1111872D+00,  -.2167249D+00,  -.1538854D+00
     &,   .3302912D+00,   .5112946D+00,   .1593240D+00
     &,  -.4374750D-01,   .5994674D+00,   .1235517D+00
     &,  -.1829399D+00,  -.1662375D+00,   .3335435D+00
     &,   .4962711D+00,   .1255866D+00,  -.6050352D-01
     &,   .5904700D+00,   .1360938D+00,  -.1472274D+00
     &,  -.1776397D+00,   .3355800D+00,   .4783905D+00
     &,   .9096207D-01,  -.7513136D-01,   .5785558D+00
     &,   .1486927D+00,  -.1102267D+00,  -.1878653D+00
     &,   .3364403D+00,   .4579482D+00,   .5604519D-01
     &,  -.8723779D-01,   .5639721D+00,   .1612261D+00
     &,  -.7253604D-01,  -.1967023D+00,   .3361705D+00
     &,   .4352539D+00,   .2139605D-01,  -.9650973D-01
     &,   .5469812D+00,   .1735720D+00,  -.3472519D-01
     &,  -.2039587D+00,   .3348222D+00,   .4106266D+00
     &,  -.1240307D-01,  -.1027713D+00,   .5278556D+00
     &,   .1856111D+00,   .2612390D-02,  -.2094772D+00
     &,   .3324520D+00,   .3843904D+00,  -.4486886D-01
     &,  -.1057778D+00,   .5068745D+00,   .1972281D+00
     &,   .3897255D-01,  -.2131319D+00,   .3291207D+00
     &,   .3568694D+00,  -.7554371D-01,  -.1055142D+00
     &,   .4843197D+00,   .2083141D+00,   .7387338D-01
     &,  -.2148283D+00,   .3248925D+00,   .3283840D+00
     &,  -.1040239D+00,  -.1021096D+00,   .4604719D+00
     &,   .2187673D+00,   .1069171D+00,  -.2144960D+00
     &,   .3198344D+00,   .2992463D+00,  -.1299645D+00
     &,  -.9560323D-01,   .4356073D+00,   .2284952D+00
     &,   .1376537D+00,  -.2121323D+00,   .3140151D+00
     &,   .2697571D+00,  -.1530830D+00,  -.8625987D-01
     &,   .4099947D+00,   .2374151D+00,   .1657990D+00
     &,  -.2077390D+00,   .3075049D+00,   .2402024D+00
     &,  -.1731607D+00,  -.7433615D-01,   .3838921D+00
     &,   .2454553D+00,   .1910932D+00,  -.2013624D+00
     &,   .3003747D+00,   .2108505D+00,  -.1900437D+00
     &,  -.6016732D-01,   .3575448D+00,   .2525557D+00
     &,   .2133380D+00,  -.1930815D+00,   .2926951D+00
     &,   .1820002D+00,  -.2036414D+00,  -.4413531D-01
     &,   .3311829D+00,   .2586684D+00,   .2323966D+00
     &,  -.1830053D+00,   .2845363D+00,   .1538332D+00
     &,  -.2139246D+00,  -.2665615D-01,   .3051091D+00
     &,   .2637619D+00,   .2481916D+00,  -.1712702D+00
     &,   .2759673D+00,   .1264873D+00/
         data (f1(i),i=291,580)/-.2209218D+00,
     &    -.8167226D-02,   .2793370D+00
     &,   .2678083D+00,   .2607028D+00,  -.1580361D+00
     &,   .2670553D+00,   .1002025D+00,  -.2247673D+00
     &,   .1090766D-01,   .2541827D+00,   .2708252D+00
     &,   .2699633D+00,  -.1434825D+00,   .2579166D+00
     &,   .7513354D-01,  -.2255275D+00,   .3011961D-01
     &,   .2297007D+00,   .2727574D+00,   .2760549D+00
     &,  -.1278046D+00,   .2485509D+00,   .5141066D-01
     &,  -.2233390D+00,   .4898190D-01,   .2060788D+00
     &,   .2736435D+00,   .2791036D+00,  -.1112090D+00
     &,   .2389828D+00,   .2913940D-01,  -.2184579D+00
     &,   .6708283D-01,   .1834328D+00,   .2735056D+00
     &,   .2792732D+00,  -.9390930D-01,   .2293152D+00
     &,   .8401435D-02,  -.2111225D+00,   .8420881D-01
     &,   .1618581D+00,   .2723755D+00,   .2767600D+00
     &,  -.7612193D-01,   .2196012D+00,  -.1074458D-01
     &,  -.2015943D+00,   .9986749D-01,   .1414312D+00
     &,   .2702934D+00,   .2717864D+00,  -.5806228D-01
     &,   .2098903D+00,  -.2821551D-01,  -.1901512D+00
     &,   .1138670D+00,   .1222098D+00,   .2673076D+00
     &,   .2645952D+00,  -.3994081D-01,   .2002282D+00
     &,  -.4404858D-01,  -.1770812D+00,   .1259585D+00
     &,   .1042341D+00,   .2634728D+00,   .2554431D+00
     &,  -.2195966D-01,   .1906567D+00,  -.5829010D-01
     &,  -.1626763D+00,   .1359686D+00,   .8752799D-01
     &,   .2588494D+00,   .2445956D+00,  -.4309579D-02
     &,   .1812138D+00,  -.7090880D-01,  -.1472643D+00
     &,   .1437788D+00,   .7210000D-01,   .2535021D+00
     &,   .2323210D+00,   .1283268D-01,   .1719654D+00
     &,  -.8193973D-01,  -.1310776D+00,   .1493242D+00
     &,   .5794499D-01,   .2474992D+00,   .2188861D+00
     &,   .2930654D-01,   .1628954D+00,  -.9143147D-01
     &,  -.1143660D+00,   .1525595D+00,   .4504543D-01
     &,   .2409109D+00,   .2045512D+00,   .4496932D-01
     &,   .1540139D+00,  -.9941677D-01,  -.9742457D-01
     &,   .1535830D+00,   .3337289D-01,   .2338092D+00
     &,   .1895668D+00,   .5969743D-01,   .1454010D+00
     &,  -.1060163D+00,  -.8049289D-01,   .1524467D+00
     &,   .2288952D-01,   .2262660D+00,   .1741752D+00
     &,   .7344308D-01,   .1370108D+00,  -.1113105D+00
     &,  -.6380066D-01,   .1493018D+00,   .1354948D-01
     &,   .2183528D+00,   .1585796D+00,   .8601275D-01
     &,   .1288998D+00,  -.1153614D+00,  -.4750870D-01
     &,   .1442774D+00,   .5300375D-02,   .2101397D+00
     &,   .1429945D+00,   .9739683D-01,   .1210469D+00
     &,  -.1182695D+00,  -.3181397D-01,   .1374539D+00
     &,  -.1915430D-02,   .2016947D+00,   .1276024D+00
     &,   .1075517D+00,   .1135514D+00,  -.1200649D+00
     &,  -.1686400D-01,   .1291655D+00,  -.8159504D-02
     &,   .1930830D+00,   .1125649D+00,   .1164525D+00
     &,   .1064145D+00,  -.1208740D+00,  -.2781550D-02
     &,   .1195846D+00,  -.1349618D-01,   .1843666D+00
     &,   .9802212D-01,   .1240925D+00,   .9953720D-01
     &,  -.1208299D+00,   .1028491D-01,   .1089368D+00
     &,  -.1799143D-01,   .1756038D+00,   .8409260D-01
     &,   .1304813D+00,   .9297554D-01,  -.1199978D+00
     &,   .2235070D-01,   .9745104D-01,  -.2171195D-01
     &,   .1668487D+00,   .7087382D-01,   .1356435D+00
     &,   .8672964D-01,  -.1184152D+00,   .3328826D-01
     &,   .8535449D-01,  -.2472417D-01,   .1581512D+00
     &,   .5844283D-01,   .1396173D+00,   .8079769D-01
     &,  -.1162692D+00,   .4313461D-01,   .7286786D-01
     &,  -.2709358D-01,   .1495566D+00,   .4685723D-01
     &,   .1424522D+00,   .7517616D-01,  -.1135979D+00
     &,   .5184603D-01,   .6020134D-01,  -.2888402D-01
     &,   .1411056D+00,   .3615655D-01,   .1442080D+00
     &,   .6985998D-01,  -.1104814D+00,   .5942786D-01
     &,   .4758412D-01,  -.3015710D-01,   .1328343D+00
     &,   .2636362D-01,   .1449520D+00,   .6489065D-01
     &,  -.1069948D+00,   .6590083D-01,   .3511471D-01
     &,  -.3097179D-01,   .1247743D+00,   .1748621D-01
     &,   .1447583D+00,   .6016732D-01,  -.1032084D+00
     &,   .7125063D-01,   .2297515D-01,  -.3138403D-01
     &,   .1169524D+00,   .9518720D-02,   .1437053D+00
     &,   .5572651D-01,  -.9918732D-01,   .7561713D-01
     &,   .1133498D-01,  -.3144644D-01,   .1093914D+00
     &,   .2443864D-02,   .1418745D+00,   .5155896D-01
     &,  -.9499117D-01,   .7900871D-01,   .4140516D-03
     &,  -.3120817D-01,   .1021096D+00,  -.3765583D-02
     &,   .1393489D+00,   .4765476D-01,  -.9067421D-01
     &,   .8148696D-01,  -.9835322D-02,  -.3071473D-01
     &,   .9512173D-01,  -.9145121D-02,   .1362116D+00
     &,   .4400351D-01,  -.8628528D-01,   .8311879D-01
     &,  -.1933267D-01,  -.3000801D-01,   .8843851D-01
     &,  -.1373692D-01,   .1325447D+00,   .4059444D-01
     &,  -.8186797D-01,   .8397476D-01,  -.2796122D-01
     &,  -.2917858D-01,   .8211638D-01,  -.1758831D-01
     &,   .1284282D+00,   .3741658D-01,  -.7746072D-01
     &,   .8412751D-01,  -.3570340D-01,  -.2815731D-01
     &,   .7605919D-01,  -.2075038D-01,   .1239392D+00
     &,   .3445879D-01,  -.7309706D-01,   .8365034D-01
     &,  -.4253957D-01,  -.2697225D-01,   .7027605D-01
     &,  -.2327671D-01,   .1191510D+00,   .3170994D-01
     &,  -.6880588D-01,   .8261603D-01,  -.4848927D-01
     &,  -.2575894D-01,   .6485618D-01,  -.2522224D-01
     &,   .1141326D+00,   .2915893D-01,  -.6461166D-01
     &,   .8109576D-01,  -.5352471D-01/
         data (f1(i),i=581,8*fist_np)/
     &  -.2448850D-01,   .5974985D-01,  -.2664227D-01
     &,   .1089483D+00,   .2679478D-01,  -.6053480D-01
     &,   .7915824D-01,  -.5768419D-01,  -.2318240D-01
     &,   .5495180D-01,  -.2759161D-01,   .1036573D+00
     &,   .2460673D-01,  -.5659192D-01,   .7686903D-01
     &,  -.6095833D-01,  -.2195654D-01,   .5050781D-01
     &,  -.2812385D-01,   .9831373D-01,   .2258424D-01
     &,  -.5279619D-01,   .7428995D-01,  -.6343969D-01
     &,  -.2062894D-01,   .4629660D-01,  -.2829075D-01
     &,   .9296628D-01,   .2067161D-01,  -.4915765D-01
     &,   .7147872D-01,  -.6525973D-01,  -.1931326D-01
     &,   .4236861D-01,  -.2814738D-01,   .8766953D-01
     &,   .1895263D-01,  -.4568349D-01,   .6852959D-01
     &,  -.6637575D-01,  -.1802088D-01,   .3871336D-01
     &,  -.2772731D-01,   .8243681D-01,   .1736978D-01
     &,  -.4237843D-01,   .6540655D-01,  -.6680263D-01
     &,  -.1676120D-01,   .3531966D-01,  -.2708192D-01
     &,   .7731515D-01,   .1591388D-01,  -.3924495D-01
     &,   .6219728D-01,  -.6669229D-01,  -.1554181D-01
     &,   .3217576D-01,  -.2625197D-01,   .7233293D-01
     &,   .1457614D-01,  -.3628362D-01,   .5897326D-01
     &,  -.6607740D-01,  -.1436869D-01,   .2926955D-01
     &,  -.2527496D-01,   .6751397D-01,   .1334820D-01
     &,  -.3349332D-01,   .5570191D-01,  -.6502025D-01
     &,  -.1324640D-01,   .2658875D-01,  -.2418508D-01
     &,   .6287773D-01,   .1222210D-01,  -.3091220D-01
     &,   .5244936D-01,  -.6356595D-01,  -.1217826D-01
     &,   .2412097D-01,  -.2301320D-01,   .5843967D-01
     &,   .1115911D-01,  -.2848411D-01,   .4924209D-01
     &,  -.6179544D-01,  -.1116650D-01,   .2185389D-01
     &,  -.2178693D-01,   .5421151D-01,   .1022051D-01
     &,  -.2617483D-01,   .4610260D-01,  -.5975946D-01
     &,  -.1021243D-01,   .1977536D-01,  -.2053065D-01
     &,   .5020155D-01,   .9362427D-02,  -.2402019D-01
     &,   .4304971D-01,  -.5754062D-01,  -.9351218D-02
     &,   .1784774D-01,  -.1926564D-01,   .4641494D-01
     &,   .8578503D-02,  -.2201424D-01,   .4009876D-01
     &,  -.5516851D-01,  -.8504841D-02,   .1610334D-01
     &,  -.1801019D-01,   .4285404D-01,   .7845753D-02
     &,  -.2015062D-01,   .3726192D-01,  -.5267193D-01
     &,  -.7698128D-02,   .1455342D-01,  -.1677974D-01
     &,   .3951863D-01,   .7200963D-02,  -.1842263D-01
     &,   .3455696D-01,  -.4999529D-01,  -.6973690D-02
     &,   .1311326D-01,  -.1558706D-01,   .3640628D-01
     &,   .6613481D-02,  -.1682408D-01,   .3196569D-01
     &,  -.4730232D-01,  -.6303820D-02,   .1180569D-01
     &,  -.1444237D-01,   .3351256D-01,   .6078406D-02
     &,  -.1533433D-01,   .2950894D-01,  -.4466195D-01
     &,  -.5686675D-02,   .1062084D-01,  -.1335358D-01
     &,   .3083133D-01,   .5591180D-02,  -.1395978D-01
     &,   .2718901D-01,  -.4198579D-01,  -.5120184D-02
     &,   .9549284D-02,  -.1232644D-01,   .2835500D-01
     &,   .5147569D-02,  -.1262362D-01,   .2500636D-01
     &,  -.3937630D-01,  -.4602107D-02,   .8582124D-02
     &,  -.1136476D-01,   .2607477D-01,   .4743653D-02
     &,  -.1154885D-01,   .2295990D-01,  -.3669787D-01
     &,  -.4130082D-02,   .7710945D-02,  -.1047058D-01
     &,   .2398089D-01,   .4345231D-02,  -.1053975D-01
     &,   .2108250D-01,  -.3431184D-01,  -.3701668D-02
     &,   .6927822D-02,  -.9644411D-02,   .2206284D-01
     &,   .3962829D-02,  -.9572567D-02,   .1930621D-01
     &,  -.3193210D-01,  -.3314374D-02,   .6225307D-02
     &,  -.8885444D-02,   .2030960D-01,   .3647444D-02
     &,  -.8687316D-02,   .1765538D-01,  -.2961155D-01
     &,  -.2965692D-02,   .5596419D-02,  -.8191732D-02
     &,   .1870982D-01,   .3359717D-02,  -.7878155D-02
     &,   .1612491D-01,  -.2736579D-01,  -.2653116D-02
     &,   .5034623D-02,  -.7560413D-02,   .1725202D-01
     &,   .3097097D-02,  -.7139522D-02,   .1470923D-01
     &,  -.2523063D-01,  -.2374160D-02,   .4533814D-02
     &,  -.6987904D-02,   .1592477D-01,   .2857248D-02
     &,  -.6466150D-02,   .1340245D-01,  -.2321208D-01
     &,  -.2126374D-02,   .4088299D-02,  -.6470093D-02
     &,   .1471686D-01,   .2638029D-02,  -.5902287D-02
     &,   .1219850D-01,  -.2141665D-01,  -.1907356D-02
     &,   .3692773D-02,  -.6002512D-02,   .1361740D-01
     &,   .2437488D-02,  -.5347077D-02,   .1109128D-01
     &,  -.1969395D-01,  -.1714762D-02,   .3342305D-02
     &,  -.5580504D-02,   .1261602D-01,   .2253841D-02
     &,  -.4842652D-02,   .1011019D-01,  -.1801384D-01
     &,  -.1546312D-02,   .3032315D-02,  -.5199370D-02
     &,   .1170286D-01/

         return
         end
      SUBROUTINE LOADFIST
      IMPLICIT NONE
C---------------------------------------------------------------------
C  Read data from {DS,FS,DD,FD,PDS,FDD}.EICHTEN into common block
C  /FIST/, arrays P, D, and F.
C
C  950722  Michael Kelsey -- Extracted from EICHTEN.F
C---------------------------------------------------------------------
      INCLUDE 'fist.inc'

C  Local variables
      INTEGER I                 !Loop variable

C  Data formats
 23     FORMAT((5D15.7))
 25     FORMAT((15X,4D15.7))
 27     FORMAT(2(3X,2(D15.7)))
 28     FORMAT((3X,3D15.11))

 233    FORMAT(5x,'&,',D15.7,',',D15.7,',',D15.7)


C  Initialization
        H=.3D-1
        DEM=5.D-3

        IPHFL=0
        IF(DEM.GT.0.0) IPHFL=1

C  Open and read data files, reporting each completion
cjc        OPEN(23,FILE='ds.eichten',STATUS='OLD')
cjc        READ(23,23) (P(I),D(1,I),D(2,I),D(3,I),D(4,I),I=1,FIST_NP)
cjc        CLOSE(23)
cjc        WRITE(6,*) 'UNIT 23 DATASET READ OK'
        
cjc        OPEN(24,FILE='fs.eichten',STATUS='OLD')
cjc        READ(24,23) (P(I),F(1,I),F(2,I),F(3,I),F(4,I),I=1,FIST_NP)
cjc        CLOSE(24)
cjc        WRITE(6,*) 'UNIT 24 DATASET READ OK'
        
cjc        OPEN(25,FILE='dd.eichten',STATUS='OLD')
cjc        READ(25,25) (D(5,I),D(6,I),D(7,I),D(8,I),I=1,FIST_NP)
cjc        CLOSE(25)
cjc        WRITE(6,*) 'UNIT 25 DATASET READ OK'
        
cjc        OPEN(26,FILE='fd.eichten',STATUS='OLD')
cjc        READ(26,25) (F(5,I),F(6,I),F(7,I),F(8,I),I=1,FIST_NP)
cjc        CLOSE(26)
cjc        WRITE(6,*) 'UNIT 26 DATASET READ OK'
        
cjc        OPEN(27,FILE='pds.eichten',STATUS='OLD')
cjc        READ(27,27) (D(9,I), D(10,I),D(11,I),D(12,I)
cjc     1              ,D(13,I),D(14,I),D(15,I),D(16,I),I=1,FIST_NP)
cjc        CLOSE(27)
cjc        WRITE(6,*) 'UNIT 27 DATASET READ OK'
        
cjc        OPEN(28,FILE='pdd.eichten',STATUS='OLD')
cjc        READ(28,28) (D(17,I),D(18,I),D(19,I),D(20,I),D(21,I),D(22,I)
cjc     1              ,I=1,FIST_NP)
cjc        CLOSE(28)
cjc        WRITE(6,*) 'UNIT 28 DATASET READ OK'

c        write(101,*) '************loadfist*****'
c        write(101,*) '        data p/'
c        write(101,233) p
cjc        write(101,*) '        data (D1(i),i=1,290)/'
cjc        write(101,233) (D1(i), i=1,290)
cjc        write(101,*) '        data (D1(i),i=291,580)/'
cjc        write(101,233) (D1(i), i=291,580)
cjc        write(101,*) '        data (D1(i),i=581,870)/'
cjc        write(101,233) (D1(i), i=581,870)
cjc        write(101,*) '        data (D1(i),i=871,1160)/'
cjc        write(101,233) (D1(i), i=871,1160)
cjc        write(101,*) '        data (D1(i),i=1161,1450)/'
cjc        write(101,233) (D1(i), i=1161,1450)
cjc        write(101,*) '        data (D1(i),i=1451,1740)/'
cjc        write(101,233) (D1(i), i=1451,1740)
cjc        write(101,*) '        data (D1(i),i=1741,2030)/'
cjc        write(101,233) (D1(i), i=1741,2030)
cjc        write(101,*) '        data (D1(i),i=2031,22*fist_np)/'
cjc        write(101,233) (D1(i), i=2031,22*fist_np)

cjc        write(101,*) '        data (f1(i),i=1,290)/'
cjc        write(101,233) (f1(i), i=1,290)
cjc        write(101,*) '        data (f1(i),i=291,580)/'
cjc        write(101,233) (f1(i), i=291,580)
cjc        write(101,*) '        data (f1(i),i=581,8*fist_np)/'
cjc        write(101,233) (f1(i), i=581,8*fist_np)

      RETURN
      END
      SUBROUTINE LOADGEN
      IMPLICIT NONE
C---------------------------------------------------------------------
C  Read data from MASSES.EICHTEN into common block /GEN/.
C
C  950722  Michael Kelsey -- Extracted from EICHTEN.F
C---------------------------------------------------------------------
      INCLUDE 'gen.inc'

C  External functions
      DOUBLE PRECISION EIGEN
      EXTERNAL EIGEN

C  Local variables
      DOUBLE PRECISION EP0, REP
      INTEGER I, J, JI          !Loop variable

C  Data formats
 10   FORMAT(5D15.8)


C  Open and read data file
cjc        OPEN(21,FILE='masses.eichten',STATUS='OLD')
cjc        READ(21,10,END=888,ERR=888) 
cjc     1       (CM(I),I=1,GEN_NCM),MASS,MP,MS,MC,A,ALPS
cjc        GOTO 666

C  Report error conditions
cjc888   CONTINUE
cjc        WRITE(6,*) 'READ ERROR ON UNIT 21'

C  Calculate derived quantities
cjc666   CONTINUE
cjc        CLOSE(21)
cjc        write(101,*) '********loadgen ******'
cjc
cjc        write(101,*) '       data cm/'
cjc        write(101,667) cm
cjc        write(101,*) '       data mass/'
cjc        write(101,667) mass
cjc        write(101,*) '       data mp,ms,mc,a,alps/'
cjc        write(101,667) MP,MS,MC,A,ALPS
cjc667     format(5x,'&,',D15.8,',',D15.8,',',D15.8)

        ALAM=(4.*ALPS/3.)*(MC*A)**(2.D+0/3.D+0)

        EP0=EIGEN(ALAM,1)/(A*(MC*A)**(1.D+0/3.D+0))
        DO J=1,GEN_ND
           JI=J+12
           REP=EIGEN(ALAM,JI)/(A*(MC*A)**(1.D+0/3.D+0))
           DCM(J)=REP+CM(1)-EP0
        ENDDO
      RETURN
      END
      SUBROUTINE LOADMIX
      IMPLICIT NONE
C---------------------------------------------------------------------
C  Read data from MIX.EICHTEN into mixing matrix MSD(n,n)
C
C  950722  Michael Kelsey -- Extracted from EICHTEN.F
C---------------------------------------------------------------------
      INCLUDE 'gen.inc'
      INCLUDE 'fist.inc'
      INCLUDE 'mix.inc'

C  Local Paramters
cjc      INTEGER N_SDM
cjc      PARAMETER (N_SDM = FIST_MIX*(FIST_MIX+1)/2)

C  Local variables
cjc      DOUBLE PRECISION SDM(N_SDM)      !Lower diagonal matrix data
      INTEGER NEWS              !Index variable for matrix calculations
      INTEGER I, J, K           !Loop variable


C  Initialization
        DO I=1, FIST_MIX
          DO J=1, FIST_MIX
            MSD(I,J) = 0D0
          ENDDO
        ENDDO

C  Open and read data file
cjc        OPEN(22,FILE='mix.eichten',STATUS='OLD')
cjc        READ(22,14) (SDM(J),J=1,N_SDM)
cjc 14     FORMAT(17X,D8.1)
cjc        CLOSE(22)
cjc        WRITE(6,*) 'UNIT 22 DATASET READ OK'

cjc        write(101,*) '********loadmix*****'
cjc        write(101,*) '       data SDM/'
cjc        write(101,667) sdm
cjc667     format(5x,'&,',D9.2,',',D9.2,',',D9.2,',',D9.2,',',D9.2)

C  Calculate mixing matrix elements
        NS1 = FIST_MIX - GEN_ND

        DO I = 1,FIST_MIX
           MSD(I,I)=CM(I)
           IF (I.GT.NS1) MSD(I,I)=DCM(I-NS1)
        ENDDO

        NEWS=0
        DO I = 1,FIST_MIX
           IF (I.GT.1) NEWS = NEWS+(FIST_MIX+2-I)
           DO J = I,FIST_MIX
              K = (J+1-I)+NEWS
              MSD(I,J) = MSD(I,J)+SDM(K)
              MSD(J,I) = MSD(I,J)
           ENDDO
        ENDDO
      RETURN
      END
      SUBROUTINE MINV(AI,N,B,C,D,R,H)
      IMPLICIT NONE
      INTEGER N
      COMPLEX AI(N,N),B(N),C(N),D,H
      REAL R
C----------------------------------------------------------------------
C  Complex matrix inversion
C
C  960109  Change arithmetic-IF to get around Linux f2c bug w/CABS
C----------------------------------------------------------------------

C  Local Variables
      REAL TOL,Q
      COMPLEX A(20,20),BIGA,G,SAVE
      INTEGER I,IB,IC,IMAX,IR,IX,IXMIN,JXMIN,J,JMAX,JX,K,L


C  Initialization
      DO 5 IR=1,N
      DO 5 IC=1,N
    5 A(IR,IC)=AI(IR,IC)
      D=CMPLX(1.0,0.0)
      DO 10 I=1,N
   10 C(I)=B(I)
      TOL=1.E-6
      DO 65 J=1,N
      BIGA=A(J,J)
      DO 30 I=J,N
C*** Linux f2c has CABS() bug -- below difference is never zero!
      IF(ABS(CABS(BIGA)-CABS(A(I,J)))-TOL) 20,20,30
   20 BIGA=A(I,J)
      IMAX=I
   30 CONTINUE
      Q=CABS(BIGA)-TOL
      IF(Q) 35,35,40
   35 WRITE(6,36)
   36 FORMAT(T20,'MATRIX IS SINGULAR')
      RETURN
   40 D=D*BIGA
      IF(IMAX.NE.J) D=-D
      DO 45 L=J,N
      SAVE=A(J,L)
      A(J, L)=A(IMAX,L)
   45 A(IMAX,L)=SAVE
      DO 50 K=J,N
   50 A(J,K)=A(J,K)/BIGA
      SAVE=C(IMAX)
      C(IMAX)=C(J)
      C(J)=SAVE/BIGA
      IF(J-N) 55,70,55
   55 CONTINUE
      IXMIN=J+1
      DO 65 IX=IXMIN,N
      JXMIN=J+1
      DO 60 JX=JXMIN,N
   60 A(IX,JX)=A(IX,JX)-A(IX,J)*A(J,JX)
   65 C(IX)=C(IX)-C(J)*A(IX,J)
   70 CONTINUE
      IF(N.EQ.1) GO TO 85
      JMAX=N-1
      DO 80 J=1,JMAX
      IB=N-J
      DO 80 K=1,J
      IC=N+1-K
      C(IB)=C(IB)-A(N-J,IC)*C(IC)
   80 CONTINUE
   85 G=CMPLX(0.0,0.0)
      DO 90 I=1,N
   90 G=G+C(I)*B(I)
      R=AIMAG(G)
      H=D*G
      RETURN
      END
      SUBROUTINE PHASE1(WE,PS,IMAX,J,IFLG)
      IMPLICIT NONE
      DOUBLE PRECISION WE(*)
      REAL PS(2,2,*)
      INTEGER IMAX,J,IFLG

      INCLUDE 'gen.inc'

C  Useful Parameters
      REAL PI
      PARAMETER (PI = 3.1415926535)

C  Local Variables
      COMPLEX DLTJ
      INTEGER I,L1,L2
      REAL MD,A1,A2,ANG,CONST,DEM1,DLTI,DLTR,EW,PANG,PMOD
      REAL PW,PW1,PW2,SAVE
      DOUBLE PRECISION EKIN


C  Initialization
 10     FORMAT(/,T30,'S-WAVE PHASE SHIFT FOR THE D-D SCATTERING AMPLITUD
     1E',/)
 20     FORMAT(/,T30,'P-WAVE PHASE SHIFT FOR THE D-D SCATTERING AMPLITUD
     1E',/)
 30     FORMAT(/,T30,'D-WAVE PHASE SHIFT FOR THE D-D SCATTERING AMPLITUD
     1E',/)
        IF(J.EQ.0) WRITE(55,10)
        IF(J.EQ.1) WRITE(55,20)
        IF(J.EQ.2) WRITE(55,30)

 40     FORMAT(T10,'ENERGY',T23,'MOMENTUM',T45,'PHASE',T70,'MODULUS'
     1  ,T85,'DLTR/PI',T100,'S-MATRIX')
C        WRITE(55,40)

        DEM1=.0051
        IF(J.EQ.0) CONST=1./(4.*PI)
        IF(J.EQ.1) CONST=1./(12.*PI)
        IF(J.EQ.2) CONST=9./(10.*PI)
        IF(IFLG.EQ.0) CONST=2.*CONST

        DO 60 I=1,IMAX
           EW=REAL(WE(I))
           PW1=REAL(DSQRT(WE(I)**2-4.*MASS(1,1)**2)/2.)

           IF(IFLG.EQ.0) THEN
              A1=1.-CONST*PW1*EW*PS(1,2,I)
              A2=CONST*PW1*EW*PS(1,1,I)
           ELSE
              EKIN=WE(I)**2-4.*(MASS(1,1)+DEM1)**2
              PW2=0.0
              IF(EKIN.GT.0.D+0) PW2=REAL(DSQRT(EKIN)/2.)
              A1=1.-CONST*EW*(PW1*PS(1,2,I)+PW2*PS(2,2,I))
              A2=CONST*EW*(PW1*PS(1,1,I)+PW2*PS(2,1,I))
           ENDIF

           MD=SQRT(A1**2+A2**2)
           DLTI=-(1./2.)*ALOG(MD)
           L1=SIGN(1.,A1)
           L2=SIGN(1.,A2)
           ANG=PI*(1.-L2*(1.+L1)/2.)+L1*L2*ATAN(ABS(A2/A1))
           DLTR=.5*ANG
           IF(PW.LE.1.E-5) THEN
              SAVE=DLTR
           ELSE
              IF(SAVE-DLTR .GE. 3.0)DLTR=DLTR+PI
              IF(DLTR-SAVE .GE. 3.0)DLTR=DLTR-PI
              SAVE=DLTR
           ENDIF
           PANG=DLTR/PI
           PMOD=MD
           DLTJ=CMPLX(DLTR,DLTI)
 50        FORMAT(T8,F10.7,T21,F10.7,T40,2(F10.7,3X),T68,F10.7,T83,
     1          F10.7,T98,2(F10.7,3X))
C           WRITE(55,50) WE(I),PW1,DLTJ,PMOD,PANG,A1,A2
 60     CONTINUE
      RETURN
      END
      SUBROUTINE RCALC(W,R,ANSW,DET,HAW)
      IMPLICIT NONE
      DOUBLE PRECISION W        !Center of mass energy
      DOUBLE PRECISION R        !Total R(charm)
      REAL ANSW(*)              !Final state production (R)
      COMPLEX DET,HAW           !Mixing results (for use in FCALC)
C---------------------------------------------------------------------
C  Perform Eichten coupled-channel calculation for specified energy.
C  Absolute production rates, in units of R, for charmed mesons are
C  computed at the e+ e- c.m. energy specified by W.  Total R(charm)
C  is returned in R, and rates for each mode are returned in ANSW.
C
C  950724  Michael Kelsey
C---------------------------------------------------------------------
      INCLUDE 'fist.inc'
      INCLUDE 'gen.inc'

C  Useful Parameters
      DOUBLE PRECISION PI
      PARAMETER (PI = 3.14159265358979D+00)

C  Local Variables
      COMPLEX QAB(FIST_MIX)
      DOUBLE PRECISION CORR
      REAL RAT
      INTEGER N,M,J,IQCD,KFLAG


C  Initialization
        DO N=1,FIST_MIX
          DO M=N,FIST_MIX
            CALL FMATRIX(N,M,W)
          ENDDO
        ENDDO

        KFLAG=1
        CALL GRNEX(W,ANSW,KFLAG)

        CALL MINV(OMEGA,FIST_MIX,B,QAB,DET,RAT,HAW)

        R=RAT*(4./3.)*(6.*MC/A**2)/(W**2)

C  Apply correction factor to results
        CORR=(1.-16.*ALPS/(3.*PI))
        R=R*CORR
        DO IQCD=1,15
          ANSW(IQCD)=ANSW(IQCD)*CORR
        ENDDO
      RETURN
      END
      REAL FUNCTION RCHANNEL(X,YFS)
      IMPLICIT NONE
      REAL X                    !Center of mass energy (GeV)
      REAL YFS                  !Final state (usually +0.5) channel
C---------------------------------------------------------------------
C  HBOOK Function.  Computes the R value (ANS) for the given channel
C  at energy X.  This routine is designed to be called internally from
C  HBFUN2, for a 2-D histogram where the X axis is energy and the Y
C  is "integer" bins from 1 to 16, for the various charm states.
C
C  950724  Michael Kelsey
C---------------------------------------------------------------------
      INCLUDE 'fist.inc'

C  Local Variables
      DOUBLE PRECISION W        !Center of mass energy
      INTEGER IFS               !Desired final-state value
      DOUBLE PRECISION R        !Total R(charm) returned by RCALC
      REAL ANSW(FIST_NFS)       !Array of final-state values (RCALC)
      COMPLEX DET,HAW           !Mixing results (for use in FCALC)
      REAL XPREV                !Keep track of call to save CPU
      DATA XPREV /-1./
      SAVE XPREV,ANSW


C  Get results for given energy
        IF (X.NE.XPREV) THEN
          XPREV = X
          W = DBLE(X)           !Program always uses double precision
          CALL RCALC(W,R,ANSW,DET,HAW)
        ENDIF

C  Return selected final-state value
        IFS = INT(YFS)          !Convert mode to integer subscript
        RCHANNEL = ANSW(IFS)
      RETURN
      END
      REAL FUNCTION RCHARM(X)
      IMPLICIT NONE
      REAL X                    !Center of mass energy (GeV)
C---------------------------------------------------------------------
C  HBOOK Function.  Computes the value of R for the specified energy
C  X.  This routine is designed to be called internally from HBFUN1,
C  to fill the "R?charm!" histogram.
C
C  950724  Michael Kelsey
C---------------------------------------------------------------------
      INCLUDE 'fist.inc'

C  Local Variables
      DOUBLE PRECISION W        !Center of mass energy
      DOUBLE PRECISION R        !Total R(charm) returned by RCALC
      REAL ANSW(FIST_NFS)       !Array of final-state values (RCALC)
      COMPLEX DET,HAW           !Mixing results (for use in FCALC)


C  Get results for given energy
        W = DBLE(X)             !Program always uses double precision
        CALL RCALC(W,R,ANSW,DET,HAW)

C  Return total charm production in units of R
        RCHARM = R
      RETURN
      END
      DOUBLE PRECISION FUNCTION SIXJ(J2,S1,S2,L,LP,LP1,LS)
      IMPLICIT NONE
      INTEGER J2,S1,S2,L,LP,LP1,LS

C External Functions
      REAL SJS
      EXTERNAL SJS

C Local Variables
      INTEGER JT,ST,SB,SB1,I1,I,JA,I2
      REAL FACT(2,3),VC11(2),VC2(2,2,2)


C  Initialization
      JT=1
      ST=1
      VC11(1)=-SQRT(1./3.)
      VC11(2)=SQRT(2./3.)
      VC2(1,1,1)=1.0
      VC2(1,1,2)=0.0
      VC2(1,2,1)=0.0
      VC2(1,2,2)=1.0
      VC2(2,1,1)=0.0
      VC2(2,1,2)=1.0
      VC2(2,2,1)=SQRT(1./5.)
      VC2(2,2,2)=-SQRT(2./7.)
      IF(L.EQ.0) I=1
      IF(L.EQ.2) I=2
      IF(LP.EQ.0) I1=1
      IF(LP.EQ.2) I1=2
      I2=LS+1
      IF(LP1.EQ.0) JA=1
      IF(LP1.EQ.2) JA=2
      FACT(I1,I2)=0.0
      DO 10 SB=1,2
      SB1=SB-1
   10 FACT(I1,I2)=SQRT((2.*L+1)*(2.*LP1+1)*(2.*LS+1.)*(2.*J2+1))*
     1   VC11(JA)*VC2(I,JA,I1)*3.*(1.+SB1)**((S1+S2)/2.)*(-1.)**SB1*
     2   SJS(LP1,SB1,J2,S2,1,1)*SJS(LP1,SB1,J2,LS,LP,L)*
     3   SJS(L,SB1,LS,S1,1,1)+FACT(I1,I2)
      SIXJ=FACT(I1,I2)
      RETURN
      END
      REAL FUNCTION SJS(I1,I2,I3,I4,I5,I6)
      IMPLICIT NONE
      INTEGER I1,I2,I3,I4,I5,I6

C  Local Variables
      INTEGER J(3,2), JFLA, IAJ1, IAJ2, KSJ1, KFLA, KSJ2


C  Initialization
      KFLA=0
      SJS=0.0
      J(1,1)=I1
      J(2,1)=I2
      J(3,1)=I3
      J(1,2)=I4
      J(2,2)=I5
      J(3,2)=I6
      DO 10 KSJ2=1,2
      DO 10 KSJ1=1,3
   10 IF(J(KSJ1,KSJ2).EQ.0) KFLA=KSJ1+3*(KSJ2-1)
      IF(KFLA.EQ.0) GO TO 30
      JFLA=0
      IF(KFLA.LE.3) JFLA=1
      KFLA=KFLA-3
      IF(JFLA.GT.0) KFLA=KFLA+3
      IAJ1=KFLA+1
      IAJ2=KFLA+2
      IF(IAJ1.GT.3) IAJ1=IAJ1-3
      IF(IAJ2.GT.3) IAJ2=IAJ2-3
      IF(JFLA.GT.0) GO TO 20
      SJS=(-1)**(J(KFLA,1)+J(IAJ1,1)+J(IAJ2,1))/SQRT((2.*J(IAJ1,1)+
     1   1.)*(2.*J(IAJ2,1)+1.))
      IF(J(IAJ1,1).NE.J(IAJ2,2).OR.J(IAJ1,2).NE.J(IAJ2,1)) SJS=0.0
      RETURN
   20 SJS=(-1)**(J(KFLA,2)+J(IAJ1,2)+J(IAJ2,1))/SQRT((2.*J(IAJ1,1)
     1   +1.)*(2.*J(IAJ2,2)+1.))
      IF(J(IAJ1,1).NE.J(IAJ2,1).OR.J(IAJ1,2).NE.J(IAJ2,2)) SJS=0.0
      RETURN
   30 IF(I5.NE.I6) GO TO 50
      IF(I5.GT.1) GO TO 40
      IF(I3.EQ.1) SJS=1./6.
      IF(I3.EQ.2) SJS=-1./(2.*SQRT(5.))
      RETURN
   40 IF(I5.GT.2) GO TO 50
      SJS=SQRT(7./3.)/10.
      IF(I3.EQ.2.AND.I4.EQ.2) SJS=-1./10.
   50 CONTINUE
      RETURN
      END
      DOUBLE PRECISION FUNCTION WFLAM(X,I)
      IMPLICIT NONE
      DOUBLE PRECISION X
      INTEGER I

C  Local Variables
      DOUBLE PRECISION ARHO(4,5),AC(5),AO
      INTEGER J


C  Initialization
      ARHO(1,1)=1.951907D+0
      ARHO(2,1)=1.139439D+0
      ARHO(3,1)=8.996869D-1
      ARHO(4,1)=7.755678D-1
      ARHO(1,2)=3.441901D+0
      ARHO(2,2)=1.529710D+0
      ARHO(3,2)=1.129173D+0
      ARHO(4,2)=9.437397D-1
      ARHO(1,3)=5.781487D+0
      ARHO(2,3)=1.947987D+0
      ARHO(3,3)=1.347184D+0
      ARHO(4,3)=1.095991D+0
      ARHO(1,4)=9.109572D+0
      ARHO(2,4)=2.409460D+0
      ARHO(3,4)=1.557306D+0
      ARHO(4,4)=1.235111D+0
      ARHO(1,5)=1.347147D+1
      ARHO(2,5)=2.940190D+0
      ARHO(3,5)=1.766301D+0
      ARHO(4,5)=1.363841D+0

      DO 10 J=1,5
        AC(J)=ARHO(I,J)
   10 CONTINUE
      CALL INTPL(1.0D+0,AC,X,AO,1.0D+0,0)
      WFLAM=DSQRT(AO*X+1.0)
      RETURN
      END

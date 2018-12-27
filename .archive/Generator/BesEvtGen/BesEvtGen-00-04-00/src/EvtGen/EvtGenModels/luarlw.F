c************************************************************
c The lund model incoporated with Area law rule, from BESII:
c The original code provided by Hu Haiming, 2010-1-25
C Move to BesEvtGen at 2010-1-25 by PING Ronggang
c---------------------------------------------------------------
c      integer NTEVENT
c      parameter (ndmax=100)
c      integer kf(ndmax),ndaug,km(ndmax)
c      real ecm
c      real*8  px(ndmax),py(ndmax),pz(ndmax),e(ndmax)
c      ecm=4.26
c      ntevent=1000
c      DO ITHEVENT=1,ntevent
c      print*, 'Evt. ',ithevent
c      call LUNDAR(1,ITHEVENT,ECM,NDAUG,KF,KM,PX,PY,PZ,E) 
c      DO N=1,NDAUG
c         PRINT*,KF(N),PX(N),PY(N),PZ(N),E(N)
c      ENDDO
c      ENDDO
c      end



*...  SUBROUTINE OF LUNDAR
      !*************!
      SUBROUTINE LUNDAR(ISR,ITHEVENT,ECM,NDAUG,KF,KM,PX,PY,PZ,E)     
*************
C NEVENT: THE TOTAL NUMBER TO BE GENERATED
C ECM : THE CM ENERGY OF e+e- BEAM
C KF(I_DAUG),KM(I_DAUG),PX,PY,PZ,E : THE I_TH DAUGTHER OF ID,
C   MASS AND FOUR VECTOR MOMENTUM
C---------------------------------------------------- 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/EVENT/IEVENT,NEVENT
      COMMON/PNCORF/IECM,PNF(50,9)
      COMMON/CHECKTAG/DECAYTAG !CHECK THE DECAY 
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      real*8 DECAYTAG
      LOGICAL FIRST /.TRUE./   !MHK  Initialization flag
      parameter (ndmax=4000)
      integer kf(ndmax),ndaug,km(ndmax)
      real  ecm
      real*8 px(ndmax),py(ndmax),pz(ndmax),e(ndmax)
 
c      print*,'ITHEVENT= ',ITHEVENT

 10   IEVENT=ITHEVENT
      IF(ITHEVENT.LT.1) RETURN
      IF(ITHEVENT.EQ.1) THEN  !initiating parameters
      parp(2)=1.08  ! lower limit set for jetset,pingrg
cc The parameter initiation is invalid when setting in the ARLWSET,
cc This is unknown. When adjusting the parameters parj(11,12,14,15,16,17,
cc 1,2,21,25,33,126), the parameter parj(11,12,2) set by the are law are released. see tag "pingrg"
      PARJ(1)=0.44905  !(D=0.1)p(qq)/p(q)
      PARJ(2)=0.54537  !(D=0.3)p(ss)/p(dd,uu) => RALPA(59,60)
      PARJ(03)=0.68    !(D=0.4)(P(us)/P(ud))/(P(s)/P(d))
      PARJ(04)=0.05    !(D=0.05)(1/3)P(ud1)/P(ud0)
      PARJ(05)=0.50    !(D=0.5)P(BMB)/[P(BB)+P(BMB)]=it/(0.5+it)
      PARJ(11)=0.41874 !(D=0.5)ratio of light meson with spin 1/0 RALPA(73) 
      PARJ(12)=0.85113 !(D=0.6)ratio of strang meson with spin 1/0 RALPA(74)
      PARJ(13)=0.75    !(D=0.75)ratio of charm meson with spin 1/0 
      PARJ(14)=0.54183 !(D=0.0)ratio of S=0 meson with L=1, J=1  
      PARJ(15)=0.34146 !(D=0.0)ratio of S=1 meson with L=1, J=0 
      PARJ(16)=0.61726 !(D=0.0)ratio of S=1 meson with L=1, J=1 
      PARJ(17)=0.28821 !(D=0.0)ratio of S=1 meson with L=1, J=2
      PARJ(19)=0.55    !(D=1.0)extra bayon supression
      PARJ(21)=0.25076 !(D=0.36), width in the Px,Py Gaussian dist
      PARJ(25)=1.0     !(D=1.0)eta extra suppression
      PARJ(26)=0.5     !(D=0.4)eta' extra suppression
      PARJ(33)=0.5     !(D=0.8)below which 2 hadrons prodced,
      PARJ(73)=48.4    !Rin(mm) of beam pipe in BESII, Rin_inner(VC)=47.63mm
      PARJ(74)=419.1   !Z(mm) for end of VC in BESII, Z(VC)=419.1mm
      PARJ(126)=0.5    !(D=2)gg minimum mass,, relased for tuning c3 in Pn function, 
      PARJ(127)=0.001  !(D=1)safety margin for small system.(will be changed) 
      PARJ(135)=0.001  !minimum photon energy fraction
      PARJ(136)=0.999  !normal maximum photon energy fraction (filled below)
      PARU(112)=0.36   !(D=0.25GeV) Lambda used in alpha_s
      PARU(113)=1.00   !(D=1)flavour threshold at Q^2=PARU(113)*Mq^2
      PARF(104)=1.65   !c quark constituent mass

      RALPA(14)=0.3     !C0 in Pn
      RALPA(15)=0.513   !C1 in Pn for IALFU(1)=1 & ECM < RALPA(61)
      RALPA(16)=-0.57702 !C2 in Pn for IALFU(1)=1 & ECM < RALPA(61)
      RALPA(17)=-0.795  !C3 in Pn for IALFU(1)=1 & ECM < RALPA(61)

      RALPA(21)=0.0     !C1 in Pn for IALFU(1)=1 & ECM > RALPA(61)
      RALPA(22)=0.01    !C2 in Pn for IALFU(1)=1 & ECM > RALPA(61)
      RALPA(23)=-0.19   !C3 in Pn for IALFU(1)=1 & ECM > RALPA(61)

      RALPA(27)=1.0     !C1 in Pn for IALFU(1)=2 & ECM < RALPA(61)
      RALPA(28)=0.1     !C2 in Pn for IALFU(1)=2 & ECM < RALPA(61)
      RALPA(29)=0.1     !C3 in Pn for IALFU(1)=2 & ECM < RALPA(61)

      RALPA(33)=0.1     !C1 in Pn for IALFU(1)=2 & ECM > RALPA(61)
      RALPA(34)=0.1     !C2 in Pn for IALFU(1)=2 & ECM > RALPA(61)
      RALPA(35)=0.1     !C3 in Pn for IALFU(1)=2 & ECM > RALPA(61)
      RALPA(67)=0.35325

      RALPA(67)=0.1855  !(D=0.1)special two-body suppress factor
      IF(ECM .lt. 3.773) THEN ! the parameters for cms energy less than psi(3770)
         PARJ(11)=0.611798
         PARJ(12)=7.92527E-12
         PARJ(14)=0.244495
         PARJ(15)=1.16573E-15
         PARJ(16)=0.436516
         PARJ(17)=0.530517
         PARJ(1)=0.0651577
         PARJ(2)=0.260378
         PARJ(21)=0.0664835
         RALPA(15)=0.576687
         RALPA(16)=0.364796
         RALPA(17)=3.19486E-06
         RALPA(67)=0.1855       !(D=0.1)special two-body suppress factor
      ENDIF
      CALL  ARLWSET(0,ECM,0,ISR)
      ELSE IF(ITHEVENT.GT.1) THEN
         CALL  ARLWSET(0,ECM,1,ISR)
      ELSE 
C         CALL  ARLWSET(0,ECM,2)
      ENDIF   
c      CALL LULIST0(1)
      if(decaytag.eq.1) return
      
       NDAUG=0
      DO 99, I=1, N
        if(abs(k(i,2)).le.22 .or. k(i,2).eq.92) goto 99 
        NDAUG = NDAUG+1
        km(NDAUG)=k(i,3) !origin, it's mother origin
        kf(NDAUG)=k(i,2) !PID
        px(NDAUG)=p(i,1)
        py(NDAUG)=p(i,2)
        pz(NDAUG)=p(i,3)
         e(NDAUG)=p(i,4)
 99   CONTINUE
c      print*, 'Lunda: N=',N
       call flush()
c       print *, RALPA(15),RALPA(16),RALPA(17),RALPA(67)
       END

****************************************************************
*                    Status ---- Seems OK                      *
*                                                              *
*--------------------------------------------------------------*
* This programme is designed to generate the hadronic eventas  *
*      in e+e- collision according to basic Lund area law      *
*                  which is used with BES-SOBER                *
*                     (  Haiming Hu  )                         *
*                       Aug. 10, 2005                          *
*--------------------------------------------------------------*
*                                                              *
****************************************************************

      !***************************!
      SUBROUTINE ARLWSET(KFL0,ECM0,IFLAG,ISR)
      !***************************!
      INTEGER M,KS(1000,5),MESONFLV(-500:+500)
      INTEGER IOD(-1000:+1000),MESON(50,3,-6:+6,-6:+6)
      REAL PS(4000,5),VS(4000,5),PN(10),FBR(6)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/PNCORF/IECM,PNF(50,9)
      COMMON/MESONINF/IOD,MESON,MESONFLV
      COMMON/EVENT/IEVENT,NEVENT
      COMMON/CHECKTAG/DECAYTAG !CHECK THE DECAY 
      real*8 DECAYTAG
      INTEGER IFLAG
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/LUDAT03/,/MESONINF/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/,/EVENT/,/PNCORF/
*...multiplicity modified factor for 2200 MeV
      DATA (PNF(3,I),I=1,6)
     & /1.000, 1.000, 1.000, 1.000, 1.000, 1.000/
       !  1      2      3      4      5      6

*...multiplicity modified factor for 2600MEV
      DATA (PNF(7,I),I=1,6)
     & /1.000, 1.000, 1.000, 1.000, 1.000, 1.000/
       !  1      2      3      4      5      6   

*...multiplicity modified factor for 3070 MeV
      DATA (PNF(12,I),I=1,6)
     & /1.000, 1.000, 1.000, 1.000, 1.000, 1.000/
       !  1      2      3      4      5      6

*...multiplicity modified factor for 3650 MeV
      DATA (PNF(18,I),I=1,6)
     & /1.000, 0.1000, 1.000, 1.000, 1.000, 1.000/ 
       !  1      2      3      4      5      6
*...initialize parameters
   10 IALFU(01)=1     !(D=1)Pn scheme:0=hand;1=Poisson;2=bino;3=area law
      IALFU(02)=0     !
      IALFU(03)=2     !(D=2)producing event by: 1=JETSET ; 2=LUARLW
      IALFU(04)=KFL0  !(D=0)flavor of initial q-qbar;0:mixed flavour
      IALFU(05)=1     !(D=1)1= do both LUARLW and SIMBES;0=do LUARLW only
      IALFU(06)=0     !(D=0)if remove photon from LULIST:0=no;1=yes
      IALFU(07)=6     !(D=6)maximum primary hadron number (<=6)
      IALFU(08)=0     !if specify multiplicity 0:no; >1:yes and as Nhad
      IALFU(09)=3     !(D=1)<Nhad>:1=exp(s);2=log(s);3=exp(sqrt(lns))
      IALFU(10)=1     !(D=1)exclusive probabilities: 1=JETSET; 2=area law
      IALFU(11)=0     !specified ID code for hadron 1 
      IALFU(12)=0     !specified ID code for hadron 2 
      IALFU(13)=0     !specified ID code for hadron 3 
      IALFU(14)=0     !specified ID code for hadron 4 
      IALFU(15)=0     !specified ID code for hadron 5 
      IALFU(16)=0     !specified ID code for hadron 6 
      IALFU(17)=2     !(D=1)q0-q0bar polar angle sample: 1=JETSET;2=QED
      IALFU(18)=1     !(D=2)produce 1=continuum only;2=both res & con 
      IALFU(19)=2     !(D=2)ISR scheme: 1=JETSET's ;2=LUARLW's
      IALFU(20)=1     !maximum primary hadron number for J/psi substring
      IALFU(21)=0     !(D=0)if fix the XK in ISR: 0=no;1=yes.
      IALFU(22)=1     !if permit small string identify as meson:0=no;1=yes
      IALFU(23)=0     !integrand index (set below)
      IALFU(24)=0     !integrand choice in hadronic cross section (set below)
      IALFU(25)=3     !(D=3)the first IALFU(25) 1-- resonance produced
      IALFU(26)=0     !(D=0) =0: set MSTJ(104) automaticly. =>1: =MSTJ(104) 
      IALFU(27)=0     !(D=0)if set q0-qb0. =0: no; =1: u; =2 d; =3 s; =4 c
      IALFU(28)=1     !(D=0)if tune the Pn one by one. 0=no ; 1=yes.
      IALFU(29)=0     !
      IALFU(30)=0     !
      IALFU(31)=0     !
      IALFU(32)=0     !
      IALFU(33)=0     !
      IALFU(34)=0     !
      IALFU(35)=0     !
      IALFU(36)=0     !
      IALFU(37)=0     !
      IALFU(38)=0     !
      IALFU(39)=0     !(D=0)if consider vertex spread: 0=no; 1=yes
      IALFU(40)=3     !(D=1)1:call LUXKFL; 2:call LUQKFL1; 3: call LUQKFL2 
      IALFU(41)=1     !(D=0)quark mass: 0=current; 1=constitut
      IALFU(93)=5     !(D=10)NI in Gaussian integral (inner)
      IALFU(94)=6     !(D=8)NG in Gaussian integral (inner)
      IALFU(95)=5     !(D=5)NI in Gaussian integral (outer)
      IALFU(96)=6     !(D=6)NG in Gaussian integral (outer)
      IALFU(97)=1     !(D=0)if write out the multiplicity informations
      IALFU(98)=0     !(D=0)if write out warning
      IALFU(99)=0     !if call LULIST: 0=no;1=yes after frag;3=yes after decay 
      IALFU(100)=1    !(D=1)produce events by: 0=JETSET; 1=LUARLW

      RALFU(01)=ECM0  !Ecm, total initinal c.m energy
      RALFU(02)=0     !
      RALFU(03)=0.0   !fixed xk for ISR
      RALFU(04)=0.0001!(D=0.001)minimum fraction of ISR
      RALFU(05)=0.999 !nominal maximum fraction of ISR 
      RALFU(06)=0.2   !rplace PARJ(127) in ALISRK 
      RALFU(22)=0.0   !P2 for IALFU(1)=0
      RALFU(23)=0.0   !P3 for IALFU(1)=0
      RALFU(24)=0.0   !P4 for IALFU(1)=0
      RALFU(25)=0.0   !P5 for IALFU(1)=0 
      RALFU(26)=0.0   !P6 for IALFU(1)=0
      RALFU(81)=0.0   !
      RALFU(82)=0.0   !
      RALFU(83)=0.0   !
      RALFU(84)=0.0   !
      RALFU(85)=0.0   !
      RALFU(86)=0.0   !
      RALFU(87)=0.0   !
      RALFU(88)=0.0   !
      RALFU(89)=0.0   !
      RALFU(90)=0.999 !(=< 1)FRAC
      RALFU(94)=0.01  !lattice scale for 4-body sampling
      RALFU(95)=0.01  !lattice scale for 5-body sampling
      RALFU(96)=0.01  !lattice scale for 6-body sampling
      RALFU(91)=0.01  !requied precision in middle calculations
      RALFU(92)=1E-03 !requied precision in final results

      IALPA(01)=1     !(D=1)area correction due to massive quark pair
      IALPA(02)=1     !
      IALPA(03)=1     !
      IALPA(04)=1     !
      IALPA(05)=1     !
      IALPA(06)=1     !
      IALPA(07)=1     !
      IALPA(08)=1     !
      IALPA(09)=1     !
      IALPA(10)=1     !
      IALPA(30)=2     !(D=2)additional condition for z in 3-body process
      IALPA(31)=0     !(D=0)way to take z solution in 3-body process
      IALPA(32)=2     !(D=2)estimation to lower limits/u
      IALPA(40)=3     !MSTJ(24)=IALPA(40)
      IALPA(71)=1     !(D=2)scheme of transverse momentum
      IALPA(95)=1     !(D=0)if write out warning for unfatal zrror 
      IALPA(96)=0     !flag producing N>6 hadron event by JETSET
      IALPA(99)=0     !idex for return when error occurring
      
      RALPA(01)=0.5     !(D=0.6)b paramter in area law exp(-bA)  
      RALPA(02)=0.3     !(D=0.3)a paramter for meson vertex (no use in area law)
      RALPA(03)=0.5     !(D=0.8)a_B parameter for baryon vertex
      RALPA(04)=0.0099  !d quark current mass used in area correction 
      RALPA(05)=0.0056  !u quark current mass used in area correction
      RALPA(06)=0.199   !s quark current mass used in area correction
      RALPA(07)=1.35    !c quark current mass used in area correction
      RALPA(08)=5.0     !b quark current mass used in area correction
      RALPA(09)=0.990   !open strange-particle threshold
      RALPA(10)=4.7     !Upper energy limit, for BES, reach to 4.6 GeV(D=3.730)open charmed-particle threshold
      RALPA(11)=3.739   !DD etc states low-threshold
      RALPA(12)=4.5     !DD etc states high-threshold
      RALPA(13)=8.7E-05 !J/psi total width (GeV)

      RALPA(18)=0.55    !C4 in Pn for IALFU(1)=1 & ECM < RALPA(61)
      RALPA(19)=0.185   !C5 in Pn for IALFU(1)=1 & ECM < RALPA(61)
      RALPA(20)=1.125   !C6 in Pn for IALFU(1)=1 & ECM < RALPA(61)
      RALPA(24)=0.25    !C4 in Pn for IALFU(1)=1 & ECM > RALPA(61)
      RALPA(25)=0.1955  !C5 in Pn for IALFU(1)=1 & ECM > RALPA(61)
      RALPA(26)=1.15    !C6 in Pn for IALFU(1)=1 & ECM > RALPA(61)
      RALPA(30)=0.1     !C4 in Pn for IALFU(1)=2 & ECM < RALPA(61) 
      RALPA(31)=0.1     !C5 in Pn for IALFU(1)=2 & ECM < RALPA(61)
      RALPA(32)=0.1     !C6 in Pn for IALFU(1)=2 & ECM < RALPA(61)
      RALPA(36)=0.1     !C4 in Pn for IALFU(1)=2 & ECM > RALPA(61)
      RALPA(37)=0.1     !C5 in Pn for IALFU(1)=2 & ECM > RALPA(61)
      RALPA(38)=0.1     !C6 in Pn for IALFU(1)=2 & ECM > RALPA(61)
      RALPA(39)=0.1     !C1 in Pn for IALFU(1)=1 & J/psi decay (glunic state)
      RALPA(40)=0.1     !C2 in Pn for IALFU(1)=1 & J/psi decay (glunic state)
      RALPA(41)=-0.1    !C3 in Pn for IALFU(1)=1 & J/psi decay (glunic state)
      RALPA(42)=0.1     !C4 in Pn for IALFU(1)=1 & J/psi decay (glunic state)
      RALPA(43)=0.01    !C5 in Pn for IALFU(1)=1 & J/psi decay (glunic state)
      RALPA(44)=0.01    !C6 in Pn for IALFU(1)=1 & J/psi decay (glunic state)
      RALPA(45)=0.1     !C1 in Pn for IALFU(1)=2 & J/psi decay (glunic state)
      RALPA(46)=0.1     !C2 in Pn for IALFU(1)=2 & J/psi decay (glunic state)
      RALPA(47)=-0.1    !C3 in Pn for IALFU(1)=2 & J/psi decay (glunic state)
      RALPA(48)=0.1     !C4 in Pn for IALFU(1)=2 & J/psi decay (glunic state)
      RALPA(49)=0.1     !C5 in Pn for IALFU(1)=2 & J/psi decay (glunic state)
      RALPA(50)=0.1     !C6 in Pn for IALFU(1)=2 & J/psi decay (glunic state)
      RALPA(51)=0.0     !
      RALPA(52)=0.0     !
      RALPA(53)=0.0     !
      RALPA(54)=0.0     !
      RALPA(55)=0.0     !
      RALPA(56)=0.0     !
      RALPA(57)=0.0     !
      RALPA(58)=0.0     !
      RALPA(59)=2.0     !(D=10.0) C in A*th(C*X) for PARJ(2)
      RALPA(60)=0.65    !(D=0.30) A in A*th(C*X) for PARJ(2)
      RALPA(61)=5.00    !(D=3.75)bound for 1st and 2nd Pn parameters 
      RALPA(62)=0.3     !(D=0.3)Lambda in ln(s/Lambda^2)
      RALPA(63)=0.1     !rho/(pi+rho) for ggg & rgg string decay
      RALPA(64)=0.1     !K*/(K+K*) for ggg & rgg string decay
      RALPA(65)=5.0     !(D=5.0) maxmum energy for LUARLW available now
      RALPA(66)=1.0     !(N=1)N^n factor for Pn distribution

      RALPA(68)=0.6     !(D=0.1)pt width at J/psi peak for continum state
      RALPA(69)=0.6     !(D=0.1)pt width in J/psi string fragmentation
      RALPA(70)=0.075   !(D=0.7)energy of exceeding threshold
      RALPA(71)=0.70    !(D=0.65)Pt width (continum state in non-DD region)
      RALPA(72)=0.01    !(D=0)Pt correlation strength rho0
      RALPA(73)=0.50    !(D=0.50)rho/(pi+rho) for continuum state PARJ(11)  
      RALPA(74)=0.60    !(D=0.60)K*/(K+K*) for continuum state PARJ(12)
      RALPA(75)=0.75    !(D=0.75)D*/(D+D*) for continuum state PARJ(13)
      RALPA(76)=0.330   !ss/uu,dd
      RALPA(77)=0.32    !d quark constituent mass
      RALPA(78)=0.32    !u quark constituent mass
      RALPA(79)=0.50    !s quark constituent mass
      RALPA(80)=1.62    !c quark constituent mass
      RALPA(81)=0.008   !left Pt scale of (NA-5)th hadron produced
      RALPA(82)=0.007   !left Pt scale of (NA-4)th hadron produced
      RALPA(83)=0.006   !left Pt scale of (NA-3)th hadron produced
      RALPA(84)=0.005   !left Pt scale of (NA-2)th hadron produced
      RALPA(85)=0.004   !left Pt scale of (NA-1)th hadron produced
      RALPA(86)=0.003   !left Pt scale of (NA-0)th hadron produced
      RALPA(87)=0.1     !safety margine for d (constitute) quark
      RALPA(88)=0.1     !safety margine for u (constitute) quark
      RALPA(89)=0.1     !safety margine for s (constitute) quark
      RALPA(90)=0.1     !safety margine for c (constitute) quark
      RALPA(91)=0.5     !minimum invariant mass for non-strange substring
      RALPA(92)=0.6     !minimum invariant mass for strange substring
      RALPA(93)=0.7     !eta mass
      RALPA(94)=0.0     !
      RALPA(95)=0.0     !
      RALPA(96)=0.0     !
      RALPA(97)=0.1     !sigmma_Vx (cm)
      RALPA(98)=0.005   !sigmma_Vy (cm)
      RALPA(99)=5.0     !sigmma_Vz (cm)
C...Parameters in JETSET
      PARU(112)=0.36   !(D=0.25GeV) Lambda used in alpha_s
      PARU(113)=1.00   !(D=1)flavour threshold at Q^2=PARU(113)*Mq^2
      PARF(104)=1.65   !c quark constituent mass
      MSTJ(01)=1       !(D=1)choice of fragmentation
      MSTJ(12)=3       !(D=2)choice of baryon production
      MSTJ(21)=0       !(D=2)form of paeticle decay
      MSTJ(22)=4       !(D=1)decay vertex is within cylindrical volume
      MSTJ(24)=3       !(D=2)way to give out particle masses
      MSTJ(51)=2       !(D=0)inclusion of BE correlation:0=no;1=expon;2=gauss
      MSTJ(101)=0      !(D=5)the type of QCD correction
      MSTJ(102)=1      !(D=2)1=QED; 2=QFD
      MSTJ(104)=4      !(D=5)number of allowed flavours   
      MSTJ(106)=1      !(D=1)angular orientation
      MSTJ(107)=ISR    !(D=0)if initial radiation or not
      MSTJ(115)=3      !(D=1)documentation contents. now >0
      MSTJ(116)=2      !(D=2)initialization cross section & ISR spectrum
      MSTU(12)=0       !writing of title papge: 0=not done; 1=write
      MSTU(21)=2       !(D=2)check on possible errors during execution
      MSTU(22)=100     !(D=10)maximum number of errors that are printed
      MSTU(26)=100     !(D=10)maximum number of warnings that to data

C...event properties (filled in event production)
      KAU(01)=0        !flag of ISR photon emission
      KAU(02)=0        !line number of ISR photon 
      KAU(03)=0        !line number of string or produced particle 
      KAU(04)=0        !possible case for initinal q0-q0bar
      KAU(05)=0        !primary hadron number
      KAU(06)=0        !flag of DDGEN etc generators were used
      KAU(07)=0        !flag of producing DD by LUARLW
      KAU(08)=0        !flag of J/psi production and decay       B C   
      KAU(09)=0        !flag of psi' production and decay        //   BC       
      KAU(10)=IALFU(7) !maximum primary hadron number           //    /\      
      KAU(11)=0        !end_A flavor of the first quark ----  A//D   /  \     
      KAU(12)=0        !end_B flavor of the first quark A  B       A/----\D 
      KAU(13)=0        !end_C flavor of the second quark if has     F    E                                        F    E
      KAU(14)=0        !end_D flavor of the second quark if has
      KAU(15)=0        !end_E flavor of the third quark if has
      KAU(16)=0        !end_F flavor of the third quark if has
      KAU(17)=0        !line number of substring 1
      KAU(18)=0        !line number of substring 2
      KAU(19)=0        !line number of substring 3
      KAU(20)=0        !flag of e+e- => production channel  
      KAU(21)=0        !1st primary hadron ID
      KAU(22)=0        !2nd primary hadron ID  
      KAU(23)=0        !3th primary hadron ID
      KAU(24)=0        !4th primary hadron ID     
      KAU(25)=0        !5th primary hadron ID     
      KAU(26)=0        !6th primary hadron ID  
      KAU(27)=0        !  
      KAU(28)=0        !  
      KAU(29)=0        !
      KAU(30)=0        !     
      KAU(31)=0        !     
      KAU(32)=0        !  
      KAU(33)=0        !  
      KAU(34)=0        !  
      KAU(35)=0        !
      KAU(36)=0        !     
      KAU(37)=0        !     
      KAU(38)=0        !  
      KAU(39)=0        !  
      KAU(40)=0        !  
      KAU(41)=0        !
      KAU(42)=0        !     
      KAU(43)=0        !     
      KAU(44)=0	       !  
      KAU(45)=0        !  
      KAU(46)=0        !  
      KAU(47)=0        !
      KAU(48)=0        !     
      KAU(49)=0        !     
      KAU(50)=0        !
      KAU(51)=0	       !
      KAU(52)=0        !  
      KAU(90)=NEVENT   !total event number
      KAU(91)=IEVENT   !ith event
C      write(*,*)'NEVENT,IEVENT',NEVENT,IEVENT

C...Event properties (filled in event production)
      XAU(01)=RALFU(1) !Ecm, total initinal c.m energy
      XAU(02)=XAU(1)   !Ecm', total c.m energy after ISR 
      XAU(03)=0        !momentum of ISR photon
      XAU(04)=0        !THETA angle of ISR photon
      XAU(05)=0        !PHI angle of ISR photon
      XAU(06)=0        !ALPK of ISR photon
      XAU(07)=0        !
      XAU(08)=0        !
      XAU(09)=0        !
      XAU(10)=0        !
      XAU(11)=0        !U in dP3(A) (fill below)
      XAU(12)=0        !V in dP3(A) (fill below)
      XAU(13)=0        !W in dP3(A) (fill below)
      XAU(22)=0.0      !P2 in multiplicity
      XAU(23)=0.0      !P3 in multiplicity
      XAU(24)=0.0      !P4 in multiplicity
      XAU(25)=0.0      !P5 in multiplicity
      XAU(26)=0.0      !P6 in multiplicity
      XAU(27)=0        !
      XAU(28)=0        !
      XAU(29)=0        !
      XAU(30)=0        !v in Pn=v^n/n!*exp[c1+c2(n-v)+c3(n-v)^2]
      XAU(31)=0        !M1 (fill below)
      XAU(32)=0        !M2 (fill below)
      XAU(33)=0        !M3 (fill below)
      XAU(34)=0        !M4 (fill below)
      XAU(35)=0        !
      XAU(36)=0        !
      XAU(37)=0        !
      XAU(38)=0        !
      XAU(39)=0        !
      XAU(40)=0        !
      XAU(41)=0        !
      XAU(42)=0        !
      XAU(43)=0        !
      XAU(44)=0        !
      XAU(45)=0        !
cc reset to the orignal value
cc      print*,'Finish to initiate the Lunda Area law parameters.'

CC-- BEGIN TO LOOP OVER EVETNS AFTER INITILIZE THE GENERATOR
*      print*,'SMTJ(107)= ',MSTJ(107)
*...Produce event by JETSET
      decaytag=0
      IF(IALFU(3).EQ.1)THEN 
       CALL LUEEVT0(KFL0,ECM0)
       GOTO 20
      ENDIF

      IF(IALFU(5).EQ.0)THEN
       IALFU(6)=1
       MDCY(LUCOMP0(111),1)=1  !if pi0 decay : 0=inhibi; 1=allow
       MDCY(LUCOMP0(310),1)=1  !if K_S0 decay: 0=inhibi; 1=allow
       PARJ(73)=1000.0
       PARJ(74)=1000.0
      ENDIF
c      IF(IEVENT.EQ.1)CALL LULIST0(0)

*...ISR epectrum
      IF(IEVENT.EQ.1.AND.MSTJ(107).EQ.1)CALL ISRCRYSTBALL(ECM0)
      if(decaytag.eq.1) return
C...ISR is off for J/psi events
      XMJPSI=3.09687
      ECML=XMJPSI-5.0*RALPA(13)
      ECMH=XMJPSI+5.0*RALPA(13)
      IF(ECML.LE.ECM0.AND.ECM0.LE.ECMH)MSTJ(107)=0

C...Predicted multiplicity distribution
      IF(IEVENT.EQ.1)CALL ALMUPD(-1)
C...read meson's achive
      CALL MESONACHIEVE

C...Read exclusive probabilities by area law
      IF(IALFU(10).EQ.2.AND.IEVENT.EQ.1.AND.IALFU(8).EQ.0)THEN
       CALL ALPROB(IEVENT)
      ENDIF
C******* debugging
c      print*, 'IALFU(8)= ', IALFU(8) 
c      IALFU(8)=2
C...check if initial KFL0 flavor reasonable for given channel
      IF(IALFU(8).GE.2)CALL LUINFLCK
C...produce event by area law
      ECM=XAU(1)
      CALL LUBEGN(KFL0,ECM)
      CALL ALCHEK
      if(decaytag.eq.1) return
C...vertex spread
   20 IF(IALFU(39).GE.1)CALL VERTEXREAL
c      IF(IEVENT.LE.1000)CALL LULIST0(1)

C...count mulplicity
      CALL ALMUPD(1)
C...count exclusive probabilities
cc      CALL LUEDIT0(2)
      IF(IALFU(97).EQ.0)RETURN
      if(decaytag.eq.1) return
C...store the original event informations
      M=N
      DO J=1,N
       DO I=1,5
        KS(J,I)=K(J,I)
        PS(J,I)=P(J,I)
        VS(J,I)=V(J,I)
       ENDDO
      ENDDO

C...remove photons in list
      IF(IALFU(5).EQ.0.AND.IALFU(6).GE.1)THEN
       DO I=1,N
        IF(K(I,2).EQ.22)K(I,1)=-1
       ENDDO
c       CALL LUEDIT0(11)
      ENDIF

C...find cchannels
c      CALL FINDCHANNEL(IEVENT,NEVENT)

C...recover the original event informations
      N=M
      DO J=1,M
       DO I=1,5
        K(J,I)=KS(J,I)
        P(J,I)=PS(J,I)
        V(J,I)=VS(J,I)
       ENDDO
      ENDDO
      LEFT=NEVENT-IEVENT
   80 FORMAT(I6,I2,5(F8.4))
   97 FORMAT(4(F6.4,1X))
C   98 IF(IALFU(97).GE.1.AND.IEVENT.EQ.NEVENT)THEN
 98    IF(IALFU(97).GE.1.AND.IFLAG.EQ.2)THEN !! FINALIZ, PINGRG-2010-1-25
       KAU(10)=6
       CALL ALMUPD(2)
       KAU(8)=0
       KAU(20)=0
       CALL POISSONDIS(ECM,PN)
       WRITE(*,*)'Single string state V=',XAU(30)
       DO I=1,6
        WRITE(*,96)'PN(',I,') =',PN(I)
       ENDDO
       WRITE(*,*)'Multi-string state V=',XAU(30)
       KAU(8)=1
       KAU(20)=4
       CALL POISSONDIS(ECM,PN)
       DO I=1,6
        WRITE(*,96)'PN(',I,') =',PN(I)
       ENDDO
       CALL JDCYBRCHR(FBR)
       WRITE(*,*)'BR of J/psi decay '
       WRITE(*,95)'BRee  =',FBR(1)
       WRITE(*,95)'BRmm  =',FBR(2)
       WRITE(*,95)'BRqq  =',FBR(3)
       WRITE(*,95)'BRggg =',FBR(4)
       WRITE(*,95)'BRrgg =',FBR(5)
       WRITE(*,95)'BRreta=',FBR(6)
   99  WRITE(*,*)'left event',LEFT
       WRITE(*,*)'*******************************************'
      ENDIF
   95 FORMAT(A8,F8.4)
   96 FORMAT(A5,I1,A3,F8.4)
  100 CONTINUE
ccc    debuging
c      print*,'parj(11)',parj(11),' parj(12)=',parj(12)
c      print*,'parj(14)',parj(14),' parj(15)=',parj(15)
c      print*,'parj(16)',parj(16),' parj(17)=',parj(17)
c      print*,'parj(1)' ,parj(1) ,' parj(2)=',parj(2)
c      print*,'parj(21)',parj(21),' parj(25)=',parj(25)
c      print *, RALPA(15),RALPA(16),RALPA(17)
c      print*,'parj(33)',parj(33),' parj(126)=',parj(126)

      END

C...check if initial KFL0 flavor reasonable for given channel
      !*****************!
      SUBROUTINE LUINFLCK
      !*****************!
      INTEGER KFL
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUDAT01/,/ARLWDAT1/,/ARLWDAT3/
      KFL0=IALFU(4)
      ECM=XAU(1)
      ECMC=XAU(2)
      IDMAX=0
      DO J=1,IALFU(8)
       ID=IABS(IALFU(10+J))
       IDMAX=MAX(IDMAX,ID)
c       print*, ID,' ',IDMAX
      ENDDO
   10 IF(IDMAX.LT.300)THEN !common meson state
       MSTJ(104)=2
       CALL LUXKFL0(KFL0,ECM,ECMC,KFL)
      ELSEIF(300.LE.IDMAX.AND.IDMAX.LE.400)THEN !stange meson state
       MSTJ(104)=3
       CALL LUXKFL0(KFL0,ECM,ECMC,KFL)
      ELSEIF(400.LE.IDMAX.AND.IDMAX.LE.500)THEN !charm meson state
       KFL=4
      ELSEIF(400.LE.IDMAX.AND.IDMAX.LE.500)THEN !bottom meson state
       KFL=5  
      ELSE
       WRITE(*,*)'NOT READY FOR TOP MESON SIMULATION. STOP !'
       STOP
      ENDIF

c      print*, 'KFL= ',kfl
      IF(KFL.EQ.0)GOTO 10
      KAU(4)=KFL
      END

C...Read exclusive probabilities by area law
      !********************!
      SUBROUTINE ALPROB(IEV)
      !********************!
      INTEGER N2000(6,4),IC2000(6,4,6,200),JC(6,200),PDG(6)
      INTEGER CA2000(6,4,200),CB2000(6,4,200) !C1 & C2
      INTEGER CA(6,4,200),CB(6,4,200) !C1 & C2
      REAL WTPAR(10000),WTSPI(10000),WTSTG(10000)
      REAL RC2000(6,4,200),PC2000(6,4,200),PCH(200),FCHN(0:200)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /ARLWDAT2/,/ARLWDAT3/,/LUJETS/
      SAVE N2000,IC2000,PC2000

C...Read data
      IF(IEV.GT.1)GOTO 70
      OPEN(UNIT=10,FILE='r2dd2000.dat',STATUS='OLD')
      OPEN(UNIT=11,FILE='r2uu2000.dat',STATUS='OLD')
      OPEN(UNIT=12,FILE='r2ss2000.dat',STATUS='OLD')
      OPEN(UNIT=13,FILE='r3dd2000.dat',STATUS='OLD')
      OPEN(UNIT=14,FILE='r3uu2000.dat',STATUS='OLD')
      OPEN(UNIT=15,FILE='r3ss2000.dat',STATUS='OLD')
      OPEN(UNIT=16,FILE='r4dd2000.dat',STATUS='OLD')
      OPEN(UNIT=17,FILE='r4uu2000.dat',STATUS='OLD')
      OPEN(UNIT=18,FILE='r4ss2000.dat',STATUS='OLD')
      OPEN(UNIT=19,FILE='r5dd2000.dat',STATUS='OLD')
      OPEN(UNIT=20,FILE='r5uu2000.dat',STATUS='OLD')
      OPEN(UNIT=21,FILE='r5ss2000.dat',STATUS='OLD')
      N2000(2,1)=18                  !dd string
      N2000(2,2)=18                  !uu string
      N2000(2,3)=10                  !ss string
      N2000(3,1)=66                  !dd string
      N2000(3,2)=67                  !uu string
      N2000(3,3)=48                  !ss string
      N2000(4,1)=129                 !dd string
      N2000(4,2)=129                 !uu string
      N2000(4,3)=85                  !ss string
      N2000(5,1)=176                 !dd string
      N2000(5,2)=181                 !uu string
      N2000(5,3)=108                 !ss string

C...2-body
      DO J=1,N2000(2,1)     !dd
       READ(10,20)I,
     & IC2000(2,1,1,J),IC2000(2,1,2,J),
     & CA2000(2,1,J),CB2000(2,1,J),RC2000(2,1,J)
      ENDDO
      DO J=1,N2000(2,2)     !uu
       READ(11,20)I,
     & IC2000(2,2,1,J),IC2000(2,2,2,J),
     & CA2000(2,2,J),CB2000(2,2,J),RC2000(2,2,J)
      ENDDO
      DO J=1,N2000(2,3)     !ss
       READ(12,20)I,
     & IC2000(2,3,1,J),IC2000(2,3,2,J),
     & CA2000(2,3,J),CB2000(2,3,J),RC2000(2,3,J)
      ENDDO
   20 FORMAT(I5,I6,I6,I3,I3,F10.6)  		

C...3 body
      DO J=1,N2000(3,1)     !dd
       READ(13,30)I,
     & IC2000(3,1,1,J),IC2000(3,1,2,J),IC2000(3,1,3,J),
     & CA2000(3,1,J),CB2000(3,1,J),RC2000(3,1,J)
      ENDDO
      DO J=1,N2000(3,2)     !uu
       READ(14,30)I,
     & IC2000(3,2,1,J),IC2000(3,2,2,J),IC2000(3,2,3,J),
     & CA2000(3,2,J),CB2000(3,2,J),RC2000(3,2,J)
      ENDDO
      DO J=1,N2000(3,3)     !ss
       READ(15,30)I,
     & IC2000(3,3,1,J),IC2000(3,3,2,J),IC2000(3,3,3,J),
     & CA2000(3,3,J),CB2000(3,3,J),RC2000(3,3,J)
      ENDDO
   30 FORMAT(I5,I6,I6,I6,I3,I3,F10.6)

C...4 body
      DO J=1,N2000(4,1)     !dd
       READ(16,40)I,
     & IC2000(4,1,1,J),IC2000(4,1,2,J),
     & IC2000(4,1,3,J),IC2000(4,1,4,J),
     & CA2000(4,1,J),CB2000(4,1,J),RC2000(4,1,J)
      ENDDO
      DO J=1,N2000(4,2)     !uu
       READ(17,40)I,
     & IC2000(4,2,1,J),IC2000(4,2,2,J),
     & IC2000(4,2,3,J),IC2000(4,2,4,J),
     & CA2000(4,2,J),CB2000(4,2,J),RC2000(4,2,J)
      ENDDO
      DO J=1,N2000(4,3)     !ss
       READ(18,40)I,
     & IC2000(4,3,1,J),IC2000(4,3,2,J),
     & IC2000(4,3,3,J),IC2000(4,3,4,J),
     & CA2000(4,3,J),CB2000(4,3,J),RC2000(4,3,J)
      ENDDO
   40 FORMAT(I5,I6,I6,I6,I6,I3,I3,F10.6)

*...5 body
      DO J=1,N2000(5,1)     !dd
       READ(19,50)I,
     & IC2000(5,1,1,J),IC2000(5,1,2,J),
     & IC2000(5,1,3,J),IC2000(5,1,4,J),IC2000(5,1,5,J),
     & CA2000(5,1,J),CB2000(5,1,J),RC2000(5,1,J)
      ENDDO
      DO J=1,N2000(5,2)     !uu
       READ(20,50)I,
     & IC2000(5,2,1,J),IC2000(5,2,2,J),
     & IC2000(5,2,3,J),IC2000(5,2,4,J),IC2000(5,2,5,J),
     & CA2000(5,2,J),CB2000(5,2,J),RC2000(5,2,J)
      ENDDO
      DO J=1,N2000(5,3)     !ss
       READ(21,50)I,
     & IC2000(5,3,1,J),IC2000(5,3,2,J),
     & IC2000(5,3,3,J),IC2000(5,3,4,J),IC2000(5,3,5,J),
     & CA2000(5,3,J),CB2000(5,3,J),RC2000(5,3,J)
      ENDDO
   50 FORMAT(I5,I6,I6,I6,I6,I6,I3,I3,F10.6)

*...6 body
      DO J=1,N2000(6,1)     !dd
       READ(19,60)I,
     & IC2000(6,1,1,J),IC2000(6,1,2,J),IC2000(6,1,3,J),
     & IC2000(6,1,4,J),IC2000(6,1,5,J),IC2000(6,1,6,J),
     & CA2000(6,1,J),CB2000(6,1,J),RC2000(6,1,J)
      ENDDO
      DO J=1,N2000(6,2)     !uu
       READ(19,60)I,
     & IC2000(6,2,1,J),IC2000(6,2,2,J),IC2000(6,2,3,J),
     & IC2000(6,2,4,J),IC2000(6,2,5,J),IC2000(6,2,6,J),
     & CA2000(6,2,J),CB2000(6,2,J),RC2000(6,2,J)
      ENDDO
      DO J=1,N2000(6,3)     !ss
       READ(19,60)I,
     & IC2000(6,3,1,J),IC2000(6,3,2,J),IC2000(6,3,3,J),
     & IC2000(6,3,4,J),IC2000(6,3,5,J),IC2000(6,3,6,J),
     & CA2000(6,3,J),CB2000(6,3,J),RC2000(6,3,J)
      ENDDO
   60 FORMAT(I5,I6,I6,I6,I6,I6,I6,I3,I3,F10.6)

      DO I=10,30
       CLOSE(I)
      ENDDO
      IF(IEV.LE.1)RETURN      

C...determine channel
   70 NBD=KAU(5)
      KFL=ABS(KAU(11))
      NCHN=N2000(NBD,KFL)
      DO IBD=1,NBD
       DO JCHN=1,NCHN
        JC(IBD,JCHN)=IC2000(NBD,KFL,IBD,JCHN)
       ENDDO
      ENDDO
      SUM=0
      DO JCHN=1,NCHN
       DO IBD=1,NBD
        PDG(IBD)=JC(IBD,JCHN)
       ENDDO
       WTPAR(JCHN)=WEIGHTPAR(NBD,KFF,PDG)  !coefficent filling string
       WTSPI(JCHN)=WEIGHTSPI(NBD,KFF,PDG)  !spin factor (2J+1) or V/PS
       WTSTG(JCHN)=STRGSUPRS(NBD,KFF,PDG)  !strange supression factor
       PCH(JCHN)=CA(NBD,KFL,JCHN)*CB(NBD,KFL,JCHN)*RC2000(NBD,KFL,JCHN)
       SUM=SUM+PCH(JCHN)
      ENDDO
      FCHN(0)=0
      DO JCHN=1,NCHN
       PCH(JCHN)=PCH(JCHN)/SUM
       FCHN(JCHN)=FCHN(JCHN-1)+PCH(JCHN)
      ENDDO
      R=RLU0(0)
      DO JCHN=1,NCHN
       IF(FCHN(JCHN-1).LT.R.AND.R.LE.FCHN(JCHN))ITCHN=JCHN
      ENDDO
      DO IBD=1,NBD
       KA(IBD,1)=1
       KA(IBD,2)=JC(IBD,ITCHN)
       KA(IBD,3)=KAU(3)
       KA(IBD,4)=0
       KA(IBD,5)=0
       PA(IBD,5)=ULMASS0(KA(IBD,2))
       K(KAU(3)+IBD,2)=KA(IBD,2)
       DO I=1,5
        V(KAU(3)+IBD,I)=0.0
       ENDDO
      ENDDO

C...check if the channel is rerasonable
      NETB=0
      NETS=0
      NETC=0
      DO IBD=1,NBD
       KF=KA(IBD,2)
       CALL PROPTY(KF,JMB,JST,JCH)
       IMB=IABS(JMB)
       IF(IMB.EQ.3)NETB=NETB+JMB
       NETS=NETS+JST
       NETC=NETC+JCH
      ENDDO
      NETBSC=IABS(NETB)+IABS(NETS)+IABS(NETC) !total quantum number
      IF(NETBSC.NE.0)THEN
       WRITE(*,*)'B or S or C not conservation'
       WRITE(*,*)'Stop ! Check area law table'
       STOP
      ENDIF
      END

*...Find out all channels and account branching ratios
      !*****************************!
      SUBROUTINE FINDCHANNEL(IEV,NEV)
      !*****************************! 
      INTEGER CONFIG(50,10000,50),PDG(50),NCF(50,10000)
      INTEGER NCONFIG(50)
      REAL PCHJ(50,10000),RCF(50,10000),SUMP(50)
      CHARACTER CHAP1*16,CHAP2*16,CHAP3*16,CHAP4*16,CHAP5*16
      CHARACTER CHAP6*16,CHAP7*16,CHAP8*16,CHAP9*16,CHAP10*16
      CHARACTER CHAP11*16,CHAP12*16,CHAP13*16,CHAP14*16,CHAP15*16
      CHARACTER CHAP16*16,CHAP17*16,CHAP18*16,CHAP19*16,CHAP20*16
      COMMON/CONFIG/NCONFIG,CONFIG
      COMMON/PCHJ/PCHJ
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /LUJETS/,/CONFIG/,/PCHJ/,/ARLWDAT1/
      NBDMAX=50
      IMAX=10000
      JMAX=50
      ECM=RALFU(1)

C...initialization
      IF(IEV.EQ.1)THEN
       CALL OPENFILE(ECM)
       DO NBD=1,NBDMAX
        DO I=1,IMAX
         DO J=1,JMAX
          CONFIG(NBD,I,J)=0
         ENDDO
        ENDDO
        NCONFIG(NBD)=0
       ENDDO
      ENDIF

C...make statistics
      DO J=1,N
       PDG(J)=K(J,2)
      ENDDO
      CALL INTSORT(PDG,N,1,N)
      IF(NCONFIG(N).EQ.0)THEN
       NCONFIG(N)=1
       DO J=1,N
        CONFIG(N,NCONFIG(N),J)=PDG(J)
       ENDDO
       NCF(N,NCONFIG(N))=1
      ELSE
       MAYNEW=0
       DO I=1,NCONFIG(N)
        IT=0
        DO J=1,N
         IF(PDG(J).EQ.CONFIG(N,I,J))IT=IT+1
        ENDDO
        IF(IT.EQ.N)THEN
         NCF(N,I)=NCF(N,I)+1
        ELSE
         MAYNEW=MAYNEW+1
        ENDIF
       ENDDO
       IF(MAYNEW.EQ.NCONFIG(N))THEN
        NCONFIG(N)=NCONFIG(N)+1
        NCF(N,NCONFIG(N))=NCF(N,NCONFIG(N))+1
        DO J=1,N
         CONFIG(N,NCONFIG(N),J)=PDG(J)
        ENDDO
       ENDIF
      ENDIF
      IF(IEV.EQ.1)THEN
       SUMNEV=1
      ELSE
       SUMNEV=SUMNEV+1
      ENDIF
      IF(IEV.LT.NEV)RETURN

C...Calculate Pn for each channel
      DO NBD=2,NBDMAX
       DO I=1,NCONFIG(NBD)
        RCF(NBD,I)=NCF(NBD,I)
       ENDDO
       NN=NCONFIG(NBD)
       MM=NCONFIG(NBD)
       CALL REASORTJTST(NBD,RCF,NN,1,MM)
       DO I=1,NCONFIG(NBD)
        PCHJ(NBD,I)=RCF(NBD,I)/SUMNEV*100.0
       ENDDO

C...rearrange channels in the order of P decreasing
       CALL REASORTJTST(NBD,PCHJ,NN,1,MM)
       SUM=0
       DO I=1,NCONFIG(NBD)
        SUM=SUM+PCHJ(NBD,I)
       ENDDO
       SUMP(NBD)=SUM
      ENDDO

C...Output
      WRITE(*,*)'*******************************************'
      WRITE(*,*)'2-body final states acounted to',SUMP(2),'%'
      DO I=1,NCONFIG(2)
       KF1=CONFIG(2,I,1)
       KF2=CONFIG(2,I,2)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       WRITE(20,20)I,CHAP1,CHAP2,PCHJ(2,I)
       WRITE(*,20)I,CHAP1,CHAP2,PCHJ(2,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'3-body final states acounted to',SUMP(3),'%'
      DO I=1,NCONFIG(3)
       KF1=CONFIG(3,I,1)
       KF2=CONFIG(3,I,2)
       KF3=CONFIG(3,I,3)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       WRITE(30,30)I,CHAP1,CHAP2,CHAP3,PCHJ(3,I)
       WRITE(*,30)I,CHAP1,CHAP2,CHAP3,PCHJ(3,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'4-body final states acounted to',SUMP(4),'%'
      DO I=1,NCONFIG(4)
       KF1=CONFIG(4,I,1)
       KF2=CONFIG(4,I,2)
       KF3=CONFIG(4,I,3)
       KF4=CONFIG(4,I,4)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       WRITE(40,40)I,CHAP1,CHAP2,CHAP3,CHAP4,PCHJ(4,I)
       WRITE(*,40)I,CHAP1,CHAP2,CHAP3,CHAP4,PCHJ(4,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'5-body final states acounted to',SUMP(5),'%'
      DO I=1,NCONFIG(5)
       KF1=CONFIG(5,I,1)
       KF2=CONFIG(5,I,2)
       KF3=CONFIG(5,I,3)
       KF4=CONFIG(5,I,4)
       KF5=CONFIG(5,I,5)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       WRITE(50,50)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,PCHJ(5,I)
       WRITE(*,50)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,PCHJ(5,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'6-body final states acounted to',SUMP(6),'%'
      DO I=1,NCONFIG(6)
       KF1=CONFIG(6,I,1)
       KF2=CONFIG(6,I,2)
       KF3=CONFIG(6,I,3)
       KF4=CONFIG(6,I,4)
       KF5=CONFIG(6,I,5)
       KF6=CONFIG(6,I,6)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       WRITE(60,60)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,CHAP6,PCHJ(6,I)
       WRITE(*,60)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,CHAP6,PCHJ(6,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'7-body final states acounted to',SUMP(7),'%'
      DO I=1,NCONFIG(7)
       KF1=CONFIG(7,I,1)
       KF2=CONFIG(7,I,2)
       KF3=CONFIG(7,I,3)
       KF4=CONFIG(7,I,4)
       KF5=CONFIG(7,I,5)
       KF6=CONFIG(7,I,6)
       KF7=CONFIG(7,I,7)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       WRITE(70,70)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,CHAP6,CHAP7,
     &             PCHJ(7,I)
       WRITE(*,70)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,CHAP6,CHAP7,
     &             PCHJ(7,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'8-body final states acounted to',SUMP(8),'%'
      DO I=1,NCONFIG(8)
       KF1=CONFIG(8,I,1)
       KF2=CONFIG(8,I,2)
       KF3=CONFIG(8,I,3)
       KF4=CONFIG(8,I,4)
       KF5=CONFIG(8,I,5)
       KF6=CONFIG(8,I,6)
       KF7=CONFIG(8,I,7)
       KF8=CONFIG(8,I,8)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       WRITE(80,80)I,CHAP1,CHAP2,CHAP3,CHAP4,
     &               CHAP5,CHAP6,CHAP7,CHAP8,PCHJ(8,I)
       WRITE(*,80)I,CHAP1,CHAP2,CHAP3,CHAP4,
     &              CHAP5,CHAP6,CHAP7,CHAP8,PCHJ(8,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'9-body final states acounted to',SUMP(9),'%'
      DO I=1,NCONFIG(9)
       KF1=CONFIG(9,I,1)
       KF2=CONFIG(9,I,2)
       KF3=CONFIG(9,I,3)
       KF4=CONFIG(9,I,4)
       KF5=CONFIG(9,I,5)
       KF6=CONFIG(9,I,6)
       KF7=CONFIG(9,I,7)
       KF8=CONFIG(9,I,8)
       KF9=CONFIG(9,I,9)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       WRITE(90,90)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,PCHJ(9,I)
       WRITE(*,90)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,PCHJ(9,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'10-body final states acounted to',SUMP(10),'%'
      DO I=1,NCONFIG(10)
       KF1=CONFIG(10,I,1)
       KF2=CONFIG(10,I,2)
       KF3=CONFIG(10,I,3)
       KF4=CONFIG(10,I,4)
       KF5=CONFIG(10,I,5)
       KF6=CONFIG(10,I,6)
       KF7=CONFIG(10,I,7)
       KF8=CONFIG(10,I,8)
       KF9=CONFIG(10,I,9)
       KF10=CONFIG(10,I,10)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       WRITE(90,91)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               PCHJ(10,I)
       WRITE(*,91)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              PCHJ(10,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'11-body final states acounted to',SUMP(11),'%'
      DO I=1,NCONFIG(11)
       KF1=CONFIG(11,I,1)
       KF2=CONFIG(11,I,2)
       KF3=CONFIG(11,I,3)
       KF4=CONFIG(11,I,4)
       KF5=CONFIG(11,I,5)
       KF6=CONFIG(11,I,6)
       KF7=CONFIG(11,I,7)
       KF8=CONFIG(11,I,8)
       KF9=CONFIG(11,I,9)
       KF10=CONFIG(11,I,10)
       KF11=CONFIG(11,I,11)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       WRITE(90,92)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,PCHJ(11,I)
       WRITE(*,92)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              CHAP11,PCHJ(11,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'12-body final states acounted to',SUMP(12),'%'
      DO I=1,NCONFIG(12)
       KF1=CONFIG(12,I,1)
       KF2=CONFIG(12,I,2)
       KF3=CONFIG(12,I,3)
       KF4=CONFIG(12,I,4)
       KF5=CONFIG(12,I,5)
       KF6=CONFIG(12,I,6)
       KF7=CONFIG(12,I,7)
       KF8=CONFIG(12,I,8)
       KF9=CONFIG(12,I,9)
       KF10=CONFIG(12,I,10)
       KF11=CONFIG(12,I,11)
       KF12=CONFIG(12,I,12)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       WRITE(90,93)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,PCHJ(12,I)
       WRITE(*,93)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              CHAP11,CHAP12,PCHJ(12,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'13-body final states acounted to',SUMP(13),'%'
      DO I=1,NCONFIG(13)
       KF1=CONFIG(13,I,1)
       KF2=CONFIG(13,I,2)
       KF3=CONFIG(13,I,3)
       KF4=CONFIG(13,I,4)
       KF5=CONFIG(13,I,5)
       KF6=CONFIG(13,I,6)
       KF7=CONFIG(13,I,7)
       KF8=CONFIG(13,I,8)
       KF9=CONFIG(13,I,9)
       KF10=CONFIG(13,I,10)
       KF11=CONFIG(13,I,11)
       KF12=CONFIG(13,I,12)
       KF13=CONFIG(13,I,13)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       WRITE(90,94)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,PCHJ(13,I)
       WRITE(*,94)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              CHAP11,CHAP12,CHAP13,PCHJ(13,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'14-body final states acounted to',SUMP(14),'%'
      DO I=1,NCONFIG(14)
       KF1=CONFIG(14,I,1)
       KF2=CONFIG(14,I,2)
       KF3=CONFIG(14,I,3)
       KF4=CONFIG(14,I,4)
       KF5=CONFIG(14,I,5)
       KF6=CONFIG(14,I,6)
       KF7=CONFIG(14,I,7)
       KF8=CONFIG(14,I,8)
       KF9=CONFIG(14,I,9)
       KF10=CONFIG(14,I,10)
       KF11=CONFIG(14,I,11)
       KF12=CONFIG(14,I,12)
       KF13=CONFIG(14,I,13)
       KF14=CONFIG(14,I,14)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       CALL LUNAME0(KF14,CHAP14)
       WRITE(90,95)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,PCHJ(14,I)
       WRITE(*,95)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,PCHJ(14,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'15-body final states acounted to',SUMP(15),'%'
      DO I=1,NCONFIG(15)
       KF1=CONFIG(15,I,1)
       KF2=CONFIG(15,I,2)
       KF3=CONFIG(15,I,3)
       KF4=CONFIG(15,I,4)
       KF5=CONFIG(15,I,5)
       KF6=CONFIG(15,I,6)
       KF7=CONFIG(15,I,7)
       KF8=CONFIG(15,I,8)
       KF9=CONFIG(15,I,9)
       KF10=CONFIG(15,I,10)
       KF11=CONFIG(15,I,11)
       KF12=CONFIG(15,I,12)
       KF13=CONFIG(15,I,13)
       KF14=CONFIG(15,I,14)
       KF15=CONFIG(15,I,15)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       CALL LUNAME0(KF14,CHAP14)
       CALL LUNAME0(KF15,CHAP15)
       WRITE(90,96)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,PCHJ(15,I)
       WRITE(*,96)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,PCHJ(15,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'16-body final states acounted to',SUMP(16),'%'
      DO I=1,NCONFIG(16)
       KF1=CONFIG(16,I,1)
       KF2=CONFIG(16,I,2)
       KF3=CONFIG(16,I,3)
       KF4=CONFIG(16,I,4)
       KF5=CONFIG(16,I,5)
       KF6=CONFIG(16,I,6)
       KF7=CONFIG(16,I,7)
       KF8=CONFIG(16,I,8)
       KF9=CONFIG(16,I,9)
       KF10=CONFIG(16,I,10)
       KF11=CONFIG(16,I,11)
       KF12=CONFIG(16,I,12)
       KF13=CONFIG(16,I,13)
       KF14=CONFIG(16,I,14)
       KF15=CONFIG(16,I,15)
       KF16=CONFIG(16,I,16)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       CALL LUNAME0(KF14,CHAP14)
       CALL LUNAME0(KF15,CHAP15)
       CALL LUNAME0(KF16,CHAP16)
       WRITE(90,97)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &               CHAP16,PCHJ(16,I)
       WRITE(*,97)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &              CHAP16,PCHJ(16,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'17-body final states acounted to',SUMP(17),'%'
      DO I=1,NCONFIG(17)
       KF1=CONFIG(17,I,1)
       KF2=CONFIG(17,I,2)
       KF3=CONFIG(17,I,3)
       KF4=CONFIG(17,I,4)
       KF5=CONFIG(17,I,5)
       KF6=CONFIG(17,I,6)
       KF7=CONFIG(17,I,7)
       KF8=CONFIG(17,I,8)
       KF9=CONFIG(17,I,9)
       KF10=CONFIG(17,I,10)
       KF11=CONFIG(17,I,11)
       KF12=CONFIG(17,I,12)
       KF13=CONFIG(17,I,13)
       KF14=CONFIG(17,I,14)
       KF15=CONFIG(17,I,15)
       KF16=CONFIG(17,I,16)
       KF17=CONFIG(17,I,17)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       CALL LUNAME0(KF14,CHAP14)
       CALL LUNAME0(KF15,CHAP15)
       CALL LUNAME0(KF16,CHAP16)
       CALL LUNAME0(KF17,CHAP17)
       WRITE(90,98)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &               CHAP16,CHAP17,PCHJ(17,I)
       WRITE(*,98)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &              CHAP16,CHAP17,PCHJ(17,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'18-body final states acounted to',SUMP(18),'%'
      DO I=1,NCONFIG(18)
       KF1=CONFIG(18,I,1)
       KF2=CONFIG(18,I,2)
       KF3=CONFIG(18,I,3)
       KF4=CONFIG(18,I,4)
       KF5=CONFIG(18,I,5)
       KF6=CONFIG(18,I,6)
       KF7=CONFIG(18,I,7)
       KF8=CONFIG(18,I,8)
       KF9=CONFIG(18,I,9)
       KF10=CONFIG(18,I,10)
       KF11=CONFIG(18,I,11)
       KF12=CONFIG(18,I,12)
       KF13=CONFIG(18,I,13)
       KF14=CONFIG(18,I,14)
       KF15=CONFIG(18,I,15)
       KF16=CONFIG(18,I,16)
       KF17=CONFIG(18,I,17)
       KF18=CONFIG(18,I,18)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       CALL LUNAME0(KF14,CHAP14)
       CALL LUNAME0(KF15,CHAP15)
       CALL LUNAME0(KF16,CHAP16)
       CALL LUNAME0(KF17,CHAP17)
       CALL LUNAME0(KF18,CHAP18)
       WRITE(90,99)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &               CHAP16,CHAP17,CHAP18,PCHJ(18,I)
       WRITE(*,99)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &              CHAP16,CHAP17,CHAP18,PCHJ(18,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'19-body final states acounted to',SUMP(19),'%'
      DO I=1,NCONFIG(19)
       KF1=CONFIG(19,I,1)
       KF2=CONFIG(19,I,2)
       KF3=CONFIG(19,I,3)
       KF4=CONFIG(19,I,4)
       KF5=CONFIG(19,I,5)
       KF6=CONFIG(19,I,6)
       KF7=CONFIG(19,I,7)
       KF8=CONFIG(19,I,8)
       KF9=CONFIG(19,I,9)
       KF10=CONFIG(19,I,10)
       KF11=CONFIG(19,I,11)
       KF12=CONFIG(19,I,12)
       KF13=CONFIG(19,I,13)
       KF14=CONFIG(19,I,14)
       KF15=CONFIG(19,I,15)
       KF16=CONFIG(19,I,16)
       KF17=CONFIG(19,I,17)
       KF18=CONFIG(19,I,18)
       KF19=CONFIG(19,I,19)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       CALL LUNAME0(KF14,CHAP14)
       CALL LUNAME0(KF15,CHAP15)
       CALL LUNAME0(KF16,CHAP16)
       CALL LUNAME0(KF17,CHAP17)
       CALL LUNAME0(KF18,CHAP18)
       CALL LUNAME0(KF19,CHAP19)
       WRITE(90,100)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &               CHAP16,CHAP17,CHAP18,CHAP19,PCHJ(19,I)
       WRITE(*,100)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &              CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &              CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &              CHAP16,CHAP17,CHAP18,CHAP19,PCHJ(19,I)
      ENDDO
      WRITE(*,*)'==========================================='
      WRITE(*,*)'20-body final states acounted to',SUMP(20),'%'
      DO I=1,NCONFIG(20)
       KF1=CONFIG(20,I,1)
       KF2=CONFIG(20,I,2)
       KF3=CONFIG(20,I,3)
       KF4=CONFIG(20,I,4)
       KF5=CONFIG(20,I,5)
       KF6=CONFIG(20,I,6)
       KF7=CONFIG(20,I,7)
       KF8=CONFIG(20,I,8)
       KF9=CONFIG(20,I,9)
       KF10=CONFIG(20,I,10)
       KF11=CONFIG(20,I,11)
       KF12=CONFIG(20,I,12)
       KF13=CONFIG(20,I,13)
       KF14=CONFIG(20,I,14)
       KF15=CONFIG(20,I,15)
       KF16=CONFIG(20,I,16)
       KF17=CONFIG(20,I,17)
       KF18=CONFIG(20,I,18)
       KF19=CONFIG(20,I,19)
       KF20=CONFIG(20,I,20)
       CALL LUNAME0(KF1,CHAP1)
       CALL LUNAME0(KF2,CHAP2)
       CALL LUNAME0(KF3,CHAP3)
       CALL LUNAME0(KF4,CHAP4)
       CALL LUNAME0(KF5,CHAP5)
       CALL LUNAME0(KF6,CHAP6)
       CALL LUNAME0(KF7,CHAP7)
       CALL LUNAME0(KF8,CHAP8)
       CALL LUNAME0(KF9,CHAP9)
       CALL LUNAME0(KF10,CHAP10)
       CALL LUNAME0(KF11,CHAP11)
       CALL LUNAME0(KF12,CHAP12)
       CALL LUNAME0(KF13,CHAP13)
       CALL LUNAME0(KF14,CHAP14)
       CALL LUNAME0(KF15,CHAP15)
       CALL LUNAME0(KF16,CHAP16)
       CALL LUNAME0(KF17,CHAP17)
       CALL LUNAME0(KF18,CHAP18)
       CALL LUNAME0(KF19,CHAP19)
       CALL LUNAME0(KF20,CHAP20)
       WRITE(90,101)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &                CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &                CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &                CHAP16,CHAP17,CHAP18,CHAP19,CHAP20,PCHJ(20,I)
       WRITE(*,101)I,CHAP1,CHAP2,CHAP3,CHAP4,CHAP5,
     &               CHAP6,CHAP7,CHAP8,CHAP9,CHAP10,
     &               CHAP11,CHAP12,CHAP13,CHAP14,CHAP15,
     &               CHAP16,CHAP17,CHAP18,CHAP19,CHAP20,PCHJ(20,I)
      ENDDO
      SUMSUM=0
      DO I=1,20
       SUMSUM=SUMSUM+SUMP(I)
      ENDDO
      SUMPCHJ=0
      DO I=1,20
       DO J=1,NCONFIG(I)
        SUMPCHJ=SUMPCHJ+PCHJ(I,J)
       ENDDO
      ENDDO
      WRITE(*,*)'*****************************************'
      WRITE(*,*)'Total percent =',SUMSUM,'    ',SUMPCHJ,'%'
   20 FORMAT(I3,1X,2A6,F7.5)
   30 FORMAT(I3,1X,3A6,F7.5)
   40 FORMAT(I3,1X,4A6,F7.5)
   50 FORMAT(I3,1X,5A6,F7.5)
   60 FORMAT(I3,1X,6A6,F7.5)
   70 FORMAT(I3,1X,7A6,F7.5)
   80 FORMAT(I3,1X,8A6,F7.5)
   90 FORMAT(I3,1X,9A6,F7.5)
   91 FORMAT(I3,1X,10A6,F7.5)
   92 FORMAT(I3,1X,11A6,F7.5)
   93 FORMAT(I3,1X,12A6,F7.5)
   94 FORMAT(I3,1X,13A6,F7.5)
   95 FORMAT(I3,1X,14A6,F7.5)
   96 FORMAT(I3,1X,15A6,F7.5)
   97 FORMAT(I3,1X,16A6,F7.5)
   98 FORMAT(I3,1X,17A6,F7.5)
   99 FORMAT(I3,1X,18A6,F7.5)
  100 FORMAT(I3,1X,19A6,F7.5)
  101 FORMAT(I3,1X,20A6,F7.5)
      END

      !*****************************!
      FUNCTION WEIGHTPAR(NBDY,KF,PDG)
      !*****************************!
      INTEGER PDG(10)
      ZRO=0.0
      OFU=0.25
      OTW=0.5 
      ONE=1.0 
      WEIGHTPAR=1.0
      DO J=1,NBDY
       K=ABS(PDG(J))
       IF(K.EQ.111.OR.K.EQ.223.OR.K.EQ.113)THEN !pi0,omega,rho0
	WEIGHTPAR=WEIGHTPAR*OFU
       ELSEIF(K.EQ.221)THEN !eta
	WEIGHTPAR=WEIGHTPAR*(OFU+OTW*RS)/(ONE+RS)
       ELSE
	WEIGHTPAR=WEIGHTPAR*OTW !K,D
       ENDIF
      ENDDO 
      RETURN
      END   		

C...factor about ratio V to PS
      !*****************************!
      FUNCTION WEIGHTSPI(NBDY,KF,PDG)
      !*****************************!
      INTEGER PDG(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /ARLWDAT1/
      ZRO=0.0
      OFU=0.25
      OTW=0.5 
      ONE=1.0 
      TWO=2.0 
      THR=3.0 
      V=RALPA(73)
      U=RALPA(74)
      W=RALPA(75)
      WEIGHTUVW=ONE 
      DO J=1,NBDY
       K=ABS(PDG(J))
       IF(K.EQ.223.OR.K.EQ.113.OR.K.EQ.213)THEN !omega,rh0
        WEIGHTUVW=WEIGHTUVW*V
       ELSEIF(K.EQ.313.OR.K.EQ.323.OR.K.EQ.333)THEN
	WEIGHTUVW=WEIGHTUVW*U
       ELSEIF(K.EQ.413.OR.K.EQ.423.OR.K.EQ.433)THEN
	WEIGHTUVW=WEIGHTUVW*W
       ELSE
	WEIGHTUVW=WEIGHTUVW*ONE
       ENDIF 
      ENDDO 
      WEIGHTSPI=WEIGHTUVW
      RETURN
      END   

      !*****************************!
      FUNCTION STRGSUPRS(NBDY,KF,PDG)
      !*****************************!
      INTEGER PDG(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      NK=0
      NPHI=0
      NETA=0
      ONE=1.0
      TWO=2.0
      RPHI=ONE
      RETA=ONE
      RRS=SQRT(RALPA(76))
      IF(KF.EQ.3.AND.NBDY.EQ.3)THEN
       FETA=RRS/(ONE+RRS)
      ELSE
       FETA=RRS/(ONE+RRS)
      ENDIF 
      STRGSUPRS=ONE

      DO J=1,NBDY   
       K=ABS(PDG(J))
       IF(K.EQ.311.OR.K.EQ.313.OR.K.EQ.321.OR.K.EQ.323)NK=NK+1
       IF(K.EQ.333)NPHI=NPHI+1
       IF(K.EQ.221)NETA=NETA+1
      ENDDO

*...strange supression
      IF(KF.NE.3)THEN 
       DO J=1,NK
        STRGSUPRS=STRGSUPRS*RRS
       ENDDO
      ELSEIF(KF.EQ.3)THEN
       IF(NK.GT.2)THEN   
        DO J=1,NK-2      
         STRGSUPRS=STRGSUPRS*RRS
        ENDDO
       ENDIF 
      ENDIF  
      IF(NPHI.GT.0)THEN
       DO J=1,NPHI
        RPHI=RSR*RPHI
       ENDDO
      ENDIF 
      IF(NETA.GT.0)THEN
       DO J=1,NETA
	RETA=FETA*RETA
       ENDDO
      ENDIF 
      STRGSUPRS=RPHI*RETA*STRGSUPRS
      RETURN
      END

      !*********************!
      SUBROUTINE MESONACHIEVE
      !*********************!
      INTEGER MESONFLV(-500:+500)
      INTEGER IOD(-1000:+1000),MESON(50,3,-6:+6,-6:+6)
      COMMON/MESONINF/IOD,MESON,MESONFLV
      SAVE /MESONINF/

*...initial, nonphyisical states=0
      DO I=-1000,+1000
       IOD(I)=0
      ENDDO
      DO I=1,50
       DO J=1,3
        DO K=-6,+6
         DO L=-6,+6
          MESON(I,J,K,L)=0
         ENDDO
        ENDDO
       ENDDO
      ENDDO
      DO J=-500,500
       MESONFLV(J)=0
      ENDDO

*...meson order
      IOD(+211)=01 !pi+
      IOD(-211)=02 !pi-
      IOD(+111)=03 !pi0
      IOD(+213)=04 !rho+
      IOD(-213)=05 !rho-
      IOD(+113)=06 !rho0
      IOD(+311)=07 !K0
      IOD(-311)=08 !K0~
      IOD(+313)=09 !K*0
      IOD(-313)=10 !K*0~
      IOD(+321)=11 !K+
      IOD(-321)=12 !K-
      IOD(+323)=13 !K*+
      IOD(-323)=14 !K*-
      IOD(+411)=15 !D+
      IOD(-411)=16 !D-
      IOD(+413)=17 !D*+
      IOD(-413)=18 !D*-
      IOD(+421)=19 !D0
      IOD(-421)=20 !D0~
      IOD(+423)=21 !D*0
      IOD(-423)=22 !D*0~
      IOD(+431)=23 !D_s+
      IOD(-431)=24 !D_s-
      IOD(+433)=25 !D*_s+
      IOD(-433)=26 !D*_s-
      IOD(+221)=27 !eta
      IOD(+223)=28 !omega
      IOD(+331)=29 !eta'
      IOD(+333)=30 !phi
      IOD(+443)=31 !J/psi
      IOD(+130)=32 !K_L0
      IOD(+310)=33 !K_S0

*...meson q-qbar state
      MESON(01,1,+2,-1)=+211
      MESON(02,1,+1,-2)=-211
      MESON(03,1,+1,-1)=+111
      MESON(03,2,+2,-2)=+111
      MESON(04,1,+2,-1)=+213
      MESON(05,1,+1,-2)=-213
      MESON(06,1,+1,-1)=+113
      MESON(06,2,+2,-2)=+113
      MESON(07,1,+1,-3)=+311
      MESON(08,1,+3,-1)=-311
      MESON(09,1,+1,-3)=+313
      MESON(10,1,+3,-1)=-313
      MESON(11,1,+2,-3)=+321
      MESON(12,1,+3,-2)=-321
      MESON(13,1,+2,-3)=+323
      MESON(14,1,+3,-2)=-323
      MESON(15,1,+4,-1)=+411
      MESON(16,1,+1,-4)=-411
      MESON(17,1,+4,-1)=+413
      MESON(18,1,+1,-4)=-413
      MESON(19,1,+4,-2)=+421
      MESON(20,1,+2,-4)=-421
      MESON(21,1,+4,-2)=+423
      MESON(22,1,+2,-4)=-423
      MESON(23,1,+4,-3)=+431
      MESON(24,1,+3,-4)=-431
      MESON(25,1,+4,-3)=+433
      MESON(26,1,+3,-4)=-433
      MESON(27,1,+1,-1)=+221
      MESON(27,2,+2,-2)=+221
      MESON(27,3,+3,-3)=+221
      MESON(28,1,+1,-1)=+223
      MESON(28,2,+2,-2)=+223
      MESON(29,1,+1,-1)=+331
      MESON(29,2,+2,-2)=+331
      MESON(29,3,+3,-3)=+331
      MESON(30,1,+3,-3)=+333
      MESON(31,1,+4,-4)=+443
      MESON(32,1,+1,-3)=+130
      MESON(32,2,+3,-1)=+130
      MESON(33,1,+1,-3)=+310
      MESON(33,2,+3,-1)=+310

*...meson's flavours
      MESONFLV(+211)=-21 !pi+ (ud~)
      MESONFLV(-211)=+12 !pi- (du~)
      MESONFLV(+213)=-21
      MESONFLV(-213)=+12
      MESONFLV(+311)=+13
      MESONFLV(-311)=-31
      MESONFLV(+313)=+13
      MESONFLV(-313)=-31
      MESONFLV(+321)=+23
      MESONFLV(-321)=-32
      MESONFLV(+323)=+23
      MESONFLV(-323)=-32
      MESONFLV(+411)=-41
      MESONFLV(-411)=+14
      MESONFLV(+413)=-41
      MESONFLV(-413)=+14
      MESONFLV(+421)=-42
      MESONFLV(-421)=+24
      MESONFLV(+423)=-42
      MESONFLV(-423)=+24
      MESONFLV(+431)=-43
      MESONFLV(-431)=+34
      MESONFLV(+433)=-43
      MESONFLV(-433)=+34
      MESONFLV(+111)=+11
      MESONFLV(+113)=+11
      MESONFLV(+221)=+11
      MESONFLV(+223)=+11
      MESONFLV(+333)=+33
      MESONFLV(+443)=+44
      MESONFLV(+130)=+13
      MESONFLV(+130)=-31
      MESONFLV(+310)=+13
      MESONFLV(+310)=-31
      END

C...Identify qurk and antiquark of flavour (F1,F2) as a meson
      !*************************!
      SUBROUTINE MESIDT(KL,KR,ID)
      !*************************!
      INTEGER MESONFLV(-500:+500),IT(10)
      INTEGER IOD(-1000:+1000),MESON(50,3,-6:+6,-6:+6)
      COMMON/MESONINF/IOD,MESON,MESONFLV
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /MESONINF/,/ARLWDAT1/
      HLF=0.5
      ID=0
      MAY=0
      R=RLU0(0)
      DO I=1,33
       DO J=1,3
        IF(MESON(I,J,KL,KR).NE.0)THEN
         MAY=MAY+1
         IT(MAY)=MESON(I,J,KL,KR)
        ENDIF
       ENDDO
      ENDDO
      IF(MAY.EQ.1)THEN
       ID=IT(MAY)
      ELSE
       MAXF=MAX(IABS(KL),IABS(KR))
       IF(MAXF.LE.2)THEN
        IF(R.LE.RALPA(73))THEN
         ID=IT(2)
        ELSE
         ID=IT(1)
        ENDIF
       ELSEIF(MAXF.EQ.3)THEN
        IF(R.LE.RALPA(74))THEN
         ID=IT(2)
        ELSE
         ID=IT(1)
        ENDIF
       ELSE
        WRITE(*,*)'No this case. check LUJGGG & MESIDT. Stop!'
        STOP
       ENDIF
      ENDIF
      END

      !***********************************!
      SUBROUTINE REASORTJTST(NBD,A,N,MM,NN)
      !***********************************!
      INTEGER CONFIG(50,10000,50),NCONFIG(50),D(50)
      REAL A(50,10000)
      COMMON/CONFIG/NCONFIG,CONFIG
      M=NN-MM+1
   10 IF(M.GT.0)THEN
       J=M+MM-2
       M=0
       DO I=MM,J
        IF(A(NBD,I).LT.A(NBD,I+1))THEN
         DA=A(NBD,I)
         A(NBD,I)=A(NBD,I+1)
         A(NBD,I+1)=DA
         M=I-MM+1
         DO IBD=1,NBD
          D(IBD)=CONFIG(NBD,I,IBD)
         ENDDO
         DO IBD=1,NBD
          CONFIG(NBD,I,IBD)=CONFIG(NBD,I+1,IBD)
         ENDDO
         DO IBD=1,NBD
          CONFIG(NBD,I+1,IBD)=D(IBD)
         ENDDO
        ENDIF
       ENDDO
       GOTO 10
      ENDIF
      RETURN
      END

      !***************************!
      SUBROUTINE INTSORT(A,N,MM,NN)
      !***************************!
      INTEGER A(10)
      M=NN-MM+1
   10 IF(M.GT.0)THEN
       J=M+MM-2
       M=0
       DO I=MM,J
        IF(A(I).GT.A(I+1))THEN
         D=A(I)
         A(I)=A(I+1)
         A(I+1)=D
         M=I-MM+1
        ENDIF
       ENDDO
       GOTO 10
      ENDIF
      RETURN
      END

      !************************************!
      SUBROUTINE NUMBIDENT(INT,N,NSORT,SORT)
      !************************************!
      INTEGER INT(10),JNT(0:10),SORT(6)
      JNT(0)=-1000000
      DO J=1,N
       JNT(J)=INT(J)
      ENDDO
      JID=1
      SORT(1)=1
      DO I=1,N-1
       IF(JNT(I).EQ.JNT(I+1))THEN
        SORT(JID)=SORT(JID)+1
       ELSEIF(JNT(I-1).EQ.JNT(I).AND.JNT(I).EQ.JNT(I+1))THEN
        SORT(JID)=SORT(JID)+1
       ELSE
        JID=JID+1
        SORT(JID)=1
       ENDIF
      ENDDO
      NSORT=JID
      END

      !********************************!
      SUBROUTINE REORDERFLV(INT,N,MM,NN)
      !********************************!
      INTEGER INT(6)
      M=NN-MM+1
   10 IF(M.GT.0)THEN
       J=M+MM-2
       M=0
       DO I=MM,J
        IF(INT(I).GT.INT(I+1))THEN
         JNT=INT(I)
         INT(I)=INT(I+1)
         INT(I+1)=JNT
         M=I-MM+1
        ENDIF
       ENDDO
       GOTO 10
      ENDIF
      RETURN
      END

C...Vertex spread
      !*******************!
      SUBROUTINE VERTEXREAL
      !*******************!
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /LUJETS/,/ARLWDAT1/
      IR=0   
    5 U=RLU0(0)
      IR=IR+1
      VT=RLU0(0)
      W=-2.0*LOG(U)
      T=6.283185307*VT
      G=SQRT(W)
      C=COS(T)
      S=SQRT(1.0-C*C)
      IF(IR.EQ.1)THEN
       VX=RALPA(97)*G*C
       VY=RALPA(98)*G*S
       GOTO 5
      ELSE
       VZ=RALPA(99)*G*C
      ENDIF
      DO J=1,N
       V(J,1)=V(J,1)+VX
       V(J,2)=V(J,2)+VY
       V(J,3)=V(J,3)+VZ
      ENDDO
      END

      !**********************************!
      SUBROUTINE REORDERREA(A,N,MM,NN,INT)
      !**********************************!
      INTEGER INT(N)
      REAL A(N)
      M=NN-MM+1
   10 IF(M.GT.0)THEN
       J=M+MM-2
       M=0
       DO I=MM,J
        IF(A(I).GT.A(I+1))THEN
         D=A(I)
         A(I)=A(I+1)
         A(I+1)=D
         IN=INT(I)
         INT(I)=INT(I+1)
         INT(I+1)=IN
         M=I-MM+1
        ENDIF
       ENDDO
       GOTO 10
      ENDIF
      RETURN
      END

*...particle mass in LUARLW
      !****************!
      SUBROUTINE GETMASS
      !****************!
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PARMASS/PARMASS(1000)
      
      END

      !**********************!
      SUBROUTINE OPENFILE(ECM)
      !**********************!
c      OPEN(UNIT=20,FILE='TWOBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=30,FILE='THRBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=40,FILE='FOUBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=50,FILE='FIVBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=60,FILE='SIXBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=70,FILE='SEVBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=80,FILE='EITBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=80,FILE='EITBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')
c      OPEN(UNIT=90,FILE='NINBDYCHPB.TEX',STATUS='UNKNOWN',
c     & ACCESS='SEQUENTIAL',FORM='FORMATTED')

      IF(1.95.LT.ECM.AND.ECM.LE.2.05)THEN
       OPEN(UNIT=50,FILE='chrghadmc2000',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.05.LT.ECM.AND.ECM.LE.2.15)THEN
       OPEN(UNIT=50,FILE='chrghadmc2100',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.15.LT.ECM.AND.ECM.LE.2.25)THEN
       OPEN(UNIT=50,FILE='chrghadmc2200',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.25.LT.ECM.AND.ECM.LE.2.35)THEN
       OPEN(UNIT=50,FILE='chrghadmc2300',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.35.LT.ECM.AND.ECM.LE.2.45)THEN
       OPEN(UNIT=50,FILE='chrghadmc2400',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.45.LT.ECM.AND.ECM.LE.2.55)THEN
       OPEN(UNIT=50,FILE='chrghadmc2500',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.55.LT.ECM.AND.ECM.LE.2.65)THEN
       OPEN(UNIT=50,FILE='chrghadmc2600',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.65.LT.ECM.AND.ECM.LE.2.75)THEN
       OPEN(UNIT=50,FILE='chrghadmc2700',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.75.LT.ECM.AND.ECM.LE.2.85)THEN
       OPEN(UNIT=50,FILE='chrghadmc2800',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.85.LT.ECM.AND.ECM.LE.2.95)THEN
       OPEN(UNIT=50,FILE='chrghadmc2900',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSEIF(2.95.LT.ECM.AND.ECM.LE.3.05)THEN
       OPEN(UNIT=50,FILE='chrghadmc3000',STATUS='UNKNOWN',
     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ELSE
       WRITE(*,*)'NOT READY FOR THE DATA FILE; CHECK. STOP NOW !'
c       OPEN(UNIT=50,FILE='chrghadmcxxxx',STATUS='UNKNOWN',
c     &      ACCESS='SEQUENTIAL',FORM='FORMATTED')
      ENDIF
      END

      !=============================!
      !     Update  in  JUly 2005   !
      !=============================!

C...Simulate event production at BEPC
      !************************!
      SUBROUTINE LUBEGN(KFL,ECM)
      !************************!
C...Purpose: to handle the generation of an e+e- annihilation jet event.
      IMPLICIT DOUBLE PRECISION(D)
      INTEGER NAROW(14),IPAMT(100)
      REAL RCR(15),RRESJ(14),FCR(0:15),RPAMT(100)
      REAL MSR(14),WER(14),WHR(14),WTR(14)
      REAL MRS(14),WEE(14),WHD(14),WTT(14)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA5/IDRES(14)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/CHECKTAG/DECAYTAG !CHECK THE DECAY 
      REAL*8 DECAYTAG
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/ARLWDAT1/,/ARLWDAT3/
      SAVE /RESDATA1/,/RESDATA2/,/RESDATA3/,/PARAMET/

      DECAYTAG=0;
C...check if the specified channel can production by threshold 
   10 TOTM=0
      NMAXRES=0
      IF(IALFU(8).GE.2)THEN
       DO J=1,IALFU(8)
        PA(J,5)=ULMASS0(IALFU(10+J))
        TOTM=TOTM+PA(J,5)
       ENDDO
       IF(TOTM.GE.ECM)THEN
        WRITE(*,*)'Ecm < Sum Mi. This channel can not produce.'
        WRITE(*,*)'STOP !'
        STOP
       ENDIF
      ENDIF
            
C...Check input parameters.
c      IF(MSTU(12).GE.1) CALL LULIST0(0)
      IF(KFL.LT.0.OR.KFL.GT.8) THEN   
       CALL LUERRM0(16,'(LUBEGN:) called with unknown flavour code')
       DECAYTAG=1
       return
c      IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(KFL.LE.5) ECMMIN=PARJ(127)+2.02*PARF(100+MAX(1,KFL))
      IF(KFL.GE.6) ECMMIN=PARJ(127)+2.02*PMAS(KFL,1)
      IF(ECM.LT.ECMMIN) THEN 
       CALL LUERRM0(16,'(LUBEGN:) called with too small CM energy')
       DECAYTAG=1
       RETURN 
c       print*,'DECAYTAG=',decaytag
c       IF(MSTU(21).GE.1) RETURN 
      ENDIF
      
C...Check consistency of MSTJ options set.
      IF(MSTJ(109).EQ.2.AND.MSTJ(110).NE.1) THEN
        CALL LUERRM0(6,'(LUBEGN:) MSTJ(109) value requires MSTJ(110)=1')
        MSTJ(110)=1 
      ENDIF 
      IF(MSTJ(109).EQ.2.AND.MSTJ(111).NE.0) THEN
        CALL LUERRM0(6,'(LUBEGN:) MSTJ(109) value requires MSTJ(111)=0')
        MSTJ(111)=0 
      ENDIF
      
C...Initialize alpha_strong and total cross-section.
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1))
     &MSTU(111)=1 
      PARU(112)=PARJ(121)
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122)
      CALL LUXTOT0(KFL,ECM,XTOT) 
      IF(MSTJ(116).GE.3) MSTJ(116)=1
      PARJ(171)=0.0
      
C...Add initial e+e- to event record (documentation only).
      NTRY=0 
  100 DO I=1,100
       DO J=1,5
        P(I,J)=0
        V(I,J)=0
        K(I,J)=0
       ENDDO
      ENDDO
      N=0 
      IALPA(99)=0
      NTRY=NTRY+1 
      IF(NTRY.GT.100) THEN
       CALL LUERRM0(14,'(LUBEGN:) caught in an infinite loop')
      ENDIF 
      MSTU(24)=0
      NC=0 
      IF(MSTJ(115).GE.2) THEN
       NC=NC+2 
       CALL LU1ENT0(NC-1,11,0.5*ECM,0.,0.)
       K(NC-1,1)=21 
       CALL LU1ENT0(NC,-11,0.5*ECM,PARU(1),0.)
       K(NC,1)=21
      ENDIF
      if(DECAYTAG.eq.1) return;
C...Radiative photon (in initial state).
  150 MK=0 
      ECMC=ECM
      IF(MSTJ(107).GE.1.AND.MSTJ(116).GE.1)THEN
       IF(IALFU(19).EQ.1)THEN
        CALL LURADK0(ECM,MK,PAK,THEK,PHIK,ALPK)  !JETSET scheme
       ELSEIF(IALFU(19).EQ.2)THEN
        CALL ARLWISR(ECM,MK,PAK,THEK,PHIK)       !Berends scheme
       ENDIF
      ENDIF 
      ECMC=ECM
      IF(MK.EQ.1)ECMC=SQRT(ECM*(ECM-2.0*PAK))
      SCMC=ECMC*ECMC
      RPAMT(5)=SCMC
      KAU(1)=MK
      XAU(2)=ECMC
      XAU(3)=PAK
      XAU(4)=THEK  ! 0 < theta < pi/2
      XAU(5)=PHIK
      XAU(6)=ALPK
      ECMMIN=PARJ(127)+2.0*PARF(101)
      IF(ECMC.LE.ECMMIN)GOTO 100
      IF(MSTJ(115).GE.1.AND.MK.EQ.1) THEN   
       NC=NC+1
       KAU(2)=NC 
       HLF=0.5
       IF(RLU0(0).LT.HLF)THEN
        K(NC,3)=1   !radiative gamma from e-
        KNC3=K(NC,3)    !original:  0 < theta < pi/2
        IF(RLU0(0).GT.HLF)THEK=PARU(1)-THEK   !pi/2 < the < pi
       ELSE
        K(NC,3)=2   !radiative gamma from e+
        KNC3=K(NC,3)
        IF(RLU0(0).LT.HLF)THEN
         THEK=THEK+PARU(1)   !pi < theta < 3*pi/2
        ELSE
         THEK=-THK           !3*pi/2 < theta < 2*pi
        ENDIF
       ENDIF 
       CALL LU1ENT0(NC,22,PAK,THEK,PHIK)
       K(NC,3)=KNC3
       if(decaytag.eq.1) return
      ENDIF
      
C...check if the specified channel can production after ISR
      TOTM=0.0
      IF(IALFU(8).GE.2)THEN
       DO J=1,IALFU(8)
        TOTM=TOTM+PA(J,5)
       ENDDO
       IF(TOTM.GE.ECMC)GOTO 150
      ENDIF
      IF(ECMC.LE.(RALPA(62)+RALPA(70)))GOTO 150
     
C...Virtual exchange gamma
      IF(MSTJ(115).GE.3) THEN 
       NC=NC+1 
       KF=22   
       IF(MSTJ(102).EQ.2) KF=23
       MSTU10=MSTU(10) 
       MSTU(10)=1
       IF(MK.LE.0)THEN 
        CALL LU1ENT0(NC,KF,ECMC,0.0,0.0)
        if(decaytag.eq.1) return
        P(NC,1)=0.0
        P(NC,2)=0.0
        P(NC,3)=0.0
        P(NC,4)=ECMC   
        P(NC,5)=0.0
       ELSE
        CALL LU1ENT0(NC,KF,ECMC,THEK,PHIK)
        if(decaytag.eq.1) return
        P(NC,1)=-P(NC-1,1)
        P(NC,2)=-P(NC-1,2)
        P(NC,3)=-P(NC-1,3)
        P(NC,4)=ECM-P(NC-1,4)   
        P(NC,5)=0.0
       ENDIF
       K(NC,1)=21 
       K(NC,3)=1  
       MSTU(10)=MSTU10
      ENDIF 
    
C...Determine how many flavours of q0q0bar included
      OS=RALPA(09)  !open strange-particle threshold 0.990
      OC=RALPA(11)  !open charmed-particle threshold 3.739
      IF(ECMC.LE.OS)THEN   
       MSTJ(104)=2  !uubar & ddbar
      ELSEIF(OS.LT.ECMC.AND.ECMC.LE.OC)THEN
       MSTJ(104)=3  !uubar & ddbar & ssbar
      ELSEIF(OC.LT.ECMC)THEN
       MSTJ(104)=4  !uubar & ddbar & ssbar & ccbar
      ENDIF
          
*...Determine PARJ(2) according to Ecm'
      IF(ECMC.LE.OS)THEN
       PARJ(2)=0
      ELSE
       IF(MSTJ(104).LE.2)THEN
        PARJ(2)=0
       ELSE
        X=RALPA(59)*(ECMC-OS)
        X1=RALPA(59)*(3-OS)
        X2=RALPA(59)*(5-OS)
        YP=EXP(+X)
        YN=EXP(-X)
        YP1=EXP(+X1)
        YN1=EXP(-X1)
        YP2=EXP(+X2)
        YN2=EXP(-X2)
        IF(ECMC.LE.3)THEN
         THX1=(YP1-YN1)/(YP1+YN1)
         A=RALPA(60)/THX1
         THX=(YP-YN)/(YP+YN)
cc        PARJ(2)=A*THX !pingrg
        ELSE
         TWO=2
         SECX=TWO/(YP+YN)
         SECX1=TWO/(YP1+YN1)
         SECX2=TWO/(YP2+YN2)
         C=(RALPA(60)-RALPA(61))/(SECX1-SECX2)
         B=(RALPA(61)*SECX1-RALPA(60)*SECX2)/(SECX1-SECX2)
cc         PARJ(2)=B+C*SECX !pingrg
        ENDIF
       ENDIF
      ENDIF
      
C...Check the consistence of flavours for fixed chanel
      IF(IALFU(8).GE.2)THEN
       MAXID=0
       DO J=1,IALFU(8)
        MID=MAX(MAXID,IABS(IALFU(10+J)))
        IF(MID.GT.MAXID)MAXID=MID
       ENDDO
       IF(1000.LT.MAXID.AND.MAXID.LT.10000)THEN   !including baryons
        IF(MAXID.LT.4000)THEN
         CONTINUE                 !not restriction non-charmed baryon
        ELSEIF(4000.LE.MAXID.AND.MAXID.LT.5000)THEN
         IF(KFLC.NE.4)GOTO 100    !initial quark-paire must be cc
        ELSE
         WRITE(*,*)'Not ready for this channel. Stop'
         STOP
        ENDIF
       ELSE                                       !only mesons of mass <B
        IF(MAXID.LT.400)THEN
         CONTINUE
        ELSEIF(400.LE.MAXID.AND.MAXID.LT.500)THEN
         IF(KFLC.NE.4)GOTO 100
        ELSE
         WRITE(*,*)'Not ready for this channel. Stop'
         STOP
        ENDIF
       ENDIF
      ENDIF
       
C...Determine channels to be produced
  151 IF(IALFU(18).EQ.1)THEN      !produce continuum only
       DO J=1,14
        FCR(J)=0
       ENDDO
       FCR(15)=1        
       ICHANELL=15
      ELSEIF(IALFU(18).GE.2)THEN  !produce both comtinuum and resonant
       NMAXRES=0
       DO J=1,IALFU(25)
        IECMTOM=ECMC/MRS(J)
        IF(IECMTOM.GE.1)NMAXRES=J
       ENDDO
       RC=RQCD(SCMC)
       RCSAV=RC
       SUMR=0
       CALL RESONDATA
       DO J=1,NMAXRES
        RRESJ(J)=RBWJ(J,SCMC)
        FCR(J)=FCR(J-1)+RRESJ(J)
        SUMR=SUMR+RRESJ(J)
       ENDDO
       RC=RC/(RC+SUMR)
       FCR(0)=0
       DO J=1,NMAXRES
        FCR(J)=FCR(J)/SUMR
       ENDDO
       JMAX=NMAXRES+1
       FCR(JMAX)=1.0
  155  RANC=RLU0(0)
       IF(RANC.LE.RC)THEN !continuum production
        ICHANELL=15       
       ELSE               !resonant production
        RJ=RLU0(0)
        ICHANELL=0
        DO J=1,JMAX
         IF(FCR(J-1).LE.RJ.AND.RJ.LT.FCR(J))ICHANELL=J
        ENDDO
       ENDIF
       IF(ICHANELL.LE.0.OR.ICHANELL.GE.16)GOTO 155
      ENDIF
      
C...Continoum hadronic state production
  160 IDR=IDRES(ICHANELL)
      IF(ICHANELL.NE.15)THEN
       IFRIGHT=0
       DO J=1,JMAX
        IF(IDR.EQ.IDRES(J))IFRIGHT=1
       ENDDO
       IF(IFRIGHT.LE.0)GOTO 10
      ENDIF
      IF(ICHANELL.LE.NMAXRES)THEN
c       WRITE(*,*)'Rcon =',RCSAV,' Rres =',SUMR       
c       WRITE(*,*)'e+e- => gamma => resonance :',IDR,ICHANELL,ECMC
       NC=NC+1
       IF(IDR.NE.443)THEN                                                                                
        CALL LUREVT(IDR) !non J/psi production and decay
       ELSEIF(ICHANELL.EQ.9)THEN
        CALL LUJDCY      !J/psi production and decay channels
       ENDIF
      ELSE
c       WRITE(*,*)'e+e- => gamma => continuum state :',ICHANELL,ECMC                    
       CALL LUCEVT(KFL)
       KAU(20)=0
      ENDIF
      IF(IALPA(99).GE.1)GOTO 150
c      IF(IALFU(99).EQ.1)CALL LULIST0(1)
      
C...continue the process after primary hadrons produced
  200 IF(MSTJ(21).GE.1)CALL LUEXEC0
     
C...Rotation and Angular orientation from radiative photon.
      IF(MK.EQ.1) THEN 
       DBEK=-PAK/(ECM-PAK)
       NMIN=NC+2-MSTJ(115)/3
       CALL LUDBRB0(NMIN,N,0.,-PHIK,0D0,0D0,0D0)
       CALL LUDBRB0(NMIN,N,ALPK,0.,DBEK*SIN(THEK),0D0,DBEK*COS(THEK))
       CALL LUDBRB0(NMIN,N,0.,PHIK,0D0,0D0,0D0) 
      ENDIF
      IF(IALFU(99).EQ.2)THEN
       WRITE(*,*)'IALFU(99) = ',IALFU(99)
c       CALL LULIST0(1)
      ENDIF 
      END

C...Continoum hadronic state production
      !********************!  
      SUBROUTINE LUCEVT(KFL)  
      !********************!
      IMPLICIT DOUBLE PRECISION(D)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/ARLWDAT1/,/ARLWDAT3/
      IALPA(99)=0
c      print*,'Eneter LUCEVT'
C...ratio of meson with spin 1 to 0
c      PARJ(11)=RALPA(73)  !pingrg
c      PARJ(12)=RALPA(74)  !pingrg

C...Choice of flavour.
      ECM=XAU(1)
      ECMC=XAU(2)
      NTRY=0
   10 NTRY=NTRY+1
      IF(NTRY.GT.1000)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      IF(IALFU(8).EQ.0)THEN
       CALL LUXKFL0(KFL,ECM,ECMC,KFLC)
       IF(KFLC.GE.5.OR.KFLC.LE.0)GOTO 10
      ELSE
       KFLC=KAU(4)
      ENDIF
      IF(KFLC.EQ.0.OR.KFLC.GE.6)GOTO 10
      KAU(11)=+KFLC
      KAU(12)=-KFLC
      MSTJ(120)=1
          
C...Estimation of maximum primary hadron number after ISR
      IF(KFLC.EQ.1.OR.KFLC.EQ.2)THEN     !uu or dd
       MSTJ24=MSTJ(24)
       MSTJ(24)=0
       PHM=ULMASS0(211)                    !~>pi mass
       MSTJ(24)=MSTJ24
       NPH=0
       DO I=2,6
        RI=I
        IPH=INT(ECMC/(RI*PHM))
        IF(IPH.GE.1)NPH=I
       ENDDO
       IF(NPH.LT.2)GOTO 10
       NPH=MIN(IALFU(7),NPH)
      ELSEIF(KFLC.EQ.3)THEN             !ss
       MSTJ(93)=1
       MSTJ24=MSTJ(24)
       MSTJ(24)=0
       PMH1=ULMASS0(321)                  !~>K+- mass
       MSTJ(24)=MSTJ24
       NPH=INT(ECMC/PMH1)
       IF(NPH.LT.2)GOTO 10
       NPH0=NPH
       MSTJ(93)=1
       MSTJ24=MSTJ(24)
       MSTJ(24)=0
       PMH2=ULMASS0(211)                  !>~pi mass
       MSTJ(24)=MSTJ24
       DO I=1,4
        RI=I
        IPH=INT(ECMC/(PMH1+PMH1+RI*PMH2))
        IF(IPH.GE.1)NPH=I+2
       ENDDO
       NPH=MAX(NPH0,NPH)
       NPH=MIN(IALFU(7),NPH)
      ELSEIF(KFLC.EQ.4)THEN                !cc
       PMH1=1.87                           !~D mass
       NPH=INT(ECMC/PMH1)
       IF(NPH.LT.2)GOTO 10
       NPH0=NPH
       MSTJ(93)=1
       MSTJ24=MSTJ(24)
       MSTJ(24)=0
       PMH2=ULMASS0(211)                  !~>pi mass
       MSTJ(24)=MSTJ24
       DO I=1,4
        RI=I   
	IPH=INT(ECMC/(PMH1+PMH1+RI*PMH2))
	IF(IPH.GE.1)NPH=I+2
       ENDDO   
       NPH=MAX(NPH0,NPH)
       NPH=MIN(IALFU(7),NPH)
      ENDIF
      KAU(10)=NPH
      RPH=NPH
             
C...Fill initial q0 & qbar0
      CALL LU2ENT0(N+1,KFLC,-KFLC,ECMC)
      K(N-1,1)=12
      K(N-0,1)=11
      K(N-1,3)=N-2
      K(N-0,3)=N-2
             
C...Angular orientation according to matrix element
      NC=N-2
      NJET=2
      IF(MSTJ(106).GE.1)THEN
       CALL LUXDIF0(NC,NJET,KFLC,ECMC,CHI,THE,PHI)
       IF(IALFU(17).EQ.2)CALL Q0Q0BARPOLAR(KFLC,ECMC,THE) 
       CALL LUDBRB0(NC+1,N,0.,CHI,0D0,0D0,0D0)    
       CALL LUDBRB0(NC+1,N,THE,PHI,0D0,0D0,0D0)
      ENDIF
            
C...Fill string
      N=N+1
      CALL LU1ENT0(N,92,P(N-1,4),0.0,0.0)
      K(N,1)=11
      K(N,3)=N-2       
      P(N,1)=P(N-1,1)+P(N-2,1)
      P(N,2)=P(N-1,2)+P(N-2,2)
      P(N,3)=P(N-1,3)+P(N-2,3)
      P(N,4)=P(N-1,4)+P(N-2,4)
      P(N,5)=P(N,4)
      KAU(3)=N
            
C...String fragmentation
      CALL LUALSF(-KFLC,+KFLC)
      RETURN
      END

C...Resonance hadronic state (rho, omiga) production
      !********************!  
      SUBROUTINE LUREVT(IDR)  
      !********************!
      IMPLICIT DOUBLE PRECISION(D)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/ARLWDAT1/,/ARLWDAT3/
      IALPA(99)=0
      ECM=XAU(1)
      ECMC=XAU(2)
      
C...Fill initial q0 & qbar0
      R=RLU0(0)
      IF(R.LE.0.2)THEN   !d:u=1:4
       KFLC=1   !d
      ELSE
       KFLC=2   !u
      ENDIF
      CALL LU2ENT0(N+1,KFLC,-KFLC,ECMC)
      K(N-1,1)=12
      K(N-0,1)=11
      K(N-1,3)=N-2
      K(N-0,3)=N-2
             
C...Angular orientation according to matrix element
      NC=N-2
      NJET=2
      IF(MSTJ(106).GE.1)THEN
       CALL LUXDIF0(NC,NJET,KFLC,ECMC,CHI,THE,PHI)
       CALL LUDBRB0(NC+1,N,0.,CHI,0D0,0D0,0D0)    
       CALL LUDBRB0(NC+1,N,THE,PHI,0D0,0D0,0D0)
      ENDIF
            
C...Fill string
      N=N+1
      CALL LU1ENT0(N,92,P(N-1,4),0.0,0.0)
      K(N,1)=11
      K(N,3)=N-2       
      P(N,1)=P(N-1,1)+P(N-2,1)
      P(N,2)=P(N-1,2)+P(N-2,2)
      P(N,3)=P(N-1,3)+P(N-2,3)
      P(N,4)=P(N-1,4)+P(N-2,4)
      P(N,5)=P(N,4)
      KAU(3)=N
            
C...String is treaded as resonance
      N=N+1
      K(N,1)=11
      K(N,2)=IDR
      K(N,3)=N-1
      P(N,1)=P(N-1,1)
      P(N,2)=P(N-1,2)
      P(N,3)=P(N-1,3)
      P(N,4)=P(N-1,4)
      MSTJ24=MSTJ(24)
      MSTJ(24)=3
      P(N,5)=ULMASS0(IDR)
      MSTJ(24)=MSTJ24
      MSTJ(22)=2
      CALL LUDECY0(N)
      K(N-1,1)=1
      K(N,1)=1
      RETURN
      END

C...J/psi production and decay channels
      !***************!
      SUBROUTINE LUJDCY
      !***************!
      REAL FBR(6)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUDAT01/,/ARLWDAT1/,/ARLWDAT3/
      KAU(8)=1  !flag of J/psi production and decay	

C...ratio of meson with spin 1 to 0
      PARJ(11)=RALPA(63)
      PARJ(12)=RALPA(64)

C...Branch ratio of J/psi decay
      CALL JDCYBRCHR(FBR)
      R=RLU0(0)

C...J/psi -> gamma -> e+e-
      IF(R.LT.FBR(1))THEN
   10  WRITE(*,*)'J/psi => gamma => e+e-'
       KAU(20)=1
       CALL LUJDLL(11)
      
C...J/psi -> gamma -> muon+muon-
      ELSEIF(FBR(1).LE.R.AND.R.LT.FBR(2))THEN
   20  WRITE(*,*)'J/psi => gamma => muon+muon-'
       KAU(20)=2
       CALL LUJDLL(13)

C...J/psi -> quark-antiquark
      ELSEIF(FBR(2).LE.R.AND.R.LT.FBR(3))THEN
   30  WRITE(*,*)'J/psi => gamma => quark-quark~ => hadrons'
       KAU(20)=3
       CALL LUJQQB

C...J/psi -> gluon-gluon-gluon
      ELSEIF(FBR(3).LE.R.AND.R.LT.FBR(4))THEN
   40  WRITE(*,*)'J/psi => gluon-gluon-gluon => hadrons'
       KAU(20)=4 
       CALL LUJGGG

C...J/psi -> gamma-gluon-gluon
      ELSEIF(FBR(4).LE.R.AND.R.LT.FBR(5))THEN
   50  WRITE(*,*)'J/psi => gamma-gluon-gluon => hadrons'
       KAU(20)=5
       CALL LUJRGG

C...J/psi -> gamma eta_c -> gamma-gamma and gluon-gluon -> hadrons
      ELSE
   60  WRITE(*,*)'J/psi => gamma eta_c => gluon-gluon => hadrons'
       KAU(20)=6 
       CALL LUJEGG
      ENDIF
      END

C...J/psi -> lepton-pair
      !********************!
      SUBROUTINE LUJDLL(KFL)
      !********************!
      IMPLICIT DOUBLE PRECISION(D)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/ARLWDAT3/

C...System energy 
      ECM=XAU(1)
      ECMC=XAU(2)

C...Fill virtual photon
      NC=N
      NC=NC+1
      CALL LU1ENT0(NC,22,P(N-1,4),0.0,0.0)
      P(N,1)=P(N-1,1)
      P(N,2)=P(N-1,2)
      P(N,3)=P(N-1,3)
      K(NC,1)=21  
      K(NC,3)=NC-1

C...Angular orientation (isotropica)
   20 THE=PARU(1)*RLU0(0)
      PHI=PARU(2)*RLU0(0)      

C...Fill lepton pair
      N=N+1
      E=0.5*ECMC
      CALL LU1ENT0(N,+KFL,E,+THE,+PHI)
      K(N,3)=N-1
      N=N+1
      CALL LU1ENT0(N,-KFL,E,THE+PARU(1),PHI)
      K(N-0,3)=N-2
      END

C...J/psi -> gamma -> quark-antiquark 
      !***************!
      SUBROUTINE LUJQQB
      !***************!
      IMPLICIT DOUBLE PRECISION(D)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/ARLWDAT1/,/ARLWDAT3/
     
C...Choice of flavour and jet configuration.
      ECM=XAU(1)
      ECMC=XAU(2)
      IALPA(99)=0
      KAU(10)=IALFU(7)
   10 KFL=0  !must be 0 here
      IF(IALFU(40).EQ.1)THEN
       CALL LUXKFL0(KFL,ECM,ECMC,KFLC)
       IF(KFLC.GE.4.OR.KFLC.LE.0)GOTO 10
      ELSEIF(IALFU(40).EQ.2)THEN
       CALL LUQKFL1(KFL,ECM,ECMC,KFLC)
       IF(KFLC.GE.4.OR.KFLC.LE.0)GOTO 10
      ELSEIF(IALFU(40).EQ.3)THEN
       CALL LUQKFL2(KFL,ECM,ECMC,KFLC)
      ENDIF
      NJET=2
      MSTJ(120)=1
      
C...Fill initinal quark and antiquark configuration
      CALL LU2ENT0(N+1,+KFLC,-KFLC,ECMC)
      K(N-1,3)=N-2
      K(N-0,3)=N-2
     
C...Angular orientation of initinal quark-antiquark according to 1+COS^2(THE)
      NC=N
      NJET=2
      MSTJ(120)=1
      CALL LUXDIF0(NC,NJET,KFLC,ECMC,CHI,THE,PHI)
      CALL LUDBRB0(NC+1,NC,0.,CHI,0D0,0D0,0D0)   
      CALL LUDBRB0(NC+1,NC,THE,PHI,0D0,0D0,0D0)
      
C...Rotation and Angular orientation from radiative photon.
      MK=KAU(1)     !flag of ISR photon emission
      PAK=XAU(3)    !momentum of ISR photon
      THEK=XAU(4)   !theta angle of ISR photon
      PHIK=XAU(5)   !phi angle of ISR photon
      ALPK=XAU(6)   !
      IF(MK.EQ.1) THEN 
       DBEK=-PAK/(ECM-PAK)
       CALL LUDBRB0(N-1,N,0.,-PHIK,0D0,0D0,0D0)
       CALL LUDBRB0(N-1,N,ALPK,0.,DBEK*SIN(THEK),0D0,DBEK*COS(THEK))
       CALL LUDBRB0(N-1,N,0.,PHIK,0D0,0D0,0D0) 
      ENDIF
      
C...Fill string
      N=N+1
      THES=THEK+PARU(1)
      PHIS=PHIK+PARU(1)
      CALL LU1ENT0(N,92,P(N-3,4),THES,PHIS)
      K(N,1)=11
      K(N,3)=N-2       
      P(N,1)=P(N-1,1)+P(N-2,1)
      P(N,2)=P(N-1,2)+P(N-2,2)
      P(N,3)=P(N-1,3)+P(N-2,3)
      P(N,4)=P(N-1,4)+P(N-2,4)
      P(N,5)=P(N,4)
      KAU(3)=N
      CALL LUEXEC0 
      
C...Quark-string fragmentation
      CALL LUQQSF(-KFLC,+KFLC)
      RETURN
      END

*...J/psi -> gluon-gluon-gluon
      !***************!
      SUBROUTINE LUJGGG
      !***************!
      INTEGER KF(3),KFSL(3),KFSR(3),NSH(0:3),MESONFLV(-500:+500)
      INTEGER IOD(-1000:+1000),MESON(50,3,-6:+6,-6:+6),NEXT(3)
      REAL SINV(3),SMIN(3),PSTR(3,5),VSTR(3,3),THES(3),PHIS(3)
      REAL PS(5),PI(5),PF(5),PIN(3),POU(3),EULER(3),XMIN(3)
      REAL PTX(10),PTY(10),Z(10),MS(10),MT(10),SMT(10),PL(10)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/MT/MT,SMT
      COMMON/MESONINF/IOD,MESON,MESONFLV
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/,/MT/,/MESONINF/
           
C...Printout. Check input parameters.
      XAU(1)=RALFU(1)
      ECM=XAU(1)
      ECMC=XAU(2)
      PAK=XAU(3) 
c      IF(MSTU(12).GE.1)CALL LULIST0(0) 
      IF(KAU(1).EQ.1)ECMC=SQRT(ECM*(ECM-2.0*XAU(6))) 
      IF(ECMC.LT.PARJ(127)+2.02*PARF(101)) THEN
       CALL LUERRM0(16,'(LUJGGG:) called with too small CM energy')
       IF(MSTU(21).GE.1)RETURN 
      ENDIF 

C...Choose x1 and x2 according to matrix element. 
      MK=0    
      ECMC=ECM
      NSAV=N
      NC=N
      NCSAV=NC 
  100 IRE=0
      N=NSAV
      NC=NCSAV
      DO ISTR=0,3
       NSH(ISTR)=0
      ENDDO
  105 X1=RLU0(0) 
      X2=RLU0(0) 
      X3=2.-X1-X2 
      IF(X3.GE.1..OR.((1.-X1)/(X2*X3))**2+((1.-X2)/(X1*X3))**2+ 
     &((1.-X3)/(X1*X2))**2.LE.2.*RLU0(0)) GOTO 105 
      NJET=3 
      IF(MSTJ(101).LE.4) CALL LU3ENT0(NC+1,21,21,21,ECMC,X1,X3) 
      IF(MSTJ(101).GE.5) CALL LU3ENT0(-(NC+1),21,21,21,ECMC,X1,X3) 
      K(N-2,3)=N-3
      K(N-1,3)=N-3
      K(N-0,3)=N-3
     
C...Differential cross-sections. Upper limit for cross-section. 
      IF(MSTJ(106).EQ.1) THEN 
       SQ2=SQRT(2.) 
       HF1=1.-PARJ(131)*PARJ(132) 
       HF3=PARJ(133)**2 
       CT13=(X1*X3-2.*X1-2.*X3+2.)/(X1*X3) 
       ST13=SQRT(1.-CT13**2) 
       SIGL=0.5*X3**2*((1.-X2)**2+(1.-X3)**2)*ST13**2 
       SIGU=(X1*(1.-X1))**2+(X2*(1.-X2))**2+(X3*(1.-X3))**2-SIGL 
       SIGT=0.5*SIGL 
       SIGI=(SIGL*CT13/ST13+0.5*X1*X3*(1.-X2)**2*ST13)/SQ2 
       SIGMAX=(2.*HF1+HF3)*ABS(SIGU)+2.*(HF1+HF3)*ABS(SIGL)
     & +2.*(HF1+2.*HF3)*ABS(SIGT)+2.*SQ2*(HF1+2.*HF3)*ABS(SIGI) 
    
C...Angular orientation of event. 
  120  CHI=PARU(2)*RLU0(0) 
       CTHE=2.*RLU0(0)-1. 
       PHI=PARU(2)*RLU0(0) 
       CCHI=COS(CHI) 
       SCHI=SIN(CHI) 
       C2CHI=COS(2.*CHI) 
       S2CHI=SIN(2.*CHI) 
       THE=ACOS(CTHE) 
       STHE=SIN(THE) 
       C2PHI=COS(2.*(PHI-PARJ(134))) 
       S2PHI=SIN(2.*(PHI-PARJ(134))) 
       SIG=((1.+CTHE**2)*HF1+STHE**2*C2PHI*HF3)*SIGU+2.*(STHE**2*HF1- 
     & STHE**2*C2PHI*HF3)*SIGL+2.*(STHE**2*C2CHI*HF1+((1.+CTHE**2)* 
     & C2CHI*C2PHI-2.*CTHE*S2CHI*S2PHI)*HF3)*SIGT-2.*SQ2*(2.*STHE*CTHE* 
     & CCHI*HF1-2.*STHE*(CTHE*CCHI*C2PHI-SCHI*S2PHI)*HF3)*SIGI 
       IF(SIG.LT.SIGMAX*RLU0(0)) GOTO 120 
       CALL LUDBRB0(NC+1,N,0.,CHI,0D0,0D0,0D0) 
       CALL LUDBRB0(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 
      K(N-2,1)=12
      K(N-1,1)=12
      K(N-0,1)=11

C...Direction vector of the three strings 
      SUM1=0.0
      SUM2=0.0
      SUM3=0.0
      DO I=1,3
       VSTR(1,I)=(P(N-1,I)-P(N-2,I))/2.0
       VSTR(2,I)=(P(N-0,I)-P(N-1,I))/2.0
       VSTR(3,I)=(P(N-2,I)-P(N-0,I))/2.0
       SUM1=SUM1+VSTR(1,I)*VSTR(1,I)
       SUM2=SUM2+VSTR(2,I)*VSTR(2,I)
       SUM3=SUM3+VSTR(3,I)*VSTR(3,I)
      ENDDO
      SUM1=SQRT(SUM1)
      SUM2=SQRT(SUM2)
      SUM3=SQRT(SUM3)

C...Directions of the three strings (unit vector)
      DO I=1,3
       VSTR(1,I)=VSTR(1,I)/SUM1
       VSTR(2,I)=VSTR(2,I)/SUM2
       VSTR(3,I)=VSTR(3,I)/SUM3
      ENDDO
    
C...Cossin and Angles of the strings
      COSTHE1=VSTR(1,3)
      COSTHE2=VSTR(2,3)
      COSTHE3=VSTR(3,3)
      THES(1)=ACOS(COSTHE1)
      THES(2)=ACOS(COSTHE2)
      THES(3)=ACOS(COSTHE3)

C...flavor for three gluons -> 3 (q and qbar)
      A=1.0/3.0
      B=2.0/3.0
      DO I=1,3
       R=RLU0(0)
       IF(R.LE.A)THEN
        KF(I)=1
       ELSEIF(A.LT.R.AND.R.LE.B)THEN
        KF(I)=2
       ELSE
        KF(I)=3
       ENDIF
      ENDDO
            
C...Quark-antiqurks flavours of the three strings
      KFSL(1)=+KF(1)
      KFSR(1)=-KF(2)
      KFSL(2)=+KF(2)
      KFSR(2)=-KF(3)
      KFSL(3)=+KF(3)
      KFSR(3)=-KF(1)

C...Quark-antiqurk data from gluon 1
      N=N+1               !q~1
      PQ=P(N-3,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,KFSR(3),PQ,0.,0.)
      P(N,1)=P(N-3,1)/2.0
      P(N,2)=P(N-3,2)/2.0
      P(N,3)=P(N-3,3)/2.0
      P(N,4)=P(N-3,4)/2.0
      P(N,5)=ULMASS0(KFSR(3))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=KFSR(3)
      K(N,3)=N-3
      K(N,4)=0
      K(N,5)=0  
      N=N+1               !q1
      PQ=P(N-4,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,KFSL(1),PQ,0.,0.)
      P(N,1)=P(N-4,1)/2.0
      P(N,2)=P(N-4,2)/2.0
      P(N,3)=P(N-4,3)/2.0
      P(N,4)=P(N-4,4)/2.0
      P(N,5)=ULMASS0(KFSL(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=KFSL(1)
      K(N,3)=N-4
      K(N,4)=0
      K(N,5)=0  
      N=N+1               !q~2
      PQ=P(N-3,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,KFSR(1),PQ,0.,0.)
      P(N,1)=P(N-4,1)/2.0
      P(N,2)=P(N-4,2)/2.0
      P(N,3)=P(N-4,3)/2.0
      P(N,4)=P(N-4,4)/2.0
      P(N,5)=ULMASS0(KFSR(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=KFSR(1)
      K(N,3)=N-4
      K(N,4)=0
      K(N,5)=0  
      N=N+1               !q2
      PQ=P(N-4,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,KFSR(1),PQ,0.,0.)
      P(N,1)=P(N-5,1)/2.0
      P(N,2)=P(N-5,2)/2.0
      P(N,3)=P(N-5,3)/2.0
      P(N,4)=P(N-5,4)/2.0
      P(N,5)=ULMASS0(KFSR(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=KFSL(2)
      K(N,3)=N-5
      K(N,4)=0
      K(N,5)=0  
      N=N+1               !q~3
      PQ=P(N-4,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,KFSR(2),PQ,0.,0.)
      P(N,1)=P(N-5,1)/2.0
      P(N,2)=P(N-5,2)/2.0
      P(N,3)=P(N-5,3)/2.0
      P(N,4)=P(N-5,4)/2.0
      P(N,5)=ULMASS0(KFSR(2))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=KFSR(2)
      K(N,3)=N-5
      K(N,4)=0
      K(N,5)=0  
      N=N+1               !q3
      PQ=P(N-4,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,KFSL(3),PQ,0.,0.)
      P(N,1)=P(N-6,1)/2.0
      P(N,2)=P(N-6,2)/2.0
      P(N,3)=P(N-6,3)/2.0
      P(N,4)=P(N-6,4)/2.0
      P(N,5)=ULMASS0(KFSL(3))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=KFSL(3)
      K(N,3)=N-6
      K(N,4)=0  
      K(N,5)=0  
     
C...The minimum invariant masses for the each string
      DO I=1,3
       MID=IABS(KFSL(I))+IABS(KFSR(I))
       IF(MID.LE.4)THEN
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(91)                       !~one pion mass
        ELSE
         XMIN(I)=RALPA(91)+RALPA(91)             !~two pion mass
        ENDIF
       ELSEIF(MID.EQ.5)THEN
        IF(IALFU(22).EQ.1)THEN !permit small string identify as meson
         XMIN(I)=RALPA(92)                       !~one K mass
        ELSE                   !small string >= 2,3...mesons
         XMIN(I)=RALPA(91)+RALPA(92)             !~one pion and one K mass
        ENDIF
       ELSEIF(MID.EQ.6)THEN
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(93)                       !~one eta mass
        ELSE
         XMIN(I)=RALPA(92)+RALPA(92)             !~two K  mass
        ENDIF
       ENDIF
       SMIN(I)=XMIN(I)*XMIN(I)
      ENDDO
           
C...Three strings 5-momentums & invariant masses
      DO I=1,4
       PSTR(1,I)=P(N-3,I)+P(N-4,I)
       PSTR(2,I)=P(N-1,I)+P(N-2,I)
       PSTR(3,I)=P(N-5,I)+P(N-0,I)
      ENDDO
      DO J=1,3
       EE=PSTR(J,4)*PSTR(J,4)
       PP=PSTR(J,1)*PSTR(J,1)+PSTR(J,2)*PSTR(J,2)+PSTR(J,3)*PSTR(J,3)
       PSTR(J,5)=SQRT(EE-PP)
      ENDDO
      IRE=0
      DO ISTR=1,3
       SINV(ISTR)=PSTR(ISTR,4)*PSTR(ISTR,4)
       DO J=1,3
        SINV(ISTR)=SINV(ISTR)-PSTR(ISTR,J)*PSTR(ISTR,J)
       ENDDO
       IF(SINV(ISTR).LE.SMIN(ISTR))IRE=IRE+1
      ENDDO
      IF(IRE.GT.0)GOTO 100
      WRITE(66,66)SQRT(SINV(1)),SQRT(SINV(2)),SQRT(SINV(3))
   66 FORMAT(F10.6,1X,F10.6,1X,F10.6)

C...Fill string 1
      N=N+1
      CALL LU1ENT0(N,92,PSTR(1,4),THES(1),PHIS(1))
      K(N,1)=11
      K(N,3)=N-5
      P(N,1)=PSTR(1,1)
      P(N,2)=PSTR(1,2)
      P(N,3)=PSTR(1,3)
      P(N,4)=PSTR(1,4)
      P(N,5)=PSTR(1,5)
      KAU(17)=N
    
C...Fill string 2
      N=N+1
      CALL LU1ENT0(N,92,PSTR(2,4),THES(2),PHIS(2))
      K(N,1)=11
      K(N,3)=N-4
      P(N,1)=PSTR(2,1)        
      P(N,2)=PSTR(2,2)        
      P(N,3)=PSTR(2,3)        
      P(N,4)=PSTR(2,4)
      P(N,5)=PSTR(2,5)
      KAU(18)=N

C...Fill string 3
      N=N+1
      CALL LU1ENT0(N,92,PSTR(3,4),THES(3),PHIS(3))
      K(N,1)=11
      K(N,3)=N-3
      P(N,1)=PSTR(3,1)        
      P(N,2)=PSTR(3,2)        
      P(N,3)=PSTR(3,3)        
      P(N,4)=PSTR(3,4)
      P(N,5)=PSTR(3,5)
      KAU(3)=N
      KAU(19)=N
      
C...Srtings fragment into hadrons or identify to hadron
  300 IRE=0
      DO ISTR=1,3          !loop for three strings fragment separately
       XAU(2)=SQRT(SINV(ISTR))
       ECL=XAU(2)
       KAU(10)=XAU(2)/XMIN(ISTR)
       KAU(10)=MIN(IALFU(20),KAU(10))  !maximum primary hadron number
       IF(KAU(10).GE.2)THEN            !string->2,3...particles
        CALL LUGLSF(ISTR,PSTR,KFSL,KFSR,NSH,Z,PTX,PTY) !string fragmentation
        IF(IALPA(99).LE.0)THEN
  400    EULER(1)=PARU(2)*RLU0(0)       !phi
         EULER(2)=THES(ISTR)            !theta
         EULER(3)=PARU(2)*RLU0(0)       !psi
         IF(ISTR.EQ.1)THEN              !total number of entries
          N=KAU(3)+NSH(1)
          NEXT(ISTR)=KAU(3)+1
         ELSEIF(ISTR.EQ.2)THEN
          N=KAU(3)+NSH(1)+NSH(2)
          NEXT(ISTR)=KAU(3)+NSH(1)+1
         ELSEIF(ISTR.EQ.3)THEN
          N=KAU(3)+NSH(1)+NSH(2)+NSH(3)
          NEXT(ISTR)=KAU(3)+NSH(1)+NSH(2)+1 
         ENDIF
         DO I=NEXT(ISTR),N
          J=I-NEXT(ISTR)+1
          MSTJ24=MSTJ(24)
          MSTJ(24)=0
          MS(J)=ULMASS0(KA(J,2))
          MSTJ(24)=MSTJ24
          DO L=1,5   
           K(I,L)=KA(J,L)
           V(I,L)=VA(J,L)
          ENDDO    
          PL(J)=(Z(J)*ECL-MT(J)*MT(J)/Z(J)/ECL)/2.0
          PIN(1)=PTX(J)  
          PIN(2)=PTY(J)  
          PIN(3)=PL(J)
          PINL=PIN(1)*PIN(1)+PIN(2)*PIN(2)+PIN(3)*PIN(3)
          CALL EULERROT(-1,EULER,PIN,POU)
          POUL=POU(1)*POU(1)+POU(2)*POU(2)+POU(3)*POU(3)
          DO M=1,5
           PS(M)=PSTR(ISTR,M)    !string 5-momentum
          ENDDO
          DO M=1,3
           PI(M)=POU(M)
          ENDDO
          PP=PI(1)*PI(1)+PI(2)*PI(2)+PI(3)*PI(3)
          PI(4)=SQRT(PP+MS(J)*MS(J))
          PI(5)=MS(J)
          K(I,3)=KAU(16+ISTR)
          CALL CMSTOLAB(PS,PI,PF)   !translate from remainder CMS to LABS
          DO M=1,5
           P(I,M)=PF(M)
          ENDDO
         ENDDO
        ELSE
         IRE=IRE+1
        ENDIF
       ELSE                         !string identifys to one particle
        ITRY=0
  500   IL=KFSL(ISTR)
        IR=KFSR(ISTR)
        CALL MESIDT(IL,IR,ID)
        IF(ID.EQ.0)THEN
         IRE=IRE+1
        ELSE
         NSH(ISTR)=1
         MSTJ24=MSTJ(24)
         MSTJ(24)=0
         N=N+1
         K(N,1)=1
         K(N,2)=ID
         K(N,3)=KAU(16+ISTR)
         K(N,4)=0
         K(N,5)=0
         DO J=1,4
          P(N,J)=PSTR(ISTR,J)
          V(N,J)=V(KAU(2),J)
         ENDDO
         P(N,5)=ULMASS0(ID)
         MSTJ(24)=MSTJ24         
         IF(P(N,5).GE.PSTR(ISTR,5))ITRY=ITRY+1
         IF(1.LE.ITRY.AND.ITRY.LE.10)THEN
          IRE=IRE+1
          GOTO 500
         ELSEIF(ITRY.GT.10)THEN
          GOTO 100
         ENDIF
         V(N,5)=0
        ENDIF
       ENDIF
      ENDDO 
      IF(IRE.GT.0)GOTO 100
      CALL LUEXEC0
      END 

*...J/psi -> gamma-gluon-gluon
      !***************!
      SUBROUTINE LUJRGG
      !***************!
      INTEGER KF(2),KFSL(3),KFSR(3),NSH(0:3),MESONFLV(-500:+500)
      INTEGER IOD(-1000:+1000),MESON(50,3,-6:+6,-6:+6),NEXT(3)
      REAL SINV(3),SMIN(3),PSTR(3,5),VSTR(3,3),THES(3),PHIS(3)
      REAL PS(5),PI(5),PF(5),PIN(3),POU(3),EULER(3),XMIN(3)
      REAL PTX(10),PTY(10),Z(10),MS(10),MT(10),SMT(10),PL(10)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/MT/MT,SMT
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/,/MT/

C...Printout. Check input parameters. 
      XAU(1)=RALFU(1)
      ECM=XAU(1)
      ECMC=XAU(2)
      PAK=XAU(3)
c      IF(MSTU(12).GE.1)CALL LULIST0(0)
      IF(KAU(1).EQ.1)ECMC=SQRT(ECM*(ECM-2.0*XAU(6)))
      IF(ECMC.LT.PARJ(127)+2.02*PARF(101)) THEN 
       CALL LUERRM0(16,'(LUJRGG:) called with too small CM energy') 
       IF(MSTU(21).GE.1) RETURN 
      ENDIF 

C...Choose x1 and x2 according to matrix element. 
      MK=0    
      ECMC=ECM
      NTRY=0 
      NSAV=N  
      NC=N
      NCSAV=NC
  100 N=NSAV  
      NC=NCSAV
      DO ISTR=0,3
       NSH(ISTR)=0
      ENDDO
      X1=RLU0(0) 
      X2=RLU0(0) 
      X3=2.-X1-X2 
      IF(X3.GE.1..OR.((1.-X1)/(X2*X3))**2+((1.-X2)/(X1*X3))**2+ 
     &((1.-X3)/(X1*X2))**2.LE.2.*RLU0(0)) GOTO 100 
      NJET=3 
      IF(MSTJ(101).LE.4) CALL LU3ENT0(NC+1,21,21,21,ECMC,X1,X3) 
      IF(MSTJ(101).GE.5) CALL LU3ENT0(-(NC+1),21,21,21,ECMC,X1,X3) 
 
C...Photon-gluon-gluon events. Small system modifications. Jet origin. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      QF=0. 
      IF(KFLC.NE.0) QF=KCHG(KFLC,1)/3. 
      RGAM=7.2*QF**2*PARU(101)/ULALPS0(ECMC**2) 
      MK=0 
      RGAM=1.0E+30 !huhm 
      IF(RLU0(0).GT.RGAM/(1.+RGAM)) THEN 
       IF(1.-MAX(X1,X2,X3).LE.MAX((PARJ(126)/ECM)**2,PARJ(125))) 
     & NJET=2 
       IF(NJET.EQ.2.AND.MSTJ(101).LE.4) CALL LU2ENT0(NC+1,21,21,ECMC) 
       IF(NJET.EQ.2.AND.MSTJ(101).GE.5) CALL LU2ENT0(-(NC+1),21,21,ECMC) 
      ELSE 
       MK=1 
       ECMJ=SQRT(1.-X1)*ECMC 
       IF(ECMJ.LT.2.*PARJ(127)) GOTO 100 
       K(NC+1,1)=1 
       K(NC+1,2)=22 
       K(NC+1,4)=0 
       K(NC+1,5)=0 
       IF(MSTJ(101).GE.5) K(NC+2,4)=MSTU(5)*(NC+3) 
       IF(MSTJ(101).GE.5) K(NC+2,5)=MSTU(5)*(NC+3) 
       IF(MSTJ(101).GE.5) K(NC+3,4)=MSTU(5)*(NC+2) 
       IF(MSTJ(101).GE.5) K(NC+3,5)=MSTU(5)*(NC+2) 
       NJET=2 
       IF(ECMJ.LT.4.*PARJ(127)) THEN 
        MSTU10=MSTU(10) 
        MSTU(10)=1 
        P(NC+2,5)=ECMJ 
        CALL LU1ENT0(NC+2,83,0.5*(X2+X3)*ECMC,PARU(1),0.) 
        MSTU(10)=MSTU10 
        NJET=0 
       ENDIF 
      ENDIF 
      DO 110 IP=NC+1,N                    !here 5(b)->4(4)  huhm
      K(IP,3)=K(IP,3)+(MSTJ(115)/2)+(KFLC/4)*(MSTJ(115)/3)*(NC-1) 
  110 CONTINUE 
 
C...Differential cross-sections. Upper limit for cross-section. 
      IF(MSTJ(106).EQ.1) THEN 
       SQ2=SQRT(2.) 
       HF1=1.-PARJ(131)*PARJ(132) 
       HF3=PARJ(133)**2 
       CT13=(X1*X3-2.*X1-2.*X3+2.)/(X1*X3) 
       ST13=SQRT(1.-CT13**2) 
       SIGL=0.5*X3**2*((1.-X2)**2+(1.-X3)**2)*ST13**2 
       SIGU=(X1*(1.-X1))**2+(X2*(1.-X2))**2+(X3*(1.-X3))**2-SIGL 
       SIGT=0.5*SIGL 
       SIGI=(SIGL*CT13/ST13+0.5*X1*X3*(1.-X2)**2*ST13)/SQ2 
       SIGMAX=(2.*HF1+HF3)*ABS(SIGU)+2.*(HF1+HF3)*ABS(SIGL)+2.*(HF1+ 
     & 2.*HF3)*ABS(SIGT)+2.*SQ2*(HF1+2.*HF3)*ABS(SIGI) 
 
C...Angular orientation of event. 
  120  CHI=PARU(2)*RLU0(0) 
       CTHE=2.*RLU0(0)-1. 
       PHI=PARU(2)*RLU0(0) 
       CCHI=COS(CHI) 
       SCHI=SIN(CHI) 
       C2CHI=COS(2.*CHI) 
       S2CHI=SIN(2.*CHI) 
       THE=ACOS(CTHE) 
       STHE=SIN(THE) 
       C2PHI=COS(2.*(PHI-PARJ(134))) 
       S2PHI=SIN(2.*(PHI-PARJ(134))) 
       SIG=((1.+CTHE**2)*HF1+STHE**2*C2PHI*HF3)*SIGU+2.*(STHE**2*HF1- 
     & STHE**2*C2PHI*HF3)*SIGL+2.*(STHE**2*C2CHI*HF1+((1.+CTHE**2)* 
     & C2CHI*C2PHI-2.*CTHE*S2CHI*S2PHI)*HF3)*SIGT-2.*SQ2*(2.*STHE*CTHE* 
     & CCHI*HF1-2.*STHE*(CTHE*CCHI*C2PHI-SCHI*S2PHI)*HF3)*SIGI 
       IF(SIG.LT.SIGMAX*RLU0(0)) GOTO 120 
       CALL LUDBRB0(NC+1,N,0.,CHI,0D0,0D0,0D0) 
       CALL LUDBRB0(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 
      K(N-1,1)=12
      K(N-0,1)=11
      K(N-2,3)=4
      K(N-1,3)=4
      K(N-0,3)=4

C...Directions of the two strings (unit vector)
      SUM1=0
      SUM2=0
      DO I=1,3
       VSTR(1,I)=(P(N-1,I)-P(N-0,I))/2.0
       VSTR(2,I)=(P(N-0,I)-P(N-1,I))/2.0
       SUM1=SUM1+VSTR(1,I)*VSTR(1,I)
       SUM2=SUM2+VSTR(2,I)*VSTR(2,I)
      ENDDO
      SUM1=SQRT(SUM1)
      SUM2=SQRT(SUM2)

C...Directions of the two strings (unit vector)
      DO I=1,3
       VSTR(1,I)=VSTR(1,I)/SUM1
       VSTR(2,I)=VSTR(2,I)/SUM2
      ENDDO

C...Cossin and polar angles of the strings
      COSTHE1=VSTR(1,3)
      COSTHE2=VSTR(2,3)
      THES(1)=ACOS(COSTHE1) 
      THES(2)=ACOS(COSTHE2) 

C...flavor for two gluons -> 2(q and qbar)
      A=1.0/3.0
      B=2.0/3.0
      DO I=1,2 
       R=RLU0(0)
       IF(R.LE.A)THEN
	KF(I)=1
       ELSEIF(A.LT.R.AND.R.LE.B)THEN
	KF(I)=2
       ELSE
        KF(I)=3
       ENDIF
      ENDDO

C...Quark-antiqurks flavours of the three strings
      KFSL(1)=+KF(1)
      KFSR(1)=-KF(2)
      KFSL(2)=+KF(2) 
      KFSR(2)=-KF(1)

C...Quark-antiqurk data from gluon 1
      N=N+1               !q1
      PQ=P(N-2,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,+KF(1),PQ,0.,0.)
      P(N,1)=P(N-2,1)/2.0
      P(N,2)=P(N-2,2)/2.0
      P(N,3)=P(N-2,3)/2.0
      P(N,4)=P(N-2,4)/2.0
      P(N,5)=ULMASS0(+KF(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=+KF(1)
      K(N,3)=N-2
      K(N,4)=0  
      K(N,5)=0  
      N=N+1               !q~1
      PQ=P(N-3,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,-KF(1),PQ,0.,0.)
      P(N,1)=P(N-3,1)/2.0
      P(N,2)=P(N-3,2)/2.0
      P(N,3)=P(N-3,3)/2.0
      P(N,4)=P(N-3,4)/2.0
      P(N,5)=ULMASS0(-KF(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=-KF(1)
      K(N,3)=N-3
      K(N,4)=0  
      K(N,5)=0  
      N=N+1               !q2
      PQ=P(N-3,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,+KF(2),PQ,0.,0.)
      P(N,1)=P(N-3,1)/2.0
      P(N,2)=P(N-3,2)/2.0
      P(N,3)=P(N-3,3)/2.0
      P(N,4)=P(N-3,4)/2.0
      P(N,5)=ULMASS0(+KF(2))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=+KF(2)
      K(N,3)=N-3
      K(N,4)=0  
      K(N,5)=0  
      N=N+1               !q~2
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      PQ=P(N-3,4)/2.0
      CALL LU1ENT0(N,-KF(2),PQ,0.,0.)
      P(N,1)=P(N-4,1)/2.0
      P(N,2)=P(N-4,2)/2.0
      P(N,3)=P(N-4,3)/2.0
      P(N,4)=P(N-4,4)/2.0
      P(N,5)=ULMASS0(-KF(2))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=-KF(2)
      K(N,3)=N-4
      K(N,4)=0  
      K(N,5)=0  

C...The minimum invariant masses for the each string
      IALPA(99)=0   
      MID=IABS(KF(1))+IABS(KF(2))
      DO I=1,2
       IF(MID.LE.4)THEN
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(91)                       !~one pion mass
        ELSE
         XMIN(I)=RALPA(91)+RALPA(91)		 !~two pion mass
        ENDIF
       ELSEIF(MID.EQ.5)THEN
        IF(IALFU(22).EQ.1)THEN !permit small string identify as meson
         XMIN(I)=RALPA(92)                       !~one K mass
        ELSE                   !small string >= 2,3...mesons
         XMIN(I)=RALPA(91)+RALPA(92)		 !~one pion and one K mass
        ENDIF
       ELSEIF(MID.EQ.6)THEN   
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(93)                       !~one eta mass
        ELSE
         XMIN(I)=RALPA(92)+RALPA(92)		 !~two K  mass
        ENDIF
       ENDIF 
       SMIN(I)=XMIN(I)*XMIN(I)
      ENDDO

C...Two strings 5-momentums & invariant masses
      DO I=1,4
       PSTR(1,I)=P(N-3,I)+P(N-0,I)
       PSTR(2,I)=P(N-1,I)+P(N-2,I)
      ENDDO
      IRE=0
      DO J=1,2
       EE=PSTR(J,4)*PSTR(J,4)
       PP=PSTR(J,1)*PSTR(J,1)+PSTR(J,2)*PSTR(J,2)+PSTR(J,3)*PSTR(J,3)
       PSTR(J,5)=SQRT(EE-PP)
       SINV(ISTR)=EE-PP
       IF(SINV(ISTR).LE.SMIN(ISTR))IRE=IRE+1
      ENDDO
      IF(IRE.GT.0)GOTO 100
      WRITE(77,77)SQRT(SINV(1)),SQRT(SINV(2))
   77 FORMAT(F10.6,1X,F10.6)

C...Fill string 1
      N=N+1
      CALL LU1ENT0(N,92,PSTR(1,4),THES(1),PHIS(1))
      K(N,1)=11
      K(N,3)=N-4
      P(N,1)=PSTR(1,1)
      P(N,2)=PSTR(1,2)
      P(N,3)=PSTR(1,3)
      P(N,4)=PSTR(1,4)
      P(N,5)=PSTR(1,5)   
      KAU(17)=N

C...Fill string 2
      N=N+1
      CALL LU1ENT0(N,92,PSTR(2,4),THES(2),PHIS(2))
      K(N,1)=11
      K(N,3)=N-3
      P(N,1)=PSTR(2,1)
      P(N,2)=PSTR(2,2)
      P(N,3)=PSTR(2,3)
      P(N,4)=PSTR(2,4)
      P(N,5)=PSTR(2,5)
      KAU(3)=N  
      KAU(18)=N
      
C...Srtings fragment into hadrons or identify to hadron
  300 IRE=0
      DO ISTR=1,2          !loop for three strings fragment separately
       XAU(2)=SQRT(SINV(ISTR))
       ECL=XAU(2)
       KAU(10)=XAU(2)/XMIN(ISTR)
       KAU(10)=MIN(IALFU(20),KAU(10))  !maximum primary hadron number
       IF(KAU(10).GE.2)THEN            !string->2,3...particles
        CALL LUGLSF(ISTR,PSTR,KFSL,KFSR,NSH,Z,PTX,PTY) !string fragmentation
        IF(IALPA(99).LE.0)THEN
  400    EULER(1)=PARU(2)*RLU0(0)       !phi
         EULER(2)=THES(ISTR)            !theta
         EULER(3)=PARU(2)*RLU0(0)       !psi
         IF(ISTR.EQ.1)THEN              !total number of entries
          N=KAU(3)+NSH(1)
          NEXT(ISTR)=KAU(3)+1
         ELSEIF(ISTR.EQ.2)THEN
          N=KAU(3)+NSH(1)+NSH(2)
          NEXT(ISTR)=KAU(3)+NSH(1)+1
         ENDIF
         DO I=NEXT(ISTR),N
          J=I-NEXT(ISTR)+1
          MS(J)=ULMASS0(KA(J,2))
          DO L=1,5   
           K(I,L)=KA(J,L)
           V(I,L)=VA(J,L)
          ENDDO    
          PL(J)=(Z(J)*ECL-MT(J)*MT(J)/Z(J)/ECL)/2.0
          PIN(1)=PTX(J)  
          PIN(2)=PTY(J)  
          PIN(3)=PL(J)
          PINL=PIN(1)*PIN(1)+PIN(2)*PIN(2)+PIN(3)*PIN(3)
          CALL EULERROT(-1,EULER,PIN,POU)
          POUL=POU(1)*POU(1)+POU(2)*POU(2)+POU(3)*POU(3)
          DO M=1,5
           PS(M)=PSTR(ISTR,M)    !string 5-momentum
          ENDDO
          DO M=1,3
           PI(M)=POU(M)
          ENDDO
          PP=PI(1)*PI(1)+PI(2)*PI(2)+PI(3)*PI(3)
          PI(4)=SQRT(PP+MS(J)*MS(J))
          PI(5)=MS(J)
          K(I,3)=KAU(16+ISTR)
          CALL CMSTOLAB(PS,PI,PF)   !translate from remainder CMS to LABS
          DO M=1,5
           P(I,M)=PF(M)
          ENDDO
         ENDDO
        ELSE
         IRE=IRE+1
        ENDIF
       ELSE                         !string identifys to one particle
        ITRY=0
  500   IL=KFSL(ISTR)
        IR=KFSR(ISTR)
        CALL MESIDT(IL,IR,ID)
        IF(ID.EQ.0)THEN
         IRE=IRE+1
        ELSE
         NSH(ISTR)=1
         N=N+1
         K(N,1)=1
         K(N,2)=ID
         K(N,3)=KAU(16+ISTR)
         K(N,4)=0
         K(N,5)=0
         DO J=1,4
          P(N,J)=PSTR(ISTR,J)
          V(N,J)=V(KAU(2),J)
         ENDDO
         P(N,5)=ULMASS0(ID)
         IF(P(N,5).GE.PSTR(ISTR,5))ITRY=ITRY+1
         IF(1.LE.ITRY.AND.ITRY.LE.10)THEN
          IRE=IRE+1
          GOTO 500
         ELSEIF(ITRY.GT.10)THEN
          GOTO 100
         ENDIF
         V(N,5)=0
        ENDIF
       ENDIF  
      ENDDO 
      IF(IRE.GT.0)GOTO 100
      CALL LUEXEC0
      END 

*...J/psi -> gamma-gluon-gluon (momentum not conservation)
      !****************!
      SUBROUTINE LUJRGG1
      !****************!
      INTEGER KF(2),KFSL(3),KFSR(3),NSH(0:3),MESONFLV(-500:+500)
      INTEGER IOD(-1000:+1000),MESON(50,3,-6:+6,-6:+6),NEXT(3)
      REAL SINV(3),SMIN(3),PSTR(3,5),VSTR(3,3),THES(3),PHIS(3)
      REAL PS(5),PI(5),PF(5),PIN(3),POU(3),EULER(3),XMIN(3)
      REAL PTX(10),PTY(10),Z(10),MS(10),MT(10),SMT(10),PL(10)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/MT/MT,SMT
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/,/MT/

C...Printout. Check input parameters. 
      XAU(1)=RALFU(1)
      ECM=XAU(1)
      ECMC=XAU(2)
      PAK=XAU(3)
c      IF(MSTU(12).GE.1)CALL LULIST0(0)
      IF(KAU(1).EQ.1)ECMC=SQRT(ECM*(ECM-2.0*XAU(6)))
      IF(ECMC.LT.PARJ(127)+2.02*PARF(101)) THEN 
       CALL LUERRM0(16,'(LUJRGG:) called with too small CM energy') 
       IF(MSTU(21).GE.1) RETURN 
      ENDIF 

C...Choose x1 and x2 according to matrix element. 
      MK=0    
      ECMC=ECM
      NTRY=0 
      NSAV=N  
      NC=N
      NCSAV=NC
  100 N=NSAV  
      NC=NCSAV
      DO ISTR=0,3
       NSH(ISTR)=0
      ENDDO
      X1=RLU0(0) 
      X2=RLU0(0) 
      X3=2.-X1-X2 
      IF(X3.GE.1..OR.((1.-X1)/(X2*X3))**2+((1.-X2)/(X1*X3))**2+ 
     &((1.-X3)/(X1*X2))**2.LE.2.*RLU0(0)) GOTO 100 
      NJET=3 
      IF(MSTJ(101).LE.4) CALL LU3ENT0(NC+1,21,21,21,ECMC,X1,X3) 
      IF(MSTJ(101).GE.5) CALL LU3ENT0(-(NC+1),21,21,21,ECMC,X1,X3) 
 
C...Photon-gluon-gluon events. Small system modifications. Jet origin. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      QF=0. 
      IF(KFLC.NE.0) QF=KCHG(KFLC,1)/3. 
      RGAM=7.2*QF**2*PARU(101)/ULALPS0(ECMC**2) 
      MK=0 
      RGAM=1.0E+30  !huhm 
      IF(RLU0(0).GT.RGAM/(1.+RGAM)) THEN 
       IF(1.-MAX(X1,X2,X3).LE.MAX((PARJ(126)/ECM)**2,PARJ(125))) 
     & NJET=2 
       IF(NJET.EQ.2.AND.MSTJ(101).LE.4) CALL LU2ENT0(NC+1,21,21,ECMC) 
       IF(NJET.EQ.2.AND.MSTJ(101).GE.5) CALL LU2ENT0(-(NC+1),21,21,ECMC) 
      ELSE 
       MK=1 
       ECMJ=SQRT(1.-X1)*ECMC 
       IF(ECMJ.LT.2.*PARJ(127)) GOTO 100 
       K(NC+1,1)=1 
       K(NC+1,2)=22 
       K(NC+1,4)=0 
       K(NC+1,5)=0 
       IF(MSTJ(101).GE.5) K(NC+2,4)=MSTU(5)*(NC+3) 
       IF(MSTJ(101).GE.5) K(NC+2,5)=MSTU(5)*(NC+3) 
       IF(MSTJ(101).GE.5) K(NC+3,4)=MSTU(5)*(NC+2) 
       IF(MSTJ(101).GE.5) K(NC+3,5)=MSTU(5)*(NC+2) 
       NJET=2 
       IF(ECMJ.LT.4.*PARJ(127)) THEN 
        MSTU10=MSTU(10) 
        MSTU(10)=1 
        P(NC+2,5)=ECMJ 
        CALL LU1ENT0(NC+2,83,0.5*(X2+X3)*ECMC,PARU(1),0.) 
        MSTU(10)=MSTU10 
        NJET=0 
       ENDIF 
      ENDIF 
      DO 110 IP=NC+1,N                    !here 5(b)->4(4)  huhm
      K(IP,3)=K(IP,3)+(MSTJ(115)/2)+(KFLC/4)*(MSTJ(115)/3)*(NC-1) 
  110 CONTINUE 
 
C...Differential cross-sections. Upper limit for cross-section. 
      IF(MSTJ(106).EQ.1) THEN 
       SQ2=SQRT(2.) 
       HF1=1.-PARJ(131)*PARJ(132) 
       HF3=PARJ(133)**2 
       CT13=(X1*X3-2.*X1-2.*X3+2.)/(X1*X3) 
       ST13=SQRT(1.-CT13**2) 
       SIGL=0.5*X3**2*((1.-X2)**2+(1.-X3)**2)*ST13**2 
       SIGU=(X1*(1.-X1))**2+(X2*(1.-X2))**2+(X3*(1.-X3))**2-SIGL 
       SIGT=0.5*SIGL 
       SIGI=(SIGL*CT13/ST13+0.5*X1*X3*(1.-X2)**2*ST13)/SQ2 
       SIGMAX=(2.*HF1+HF3)*ABS(SIGU)+2.*(HF1+HF3)*ABS(SIGL)+2.*(HF1+ 
     & 2.*HF3)*ABS(SIGT)+2.*SQ2*(HF1+2.*HF3)*ABS(SIGI) 
 
C...Angular orientation of event. 
  120  CHI=PARU(2)*RLU0(0) 
       CTHE=2.*RLU0(0)-1. 
       PHI=PARU(2)*RLU0(0) 
       CCHI=COS(CHI) 
       SCHI=SIN(CHI) 
       C2CHI=COS(2.*CHI) 
       S2CHI=SIN(2.*CHI) 
       THE=ACOS(CTHE) 
       STHE=SIN(THE) 
       C2PHI=COS(2.*(PHI-PARJ(134))) 
       S2PHI=SIN(2.*(PHI-PARJ(134))) 
       SIG=((1.+CTHE**2)*HF1+STHE**2*C2PHI*HF3)*SIGU+2.*(STHE**2*HF1- 
     & STHE**2*C2PHI*HF3)*SIGL+2.*(STHE**2*C2CHI*HF1+((1.+CTHE**2)* 
     & C2CHI*C2PHI-2.*CTHE*S2CHI*S2PHI)*HF3)*SIGT-2.*SQ2*(2.*STHE*CTHE* 
     & CCHI*HF1-2.*STHE*(CTHE*CCHI*C2PHI-SCHI*S2PHI)*HF3)*SIGI 
       IF(SIG.LT.SIGMAX*RLU0(0)) GOTO 120 
       CALL LUDBRB0(NC+1,N,0.,CHI,0D0,0D0,0D0) 
       CALL LUDBRB0(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 
      K(N-1,1)=12
      K(N-0,1)=11

C...Directions of the two strings (unit vector)
      SUM1=0
      SUM2=0
      DO I=1,3
       VSTR(1,I)=(P(N-1,I)-P(N-0,I))/2.0
       VSTR(2,I)=(P(N-0,I)-P(N-1,I))/2.0
       SUM1=SUM1+VSTR(1,I)*VSTR(1,I)
       SUM2=SUM2+VSTR(2,I)*VSTR(2,I)
      ENDDO
      SUM1=SQRT(SUM1)
      SUM2=SQRT(SUM2)

C...Directions of the two strings (unit vector)
      DO I=1,3
       VSTR(1,I)=VSTR(1,I)/SUM1
       VSTR(2,I)=VSTR(2,I)/SUM2
      ENDDO

C...Cossin and polar angles of the strings
      COSTHE1=VSTR(1,3)
      COSTHE2=VSTR(2,3)
      THES(1)=ACOS(COSTHE1) 
      THES(2)=ACOS(COSTHE2) 

C...flavor for two gluons -> 2(q and qbar)
      A=1.0/3.0
      B=2.0/3.0
      DO I=1,2 
       R=RLU0(0)
       IF(R.LE.A)THEN
	KF(I)=1
       ELSEIF(A.LT.R.AND.R.LE.B)THEN
	KF(I)=2
       ELSE
        KF(I)=3
       ENDIF
      ENDDO

C...Quark-antiqurks flavours of the three strings
      KFSL(1)=+KF(1)
      KFSR(1)=-KF(2)
      KFSL(2)=+KF(2) 
      KFSR(2)=-KF(1)

C...Quark-antiqurk data from gluon 1
      N=N+1               !q1
      PQ=P(N-2,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,+KF(1),PQ,0.,0.)
      P(N,1)=P(N-2,1)/2.0
      P(N,2)=P(N-2,2)/2.0
      P(N,3)=P(N-2,3)/2.0
      P(N,4)=P(N-2,4)/2.0
      P(N,5)=ULMASS0(+KF(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=+KF(1)
      K(N,3)=N-2
      K(N,4)=0  
      K(N,5)=0  
      N=N+1               !q~1
      PQ=P(N-3,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,-KF(1),PQ,0.,0.)
      P(N,1)=P(N-3,1)/2.0
      P(N,2)=P(N-3,2)/2.0
      P(N,3)=P(N-3,3)/2.0
      P(N,4)=P(N-3,4)/2.0
      P(N,5)=ULMASS0(-KF(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=-KF(1)
      K(N,3)=N-3
      K(N,4)=0  
      K(N,5)=0  
      N=N+1               !q2
      PQ=P(N-3,4)/2.0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,+KF(2),PQ,0.,0.)
      P(N,1)=P(N-3,1)/2.0
      P(N,2)=P(N-3,2)/2.0
      P(N,3)=P(N-3,3)/2.0
      P(N,4)=P(N-3,4)/2.0
      P(N,5)=ULMASS0(+KF(2))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=+KF(2)
      K(N,3)=N-3
      K(N,4)=0  
      K(N,5)=0  
      N=N+1               !q~2
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      PQ=P(N-3,4)/2.0
      CALL LU1ENT0(N,-KF(2),PQ,0.,0.)
      P(N,1)=P(N-4,1)/2.0
      P(N,2)=P(N-4,2)/2.0
      P(N,3)=P(N-4,3)/2.0
      P(N,4)=P(N-4,4)/2.0
      P(N,5)=ULMASS0(-KF(2))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=-KF(2)
      K(N,3)=N-4
      K(N,4)=0  
      K(N,5)=0  

C...The minimum invariant masses for the each string
      IALPA(99)=0   
      MID=IABS(KF(1))+IABS(KF(2))
      DO I=1,2
       IF(MID.LE.4)THEN
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(91)                       !~one pion mass
        ELSE
         XMIN(I)=RALPA(91)+RALPA(91)		 !~two pion mass
        ENDIF
       ELSEIF(MID.EQ.5)THEN
        IF(IALFU(22).EQ.1)THEN !permit small string identify as meson
         XMIN(I)=RALPA(92)                       !~one K mass
        ELSE                   !small string >= 2,3...mesons
         XMIN(I)=RALPA(91)+RALPA(92)		 !~one pion and one K mass
        ENDIF
       ELSEIF(MID.EQ.6)THEN   
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(93)                       !~one eta mass
        ELSE
         XMIN(I)=RALPA(92)+RALPA(92)		 !~two K  mass
        ENDIF
       ENDIF 
       SMIN(I)=XMIN(I)*XMIN(I)
      ENDDO

C...Two strings 5-momentums & invariant masses
      DO I=1,4
       PSTR(1,I)=P(N-3,I)+P(N-0,I)
       PSTR(2,I)=P(N-1,I)+P(N-2,I)
      ENDDO
      IRE=0
      DO J=1,2
       EE=PSTR(J,4)*PSTR(J,4)
       PP=PSTR(J,1)*PSTR(J,1)+PSTR(J,2)*PSTR(J,2)+PSTR(J,3)*PSTR(J,3)
       PSTR(J,5)=SQRT(EE-PP)
       SINV(J)=EE-PP
       IF(SINV(J).LE.SMIN(J))IRE=IRE+1
      ENDDO
      IF(IRE.GT.0)GOTO 100
      WRITE(77,77)SQRT(SINV(1)),SQRT(SINV(2))
   77 FORMAT(F10.6,1X,F10.6)

C...Fill string 1
      N=N+1
      CALL LU1ENT0(N,92,PSTR(1,4),THES(1),PHIS(1))
      K(N,1)=11
      K(N,3)=N-4
      P(N,1)=PSTR(1,1)
      P(N,2)=PSTR(1,2)
      P(N,3)=PSTR(1,3)
      P(N,4)=PSTR(1,4)
      P(N,5)=PSTR(1,5)   
      KAU(17)=N

C...Fill string 2
      N=N+1
      CALL LU1ENT0(N,92,PSTR(2,4),THES(2),PHIS(2))
      K(N,1)=11
      K(N,3)=N-3
      P(N,1)=PSTR(2,1)
      P(N,2)=PSTR(2,2)
      P(N,3)=PSTR(2,3)
      P(N,4)=PSTR(2,4)
      P(N,5)=PSTR(2,5)
      KAU(3)=N  
      KAU(18)=N
      
C...Srtings fragment into hadrons or identify to hadron
  300 IRE=0
      DO ISTR=1,2          !loop for three strings fragment separately
       XAU(2)=SQRT(SINV(ISTR))
       ECL=XAU(2)
       KAU(10)=XAU(2)/XMIN(ISTR)
       KAU(10)=MIN(IALFU(20),KAU(10))  !maximum primary hadron number
       IF(KAU(10).GE.2)THEN            !string->2,3...particles
        CALL LUGLSF(ISTR,PSTR,KFSL,KFSR,NSH,Z,PTX,PTY) !string fragmentation
        IF(IALPA(99).LE.0)THEN
  400    EULER(1)=PARU(2)*RLU0(0)       !phi
         EULER(2)=THES(ISTR)            !theta
         EULER(3)=PARU(2)*RLU0(0)       !psi
         IF(ISTR.EQ.1)THEN              !total number of entries
          N=KAU(3)+NSH(1)
          NEXT(ISTR)=KAU(3)+1
         ELSEIF(ISTR.EQ.2)THEN
          N=KAU(3)+NSH(1)+NSH(2)
          NEXT(ISTR)=KAU(3)+NSH(1)+1
         ENDIF
         DO I=NEXT(ISTR),N
          J=I-NEXT(ISTR)+1
          MS(J)=ULMASS0(KA(J,2))
          DO L=1,5   
           K(I,L)=KA(J,L)
           V(I,L)=VA(J,L)
          ENDDO    
          PL(J)=(Z(J)*ECL-MT(J)*MT(J)/Z(J)/ECL)/2.0
          PIN(1)=PTX(J)  
          PIN(2)=PTY(J)  
          PIN(3)=PL(J)
          PINL=PIN(1)*PIN(1)+PIN(2)*PIN(2)+PIN(3)*PIN(3)
          CALL EULERROT(-1,EULER,PIN,POU)
          POUL=POU(1)*POU(1)+POU(2)*POU(2)+POU(3)*POU(3)
          DO M=1,5
           PS(M)=PSTR(ISTR,M)    !string 5-momentum
          ENDDO
          DO M=1,3
           PI(M)=POU(M)
          ENDDO
          PP=PI(1)*PI(1)+PI(2)*PI(2)+PI(3)*PI(3)
          PI(4)=SQRT(PP+MS(J)*MS(J))
          PI(5)=MS(J)
          K(I,3)=KAU(16+ISTR)
          CALL CMSTOLAB(PS,PI,PF)   !translate from remainder CMS to LABS
          DO M=1,5
           P(I,M)=PF(M)
          ENDDO
         ENDDO
        ELSE
         IRE=IRE+1
        ENDIF
       ELSE                         !string identifys to one particle
        IL=KFSL(ISTR)
        IR=KFSR(ISTR)
        CALL MESIDT(IL,IR,ID)
        IF(ID.EQ.0)THEN
         IRE=IRE+1
        ELSE
         NSH(ISTR)=1
         N=N+1
         K(N,1)=1
         K(N,2)=ID
         K(N,3)=KAU(16+ISTR)
         K(N,4)=0
         K(N,5)=0
         DO J=1,4
          P(N,J)=PSTR(ISTR,J)
          V(N,J)=V(KAU(2),J)
         ENDDO
         P(N,5)=ULMASS0(ID)
         V(N,5)=0
        ENDIF
       ENDIF  
      ENDDO 
      IF(IRE.GT.0)GOTO 100
      CALL LUEXEC0
      END 

C...J/psi => gamma eta_c => ...
      !***************!
      SUBROUTINE LUJEGG
      !***************!
      INTEGER KF(2),KFSL(3),KFSR(3),NSH(0:3),MESONFLV(-500:+500)
      INTEGER IOD(-1000:+1000),MESON(50,3,-6:+6,-6:+6),NEXT(3)
      REAL SINV(3),SMIN(3),PSTR(3,5),VSTR(3,3),THES(3),PHIS(3)
      REAL PS(5),PI(5),PF(5),PIN(3),POU(3),EULER(3),XMIN(3)
      REAL PTX(10),PTY(10),Z(10),MS(10),MT(10),SMT(10),PL(10)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/MT/MT,SMT
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/,/MT/

C...Printout. Check input parameters. 
      XAU(1)=RALFU(1)
      ECM=XAU(1)
      ECMC=XAU(2)
      PAK=XAU(3)
c      IF(MSTU(12).GE.1)CALL LULIST0(0)
      IF(KAU(1).EQ.1)ECMC=SQRT(ECM*(ECM-2.0*XAU(6)))
      IF(ECMC.LT.PARJ(127)+2.02*PARF(101)) THEN 
       CALL LUERRM0(16,'(LUJRGG:) called with too small CM energy') 
       IF(MSTU(21).GE.1) RETURN 
      ENDIF 

C...Radiative photon from J/psi
      N=N+1
      THE=PARU(1)*RLU0(0)
      PHI=PARU(2)*RLU0(0)
      XPSI=ULMASS0(443)
      XETA=ULMASS0(441)
      E=(XPSI+XETA)*(XPSI-XETA)/XPSI/2.0
      CALL LU1ENT0(N,22,E,THE,PHI)
      K(N,3)=N-1

C...Fill eta_c 
      N=N+1
      E=P(N-2,4)-P(N-1,4)
      CALL LU1ENT0(N,441,E,THE+PARU(1),PHI)
      K(N,1)=21
      K(N,3)=N-2
      PS(1)=P(N,1)
      PS(2)=P(N,2)
      PS(3)=P(N,3)
      PS(4)=P(N,4)
      PS(5)=P(N,5)

C...eta => gamma gamma or gluon gluon
      E=PS(5)/2.0
      THE=PARU(1)*RLU0(0) 
      PHI=PARU(2)*RLU0(0)
      RGG=0.01
      IF(RLU0(0).LE.RGG)THEN
       KFIL=22
      ELSE
       KFIL=21
      ENDIF
      N=N+1
      CALL LU1ENT0(N,KFIL,E,THE,PHI)
      DO I=1,5
       PI(I)=P(N,I)
      ENDDO
      CALL CMSTOLAB(PS,PI,PF)    !translate from remainder CMS to LABS
      DO I=1,5
       P(N,I)=PF(I)
      ENDDO
      K(N,1)=12
      K(N,3)=N-1
      N=N+1
      CALL LU1ENT0(N,KFIL,E,THE+PARU(1),PHI)
      DO I=1,5
       PI(I)=P(N,I)
      ENDDO
      CALL CMSTOLAB(PS,PI,PF)    !translate from remainder CMS to LABS
      DO I=1,5
       P(N,I)=PF(I)
      ENDDO
      K(N,1)=11
      K(N,3)=N-2
      IF(KFIL.EQ.22)RETURN

C...The flavour of gluon splitting into quark-antiquark
      A=1.0/3.0
      B=2.0/3.0
      DO I=1,2 
       R=RLU0(0)
       IF(R.LE.A)THEN
	KF(I)=1
       ELSEIF(A.LT.R.AND.R.LE.B)THEN
	KF(I)=2
       ELSE
	KF(I)=3
       ENDIF
      ENDDO

C...Quark-antiqurks flavours of the two strings
      KFSL(1)=+KF(1)
      KFSR(1)=-KF(2)
      KFSL(2)=+KF(2)
      KFSR(2)=-KF(1)
      KFSL(3)=0
      KFSR(3)=0

C...Quark-antiqurk data from gluon 1 
      N=N+1               !q1
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      PQ=P(N-2,4)/2.0
      CALL LU1ENT0(N,+KF(1),PQ,0.,0.)
      P(N,1)=P(N-2,1)/2.0
      P(N,2)=P(N-2,2)/2.0
      P(N,3)=P(N-2,3)/2.0
      P(N,4)=P(N-2,4)/2.0
      P(N,5)=ULMASS0(+KF(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=+KF(1)  
      K(N,3)=N-2
      K(N,4)=0  
      K(N,5)=0
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      N=N+1               !q~1
      CALL LU1ENT0(N,-KF(1),PQ,0.,0.)
      P(N,1)=P(N-3,1)/2.0
      P(N,2)=P(N-3,2)/2.0
      P(N,3)=P(N-3,3)/2.0
      P(N,4)=P(N-3,4)/2.0
      P(N,5)=ULMASS0(-KF(1))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=-KF(1)
      K(N,3)=N-3   
      K(N,4)=0     
      K(N,5)=0

C...Quark-antiqurk data from gluon 2 
      N=N+1               !q2
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      PQ=P(N-3,4)/2.0
      CALL LU1ENT0(N,+KF(2),PQ,0.,0.)
      P(N,1)=P(N-3,1)/2.0
      P(N,2)=P(N-3,2)/2.0
      P(N,3)=P(N-3,3)/2.0
      P(N,4)=P(N-3,4)/2.0
      P(N,5)=ULMASS0(+KF(2))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=12
      K(N,2)=+KF(2)
      K(N,3)=N-3   
      K(N,4)=0     
      K(N,5)=0     
      N=N+1               !q~2
      MSTJ24=MSTJ(24)
      MSTJ(24)=0
      CALL LU1ENT0(N,-KF(2),PQ,0.,0.)
      P(N,1)=P(N-4,1)/2.0
      P(N,2)=P(N-4,2)/2.0
      P(N,3)=P(N-4,3)/2.0
      P(N,4)=P(N-4,4)/2.0
      P(N,5)=ULMASS0(-KF(2))
      MSTJ(24)=MSTJ24
      DO J=1,5
       V(N,J)=0
      ENDDO
      K(N,1)=11
      K(N,2)=-KF(2)
      K(N,3)=N-4   
      K(N,4)=0     
      K(N,5)=0

C...store some data 
      MK=0    
      ECMC=ECM
      NTRY=0 
      NSAV=N  
      NC=N
      NCSAV=NC
  100 N=NSAV  
      NC=NCSAV
      DO ISTR=0,3
       NSH(ISTR)=0
      ENDDO
 
C...Directions of the two strings (unit vector)
      SUM1=0
      SUM2=0
      DO I=1,3
       VSTR(1,I)=P(N-3,I)-P(N-0,I)
       VSTR(2,I)=P(N-2,I)-P(N-1,I)
       SUM1=SUM1+VSTR(1,I)*VSTR(1,I)
       SUM2=SUM2+VSTR(2,I)*VSTR(2,I)
      ENDDO
      SUM1=SQRT(SUM1)
      SUM2=SQRT(SUM2)
      DO I=1,3
       VSTR(1,I)=VSTR(1,I)/SUM1
       VSTR(2,I)=VSTR(2,I)/SUM2
      ENDDO

C...Cossin and polar angles of the strings
      COSTHE1=VSTR(1,3)
      COSTHE2=VSTR(2,3)
      THES(1)=ACOS(COSTHE1) 
      THES(2)=ACOS(COSTHE2) 
      PHIS(1)=PARU(2)*RLU0(0)
      PHIS(2)=PARU(2)*RLU0(0)

C...The minimum invariant masses for the each string
      IALPA(99)=0   
      MID=IABS(KF(1))+IABS(KF(2))
      DO I=1,2
       IF(MID.LE.4)THEN
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(91)                       !~one pion mass
        ELSE
         XMIN(I)=RALPA(91)+RALPA(91)		 !~two pion mass
        ENDIF
       ELSEIF(MID.EQ.5)THEN
        IF(IALFU(22).EQ.1)THEN !permit small string identify as meson
         XMIN(I)=RALPA(92)                       !~one K mass
        ELSE                   !small string >= 2,3...mesons
         XMIN(I)=RALPA(91)+RALPA(92)		 !~one pion and one K mass
        ENDIF
       ELSEIF(MID.EQ.6)THEN   
        IF(IALFU(22).EQ.1)THEN
         XMIN(I)=RALPA(93)                       !~one eta mass
        ELSE
         XMIN(I)=RALPA(92)+RALPA(92)		 !~two K  mass
        ENDIF
       ENDIF 
       SMIN(I)=XMIN(I)*XMIN(I)
      ENDDO

C...Two strings 5-momentums & invariant masses
      DO I=1,4
       PSTR(1,I)=P(N-3,I)+P(N-0,I)
       PSTR(2,I)=P(N-1,I)+P(N-2,I)
      ENDDO
      IRE=0
      DO I=1,2
       EE=PSTR(I,4)*PSTR(I,4)
       PP=PSTR(I,1)*PSTR(I,1)+PSTR(I,2)*PSTR(I,2)+PSTR(I,3)*PSTR(I,3)
       SINV(I)=EE-PP
       PSTR(I,5)=SQRT(SINV(I))
       IF(SINV(I).LE.SMIN(I))IRE=IRE+1
      ENDDO
      IF(IRE.GT.0)GOTO 100
      WRITE(77,77)SQRT(SINV(1)),SQRT(SINV(2))
   77 FORMAT(F10.6,1X,F10.6)

C...Fill string 1
      N=N+1
      E=PSTR(1,4)
      THE=THES(1)
      PHI=PHIS(1)
      CALL LU1ENT0(N,92,E,THE,PHI)
      K(N,1)=12
      K(N,3)=N-4
      P(N,1)=PSTR(1,1)
      P(N,2)=PSTR(1,2)
      P(N,3)=PSTR(1,3)
      P(N,4)=PSTR(1,4)
      P(N,5)=PSTR(1,5)   
      KAU(17)=N

C...Fill string 2
      N=N+1
      E=PSTR(2,4)
      THE=THES(2)
      PHI=PHIS(2)
      CALL LU1ENT0(N,92,E,THE,PHI)
      K(N,1)=11
      K(N,3)=N-3
      P(N,1)=PSTR(2,1)
      P(N,2)=PSTR(2,2)
      P(N,3)=PSTR(2,3)
      P(N,4)=PSTR(2,4)
      P(N,5)=PSTR(2,5)
      KAU(3)=N  
      KAU(18)=N
     
C...Srtings fragment into hadrons or identify to hadron
  300 IRE=0
      DO ISTR=1,2          !loop for three strings fragment separately
       DO I=1,5
	PS(I)=PSTR(ISTR,I)	       !string 5-momentum
       ENDDO
       XAU(2)=SQRT(SINV(ISTR))
       ECL=XAU(2)
       KAU(10)=XAU(2)/XMIN(ISTR)
       KAU(10)=MIN(IALFU(20),KAU(10))  !maximum primary hadron number
       IF(KAU(10).GE.2)THEN            !string->2,3...particles
        CALL LUGLSF(ISTR,PSTR,KFSL,KFSR,NSH,Z,PTX,PTY) !string fragmentation
        IF(IALPA(99).LE.0)THEN
  400    EULER(1)=PARU(2)*RLU0(0)       !phi
         EULER(2)=THES(ISTR)            !theta
         EULER(3)=PARU(2)*RLU0(0)       !psi
         IF(ISTR.EQ.1)THEN              !total number of entries
          N=KAU(3)+NSH(1)
          NEXT(ISTR)=KAU(3)+1
         ELSEIF(ISTR.EQ.2)THEN
          N=KAU(3)+NSH(1)+NSH(2)
          NEXT(ISTR)=KAU(3)+NSH(1)+1
         ENDIF
         DO I=NEXT(ISTR),N
          J=I-NEXT(ISTR)+1
          MS(J)=PA(J,5)
          DO L=1,5   
           K(I,L)=KA(J,L)
           V(I,L)=VA(J,L)
          ENDDO    
          PL(J)=(Z(J)*ECL-MT(J)*MT(J)/Z(J)/ECL)/2.0
          PIN(1)=PTX(J)  
          PIN(2)=PTY(J)  
          PIN(3)=PL(J)
          CALL EULERROT(-1,EULER,PIN,POU)
          DO M=1,3
           PI(M)=POU(M)
          ENDDO
          PP=PI(1)*PI(1)+PI(2)*PI(2)+PI(3)*PI(3)
          PI(4)=SQRT(PP+MS(J)*MS(J))
          PI(5)=MS(J)
          K(I,3)=KAU(16+ISTR)
          CALL CMSTOLAB(PS,PI,PF)   !translate from remainder CMS to LABS
          DO M=1,5
           P(I,M)=PF(M)
          ENDDO
         ENDDO
        ELSE
         IRE=IRE+1
        ENDIF
       ELSE                         !string identifys to one particle
        IL=KFSL(ISTR)
        IR=KFSR(ISTR)
        CALL MESIDT(IL,IR,ID)
        IF(ID.EQ.0)THEN
         IRE=IRE+1
        ELSE
         NSH(ISTR)=1
         N=N+1
         K(N,1)=1
         K(N,2)=ID
         K(N,3)=KAU(16+ISTR)
         K(N,4)=0
         K(N,5)=0
         DO J=1,4
          P(N,J)=PSTR(ISTR,J)
          V(N,J)=V(KAU(2),J)
         ENDDO
         P(N,5)=ULMASS0(ID)
         V(N,5)=0
        ENDIF
       ENDIF  
      ENDDO
      IF(IRE.GT.0)GOTO 100
      CALL LUEXEC0
      END 

C...Determine primary hadron multiplicity
      !****************************!
      SUBROUTINE LUALPN(KFLCL,KFLCR)
      !****************************!
      REAL PN(10),FN(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/
      IALPA(99)=0
      ECM=XAU(2)
      NA=0

*...set primary hadron multiplicity by hand
      IF(IALFU(1).EQ.0)THEN
       PN(1)=0.0
       DO J=2,6
        PN(J)=RALFU(20+J)
       ENDDO

C...Poisson distribution 
      ELSEIF(IALFU(1).EQ.1)THEN
       CALL POISSONDIS(ECM,PN)
      
C...Biominal
      ELSEIF(IALFU(1).EQ.2)THEN
       WRITE(*,*)'Binomial distribution NOT READY YET. STOP'
       STOP 

C...Area law
      ELSEIF(IALFU(1).EQ.3)THEN
       WRITE(*,*)'Area Law calculation NOT READY YET. STOP'
       STOP 
      ENDIF
      IF(IALPA(99).GE.1)RETURN

C...Distribution used
      PNSUM=0
      DO I=2,KAU(10)
       PNSUM=PNSUM+PN(I)
      ENDDO
      FN(1)=0
      DO I=2,KAU(10)
       PN(I)=PN(I)/PNSUM
       FN(I)=FN(I-1)+PN(I)
      ENDDO

C...Determine NA to be produced
   10 R=RLU0(0)
      DO I=2,KAU(10)
       IF(FN(I-1).LT.R.AND.R.LE.FN(I))NA=I
      ENDDO
      IF(NA.LT.2)GOTO 10
      KAU(5)=NA
      RETURN  
      END

C...String fragmentation by area law
      !****************************!
      SUBROUTINE LUALSF(KFLCL,KFLCR)
      !****************************!
      REAL MS(10),MT(10),SMT(10),PTX(10),PTY(10),PIN(3),POU(3)
      REAL Z(10),PL(10),EULER(3),PS(5),PI(5),PF(5)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/MT/MT,SMT
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/MT/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/
      EXTERNAL PLU0,RLU0,ULANGL0
      IALPA(99)=0
      
C...Effective c.m energy of system after ISR etc
      ECM=XAU(1) !initial c.m. energy
      ECL=XAU(2) !effective c.m energy after ISR
      SCM=ECM*ECM
      SCL=ECL*ECL
      ISRPHTON=KAU(2)
      ISTRING=KAU(3) 
    
C...initial quark-antiquark data
      JQ=ISTRING-2
      IF(KAU(1).LE.0)THEN         !without ISR
       PQX=P(JQ,1)                !Px of initial q0 in remainder system 
       PQY=P(JQ,2)                !Py of initial q0 in remainder system
       PQZ=P(JQ,3)                !Pz of initial q0 in remainder system
       THE=PLU0(JQ,13)            !polar angle theta
       PHI=PLU0(JQ,15)            !azimuthal angle phi 
      ELSEIF(KAU(1).GE.1)THEN     !with ISR
       PS(1)=P(ISTRING,1)         !Px of string (remainder system after ISR)
       PS(2)=P(ISTRING,2)         !Py
       PS(3)=P(ISTRING,3)         !Pz
       PS(4)=P(ISTRING,4)         !E
       PS(5)=P(ISTRING,5)         !M
       PI(1)=P(JQ,1)              !the momentum of initial q0
       PI(2)=P(JQ,2)              !''
       PI(3)=P(JQ,3)              !''
       PI(4)=P(JQ,4)              !''
       PI(5)=P(JQ,5)              !''
       CALL LABTOCMS(PS,PI,PF)    !transfer (PI) of q0 from LBS to CMS (PF)
       PQX=PF(1)                  !Px of initial q0 in remainder system  
       PQY=PF(2)                  !Py of initial q0 in remainder system
       PQZ=PF(3)                  !Pz of initial q0 in remainder system
       PQT=SQRT(PQX*PQX+PQY*PQY)  !transerse momentum of q0 in remainder CMS
       THE=ULANGL0(PQZ,PQT)      !polar angle
       PHI=ULANGL0(PQX,PQY)      !azimuthal angle
      ENDIF 
      COSTHE=COS(THE)         
      SINTHE=SIN(THE)         
      COSPHI=COS(PHI)         
      SINPHI=SIN(PHI)       
   
C...Determine primary hadron multiplicity
      IF(IALFU(8).EQ.0)THEN
       CALL LUALPN(KFLCL,KFLCR)
      ELSE
       NA=IALFU(8)
       KAU(5)=NA
      ENDIF
      IF(IALPA(99).GE.1)RETURN
     
C...Determine primary hadrons
      IF(IALFU(8).GE.1.OR.IALFU(10).EQ.1.OR.KAU(5).EQ.6)THEN
   10  IALPA(99)=0
       CALL ALBODY(KFLCL,KFLCR)
       IF(IALPA(99).GE.1)RETURN   !redo for some errors
      ELSE !read exclusive probabilities given by area law
       KAU(11)=KFLCL
       CALL ALPROB(KAU(91))
      ENDIF
      
C...Sampling for Pt
      ITRY=0
   11 IALPA(99)=0
      ITRY=ITRY+1
      IF(ITRY.GE.1000)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      IF(IALPA(71).EQ.1)THEN
       CALL ALNCPT(PTX,PTY)      !uncorelated Pt sampling
      ELSEIF(IALPA(71).EQ.2)THEN
       CALL ALCOPT(PTX,PTY)	 !corelated Pt sampling
      ENDIF
      IF(IALPA(99).GE.1)GOTO 11
      SPTX=0.0 
      SPTY=0.0 
      DO J=1,NA
       SPTX=SPTX+PTX(J)
       SPTY=SPTY+PTY(J)
       MS(J)=PA(J,5)      !primary hadron mass
       SMT(J)=MS(J)*MS(J)+PTX(J)*PTX(J)+PTY(J)*PTY(J)
       MT(J)=SQRT(SMT(J)) !transverse mass
      ENDDO
      IF(ABS(SPTX).GT.RALFU(91).OR.SPTY.GT.RALFU(91))THEN
       WRITE(*,*)'in LUALLT: Pt non-conservative'
      ENDIF
      
C...Goto corresponding processes
   15 CONTINUE 
      IF(NA.EQ.2)GOTO 20
      IF(NA.EQ.3)GOTO 30
      IF(NA.EQ.4)GOTO 40
      IF(NA.EQ.5)GOTO 50
      IF(NA.EQ.6)GOTO 60

C...determine Z(1) and Z(2) of two body process
   20 CALL TWOBDYZ(2,SCL,SMT,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...determine Z(1) and Z(2) of three body process
   30 CALL THRBDYZ(3,SCL,SMT,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 4 body processes
   40 CONTINUE
      CALL FOUBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11 

C...determine the {zi} values of 4 primary hadrons
      CALL FOUBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 5 body processes
   50 CONTINUE
      CALL FIVBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11

C...determine the {zi} values of 5 primary hadrons
      CALL FIVBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 6 body processes
   60 CALL SIXBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11

C...determine the {zi} values of 6 primary hadrons
      CALL SIXBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...calculate 4-momentum P(I,1-4) of primary by Z(I) of AREALAW
  100 N=KAU(3)+NA               !total number of entries
      EULER(1)=PARU(2)*RLU0(0)  !phi
      EULER(2)=THE              !theta
      EULER(3)=PARU(2)*RLU0(0)  !psi  
      DO I=KAU(3)+1,N
       J=I-KAU(3)
       DO L=1,5   
        K(I,L)=KA(J,L)
        V(I,L)=VA(J,L)
       ENDDO    
       PL(J)=(Z(J)*ECL-MT(J)*MT(J)/Z(J)/ECL)/2.0
       SPL=SPL+PL(J)  
       PIN(1)=PTX(J)  
       PIN(2)=PTY(J)  
       PIN(3)=PL(J)   
       PINL=PIN(1)*PIN(1)+PIN(2)*PIN(2)+PIN(3)*PIN(3)
       CALL EULERROT(-1,EULER,PIN,POU)
       POUL=POU(1)*POU(1)+POU(2)*POU(2)+POU(3)*POU(3)
       IF(ABS(PINL-POUL).GT.RALFU(91))THEN
        WRITE(*,*)'LUALLT: error in rotation'
        IALPA(99)=1
        RETURN
       ENDIF  
       P(I,1)=POU(1)
       P(I,2)=POU(2)
       P(I,3)=POU(3)
       PPP=P(I,1)*P(I,1)+P(I,2)*P(I,2)+P(I,3)*P(I,3)
       P(I,4)=SQRT(PPP+MS(J)*MS(J))
       P(I,5)=MS(J)
       IF(KAU(1).GE.1)THEN	 !if ISR
        PI(1)=P(I,1)
        PI(2)=P(I,2)
        PI(3)=P(I,3)
        PI(4)=P(I,4)
        PI(5)=P(I,5)
        CALL CMSTOLAB(PS,PI,PF)  !translate from remainder CMS to LABS
        P(I,1)=PF(1)             !Px of primary hadron in LABS
        P(I,2)=PF(2)             !Py of primary hadron in LABS
        P(I,3)=PF(3)             !Pz of primary hadron in LABS
        P(I,4)=PF(4)             !E  of primary hadron in LABS
        P(I,5)=PF(5)             !M  of primary hadron in LABS
       ENDIF
      ENDDO 
      IALPA(99)=0
      !CALL ALCHEK    !huhm  ? 
      IF(IALPA(99).GE.1)RETURN
      K(KAU(2)-2,1)=12
      K(KAU(2)-1,1)=11
      RETURN
      END   

C...Quark-string fragmentation in J/psi decay by area law
      !****************************!
      SUBROUTINE LUQQSF(KFLCL,KFLCR)
      !****************************!
      REAL MS(10),MT(10),SMT(10),PTX(10),PTY(10),PIN(3),POU(3)
      REAL Z(10),PL(10),EULER(3),PS(5),PI(5),PF(5)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/MT/MT,SMT
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/MT/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/
      EXTERNAL PLU0,RLU0,ULANGL0

C...Effective c.m energy of system after ISR etc
      ECM=XAU(1)      !initial c.m. energy
      ECL=XAU(2)      !effective c.m energy after ISR
      SCM=ECM*ECM
      SCL=ECL*ECL
      ISRGAMA=KAU(2)
      ISTRING=KAU(3)
   
C...initial quark-antiquark data
      JQ=ISTRING-2
      IF(KAU(1).LE.0)THEN         !without ISR
       PQX=P(JQ,1)                !Px of initial q0 in remainder system
       PQY=P(JQ,2)                !Py of initial q0 in remainder system
       PQZ=P(JQ,3)                !Pz of initial q0 in remainder system
       THE=PLU0(JQ,13)             !polar angle theta
       PHI=PLU0(JQ,15)         	  !azimuthal angle phi 
      ELSEIF(KAU(1).GE.1)THEN     !with ISR
       PS(1)=P(ISTRING,1)         !Px of string (remainder system after ISR)
       PS(2)=P(ISTRING,2)         !Py
       PS(3)=P(ISTRING,3)         !Pz
       PS(4)=P(ISTRING,4)         !E 
       PS(5)=P(ISTRING,5)         !M 
       PI(1)=P(JQ,1)              !the momentum of initial q0
       PI(2)=P(JQ,2)              !''
       PI(3)=P(JQ,3)              !''
       PI(4)=P(JQ,4)              !''
       PI(5)=P(JQ,5)          	  !''
       CALL LABTOCMS(PS,PI,PF)    !transfer (PI) of q0 from LBS to CMS (PF)
       PQX=PF(1)                  !Px of initial q0 in remainder system    
       PQY=PF(2)                  !Py of initial q0 in remainder system    
       PQZ=PF(3)                  !Pz of initial q0 in remainder system    
       PQT=SQRT(PQX*PQX+PQY*PQY)  !transerse momentum of q0 in remainder CMS
       THE=ULANGL0(PQZ,PQT)      !polar angle
       PHI=ULANGL0(PQX,PQY)      !azimuthal angle
      ENDIF 
      COSTHE=COS(THE)
      SINTHE=SIN(THE)
      COSPHI=COS(PHI)
      SINPHI=SIN(PHI)
     
C...Determine primary hadron multiplicity
      CALL LUALPN(KFLCL,KFLCR)
       
C...Determine primary hadrons
      IF(IALFU(10).EQ.1)THEN !chose exclusive probabilities by quark model
   10  IALPA(99)=0
       CALL ALBODY(KFLCL,KFLCR)
       IF(IALPA(99).GE.1)RETURN   !redo for some errors
      ELSE                   !read exclusive probabilities given by area law
       KAU(11)=KFLCL
       KAU(12)=KFLCR
       CALL ALPROB(KAU(91))
      ENDIF
      
C...Sampling for Pt
      ITRY=0
   11 IALPA(99)=0
      ITRY=ITRY+1
      IF(ITRY.GE.1000)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      IF(IALPA(71).EQ.1)THEN
       CALL ALNCPT(PTX,PTY)      !uncorelated Pt sampling
      ELSEIF(IALPA(71).EQ.2)THEN
       CALL ALCOPT(PTX,PTY)	 !corelated Pt sampling
      ELSE
       WRITE(*,*)'No this scheme. Stop!'
      ENDIF
      IF(IALPA(99).GE.1)GOTO 11
      SPTX=0.0 
      SPTY=0.0 
      DO J=1,NA
       SPTX=SPTX+PTX(J)
       SPTY=SPTY+PTY(J)
       MS(J)=PA(J,5)      !primary hadron mass
       SMT(J)=MS(J)*MS(J)+PTX(J)*PTX(J)+PTY(J)*PTY(J)
       MT(J)=SQRT(SMT(J)) !transverse mass
      ENDDO
      IF(ABS(SPTX).GT.RALFU(91).OR.SPTY.GT.RALFU(91))THEN
       WRITE(*,*)'in LUALLT: Pt non-conservative'
      ENDIF
   
C...Goto corresponding processes
   15 CONTINUE 
      IF(NA.EQ.2)GOTO 20
      IF(NA.EQ.3)GOTO 30
      IF(NA.EQ.4)GOTO 40
      IF(NA.EQ.5)GOTO 50
      IF(NA.EQ.6)GOTO 60

C...determine Z(1) and Z(2) of two body process
   20 CALL TWOBDYZ(2,SCL,SMT,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...determine Z(1) and Z(2) of three body process
   30 CALL THRBDYZ(3,SCL,SMT,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 4 body processes
   40 CONTINUE
      CALL FOUBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11 

C...determine the {zi} values of 4 primary hadrons
      CALL FOUBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 5 body processes
   50 CONTINUE
      CALL FIVBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11

C...determine the {zi} values of 5 primary hadrons
      CALL FIVBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 6 body processes
   60 CALL SIXBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11

C...determine the {zi} values of 6 primary hadrons
      CALL SIXBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...calculate 4-momentum P(I,1-4) of primary by Z(I) of AREALAW
  100 N=KAU(3)+NA               !total number of entries
      EULER(1)=PARU(2)*RLU0(0)  !phi
      EULER(2)=THE              !theta
      EULER(3)=PARU(2)*RLU0(0)  !psi  
      DO I=KAU(3)+1,N
       J=I-KAU(3)
       DO L=1,5   
        K(I,L)=KA(J,L)
        V(I,L)=VA(J,L)
       ENDDO    
       PL(J)=(Z(J)*ECL-MT(J)*MT(J)/Z(J)/ECL)/2.0
       SPL=SPL+PL(J)  
       PIN(1)=PTX(J)  
       PIN(2)=PTY(J)  
       PIN(3)=PL(J)   
       PINL=PIN(1)*PIN(1)+PIN(2)*PIN(2)+PIN(3)*PIN(3)
       CALL EULERROT(-1,EULER,PIN,POU)
       POUL=POU(1)*POU(1)+POU(2)*POU(2)+POU(3)*POU(3)
       IF(ABS(PINL-POUL).GT.RALFU(91))THEN
        WRITE(*,*)'LUALLT: error in rotation'
        IALPA(99)=1
        RETURN
       ENDIF  
       P(I,1)=POU(1)
       P(I,2)=POU(2)
       P(I,3)=POU(3)
       PPP=P(I,1)*P(I,1)+P(I,2)*P(I,2)+P(I,3)*P(I,3)
       P(I,4)=SQRT(PPP+MS(J)*MS(J))
       P(I,5)=MS(J)
       IF(KAU(1).GE.1)THEN	 !if ISR
        PI(1)=P(I,1)
        PI(2)=P(I,2)
        PI(3)=P(I,3)
        PI(4)=P(I,4)
        PI(5)=P(I,5)
        CALL CMSTOLAB(PS,PI,PF)  !translate from remainder CMS to LABS
        P(I,1)=PF(1)             !Px of primary hadron in LABS
        P(I,2)=PF(2)             !Py of primary hadron in LABS
        P(I,3)=PF(3)             !Pz of primary hadron in LABS
        P(I,4)=PF(4)             !E  of primary hadron in LABS
        P(I,5)=PF(5)             !M  of primary hadron in LABS
       ENDIF
      ENDDO 
      RETURN
      END   

C...Glunic-string fragmentation in J/psi decay by area law
      !****************************************************!
      SUBROUTINE LUGLSF(ISTR,PSTR,KFLSL,KFLSR,NSH,Z,PTX,PTY)
      !****************************************************!
      INTEGER KFLSL(3),KFLSR(3),NSH(0:3)
      REAL MS(10),MT(10),SMT(10),PTX(10),PTY(10),Z(10),PSTR(3,5)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/MT/MT,SMT
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/MT/
      SAVE /ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/
      EXTERNAL PLU0,RLU0,ULANGL0

C...Effective c.m. energy of the sub-string ISTR
      IALPA(99)=0
      ECM=XAU(1)      !initial c.m. energy
      SCM=ECM*ECM
      SCL=PSTR(ISTR,4)*PSTR(ISTR,4)
      DO J=1,3
       SCL=SCL-PSTR(ISTR,J)*PSTR(ISTR,J)
      ENDDO
      ECL=SQRT(SCL)   
      XAU(2)=ECL

C...Determine primary hadron multiplicity
      KFL=KFLSL(ISTR)
      KFR=KFLSR(ISTR)
      CALL LUALPN(KFL,KFR)
      NSH(ISTR)=NA
   
C...Determine primary hadrons
   10 IF(KAU(5).GT.6)THEN
       WRITE(*,*)'CHECK LUALPN. STOP'
       STOP
      ELSEIF(IALFU(10).EQ.1.OR.KAU(5).LE.6)THEN
       CALL ALBODY(KFL,KFR)
       IF(IALPA(99).GE.1)RETURN   !redo for some errors
      ELSE !read exclusive probabilities given by area law
       KAU(10+ISTR)=KFLSL(ISTR)
       KAU(11+ISTR)=KFLSR(ISTR)
       CALL ALPROB(KAU(91))
      ENDIF

C...Sampling for Pt
      ITRY=0
   11 IALPA(99)=0
      ITRY=ITRY+1
      IF(ITRY.GE.1000)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      IF(IALPA(71).EQ.1)THEN
       CALL ALNCPT(PTX,PTY)      !uncorelated Pt sampling
      ELSEIF(IALPA(71).EQ.2)THEN
       CALL ALCOPT(PTX,PTY)	 !corelated Pt sampling
      ELSE
       WRITE(*,*)'No this scheme. Stop!'
      ENDIF
      IF(IALPA(99).GE.1)GOTO 11
      SPTX=0.0 
      SPTY=0.0 
      DO J=1,NA
       SPTX=SPTX+PTX(J)
       SPTY=SPTY+PTY(J)
       MS(J)=PA(J,5)      !primary hadron mass
       SMT(J)=MS(J)*MS(J)+PTX(J)*PTX(J)+PTY(J)*PTY(J)
       MT(J)=SQRT(SMT(J)) !transverse mass
      ENDDO
      IF(ABS(SPTX).GT.RALFU(91).OR.SPTY.GT.RALFU(91))THEN
       WRITE(*,*)'in LUALLT: Pt non-conservative'
      ENDIF

C...Goto corresponding processes
   15 CONTINUE 
      IF(NA.EQ.2)GOTO 20
      IF(NA.EQ.3)GOTO 30
      IF(NA.EQ.4)GOTO 40
      IF(NA.EQ.5)GOTO 50
      IF(NA.EQ.6)GOTO 60

C...determine Z(1) and Z(2) of two body process
   20 CALL TWOBDYZ(2,SCL,SMT,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...determine Z(1) and Z(2) of three body process
   30 CALL THRBDYZ(3,SCL,SMT,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 4 body processes
   40 CONTINUE
      CALL FOUBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11 

C...determine the {zi} values of 4 primary hadrons
      CALL FOUBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 5 body processes
   50 CONTINUE
      CALL FIVBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11

C...determine the {zi} values of 5 primary hadrons
      CALL FIVBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
      GOTO 100

C...light-cone momenta of two clusters in 6 body processes
   60 CALL SIXBDYZSMP(SCL,S1,S2,Z1,Z2)
      IF(IALPA(99).GE.1)GOTO 11

C...determine the {zi} values of 6 primary hadrons
      CALL SIXBDYZ(SCL,S1,S2,Z1,Z2,Z)
      IF(IALPA(99).GE.1)GOTO 11
  100 CONTINUE
      END   

C...Sample to Z(i) for 2-body events
      !******************************!
      SUBROUTINE TWOBDYZ(NBDY,S,MTS,Z)
      !******************************!
      REAL MTS1,MTS2,MTS(10),Z(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      SAVE /ARLWDAT1/,/ARLWDAT2/
      EXTERNAL RLU0,SMLAMDA
      ZRO=1E-26
      ONE=1.0
      TWO=2.0
      B=RALPA(1)
      IALPA(99)=0
      MTS1=MTS(1)
      MTS2=MTS(2)
      SMLMD=SMLAMDA(S,MTS1,MTS2)
      IF(SMLMD.LE.ZRO)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      SRLMD=SQRT(SMLMD)
     
C...Get the fraction of light-cone momenta of primary particles
      A1=(S+MTS1+MTS2-SRLMD)/TWO !the first solution
      A2=(S+MTS1+MTS2+SRLMD)/TWO !the second solution
      EXPA1=EXP(-B*A1)
      EXPA2=EXP(-B*A2)
      FA1=EXPA1/(EXPA1+EXPA2)
      FA2=EXPA2/(EXPA1+EXPA2)
    
C...Decide the Z1(+) and Z2(+)
      R=RLU0(0)
      IF(R.LE.FA1) THEN
       A=A1  !1st solution
      ELSE
       A=A2  !2nd solution
      ENDIF
      Z(1)=MTS1/(A-MTS2)
      Z(2)=ONE-Z(1)
      IF(Z(1).LE.ZRO.OR.Z(1).GE.ONE) THEN
c       WRITE(*,*)'in TWOBDYZ: Z2 unphysical',Z(1),Z(2)
       IALPA(99)=1
      ENDIF

C...Check by A and S conditions
      AX=MTS1/Z(1)+MTS2
      SX=MTS1/Z(1)+MTS2/Z(2)
      DA=AX-A
      DS=SX-S
      IF(ABS(DA).GT.RALFU(91).OR.ABS(DS).GT.RALFU(91))THEN
       IF(IALPA(95).GT.0)THEN
c        WRITE(*,*)'in TWOBDYZ: A & S errors'
c        WRITE(*,*)'A_in=',A,'   A_ou=',AX
c        WRITE(*,*)'S_in=',S,'   S_ou=',SX
       ENDIF
      ENDIF  
      END

C...Sample to Z(i) for 3-body processes
      !******************************!
      SUBROUTINE THRBDYZ(NBDY,S,MTS,Z)
      !******************************!
      REAL B,MTS(10),Z(10),ZP(10)
      REAL MTS1,MTS2,MTS3,LMDA,LMDCUT,DP3DA
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /ARLWDAT1/
      ZRO=1E-20
      ONE=1.0
      TWO=2.0
      FOU=4.0
      FTH=0.25
      HLF=0.5
      B=RALPA(1)
      IALPA(99)=0
      NTRY=10000000
      LMDCUT=1E-10
      ECM=SQRT(S)
      SX=S
      MTS1=MTS(1)
      MTS2=MTS(2)
      MTS3=MTS(3)
      ITRY=0
      
C...get the fraction of light-cone momenta of primary particles
      CALL A3MINAX(S,MTS1,MTS2,MTS3,AMIN,AMAX)
      IF(AMIN.LE.ZRO.OR.AMAX.LE.ZRO)THEN
       IALPA(99)=1
       RETURN
      ENDIF 
      FUNCL=DP3DA(S,MTS1,MTS2,MTS3,AMIN)
      FUNCR=DP3DA(S,MTS1,MTS2,MTS3,AMAX)
      SUP=MAX(FUNCL,FUNCR)
      IF(IALPA(31).NE.0)THEN
       IZ=IALPA(31)
      ELSE
       IZ=1
       R=RLU0(0)
       IF(R.GE.HLF)IZ=2
      ENDIF 
  100 ITRY=ITRY+1
      IF(ITRY.GT.NTRY)IALPA(99)=1
      IF(IALPA(99).GE.1)RETURN
      DTA=AMIN+(AMAX-AMIN)*RLU0(0)
      G=DP3DA(S,MTS1,MTS2,MTS3,DTA)/SUP
      ETA=RLU0(1)
      IF(ETA.GT.G)GOTO 100
      A=DTA
      U=MTS1+MTS2+MTS3
      V=MTS1*MTS2+MTS2*MTS3+MTS3*MTS1
      W=FOU*S*MTS1*MTS2*MTS3
      LMDA=((S-A)*(A-U)-V)*((S-A)*(A-U)-V)-W
      IF(LMDA.LT.LMDCUT)GOTO 100
      SRLMD=SQRT(LMDA)
      BX=A*(A-U)-S*(A+MTS1-MTS2-MTS3)+V
      Z1_1=(-BX+SRLMD)/(TWO*S*(A-MTS2-MTS3))
      Z2_1=Z1_1*(ONE-Z1_1)*MTS2/((A-MTS3)*Z1_1-MTS1)
      Z3_1=ONE-Z1_1-Z2_1
      Z1_2=(-BX-SRLMD)/(TWO*S*(A-MTS2-MTS3))
      Z2_2=Z1_2*(ONE-Z1_2)*MTS2/((A-MTS3)*Z1_2-MTS1)
      Z3_2=ONE-Z1_2-Z2_2
      CALL CHECKZ(S,A,MTS1,MTS2,MTS3,Z1_1,Z2_1,Z3_1)
      CALL CHECKZ(S,A,MTS1,MTS2,MTS3,Z1_2,Z2_2,Z3_2)
      IGOTO=0
      IZSET1=1
      IZSET2=1
      IF(IALPA(30).EQ.1) THEN
       Z1CUTL=MTS1/S
       Z1CUTR=ONE-(MTS2+MTS3)/S
       Z2CUTL_1=MTS2/(S-MTS1/Z1_1)
       Z2CUTR_1=ONE-Z1_1
       Z2CUTL_2=MTS2/(S-MTS1/Z1_2)
       Z2CUTR_2=ONE-Z1_2
       Z3CUTL_1=MTS3/(S-MTS1/Z1_1-MTS2/Z2_1)
       Z3CUTR_1=ONE
       Z3CUTL_2=MTS3/(S-MTS1/Z1_2-MTS2/Z2_2)
       Z3CUTR_2=ONE
       IF(Z1_1.LE.Z1CUTL.OR.Z1_1.GE.Z1CUTR.OR.
     &    Z1_2.LE.Z1CUTL.OR.Z1_2.GE.Z1CUTR.OR.
     &    Z2_1.LE.Z2CUTL_1.OR.Z2_1.GE.Z2CUTR_1.OR.
     &    Z2_2.LE.Z2CUTL_2.OR.Z2_2.GE.Z2CUTR_2.OR.
     &    Z3_1.LE.Z3CUTL_1.OR.Z3_1.GE.Z3CUTR_1.OR.
     &    Z3_2.LT.Z3CUTL_2.OR.Z3_2.GT.Z3CUTR_2) IGOTO=100
      ELSEIF(IALPA(30).EQ.2) THEN
       DZS1=ABS(ONE-Z1_1-Z2_1-Z3_1)
       DZS2=ABS(ONE-Z1_2-Z2_2-Z3_2)
       DZS=0.000001
       IF(Z1_1.LT.ZRO.OR.Z1_1.GT.ONE.OR.
     &    Z1_2.LT.ZRO.OR.Z1_2.GT.ONE.OR.
     &    Z2_1.LT.ZRO.OR.Z2_1.GT.ONE.OR.
     &    Z2_2.LT.ZRO.OR.Z2_2.GT.ONE.OR.
     &    Z3_1.LT.ZRO.OR.Z3_1.GT.ONE.OR.
     &    Z3_2.LT.ZRO.OR.Z3_2.GT.ONE.OR.
     &    DZS1.GT.DZS.OR.DZS2.GT.DZS) IGOTO=100
      ELSEIF(IALPA(30).EQ.3) THEN
       Z1CUTL=MTS1/S
       Z1CUTR=ONE-(MTS2+MTS3)/S
       Z2CUTL_1=MTS2/(S-MTS1/Z1_1)
       Z2CUTR_1=ONE-Z1_1
       Z2CUTL_2=MTS2/(S-MTS1/Z1_2)
       Z2CUTR_2=ONE-Z1_2
       Z3CUTL_1=ZRO
       Z3CUTR_1=ONE
       Z3CUTL_2=ZRO
       Z3CUTR_2=ONE
       Z3CUTR_2=ONE
       IF(Z1_1.LT.Z1CUTL.OR.Z1_1.GT.Z1CUTR.OR.
     &    Z2_1.LT.Z2CUTL_1.OR.Z2_1.GT.Z2CUTR_1.OR.
     &    Z3_1.LT.Z3CUTL_1.OR.Z3_1.GT.Z3CUTR_1) IZSET1=-1
       IF(Z1_2.LT.Z1CUTL.OR.Z1_2.GT.Z1CUTR.OR.
     &    Z2_2.LT.Z2CUTL_2.OR.Z2_2.GT.Z2CUTR_2.OR.
     &    Z3_2.LT.Z3CUTL_2.OR.Z3_2.GT.Z3CUTR_2) IZSET2=-1
       IF(IZSET1.GT.0.AND.IZSET2.LT.0) IZ=1
       IF(IZSET1.LT.0.AND.IZSET2.GT.0) IZ=2
       IF(IZSET1.LT.0.AND.IZSET2.LT.0) IGOTO=100
      ENDIF
      IF(IGOTO.EQ.100)GOTO 100
      IF(IZ.EQ.1) THEN
       Z(1)=Z1_1
       Z(2)=Z2_1
       Z(3)=Z3_1
      ELSEIF(IZ.EQ.2) THEN
       Z(1)=Z1_2
       Z(2)=Z2_2
       Z(3)=Z3_2
      ENDIF
      ZP(1)=Z(1)
      ZP(2)=Z(2)
      ZP(3)=Z(3)

*...check the resluts by area and energy
      AX=MTS1/Z(1)+(ONE-Z(1))*MTS2/Z(2)+MTS3
      SX=MTS1/Z(1)+MTS2/Z(2)+MTS3/Z(3)
      DA=AX-A
      DS=SX-S
      IF(ABS(DA).GT.RALFU(91).OR.ABS(DS).GT.RALFU(91))THEN
       IF(IALPA(95).GT.0)THEN
c        WRITE(*,*)'in THRBDYZ: Z(1,2,3) errors:'
c        WRITE(*,*)'S_in=',S,'S_ou=',SX
c        WRITE(*,*)'A_in=',A,'A_ou=',AX
       ENDIF
      ENDIF
      RETURN
      END

C...String four body decay 
      !*********************************!
      SUBROUTINE FOUBDYZ(S,S1,S2,Z1,Z2,Z)
      !*********************************!
      REAL MT(10),MTS(10),MTSC1(10),MTSC2(10),ZC1(10),ZC2(10)
      REAL ZCT1(10),ZCT2(10),Z(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/MT/MT,MTS
      SAVE /MT/
      ONE=1.0
      IALPA(99)=0
      B=RALPA(1)

*..check if Z1 and Z2 correct
      SX1=S1/Z1+S2/(ONE-Z1)
      SX2=S2/Z2+S1/(ONE-Z2)
      DS1=SX1-S
      DS2=SX2-S
      IF(ABS(DS1).GT.RALFU(91).OR.ABS(DS2).GT.RALFU(91))THEN
       IF(IALPA(95).GT.0)THEN
        WRITE(*,*)'in FOUBDYZ: S1,S2 errors'
        WRITE(*,*)'S1_in=',S1,'   S1_ou=',SX1
        WRITE(*,*)'S2_in=',S2,'   S2_ou=',SX2
       ENDIF
      ENDIF

*...transverse masses of two clusters
      MTSC1(1)=MTS(1)
      MTSC1(2)=MTS(2)
      MTSC2(1)=MTS(3)
      MTSC2(2)=MTS(4)

*...for the first cluster
      CALL TWOBDYZ(4,S1,MTSC1,ZC1)
      IF(IALPA(99).GE.1)RETURN
      CALL BOOSTTOTOT(2,1,S,Z1,Z2,ZC1,ZCT1)

*...for the second cluster
      CALL TWOBDYZ(4,S2,MTSC2,ZC2)
      IF(IALPA(99).GE.1)RETURN
      CALL BOOSTTOTOT(2,2,S,Z1,Z2,ZC2,ZCT2)

*....final Z(i) values in total c.m. system
      Z(1)=ZCT1(1)
      Z(2)=ZCT1(2)
      Z(3)=ZCT2(1)
      Z(4)=ZCT2(2)
      RETURN
      END

*...sampling Z(i) for 5-body events
      !*********************************!
      SUBROUTINE FIVBDYZ(S,S1,S2,Z1,Z2,Z)
      !*********************************!
      REAL MT(10),MTS(10),ZC1(10),ZC2(10),Z(10)
      REAL MTSC1(10),MTSC2(10),ZCT1(10),ZCT2(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/MT/MT,MTS
      SAVE /MT/
      ONE=1.0
      ZRO=1E-20
      DPM=1E-06
      B=RALPA(1)
      IALPA(99)=0

*..check if Z1 and Z2 correct
      SX1=S1/Z1+S2/(ONE-Z1)
      SX2=S2/Z2+S1/(ONE-Z2)
      DS1=SX1-S
      DS2=SX2-S
      IF(ABS(DS1).GT.RALFU(91).OR.ABS(DS2).GT.RALFU(91))THEN
       IF(IALPA(95).GT.0)THEN
        WRITE(*,*)'in FIVBDYZ: Z1,Z2 errors'
        WRITE(*,*)'S1_in=',S1,'  S1_ou=',SX1
        WRITE(*,*)'S2_in=',S2,'  S2_ou=',SX2
       ENDIF
      ENDIF

*...transverse masses of two clusters
      MTSC1(1)=MTS(1)
      MTSC1(2)=MTS(2)
      MTSC2(1)=MTS(3)
      MTSC2(2)=MTS(4)
      MTSC2(3)=MTS(5)

*...for the first cluster
      CALL TWOBDYZ(5,S1,MTSC1,ZC1)
      IF(IALPA(99).GE.1)RETURN
      CALL BOOSTTOTOT(2,1,S,Z1,Z2,ZC1,ZCT1)

*...for the second cluster
      CALL THRBDYZ(5,S2,MTSC2,ZC2)
      IF(IALPA(99).GE.1)RETURN
      CALL BOOSTTOTOT(3,2,S,Z1,Z2,ZC2,ZCT2)
     
*...z values 
      Z(1)=ZCT1(1)
      Z(2)=ZCT1(2)
      Z(3)=ZCT2(1)
      Z(4)=ZCT2(2)
      Z(5)=ZCT2(3)
      RETURN
      END

*...sampling for 6-body events
      !*********************************!
      SUBROUTINE SIXBDYZ(S,S1,S2,Z1,Z2,Z)
      !*********************************!
      REAL B,ZC1(10),ZC2(10),Z(10),ZCT1(10),ZCT2(10)
      REAL MT(10),MTS(10),MTSC1(10),MTSC2(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/MT/MT,MTS
      SAVE /ARLWDAT1/
      ONE=1.0
      DPM=1E-06
      IALPA(99)=0
      B=RALPA(1)
      
*..check if Z1 and Z2 correct
      SX1=S1/Z1+S2/(ONE-Z1)
      SX2=S2/Z2+S1/(ONE-Z2)
      DS1=SX1-S
      DS2=SX2-S
      IF(ABS(DS1).GT.RALFU(91).OR.ABS(DS2).GT.RALFU(91))THEN
       IF(IALPA(95).GT.0)THEN
        WRITE(*,*)'in SIXBDYZ: Z1,Z2 errors'
        WRITE(*,*)'S1_in=',S1,'S1_ou=',SX1
        WRITE(*,*)'S2_in=',S2,'S2_ou=',SX2
       ENDIF
      ENDIF

*...transverse masses of two clusters
      DO J=1,3
       MTSC1(J)=MTS(J)
       MTSC2(J)=MTS(3+J)
      ENDDO 

*...for the first cluster
      CALL THRBDYZ(6,S1,MTSC1,ZC1)
      IF(IALPA(99).GE.1)RETURN
      CALL BOOSTTOTOT(3,1,S,Z1,Z2,ZC1,ZCT1)

*...for the second cluster
      CALL THRBDYZ(6,S2,MTSC2,ZC2)
      IF(IALPA(99).GE.1)RETURN
      CALL BOOSTTOTOT(3,2,S,Z1,Z2,ZC2,ZCT2)
       
*...z values
      DO J=1,3
       Z(J)=ZCT1(J)
       Z(3+J)=ZCT2(J)
      ENDDO
      RETURN
      END

*...sampling for 2-clusters in 4-body production
      !**********************************!
      SUBROUTINE FOUBDYZSMP(S,S1,S2,Z1,Z2)
      !**********************************!
      REAL MT(10),MTS(10)
      REAL F4Q1Q2_1(100000),F4Q1Q2_2(100000)
      EXTERNAL SAMPF,RLU0
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/MT/MT,MTS
      SAVE /ARLWDAT1/
      NBDY=4
      ZRO=0.0
      ONE=1.0
      TWO=2.0
      FOU=4.0
      NTRY=1000000
      IALPA(99)=0
      B=RALPA(1)
      EPSL=1E-20
      ECM=SQRT(S)
      TM=MT(1)+MT(2)+MT(3)+MT(4)+RALPA(86)
      IF(TM.GE.ECM)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      RTEM=SQRT(ECM-TM)
      DNQ=RALFU(94)
      NQ=INT(RTEM/DNQ)
      IF(NQ*NQ.GT.100000)THEN
       DO J=1,1000
	DNQ=RALFU(94)+0.001*J
	NQ=INT(RTEM/DNQ)
	IF(NQ*NQ.LE.100000)GOTO 5
       ENDDO
      ENDIF
    5 SUPF4_1=ZRO
      SUPF4_2=ZRO
      LINE1=0
      LINE2=0
      ITRY=0

*...minimum squared c.m.s. energies of subsystems
      SMIN1=(MT(1)+MT(2))*(MT(1)+MT(2))
      SMIN2=(MT(3)+MT(4))*(MT(3)+MT(4))
      
*...all posible values of sampling function in physical region
      DO I=1,NQ
       DO J=1,NQ
        Q1=I*DNQ !RALFU(94)
        Q2=J*DNQ !RALFU(94)
        E1=Q1*Q1+MT(1)+MT(2)
        E2=Q2*Q2+MT(3)+MT(4)
        IF(ECM.LE.(E1+E2+EPSL))GOTO 33
        S1=E1*E1
        S2=E2*E2 
        SLMD=SMLAMDA(S,S1,S2) 
        IF(S1.GT.SMIN1.AND.S2.GT.SMIN2.AND.SLMD.GT.ZRO)THEN
         RTSLMD=SQRT(SLMD)
         GAMA1=(S-S1+S2-RTSLMD)*(S+S1-S2-RTSLMD)/FOU/S
         GAMA2=(S-S1+S2+RTSLMD)*(S+S1-S2+RTSLMD)/FOU/S
         SMP=SAMPF(NBDY,Q1,Q2)
         IF(IALPA(99).GE.1)GOTO 33
         IF(GAMA1.GT.ZRO)THEN
          LINE1=LINE1+1
          F4Q1Q2_1(LINE1)=SMP*EXP(-B*GAMA1)
         ENDIF
         IF(GAMA2.GT.ZRO)THEN
          LINE2=LINE2+1
          F4Q1Q2_2(LINE2)=SMP*EXP(-B*GAMA2)
         ENDIF
        ENDIF
   33   CONTINUE
       ENDDO
      ENDDO
      IF(LINE1.LE.1.AND.LINE2.LE.1)THEN
       IALPA(99)=1
       RETURN
      ENDIF
    
*...recorder for F4(z1,z2) in non-decreased order
      CALL HEAP(F4Q1Q2_1,LINE1,1,LINE1)
      CALL HEAP(F4Q1Q2_2,LINE2,1,LINE2)

*...give out the SUP
      NTAKE1=RALFU(90)*LINE1
      NTAKE2=RALFU(90)*LINE2
      SUPF4_1=F4Q1Q2_1(NTAKE1)
      SUPF4_2=F4Q1Q2_2(NTAKE2)
    
*...determine the Z1 and Z2 of this event
   10 IALPA(99)=0
      ITRY=ITRY+1
      IF(ITRY.GT.NTRY)IALPA(99)=1
      IF(IALPA(99).GE.1)RETURN
      Q1=RTEM*RLU0(0)
      Q2=RTEM*RLU0(0)
      E1=Q1*Q1+MT(1)+MT(2)
      E2=Q2*Q2+MT(3)+MT(4)
      ET=E1+E2+EPSL
      S1=E1*E1
      S2=E2*E2
      IF(S1.LE.SMIN1.OR.S2.LE.SMIN2.OR.ECM.LT.ET)GOTO 10
      SLMD=SMLAMDA(S,S1,S2)
      IF(SLMD.LT.EPSL)GOTO 10
      RTSLMD=SQRT(SLMD)
      GAMA1=(S-S1+S2-RTSLMD)*(S+S1-S2-RTSLMD)/FOU/S
      GAMA2=(S-S1+S2+RTSLMD)*(S+S1-S2+RTSLMD)/FOU/S
      UP=SAMPF(NBDY,Q1,Q2)
      SMPF=UP/RTSLMD
      IF(IALPA(99).GE.1.OR.SMPF.LE.EPSL)GOTO 10
      F4_1=SMPF*EXP(-B*GAMA1) !1st GAMMA solution
      F4_2=SMPF*EXP(-B*GAMA2) !2nd GAMMA solution
      F41=F4_1/(F4_1+F4_2)
      F42=F4_2/(F4_1+F4_2)
      R=RLU0(0)
      ISO=0
      IF(R.LT.F41.AND.NTAKE1.GT.1)THEN
       RF4=F4_1/SUPF4_1
       F4=F4_1
       ISO=1
      ELSEIF(R.GE.F41.AND.NTAKE2.GT.1)THEN
       RF4=F4_2/SUPF4_2
       F4=F4_2
       ISO=2
      ENDIF
      RX=RLU0(1)
      IF(RX.GT.RF4)GOTO 10
      IF(ISO.EQ.1.AND.NTAKE1.GT.1)THEN
       Z1=(S+S1-S2+RTSLMD)/TWO/S
       Z2=(S-S1+S2+RTSLMD)/TWO/S
      ELSEIF(ISO.EQ.2.AND.NTAKE2.GT.1)THEN
       Z1=(S+S1-S2-RTSLMD)/TWO/S
       Z2=(S-S1+S2-RTSLMD)/TWO/S
      ENDIF
      RETURN
      END

      !**********************************!
      SUBROUTINE FIVBDYZSMP(S,S1,S2,Z1,Z2)
      !**********************************!
      REAL MT(10),MTS(10)
      REAL F5Q1Q2_1(100000),F5Q1Q2_2(100000)
      EXTERNAL SAMPF,RLU0
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/MT/MT,MTS
      SAVE /ARLWDAT1/,/MT/
      NBDY=5
      ZRO=1E-26
      ONE=1.0
      TWO=2.0
      FOU=4.0
      IALPA(99)=0
      B=RALPA(1)
      IALFU(23)=3
      NTRY=1000000
      EPSL=1E-20
      CUT=1E-20
      ECM=SQRT(S)
      TM=MT(1)+MT(2)+MT(3)+MT(4)+MT(5)+RALPA(86)
      IF(TM.GE.ECM)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      RTEM=SQRT(ECM-TM)
      DNQ=RALFU(95)
      NQ=INT(RTEM/DNQ)
      IF(NQ*NQ.GT.100000)THEN
       DO J=1,1000
        DNQ=RALFU(94)+0.001*J
        NQ=INT(RTEM/DNQ)
        IF(NQ*NQ.LE.100000)GOTO 5
       ENDDO
      ENDIF
    5 SUPF=ZRO
      LINE1=0
      LINE2=0
      ITRY=0
      
*...minimum squared c.m.s. energies of subsystems
      SMIN1=(MT(1)+MT(2))*(MT(1)+MT(2))
      SMIN2=(MT(3)+MT(4)+MT(5))*(MT(3)+MT(4)+MT(5))

*...all posible values of sampling function in physical region
      DO I=1,NQ
       DO J=1,NQ
        Q1=I*DNQ !RALFU(95)
        Q2=J*DNQ !RALFU(95)
        E1=Q1*Q1+MT(1)+MT(2)
        E2=Q2*Q2+MT(3)+MT(4)+MT(5)
        IF(ECM.LE.(E1+E2+EPSL))GOTO 33
        S1=E1*E1
        S2=E2*E2
        SLMD=SMLAMDA(S,S1,S2)
        IF(S1.GT.SMIN1.AND.S2.GT.SMIN2.AND.SLMD.GT.EPSL)THEN
         RTSLMD=SQRT(SLMD)
         GAMA1=(S-S1+S2-RTSLMD)*(S+S1-S2-RTSLMD)/FOU/S
         GAMA2=(S-S1+S2+RTSLMD)*(S+S1-S2+RTSLMD)/FOU/S
         SMP=SAMPF(NBDY,Q1,Q2)/RTSLMD
         IF(IALPA(99).GE.1)GOTO 33
         IF(GAMA1.GT.ZRO)THEN
          LINE1=LINE1+1
          F5Q1Q2_1(LINE1)=SMP*EXP(-B*GAMA1)
         ENDIF
         IF(GAMA2.GT.ZRO)THEN
          LINE2=LINE2+1
          F5Q1Q2_2(LINE2)=SMP*EXP(-B*GAMA2)
         ENDIF
        ENDIF
   33   CONTINUE
       ENDDO
      ENDDO
      IF(LINE1.LE.1.AND.LINE2.LE.1)THEN
       IALPA(99)=1
       RETURN
      ENDIF

*...reorder for F5(z1,z2) in non-decreased order
      CALL HEAP(F5Q1Q2_1,LINE1,1,LINE1)
      CALL HEAP(F5Q1Q2_2,LINE2,1,LINE2)
    
*...give out the SUP
      NTAKE1=RALFU(90)*LINE1
      NTAKE2=RALFU(90)*LINE2
      SUPF5_1=F5Q1Q2_1(NTAKE1)
      SUPF5_2=F5Q1Q2_2(NTAKE2)

*...determine the Z1 and Z2 of this event
   10 IALPA(99)=0
      ITYR=ITRY+1
      IF(ITRY.GE.NTRY)IALPA(99)=1
      IF(IALPA(99).GE.1)RETURN
      Q1=RTEM*RLU0(0)
      Q2=RTEM*RLU0(0)
      E1=Q1*Q1+MT(1)+MT(2)
      E2=Q2*Q2+MT(3)+MT(4)+MT(5)
      ET=E1+E2+EPSL
      IF(ECM.LE.ET)GOTO 10
      S1=E1*E1
      S2=E2*E2
      SLMD=SMLAMDA(S,S1,S2)
      IF(SLMD.LT.CUT)GOTO 10
      RTSLMD=SQRT(SLMD) 
      GAMA1=(S-S1+S2-RTSLMD)*(S+S1-S2-RTSLMD)/FOU/S
      GAMA2=(S-S1+S2+RTSLMD)*(S+S1-S2+RTSLMD)/FOU/S
      SMPF=SAMPF(NBDY,Q1,Q2)/RTSLMD
      IF(IALPA(99).GE.1.OR.SMPF.LE.CUT)GOTO 10
      F5_1=SMPF*EXP(-B*GAMA1) !1st GAMMA solution
      F5_2=SMPF*EXP(-B*GAMA2) !2nd GAMMA solution
      F51=F5_1/(F5_1+F5_2)
      F52=F5_2/(F5_1+F5_2)
      R=RLU0(0)
      IF(R.LT.F51)THEN
       RF5=F5_1/SUPF5_1
       F5=F5_1
       ISO=1
      ELSE
       RF5=F5_2/SUPF5_2
       F5=F5_2
       ISO=2
      ENDIF
      RX=RLU0(1)
      IF(RX.GT.RF5)GOTO 10
      IF(ISO.EQ.1)THEN
       Z1=(S+S1-S2+RTSLMD)/TWO/S
       Z2=(S-S1+S2+RTSLMD)/TWO/S
      ELSEIF(ISO.EQ.2)THEN
       Z1=(S+S1-S2-RTSLMD)/TWO/S
       Z2=(S-S1+S2-RTSLMD)/TWO/S
      ENDIF
      ZMIN=MIN(Z1,Z2)
      ZMAX=MAX(Z1,Z2)
      IF(ZMIN.LE.ZRO.OR.ZMAX.GE.ONE)GOTO 10
      RETURN
      END

      !**********************************!
      SUBROUTINE SIXBDYZSMP(S,S1,S2,Z1,Z2)
      !**********************************!
      REAL F6Q1Q2_1(100000),F6Q1Q2_2(100000)
      REAL MT(10),MTS(10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      EXTERNAL SAMPF,RLU0
      COMMON/MT/MT,MTS
      SAVE /ARLWDAT1/
      NBDY=6
      ZRO=0.0
      ONE=1.0
      TWO=2.0
      FOU=4.0
      IALPA(99)=0
      B=RALPA(1)
      NTRY=1000000
      EPSL=1E-10
      CUT=1E-18
      ECM=SQRT(S)
      TM=MT(1)+MT(2)+MT(3)+MT(4)+MT(5)+MT(6)+RALPA(86)
      IF(TM.GE.ECM)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      RTEM=SQRT(ECM-TM)
      DNQ=RALFU(96)
      NQ=INT(RTEM/DNQ)
      IF(NQ*NQ.GT.100000)THEN
       DO J=1,1000
        DNQ=RALFU(94)+0.001*J
        NQ=INT(RTEM/DNQ)
        IF(NQ*NQ.LE.100000)GOTO 5
       ENDDO
      ENDIF
    5 LINE1=0
      LINE2=0
      ITRY=0
      
*...minimum squared c.m.s. energies of subsystems
      SMIN1=(MT(1)+MT(2)+MT(3))*(MT(1)+MT(2)+MT(3))
      SMIN2=(MT(4)+MT(5)+MT(6))*(MT(4)+MT(5)+MT(6))

*...all posible values of sampling function in physical region
      DO I=1,NQ
       DO J=1,NQ
        Q1=I*DNQ !RALFU(96)
        Q2=J*DNQ !RALFU(96)
        E1=Q1*Q1+MT(1)+MT(2)+MT(3)
        E2=Q2*Q2+MT(4)+MT(5)+MT(6)
        IF(ECM.LE.(E1+E2))GOTO 33
        S1=E1*E1
        S2=E2*E2
        SLMD=SMLAMDA(S,S1,S2)
        IF(S1.GT.SMIN1.AND.S2.GT.SMIN2.AND.SLMD.GT.ZRO)THEN
         RTSLMD=SQRT(SLMD)
         GAMA1=(S-S1+S2-RTSLMD)*(S+S1-S2-RTSLMD)/FOU/S
         GAMA2=(S-S1+S2+RTSLMD)*(S+S1-S2+RTSLMD)/FOU/S
         SMP=SAMPF(NBDY,Q1,Q2)/RTSLMD
         IF(IALPA(99).GE.1)GOTO 33
         IF(GAMA1.GT.ZRO)THEN
          LINE1=LINE1+1
          F6Q1Q2_1(LINE1)=SMP*EXP(-B*GAMA1)
         ENDIF
         IF(GAMA2.GT.ZRO)THEN
          LINE2=LINE2+1
          F6Q1Q2_2(LINE2)=SMP*EXP(-B*GAMA2)
         ENDIF
        ENDIF
   33   CONTINUE
       ENDDO
      ENDDO
      IF(LINE1.LE.1.AND.LINE2.LE.1)THEN
       IALPA(99)=1
       RETURN
      ENDIF

*...recorder for F4(z1,z2) in non-decreased order
      CALL HEAP(F6Q1Q2_1,LINE1,1,LINE1)
      CALL HEAP(F6Q1Q2_2,LINE2,1,LINE2)

*...give out the SUP
      NTAKE1=RALFU(90)*LINE1
      NTAKE2=RALFU(90)*LINE2
      SUPF6_1=F6Q1Q2_1(NTAKE1)
      SUPF6_2=F6Q1Q2_2(NTAKE2)

*...determine the Z1 and Z2 of this event
   10 ITRY=ITRY+1
      IF(ITRY.GE.NTRY)IALPA(99)=1
      IF(IALPA(99).GE.1)RETURN
      Q1=RTEM*RLU0(0)
      Q2=RTEM*RLU0(0)
      E1=Q1*Q1+MT(1)+MT(2)+MT(3)
      E2=Q2*Q2+MT(4)+MT(5)+MT(6)
      IF(ECM.LE.(E1+E2+EPSL))GOTO 10
      S1=E1*E1
      S2=E2*E2
      SLMD=SMLAMDA(S,S1,S2)
      IF(SLMD.LT.EPSL)GOTO 10
      RTSLMD=SQRT(SLMD)
      GAMA1=(S-S1+S2-RTSLMD)*(S+S1-S2-RtSLMD)/FOU/S
      GAMA2=(S-S1+S2+RTSLMD)*(S+S1-S2+RTSLMD)/FOU/S
      SMPF=SAMPF(NBDY,Q1,Q2)/RTSLMD
      IF(IALPA(99).GE.1.OR.SMPF.LE.CUT)GOTO 10
      F6_1=SMPF*EXP(-B*GAMA1) !1st GAMMA solution
      F6_2=SMPF*EXP(-B*GAMA2) !2nd GAMMA solution
      F61=F6_1/(F6_1+F6_2)
      F62=F6_2/(F6_1+F6_2)
      R=RLU0(0)
      IF(R.LT.F61)THEN
       RF6=F6_1/SUPF6_1
       F6=F6_1
       ISO=1
      ELSE
       RF6=F6_2/SUPF6_2
       F6=F6_2
       ISO=2
      ENDIF
      RX=RLU0(1)
      IF(RX.GT.RF6)GOTO 10
      IF(ISO.EQ.1)THEN
       Z1=(S+S1-S2+RTSLMD)/TWO/S
       Z2=(S-S1+S2+RTSLMD)/TWO/S
      ELSEIF(ISO.EQ.2)THEN
       Z1=(S+S1-S2-RTSLMD)/TWO/S
       Z2=(S-S1+S2-RTSLMD)/TWO/S
      ENDIF
      RETURN
      END

      !************************!
      FUNCTION SAMPF(NBDY,Q1,Q2)
      !************************!
      REAL MT(10),MTS(10),MCS1,MCD1,MCS2,MCD2
      REAL MTSI,MTSJ,MTSK,MTS1,MTS2,MTS3,MTS4,MTS5,MTS6
      EXTERNAL FSUB,RGMLT
      COMMON/MT/MT,MTS
      COMMON/FSUBUSE/SX,MTSI,MTSJ,MTSK
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /ARLWDAT1/,/MT/
      ZRO=1E-26 
      ONE=1.0
      TWO=2.0
      FOU=4.0
      HLF=0.5
      F1=ZRO
      F2=ZRO
      EPS=1E-01
      SAMPF=ZRO
      C1=Q1*Q1
      C2=Q2*Q2
      B=RALPA(1)
      IALPA(99)=0
      IALFU(23)=3
      NI=IALFU(93)
      NG=IALFU(94)
      MTS1=MTS(1)
      MTS2=MTS(2)
      MTS3=MTS(3)
      MTS4=MTS(4)
      MTS5=MTS(5)
      MTS6=MTS(6)
      
*...go to corresponding process
      IF(NBDY.EQ.4) GOTO 40
      IF(NBDY.EQ.5) GOTO 50
      IF(NBDY.EQ.6) GOTO 60

*...for 4-body
   40 CONTINUE
      MCS1=MT(1)+MT(2)
      MCS2=MT(3)+MT(4)
      MCD1=MT(1)-MT(2)
      MCD2=MT(3)-MT(4)
      E1=C1+MCS1
      E2=C2+MCS2
      S1=E1*E1
      S2=E2*E2
      HI1=S1-MCD1*MCD1
      HI2=S2-MCD2*MCD2
      IF(HI1.LE.ZRO.OR.HI2.LE.ZRO)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      FC1=FOU*E1/SQRT(HI1*(C1+TWO*MCS1))
      FC2=FOU*E2/SQRT(HI2*(C2+TWO*MCS2))
      SLMD1=SMLAMDA(S1,MTS1,MTS2)
      SLMD2=SMLAMDA(S2,MTS3,MTS4)
      IF(SLMD1.LE.ZRO.OR.SLMD2.LE.ZRO)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      RSMLMD1=SQRT(SLMD1)
      RSMLMD2=SQRT(SLMD2)
      AS11=HLF*(S1+MTS1+MTS2-RSMLMD1)
      AS12=HLF*(S1+MTS1+MTS2+RSMLMD1)
      AS21=HLF*(S2+MTS3+MTS4-RSMLMD2)
      AS22=HLF*(S2+MTS3+MTS4+RSMLMD2)
      F1=FC1*(EXP(-B*AS11)+EXP(-B*AS12))
      F2=FC2*(EXP(-B*AS21)+EXP(-B*AS22))
      SAMPF=F1*F2
      RETURN

*...for 5-body
   50 CONTINUE
*....first cluster with 2 hadrons
      MCS1=MT(1)+MT(2)
      MCD1=MT(1)-MT(2)
      E1=C1+MCS1
      S1=E1*E1
      HI=S1-MCD1*MCD1
      IF(HI.LE.ZRO)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      FC=FOU*E1/SQRT(HI*(C1+TWO*MCS1))
      RSMLMD=SQRT(SMLAMDA(S1,MTS1,MTS2))
      AS1=HLF*(S1+MTS1+MTS2-RSMLMD)
      AS2=HLF*(S1+MTS1+MTS2+RSMLMD)
      F1=FC*(EXP(-B*AS1)+EXP(-B*AS2))
*....second cluster with 3 hadrons
      MTSI=MTS(3)
      MTSJ=MTS(4)
      MTSK=MTS(5)
      E2=C2+MT(3)+MT(4)+MT(5)
      S2=E2*E2
      SX=S2    
      FC=FOU*Q2*E2
      CALL A3MINAX(S2,MTSI,MTSJ,MTSK,AMIN,AMAX)
      OUT=RGMLT(FSUB,AMIN,AMAX,NI,NG)
      F2=FC*OUT
      SAMPF=F1*F2
      RETURN

C...For 6-body
   60 CONTINUE
C.....1st cluster
      MTSI=MTS(1)
      MTSJ=MTS(2)
      MTSK=MTS(3)
      E1=Q1*Q1+MT(1)+MT(2)+MT(3)
      S1=E1*E1
      SX=S1
      FC=FOU*Q1*E1
      CALL A3MINAX(S1,MTSI,MTSJ,MTSK,AMIN,AMAX)
      OUT=RGMLT(FSUB,AMIN,AMAX,NI,NG)
      F1=FC*OUT
C.....2nd cluster
      MTSI=MTS(4)
      MTSJ=MTS(5)
      MTSK=MTS(6)
      E2=Q2*Q2+MT(4)+MT(5)+MT(6)
      S2=E2*E2
      SX=S2
      FC=FOU*Q2*E2
      CALL A3MINAX(S2,MTSI,MTSJ,MTSK,AMIN,AMAX)
      OUT=RGMLT(FSUB,AMIN,AMAX,NI,NG)
      F2=FC*OUT
      SAMPF=F1*F2
      RETURN 
      END 


C...Kinematic factor lambda
      !*********************!
      FUNCTION SMLAMDA(X,Y,Z)
      !*********************!
      TWO=2.0
      TYN=1E-10
      SMLAMDA=X*X+Y*Y+Z*Z-TWO*(X*Y+Y*Z+Z*X)
      IF(SMLAMDA.LT.TYN)SMLAMDA=TYN
      RETURN
      END

C...Boost Zi from subsystem to total c.m. system
      !***************************************!
      SUBROUTINE BOOSTTOTOT(N,I,S,Z1,Z2,ZC,ZCT)
      !***************************************!
      REAL ZC(10),ZCT(10)
      HLF=0.5
      ONE=1.0

C...c.m.s energy of cluster I
      IF(I.EQ.1)SS=Z1*(ONE-Z2)*S 
      IF(I.EQ.2)SS=Z2*(ONE-Z1)*S
      ES=SQRT(SS)
      EC=SQRT(S)
     
C...Rapidity of subsystem 
      IF(I.EQ.1)YC=HLF*LOG(Z1/(ONE-Z2))
      IF(I.EQ.2)YC=HLF*LOG((ONE-Z1)/Z2)

C...Transformation factor
      FACP=EXP(YC) !FACP=EXP(-YC) ? only to get p+

C...p+ 
      DO J=1,N
       PP=ZC(J)*ES*FACP
       ZCT(J)=PP/EC
      ENDDO
      END

*...Euler rotations
      !***********************************!
      SUBROUTINE EULERROT(ID,ANGLE,XIN,XOU)
      !***********************************!
      REAL ANGLE(3),XIN(3),XOU(3),A(3,3),B(3,3)
      ZRO=0.0
      CSPHI=COS(ANGLE(1))
      SNPHI=SIN(ANGLE(1))
      CSTHE=COS(ANGLE(2))
      SNTHE=SIN(ANGLE(2))
      CSPSI=COS(ANGLE(3))
      SNPSI=SIN(ANGLE(3))
      A(1,1)=+CSPSI*CSPHI-CSTHE*SNPHI*SNPSI
      A(1,2)=+CSPHI*SNPHI+CSTHE*CSPHI*SNPSI
      A(1,3)=+SNPSI*SNTHE
      A(2,1)=-SNPSI*CSPHI-CSTHE*SNPHI*CSPSI
      A(2,2)=-SNPSI*SNPHI+CSTHE*CSPHI*CSPSI
      A(2,3)=+CSPSI*SNTHE
      A(3,1)=+SNTHE*SNPHI
      A(3,2)=-SNTHE*CSPHI
      A(3,3)=+CSTHE
      B(1,1)=+CSPSI*CSPHI-CSTHE*SNPHI*SNPSI
      B(1,2)=-SNPSI*CSPHI-CSTHE*SNPHI*CSPSI
      B(1,3)=+SNTHE*SNPHI
      B(2,1)=+CSPSI*SNPHI+CSTHE*CSPHI*SNPSI
      B(2,2)=-SNPSI*SNPHI+CSTHE*CSPHI*CSPSI
      B(2,3)=-SNTHE*CSPHI
      B(3,1)=+SNTHE*SNPSI
      B(3,2)=+SNTHE*CSPSI
      B(3,3)=+CSTHE
      IF(ID.GT.0)THEN
       DO I=1,3
        OUT=ZRO
        DO J=1,3
         OUT=OUT+A(I,J)*XIN(J)
        ENDDO
        XOU(I)=OUT
       ENDDO
      ELSEIF(ID.LT.0)THEN
       DO I=1,3
        OUT=ZRO
        DO J=1,3
         OUT=OUT+B(I,J)*XIN(J)
        ENDDO
        XOU(I)=OUT
       ENDDO
      ENDIF
      END 

C...To determine the region of A3 in 3-body fragmentation
      !********************************************!
      SUBROUTINE A3MINAX(S,MTSI,MTSJ,MTSK,AMIN,AMAX)
      !********************************************!
      REAL S,ECM,MTSI,MTSJ,MTSK,AMIN,AMAX
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /ARLWDAT3/
      ZRO=0.0
      ONE=1.0
      TWO=2.0
      FOU=4.0
      FSU=-1.0
      DA=1E-08
      ECM=SQRT(S)

C...Basic requirement for Amin and Amax
      AMIN0=MTSI+MTSJ+MTSK+DA
      AMAX0=S-DA    

C...Require capital-lambda is positive
      XAU(11)=AMIN0
      XAU(12)=MTSI*MTSJ+MTSJ*MTSK+MTSK*MTSI
      XAU(13)=FOU*S*MTSI*MTSJ*MTSK
      U=XAU(11)
      V=XAU(12)
      W=XAU(13)
      RW=SQRT(W) 
      Z=(S+U)/TWO
      X=Z*Z-S*U-V-RW
      IF(X.GT.ZRO)THEN
       RTX=SQRT(X) 
       AMIN=MAX(U,(Z-RTX))+DA
       AMAX=MIN(S,(Z+RTX))-DA
       IF(AMIN.GE.AMAX)THEN
        AMIN=FSU
        AMAX=FSU
       ENDIF
      ELSE
       AMIN=FSU
       AMAX=FSU
      ENDIF
      END

C...Area distributions function of 3 body
      !********************************!
      FUNCTION DP3DA(S,MTSI,MTSJ,MTSK,A)
      !********************************!
      IMPLICIT NONE
      INTEGER IALFU,IALPA
      REAL RALFU,RALPA
      REAL A,B,S,MTSI,MTSJ,MTSK,DP3DA
      REAL LMDA,LMDCUT,RLU0,FOU,U,V,W
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /ARLWDAT1/
      EXTERNAL RLU0
      FOU=4.0
      LMDCUT=1E-08
      B=RALPA(1)
      U=MTSI+MTSJ+MTSK
      V=MTSI*MTSJ+MTSJ*MTSK+MTSK*MTSI
      W=FOU*S*MTSI*MTSJ*MTSK
      LMDA=((S-A)*(A-U)-V)*((S-A)*(A-U)-V)-W
      IF(LMDA.LT.LMDCUT)LMDA=LMDCUT*RLU0(1)
      DP3DA=EXP(-B*A)/SQRT(LMDA)
      RETURN
      END

C...Check Z(i) if values being reasonable
      !********************************************!
      SUBROUTINE CHECKZ(S,A,SMT1,SMT2,SMT3,Z1,Z2,Z3)
      !********************************************!
      DPERMIT=0.1
      ONE=1.0
      IERR1=0
      IERR2=0
      A3=SMT1/Z1+(ONE-Z1)*SMT2/Z2+SMT3
      SZ=SMT1/Z1+SMT2/Z2+SMT3/Z3
      DA=ABS(A3-A)
      DS=ABS(SZ-S)
      IF(DA.GT.DPERMIT)IERR1=1
      IF(DS.GT.DPERMIT)IERR2=1
      IF(IERR1.GE.1.OR.IERR2.GE.1)THEN
       CALL LUERRM0(2,'(CHECKZ:) Zi are not the correct solutions') 
        WRITE(*,*)'S_in = ',S, '   S_ou = ',SZ
        WRITE(*,*)'A_cal= ',A3,'   A_sam= ',A
      ENDIF
      END

*...momentum conservation check before output
      !***************!
      SUBROUTINE ALCHEK
      !***************!
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUJETS/,/ARLWDAT1/,/ARLWDAT3/
      D=RALFU(92)
      IALPA(99)=0
      SX=0
      SY=0
      SZ=0
      SE=0
      DO J=1,N
       IF(K(J,1).EQ.1.OR.K(J,1).EQ.4)THEN
        SX=SX+P(J,1)
	SY=SY+P(J,2)
	SZ=SZ+P(J,3)
	SE=SE+P(J,4)
       ENDIF	
      ENDDO
      SUME=SE
      SE=ABS(XAU(1)-SE)
      IF(SX.GT.D.OR.SY.GT.D.OR.SZ.GT.D.OR.SE.GT.D)THEN
c       WRITE(*,*)'Possible momentum consercation violation :'
c       WRITE(*,*)'Sum of Px,Py,Pz =',sx,sy,sz
c       WRITE(*,*)'Ecm, SumE Ecm-SumE =',XAU(2),SUME,SE
c       CALL LULIST0(1)
c       WRITE(*,*)'??????????????????????'
       IALPA(99)=1
      ENDIF
      END

*...write out mulplicity informations
      !*********************!
      SUBROUTINE ALMUPD(JDEX)
      !*********************!
      INTEGER IPPR(100),IPHD(100),IPCH(100)
      REAL PPR(100),PHD(100),PCH(100),PN(10)
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE IPPR,PPR,IPHD,PHD,IPCH,PCH
      SAVE /LUJETS/,/ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/
      NHPR=0
      NHAD=0
      NCHR=0
      ONE=1.0
      IF(JDEX.LE.0)GOTO 50  !Predicted multiplicity distribution
      IF(JDEX.EQ.2)GOTO 30
      
*...initialisation
      IF(JDEX.LE.0)THEN
       DO I=1,100
        IPPR(I)=0
	IPHD(I)=0
	IPCH(I)=0
       ENDDO
       RETURN
      ENDIF 

*...statistics to primary hadrons
      DO J=KAU(3)+1,N
       IF(K(J,3).EQ.KAU(3))THEN
        IF(K(K(J,3),2).EQ.92)NHPR=NHPR+1
       ENDIF	
      ENDDO
      IPPR(NHPR)=IPPR(NHPR)+1
   
*...statistics to final state of single event    
c      CALL LUEDIT0(2)
      DO J=1,N
       K1=K(J,1)
       K2=IABS(K(J,2))
       IF(K1.EQ.1.OR.K1.EQ.4)THEN
        IF(K2.EQ.11.OR.K2.EQ.13.OR.K2.GT.100)NHAD=NHAD+1
        IF(LUCHGE0(K2).NE.0)NCHR=NCHR+1
       ENDIF
      ENDDO
      IF(NHAD.GT.1)IPHD(NHAD)=IPHD(NHAD)+1
      IF(NCHR.GT.1)IPCH(NCHR)=IPCH(NCHR)+1
      WRITE(54,*)NCHR !write to UNIT=54,FILE=mulchrg.dat
      IF(JDEX.LT.2)RETURN
      
*...final informations for multiplicity
   30 WRITE(*,*)'***************************************'
      SUMPH=0
      SUMHD=0
      SUMCH=0
      AVNPS=0
      AVNPH=0
      AVNHD=0
      AVNCH=0
      DO I=1,10
       PPR(I)=IPPR(I)
       PHD(I)=IPHD(I)
       PCH(I)=IPCH(I)
       SUMPH=SUMPH+PPR(I)
       SUMHD=SUMHD+PHD(I)
       SUMCH=SUMCH+PCH(I)
      ENDDO
      WRITE(*,79)'Ecm =',RALFU(1),' GeV'
      WRITE(*,*)'MC multiplicity informations:'
      DO I=1,10 
       PPR(I)=PPR(I)/SUMPH
       PHD(I)=PHD(I)/SUMHD
       PCH(I)=PCH(I)/SUMCH
       AVNPH=AVNPH+I*PPR(I)
       AVNHD=AVNHD+I*PHD(I)
       AVNCH=AVNCH+I*PCH(I)
       WRITE(*,80)'Pph(',I,')=',PPR(I),
     &            'Pfh(',I,')=',PHD(I),'Pch(',I,')=',PCH(I)
      ENDDO
      WRITE(*,90)'<Npd>=',AVNPH,'<Nhd>=',AVNHD,'<Nch>=',AVNCH
      WRITE(*,*)'V =',XAU(30),'@ Ecm =',RALFU(1)
  50  ECM0=RALFU(1)
      CALL  POISSONDIS(ECM0,PN)
      WRITE(*,*)'Predicted MC multiplicity distribution:'
      WRITE(*,*)'ECM =',ECM0,' GeV'
      WRITE(*,91)'PN(2)=',PN(2)
      WRITE(*,91)'PN(3)=',PN(3)
      WRITE(*,91)'PN(4)=',PN(4)
      WRITE(*,91)'PN(5)=',PN(5)
      WRITE(*,91)'PN(6)=',PN(6)
    5 FORMAT(A4,I2,A3,F10.6)
   79 FORMAT(A6,F8.4,A6)  
   80 FORMAT(3(A4,I2,A2,F10.6,1X))
   90 FORMAT(3(A6,F8.4,2X))
   91 FORMAT(A8,F6.4)
      END

C...boost from CMS to LABS
      !***************************!
      SUBROUTINE LABTOCMS(PS,PI,PF)
      !***************************!
      REAL PF4,FN,PS(5),PI(5),PF(5)
      IF(PS(4).EQ.PS(5))THEN
        PF(1)=PI(1)
        PF(2)=PI(2)
        PF(3)=PI(3)
        PF(4)=PI(4)
      ELSE        
        PF4=(PI(4)*PS(4)-PI(3)*PS(3)
     &     -PI(2)*PS(2)-PI(1)*PS(1))/PS(5)
        FN=(PF4+PI(4))/(PS(4)+PS(5))
        PF(1)=PI(1)-FN*PS(1)
        PF(2)=PI(2)-FN*PS(2)
        PF(3)=PI(3)-FN*PS(3)
        PF(4)=PF4
      ENDIF
      PF(5)=PI(5)      
      END

C...boost from CMS to LABS
      !***************************!
      SUBROUTINE CMSTOLAB(PS,PI,PF)
      !***************************!
      REAL PF4,FN,PS(5),PI(5),PF(5)
      SMALL=1.0E-20
      D=ABS(PS(4)-PS(5))
      IF(D.LT.SMALL)THEN
       PF(1)=PI(1)
       PF(2)=PI(2)
       PF(3)=PI(3)
       PF(4)=PI(4)
      ELSE
       PF4=(PI(1)*PS(1)+PI(2)*PS(2)
     &    +PI(3)*PS(3)+PI(4)*PS(4))/PS(5)
       FN=(PF4+PI(4))/(PS(4)+PS(5))
       PF(1)=PI(1)+FN*PS(1)
       PF(2)=PI(2)+FN*PS(2)
       PF(3)=PI(3)+FN*PS(3)
       PF(4)=PF4
      ENDIF
      PF(5)=PI(5)
      END

C...Determine primary hadron number
      !********************************!
      SUBROUTINE ALBODY(KFENDL0,KFENDR0)
      !********************************!
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUJETS/,/LUDAT01/,/ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/
      
      IALPA(99)=0
      NCHRG=(LUCHGE0(KFENDL0)+LUCHGE0(KFENDR0))/3.0+0.0001

C...Determine the production channel 
    5 NTRY=1000
      ITRY=0
   10 KFENDL=KFENDL0
      KFENDR=KFENDR0
      ITRY=ITRY+1
      IF(ITRY.GT.NTRY)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      NLOOP=NA-1
    
C...When the channel is not specified
      IF(IALFU(8).LE.0)THEN
   20  NHADRON=0
       DO J=1,NLOOP
        CALL LUKFDI0(KFENDL,0,KFNEW,KFHAD)    !give out flavour of hadron
        IF(KFHAD.NE.0)NHADRON=NHADRON+1       !sum to known particle
        KA(J,2)=KFHAD                         !PDG code of hadron
        KFENDL=-KFNEW                         !new flavour of quark/antiquark
       ENDDO
       KFENDL=-KFNEW
       CALL LUKFDI0(KFENDL,KFENDR,KFDUM,KFHAD)!give out flavour of hadron
       KA(NA,2)=KFHAD
       IF(KFHAD.NE.0)NHADRON=NHADRON+1        !sum to known particle
       IF(ITRY.LE.NTRY)THEN
        IF(NHADRON.NE.NA)GOTO 10              !this event must be NA
       ELSE
        IALPA(99)=1
        RETURN
       ENDIF
      ELSE
      
C...When the channel is specified
       NA=0
       DO J=1,6
        IF(IALFU(10+J).NE.0)NA=NA+1
       ENDDO
       DO J=1,NA
        KA(J,2)=IALFU(10+J)
       ENDDO
      ENDIF
    
C...Check charge conservation
      NCH=0
      DO J=1,NA
       KF=KA(J,2)
       IF(KF.EQ.0)GOTO 10      !meet unknown hadron
       JCH=LUCHGE0(KF)/3        !charge
       NCH=NCH+JCH
      ENDDO
      IF(IALFU(8).GE.1.AND.NCH.NE.NCHRG)THEN
       CALL LUERRM0(1,'(ALBODY:) charge not conservation')  
       WRITE(*,*)'STOP !'
       STOP
      ELSEIF(NCH.NE.NCHRG)THEN
       GOTO 10
      ENDIF
     
C...Check flavour combination
      NETB=0
      NETS=0
      NETC=0
      DO J=1,NA
       KF=KA(J,2)
       CALL PROPTY(KF,JMB,JST,JCH)
       IMB=IABS(JMB)
       IF(IMB.EQ.3)NETB=NETB+JMB
       NETS=NETS+JST
       NETC=NETC+JCH
      ENDDO
      NETBSC=IABS(NETB)+IABS(NETS)+IABS(NETC) !total quantum number
      IF(KAU(8).EQ.0.AND.NETBSC.NE.0)GOTO 10
     
C...Check threshold
      SUM=RALPA(70)
      DO J=1,NA
       MSTJ(93)=1
       PA(J,5)=ULMASS0(KA(J,2)) !masses of hadrons
       SUM=SUM+PA(J,5)
      ENDDO
      IF(SUM.GT.XAU(2))THEN
       IF(ITRY.LT.NTRY)THEN
        GOTO 10
       ELSE
        IALPA(99)=1
        RETURN                !this event can't produce anyway
       ENDIF
      ENDIF

*...records of primary hadrons
      DO J=1,NA
       KA(J,1)=1
       KA(J,3)=KAU(3)         !means all NA hadrons come from string
       KA(J,4)=0              !means hadron don't decay temporarily
       KA(J,5)=0              !means hadron don't decay temporarily
       DO I=1,5
        VA(J,I)=0             !primary hadrons are produced at original
       ENDDO
      ENDDO
      RETURN
      END

*...Poisson distribution
      !***************************!
      SUBROUTINE POISSONDIS(ECM,PN)
      !***************************!
      REAL PN(10)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /ARLWDAT1/,/ARLWDAT3/,/LUDAT01/
      IALPA(99)=0
      S=ECM*ECM
      C0=RALPA(14)
      D=C0*C0
      SD=S/D
      ONE=1.0
      DO I=1,10
       PN(I)=0
      ENDDO
      XAU(30)=0
      V=0
      IF(KAU(8).GE.1.AND.KAU(20).NE.3)GOTO 20

C...V for continous state 
   10 CONTINUE
      IF(IALFU(9).EQ.1)THEN
       IF(ECM.LT.RALPA(10))THEN  
        C1=RALPA(15)
        C2=RALPA(16)
        C3=RALPA(17)
        V=RALPA(18)+RALPA(19)*EXP(RALPA(20)*SQRT(SD))
       ELSE
        C1=RALPA(21)
        C2=RALPA(22)
        C3=RALPA(23)
        V=RALPA(18)+RALPA(19)*EXP(RALPA(20)*SQRT(SD))
       ENDIF
      ELSEIF(IALFU(9).EQ.2)THEN
       RLN=LOG(SD)
       IF(ECM.LT.RALPA(10))THEN
        C1=RALPA(27)
        C2=RALPA(28)
        C3=RALPA(29)
        V=RALPA(30)+RALPA(31)*RLN+RALPA(32)*RLN*RLN
       ELSE
        C1=RALPA(33)
        C2=RALPA(34)
        C3=RALPA(35)
        V=RALPA(36)+RALPA(37)*RLN+RALPA(38)*RLN*RLN
       ENDIF
      ELSEIF(IALFU(9).EQ.3)THEN
       IF(ECM.LT.RALPA(10))THEN
        C1=RALPA(15)
        C2=RALPA(16)
        C3=RALPA(17)
        TMP=ALOG(S/RALPA(62)/RALPA(62))
        V=RALPA(18)+RALPA(19)*EXP(RALPA(20)*SQRT(TMP))
       ELSE
        WRITE(*,*)'NOT READY FOR Ecm =',ECM,'STOP !'
        !STOP
       ENDIF 
      ENDIF
      JPSI=0
      XAU(30)=V
      GOTO 30

C...J/psi decay in two or three gluons mode
   20 CONTINUE
      IF(IALFU(9).EQ.1)THEN
       C1=RALPA(39)
       C2=RALPA(40)
       C3=RALPA(41)
       V=RALPA(42)+RALPA(43)*EXP(RALPA(44)*SQRT(SD))
      ELSEIF(IALFU(9).EQ.2)THEN
       RLN=LOG(SD)
       C1=RALPA(45)
       C2=RALPA(46)
       C3=RALPA(47)
       V=RALPA(48)+RALPA(49)*RLN+RALPA(50)*RLN*RLN
      ENDIF
      IF(V.LE.0)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      JPSI=1
      XAU(30)=V

   30 SUM=0
      DO N=2,KAU(10)
       XN=N
       XM=ONE
       VN=ONE
       RF=ONE
       DO I=1,N
        XM=XM*I          !calculate n!
	VN=VN*V
        RF=RF*RALPA(66)  !RF^N
       ENDDO
       SUP=C1+C2*(XN-V)+C3*(XN-V)*(XN-V)
       IF(N.EQ.2.AND.JPSI.EQ.0)THEN
        F2=RALPA(67)
       ELSE
        F2=1.0
       ENDIF
       PN(N)=F2*RF*VN/XM*EXP(SUP)
       SUM=SUM+PN(N)
      ENDDO
      DO N=2,KAU(10)
       PN(N)=PN(N)/SUM
      ENDDO 
c      WRITE(*,*)'This event:'
c      WRITE(*,*)'Ecm0 =',RALFU(1), ' Ecm  = ',ECM,' V = ',V
      END

C...Non-concorrelated transverse momentum
      !**********************!
      SUBROUTINE ALNCPT(KX,KY)
      !**********************!
      REAL MS(10),MT(10),MTS(10),KX(10),KY(10)
      REAL KS,KSMAX,KX2,KX3,KX4,KX5,KY2,KY3,KY4,KY5
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUDAT01/,/ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/

C...constants
      ZRO=0.0
      ONE=1.0
      TWO=2.0
      THR=3.0
      FOU=4.0
      FIV=5.0
      ITRY1=0
      ITRY2=0
      ITRY3=0
      ITRY4=0
      ITRY5=0
      ITRY6=0
      NTRY=100000000
      TPI=6.82831852
      ECM=XAU(2)
      NBDY=NA
      XMJPSI=3.09687
      ECML=XMJPSI-5.0*RALPA(13)
      ECMH=XMJPSI+5.0*RALPA(13)
      IF(ECML.LE.ECM.AND.ECM.LE.ECMH)THEN
       IF(KAU(8).EQ.0)THEN
        SIGM=RALPA(68)
       ELSE
        SIGM=RALPA(69)
       ENDIF
      ELSE
       SIGM=RALPA(71)
      ENDIF
      RNB=NA
      IALPA(99)=0
      
C...Check if larger than threshold engery
      SUM=RALPA(70)
      DO J=1,NBDY
       MS(J)=PA(J,5)
       SUM=SUM+MS(J)
      ENDDO
      IF(ECM.LE.SUM)THEN
       CALL LUERRM0(1,'(ALNCPT:) too small c.m. energy')
       IALPA(99)=1
       RETURN
      ENDIF
     
C...To corresponding process
      IF(NBDY.EQ.2)GOTO 20
      IF(NBDY.EQ.3)GOTO 30
      IF(NBDY.EQ.4)GOTO 40
      IF(NBDY.EQ.5)GOTO 50
      IF(NBDY.EQ.6)GOTO 60

C...2 body process
   20 CONTINUE
      D1=SQRT((RNB-ONE)/RNB)*SIGM
C.....1st particle
      KSMAX=(ECM-MS(2))*(ECM-MS(2))-MS(1)*MS(1)
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   21 ITRY1=ITRY1+1
      IF(ITRY1.GE.NTRY)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX(1)=RK*COS(TPI*R2)*D1
      KY(1)=RK*SIN(TPI*R2)*D1
      KS=KX(1)*KX(1)+KY(1)*KY(1)
      MTS(1)=MS(1)*MS(1)+KS
      MT(1)=SQRT(MTS(1)) 
C.....2nd particle
      KX(2)=-KX(1)
      KY(2)=-KY(1)
      MTS(2)=MS(2)*MS(2)+KS
      MT(2)=SQRT(MTS(2))
C...theta condition
      SUMMT=RALPA(86)
      DO I=1,NBDY
       SUMMT=SUMMT+SQRT(MS(I)*MS(I)+KX(I)*KX(I)+KY(I)*KY(I))
      ENDDO
      IF(SUMMT.GE.ECM)GOTO 21
      RETURN

C...3 body process
   30 CONTINUE
      D1=SQRT((RNB-ONE)/RNB)*SIGM
      D2=SQRT((RNB-TWO)/(RNB-ONE))*SIGM
C.....1st particle
      SUMMS=MS(2)+MS(3)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(1)*MS(1)
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   31 ITRY1=ITRY1+1
      IF(ITRY1.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX(1)=RK*COS(TPI*R2)*D1
      KY(1)=RK*SIN(TPI*R2)*D1
      KS=KX(1)*KX(1)+KY(1)*KY(1)
      MTS(1)=MS(1)*MS(1)+KS
      MT(1)=SQRT(MTS(1))
C.....2nd particle
      SUMMS=MT(1)+MS(3)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(2)*MS(2)
      IF(KSMAX.LT.RALPA(85))GOTO 31
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   32 ITRY2=ITRY2+1
      IF(ITRY2.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX2=RK*COS(TPI*R2)
      kY2=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX2)*(KX(1)+KX2)
      ARG=ARG+(KY(1)+KY2)*(KY(1)+KY2)
      ARG=ARG/TWO/D2/D2
      F2=EXP(-ARG)
      IF(R3.GE.F2)GOTO 32
      KX(2)=KX2
      KY(2)=KY2
      KS=KX(2)*KX(2)+KY(2)*KY(2)
      MTS(2)=MS(2)*MS(2)+KS
      MT(2)=SQRT(MTS(2))
C...3th particle
      KX(3)=-KX(2)-KX(1)
      KY(3)=-KY(2)-KY(1)
      KS=KX(3)*KX(3)+KY(3)*KY(3)
      MTS(3)=MS(3)*MS(3)+KS
      MT(3)=SQRT(MTS(3))
C...theta condition
      SUMMT=RALPA(86)
      DO I=1,NBDY
       SUMMT=SUMMT+SQRT(MS(I)*MS(I)+KX(I)*KX(I)+KY(I)*KY(I))
      ENDDO
      IF(SUMMT.GE.ECM)GOTO 31
      RETURN

C...4 body process
   40 CONTINUE
      D1=SQRT((RNB-ONE)/RNB)*SIGM
      D2=SQRT((RNB-TWO)/(RNB-ONE))*SIGM
      D3=SQRT((RNB-THR)/(RNB-TWO))*SIGM
C...1st particle
      SUMMS=MS(2)+MS(3)+MS(4)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(1)*MS(1)
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   41 ITRY1=ITRY1+1
      IF(ITRY1.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX(1)=RK*COS(TPI*R2)*D1
      KY(1)=RK*SIN(TPI*R2)*D1
      KS=KX(1)*KX(1)+KY(1)*KY(1)
      MTS(1)=MS(1)*MS(1)+KS
      MT(1)=SQRT(MTS(1))
C.....2nd particle
      SUMMS=MT(1)+MS(3)+MS(4)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(2)*MS(2)
      IF(KSMAX.LE.RALPA(84))GOTO 41
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   42 ITRY2=ITRY2+1
      IF(ITRY2.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX2=RK*COS(TPI*R2) 
      kY2=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX2)*(KX(1)+KX2)
      ARG=ARG+(KY(1)+KY2)*(KY(1)+KY2)
      ARG=ARG/TWO/D2/D2
      F2=EXP(-ARG)
      IF(R3.GE.F2)GOTO 42
      KX(2)=KX2
      KY(2)=KY2
      KS=KX2*KX2+KY2*KY2
      MTS(2)=MS(2)*MS(2)+KS
      MT(2)=SQRT(MTS(2))
C...3th particle
      SUMMS=MT(1)+MT(2)+MS(4)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(3)*MS(3)
      IF(KSMAX.LE.RALPA(85))GOTO 42
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   43 ITRY3=ITRY3+1
      IF(ITRY3.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX3=RK*COS(TPI*R2)
      KY3=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX(2)+KX3)*(KX(1)+KX(2)+KX3)
      ARG=ARG+(KY(1)+KY(2)+KY3)*(KY(1)+KY(2)+KY3)
      ARG=ARG/TWO/D3/D3
      F3=EXP(-ARG)
      IF(R3.GE.F3)GOTO 43
      KX(3)=KX3
      KY(3)=KY3
      KS=KX3*KX3+KY3*KY3
      MTS(3)=MS(3)*MS(3)+KS
      MT(3)=SQRT(MTS(3))
C...4th particle
      KX(4)=-KX(1)-KX(2)-KX(3)
      KY(4)=-KY(1)-KY(2)-KY(3)
      KS=KX(4)*KX(4)+KY(4)*KY(4)
      MTS(4)=MS(4)*MS(4)+KS
      MT(4)=SQRT(MTS(4))
C...theta condition
      SUMMT=RALPA(86)
      DO I=1,NBDY
       SUMMT=SUMMT+SQRT(MS(I)*MS(I)+KX(I)*KX(I)+KY(I)*KY(I))
      ENDDO
      IF(SUMMT.GE.ECM)GOTO 41
      RETURN

C...5 body
   50 CONTINUE
      D1=SQRT((RNB-ONE)/RNB)*SIGM
      D2=SQRT((RNB-TWO)/(RNB-ONE))*SIGM
      D3=SQRT((RNB-THR)/(RNB-TWO))*SIGM
      D4=SQRT((RNB-FOU)/(RNB-THR))*SIGM
C...1st particle
      SUMMS=MS(2)+MS(3)+MS(4)+MS(5)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(1)*MS(1)
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   51 ITRY1=ITRY1+1
      IF(ITRY1.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX(1)=RK*COS(TPI*R2)*D1
      KY(1)=RK*SIN(TPI*R2)*D1
      KS=KX(1)*KX(1)+KY(1)*KY(1)
      MTS(1)=MS(1)*MS(1)+KS
      MT(1)=SQRT(MTS(1))
C.....2nd particle
      SUMMS=MT(1)+MS(3)+MS(4)+MS(5)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(2)*MS(2)
      IF(KSMAX.LE.RALPA(83))GOTO 51
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   52 ITRY2=ITRY2+1
      IF(ITRY2.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX2=RK*COS(TPI*R2)
      kY2=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX2)*(KX(1)+KX2)
      ARG=ARG+(KY(1)+KY2)*(KY(1)+KY2)
      ARG=ARG/TWO/D2/D2
      F2=EXP(-ARG)
      IF(R3.GE.F2)GOTO 51
      KX(2)=KX2
      KY(2)=KY2
      KS=KX2*KX2+KY2*KY2
      MTS(2)=MS(2)*MS(2)+KS
      MT(2)=SQRT(MTS(2))
C.....3th particle
      SUMMS=MT(1)+MT(2)+MS(4)+MS(5)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(3)*MS(3)
      IF(KSMAX.LE.RALPA(84))GOTO 51
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   53 ITRY3=ITRY3+1
      IF(ITRY3.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX3=RK*COS(TPI*R2)
      KY3=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX(2)+KX3)*(KX(1)+KX(2)+KX3)
      ARG=ARG+(KY(1)+KY(2)+KY3)*(KY(1)+KY(2)+KY3)
      ARG=ARG/TWO/D3/D3
      F5=EXP(-ARG)
      IF(R3.GE.F5)GOTO 51
      KX(3)=KX3
      KY(3)=KY3
      KS=KX3*KX3+KY3*KY3
      MTS(3)=MS(3)*MS(3)+KS
      MT(3)=SQRT(MTS(3))
C.....4th particle
      SUMMS=MT(1)+MT(2)+MT(3)+MS(5)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(4)*MS(4)
      IF(KSMAX.LE.RALPA(85))GOTO 51
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   54 ITRY4=ITRY4+1
      IF(ITRY4.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX4=RK*COS(TPI*R2)
      KY4=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX(2)+KX(3)+KX4)*(KX(1)+KX(2)+KX(3)+KX4)
      ARG=ARG+(KY(1)+KY(2)+KY(3)+KY4)*(KY(1)+KY(2)+KY(3)+KY4)
      ARG=ARG/TWO/D4/D4
      F5=EXP(-ARG)
      IF(R3.GE.F5)GOTO 51
      KX(4)=KX4
      KY(4)=KY4
      KS=KX4*KX4+KY4*KY4
      MTS(4)=MS(4)*MS(4)+KS
      MT(4)=SQRT(MTS(4))
C.....5th particle
      KX(5)=-KX(1)-KX(2)-KX(3)-KX(4)
      KY(5)=-KY(1)-KY(2)-KY(3)-KY(4)
      KS=KX(5)*KX(5)+KY(5)*KY(5)
      MTS(5)=MS(5)*MS(5)+KS
      MT(5)=SQRT(MTS(5))
C...theta condition
      SUMMT=RALPA(86)
      DO I=1,NBDY
       SUMMT=SUMMT+SQRT(MS(I)*MS(I)+KX(I)*KX(I)+KY(I)*KY(I))
      ENDDO
      IF(SUMMT.GE.ECM)GOTO 51
      RETURN

C...6 body
   60 CONTINUE
      D1=SQRT((RNB-ONE)/RNB)*SIGM
      D2=SQRT((RNB-TWO)/(RNB-ONE))*SIGM
      D3=SQRT((RNB-THR)/(RNB-TWO))*SIGM
      D4=SQRT((RNB-FOU)/(RNB-THR))*SIGM
      D5=SQRT((RNB-FIV)/(RNB-FOU))*SIGM
C...1st particle
      SUMMS=MS(2)+MS(3)+MS(4)+MS(5)+MS(6)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(1)*MS(1)
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   61 ITRY1=ITRY1+1
      IF(ITRY1.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX(1)=RK*COS(TPI*R2)*D1
      KY(1)=RK*SIN(TPI*R2)*D1
      KS=KX(1)*KX(1)+KY(1)*KY(1)
      MTS(1)=MS(1)*MS(1)+KS
      MT(1)=SQRT(MTS(1))
C.....2nd particle
      SUMMS=MT(1)+MS(3)+MS(4)+MS(5)+MS(6)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(2)*MS(2)
      IF(KSMAX.LE.RALPA(82))GOTO 61
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   62 ITRY2=ITRY2+1
      IF(ITRY2.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX2=RK*COS(TPI*R2)
      kY2=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX2)*(KX(1)+KX2)
      ARG=ARG+(KY(1)+KY2)*(KY(1)+KY2)
      ARG=ARG/TWO/D2/D2
      F2=EXP(-ARG)
      IF(R3.GE.F2)GOTO 61
      KX(2)=KX2
      KY(2)=KY2
      KS=KX2*KX2+KY2*KY2
      MTS(2)=MS(2)*MS(2)+KS
      MT(2)=SQRT(MTS(2))
C.....3th particle
      SUMMS=MT(1)+MT(2)+MS(4)+MS(5)+MS(6)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(3)*MS(3)
      IF(KSMAX.LE.RALPA(83))GOTO 61
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   63 ITRY3=ITRY3+1
      IF(ITRY3.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX3=RK*COS(TPI*R2)
      KY3=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX(2)+KX3)*(KX(1)+KX(2)+KX3)
      ARG=ARG+(KY(1)+KY(2)+KY3)*(KY(1)+KY(2)+KY3)
      ARG=ARG/TWO/D3/D3
      F5=EXP(-ARG)
      IF(R3.GE.F5)GOTO 61
      KX(3)=KX3
      KY(3)=KY3
      KS=KX3*KX3+KY3*KY3
      MTS(3)=MS(3)*MS(3)+KS
      MT(3)=SQRT(MTS(3))
C.....4th particle
      SUMMS=MT(1)+MT(2)+MT(3)+MS(5)+MS(6)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(4)*MS(4)
      IF(KSMAX.LE.RALPA(84))GOTO 61
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   64 ITRY4=ITRY4+1
      IF(ITRY4.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX4=RK*COS(TPI*R2)
      KY4=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX(2)+KX(3)+KX4)*(KX(1)+KX(2)+KX(3)+KX4)
      ARG=ARG+(KY(1)+KY(2)+KY(3)+KY4)*(KY(1)+KY(2)+KY(3)+KY4)
      ARG=ARG/TWO/D4/D4
      F4=EXP(-ARG)
      IF(R3.GE.F4)GOTO 61
      KX(4)=KX4
      KY(4)=KY4
      KS=KX4*KX4+KY4*KY4
      MTS(4)=MS(4)*MS(4)+KS
      MT(4)=SQRT(MTS(4))
C.....5th particle
      SUMMS=MT(1)+MT(2)+MT(3)+MT(4)+MS(6)
      KSMAX=(ECM-SUMMS)*(ECM-SUMMS)-MS(5)*MS(5)
      IF(KSMAX.LE.RALPA(85))GOTO 61
      R1MIN=EXP(-KSMAX/TWO)
      R1MAX=ONE
   65 ITRY5=ITRY5+1
      IF(ITRY5.GE.NTRY)THEN 
       IALPA(99)=1
       RETURN 
      ENDIF
      R1=R1MIN+(R1MAX-R1MIN)*RLU0(0)
      R2=RLU0(0)
      R3=RLU0(0)
      RK=SQRT(-TWO*LOG(R1))
      KX5=RK*COS(TPI*R2)
      KY5=RK*SIN(TPI*R2)
      ARG=(KX(1)+KX(2)+KX(3)+KX(4)+KX5)
     &   *(KX(1)+KX(2)+KX(3)+KX(4)+KX5)
      ARG=ARG+(KY(1)+KY(2)+KY(3)+KY(4)+KY5)
     &       *(KY(1)+KY(2)+KY(3)+KY(4)+KY5)
      ARG=ARG/TWO/D5/D5
      F5=EXP(-ARG)
      IF(R3.GE.F5)GOTO 61
      KX(5)=KX5
      KY(5)=KY5
      KS=KX5*KX5+KY5*KY5
      MTS(5)=MS(5)*MS(5)+KS
      MT(5)=SQRT(MTS(5))
C.....6th particle
      KX(6)=-KX(1)-KX(2)-KX(3)-KX(4)-KX(5)
      KY(6)=-KY(1)-KY(2)-KY(3)-KY(4)-KY(5)
      KS=KX(6)*KX(6)+KY(6)*KY(6)
      MTS(6)=MS(6)*MS(6)+KS
      MT(6)=SQRT(MTS(6))

C...theta condition
      SUMMT=RALPA(86)
      DO I=1,NBDY
       SUMMT=SUMMT+SQRT(MS(I)*MS(I)+KX(I)*KX(I)+KY(I)*KY(I))
      ENDDO
      IF(SUMMT.GE.ECM)GOTO 61
      RETURN
      END

*...Correlated transverse momentum
      !**********************!
      SUBROUTINE ALCOPT(KX,KY)
      !**********************!
      REAL MS(10),MT(10),MTS(10),V(10,10),C(10,10)
      REAL RX(10),RY(10),QX(10),QY(10),KX(10),KY(10)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT2/NA,KA(10,5),PA(10,5),VA(10,5)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/VC/V,C
      SAVE /LUDAT01/,/ARLWDAT1/,/ARLWDAT2/,/ARLWDAT3/,/VC/
      ZRO=0.0
      TWO=2.0
      TPI=6.2831852
      ECM=XAU(2)
      NBDY=NA
      ITRY=0
      NTRY=1000
      IALPA(99)=0
      DO I=1,10
       KX(I)=ZRO
       KY(I)=ZRO
      ENDDO

C...Check if larger than threshold engery
      SUM=RALPA(81)
      DO J=1,NBDY
       MSTJ(93)=1
       MSTJ24=MSTJ(24)
       MSTJ(24)=0
       MS(J)=ULMASS0(KA(J,2))
       MSTJ(24)=MSTJ24
       SUM=SUM+MS(J)
      ENDDO
      IF(ECM.LE.SUM)THEN
       DO J=1,NBDY 
        WRITE(*,*)J,KA(J,2),MS(J)
       ENDDO 
       WRITE(*,*)'Sum_Ms, Ecm=',SUM,ECM
       CALL LUERRM0(1,'(ALCOPT:) too small c.m. energy')
       IALPA(99)=1
       RETURN
      ENDIF

C...Independent standard normal random numbers
   10 ITRY=ITRY+1
      IF(ITRY.GE.NTRY)THEN
       IALPA(99)=1
       RETURN
      ENDIF
      DO I=1,NBDY
       R1=RLU0(1)
       R2=TPI*RLU0(1)
       T=SQRT(-TWO*LOG(R1))
       RX(I)=T*COS(R2)
       RY(I)=T*SIN(R2)
      ENDDO

C...Dependent normal random numbers
      IF(NBDY.EQ.2)THEN
       QX(1)=C(1,1)*RX(1)
       QX(2)=C(2,1)*RX(1)+C(2,2)*RX(2)
       QY(1)=C(1,1)*RY(1)
       QY(2)=C(2,1)*RY(1)+C(2,2)*RY(2)
      ELSEIF(NBDY.EQ.3)THEN
       QX(1)=C(1,1)*RX(1)
       QX(2)=C(2,1)*RX(1)+C(2,2)*RX(2)
       QX(3)=C(3,1)*RX(1)+C(3,2)*RX(2)+C(3,3)*RX(3)
       QY(1)=C(1,1)*RY(1)
       QY(2)=C(2,1)*RY(1)+C(2,2)*RY(2)
       QY(3)=C(3,1)*RY(1)+C(3,2)*RY(2)+C(3,3)*RY(3)
      ELSEIF(NBDY.EQ.4)THEN
       QX(1)=C(1,1)*RX(1)
       QX(2)=C(2,1)*RX(1)+C(2,2)*RX(2)
       QX(3)=C(3,1)*RX(1)+C(3,2)*RX(2)+C(3,3)*RX(3)
       QX(4)=C(4,1)*RX(1)+C(4,2)*RX(2)+C(4,3)*RX(3)
     &      +C(4,4)*RX(4)
       QY(1)=C(1,1)*RY(1)
       QY(2)=C(2,1)*RY(1)+C(2,2)*RY(2)
       QY(3)=C(3,1)*RY(1)+C(3,2)*RY(2)+C(3,3)*RY(3)
       QY(4)=C(4,1)*RY(1)+C(4,2)*RY(2)+C(4,3)*RY(3)
     &      +C(4,4)*RY(4)
      ELSEIF(NBDY.EQ.5)THEN
       QX(1)=C(1,1)*RX(1)
       QX(2)=C(2,1)*RX(1)+C(2,2)*RX(2)
       QX(3)=C(3,1)*RX(1)+C(3,2)*RX(2)+C(3,3)*RX(3)
       QX(4)=C(4,1)*RX(1)+C(4,2)*RX(2)+C(4,3)*RX(3)
     &      +C(4,4)*RX(4)
       QX(5)=C(5,1)*RX(1)+C(5,2)*RX(2)+C(5,3)*RX(3)
     &      +C(5,4)*RX(4)+C(5,5)*RX(5)
       QY(1)=C(1,1)*RY(1)
       QY(2)=C(2,1)*RY(1)+C(2,2)*RY(2)
       QY(3)=C(3,1)*RY(1)+C(3,2)*RY(2)+C(3,3)*RY(3)
       QY(4)=C(4,1)*RY(1)+C(4,2)*RY(2)+C(4,3)*RY(3)
     &      +C(4,4)*RY(4)
       QY(5)=C(5,1)*RY(1)+C(5,2)*RY(2)+C(5,3)*RY(3)
     &      +C(5,4)*RY(4)+C(5,5)*RY(5)
      ELSEIF(NBDY.EQ.6)THEN
       QX(1)=C(1,1)*RX(1)
       QX(2)=C(2,1)*RX(1)+C(2,2)*RX(2)
       QX(3)=C(3,1)*RX(1)+C(3,2)*RX(2)+C(3,3)*RX(3)
       QX(4)=C(4,1)*RX(1)+C(4,2)*RX(2)+C(4,3)*RX(3)
     &      +C(4,4)*RX(4)
       QX(5)=C(5,1)*RX(1)+C(5,2)*RX(2)+C(5,3)*RX(3)
     &      +C(5,4)*RX(4)+C(5,5)*RX(5)
       QX(6)=C(6,1)*RX(1)+C(6,2)*RX(2)+C(6,3)*RX(3)
     &      +C(6,4)*RX(4)+C(6,5)*RX(5)+C(6,6)*RX(6)
       QY(1)=C(1,1)*RY(1)
       QY(2)=C(2,1)*RY(1)+C(2,2)*RY(2)
       QY(3)=C(3,1)*RY(1)+C(3,2)*RY(2)+C(3,3)*RY(3)
       QY(4)=C(4,1)*RY(1)+C(4,2)*RY(2)+C(4,3)*RY(3)
     &      +C(4,4)*RY(4)
       QY(5)=C(5,1)*RY(1)+C(5,2)*RY(2)+C(5,3)*RY(3)
     &      +C(5,4)*RY(4)+C(5,5)*RY(5)
       QY(6)=C(6,1)*RY(1)+C(6,2)*RY(2)+C(6,3)*RY(3)
     &      +C(6,4)*RY(4)+C(6,5)*RY(5)+C(6,6)*RY(6)
      ENDIF

C...Get Kx and Ky by Qx and Qy
      IF(NBDY.EQ.2)THEN
       KX(1)=QX(1)
       KY(1)=QY(1)
       KX(2)=-QX(1)
       KY(2)=-QY(1)
      ELSEIF(NBDY.EQ.3)THEN
       KX(1)=QX(1)
       KY(1)=QY(1)
       KX(2)=QX(2)-QX(1)
       KY(2)=QY(2)-QY(1)
       KX(3)=-QX(2)
       KY(3)=-QY(2)
      ELSEIF(NBDY.EQ.4)THEN
       KX(1)=QX(1)
       KY(1)=QY(1)
       KX(2)=QX(2)-QX(1)
       KY(2)=QY(2)-QY(1)
       KX(3)=QX(3)-QX(2)
       KY(3)=QY(3)-QY(2)
       KX(4)=-QX(3)
       KY(4)=-QY(3)
      ELSEIF(NBDY.EQ.5)THEN
       KX(1)=QX(1)
       KY(1)=QY(1)
       KX(2)=QX(2)-QX(1)
       KY(2)=QY(2)-QY(1)
       KX(3)=QX(3)-QX(2)
       KY(3)=QY(3)-QY(2)
       KX(4)=QX(4)-QX(3)
       KY(4)=QY(4)-QY(3)
       KX(5)=-QX(4)
       KY(5)=-QY(4)
      ELSEIF(NBDY.EQ.6)THEN
       KX(1)=QX(1)
       KY(1)=QY(1)
       KX(2)=QX(2)-QX(1)
       KY(2)=QY(2)-QY(1)
       KX(3)=QX(3)-QX(2)
       KY(3)=QY(3)-QY(2)
       KX(4)=QX(4)-QX(3)
       KY(4)=QY(4)-QY(3)
       KX(5)=QX(5)-QX(4)
       KY(5)=QY(5)-QY(4)
       KX(6)=-QX(5)
       KY(6)=-QY(5)
      ENDIF

C...Judge energy-monmentum condition
      SUM=RALPA(86)
      DO I=1,NBDY
       MTS(I)=MS(I)*MS(I)+KX(I)*KX(I)+KY(I)*KY(I)
       MT(I)=SQRT(MTS(I))
       SUM=SUMMT+MT(I)
      ENDDO
      IF(SUM.GE.ECM)GOTO 10
      RETURN
      END

C...Set paramters for Pt
      !*******************!
      SUBROUTINE INIPTPARMT
      !*******************!
      REAL RHO(10),V(10,10),C(10,10)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/VC/V,C
      SAVE /ARLWDAT1/
      ZRO=0.0
      TWO=2.0
      SIGM=RALPA(71)
      RHO0=RALPA(72)
      V0=SIGM*SIGM/TWO
      DO I=1,10
       RHO(I)=ZRO
      ENDDO

*...variance matrix Vij
      V(1,1)=V0
      V(2,1)=V0*RHO(2)
      V(1,2)=V(2,1)
      V(2,2)=V0
      V(3,1)=V0*RHO(2)*RHO(3)
      V(1,3)=V(3,1)
      V(3,2)=V0*RHO(3)
      V(2,3)=V(3,2)
      V(3,3)=V0
      V(4,1)=V0*RHO(2)*RHO(3)*RHO(4)
      V(1,4)=V(4,1)
      V(4,2)=V0*RHO(3)*RHO(4)
      V(2,4)=V(4,2)
      V(4,3)=V0*RHO(4)
      V(3,4)=V(4,3)
      V(4,4)=V0
      V(5,1)=V0*RHO(2)*RHO(3)*RHO(4)*RHO(5)
      V(1,5)=V(5,1)
      V(5,2)=V0*RHO(3)*RHO(4)*RHO(5)
      V(2,5)=V(5,2)
      V(5,3)=V0*RHO(4)*RHO(5)
      V(3,5)=V(5,3)
      V(5,4)=V0*RHO(5)
      V(4,5)=V(5,4)
      V(5,5)=V0
      V(6,1)=V0*RHO(2)*RHO(3)*RHO(4)*RHO(5)*RHO(6)
      V(1,6)=V(6,1)
      V(6,2)=V0*RHO(3)*RHO(4)*RHO(5)*RHO(6)
      V(2,6)=V(6,2)
      V(6,3)=V0*RHO(4)*RHO(5)*RHO(6)
      V(3,6)=V(6,3)
      V(6,4)=V0*RHO(5)*RHO(6)
      V(4,6)=V(6,4)
      V(6,5)=V0*RHO(6)
      V(5,6)=V(6,5)
      V(6,6)=V0

C...lower triangle matrix Cij
      DO I=1,10
       DO J=1,10
        C(I,J)=ZRO
       ENDDO
      ENDDO      
      C(1,1)=SQRT(V(1,1))
      C(2,1)=V(2,1)/C(1,1)
      C(2,2)=SQRT(V(2,2)-C(2,1)*C(2,1))
      C(3,1)=V(3,1)/C(1,1)
      C(3,2)=(V(3,2)-C(3,1)*C(2,1))/C(2,2)
      C(3,3)=SQRT(V(3,3)-C(3,1)*C(3,1)-C(3,2)*C(3,2))
      C(4,1)=V(4,1)/C(1,1)
      C(4,2)=(V(4,2)-C(4,1)*C(2,1))/C(2,2)
      C(4,3)=(V(4,3)-C(4,1)*C(3,1)-C(4,2)*C(3,2))/C(3,3)
      C(4,4)=SQRT(V(4,4)-C(4,1)*C(4,1)-C(4,2)*C(4,2)
     &      -C(4,3)*C(4,3))
      C(5,1)=V(5,1)/C(1,1)
      C(5,2)=(V(5,2)-C(5,1)*C(2,1))/C(2,2)
      C(5,3)=(V(5,3)-C(5,1)*C(3,1)-C(5,2)*C(3,2))/C(3,3)
      C(5,4)=(V(5,4)-C(5,1)*C(4,1)-C(5,2)*C(4,2)
     &      -C(5,3)*C(4,3))/C(4,4)
      C(5,5)=SQRT(V(5,5)-C(5,1)*C(5,1)-C(5,2)*C(5,2)
     &      -C(5,3)*C(5,3)-C(5,4)*C(5,4))
      C(6,1)=V(6,1)/C(1,1)
      C(6,2)=(V(6,2)-C(6,1)*C(2,1))/C(2,2)
      C(6,3)=(V(6,3)-C(6,1)*C(3,1)-C(6,2)*C(3,2))/C(3,3)
      C(6,4)=(V(6,4)-C(6,1)*C(4,1)-C(6,2)*C(4,2)
     &      -C(6,3)*C(4,3))/C(4,4)
      C(6,5)=(V(6,5)-C(6,1)*C(5,1)-C(6,2)*C(5,2)
     &      -C(6,3)*C(5,3)-C(6,4)*C(5,4))/C(5,5)
      C(6,6)=SQRT(V(6,6)-C(6,1)*C(6,1)-C(6,2)*C(6,2)
     &      -C(6,3)*C(6,3)-C(6,4)*C(6,4)-C(6,5)*C(6,5))
      RETURN 
      END
 
C...to select flavour for produced qqbar pair in area law
      !***********************************!
      SUBROUTINE LUQKFL1(KFL,ECM,ECMC,KFLC)
      !***********************************!
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /LUDAT01/,/LUDAT02/,/ARLWDAT1/

      RALPA(99)=0
      RFMAX=4.0/9.0	

C...Choose flavour. Gives charge and velocity.
      NTRY=0		
  100 NTRY=NTRY+1	
      IF(NTRY.GT.1000) THEN
        !CALL LUERRM0(1,'(LUQKFL:) caught in an infinite loop')
        KFLC=0		
        RETURN		
      ENDIF		
      KFLC=KFL		
      IF(KFL.LE.0) KFLC=1+INT(MSTJ(104)*RLU0(0))
      MSTJ(93)=1
      PMQ=ULMASS0(KFLC)
      JDEX=86+KFLC 
      IF(1.LE.KFLC.AND.KFLC.LE.4)PARJ(127)=RALPA(JDEX)
      IF(ECMC.LT.2.*PMQ+PARJ(127))GOTO 100
      QF=KCHG(KFLC,1)/3.0
      VQ=1.0		
      IF(MOD(MSTJ(103),2).EQ.1) VQ=SQRT(MAX(0.,1.-(2.*PMQ/ECMC)**2))

C...Calculate weight in QED or QFD case.
      RF=QF**2	
      RFV=0.5*VQ*(3.-VQ**2)*QF**2

C...Weighting or new event (radiative photon). Cross-section update.
      IF(KFL.LE.0.AND.RF.LT.RLU0(0)*RFMAX) GOTO 100
      PARJ(158)=PARJ(158)+1.
      IF(ECMC.LT.2.*PMQ+PARJ(127).OR.RFV.LT.RLU0(0)*RF) KFLC=0
      IF(MSTJ(107).LE.0.AND.KFLC.EQ.0) GOTO 100
      IF(KFLC.NE.0) PARJ(159)=PARJ(159)+1.
      PARJ(144)=PARJ(157)*PARJ(159)/PARJ(158)
      PARJ(148)=PARJ(144)*86.8/ECM**2
      PARJ(127)=PARJ127
      RETURN		
      END

C...Select flavour for gamma -> quark and antiquark pair 
      !***********************************!
      SUBROUTINE LUQKFL2(KFL,ECM,ECMC,KFLC)
      !***********************************!
      REAL WGHT(4),F(0:4)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      SAVE /LUDAT01/,/LUDAT02/,/ARLWDAT1/,/ARLWDAT3/
      
      IF(KFL.GT.0)THEN
       KFLC=KFL
       RETURN
      ENDIF
      KFMAX=4
      KFLC=0
      IF(XAU(2).LE.RALPA(10))KFMAX=3

C...weight for gamma -> q and qbar
      S=ECMC*ECMC
      SUM=0
      DO J=1,KFMAX     !for uubar, ddbar, ssbar, ccbar
       E=LUCHGE0(J)
       X=ULMASS0(J)  
       U=1-4*X*X/S
       IF(U.GT.0)THEN
        B=SQRT(1-4*X*X/S)
        WGHT(J)=E*E/S*B*(3-B*B)
       ELSE
        WGHT(J)=0
       ENDIF
       SUM=SUM+WGHT(J)
      ENDDO 
      DO J=1,KFMAX
       WGHT(J)=WGHT(J)/SUM
      ENDDO
      F(0)=0
      DO J=1,KFMAX
       F(J)=F(J-1)+WGHT(J)
      ENDDO
     
C...Flavours of the gluons spliting into quark and antiquark
  300 R=RLU0(0)
      DO J=1,KFMAX
       IF(F(J-1).LT.R.AND.R.LE.F(J))KFLC=J
      ENDDO
      IF(KFLC.LE.0)THEN
       CALL LUERRM0(100,'(LUQKFL2:) wrong weight for gamma -> q qbar') 
      ENDIF
      END

C...J/psi cross section in R value
      !***************!
      FUNCTION RJPSI(S)
      !***************!
      REAL M
      A=0.007297353
      M=3.09687
      WE=0.00000526
      WT=0.000088
      WH=WT-2.0*WE
      ECM=SQRT(S)
      UP=WE*WH
      DW=(S-M*M)*(S-M*M)+M*M*WT*WT
      RJPSI=9.0*S/A/A*UP/DW
      RETURN
      END

*...R valuse contributed fro rho
      !**************!
      FUNCTION RRHO(S)
      !**************!
      REAL MRHO
      COMMON/RHO/WERHO,WHRHO,WTRHO,MRHO
      A=0.007297353
      UP=WERHO*WTRHO
      DW=(S-MRHO*MRHO)*(S-MRHO*MRHO)+MRHO*MRHO*WTRHO*WTRHO
      RRHO=9.0*S/A/A*UP/DW
      RETURN
      END 

*...psi(4030),psi(4160),psi(4415) contribution to R
      !**************!
      FUNCTION RDDR(S)
      !**************!
      REAL WEE(20),WHD(20),WTOT(20),MRES(20)
      COMMON/DATASET/WEE,WHD,WTOT,MRES
      A=0.0072973531
      SUM=0
      DO J=10,13
       UP=WEE(J)*WTOT(J)
       DW=(S-MRES(J)*MRES(J))*(S-MRES(J)*MRES(J))
     &   +MRES(J)*MRES(J)*WTOT(J)*WTOT(J)
       SUM=SUM+UP/DW
      ENDDO
      RDDR=9.0*S/A/A*SUM
	RETURN
      END

      !***************!
      FUNCTION SDRDS(S)
      !***************!
      INTEGER N,NF,NC,NFQ(6)
      REAL PI,ECM,S,LAM,AS,MPI,QF(10),MF(10),MFI,ASPI,ONE
      DATA (QF(I),I=1,6)/-1.0,+2.0,-1.0,+2.0,-1.0,+2.0/
      DATA (MF(I),I=1,6)/0.32,0.32,0.550,1.80,5.20,174.0/
      PI=3.14159265
      A=0.007297353
      ECM=SQRT(S)
      E=0.5*ECM
      MPI=0.13956995
      ECMTH=2.0*MPI
      LAM=0.30
      MPI=0.13956995
      ONE=1.0
      HALF=0.5
      ONEONE=1.1
      ONSIX=1.6
      NC=3
      N=6
      RQCD=0.0
      SDRDS=0.0
      IF(ECM.LT.ECMTH)THEN
       SDRDS=0.0
      ELSEIF(ECM.GT.ECMTH.AND.ECM.LT.HALF)THEN
       SDRDS=1.25
      ELSEIF(ECM.GT.HALF.AND.ECM.LT.ONONE)THEN
       RQCD=0.4
      ELSEIF(ECM.GT.ONENE.AND.ECM.LT.ONSIX)THEN
       RQCD=3.0
      ELSEIF(ECM.GE.ONSIX)THEN
       DO I=1,N
        NFQ(I)=INT(E/MF(I))
       ENDDO
       DO I=N,1,-1  
        IF(NFQ(I).GE.1)THEN
         NF=I
         GOTO 10    
        ENDIF
       ENDDO
   10  CONTINUE
       AS=12.0*PI/(33.0-2.0*NF)/ALOG(S/LAM/LAM)
       ASPI=AS/PI   
       SUMQ=0.0
       Q2SUM=0.0    
       DO I=1,NF    
        QFI=QF(I)/3.0
        MFI=MF(I)   
        STH=4.0*MFI*MFI
        IF(S.GT.STH)THEN
         Q2SUM=Q2SUM+QFI*QFI
         SUMQ=SUMQ+QFI
        ENDIF
       ENDDO
       SUMQ2=SUMQ*SUMQ
       RF=0.0
       DO I=1,NF    
        QFI=QF(I)/3.0
        MFI=MF(I)   
        STH=4.0*MFI*MFI
        IF(S.GT.STH)THEN
         BI=SQRT(1.0-STH/S)
         F0=BI*(1.0+0.5*STH/S)
         RF=RF+QFI*QFI*F0
        ENDIF
       ENDDO
       R1=1.9857-0.1153*NF
       R2=-6.6368-1.2001*NF-1.2395*SUMQ2/Q2SUM/3.0
       PASPI=1.0+R1*ASPI+R2*ASPI**2 
       ALOG2=ALOG(S/LAM/LAM)*ALOG(S/LAM/LAM)
       SDRDS=-12.0*NC*RF*PASPI/(33.0-2.0*NF)/ALOG2
      ENDIF
      RETURN
      END

      !*******************************!
      SUBROUTINE PROPTY(KF,JMB,JST,JCH)
      !*******************************!
      INTEGER F
      F=IABS(KF)
      JLP=0
      JMB=0
      JST=0
      JCH=0
      IF(F.LE.100)RETURN

*...judge meson or baryon      
   10 IF((100.LT.F.AND.F.LT.1000).OR.F.GE.10000)THEN
       IF(KF.GT.0)JMB=+2  !meson
       IF(KF.LT.0)JMB=-2  !anti-meson
      ELSEIF(1000.LT.F.AND.F.LT.10000)THEN
       IF(KF.GT.0)JMB=+3  !baryon
       IF(KF.LT.0)JMB=-3  !anti-baryon
      ENDIF 
      IF(JLP.EQ.0.AND.JMB.EQ.0)THEN
       WRITE(*,*)'meet unknow particle. Stop!'
       WRITE(*,*)'check, and run again'
       STOP
      ENDIF
      IJMB=IABS(JMB)
    
*...judge if strange particle
   20 IF(IJMB.EQ.2)THEN
       IF(F.EQ.311.OR.F.EQ.321.OR.F.EQ.313.OR.F.EQ.323)THEN
        IF(KF.LT.0)JST=-1 !K~0 K- K*0~ K*-
        IF(KF.GT.0)JST=+1 !K0  K+ K*0  K*+ 
       ELSEIF(F.EQ.130)THEN
        JST=+1            !K_L0
       ENDIF
      ELSEIF(IJMB.EQ.3)THEN
       IF(F.EQ.3122.OR.                              !Lambda0
     &    F.EQ.3112.OR.F.EQ.3212.OR.F.EQ.3222.OR.    !Sigma  & Sigma~ 
     &    F.EQ.3114.OR.F.EQ.3214.OR.F.EQ.3224)THEN   !Sigma* & Sigma*~
          IF(KF.LT.0)JST=+1                          !s =-1
	  IF(KF.GT.0)JST=-1                          !s~=+1
       ELSEIF(F.EQ.3312.OR.F.EQ.3322.OR.             !Xi  & Xi0
     &        F.EQ.3314.OR.F.EQ.3324)THEN            !Xi* & Xi*0
        IF(KF.LT.0)JST=+2                            !
	IF(KF.GT.0)JST=-2                            !
       ELSEIF(F.EQ.3334)THEN
        IF(KF.LT.0)JST=+3
	IF(KF.GT.0)JST=-3
       ENDIF	
      ENDIF

*...judge if charmed particle
   30 IF(IJMB.EQ.2)THEN
       IF(F.EQ.411.OR.F.EQ.421.OR.F.EQ.413.OR.F.EQ.423)THEN
        IF(KF.GT.0)JCH=+1 !
	IF(KF.LT.0)JCH=-1 !
       ENDIF
      ELSEIF(IJMB.EQ.3)THEN
       IF(F.EQ.4112.OR.F.EQ.4114.OR.F.EQ.4122.OR.
     &    F.EQ.4122.OR.F.EQ.4214.OR.F.EQ.4132)THEN
        IF(KF.LT.0)JCH=-1
	IF(KF.GT.0)JCH=+1
       ENDIF
      ENDIF 
      RETURN
      END

C...Branch ratio of J/psi decay
      !***********************!
      SUBROUTINE JDCYBRCHR(FBR)
      !***********************!
      REAL BR(6),FBR(6)
      PI=3.1415926
      AE=0.007297353 !fine structure constant
      AS=0.178470865 !strong coupling constant at J/psi

*...J/psi -> ee
      BR(1)=1.0  

*...J/psi -> muon-muon
      BR(2)=1.0

*...J/psi -> quark-antiquark 
      BR(3)=2.0*BR(1)

*...J/psi -> gluon-gluon-gluon
      BR(4)=5.0/18.0/PI*(PI*PI-9.0)*AS*AS*AS/AE/AE
     &     *(1.0+103.0/10.0*AS/PI)*BR(1)

*...J/psi -> gamma-gluon-gluon
      BR(5)=16.0/5.0*AE/AS*(1.0-29.0/5.0*AS/PI)*BR(4)

C...J/psi -> gamma eta_c -> gamma-gamma and gluon-gluon -> hadrons
      BR(6)=0.033*(BR(1)+BR(2)+BR(3)+BR(4)+BR(5))

*...Distribution
      SUM=BR(1)+BR(2)+BR(3)+BR(4)+BR(5)+BR(6)
      FBR(1)=BR(1)/SUM
      DO I=2,6
       FBR(I)=FBR(I-1)+BR(I)/SUM
      ENDDO
      END

*...continue decay for unstable hadrons
      !***************!
      SUBROUTINE ALFDCY
      !***************!
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5)
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      SAVE /LUJETS/,/LUDAT01/
  
*...find string
   10 DO J=1,N
       IF(K(J,2).EQ.92)THEN
        ISTRING=J
        GOTO 20
       ENDIF
      ENDDO
     
*...forece unstable particle decay
   20 DO J=ISTRING+1,N
       !IF(K(J,1).EQ.4)K(J,1)=5   !?
      ENDDO
      CALL LUEXEC0

*...check if all unstable particle have decayed
      NUS=0
      DO J=ISTRING+1,N
       !IF(K(J,1).EQ.4)NUS=NUS+1
      ENDDO
      IF(NUS.GT.0)GOTO 10
      END

*...recoder the number group by heap-method
      !************************!
      SUBROUTINE HEAP(A,N,MM,NN)
      !************************!
      REAL A(N)
      L=NN-MM+1
      K=(NN-MM+1)/2
      DO M=K,1,-1
       I=M
       J=2*I
       X=A(I+MM-1)
   10  IF(J.LE.L)THEN
        IF(J.LT.L)THEN
         IF(A(J+MM-1).LT.A(J+MM))J=J+1
        ENDIF
        IF(X.LT.A(J+MM-1))THEN
         A(I+MM-1)=A(J+MM-1)
         I=J
         J=2*I
        ELSE
         GOTO 20
        ENDIF
        GOTO 10
       ENDIF
   20  A(I+MM-1)=X
      ENDDO
      DO NS=L,2,-1
       X=A(MM)
       A(MM)=A(NS+MM-1)
       A(NS+MM-1)=X
       I=1
       J=2
       X=A(MM)
       K=NS-1
   50  IF(J.LE.K)THEN
        IF(J.LT.K)THEN
         IF(A(J+MM-1).LT.A(J+MM))J=J+1
        ENDIF
        IF(X.LT.A(J+MM-1))THEN
         A(I+MM-1)=A(J+MM-1)
         I=J
         J=2*I
        ELSE
         GOTO 60
        ENDIF
        GOTO 50
       ENDIF
   60  A(I+MM-1)=X
      ENDDO
      RETURN
      END

      !*********************!
      SUBROUTINE FSUB(M,U,GF)
      !*********************!
      REAL GF(*),U(*),MPI,LAMD,LMDCUT,MTSI,MTSJ,MTSK
      EXTERNAL RQCD,SDRDS,RDDR
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/ARLWDAT3/KAU(100),XAU(100)
      COMMON/FSUBUSE/S,MTSI,MTSJ,MTSK
      SAVE /ARLWDAT1/,/ARLWDAT3/
      C=86.8
      ONE=1.0
      TWO=2.0
      FOU=4.0
      ICRX=IALFU(24)
      ECM=RALFU(1)
      B=RALPA(1)
      SCM0=ECM*ECM
      MPI=0.13957
      D=0.00001
      LMDCUT=1E-08
      SM=FOU*MPI*MPI
      
      IF(IALFU(23).EQ.1)GOTO 10
      IF(IALFU(23).EQ.2)GOTO 20
      IF(IALFU(23).EQ.3)GOTO 30
      
   10 DO L=1,M
       SCMP=U(L)
       ESP=SQRT(SCMP)
       IF(SCMP.LT.SM)GOTO 5
       IF(ICRX.EQ.1)THEN
        RSCMP=RQCD(SCMP)
	RSCM0=RQCD(SCM0)
       ELSEIF(ICRX.EQ.2)THEN
        RSCMP=RRHO(SCMP)+RDDR(SCMP)
	RSCM0=RRHO(SCM0)+RDDR(SCM0)
       ELSEIF(ICRX.EQ.3)THEN
        RSCMP=RQCD(SCMP)+RRHO(SCMP)+RDDR(SCMP)
        RSCM0=RQCD(SCM0)+RRHO(SCM0)+RDDR(SCM0)
       ENDIF
       GU=RSCMP/SCMP-RSCM0/SCM0
       DS=ABS(SCMP-SCM0)
       IF(DS.LE.D)THEN
        GF(L)=-(SDRDS(SCM0)-RSCM0)/SCM0/SCM0
       ELSE
        GF(L)=GU/(SCMP-SCM0)
       ENDIF
    5  CONTINUE
      ENDDO
      RETURN

   20 DO L=1,M
       X=U(L)
       SCMP=SCM0*(ONE-X)
       CROXM=C/SCMP
       IF(ICRX.EQ.1)THEN
	R=RQCD(SCMP)
       ELSEIF(ICRX.EQ.2)THEN
        R=RRHO(SCMP)+RDDR(SCMP)
       ELSEIF(ICRX.EQ.3)THEN
        R=RQCD(SCMP)+RRHO(SCMP)+RDDR(SCMP)
       ENDIF
       GF(L)=(ONE-X+X*X/TWO)/X*CROXM*R
      ENDDO
      RETURN
      
   30 X=XAU(11)
      V=XAU(12)
      W=XAU(13)
      DO L=1,M
       A=U(L)
       LAMD=((S-A)*(A-X)-V)*((S-A)*(A-X)-V)-W
       IF(LAMD.LT.LMDCUT)LAMD=LMDCUT*RLU0(0)
       GF(L)=EXP(-B*A)/SQRT(LAMD)
      ENDDO 
      RETURN
      END

      !****************************!
      FUNCTION RGMLT(FSUB,A,B,NI,NG)
      !****************************!
      IMPLICIT REAL (A-H,M,O-Z)
      IMPLICIT INTEGER(I-L,N)
      INTEGER NI,M0,NG,I0,I,J,MV,K
      CHARACTER NAME*(*)
      CHARACTER*80 ERRTXT
      PARAMETER (NAME='RGMLT')
      PARAMETER (Z1=1, HALF=Z1/2)
      DIMENSION W(14),T(14),V(64),U(100000),F(100000)
      EXTERNAL FSUB
      DATA (T(I),W(I),I=1,14)
     1/-0.93246 95142 03152 028D0, 0.17132 44923 79170 345D0,
     2 -0.66120 93864 66264 514D0, 0.36076 15730 48138 608D0,
     3 -0.23861 91860 83196 909D0, 0.46791 39345 72691 047D0,
     4  0.23861 91860 83196 909D0, 0.46791 39345 72691 047D0,
     5  0.66120 93864 66264 514D0, 0.36076 15730 48138 608D0,
     6  0.93246 95142 03152 028D0, 0.17132 44923 79170 345D0,
     7 -0.96028 98564 97536 232D0, 0.10122 85362 90376 259D0,
     8 -0.79666 64774 13626 740D0, 0.22238 10344 53374 471D0,
     9 -0.52553 24099 16328 986D0, 0.31370 66458 77887 287D0,
     A -0.18343 46424 95649 805D0, 0.36268 37833 78361 983D0,
     B  0.18343 46424 95649 805D0, 0.36268 37833 78361 983D0,
     C  0.52553 24099 16328 986D0, 0.31370 66458 77887 287D0,
     D  0.79666 64774 13626 740D0, 0.22238 10344 53374 471D0,
     E  0.96028 98564 97536 232D0, 0.10122 85362 90376 259D0/
      IF(NI .LE. 0) THEN
       WRITE(ERRTXT,101) NI
       !CALL MTLPRT(NAME,'D110.1',ERRTXT)
       STOP
      ENDIF
      M0=NG
      IF(M0 .NE. 8) M0=6
      I0=0
      IF(M0 .EQ. 8) I0=6
      D=(B-A)/NI
      R=HALF*D
      RA=R+A
      MV=MOD(M0*NI-1,64)+1
      S=0
      J=0
      DO 1 I = 1+I0,M0+I0
       RTA=R*T(I)+RA
      DO 2 K = 1,NI
      J=J+1
      V(J)=W(I)
      U(J)=RTA+(K-1)*D
      IF(J .EQ. MV) THEN
       CALL FSUB(MV,U,F)
       DO 3 J = 1,MV
    3  S=S+V(J)*F(J)
       MV=64
       J=0
      END IF
    2 CONTINUE
    1 CONTINUE
      RGMLT=R*S
      RETURN
  101 FORMAT('N1 = ',I4,' <= 0')
      END
      
*
*#include "gen/pilot.h"
      SUBROUTINE CORSET (V,C,NP)
C         Correlated Gaussian random number package
C
C         CORSET sets up the generation
C         CORGEN actually generates a set of NP random numbers
C                Gaussian-distributed with covariance matrix V
C                and mean values zero.
C
C         NP is dimension of vectors and matrices
C         V(NP,NP) is covariance matrix 
C         C(NP,NP) is working space used internally to hold the
C                  'square root' of V
C         X(NP) is the vector of random numbers generated by CORGEN
C
      DIMENSION V(NP,NP), C(NP,NP)
C
C               Compute C = square root of matrix V
      DO 40 J=1,NP
C               Diagonal terms
      CK = 0.
         DO 10 K= 1, J-1
   10    CK = CK + C(J,K)*C(J,K)
      C(J,J) = SQRT(ABS(V(J,J) - CK))
C
C               Off-diagonal terms
      DO 30 I= J+1, NP
         CK = 0.
         DO 20 K= 1, J-1
   20    CK = CK + C(I,K)*C(J,K)
      C(I,J) = (V(I,J) - CK) /C(J,J)
   30 CONTINUE
   40 CONTINUE
C
      RETURN
      END

*
* $Id: luarlw.F,v 1.22 2017/11/01 08:13:43 pingrg Exp $
*
* $Log: luarlw.F,v $
* Revision 1.22  2017/11/01 08:13:43  pingrg
* *** empty log message ***
*
* Revision 1.21  2017/05/09 07:59:37  pingrg
* *** empty log message ***
*
* Revision 1.20  2016/09/13 05:24:27  pingrg
* *** empty log message ***
*
* Revision 1.19  2016/01/13 07:55:30  pingrg
* update the sampling method for ConExc model
*
* Revision 1.18  2015/10/30 03:32:29  pingrg
* *** empty log message ***
*
* Revision 1.17  2015/10/12 07:19:39  pingrg
* *** empty log message ***
*
* Revision 1.16  2015/08/14 12:40:18  pingrg
* charm quark produced in lunda
*
* Revision 1.15  2015/08/07 12:07:09  pingrg
* *** empty log message ***
*
* Revision 1.14  2015/07/13 11:47:40  pingrg
* *** empty log message ***
*
* Revision 1.13  2015/03/23 13:10:36  pingrg
* smooth R-value xsection
*
* Revision 1.12  2015/01/12 00:55:46  pingrg
* *** empty log message ***
*
* Revision 1.10  2015/01/09 14:19:34  pingrg
* modify vacuum pol.,KKmode and selection of lunda model final state
*
* Revision 1.9  2014/09/22 12:23:17  pingrg
* *** empty log message ***
*
* Revision 1.8  2014/09/06 09:00:50  pingrg
* *** empty log message ***
*
* Revision 1.7  2014/08/26 13:56:58  pingrg
* *** empty log message ***
*
* Revision 1.6  2014/08/20 05:03:52  pingrg
* add mode 67 to ConExc model
*
* Revision 1.5  2014/08/12 00:08:16  pingrg
* *** empty log message ***
*
* Revision 1.4  2014/08/05 02:40:38  pingrg
* update conexc
*
* Revision 1.3  2013/06/09 10:14:00  pingrg
* modify the Lunda to BOSS662
*
* Revision 1.5  2012/05/24 03:53:44  pingrg
* update the fortran output at 64x
*
* Revision 1.4  2010/04/28 01:05:57  pingrg
* modify the RALPA(10)=4.0 in lunda.F
*
* Revision 1.3  2010/04/02 09:11:28  pingrg
* add ISR option to Lunda
*
* Revision 1.2  2010/02/02 09:40:59  pingrg
* allow user to set parameter for Lundcharm and Lunda model
*
* Revision 1.1  2010/01/26 02:40:14  pingrg
* add model LUNDA
*
* Revision 1.1.1.1  1996/04/01 15:02:55  mclareni
* Mathlib gen
*
*
*#include "gen/pilot.h"
      SUBROUTINE CORGEN(C,X,NP)
C
C         CORSET sets up the generation by calculating C from V.
C         CORGEN generates a set of NP random numbers
C                Gaussian-distributed with covariance matrix V
C                (V = C*C') and mean values zero.
C
      DIMENSION C(NP,NP), X(NP)
      PARAMETER (NMAX=100)
      DIMENSION Z(NMAX)
C
      IF (NP .GT. NMAX)  GO TO 120
C
      DO I=1,NP
       Z(I)=RLU0(0)  !CALL RNORML(Z,NP)
      ENDDO
C
      DO 100 I= 1, NP
         X(I) = 0.
         DO 90 J= 1, I
         X(I) = X(I) + C(I,J)*Z(J)
   90    CONTINUE
  100 CONTINUE
C
      RETURN
C                Error return
  120 CONTINUE
      WRITE (6,121) NP,NMAX
  121 FORMAT (' ERROR IN CORGEN. VECTOR LENGTH NP=',I5,
     1   ', BUT MAXIMUM ALLOWED IS',I5)
      RETURN
      END

      !----------------      Hu Haiming     --------------------!
      !             original:       April 13, 2000              !
      !             update  :       October 15, 2004            !    
      !*********************************************************!
      ! By The Strategy Of Described In SLAC-PUB-4160 Dec. 1986 !
      ! By The Strategy Of Described In SLAC-PUB-5160 Jan. 1990 !
      !---------------------------------------------------------!
      ! This program have the following characteristics         !
      !* spread of beam energy was considered                   !
      !* continuus states were calculated numericaly            !
      !* narrow resonances were treated as analytical formula   !
      !* wide-resonant psi(M>4000) widthes are energy-dependent !
      !* wide-resonant psi(M>4000) were calculated numericaly   !
      !* error-distribution of resonant parameters              !
      !*********************************************************! 

      !***************************!
      SUBROUTINE ISRCRYSTBALL(ECM0)
      !***************************!
      INTEGER IPAMT(100),NAROW(14)
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      REAL MSR(14),WER(14),WHR(14),WTR(14)
      REAL RSTH(14),GMT(14),DGMRC(14),SUMRCH(14),DGARC(14)
      REAL DNDK(0:2000),DNDTHE(0:2000)
      REAL ECMI(100),LUM(100),DLUM(100),NHD(100),DNHD(100)
      DOUBLE PRECISION DSUM
      EXTERNAL FSUBISR,RGMLTISR,RQCD
      COMMON/BESDATA/ECMI,LUM,DLUM,NHD,DNHD
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      COMMON/RAD/XK(0:2000),FDNDK(0:2000),THE(0:2000),FDNDTHE(0:2000)
      SAVE /PARAMET/,/RESDATA1/,/RESDATA2/,/RESDATA3/,/RAD/
      
*...set parameters IPAMT(*)
      IPAMT(01)=0     !0: no energy spread; 1:with energy spre
      IPAMT(02)=1     !1:RL resonances; 2:NRL resonances
      IPAMT(03)=0     !0: nointerference; 1: with interference
      IPAMT(04)=2     !1:qqbar as fermion pair; 2:as hadronic state
      IPAMT(05)=5     !number of quark flavours
      IPAMT(06)=1     !1:consider resonance threshold; 0:don't
      IPAMT(07)=1     !1:use PDG resonant parameters; 2:BES's 
      IPAMT(08)=1     !1:wide-res numerical, other analy; 2: all numerical
      IPAMT(09)=0     !0: eff=1; 1: eff=real values (no use)
      IPAMT(10)=1     !1:energy with step; 2:BES scan energies
      IPAMT(11)=2     !res-vac-pol form. 1:SLAC(86)-4160;2:SLAC(90)-5160
      IPAMT(12)=0     !res-paramt erro distributions. 0:no; 1:yes
      IPAMT(52)=301   !number of calculated energy points
      IPAMT(54)=100   !number of sample for each energy point 
      IPAMT(55)=1000  !number of inteval in Gaussian integral NI
      IPAMT(56)=8     !number of Gaussian point in integral NG
      IPAMT(58)=2     !integral formula choice. 1: math manu; 2:wangp
      IPAMT(59)=1     !which integration to be calculate
      IPAMT(60)=1     !fit way
      IPAMT(61)=1     !BES Ecm order number fill in calculations
      IPAMT(62)=1     !(D=1)1: non-exponetial ISR; 2: exponetial ISR
      IPAMT(71)=0     !Nbin for k
      IPAMT(72)=0     !Nbin for theta

*...set parameters RPAMT(*)
      RPAMT(01)=ECM0        !the startting or setting beam cms energ
      RPAMT(02)=RPAMT(1)    !real beam Ecm (filled bellow)
      RPAMT(03)=RPAMT(2)    !real collision Ecmp after ISR (filled below)
      RPAMT(04)=RPAMT(1)**2 !SCM0 (fill again bellow)
      RPAMT(05)=RPAMT(2)**2 !SCM  (fill again bellow)
      RPAMT(06)=RPAMT(3)**2 !SCMP (fill again bellow)
      RPAMT(07)=30.0        !BEPC beam current (mA) in data taking
      RPAMT(08)=0.001       !small photon k0
      RPAMT(09)=0.0         !1+d of ISR (get below)
      RPAMT(10)=1.0         !average hadronic effciency (get below)
      RPAMT(11)=5.02        !maximum Ecm in calculation
      RPAMT(12)=0.0         !delta_AR (filled below)
      RPAMT(13)=0.0         !R(S) (filled below)
      RPAMT(14)=0.0         !Sigmma_0vs (filled below)
      RPAMT(15)=0.0         !Sigmma_hrd (filled below)
      RPAMT(16)=0.0         !radiator beta (filled below)
      RPAMT(41)=0.325       !d mass
      RPAMT(42)=0.325       !u mass
      RPAMT(43)=0.600       !s mass
      RPAMT(44)=1.867       !c mass
      RPAMT(45)=5.200       !b mass
      RPAMT(46)=174.0       !t mass
      RPAMT(47)=0.000511    !electron mass
      RPAMT(48)=0.105658    !muon mass
      RPAMT(49)=1.777050    !tau mass
      RPAMT(50)=0.139570    !pi+- mass
      RPAMT(51)=1.8646      !D0 mass
      RPAMT(52)=0.007297    !EM coupling constant
      RPAMT(55)=3.1415926535!ratio of the circumference
      RPAMT(61)=0.0001      !DS for SCM-DS < SCMP < SCM+DS in Dvac
      RPAMT(62)=10.000      !Ecm1:RQCD(Scm)=const for Scm>S1=Ecm1^2
      RPAMT(71)=1.8646	    !D0 D~0    mass
      RPAMT(72)=1.8693	    !D+ D-     mass
      RPAMT(73)=2.0067	    !D*0 D~*0  mass
      RPAMT(74)=2.0100	    !D*+ D*-   mass
      RPAMT(75)=1.9685	    !Ds+ Ds-   mass
      RPAMT(76)=2.1124      !Ds*+ Ds*- mass
      RPAMT(88)=0.001       !step length in calculation of delta_theta
      RPAMT(89)=0.001       !step length in calculation of delta_k
      RPAMT(90)=300.0       !random seed for Gaussian distribution
      RPAMT(95)=20000.0     !maximum of output 1+d
      JBG=1                 !first resonance
      JED=14                !last resonance

*...constants
      DE=0.01
      RSEED=RPAMT(90)
      IF(IPAMT(10).EQ.2)IPAMT(52)=85
      IF(IPAMT(1).EQ.0)IPAMT(54)=1
      RSIM=IPAMT(54)

*...input contents used
      CALL RESONDATA
      X=0.27
c      WRITE(*,*)'R value ISR calculation :'
c      WRITE(*,*)'Ecm   Rcon  Rres'
      DO J=1,473
       X=X+0.01
       XX=X*X
       Y=RQCD(XX)
       Z=RRES(XX)
       YZ=Y+Z
c       WRITE(*,96)X,Y,Z,YZ
      ENDDO

*...energy 
      ECM0=RPAMT(1)
      SCM0=RPAMT(4)

*...calculate the efficiency
      IF(IPAMT(9).GE.1.AND.IPAMT(10).EQ.2)THEN
       IE=9+NPOINT
       CALL EFFAVE(IE,OUT)
       RPAMT(10)=OUT
      ELSE
       RPAMT(10)=1.0
      ENDIF

*...calculate ISR
      SUMRVL=0
      SUMRCO=0
      SUMRRE=0
      SUMVRT=0
      SUMVAC=0
      SUMGAC=0 
      SUMGAR=0
      DO J=JBG,JED
       SUMRCH(J)=0
      ENDDO
      DO ISIM=1,IPAMT(54)
       IF(IPAMT(1).EQ.0)THEN
        ECM=ECM0
       ELSE
        DECM=0.001*(0.00722*RPAMT(7)+0.4780)*(0.0705*SCM0+1.0656)
        ECM=GRN(ECM0,DECM,RSEED)
       ENDIF
       SCM=ECM*ECM
       RC=RQCD(SCM)
       RR=RRES(SCM)
       R=RC+RR
       RPAMT(2)=ECM
       RPAMT(5)=SCM
       RPAMT(13)=R

*...resonant paramters distributions
       IF(IPAMT(12).GE.1.AND.IPAMT(54).GT.1)CALL RESONDATA

*...vertex correction
       CALL VERCOREC(DVERT) 
       
*...vacuum polarization (only corresponding to continuum states) 
       CALL VACUMPOL(DVACM)

*...bremsstrahlung for continuus states       
       CALL BREMSCON(DGAMC) 
      
*...bremsstrahlung for resonant states
       CALL BREMSRES(DGAMR)

*...bremsstrahlung for resonant each state of mass>J/psi
       CALL BRECHRES(JBG,JED,DGMRC)
       
*...total 1+d
       SUMRVL=SUMRVL+R
       SUMRCO=SUMRCO+RC
       SUMRRE=SUMRRE+RR
       SUMVRT=SUMVRT+DVERT
       SUMVAC=SUMVAC+DVACM
       SUMGAC=SUMGAC+DGAMC
       SUMGAR=SUMGAR+DGAMR
       DO J=JBG,JED
        SUMRCH(J)=SUMRCH(J)+DGMRC(J)
       ENDDO
       WRITE(*,300)IPAMT(54)-ISIM,ECM,DVERT,DVACM,DGAMC,DGAMR
      ENDDO
      RVALU=SUMRVL/RSIM
      RCONH=SUMRCO/RSIM
      RRESH=SUMRRE/RSIM
      DVERT=SUMVRT/RSIM
      DVACM=SUMVAC/RSIM
      DGAMC=SUMGAC/RSIM
      DGAMR=SUMGAR/RSIM
      DO J=JBG,JED
       DGARC(J)=SUMRCH(J)/RSIM
      ENDDO
      ONEDELTA=DVERT+DVACM+DGAMC+DGAMR
      RPAMT(9)=ONEDELTA
      
*...x2 
      IF(IPAMT(10).EQ.2)THEN
       CALL XSQURE(DR)
       X2=X2+DR
      ENDIF

*...soft and hard cross cection
      RPAMT(11)=DVERT+DVACM
      CALL SIGMVSH(SIGMVS,SIGMHD)
      RPAMT(14)=SIGMVS
      RPAMT(15)=SIGMHD

*...writeout the results
      WRITE(6,*)'-----*',IPAMT(52)-NPOINT,' *-----'
      WRITE(6,98)'Ecm = ',ECM0,' GeV  ISR 1+d =',ONEDELTA 
      IF(ONEDELTA.LT.RPAMT(95))THEN
       R0VS=SIGMVS/(SIGMVS+SIGMHD)
       RHRD=SIGMHD/(SIGMVS+SIGMHD)
       !WRITE(11,100)ECM0,RVALU,DVERT,DVACM,DGAMA,ONEDELTA
       !WRITE(31,100)ECM0,DVERT,DVACM,DGAMC,DGAMR,ONEDELTA
       !WRITE(41,200)ECM0,DGARC(09),DGARC(10),DGARC(11)
       !WRITE(61,200)ECM0,DGARC(12),DGARC(13),DGARC(14)
       !WRITE(*,95)'SIGMVS =',SIGMVS,'SIGMHD =',SIGMHD
       !WRITE(*,*)'R0VS =',R0VS,'   RHRD =',RHRD
      ENDIF

*...calculate the bremmstrulung spectrum (k)
      NK=(ECM0*SQRT(1-RPAMT(8))-2*RPAMT(50))/RPAMT(89)
      ONE=1
      TWO=2
      C=86.8
      MK=0
      DSUM=0D0
      EM=RPAMT(47)
      T=TWO*RPAMT(52)/RPAMT(55)*(ALOG(SCM/EM/EM)-ONE)
      DNDK(0)=0
      DO K=1,NK
       X=RPAMT(8)+(K-1)*RPAMT(89)
       IF(X.LT.1)THEN
        MK=MK+1
        XK(MK)=X
        DNDK(MK)=C/SCM*T*XEPECTRUM(ECM0,X)/RPAMT(15)*RPAMT(89)
        DSUM=DSUM+DNDK(MK)
       ENDIF
      ENDDO
      MK=MK+1
      XK(MK)=1.0
      DNDK(MK)=0
      SUN=0
      FDNDK(0)=0
      DO I=1,MK
       DNDK(I)=DNDK(I)/DSUM
       FDNDK(I)=FDNDK(I-1)+DNDK(I)
       IF(FDNDK(I).GT.1.0)FDNDK(I)=1.0
       ECMP=ECM0*SQRT(ONE-XK(I))
c       WRITE(*,93)I,XK(I),ECMP,DNDK(I),FDNDK(I)
       WRITE(70,93)I,XK(I),ECMP,DNDK(I),FDNDK(I)
      ENDDO
      IPAMT(71)=MK     !Nbin for k

*...calculate the bremmstrulung spectrum (theta)
      NTHE=0.5*RPAMT(55)/RPAMT(88)
      DSUM=0D0
      DO J=0,NTHE
       THE(J)=J*RPAMT(88)  !0 degree < theta < pi/2
       COSTHEJ=COS(THE(J))
       IF(J.EQ.NTHE)THEN
        THE(J)=0.5*RPAMT(55)
        COSTHEJ=0.0
       ENDIF
       DNDTHE(J)=COSSPECTRUM(ECM0,COSTHEJ)
       DSUM=DSUM+DNDTHE(J)
      ENDDO

*...polar angle distribution
c      WRITE(*,*)'Polar angle distribution:'
c      WRITE(*,*)'#, theta(degree), cos_theta, dsdtheta, F(theta)'
      DO J=0,NTHE
       THE(J)=J*RPAMT(88)
       IF(J.EQ.NTHE)THE(J)=0.5*RPAMT(55)
       DNDTHE(J)=DNDTHE(J)/DSUM
       FDNDTHE(J)=FDNDTHE(J-1)+DNDTHE(J)
       IF(FDNDTHE(J).GT.1.0)FDNDTHE(J)=1.0
       THEDG=THE(J)*180.0/RPAMT(55)
c       WRITE(*,92)J,THEDG,COS(THE(J)),DNDTHE(J),FDNDTHE(J)
      ENDDO
      DO J=0,NTHE
       DNDTHE(J)=DNDTHE(J)/DNDTHE(NTHE)
      ENDDO
      IPAMT(72)=NTHE     !Nbin for cos(theta)
      IF(IPAMT(10).EQ.2)WRITE(6,*)'X2 =',X2
   92 FORMAT(I5,1X,F10.4,1X,F10.8,1X,F12.10,1X,F10.8)
   93 FORMAT(I5,1X,F6.3,1X,F8.4,1X,F10.6,1X,F10.6)
   94 FORMAT(I8,3X,F8.6,1X,F12.8,1X,F10.8)
   95 FORMAT(A9,F8.4,1X,A9,F8.4)
   96 FORMAT(F6.2,1X,F8.4,1X,F8.4,1X,F8.4)
   97 FORMAT(I5,A4,I5,A4,F8.4)
   98 FORMAT(A8,F6.4,A15,F12.6)
   99 FORMAT(A10,F8.4)
  100 FORMAT(6(F8.4))
  200 FORMAT(F8.4,3(F12.8))
  300 FORMAT(I5,F8.4,1X,4(F12.8,1X))
      END

*...vertex correction factor
      !************************!
      SUBROUTINE VERCOREC(DVERT)
      !************************!
      INTEGER IPAMT(100)
      REAL RPAMT(100)
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /PARAMET/
      SCM=RPAMT(5)
      EM=RPAMT(47) 
      AEM=RPAMT(52)
      PI=RPAMT(55)
      ONE=1.0
      TWO=2.0
      THR=3.0
      FUR=4.0
      SIX=6.0
      BRK=THR/FUR*ALOG(SCM/EM/EM)-ONE+PI*PI/SIX
      DVERT=TWO*AEM/PI*BRK
      END

*...vacuum polarization factor (only for continuum states)
      !************************!
      SUBROUTINE VACUMPOL(DVACM)
      !************************!
      INTEGER IPAMT(100),NAROW(14)   
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      REAL MSR(14),WER(14),WHR(14),WTR(14)
      REAL RSTH(14),GMT(14),MS,NIN
      DOUBLE PRECISION DSM,DMRS,DZ,DF1,DF2,DF3,DSUM
      DOUBLE PRECISION DECM,DSCM,DWTT,DWEE,DWHD
      DOUBLE PRECISION DZRO,DTWO,DTHR,DPI
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      SAVE /PARAMET/
      SAVE /RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      EXTERNAL FSUBISR,RGMLTISR,RQCD
      NI=IPAMT(55)
      NG=IPAMT(56)
      ECM=RPAMT(2)
      DECM=ECM
      SCM=RPAMT(5)
      DSCM=SCM
      AEM=RPAMT(52)
      PI=RPAMT(55)
      EPS=RPAMT(60)
      ONE=1.0
      TWO=2.0
      THR=3.0
      FUR=4.0
      FIV=5.0
      SIX=6.0
      NIN=9.0
      DZRO=0D0
      DTWO=TWO
      DTHR=THR
      DTEN=1.0D01
      DPI=PI
      TEN=10.0
      
*...vacuum polarization for e+e- pair
      MS=RPAMT(47)
      CALL FERMILOOP(MS,ONE,FMLP)
      DVACE=FMLP

*...vacuum polarization for muon pair
      MS=RPAMT(48)
      CALL FERMILOOP(MS,ONE,FMLP)
      DVACM=FMLP

*...vacuum polarization for muon pair:
      MS=RPAMT(49)
      CALL FERMILOOP(MS,ONE,FMLP)
      DVACT=FMLP

*...vacuum polarization of qqbar pair
      IF(IPAMT(4).EQ.1)THEN
       SUM=0
       DO J=1,IPAMT(5)
        MS=RPAMT(40+J)
        IF(J.EQ.1.OR.J.EQ.3.OR.J.EQ.5)Q=0.33333333
        IF(J.EQ.2.OR.J.EQ.4.OR.J.EQ.6)Q=0.66666667
        CALL FERMILOOP(MS,Q,FMLP)
        SUM=SUM+FMLP
       ENDDO
       DVACQ=1.2*THR*SUM       
      ELSE
       DSUM=0D00
       DO J=1,NRES
        DMRS=MRS(J)
        DWEE=WEE(J)
        DWTT=WTT(J)
        DWHD=WHD(J)
        IF(IPAMT(7).EQ.2.AND.J.GE.11)THEN
         DWTT=WTTJSCM(J,SCM)
         DWHD=DWTT-DTHR*DWEE
        ENDIF
        IF(DWHD.GT.DZRO)THEN
         DSM=(DECM+DMRS)*(DECM-DMRS)
         DZ=DSQRT((DECM+DWTT)*(DECM-DWTT))
         DF1=DWEE/DMRS
         IF(IPAMT(11).EQ.1)THEN
          DF2=DSM+DWTT*DWTT
         ELSE
          DF2=DSM+DTWO*DWTT*DMRS*DLOG(DECM/DMRS)/DPI
         ENDIF
         DF3=DSM*DSM+DMRS*DMRS*DWTT*DWTT
         DSUM=DSUM+DF1*DF2/DF3
        ENDIF
       ENDDO
       SUM=DSUM
       DVACRES=SIX*SCM/AEM*SUM
       D=RPAMT(61)
       STH=FUR*RPAMT(50)*RPAMT(50)
       SUP1=SCM-D
       SDW1=FUR*RPAMT(50)*RPAMT(50)
       SUP2=RPAMT(62)*RPAMT(62)
       SDW2=SCM+D
       IPAMT(59)=1
       OUT1=RGMLTISR(FSUBISR,SDW1,SUP1,NI,NG)
       OUT2=RGMLTISR(FSUBISR,SDW2,SUP2,NI,NG)
       OUT3=RQCD(SCM)*ALOG(SDW2/SUP1)
       ARG=ABS(SUP2-SCM)/SUP2
       OUT4=RQCD(SUP2)*ALOG(ARG)
       ARG=ABS((SUP2-SCM)/(STH-SCM))
       OUT5=RQCD(SCM)*ALOG(ARG)
       OUT=-SCM*(OUT1+OUT2)+OUT3+OUT4-OUT5
       DVACCON=TWO*AEM/THR/PI*OUT
       DVACQ=DVACRES+DVACCON
      ENDIF

*...total vacuum polarization
      DVACM=DVACE+DVACM+DVACT+DVACQ
      END 

*...fermion loop in vacuum polarization
      !*****************************!
      SUBROUTINE FERMILOOP(MS,Q,FMLP)
      !*****************************!
      INTEGER IPAMT(100)
      REAL MS,NIN,RPAMT(100)
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /PARAMET/
      ECM=RPAMT(2)
      SCM=RPAMT(5)
      A=RPAMT(52)
      PI=RPAMT(55)
      ONE=1.0
      TWO=2.0
      THR=3.0
      FUR=4.0
      SIX=6.0
      NIN=9.0
      TEN=10.0
      YMX=0.9999
      STH=FUR*MS*MS  !threshold value for producing pair
      X=STH/SCM
      IF(X.LE.ONE)THEN
       Y=SQRT(ONE-X)
       IF(Y.LT.YMX)THEN
        FMLP=-TEN-SIX*X+THR*Y*(TWO+X)*ALOG((ONE+Y)/(ONE-Y))
       ELSE
        FMLP=SIX*ALOG(SCM/MS/MS)-TEN
       ENDIF
      ELSE 
       Y=SQRT(X-ONE)
       FMLP=-TEN-SIX*X+SIX*Y*(TWO+X)*ATAN(ONE/Y)
      ENDIF
      FMLP=A*Q*Q/PI/NIN*FMLP
      RETURN
      END

*...bremmstrahung radiation correction for continuum states
      !************************!
      SUBROUTINE BREMSCON(DGAMA)
      !************************!
      INTEGER IPAMT(100),NAROW(14)   
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14),NIN
      REAL MSR(14),WER(14),WHR(14),WTR(14),RSTH(14),GMT(14)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      SAVE /PARAMET/
      SAVE /RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      EXTERNAL FSUBISR,RGMLTISR,RQCD,RRES,PHICST,PSICST,FIT
      ZRO=0.0
      ONE=1.0
      TWO=2.0
      THR=3.0
      FUR=4.0
      NIN=9.0
      DZRO=0.0D00
      DHLF=5.0D-1
      DONE=1.0D00
      DTWO=2.0D00
      DTHR=3.0D00
      DFUR=4.0D00
      DTEN=1.0D01
      NI=IPAMT(55)
      NG=IPAMT(56)
      W=RPAMT(2)
      SCM=RPAMT(5)
      S=SCM
      X0=RPAMT(08)
      EM=RPAMT(47)
      PIM=RPAMT(50)
      AEM=RPAMT(52)
      PI=RPAMT(55)
      DX0=1.0D-10
      DPI=3.1415926D00
      EFF0=1.0

*...Rcon value
      RC=RQCD(SCM)    
      RR=RRES(SCM)
      IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)EFF0=FIT(3,ZRO)
      REFF=EFF0*(RC+RR)

*...constants
      T=TWO*AEM/PI*(ALOG(SCM/EM/EM)-ONE)
      RPAMT(16)=T
      X0T=X0**T
      DT=T
      T1=T+ONE
      T2=T+TWO

*...contribution from continuum hadronic states in small k region
      TERM1=X0T*(ONE-T/T1*X0+T/T2/TWO*X0*X0)

*...contribution from continuum hadronic states in large k region
      IPAMT(59)=2
      XM=ONE-FUR*PIM*PIM/SCM
      TERM2=T*RGMLTISR(FSUBISR,X0,XM,NI,NG)/REFF

*...total bremsstrahm
      DGAMA=TERM1+TERM2
      END 

*...bremmstrahung radiation correction for resonant states
      !************************!
      SUBROUTINE BREMSRES(DGAMA)
      !************************!
      IMPLICIT NONE
      INTEGER J,NI,NG,NRES
      INTEGER IPAMT(100),NAROW(14)   
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14),C(4)
      REAL MSR(14),WER(14),WHR(14),WTR(14),RSTH(14),GMT(14)
      REAL INTEGRAL0,INTEGRAL1,INTEGRAL2,INTEGRAL3,INTEGRAL4
      REAL INTEGRAL5,INTEGRAL6,FIT,RGMLTISR,CONHRES
      REAL DGAMA,RINT,MS,WE,WH,W,SCM0,SCM,S,X0,EM,PIM,AEM,PI
      REAL T,SUM,XMIN,XMAX,EFF0,RQCD,RRES,REFF
      REAL ZRO,ONE,TWO,FUR,NIN
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      COMMON/EFFCOFIE/C
      SAVE /PARAMET/,/RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      EXTERNAL FSUBISR,RGMLTISR,RQCD,RRES,PHICST,PSICST,FIT
      EXTERNAL INTEGRAL0,INTEGRAL1,INTEGRAL2,INTEGRAL3
      EXTERNAL INTEGRAL4,INTEGRAL5,INTEGRAL6
      NI=IPAMT(55)
      NG=IPAMT(56)
      W=RPAMT(2)
      SCM0=RPAMT(4)
      SCM=RPAMT(5)
      S=SCM
      X0=RPAMT(08)
      EM=RPAMT(47)
      PIM=RPAMT(50)
      AEM=RPAMT(52)
      PI=RPAMT(55)
      ZRO=0.0
      ONE=1.0
      TWO=2.0    
      FUR=4.0
      NIN=9.0

*...efficiency
      IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)THEN
       EFF0=FIT(3,ZRO)  !real efficiency
      ELSE
       EFF0=ONE         !efficiency=1
       C(1)=ONE
       DO J=2,4
        C(J)=ZRO
       ENDDO
      ENDIF

*...Rcon value
      REFF=EFF0*(RQCD(SCM)+RRES(SCM))

      IF(IPAMT(8).GE.2)GOTO 88

*...constants
      T=TWO*AEM/PI*(ALOG(SCM/EM/EM)-ONE)

*...contribution from resonances (incoherent, analytical)
      SUM=ZRO
      DO J=1,11
       MS=MRS(J)
       WE=WEE(J)
       WH=WHD(J)
       IF(IPAMT(6).LE.0)THEN !don't consider threshold
        XMAX=ONE
       ELSE                  !consider threshold
        XMAX=ONE-FUR*EM*EM/SCM
       ENDIF
       XMIN=ZRO
       IF(XMAX.GT.XMIN)THEN
        SUM=SUM
     &     +WE*WH*(C(1)*INTEGRAL1(J,XMIN,XMAX)
     &     +(-C(1)+C(2))*INTEGRAL2(J,XMIN,XMAX)
     &     +(C(1)/TWO-C(2)+C(3))*INTEGRAL3(J,XMIN,XMAX)
     &     +(C(2)/TWO-C(3)+C(4))*INTEGRAL4(J,XMIN,XMAX)
     &     +(C(3)/TWO-C(4))*INTEGRAL5(J,XMIN,XMAX)
     &     +C(4)/TWO*INTEGRAL6(J,XMIN,XMAX))
       ENDIF
      ENDDO
      DGAMA=T*NIN/AEM/AEM*SUM/SCM/REFF
     
*...contribution from wide resonanaces (numericaly)
      IPAMT(59)=3
      XMAX=ONE-FUR*EM*EM/SCM
      DGAMA=DGAMA+T*RGMLTISR(FSUBISR,X0,XMAX,NI,NG)/REFF
     
*...contribution from resonances (coherent)
      IF(IPAMT(3).GE.1)THEN
       CALL CONHRNT(CONHRES)
       DGAMA=DGAMA+CONHRES/REFF
      ENDIF
      RETURN

*...contribution from resonances (all numerical)
   88 IPAMT(59)=9
      XMAX=ONE-FUR*EM*EM/SCM
      DGAMA=T*RGMLTISR(FSUBISR,X0,XMAX,NI,NG)/REFF
      RETURN
      END 

*...bremsstrahlung contribution for each resonance
      !**********************************!
      SUBROUTINE BRECHRES(JB,JE,DGAMARESJ)
      !**********************************!
      IMPLICIT NONE
      INTEGER J,NI,NG,NRES,JB,JE
      INTEGER IPAMT(100),NAROW(14)   
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14),C(4)
      REAL MSR(14),WER(14),WHR(14),WTR(14),RSTH(14),GMT(14)
      REAL INTEGRAL0,INTEGRAL1,INTEGRAL2,INTEGRAL3,INTEGRAL4
      REAL INTEGRAL5,INTEGRAL6,FIT,DGAMARESJ(14)
      REAL RINT,MS,WE,WH,W,SCM0,SCM,S,X0,EM,PIM,AEM,PI
      REAL T,XMIN,XMAX,EFF0,RQCD,RRES,REFF
      REAL ZRO,ONE,TWO,FUR,NIN
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      COMMON/EFFCOFIE/C
      SAVE /PARAMET/,/RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      EXTERNAL FSUBISR,RGMLTISR,RQCD,RRES,PHICST,PSICST,FIT
      EXTERNAL INTEGRAL0,INTEGRAL1,INTEGRAL2,INTEGRAL3
      EXTERNAL INTEGRAL4,INTEGRAL5,INTEGRAL6
      NI=IPAMT(55)
      NG=IPAMT(56)
      W=RPAMT(2)
      SCM0=RPAMT(4)
      SCM=RPAMT(5)
      S=SCM
      X0=RPAMT(08)
      EM=RPAMT(47)
      PIM=RPAMT(50)
      AEM=RPAMT(52)
      PI=RPAMT(55)
      ZRO=0.0
      ONE=1.0
      TWO=2.0    
      FUR=4.0
      NIN=9.0

*...efficiency
      IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)THEN
       EFF0=FIT(3,ZRO)  !real efficiency
      ELSE
       EFF0=ONE         !efficiency=1
       C(1)=ONE
       DO J=2,4
        C(J)=ZRO
       ENDDO
      ENDIF

*...Rcon value
      REFF=EFF0*(RQCD(SCM)+RRES(SCM))

*...constants
      T=TWO*AEM/PI*(ALOG(SCM/EM/EM)-ONE)

*...contribution from resonances (incoherent, analytical)
      DO J=1,14
       DGAMARESJ(J)=ZRO
      ENDDO
      DO J=JB,JE
       MS=MRS(J)
       WE=WEE(J)
       WH=WHD(J)
       IF(IPAMT(6).LE.0)THEN !don't consider threshold
        XMAX=ONE
       ELSE                  !consider threshold
        XMAX=ONE-FUR*EM*EM/SCM
       ENDIF
       XMIN=ZRO
       IF(XMAX.GT.XMIN)THEN
        DGAMARESJ(J)=T*NIN/AEM/AEM*WE*WH
     &     *(C(1)*INTEGRAL1(J,XMIN,XMAX)
     &     +(-C(1)+C(2))*INTEGRAL2(J,XMIN,XMAX)
     &     +(C(1)/TWO-C(2)+C(3))*INTEGRAL3(J,XMIN,XMAX)
     &     +(C(2)/TWO-C(3)+C(4))*INTEGRAL4(J,XMIN,XMAX)
     &     +(C(3)/TWO-C(4))*INTEGRAL5(J,XMIN,XMAX)
     &     +C(4)/TWO*INTEGRAL6(J,XMIN,XMAX))/SCM/REFF
       ENDIF
      ENDDO
      END

      !*************************!
      SUBROUTINE CONHRNT(CONHRES)
      !*************************!
      INTEGER IPAMT(100),NAROW(14)
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14),MD
      REAL MSR(14),WER(14),WHR(14),WTR(14),RSTH(14),GMT(14)
      DOUBLE PRECISION CONHAM,C12,C23,C31
      EXTERNAL FSUBISR,RGMLTISR,RQCD,CONHAM
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      SAVE /PARAMET/,/RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      ONE=1.0
      TWO=2.0
      FUR=4.0
      NIN=9.0
      MD=1.8646
      TRM1=0.0
      TRM2=0.0
      NI=IPAMT(55)
      NG=IPAMT(56)
      SCM=RPAMT(5)
      X0=RPAMT(8)
      XM=ONE-FUR*MD*MD/SCM
      EM=RPAMT(47)
      AEM=RPAMT(52)
      PI=RPAMT(55)
      T=TWO*AEM/PI*(ALOG(SCM/EM/EM)-ONE)
      T1=T-ONE
      T2=T-TWO

*...contribution fron k<k0 region
      IF(X0.GT.ZRO)THEN
       C12=CONHAM(12,13,SCM)
       C23=CONHAM(13,14,SCM)
       C31=CONHAM(14,12,SCM)
       C=C12+C23+C31
       TRM1=C*(X0**T)*(ONE-X0*T/T1+X0*X0*T/TWO/T2)
      ENDIF

*...contribution fron k>k0 region
      IPAMT(59)=4
      IF(XM.GT.X0)THEN
       TRM2=T*RGMLTISR(FSUBISR,X0,XM,NI,NG)
      ENDIF

*...conherent contribution
      CONHRES=TRM1+TRM2
      END

*...integral for bremsstrahlung
      !*****************************!
      FUNCTION INTEGRAL0(J,XMIN,XMAX)
      !*****************************!
      IMPLICIT NONE
      INTEGER J,IPAMT(100)
      REAL INTEGRAL0,XMIN,XMAX,RPAMT(100)
      REAL MRS(14),WEE(14),WHD(14),WTT(14)
      DOUBLE PRECISION A,B,C,T,ZRO,ONE,TWO,FUR,MS,WT,SCM
      DOUBLE PRECISION DXMIN,DXMAX,TRMAX,TRMIN,TMP
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /RESDATA1/,/PARAMET/
      ONE=1D00
      TWO=2D00
      FUR=4D00
      ZRO=1D-30
      DXMIN=XMIN
      DXMAX=XMAX
      MS=MRS(J)
      WT=WTT(J)
      SCM=RPAMT(5)
      A=ONE
      B=-TWO*(ONE-MS*MS/SCM)
      C=B*B/FUR+MS*MS*WT*WT/SCM/SCM
      T=B*B-FUR*A*C
      
*...result
      IF(T.GT.ZRO)THEN       ! b*b > 4ac
       TMP=DSQRT(T)
       TRMAX=DLOG((TWO*A*DXMAX+B-TMP)/(TWO*A*DXMAX+B+TMP))
       TRMIN=DLOG((TWO*A*DXMIN+B-TMP)/(TWO*A*DXMIN+B+TMP))
       INTEGRAL0=(TRMAX-TRMIN)/TMP
      ELSEIF(T.LT.ZRO)THEN   ! b*b < 4ac
       TMP=DSQRT(-T)
       TRMAX=DATAN((TWO*A*DXMAX+B)/TMP)
       TRMIN=DATAN((TWO*A*DXMIN+B)/TMP)
       INTEGRAL0=(TRMAX-TRMIN)/TMP
      ELSE                   ! b*b = 4ac
       TRMAX=-TWO/(TWO*A*DXMAX+B)
       TRMIN=-TWO/(TWO*A*DXMIN+B) 
       INTEGRAL0=TRMAX-TRMIN
      ENDIF
      RETURN
      END

*...integral for K^(beta-1) term in bremsstrahlung
      !*****************************!
      FUNCTION INTEGRAL1(J,XMIN,XMAX)
      !*****************************!
      IMPLICIT NONE
      INTEGER J,IPAMT(100)
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      REAL INTEGRAL1,XMAX,XMIN
      DOUBLE PRECISION MS,WE,WT,WH,COSTHE,THE,R2,R3,PSICST
      DOUBLE PRECISION SCM,W,ONE,TWO,THR,FUR,PI,AEM,EM
      DOUBLE PRECISION DXMAX,DXMIN,T,T2,T3,T4,A,A2 
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/PARAMET/IPAMT,RPAMT
      EXTERNAL PSICST 
      ONE=1D00
      TWO=2D00
      THR=3D00
      FUR=4D00
      W=RPAMT(2)
      SCM=RPAMT(5)
      EM=RPAMT(47)
      AEM=RPAMT(52)
      PI=RPAMT(55)
      DXMAX=XMAX
      DXMIN=XMIN

*...constants and parameters
      T=TWO*AEM/PI*(DLOG(SCM/EM/EM)-ONE)
      T2=T-TWO
      T3=T-THR
      T4=T-FUR
      MS=MRS(J)
      WE=WEE(J)
      WT=WTT(J)
      WH=WHD(J)
      A2=(ONE-MS*MS/SCM)*(ONE-MS*MS/SCM)+MS*MS*WT*WT/SCM/SCM
      A=DSQRT(A2)
      COSTHE=-(ONE-MS*MS/SCM)/A
      THE=DACOS(COSTHE)
      R2=TWO*(ONE-MS*MS/SCM)
      R3=THR*(ONE-MS*MS/SCM)*(ONE-MS*MS/SCM)-MS*MS*WT*WT/SCM/SCM
      
*...result     
      INTEGRAL1=(A**T2)*PSICST(THE,T)
     &         +(DXMAX**T2)/T2
     &         +(DXMAX**T3)/T3*R2
     &         +(DXMAX**T4)/T4*R3
      RETURN
      END

*...integral for K^beta term in bremsstrahlung
      !*****************************!
      FUNCTION INTEGRAL2(J,XMIN,XMAX)
      !*****************************!
      IMPLICIT NONE
      INTEGER J,IPAMT(100)
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      REAL INTEGRAL2,XMAX,XMIN
      DOUBLE PRECISION MS,WT,COSTHE,THE,R2,R3,PSICST
      DOUBLE PRECISION SCM,ONE,TWO,THR,PI,AEM,EM
      DOUBLE PRECISION DXMAX,DXMIN,T,T1,T2,T3,A,A2 
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/PARAMET/IPAMT,RPAMT
      EXTERNAL PSICST 
      ONE=1D00
      TWO=2D00
      THR=3D00
      SCM=RPAMT(5)
      EM=RPAMT(47)
      AEM=RPAMT(52)
      PI=RPAMT(55)
      DXMAX=XMAX
      DXMIN=XMIN

*...constants and parameters
      T=TWO*AEM/PI*(DLOG(SCM/EM/EM)-ONE)
      T1=T-ONE
      T2=T-TWO
      T3=T-THR
      MS=MRS(J)
      WT=WTT(J)
      A2=(ONE-MS*MS/SCM)*(ONE-MS*MS/SCM)+MS*MS*WT*WT/SCM/SCM
      A=DSQRT(A2)
      COSTHE=-(ONE-MS*MS/SCM)/A
      THE=DACOS(COSTHE)
      R2=TWO*(ONE-MS*MS/SCM)
      R3=THR*(ONE-MS*MS/SCM)*(ONE-MS*MS/SCM)-MS*MS*WT*WT/SCM/SCM
      
*...result     
      INTEGRAL2=(A**T1)*PSICST(THE,T+ONE)
     &         +(DXMAX**T1)/T1
     &         +(DXMAX**T2)/T2*R2
     &         +(DXMAX**T3)/T3*R3
      RETURN
      END

*...integral for K term in bremsstrahlung
      !*****************************!
      FUNCTION INTEGRAL3(J,XMIN,XMAX)
      !*****************************!
      IMPLICIT NONE
      INTEGER J,IPAMT(100)
      REAL INTEGRAL0,INTEGRAL3,XMIN,XMAX
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      DOUBLE PRECISION A,B,C,ZRO,ONE,TWO,FUR,W,SCM,MS,WE,WT,WH
      DOUBLE PRECISION DXMIN,DXMAX,TRUP,TRDW,DINT,A2,AA,BB
      DOUBLE PRECISION COSTHE,SINTHE,THE,PI
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /RESDATA1/,/PARAMET/
      EXTERNAL INTEGRAL0

*...constants
      ONE=1D00
      TWO=2D00
      FUR=4D00
      ZRO=1D-30
      W=RPAMT(2)
      SCM=RPAMT(5)
      PI=RPAMT(55)
      MS=MRS(J)
      WE=WEE(J)
      WT=WTT(J)
      WH=WHD(J)
      DXMIN=XMIN
      DXMAX=XMAX

*...integral
      IF(IPAMT(58).EQ.1)THEN
       A=ONE
       B=-TWO*(ONE-MS*MS/SCM)
       C=B*B/FUR+MS*MS*WT*WT/SCM/SCM
       TRUP=DLOG(A*DXMAX*DXMAX+B*DXMAX+C)
       TRDW=DLOG(A*DXMIN*DXMIN+B*DXMIN+C)
       DINT=INTEGRAL0(J,XMIN,XMAX)
       INTEGRAL3=(TRUP-TRDW-B*DINT)/TWO/A
      ELSE
       A2=(ONE-MS*MS/SCM)*(ONE-MS*MS/SCM)+MS*MS*WT*WT/SCM/SCM
       A=DSQRT(A2)
       COSTHE=-(ONE-MS*MS/SCM)/A
       THE=DACOS(COSTHE)
       SINTHE=DSIN(THE)
       AA=DATAN((DXMAX+A*COSTHE)/(A*SINTHE))-PI/TWO+THE
       BB=DLOG((DXMAX*DXMAX+TWO*DXMAX*A*COSTHE+A2)/A2)
       INTEGRAL3=BB/TWO-AA/DTAN(THE)
      ENDIF
      RETURN
      END

*...integral for K^2 term bremsstrahlung
      !*****************************!
      FUNCTION INTEGRAL4(J,XMIN,XMAX)
      !*****************************!
      IMPLICIT NONE
      INTEGER J,IPAMT(100)
      REAL INTEGRAL0,INTEGRAL4,XMIN,XMAX
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      DOUBLE PRECISION A,B,C,PI,ONE,TWO,FUR,SCM,MS,WT,OUT
      DOUBLE PRECISION DXMIN,DXMAX,TRUP,T,TRDW,DINT
      DOUBLE PRECISION A2,THE,COSTHE,SINTHE,AA,BB
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /RESDATA1/,/PARAMET/
      EXTERNAL INTEGRAL0

*...constants
      ONE=1D00
      TWO=2D00
      FUR=4D00
      SCM=RPAMT(4)
      PI=RPAMT(55)
      MS=MRS(J)
      WT=WTT(J)
      DXMIN=XMIN
      DXMAX=XMAX

*...integral
      IF(IPAMT(58).EQ.1)THEN
       A=ONE
       B=-TWO*(ONE-MS*MS/SCM)
       C=B*B/FUR+MS*MS*WT*WT/SCM/SCM
       T=B*B-FUR*A*C
       TRUP=TWO*A*DXMAX-B*DLOG(A*DXMAX*DXMAX+B*DXMAX+C)
       TRDW=TWO*A*DXMIN-B*DLOG(A*DXMIN*DXMIN+B*DXMIN+C)
       OUT=INTEGRAL0(J,XMIN,XMAX)
       DINT=(B*B-TWO*A*C)*OUT
       INTEGRAL4=(TRUP-TRDW+DINT)/TWO/A/A
      ELSE
       A2=(ONE-MS*MS/SCM)*(ONE-MS*MS/SCM)+MS*MS*WT*WT/SCM/SCM
       A=DSQRT(A2)
       COSTHE=-(ONE-MS*MS/SCM)/A
       THE=DACOS(COSTHE)
       SINTHE=DSIN(THE)
       AA=DATAN((DXMAX+A*COSTHE)/(A*SINTHE))-PI/TWO+THE
       BB=DLOG((DXMAX*DXMAX+TWO*DXMAX*A*COSTHE+A2)/A2)
       INTEGRAL4=DXMAX-TWO*A*COSTHE*(BB/TWO-AA/DTAN(THE))
     &          -A*AA/SINTHE
      ENDIF
      RETURN
      END

*...integral for K^3 term bremsstrahlung
      !*****************************!
      FUNCTION INTEGRAL5(J,XMIN,XMAX)
      !*****************************!
      IMPLICIT NONE
      INTEGER J,IPAMT(100)
      REAL INTEGRAL3,INTEGRAL4,INTEGRAL5,XMIN,XMAX
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      REAL A,B,C,ONE,TWO,FUR,OUT1,OUT2,SCM,MS,WT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /RESDATA1/,/PARAMET/
      EXTERNAL INTEGRAL3,INTEGRAL4

*...constants
      ONE=1.0
      TWO=2.0
      FUR=4.0
      SCM=RPAMT(4)
      MS=MRS(J)
      WT=WTT(J)
      A=ONE
      B=-TWO*(ONE-MS*MS/SCM)
      C=B*B/FUR+MS*MS*WT*WT/SCM/SCM

*...integral
      OUT1=INTEGRAL3(J,XMIN,XMAX)
      OUT2=INTEGRAL4(J,XMIN,XMAX)
      INTEGRAL5=((XMAX*XMAX-XMIN*XMIN)/TWO-C*OUT1-B*OUT2)/A
      RETURN
      END

*...integral for K^4 term bremsstrahlung
      !*****************************!
      FUNCTION INTEGRAL6(J,XMIN,XMAX)
      !*****************************!
      IMPLICIT NONE
      INTEGER J,IPAMT(100)
      REAL INTEGRAL4,INTEGRAL5,INTEGRAL6,XMIN,XMAX
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      DOUBLE PRECISION T,A,B,C,ONE,TWO,THR,FUR
      DOUBLE PRECISION DXMIN,DXMAX,SCM,MS,WT,OUT1,OUT2
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /RESDATA1/,/PARAMET/
      EXTERNAL INTEGRAL4,INTEGRAL5

*...constants
      ONE=1D00
      TWO=2D00
      THR=3D00
      FUR=4D00
      SCM=RPAMT(4)
      MS=MRS(J)
      WT=WTT(J)
      DXMIN=XMIN
      DXMAX=XMAX
      A=ONE
      B=-TWO*(ONE-MS*MS/SCM)
      C=B*B/FUR+MS*MS*WT*WT/SCM/SCM
      T=B*B-FUR*A*C

*...integral
      OUT1=INTEGRAL4(J,XMIN,XMAX)
      OUT2=INTEGRAL5(J,XMIN,XMAX)
      INTEGRAL6=(DXMAX*DXMAX*DXMAX-DXMIN*DXMIN*DXMIN)/THR/A
     &         -C/A*OUT1-B/A*OUT2
      RETURN
      END

      !***************************************!
      DOUBLE PRECISION FUNCTION CONHAM(I,J,SCM)
      !***************************************!
      INTEGER IPAMT(100)
      REAL MRS(14),WEE(14),WHD(14),WTT(14)
      REAL RSTH(14),GMT(14),RPAMT(100)
      DOUBLE PRECISION MSI,MSJ,WEI,WEJ,WHI,WHJ,WTI,WTJ
      DOUBLE PRECISION THR,ETY,FI,FJ,GI,GJ,HI,HJ,S,A,TEN
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA4/RSTH,GMT,RINT
      SAVE /PARAMET/,/RESDATA1/,/RESDATA4/
      CONHAM=0.0D00
      STH=RSTH(12)*RSTH(12)
      IF(SCM.LT.STH)RETURN
      THR=3.0D00
      ETY=1.8D01
      TEN=1.0D01
      S=SCM
      A=RPAMT(52)
      MSI=MRS(I)
      MSJ=MRS(J)
      WEI=WEE(I)
      WEJ=WEE(J)
      WTI=WTT(I)
      WTJ=WTT(J)
      WHI=WHD(I)
      WHJ=WHD(J)
      IF(IPAMT(7).EQ.2)THEN
       WTI=WTTJSCM(I,SCM)
       WTJ=WTTJSCM(J,SCM)
       WHI=WTI-THR*WEI
       WHJ=WTJ-THR*WEJ 
      ENDIF
      IF(WHI.LE.0.0D0)RETURN
      GI=S-MSI*MSI
      HI=MSI*WTI
      FI=MSI*DSQRT(WEI*WHI)/(GI*GI+HI*HI)
      GJ=S-MSJ*MSJ
      HJ=MSJ*WTJ
      FJ=MSJ*DSQRT(WEJ*WHJ)/(GJ*GJ+HJ*HJ)
      CONHAM=ETY*FI*FJ*(GI*GJ+HI*HJ)/A/A 
      RETURN
      END

      !*************************************!
      DOUBLE PRECISION FUNCTION PHICST(THE,T)
      !*************************************!
      DOUBLE PRECISION ONE,TINY,PI,THE,T,F
      TINY=1.0D-20
      PI=3.1415926D00
      ONE=1.0D00
      IF(THE.LT.TINY)THEN
       F=ONE-T
      ELSE
       F=DSIN(THE*(ONE-T))/DSIN(THE)
      ENDIF
      PHICST=PI*T*F/DSIN(PI*T)
      RETURN 
      END 

      !*************************************!
      DOUBLE PRECISION FUNCTION PSICST(THE,T)
      !*************************************!
      DOUBLE PRECISION ONE,TINY,PI,THE,T,F
      ONE=1D00
      TINY=1D-20
      PI=3.1415926D00
      IF(THE.LT.TINY)THEN
       F=ONE-T
      ELSE
       F=DSIN(THE*(ONE-T))/DSIN(THE)
      ENDIF
      PSICST=PI*F/DSIN(T*PI)
      RETURN 
      END 

      !****************!
      FUNCTION RRES(SCM)
      !****************!
      INTEGER IPAMT(100),NAROW(14)
      REAL RPAMT(100),MRS(14),WEE(14),WHD(14),WTT(14)
      REAL MSR(14),WER(14),WHR(14),WTR(14),RSTH(14)
      REAL A(14),B(14),GMT(14),MS
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      SAVE /PARAMET/,/RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      EM=RPAMT(47)
      AEM=RPAMT(52)
      W=SQRT(SCM)
      ZRO=0.0
      HLF=0.5
      ONE=1.0
      TWO=2.0
      THR=3.0
      FUR=4.0
      TEN=10.0
      SUM=0.0
      AB=0.0
      RRES=0.0
      IF(W.LT.TWO*EM)RETURN

*...incoherent terms
      I6=IPAMT(6)
      IF(IPAMT(59).EQ.3)THEN
       IRES=12 !only wide resonances
      ELSE
       IRES=1
      ENDIF
      DO J=IRES,NRES
       F=0.0
       MS=MRS(J)
       WE=WEE(J)
       WT=WTT(J)
       WH=WHD(J)
       IF(IPAMT(7).EQ.2.AND.J.GE.11)THEN
        WT=WTTJSCM(J,SCM)
        WH=WT-THR*WE
       ENDIF
       IF(WT.GT.TEN*WE)THEN
        XF=ONE-RSTH(J)*RSTH(J)/SCM
        IF((I6.LE.0).OR.(I6.GE.1.AND.XF.GT.ZRO))F=1.0
        IF(IPAMT(2).EQ.1)THEN   !RL
         IF(W.LE.HLF)THEN
          WM=SCM
 	 ELSE 
	  WM=MS*MS
	 ENDIF
         UP=WM*WE*WH
         DW1=(SCM-MS*MS)*(SCM-MS*MS)
         DW1=(W+MS)*(W+MS)*(W-MS)*(W-MS)
         DW2=MS*MS*WT*WT
        ELSE                    !NRL
         UP=WE*WH
         DW1=FUR*(W-MS)*(W-MS)
         DW2=WT*WT
        ENDIF   
        SUM=SUM+F*UP/(DW1+DW2)
       ENDIF   
      ENDDO
      IF(IPAMT(3).EQ.0)GOTO 99

*...coherent terms of psi(4040,4140,4415)
      I6=IPAMT(6)
      IF(IPAMT(2).EQ.1)THEN   !RL
       DO J=11,14
        F=0.0 
        MS=MRS(J)
        WE=WEE(J)    
        WT=WTT(J)
        WH=WHD(J)
        IF(IPAMT(7).EQ.2)THEN
         WH=WTTJSCM(J,SCM)
         WT=WH+THR*WE
        ENDIF
        IF(WH.GT.ZRO)THEN
         XF=ONE-RSTH(J)*RSTH(J)/SCM
	 IF((I6.LE.0).OR.(I6.GE.1.AND.XF.GT.ZRO))F=1.0
         IF(W.LE.HLF)THEN
          WM=W
         ELSE 
          WM=MS
         ENDIF
         D=(W+MS)*(W-MS)
         X=WM*SQRT(TWO*WE*WH)
         Y=D*D+MS*MS*WT*WT
         A(J)=F*X*D/Y
         B(J)=F*X*MS*WT/Y
        ELSE
         A(J)=ZRO
         B(J)=ZRO
        ENDIF
       ENDDO
      ELSE                    !NRL
       DO J=11,14
        MS=MRS(J)
        WE=WEE(J)
        WT=WTT(J)
        WH=WHD(J)
        IF(IPAMT(7).EQ.2)THEN
         WH=WTTJSCM(J,SCM)
         WT=WH+THR*WE
        ENDIF
        IF(WH.GT.ZRO)THEN
         D=W-MS
         X=SQRT(WE*WH/TWO)
         Y=D*D+WT*WT/FUR
         A(J)=F*X*D/Y
         B(J)=F*X*WT/TWO/Y
        ELSE
         A(J)=ZRO
         B(J)=ZRO
        ENDIF
       ENDDO
      ENDIF
      AIJ=A(12)*A(13)+A(13)*A(14)+A(14)*A(12)
      BIJ=B(12)*B(13)+B(13)*B(14)+B(14)*B(12)
      AB=AIJ+BIJ
   99 CONTINUE
      RRES=9.0/AEM/AEM*(SUM+AB)
      IF(RRES.LT.ZRO)RRES=ZRO !cross section must be >0
      END

*...bremmstrulung function (k)
      !***********************!
      FUNCTION XEPECTRUM(ECM,X)
      !***********************!
      INTEGER IPAMT(100)
      REAL RPAMT(100)
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /PARAMET/
      EXTERNAL RQCD,RRES
      ONE=1.0
      TWO=2.0
      C=86.8
      SCM=ECM*ECM
      S=SCM*(1-X)
      RC=RQCD(S)
      RR=RRES(S)
      XEPECTRUM=(ONE-X+X*X/TWO)/X/(ONE-X)*(RC+RR)
      RETURN
      END

*...bremmstrulung function (cos_theta)
      !******************************!
      FUNCTION COSSPECTRUM(ECM,COSTHE)
      !******************************!
      DOUBLE PRECISION ONE,FOU,SCM,P47,DCOSTHE,SINTHE2,AS,DW
      INTEGER IPAMT(100)
      REAL RPAMT(100)
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /PARAMET/
      ONE=1.0D00
      FOU=4.0D00
      SCM=ECM*ECM
      P47=RPAMT(47)
      DCOSTHE=COSTHE
      SINTHE2=ONE-COSTHE*COSTHE
      AS=ONE-FOU*P47*P47/SCM
      DW=ONE-AS*DCOSTHE*DCOSTHE
      OUT=SINTHE2/DW/DW
      COSSPECTRUM=RPAMT(52)/RPAMT(55)/RPAMT(55)*OUT
      RETURN
      END

*...calculate average hadronic efficiency
      !***********************!
      SUBROUTINE EFFAVE(IE,EFF)
      !***********************!
      INTEGER IPAMT(100)
      REAL RPAMT(100),RADK(10),EFFJK(100,10)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/EFF/RADK,EFFJK
      EXTERNAL FSUBISR,RGMLTISR,RQCD,RRES
      EFF0=EFFJK(IE,1)
      NI=IPAMT(55)
      NG=IPAMT(56)
      X0=RPAMT(8)
      SCM=RPAMT(5)
      EM=RPAMT(47) 
      PIM=RPAMT(50)
      AEM=RPAMT(52)    
      PI=RPAMT(55)
      ONE=1.0
      TWO=2.0
      THR=3.0
      FUR=4.0
      RC=RQCD(SCM)
      RR=RRES(SCM)
      R=RC+RR
      T=TWO*AEM/PI*(ALOG(SCM/EM/EM)-ONE)
      X0T=X0**T
      OUT0=X0T*(ONE-X0*T/(T+ONE)+X0*X0*T/TWO/(T+TWO))*R

*...integrations of Rcon    
      IPAMT(59)=5  !Rcon no eff
      XM=ONE-FUR*PIM*PIM/SCM
      OUT1=RGMLTISR(FSUBISR,X0,XM,NI,NG)

      IPAMT(59)=6  !Rcon with eff
      XM=ONE-FUR*PIM*PIM/SCM 
      OUT2=RGMLTISR(FSUBISR,X0,XM,NI,NG)

*...integrations of Rres
      IPAMT(59)=7  !Rres no eff
      XM=ONE-FUR*EM*EM/SCM
      OUT3=RGMLTISR(FSUBISR,X0,XM,NI,NG)

      IPAMT(59)=8  !Rres with eff
      XM=ONE-FUR*EM*EM/SCM
      OUT4=RGMLTISR(FSUBISR,X0,XM,NI,NG)

*...effave
      EFF=(EFF0*OUT0+T*(OUT2+OUT4))/(OUT0+T*(OUT1+OUT3))
      END

*...soft and hard cross cection
      !*******************************!
      SUBROUTINE SIGMVSH(SIGMVS,SIGMHD)
      !*******************************!
      INTEGER IPAMT(100)
      REAL RPAMT(100)
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /PARAMET/
      EXTERNAL FSUBISR,RGMLTISR

*...constants
      ONE=1
      TWO=2
      SCM=RPAMT(4)
      R=RPAMT(13)
      SIGMAMU=86.8/RPAMT(1)/RPAMT(1)
      X0=RPAMT(8)
      XM=ONE-TWO*TWO*RPAMT(50)*RPAMT(50)/RPAMT(1)/RPAMT(1)
      T=TWO*RPAMT(52)/RPAMT(55)*(ALOG(SCM/RPAMT(48)/RPAMT(48))-ONE)
      IF(IPAMT(62).EQ.2)GOTO 20

*...Born and virture and soft cross cection of non-exponential
      SIGMVS=SIGMAMU*R*(ONE+T*ALOG(X0)+RPAMT(12))

*...hard bremsstalung cross section of non-exponential
      IPAMT(59)=10
      SIGMHD=SIGMAMU*R*T*RGMLTISR(FSUBISR,X0,XM,IPAMT(55),IPAMT(56))
      RETURN                

*...Born and virture and soft cross cection of exponential
   20 PART1=T*T0*X0**T
      PRT2=X0*X0/TWO/(T+TWO)-X0/(T+1)
      SIGMVS=SIGMAMU*R*(PART1*PART2+XM**T+RPAMT(12)+X0**T-XM**T)

*...hard bremsstalung cross section of exponential
      IPAMT(59)=11
      SIGMHD=SIGMAMU*T*RGMLTISR(FSUBISR,X0,XM,IPAMT(55),IPAMT(56))
      END

*...integrand functions
      !************************!
      SUBROUTINE FSUBISR(M,U,GF)
      !************************!
      INTEGER IPAMT(100),NAROW(14)
      REAL RPAMT(100),GF(*),U(*),RQCD,FIT
      REAL MRS(14),WEE(14),WHD(14),WTT(14)
      DOUBLE PRECISION C12,C23,C31,CONHAM
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA3/NRES,NAROW
      COMMON/PARAMET/IPAMT,RPAMT
      SAVE /PARAMET/
      EXTERNAL RQCD,RRES,FIT
      ECM=RPAMT(2)
      SCM=RPAMT(5)
      RS=RPAMT(13)
      EM=RPAMT(47)
      A=RPAMT(52)
      PI=RPAMT(55)
      ONE=1.0
      TWO=2.0
      EFFX=ONE
      T=TWO*A/PI*(ALOG(SCM/EM/EM)-ONE)

*...integrand functions
      IF(IPAMT(59).EQ.1)THEN
       F1=RQCD(SCM)/SCM
       DO L=1,M  
        S=U(L)
        F2=RQCD(S)/S
        GF(L)=(F2-F1)/(S-SCM)
       ENDDO
      ELSEIF(IPAMT(59).EQ.2)THEN
       DO L=1,M
        X=U(L)
        S=SCM*(ONE-X)
        IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)EFFX=FIT(3,X)
        GF(L)=X**T/(X*(ONE-X))*(ONE-X+X*X/TWO)*EFFX*RQCD(S)
       ENDDO
      ELSEIF(IPAMT(59).EQ.3)THEN
       DO L=1,M
        X=U(L)
        S=SCM*(ONE-X)
        IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)EFFX=FIT(3,X)
        GF(L)=(X**T)/X*(ONE-X+X*X/TWO)*EFFX*RRES(S)
       ENDDO
      ELSEIF(IPAMT(59).EQ.4)THEN
       DO L=1,M
        X=U(L)
        S=SCM*(ONE-X)
        C12=CONHAM(12,13,S)
        C23=CONHAM(13,14,S)
        C31=CONHAM(14,12,S)
        C=C12+C23+C31
        GF(L)=X**T/(X*(ONE-X))*(ONE-X+X*X/TWO)*C
       ENDDO
      ELSEIF(IPAMT(59).EQ.5)THEN
       DO L=1,M
        X=U(L)
        S=SCM*(ONE-X)
        GF(L)=X**T/(X*(ONE-X))*(ONE-X+X*X/TWO)*RQCD(S)
       ENDDO
      ELSEIF(IPAMT(59).EQ.6)THEN
       DO L=1,M
        X=U(L) 
        S=SCM*(ONE-X)
        GF(L)=X**T/(X*(ONE-X))*(ONE-X+X*X/TWO)*RRES(S)
       ENDDO
      ELSEIF(IPAMT(59).EQ.7)THEN
       DO L=1,M
        X=U(L)
        S=SCM*(ONE-X)
        IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)EFFX=FIT(3,X)
	GF(L)=X**T/(X*(ONE-X))*(ONE-X+X*X/TWO)*EFFX*RQCD(S)
       ENDDO
      ELSEIF(IPAMT(59).EQ.8)THEN
       DO L=1,M
        X=U(L) 
        S=SCM*(ONE-X)
        IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)EFFX=FIT(3,X)
        GF(L)=X**T/(X*(ONE-X))*(ONE-X+X*X/TWO)*EFFX*RRES(S)
       ENDDO
      ELSEIF(IPAMT(59).EQ.9)THEN
       DO L=1,M   
        X=U(L) 
        S=SCM*(ONE-X)
	IF(IPAMT(9).EQ.1.AND.IPAMT(10).EQ.2)EFFX=FIT(3,X)
        GF(L)=X**T/(X*(ONE-X))*(ONE-X+X*X/TWO)*EFFX*RRES(S)
       ENDDO
      ELSEIF(IPAMT(59).EQ.10)THEN
       DO L=1,M
        X=U(L)
        S=SCM*(ONE-X)
        R=RQCD(S)+RRES(S)
        GF(L)=(ONE-X+X*X/TWO)/X/(ONE-X)*R
       ENDDO
      ELSEIF(IPAMT(59).EQ.11)THEN
       DO L=1,M
        X=U(L)
        S=SCM*(ONE-X)
        R=RQCD(S)+RRES(S)
        GF(L)=X**T/X*(ONE-X+X*X/TWO)/(ONE-X)*R
       ENDDO
      ENDIF
      END

C...R value predicted by QCD
      !****************!
      FUNCTION RPQCD(SCM)
      !****************!
      INTEGER N,NF,NC,NFQ(6)
      REAL PI,ECM,S,LAM,AS,MPI,QF(10),MF(10),MFI,MP,ASPI,ONE
      DATA (QF(I),I=1,6)/-1.0,+2.0,-1.0,+2.0,-1.0,+2.0/
      DATA (MF(I),I=1,6)/0.32,0.32,0.550,1.80,5.20,174.0/
      ECM=SQRT(SCM)
      IF(ECM.LT.1.0)THEN
       RQCD=0.5
       RETURN
      ENDIF
      PI=3.1415926
      A=0.007297353
      S=ECM*ECM
      E=0.5*ECM
      MP=0.13956995
      ECMTH=2.0*MP
      LAM=0.280  
      MPI=0.13956995
      ONE=1.0   
      HALF=0.5  
      ONON=1.1
      ONSIX=1.6  
      NC=3
      N=6
      DO I=1,N 
       NFQ(I)=INT(E/MF(I))
      ENDDO
      DO I=N,1,-1
       IF(NFQ(I).GE.1)THEN
        NF=I   
        GOTO 10
       ENDIF   
      ENDDO
   10 CONTINUE 
      AS=12.0*PI/(33.0-2.0*NF)/LOG(S/LAM/LAM)
      ASPI=AS/PI
      SUMQ=0.0
      Q2SUM=0.0
      DO I=1,NF
       QFI=QF(I)/3.0
       MFI=MF(I)
       STH=4.0*MFI*MFI
       IF(S.GT.STH)THEN
        Q2SUM=Q2SUM+QFI*QFI
        SUMQ=SUMQ+QFI
       ENDIF
      ENDDO
      SUMQ2=SUMQ*SUMQ
      RF=0.0
      DO I=1,NF
       QFI=QF(I)/3.0
       MFI=MF(I)
       STH=4.0*MFI*MFI
       IF(S.GT.STH)THEN
        BI=SQRT(1.0-STH/S)
        F0=BI*(1.0+0.5*STH/S)
        RF=RF+QFI*QFI*F0
       ENDIF
      ENDDO
      R1=1.9857-0.1153*NF
      R2=-6.6368-1.2001*NF-1.2395*SUMQ2/Q2SUM/3.0
      PASPI=1.0+ASPI+R1*ASPI**2+R2*ASPI**3
      PRQCD=NC*RF*PASPI
      RETURN
      END

      !**************!
      FUNCTION RQCD(S)
      !**************!
      INTEGER N,NF,NC,NFQ(6)
      REAL PI,ECM,S,LAM,AS,QF(10),MF(10),MFI,MP
      COMMON/NF/NF
      DATA (QF(I),I=1,6)/-1.0,+2.0,-1.0,+2.0,-1.0,+2.0/
      DATA (MF(I),I=1,6)/0.325,0.325,0.60,1.867,5.20,174.0/
      PI=3.1415926
      A=0.007297353
      ECM=SQRT(S)
      E=0.5*ECM
      MP=0.14
      ECMTH=2.0*MP
      ONE=1.0
      HALF=0.5
      ONSIX=1.6
      NC=3
      N=6
      NF=0
      RQCD=0.0
      DO I=1,6
       NFQ(I)=0
      ENDDO
      IF(ECM.LT.ECMTH)THEN
       RQCD=0.0
       NF=0
      ELSEIF(ECM.GT.ECMTH.AND.ECM.LT.HALF)THEN
       RQCD=FIT(1,S)
       NF=2
      ELSEIF(ECM.GT.HALF.AND.ECM.LT.ONSIX)THEN
       RQCD=FIT(2,S)
       NF=2
      ELSEIF(ECM.GE.ONSIX)THEN
       DO I=1,N
        NFQ(I)=INT(E/MF(I))
       ENDDO
       DO I=N,1,-1
        IF(NFQ(I).GE.1)THEN
         NF=I
         GOTO 10
        ENDIF
       ENDDO
   10  CONTINUE
       IF(NF.LE.0)STOP
       IF(NF.LE.3)THEN
        LAM=1.01
       ELSEIF(NF.EQ.4)THEN
        LAM=0.8 
       ELSE
        LAM=0.50
       ENDIF
       RNF=NF
       AS=12.0*PI/(33.0-2.0*NF)/ALOG(S/LAM/LAM)
       AS=RUN_ALPHAS(0.118,91.2,5.0,ECM,RNF)
       ASPI=AS/PI
       SUMQ=0.0
       Q2SUM=0.0
       DO I=1,NF
        QFI=QF(I)/3.0
        MFI=MF(I)
        STH=4.0*MFI*MFI
        IF(S.GT.STH)THEN
         Q2SUM=Q2SUM+QFI*QFI
         SUMQ=SUMQ+QFI
        ENDIF
       ENDDO
       SUMQ2=SUMQ*SUMQ
       RF=0.0
       DO I=1,NF
        QFI=QF(I)/3.0
        MFI=MF(I)
        STH=4.0*MFI*MFI
        IF(S.GT.STH)THEN
         BI=SQRT(1.0-STH/S)
         F0=BI*(1.0+0.5*STH/S)
         RF=RF+QFI*QFI*F0
        ENDIF
       ENDDO
       R1=1.9857-0.1153*NF
       R2=-6.6368-1.2001*NF-1.2395*SUMQ2/Q2SUM/3.0
       PASPI=1.0+ASPI+R1*ASPI**2+R2*ASPI**3
       RQCD=NC*RF*PASPI
      ENDIF
      IF(RQCD.LT.0)RQCD=0
      RETURN
      END

*...kenametic lambda(s,m1^2,m2^2)
      !**********************!
      FUNCTION LAMBDA(W,M1,M2)
      !**********************!
      REAL M1,M2,LAMBDA
      LAMBDA=0.0
      IF(W.LE.(M1+M2))RETURN
      F1=(W+M1+M2)*(W-M1-M2)
      F2=(W+M1-M2)*(W-M1+M2)
      LAMBDA=SQRT(F1*F2)
      RETURN
      END   

*...BL function for L partial wave
      !***************!
      FUNCTION BLZ(L,Z)
      !***************!
      IF(L.EQ.0)THEN
       BLZ=1.0
      ELSEIF(L.EQ.1)THEN
       BLZ=1.0+Z*Z
      ELSEIF(L.EQ.2)THEN
       BLZ=9.0+3.0*Z*Z+Z*Z*Z*Z
      ELSEIF(L.EQ.3)THEN
       BLZ=225.0+45.0*Z*Z+6.0*Z*Z*Z*Z+Z*Z*Z*Z*Z*Z
      ENDIF
      RETURN
      END

*...Breit-Wigner cross section (in R) for single resonance
      !******************!
      FUNCTION RBWJ(J,SCM)
      !******************!
      INTEGER NAROW(14),IPAMT(100)
      REAL MRS(14),WEE(14),WHD(14),WTT(14)
      REAL RPAMT(100),RSTH(14),GMT(14)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      SAVE /RESDATA1/,/RESDATA3/,/RESDATA4/
      W=SQRT(SCM)
      IF(IPAMT(2).EQ.2)THEN  !NRL
       A=WEE(J)*WHD(J)
       B=W-MRS(J)
       C=WTT(J)/2.0
       RBWJ=2.5/RPAMT(52)/RPAMT(52)*A/(B*B+C*C)
      ELSE                   !RL
       A=WEE(J)*WHD(J)
       B=SCM-MRS(J)*MRS(J)
       C=MRS(J)*WTT(J)
       RBWJ=9*SCM/RPAMT(52)/RPAMT(52)*A/(B*B+C*C)
      ENDIF
      END

*...energy dependent resonant total widths
      !**********************!
      FUNCTION WTTJSCM(IDEX,S)
      !**********************!
      INTEGER NAROW(14),IPAMT(100)
      REAL MRS(14),WEE(14),WHD(14),WTT(14)
      REAL MSR(14),WER(14),WHR(14),WTR(14)
      REAL RPAMT(100),RSTH(14),GMT(14),M1,M2,M,LAMBDA
      EXTERNAL LAMBDA,BLZ
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      SAVE /RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      R=RINT
      G=GMT(IDEX)
      W=SQRT(S)
      ZRO=0.0
      TWO=2.0
      WTTJSCM=0.0

*...s-independent widths
      IF(IDEX.LE.10)THEN
       WTTJSCM=WTT(IDEX)
       RETURN
      ENDIF

*...s-dependent withs for psi(3770,4040,4160,4416)
      M=RPAMT(71)               !=> D0 D~0
      P=LAMBDA(W,M,M)/TWO/W    
      Z=R*P
      WT1=Z*Z*Z/BLZ(1,Z)         !L=1
      WTTJSCM=WTTJSCM+G*WT1
      M=RPAMT(72)               !=> D+ D-
      P=LAMBDA(W,M,M)/TWO/W      
      Z=R*P
      WT1=Z*Z*Z/BLZ(1,Z)         !L=1
      WTTJSCM=WTTJSCM+G*WT1
      IF(IDEX.LE.11)RETURN   !above for psi(3770)
      M=RPAMT(71)               !=> D*+ D*-
      P=LAMBDA(W,M,M)/TWO/W
      Z=R*P
      WT0=Z/BLZ(0,Z)             !L=0
      WT1=Z*Z*Z/BLZ(1,Z)         !L=1  
      WT2=Z*Z*Z*Z*Z/BLZ(2,Z)     !L=2
      WT3=Z*Z*Z*Z*Z*Z*Z/BLZ(3,Z) !L=3
      WTTJSCM=WTTJSCM+G*(WT0+WT1+WT2+WT3)
      M=RPAMT(73)               !=> D*0 D~*0
      P=LAMBDA(W,M,M)/TWO/W
      Z=R*P
      WT0=Z/BLZ(0,Z)             !L=0
      WT1=Z*Z*Z/BLZ(1,Z)         !L=1
      WT2=Z*Z*Z*Z*Z/BLZ(2,Z)     !L=2
      WT3=Z*Z*Z*Z*Z*Z*Z/BLZ(3,Z) !L=3
      WTTJSCM=WTTJSCM+G*(WT0+WT1+WT2+WT3)
      M1=RPAMT(72)              !=> D+- D*-+
      M2=RPAMT(74)
      P=LAMBDA(W,M1,M2)/TWO/W
      Z=R*P
      WT0=Z/BLZ(0,Z)             !L=0
      WT1=Z*Z*Z/BLZ(1,Z)         !L=1
      WT2=Z*Z*Z*Z*Z/BLZ(2,Z)     !L=2
      WTTJSCM=WTTJSCM+G*TWO*(WT0+WT1+WT2)
      M1=RPAMT(71)              !D0 D~*0 and D~0 D*0
      M2=RPAMT(73)
      P=LAMBDA(W,M1,M2)/TWO/W              
      Z=R*P
      WT0=Z/BLZ(0,Z)             !L=0
      WT1=Z*Z*Z/BLZ(1,Z)         !L=1
      WT2=Z*Z*Z*Z*Z/BLZ(2,Z)     !L=2
      WTTJSCM=WTTJSCM+G*TWO*(WT0+WT1+WT2)
      M=RPAMT(75)               !Ds+ Ds-
      P=LAMBDA(W,M,M)/TWO/W
      Z=R*P
      WT1=Z*Z*Z/BLZ(1,Z)	 !L=1  
      WTTJSCM=WTTJSCM+G*WT1
      IF(IDEX.LE.12)RETURN   !above for psi(4040)

      M1=RPAMT(75)              !Ds+- Ds*-+
      M2=RPAMT(76)
      P=LAMBDA(W,M1,M2)/TWO/W
      Z=R*P
      WT0=Z/BLZ(0,Z)             !L=0
      WT1=Z*Z*Z/BLZ(1,Z)	 !L=1
      WT2=Z*Z*Z*Z*Z/BLZ(2,Z)     !L=2
      WTTJSCM=WTTJSCM+G*TWO*(WT0+WT1+WT2)
      IF(IDEX.LE.13)RETURN   !above for psi(41600)

      M=RPAMT(76)               !Ds*+ Ds*-
      P=LAMBDA(W,M,M)/TWO/W
      Z=R*P
      WT0=Z/BLZ(0,Z)             !L=0
      WT1=Z*Z*Z/BLZ(1,Z)	 !L=1
      WT2=Z*Z*Z*Z*Z/BLZ(2,Z)     !L=2
      WT2=Z*Z*Z*Z*Z*Z*Z/BLZ(3,Z) !L=3   
      WTTJSCM=WTTJSCM+G*(WT0+WT1+WT2+WT3)
      RETURN
      END

*...X2
      !******************!
      SUBROUTINE XSQURE(D)
      !******************!
      INTEGER IPAMT(100)
      REAL RPAMT(100),ISRF
      REAL ECMI(100),LUM(100),DLUM(100),NHD(100),DNHD(100)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/BESDATA/ECMI,LUM,DLUM,NHD,DNHD
      EXTERNAL RQCD,RRES,FIT
      ZRO=0.0
      U=86.8
      J=IPAMT(61)
      W=ECMI(J)
      S=W*W
      ISRF=RPAMT(9)
      SIGMU0=U/S
      DR2=DNHD(J)*DNHD(J)+DLUM(J)*DLUM(J)
      REXP=NHD(J)/LUM(J)/SIGMU0/ISRF/FIT(3,ZRO)
      RTHE=RQCD(S)+RRES(S)
      D=(REXP-RTHE)*(REXP-RTHE)/DR2
      END  

*...store resonant related parameters
      !******************!
      SUBROUTINE RESONDATA
      !******************!
      INTEGER NAROW(14),IPAMT(100)
      REAL MRS0(14),WEE0(14),WHD0(14),WTT0(14)
      REAL MRS(14),WEE(14),WHD(14),WTT(14),RPAMT(100)
      REAL MSR(14),WER(14),WHR(14),WTR(14),RSTH(14),GMT(14)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/RESDATA1/MRS,WEE,WHD,WTT
      COMMON/RESDATA2/MSR,WER,WHR,WTR
      COMMON/RESDATA3/NRES,NAROW
      COMMON/RESDATA4/RSTH,GMT,RINT
      COMMON/RESDATA5/IDRES(14)
      SAVE /PARAMET/,/RESDATA1/,/RESDATA2/,/RESDATA3/,/RESDATA4/
      EXTERNAL GRN
      SEED=RPAMT(90)
      NRES=14
      SCM=RPAMT(5)
      ZRO=0E-20

*...PDG codes of 1-- resonances
      DATA (IDRES(J),J=1,14)/
      ! 1   2   3     4     5      6      7     8
     & 113,223,333,100223,100113,30223,100333,30113,
      ! 9   10    11     12      13      14
     & 443,30443,30443,9000443,9010443,9020443/

*...narrow resonances index
      DATA (NAROW(J),J=1,14)/
      !1  2  3  4  5  6  7  8  9 10 11 12 13 14
     & 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0/ 

*...mass of resonances (GeV):
      DATA (MRS0(J),J=1,14)/
      !  1      2      3      4      5      6      7
     & 0.7758,0.7826,1.0194,1.4200,1.4650,1.6700,1.6800,
      !   8      9     10     11     12     13     14
     & 1.7200,3.0969,3.6860,3.7699,4.0400,4.1590,4.4150/

      DATA (MSR(J),J=1,14)/
      !  1        2        3        4      5          6        7
     & 0.000800,0.000120,0.000008,0.031000,0.025000,0.013000,0.050000,
      !  8        9        10       11       12       13       14
     & 0.020000,0.000040,0.000090,0.002500,0.010000,0.020000,0.006000/

*...total width of resonances (GeV):
      DATA (WTT0(J),J=1,14)/
      !  1        2        3        4        5        6        7
     & 0.160300,0.008440,0.004458,0.200000,0.235000,0.315000,0.1500000,
      !  8        9        10       11       12       13       14
     & 0.220000,0.000088,0.000277,0.023600,0.052000,0.078000,0.043000/

      DATA (WTR(J),J=1,14)/
     & 0.001100,0.000100,0.000060,0.060000,0.060000,0.024000,0.050000,
     & 0.050000,0.000005,0.000031,0.002700,0.010000,0.020000,0.015000/

*...electronic width of resonances (GeV):
      DATA (WEE0(J),J=1,14)/
      !  1          2          3          4          5
     & 0.00000702,0.00000060,0.00000137,0.00000117,0.00000050,
      !  6          7          8          9          10
     & 0.000000001,0.000000001,0.00000010,0.00000526,0.00000214,
      !  11         12         13         14
     & 0.00000026,0.00000075,0.00000077,0.00000047/
    
      DATA (WER(J),J=1,14)/
     & 0.00000032,0.00000002,0.00000005,0.00000001,0.00000001,
     & 0.00000001,0.00000001,0.00000001,0.00000037,0.00000021,
     & 0.00000004,0.00000015,0.00000023,0.00000010/

      DATA (RSTH(J),J=1,14)/14*0.00102/

*...hadronic width of resonances:
      DO J=1,14    
       IF(J.LE.9)THEN
        WHD0(J)=WTT0(J)-2.0*WEE0(J)
       ELSE
        WHD0(J)=WTT0(J)-3.0*WEE0(J)
       ENDIF
       WHR(J)=WTR(J)
      ENDDO

*...use BES's resonant parameters
      IF(IPAMT(7).EQ.2)THEN
       RINT=1.5      !(fermi) radius of interaction
       MRS0(11)=3.7727
       MRS0(12)=4.0625
       MRS0(13)=4.1627
       MRS0(14)=4.3814
       WEE0(11)=193.00*1.0E-09 !eV  -> GeV
       WEE0(12)=1180.0*1.0E-09 !eV  -> GeV
       WEE0(13)=408.00*1.0E-09 !eV  -> GeV
       WEE0(14)=306.00*1.0E-09 !eV  -> GeV
       GMT(11)=55.320*1.0E-03  !MeV -> GeV
       GMT(12)=4.1040*1.0E-03  !MeV -> GeV
       GMT(13)=2.5560*1.0E-03  !MeV -> GeV
       GMT(14)=2.4000*1.0E-03  !MeV -> GeV
      ENDIF

*...error distribution
      IF(IPAMT(12).EQ.0)THEN
       DO J=1,14
        MRS(J)=MRS0(J)
        WEE(J)=WEE0(J)
        WHD(J)=WHD0(J)
        WTT(J)=WTT0(J)        
       ENDDO
      ELSE
       DO J=1,14
        MRS(J)=GRN(MRS0(J),MSR(J),SEED)
        WEE(J)=GRN(WEE0(J),WER(J),SEED)
        WHD(J)=GRN(WHD0(J),WHR(J),SEED)
        WTT(J)=GRN(WTT0(J),WTR(J),SEED)
        IF(MRS(J).LE.ZRO)MRS(J)=MRS0(J)
        IF(WEE(J).LE.ZRO)WEE(J)=WEE0(J)
        IF(WHD(J).LE.ZRO)WHD(J)=WHD0(J)
        IF(WTT(J).LE.ZRO)WTT(J)=WTT0(J)
        IF(
     &   MRS(J).LE.ZRO.OR.
     &   WEE(J).LE.ZRO.OR.
     &   WHD(J).LE.ZRO.OR.
     &   WTT(J).LE.ZRO.OR.
     &   WHD(J).LE.WEE(J).OR.
     &   WTT(J).LE.WHD(J).OR.
     &   WTT(J).LE.WEE(J))THEN
         MRS(J)=MRS0(J)
         WEE(J)=WEE0(J)
         WHD(J)=WHD0(J)
         WTT(J)=WTT0(J)
        ENDIF
       ENDDO
      ENDIF
      RPAMT(90)=SEED
      END

      !**********************************!
      FUNCTION RGMLTISR(FSUBISR,A,B,NI,NG)
      !**********************************!
      IMPLICIT REAL (A-H,M,O-Z)
      IMPLICIT INTEGER(I-L,N)
      INTEGER NI,M0,NG,I0,I,J,MV,K
      CHARACTER NAME*(*)
      CHARACTER*80 ERRTXT
      PARAMETER (NAME='RGMLTISR')
      PARAMETER (Z1=1, HALF=Z1/2)
      DIMENSION W(14),T(14),V(64),U(100000),F(100000)
      EXTERNAL FSUBISR
      DATA (T(I),W(I),I=1,14)
     1/-0.93246 95142 03152 028D0, 0.17132 44923 79170 345D0,
     2 -0.66120 93864 66264 514D0, 0.36076 15730 48138 608D0,
     3 -0.23861 91860 83196 909D0, 0.46791 39345 72691 047D0,
     4  0.23861 91860 83196 909D0, 0.46791 39345 72691 047D0,
     5  0.66120 93864 66264 514D0, 0.36076 15730 48138 608D0,
     6  0.93246 95142 03152 028D0, 0.17132 44923 79170 345D0,
     7 -0.96028 98564 97536 232D0, 0.10122 85362 90376 259D0,
     8 -0.79666 64774 13626 740D0, 0.22238 10344 53374 471D0,
     9 -0.52553 24099 16328 986D0, 0.31370 66458 77887 287D0,
     A -0.18343 46424 95649 805D0, 0.36268 37833 78361 983D0,
     B  0.18343 46424 95649 805D0, 0.36268 37833 78361 983D0,
     C  0.52553 24099 16328 986D0, 0.31370 66458 77887 287D0,
     D  0.79666 64774 13626 740D0, 0.22238 10344 53374 471D0,
     E  0.96028 98564 97536 232D0, 0.10122 85362 90376 259D0/
      IF(NI .LE. 0) THEN
       WRITE(ERRTXT,101) NI
       !CALL MTLPRT(NAME,'D110.1',ERRTXT)
       STOP
      END IF
      M0=NG
      IF(M0 .NE. 8) M0=6
      I0=0
      IF(M0 .EQ. 8) I0=6
      D=(B-A)/NI
      R=HALF*D
      RA=R+A
      MV=MOD(M0*NI-1,64)+1
      S=0
      J=0
      DO 1 I = 1+I0,M0+I0
      RTA=R*T(I)+RA
      DO 2 K = 1,NI
      J=J+1
      V(J)=W(I)
      U(J)=RTA+(K-1)*D
      IF(J .EQ. MV) THEN
       CALL FSUBISR(MV,U,F)
       DO 3 J = 1,MV
    3  S=S+V(J)*F(J)
       MV=64
       J=0
      END IF
    2 CONTINUE
    1 CONTINUE
      RGMLTISR=R*S
      RETURN
  101 FORMAT('N1 = ',I4,' <= 0')
      END

      !*****************!
      FUNCTION GRN(E,G,R)
      !*****************!
      S=65536
      U=2053 
      V=13849
      M=R/S  
      R=R-M*S
      R=U*R+V
      M=R/S  
      R=R-M*S
      RN=R/S 
      T=0    
      DO I=1,12
       T=T+RN  
      ENDDO    
      GRN=E+G*(T-6)
      RETURN
      END   

      !******************!
      FUNCTION FIT(IFIT,X)
      !******************!
      INTEGER I,N,M,IPAMT(100)
      REAL ECM1(20),ECM2(20),S1(20),S2(20),R1(20),R2(20)
      REAL RPAMT(100),RADK(10),EFF(100,10),EFFK(10),A(10)
      REAL ECMI(100),LUM(100),DLUM(100),NHD(100),DNHD(100)
      REAL FIT,CEFF(4)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/BESDATA/ECMI,LUM,DLUM,NHD,DNHD
      COMMON/EFF/RADK,EFF
      COMMON/EFFCOFIE/CEFF
      IFITW=IPAMT(60)
      ZRO=0.0

      IF(IFIT.EQ.1)THEN
       N=5 !number of data
       DATA (ECM1(I),I=1,5)/0.28,0.2,0.3,0.4,0.5/
       DATA (R1(I),I=1,5)/0.0,0.05,0.1,0.15,0.48/
       SQRTX=SQRT(X)
       IF(SQRTX.LE.ECM1(1))THEN
        FIT=0.0
       ELSEIF(ECM1(1).LT.SQRTX.AND.SQRTX.LT.ECM1(2))THEN
	X1=ECM1(1) !*ECM1(1)
	X2=ECM1(2) !*ECM1(2)
        FIT=R1(1)+(R1(2)-R1(1))/(X2-X1)*(SQRTX-X1)
       ELSE	
        M=3 
        S=X
        SV=0.0
        IF(IFITW.GE.1)THEN
         RN=N
         DO I=1,N
          S1(I)=ECM1(I)*ECM1(I)
          SV=SV+S1(I)
         ENDDO
         SV=SV/RN 
        ENDIF	
        CALL PCIR(S1,R1,A,N,M,DT1,DT2,DT3)
        RFIT=A(1)
        DO I=2,M
         I1=I-1
         RFIT=RFIT+A(I)*(S-SV)**I1
        ENDDO
        FIT=RFIT
        FIT=0.725*(SQRTX-0.28)/(0.5-0.28)
       ENDIF
       FIT=R1(5)/(ECM1(5)-ECM1(1))*(SQRTX-ECM1(1))
      ELSEIF(IFIT.EQ.2)THEN
       DATA (ECM2(I),I=1,11)/
     & 0.5,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6/
       DATA (R2(I),I=1,11)/
     & 0.0,0.01,0.025,0.07,0.12,0.25,0.50,0.80,1.0,1.3,1.5/
       N=10 !number of data
       M=3 !
       S=X
       SV=0.0
       IF(IFITW.GE.1)THEN
        RN=N
        DO I=1,N
         S2(I)=ECM2(I)*ECM2(I)
         SV=SV+S2(I)
        ENDDO
        SV=SV/RN
       ENDIF
       CALL PCIR(S2,R2,A,N,M,DT1,DT2,DT3)
       RFIT=A(1)
       DO I=2,M
        I1=I-1
        RFIT=RFIT+A(I)*(S-SV)**I1
       ENDDO
       FIT=RFIT
      ELSEIF(IFIT.EQ.3)THEN
       IE=IPAMT(61)
       DO I=1,10
        EFFK(I)=EFF(IE,I)
       ENDDO

       N=10 !number of data
       M=4  !must be 4
       RN=N
 
       CALL PCIR(RADK,EFFK,A,N,M,DT1,DT2,DT3)

*...fitting result
       XV=0.0
       IF(IFITW.GE.1)THEN
        DO J=1,N
         XV=XV+RADK(J)
        ENDDO
        XV=XV/RN
       ENDIF
       EFIT=A(1)
       CEFF(1)=A(1)
       DO I=2,M
        I1=I-1
        CEFF(I)=A(I)
        EFIT=EFIT+A(I)*((X-XV)**I1)
       ENDDO
       FIT=EFIT
       IF(FIT.LT.ZRO)FIT=ZRO
      ENDIF
      RETURN
      END

      !************************************!
      SUBROUTINE PCIR(X,Y,A,N,M,DT1,DT2,DT3)
      !************************************!
      INTEGER I,J,M,N,IPAMT(100)
      REAL X(N),Y(N),A(M),S(20),T(20),B(20)
      REAL Z,D1,D2,P,Q,C,G,RPAMT(100)
      COMMON/PARAMET/IPAMT,RPAMT
      IFITW=IPAMT(60)
      Z=0.0
      IF(IFITW.GE.1)THEN
       DO I=1,N
        Z=Z+X(I)
       ENDDO
       Z=Z/FLOAT(N)
      ENDIF 
      B(1)=1.0
      D1=N
      P=0.0
      C=0.0
      DO I=1,N
       P=P+(X(I)-Z)
       C=C+Y(I)
      ENDDO
      C=C/D1
      P=P/D1
      A(1)=C*B(1)
      IF (M.GT.1) THEN
       T(2)=1.0
       T(1)=-P
       D2=0.0
       C=0.0
       G=0.0
       DO I=1,N
        Q=X(I)-Z-P
        D2=D2+Q*Q
        C=Y(I)*Q+C
        G=(X(I)-Z)*Q*Q+G
       ENDDO
       C=C/D2
       P=G/D2
       Q=D2/D1
       D1=D2
       A(2)=C*T(2)
       A(1)=C*T(1)+A(1)
      ENDIF
      DO J=3,M
       S(J)=T(J-1)
       S(J-1)=-P*T(J-1)+T(J-2)
       IF (J.GE.4) THEN
        DO K=J-2,2,-1
         S(K)=-P*T(K)+T(K-1)-Q*B(K)
        ENDDO
       ENDIF
       S(1)=-P*T(1)-Q*B(1)
       D2=0.0
       C=0.0
       G=0.0
       DO I=1,N
        Q=S(J)
        DO K=J-1,1,-1
         Q=Q*(X(I)-Z)+S(K)
        ENDDO
        D2=D2+Q*Q
        C=Y(I)*Q+C
        G=(X(I)-Z)*Q*Q+G
       ENDDO
       C=C/D2
       P=G/D2
       Q=D2/D1
       D1=D2
       A(J)=C*S(J)
       T(J)=S(J)
       DO K=J-1,1,-1
        A(K)=C*S(K)+A(K)
        B(K)=T(K)
        T(K)=S(K)
       ENDDO
      ENDDO
      DT1=0.0
      DT2=0.0
      DT3=0.0
      DO I=1,N
       Q=A(M)
       DO K=M-1,1,-1
        Q=Q*(X(I)-Z)+A(K)
       ENDDO
       DT=Q-Y(I)
       IF (ABS(DT).GT.DT3) DT3=ABS(DT)
       DT1=DT1+DT*DT
       DT2=DT2+ABS(DT)
      ENDDO
      RETURN
      END

*...radiative photon sampling 
      !***************************************!
      SUBROUTINE ARLWISR(ECM,MK,PARK,THEK,PHIK)
      !***************************************!
      INTEGER IPAMT(100)
      REAL RPAMT(100)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      COMMON/PARAMET/IPAMT,RPAMT
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/RAD/XK(0:2000),FDNDK(0:2000),THE(0:2000),FDNDTHE(0:2000)
      SAVE /PARAMET/,/LUDAT01/,/RAD/,/ARLWDAT1/
      R0VS=RPAMT(14)/(RPAMT(14)+RPAMT(15))
      RHRD=RPAMT(15)/(RPAMT(14)+RPAMT(15))
      MK=0
      PARK=0
      THEK=0
      PHIK=0
      IF(RLU0(0).LT.R0VS.AND.IALFU(21).EQ.0)RETURN
      MK=1
      HLF=0.5
      NXK=IPAMT(71)
      NTHE=IPAMT(72)
      EBEAM=0.5*ECM

*...phi angle sampling
   10 PHIK=RLU0(0)*PARU(2)

*...theta angle sampling
   20 R=RLU0(0)
      DO J=0,NTHE
       FDNDTHE(J)=FDNDTHE(J)/FDNDTHE(NTHE)
      ENDDO
      DO J=1,NTHE
       IF(FDNDTHE(J-1).LE.R.AND.R.LT.FDNDTHE(J))THEN
        THEK=THE(J-1)
     &  +(THE(J)-THE(J-1))/(FDNDTHE(J)-FDNDTHE(J-1))
     &  *(R-FDNDTHE(J-1))
       ENDIF    
      ENDDO
      R=RLU0(0)
      IF(R.GT.HLF)THEK=PARU(2)-THEK
      THEMIN=-PARU(2)  !PARU(2)=2*PI=6.2831854
      THEMAX=+PARU(2)  
      IF(THEK.LE.THEMIN.OR.THEMAX.LE.THEK)GOTO 20
      
*...k sampling
      IF(IALFU(21).GE.1)THEN
       XKGAMMA=RALFU(3)
      ELSE
   30  R=RLU0(0)
       XKGAMMA=-1
       XK(0)=0
       DO J=1,NXK
        IF(FDNDK(J-1).LE.R.AND.R.LT.FDNDK(J))THEN
         XKGAMMA=XK(J-1)+(XK(J)-XK(J-1))*RLU0(0)
        ENDIF
       ENDDO
       IF(XKGAMMA.LT.PARJ(135).OR.XKGAMMA.GT.PARJ(136))GOTO 30
      ENDIF
      PARK=0.5*ECM*XKGAMMA
      END   

*...initial q0-q0bar polar angle sampling
      !**********************************!
      SUBROUTINE Q0Q0BARPOLAR(KFL,ECM,THE)
      !**********************************!
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      SAVE /LUDAT01/,/ARLWDAT1/
      ONE=1
      TWO=2
      FOU=4
      ID=ABS(KFL)
      QMASS=RALPA(76+ID) !QMASS=ULMASS0(KFL)
      QMASS=ULMASS0(KFL)
      BETA2=ONE-FOU*QMASS*QMASS/ECM/ECM
      FMAX=TWO/(TWO-BETA2)
   10 THE=RLU0(0)*PARU(1)
      COSTHE=COS(THE)
      FTHE=ONE+BETA2/(TWO-BETA2)*COSTHE*COSTHE
      R0=RLU0(0)
      R=FTHE/FMAX
      IF(R0.GT.R)GOTO 10
      END

*...QCD alpha_s
      !********************************************!
      real function run_alphas(alphas,Q0,nf0,Q1,nf1)
      !********************************************!
      implicit none
      real alphas,Q0,Q1,nf0,nf1
*
      double precision asmin,asmax,eps,zbeta,r,asQ1
      external                         zbeta,dzero
      double precision mu0,mu1
      integer  inf
      double precision logQQ,asQ0,nf
      common /aspara/  logQQ,asQ0,nf
*
      real          b4
      common /bet4/ b4
*
      double precision pi
      parameter       (pi=3.1415926535897932385D0)
*
      double precision mq(6),emq(6)
      save             mq   ,emq
*                  u       , d       , s      , c     , b     , t
      data    mq / 0.0087D0, 0.0154D0, 0.270D0, 1.25D0, 4.25D0, 173.8D0/
      data   emq / 0.0015D0, 0.0015D0, 0.030D0, 0.15D0, 0.15D0,   5.2D0/
*
      run_alphaS=0.
      if (alphas.eq.0.) then
         b4=0.
         return
      endif
      if (Q0.gt.0. .and. Q1.gt.0. .and. alphas.gt.0.) then
*** study sensitivity of alpha_s(Q) on quark mass values
*        mq(4) = mq(4)+emq(4)
*        mq(4) = mq(4)-emq(4)
*        mq(5) = mq(5)+emq(5)
*        mq(5) = mq(5)-emq(5)
         logQQ = log( (mu1/mu0)**2 )
         asQ0  = alphas
         asQ1  = alphas
         asmin = 0.001D0
         asmax = 9.99D0
         eps   = 1.D-8
         mu0   = dble(Q0)
         mu1   = dble(Q0)
         if (nf0.le.nf1) then
            do inf=int(nf0),int(nf1)-1
               asQ0  = asQ1
               mu0   = mu1
               mu1   = mq(inf+1)
               logQQ = log( (mu1/mu0)**2 )
               nf    = dble(inf)
               call dzero(asmin,asmax,asQ1,r,eps,100,zbeta)
            enddo
            asQ0  = asQ1
            mu0   = mu1
            mu1   = Q1
            logQQ = log( (mu1/mu0)**2 )
            nf    = dble(nf1)
            if (nf.eq.0.D0) nf=5.D0
            call dzero(asmin,asmax,asQ1,r,eps,100,zbeta)
         else
            do inf=int(nf0),int(nf1)+1,-1
               asQ0  = asQ1
               mu0   = mu1
               mu1   = mq(inf)
               logQQ = log( (mu1/mu0)**2 )
               nf    = dble(inf)
               call dzero(asmin,asmax,asQ1,r,eps,100,zbeta)
            enddo
            asQ0  = asQ1
            mu0   = mu1
            mu1   = Q1
            logQQ = log( (mu1/mu0)**2 )
            nf    = dble(nf1)
            if (nf.eq.0.D0) nf=5.D0
            call dzero(asmin,asmax,asQ1,r,eps,100,zbeta)
         endif
         run_alphaS=asQ1
      else
         print *,' parameter(s) out of range:'
         print *,' alphas > 0', alphas
         print *,' Q0     > 0', Q0    
         print *,' Q1     > 0', Q1    
      end if
*
      return
      end

      function beta(as)
      implicit none
      double precision beta,as
      double precision b0, b1, b2, b3
      save             b0, b1, b2, b3
      real          b4
      common /bet4/ b4
*
      double precision logQQ,asQ0,nf
      common /aspara/  logQQ,asQ0,nf
*
      double precision pi
      parameter       (pi=3.1415926535897932385D0)
*
      b0=1.D0/(12.D0*pi)*(33.D0-2.D0*nf)
      b1=1.D0/(24.D0*pi**2)*(153.D0-19.D0*nf)
      b2=1.D0/(pi**3)*(22.320D0-4.3689D0*nf+0.09404D0*nf**2)
      b3=1.D0/(pi**4)*(114.23D0-27.134D0*nf+1.5824D0*nf**2
     +                                    +0.00587D0*nf**3)

      beta=-1.D0/(as**2*(b0+b1*as+b2*as**2+b3*as**3+b4*as**4))

      return
      end

      function zbeta(as,i)
      implicit none
      double precision zbeta,as
      integer i
      
      double precision  dgauss,beta,eps
      external                 beta
*
      double precision logQQ,asQ0,nf
      common /aspara/  logQQ,asQ0,nf
*
      eps=1.D-8
      zbeta = logQQ-dgauss(beta,asQ0,as,eps)

      end

      FUNCTION DGAUSS(F,A,B,EPS)
*#include "gen/imp64.inc"
      IMPLICIT DOUBLE PRECISION  (A-H,O-Z)

      CHARACTER NAME*(*)
      PARAMETER (NAME = 'DGAUSS')

*#include "gausscod.inc"
*
      external f
      DIMENSION W(12),X(12)

      PARAMETER (Z1 = 1, HF = Z1/2, CST = 5*Z1/1000)

      DATA X( 1) /9.6028985649753623D-1/, W( 1) /1.0122853629037626D-1/
      DATA X( 2) /7.9666647741362674D-1/, W( 2) /2.2238103445337447D-1/
      DATA X( 3) /5.2553240991632899D-1/, W( 3) /3.1370664587788729D-1/
      DATA X( 4) /1.8343464249564980D-1/, W( 4) /3.6268378337836198D-1/
      DATA X( 5) /9.8940093499164993D-1/, W( 5) /2.7152459411754095D-2/
      DATA X( 6) /9.4457502307323258D-1/, W( 6) /6.2253523938647893D-2/
      DATA X( 7) /8.6563120238783174D-1/, W( 7) /9.5158511682492785D-2/
      DATA X( 8) /7.5540440835500303D-1/, W( 8) /1.2462897125553387D-1/
      DATA X( 9) /6.1787624440264375D-1/, W( 9) /1.4959598881657673D-1/
      DATA X(10) /4.5801677765722739D-1/, W(10) /1.6915651939500254D-1/
      DATA X(11) /2.8160355077925891D-1/, W(11) /1.8260341504492359D-1/
      DATA X(12) /9.5012509837637440D-2/, W(12) /1.8945061045506850D-1/

      H=0
      IF(B .EQ. A) GO TO 99
      CONST=CST/ABS(B-A)
      BB=A
    1 AA=BB
      BB=B
    2 C1=HF*(BB+AA)
      C2=HF*(BB-AA)
      S8=0
      DO 3 I = 1,4
      U=C2*X(I)
    3 S8=S8+W(I)*(F(C1+U)+F(C1-U))
      S16=0
      DO 4 I = 5,12
      U=C2*X(I)
    4 S16=S16+W(I)*(F(C1+U)+F(C1-U))
      S16=C2*S16
      IF(ABS(S16-C2*S8) .LE. EPS*(1+ABS(S16))) THEN
       H=H+S16
       IF(BB .NE. B) GO TO 1
      ELSE
       BB=C1
       IF(1+CONST*ABS(C2) .NE. 1) GO TO 2
       H=0
** *   CALL MTLPRT(NAME,'D103.1','TOO HIGH ACCURACY REQUIRED')
       PRINT *,' ***',NAME,'(D103.1): TOO HIGH ACCURACY REQUIRED'
       GO TO 99
      END IF

   99 DGAUSS=H
      RETURN
      END
 
      SUBROUTINE DZERO(A,B,X0,R,EPS,MXF,F)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C     LOGICAL MFLAG,RFLAG
 
      EXTERNAL F
      PARAMETER (ONE = 1, HALF = ONE/2)
 

      XA=MIN(A,B)
      XB=MAX(A,B)
      FA=F(XA,1)
      FB=F(XB,2)
      IF(FA*FB .GT. 0) GO TO 5
      MC=0
 
    1 X0=HALF*(XA+XB)
      R=X0-XA
      EE=EPS*(ABS(X0)+1)
      IF(R .LE. EE) GO TO 4
      F1=FA
      X1=XA
      F2=FB
      X2=XB
 
    2 FX=F(X0,2)
      MC=MC+1
      IF(MC .GT. MXF) GO TO 6
      IF(FX*FA .GT. 0) THEN
       XA=X0
       FA=FX
      ELSE
       XB=X0
       FB=FX
      END IF
 
    3 U1=F1-F2
      U2=X1-X2
      U3=F2-FX
      U4=X2-X0
      IF(U2 .EQ. 0 .OR. U4 .EQ. 0) GO TO 1
      F3=FX
      X3=X0
      U1=U1/U2
      U2=U3/U4
      CA=U1-U2
      CB=(X1+X2)*U2-(X2+X0)*U1
      CC=(X1-X0)*F1-X1*(CA*X1+CB)
      IF(CA .EQ. 0) THEN
       IF(CB .EQ. 0) GO TO 1
       X0=-CC/CB
      ELSE
       U3=CB/(2*CA)
       U4=U3*U3-CC/CA
       IF(U4 .LT. 0) GO TO 1
*ORIG  X0=-U3+SIGN(SQRT(U4),X0+U3)
       IF (X0+U3.GE.0.) X0=-U3+ABS(SQRT(U4))
       IF (X0+U3.LT.0.) X0=-U3-ABS(SQRT(U4))
*END-MOD
      END IF
      IF(X0 .LT. XA .OR. X0 .GT. XB) GO TO 1
 
      R=MIN(ABS(X0-X3),ABS(X0-X2))
      EE=EPS*(ABS(X0)+1)
      IF(R .GT. EE) THEN
       F1=F2
       X1=X2
       F2=F3
       X2=X3
       GO TO 2
      END IF
 
      FX=F(X0,2)
      IF(FX .EQ. 0) GO TO 4
      IF(FX*FA .LT. 0) THEN
       XX=X0-EE
       IF(XX .LE. XA) GO TO 4
       FF=F(XX,2)
       FB=FF
       XB=XX
      ELSE
       XX=X0+EE
       IF(XX .GE. XB) GO TO 4
       FF=F(XX,2)
       FA=FF
       XA=XX
      END IF
      IF(FX*FF .GT. 0) THEN
       MC=MC+2
       IF(MC .GT. MXF) GO TO 6
       F1=F3
       X1=X3
       F2=FX
       X2=X0
       X0=XX
       FX=FF
       GO TO 3
      END IF
 
    4 R=EE
      FF=F(X0,3)
      RETURN
C+
C    5 CALL KERMTR('C205.1',LGFILE,MFLAG,RFLAG)
C      IF(MFLAG) THEN
C       IF(LGFILE .EQ. 0) WRITE(*,100)
C       IF(LGFILE .NE. 0) WRITE(LGFILE,100)
C      END IF
C      IF(.NOT.RFLAG) CALL ABEND
    5 PRINT *,'***** DZERO ... F(A) AND F(B) HAVE THE SAME SIGN'
C-
      R=-2*(XB-XA)
      X0=0
      RETURN
C+
C    6 CALL KERMTR('C205.2',LGFILE,MFLAG,RFLAG)
C      IF(MFLAG) THEN
C       IF(LGFILE .EQ. 0) WRITE(*,101)
C       IF(LGFILE .NE. 0) WRITE(LGFILE,101)
C      END IF
C      IF(.NOT.RFLAG) CALL ABEND
    6 PRINT *,'***** DZERO ... TOO MANY FUNCTION CALLS'
C-
      R=-HALF*ABS(XB-XA)
      X0=0
      RETURN
  100 FORMAT(1X,'***** CERN C205 DZERO ... F(A) AND F(B)',
     1          ' HAVE THE SAME SIGN')
  101 FORMAT(1X,'***** CERN C205 DZERO ... TOO MANY FUNCTION CALLS')
      END

C=====================================================================
C GET FROM:                                                          =
C                                                                    =
C    http://www.thep.lu.se/tf2/staff/torbjorn/jetset/jetset7410.f    =
C                                                                    =
C=====================================================================

C********************************************************************* 
C********************************************************************* 
C*                                                                  ** 
C*                                                 December 1993    ** 
C*                                                                  ** 
C*   The Lund Monte Carlo for Jet Fragmentation and e+e- Physics    ** 
C*                                                                  ** 
C*                        JETSET version 7.4                        ** 
C*                                                                  ** 
C*                        Torbjorn Sjostrand                        ** 
C*                Department of theoretical physics 2               ** 
C*                        University of Lund                        ** 
C*               Solvegatan 14A, S-223 62 Lund, Sweden              **
C*                    E-mail torbjorn@thep.lu.se                    ** 
C*                    phone +46 - 46 - 222 48 16                    ** 
C*                                                                  ** 
C*          LUSHOW0 is written together with Mats Bengtsson         ** 
C*                                                                  **
C*   The latest program version and documentation is found on WWW   **
C*         http://thep.lu.se/tf2/staff/torbjorn/Welcome.html        **
C*                                                                  ** 
C*        Copyright Torbjorn Sjostrand and CERN, Geneva 1993        ** 
C*                                                                  ** 
C********************************************************************* 
C********************************************************************* 
C                                                                    * 
C  List of subprograms in order of appearance, with main purpose     * 
C  (S = subroutine, F = function, B = block data)                    * 
C                                                                    * 
C  S   LU1ENT0   to fill one entry (= parton or particle)             * 
C  S   LU2ENT0   to fill two entries                                  * 
C  S   LU3ENT0   to fill three entries                                * 
C  S   LU4ENT0   to fill four entries                                 * 
C  S   LUJOIN0   to connect entries with colour flow information      * 
C  S   LUGIVE0   to fill (or query) commonblock variables             * 
C  S   LUEXEC0   to administrate fragmentation and decay chain        * 
C  S   LUPREP0   to rearrange showered partons along strings          * 
C  S   LUSTRF0   to do string fragmentation of jet system             * 
C  S   LUINDF0   to do independent fragmentation of one or many jets  * 
C  S   LUDECY0   to do the decay of a particle                        * 
C  S   LUKFDI0   to select parton and hadron flavours in fragm        * 
C  S   LUPTDI0   to select transverse momenta in fragm                * 
C  S   LUZDIS0   to select longitudinal scaling variable in fragm     * 
C  S   LUSHOW0   to do timelike parton shower evolution               * 
C  S   LUBOEI0   to include Bose-Einstein effects (crudely)           * 
C  F   ULMASS0   to give the mass of a particle or parton             * 
C  S   LUNAME0   to give the name of a particle or parton             * 
C  F   LUCHGE0   to give three times the electric charge              * 
C  F   LUCOMP0   to compress standard KF flavour code to internal KC  * 
C  S   LUERRM0   to write error messages and abort faulty run         * 
C  F   ULALEM0   to give the alpha_electromagnetic value              * 
C  F   ULALPS0   to give the alpha_strong value                       * 
C  F   ULANGL0   to give the angle from known x and y components      * 
C  F   RLU0      to provide a random number generator                 * 
C  S   RLUGET0   to save the state of the random number generator     * 
C  S   RLUSET0   to set the state of the random number generator      * 
C  S   LUROBO0   to rotate and/or boost an event                      * 
C  S   LUEDIT0   to remove unwanted entries from record               * 
C  S   LULIST0   to list event record or particle data =>LULIST0      * 
C  S   LULOGO0   to write a logo for JETSET and PYTHIA                * 
C  S   LUUPDA0   to update particle data                              * 
C  F   KLU0      to provide integer-valued event information          * 
C  F   PLU0      to provide real-valued event information             * 
C  S   LUSPHE0   to perform sphericity analysis                       * 
C  S   LUTHRU0   to perform thrust analysis                           * 
C  S   LUCLUS0   to perform three-dimensional cluster analysis        * 
C  S   LUCELL0   to perform cluster analysis in (eta, phi, E_T)       * 
C  S   LUJMAS0   to give high and low jet mass of event               * 
C  S   LUFOWO0   to give Fox-Wolfram moments                          * 
C  S   LUTABU0   to analyze events, with tabular output               *                                                                     * 
C  S   LUEEVT0   to administrate the generation of an e+e- event      * 
C  S   LUXTOT0   to give the total cross-section at given CM energy   * 
C  S   LURADK0   to generate initial state photon radiation           * 
C  S   LUXKFL0   to select flavour of primary qqbar pair              * 
C  S   LUXJET0   to select (matrix element) jet multiplicity          * 
C  S   LUX3JT0   to select kinematics of three-jet event              * 
C  S   LUX4JT0   to select kinematics of four-jet event               * 
C  S   LUXDIF0   to select angular orientation of event               * 
C  S   LUONIA0   to perform generation of onium decay to gluons       * 
C                                                                    * 
C  S   LUHEPC0   to convert between /LUJETS/ and /HEPEVT/ records     * 
C  S   LUTEST0   to test the proper functioning of the package        * 
C  B   LUDATA0   to contain default values and particle data          * 
C                                                                    * 
C********************************************************************* 
 
      SUBROUTINE LU1ENT0(IP,KF,PE,THE,PHI) 
 
C...Purpose: to store one parton/particle in commonblock LUJETS. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/CHECKTAG/DECAYTAG !CHECK THE DECAY 
      real*8 DECAYTAG
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Standard checks.
      DECAYTAG=0
      MSTU(28)=0 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      IPA=MAX(1,IABS(IP)) 

      IF(IPA.GT.MSTU(4))then
         CALL LUERRM0(21, 
     &'(LU1ENT0:) writing outside LUJETS memory') 
         DECAYTAG=1
         return
      endif
      KC=LUCOMP0(KF) 
      IF(KC.EQ.0)THEN
         WRITE(*,*)'KC =',KC
         CALL LUERRM0(12,'(LU1ENT0:) unknown flavour code') 
         DECAYTAG=1
         return
      ENDIF

C...Find mass. Reset K, P and V vectors. 
      PM=0. 
      IF(MSTU(10).EQ.1) PM=P(IPA,5) 
      IF(MSTU(10).GE.2) PM=ULMASS0(KF) 
      DO 100 J=1,5 
      K(IPA,J)=0 
      P(IPA,J)=0. 
      V(IPA,J)=0. 
  100 CONTINUE 
 
C...Store parton/particle in K and P vectors. 
      K(IPA,1)=1 
      IF(IP.LT.0) K(IPA,1)=2 
      K(IPA,2)=KF 
      P(IPA,5)=PM 
      P(IPA,4)=MAX(PE,PM) 
      PA=SQRT(P(IPA,4)**2-P(IPA,5)**2) 
      P(IPA,1)=PA*SIN(THE)*COS(PHI) 
      P(IPA,2)=PA*SIN(THE)*SIN(PHI) 
      P(IPA,3)=PA*COS(THE) 
 
C...Set N. Optionally fragment/decay. 
      N=IPA 
      IF(IP.EQ.0) CALL LUEXEC0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LU2ENT0(IP,KF1,KF2,PECM) 
 
C...Purpose: to store two partons/particles in their CM frame, 
C...with the first along the +z axis. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/CHECKTAG/DECAYTAG !CHECK THE DECAY 
      real*8 DECAYTAG
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Standard checks. 
      MSTU(28)=0 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      IPA=MAX(1,IABS(IP)) 
      IF(IPA.GT.MSTU(4)-1) CALL LUERRM0(21, 
     &'(LU2ENT0:) writing outside LUJETS memory') 
      DECAYTAG=1
      return
      KC1=LUCOMP0(KF1) 
      KC2=LUCOMP0(KF2) 
      IF(KC1.EQ.0.OR.KC2.EQ.0) CALL LUERRM0(12, 
     &'(LU2ENT0:) unknown flavour code') 
 
C...Find masses. Reset K, P and V vectors. 
      PM1=0. 
      IF(MSTU(10).EQ.1) PM1=P(IPA,5) 
      IF(MSTU(10).GE.2) PM1=ULMASS0(KF1) 
      PM2=0. 
      IF(MSTU(10).EQ.1) PM2=P(IPA+1,5) 
      IF(MSTU(10).GE.2) PM2=ULMASS0(KF2) 
      DO 110 I=IPA,IPA+1 
      DO 100 J=1,5 
      K(I,J)=0 
      P(I,J)=0. 
      V(I,J)=0. 
  100 CONTINUE 
  110 CONTINUE 
 
C...Check flavours. 
      KQ1=KCHG(KC1,2)*ISIGN(1,KF1) 
      KQ2=KCHG(KC2,2)*ISIGN(1,KF2) 
      IF(MSTU(19).EQ.1) THEN 
        MSTU(19)=0 
      ELSE 
        IF(KQ1+KQ2.NE.0.AND.KQ1+KQ2.NE.4) CALL LUERRM0(2, 
     &  '(LU2ENT0:) unphysical flavour combination') 
      ENDIF 
      K(IPA,2)=KF1 
      K(IPA+1,2)=KF2 
 
C...Store partons/particles in K vectors for normal case. 
      IF(IP.GE.0) THEN 
        K(IPA,1)=1 
        IF(KQ1.NE.0.AND.KQ2.NE.0) K(IPA,1)=2 
        K(IPA+1,1)=1 
 
C...Store partons in K vectors for parton shower evolution. 
      ELSE 
        K(IPA,1)=3 
        K(IPA+1,1)=3 
        K(IPA,4)=MSTU(5)*(IPA+1) 
        K(IPA,5)=K(IPA,4) 
        K(IPA+1,4)=MSTU(5)*IPA 
        K(IPA+1,5)=K(IPA+1,4) 
      ENDIF 
 
C...Check kinematics and store partons/particles in P vectors. 
      IF(PECM.LE.PM1+PM2) CALL LUERRM0(13, 
     &'(LU2ENT0:) energy smaller than sum of masses') 
      PA=SQRT(MAX(0.,(PECM**2-PM1**2-PM2**2)**2-(2.*PM1*PM2)**2))/ 
     &(2.*PECM) 
      P(IPA,3)=PA 
      P(IPA,4)=SQRT(PM1**2+PA**2) 
      P(IPA,5)=PM1 
      P(IPA+1,3)=-PA 
      P(IPA+1,4)=SQRT(PM2**2+PA**2) 
      P(IPA+1,5)=PM2 
 
C...Set N. Optionally fragment/decay. 
      N=IPA+1 
      IF(IP.EQ.0) CALL LUEXEC0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LU3ENT0(IP,KF1,KF2,KF3,PECM,X1,X3) 
 
C...Purpose: to store three partons or particles in their CM frame, 
C...with the first along the +z axis and the third in the (x,z) 
C...plane with x > 0. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Standard checks. 
      MSTU(28)=0 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      IPA=MAX(1,IABS(IP)) 
      IF(IPA.GT.MSTU(4)-2) CALL LUERRM0(21, 
     &'(LU3ENT0:) writing outside LUJETS memory') 
      KC1=LUCOMP0(KF1) 
      KC2=LUCOMP0(KF2) 
      KC3=LUCOMP0(KF3) 
      IF(KC1.EQ.0.OR.KC2.EQ.0.OR.KC3.EQ.0) CALL LUERRM0(12, 
     &'(LU3ENT0:) unknown flavour code') 
 
C...Find masses. Reset K, P and V vectors. 
      PM1=0. 
      IF(MSTU(10).EQ.1) PM1=P(IPA,5) 
      IF(MSTU(10).GE.2) PM1=ULMASS0(KF1) 
      PM2=0. 
      IF(MSTU(10).EQ.1) PM2=P(IPA+1,5) 
      IF(MSTU(10).GE.2) PM2=ULMASS0(KF2) 
      PM3=0. 
      IF(MSTU(10).EQ.1) PM3=P(IPA+2,5) 
      IF(MSTU(10).GE.2) PM3=ULMASS0(KF3) 
      DO 110 I=IPA,IPA+2 
      DO 100 J=1,5 
      K(I,J)=0 
      P(I,J)=0. 
      V(I,J)=0. 
  100 CONTINUE 
  110 CONTINUE 
 
C...Check flavours. 
      KQ1=KCHG(KC1,2)*ISIGN(1,KF1) 
      KQ2=KCHG(KC2,2)*ISIGN(1,KF2) 
      KQ3=KCHG(KC3,2)*ISIGN(1,KF3) 
      IF(MSTU(19).EQ.1) THEN 
        MSTU(19)=0 
      ELSEIF(KQ1.EQ.0.AND.KQ2.EQ.0.AND.KQ3.EQ.0) THEN 
      ELSEIF(KQ1.NE.0.AND.KQ2.EQ.2.AND.(KQ1+KQ3.EQ.0.OR. 
     &KQ1+KQ3.EQ.4)) THEN 
      ELSE 
        CALL LUERRM0(2,'(LU3ENT0:) unphysical flavour combination') 
      ENDIF 
      K(IPA,2)=KF1 
      K(IPA+1,2)=KF2 
      K(IPA+2,2)=KF3 
 
C...Store partons/particles in K vectors for normal case. 
      IF(IP.GE.0) THEN 
        K(IPA,1)=1 
        IF(KQ1.NE.0.AND.(KQ2.NE.0.OR.KQ3.NE.0)) K(IPA,1)=2 
        K(IPA+1,1)=1 
        IF(KQ2.NE.0.AND.KQ3.NE.0) K(IPA+1,1)=2 
        K(IPA+2,1)=1 
 
C...Store partons in K vectors for parton shower evolution. 
      ELSE 
        K(IPA,1)=3 
        K(IPA+1,1)=3 
        K(IPA+2,1)=3 
        KCS=4 
        IF(KQ1.EQ.-1) KCS=5 
        K(IPA,KCS)=MSTU(5)*(IPA+1) 
        K(IPA,9-KCS)=MSTU(5)*(IPA+2) 
        K(IPA+1,KCS)=MSTU(5)*(IPA+2) 
        K(IPA+1,9-KCS)=MSTU(5)*IPA 
        K(IPA+2,KCS)=MSTU(5)*IPA 
        K(IPA+2,9-KCS)=MSTU(5)*(IPA+1) 
      ENDIF 
 
C...Check kinematics. 
      MKERR=0 
      IF(0.5*X1*PECM.LE.PM1.OR.0.5*(2.-X1-X3)*PECM.LE.PM2.OR. 
     &0.5*X3*PECM.LE.PM3) MKERR=1 
      PA1=SQRT(MAX(1E-10,(0.5*X1*PECM)**2-PM1**2)) 
      PA2=SQRT(MAX(1E-10,(0.5*(2.-X1-X3)*PECM)**2-PM2**2)) 
      PA3=SQRT(MAX(1E-10,(0.5*X3*PECM)**2-PM3**2)) 
      CTHE2=(PA3**2-PA1**2-PA2**2)/(2.*PA1*PA2) 
      CTHE3=(PA2**2-PA1**2-PA3**2)/(2.*PA1*PA3) 
      IF(ABS(CTHE2).GE.1.001.OR.ABS(CTHE3).GE.1.001) MKERR=1 
      CTHE3=MAX(-1.,MIN(1.,CTHE3)) 
      IF(MKERR.NE.0) CALL LUERRM0(13, 
     &'(LU3ENT0:) unphysical kinematical variable setup') 
 
C...Store partons/particles in P vectors. 
      P(IPA,3)=PA1 
      P(IPA,4)=SQRT(PA1**2+PM1**2) 
      P(IPA,5)=PM1 
      P(IPA+2,1)=PA3*SQRT(1.-CTHE3**2) 
      P(IPA+2,3)=PA3*CTHE3 
      P(IPA+2,4)=SQRT(PA3**2+PM3**2) 
      P(IPA+2,5)=PM3 
      P(IPA+1,1)=-P(IPA+2,1) 
      P(IPA+1,3)=-P(IPA,3)-P(IPA+2,3) 
      P(IPA+1,4)=SQRT(P(IPA+1,1)**2+P(IPA+1,3)**2+PM2**2) 
      P(IPA+1,5)=PM2 
 
C...Set N. Optionally fragment/decay. 
      N=IPA+2 
      IF(IP.EQ.0) CALL LUEXEC0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LU4ENT0(IP,KF1,KF2,KF3,KF4,PECM,X1,X2,X4,X12,X14) 
 
C...Purpose: to store four partons or particles in their CM frame, with 
C...the first along the +z axis, the last in the xz plane with x > 0 
C...and the second having y < 0 and y > 0 with equal probability. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Standard checks. 
      MSTU(28)=0 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      IPA=MAX(1,IABS(IP)) 
      IF(IPA.GT.MSTU(4)-3) CALL LUERRM0(21, 
     &'(LU4ENT0:) writing outside LUJETS momory') 
      KC1=LUCOMP0(KF1) 
      KC2=LUCOMP0(KF2) 
      KC3=LUCOMP0(KF3) 
      KC4=LUCOMP0(KF4) 
      IF(KC1.EQ.0.OR.KC2.EQ.0.OR.KC3.EQ.0.OR.KC4.EQ.0) CALL LUERRM0(12, 
     &'(LU4ENT0:) unknown flavour code') 
 
C...Find masses. Reset K, P and V vectors. 
      PM1=0. 
      IF(MSTU(10).EQ.1) PM1=P(IPA,5) 
      IF(MSTU(10).GE.2) PM1=ULMASS0(KF1) 
      PM2=0. 
      IF(MSTU(10).EQ.1) PM2=P(IPA+1,5) 
      IF(MSTU(10).GE.2) PM2=ULMASS0(KF2) 
      PM3=0. 
      IF(MSTU(10).EQ.1) PM3=P(IPA+2,5) 
      IF(MSTU(10).GE.2) PM3=ULMASS0(KF3) 
      PM4=0. 
      IF(MSTU(10).EQ.1) PM4=P(IPA+3,5) 
      IF(MSTU(10).GE.2) PM4=ULMASS0(KF4) 
      DO 110 I=IPA,IPA+3 
      DO 100 J=1,5 
      K(I,J)=0 
      P(I,J)=0. 
      V(I,J)=0. 
  100 CONTINUE 
  110 CONTINUE 
 
C...Check flavours. 
      KQ1=KCHG(KC1,2)*ISIGN(1,KF1) 
      KQ2=KCHG(KC2,2)*ISIGN(1,KF2) 
      KQ3=KCHG(KC3,2)*ISIGN(1,KF3) 
      KQ4=KCHG(KC4,2)*ISIGN(1,KF4) 
      IF(MSTU(19).EQ.1) THEN 
        MSTU(19)=0 
      ELSEIF(KQ1.EQ.0.AND.KQ2.EQ.0.AND.KQ3.EQ.0.AND.KQ4.EQ.0) THEN 
      ELSEIF(KQ1.NE.0.AND.KQ2.EQ.2.AND.KQ3.EQ.2.AND.(KQ1+KQ4.EQ.0.OR. 
     &KQ1+KQ4.EQ.4)) THEN 
      ELSEIF(KQ1.NE.0.AND.KQ1+KQ2.EQ.0.AND.KQ3.NE.0.AND.KQ3+KQ4.EQ.0.) 
     &THEN 
      ELSE 
        CALL LUERRM0(2,'(LU4ENT0:) unphysical flavour combination') 
      ENDIF 
      K(IPA,2)=KF1 
      K(IPA+1,2)=KF2 
      K(IPA+2,2)=KF3 
      K(IPA+3,2)=KF4 
 
C...Store partons/particles in K vectors for normal case. 
      IF(IP.GE.0) THEN 
        K(IPA,1)=1 
        IF(KQ1.NE.0.AND.(KQ2.NE.0.OR.KQ3.NE.0.OR.KQ4.NE.0)) K(IPA,1)=2 
        K(IPA+1,1)=1 
        IF(KQ2.NE.0.AND.KQ1+KQ2.NE.0.AND.(KQ3.NE.0.OR.KQ4.NE.0)) 
     &  K(IPA+1,1)=2 
        K(IPA+2,1)=1 
        IF(KQ3.NE.0.AND.KQ4.NE.0) K(IPA+2,1)=2 
        K(IPA+3,1)=1 
 
C...Store partons for parton shower evolution from q-g-g-qbar or 
C...g-g-g-g event. 
      ELSEIF(KQ1+KQ2.NE.0) THEN 
        K(IPA,1)=3 
        K(IPA+1,1)=3 
        K(IPA+2,1)=3 
        K(IPA+3,1)=3 
        KCS=4 
        IF(KQ1.EQ.-1) KCS=5 
        K(IPA,KCS)=MSTU(5)*(IPA+1) 
        K(IPA,9-KCS)=MSTU(5)*(IPA+3) 
        K(IPA+1,KCS)=MSTU(5)*(IPA+2) 
        K(IPA+1,9-KCS)=MSTU(5)*IPA 
        K(IPA+2,KCS)=MSTU(5)*(IPA+3) 
        K(IPA+2,9-KCS)=MSTU(5)*(IPA+1) 
        K(IPA+3,KCS)=MSTU(5)*IPA 
        K(IPA+3,9-KCS)=MSTU(5)*(IPA+2) 
 
C...Store partons for parton shower evolution from q-qbar-q-qbar event. 
      ELSE 
        K(IPA,1)=3 
        K(IPA+1,1)=3 
        K(IPA+2,1)=3 
        K(IPA+3,1)=3 
        K(IPA,4)=MSTU(5)*(IPA+1) 
        K(IPA,5)=K(IPA,4) 
        K(IPA+1,4)=MSTU(5)*IPA 
        K(IPA+1,5)=K(IPA+1,4) 
        K(IPA+2,4)=MSTU(5)*(IPA+3) 
        K(IPA+2,5)=K(IPA+2,4) 
        K(IPA+3,4)=MSTU(5)*(IPA+2) 
        K(IPA+3,5)=K(IPA+3,4) 
      ENDIF 
 
C...Check kinematics. 
      MKERR=0 
      IF(0.5*X1*PECM.LE.PM1.OR.0.5*X2*PECM.LE.PM2.OR.0.5*(2.-X1-X2-X4)* 
     &PECM.LE.PM3.OR.0.5*X4*PECM.LE.PM4) MKERR=1 
      PA1=SQRT(MAX(1E-10,(0.5*X1*PECM)**2-PM1**2)) 
      PA2=SQRT(MAX(1E-10,(0.5*X2*PECM)**2-PM2**2)) 
      PA4=SQRT(MAX(1E-10,(0.5*X4*PECM)**2-PM4**2)) 
      X24=X1+X2+X4-1.-X12-X14+(PM3**2-PM1**2-PM2**2-PM4**2)/PECM**2 
      CTHE4=(X1*X4-2.*X14)*PECM**2/(4.*PA1*PA4) 
      IF(ABS(CTHE4).GE.1.002) MKERR=1 
      CTHE4=MAX(-1.,MIN(1.,CTHE4)) 
      STHE4=SQRT(1.-CTHE4**2) 
      CTHE2=(X1*X2-2.*X12)*PECM**2/(4.*PA1*PA2) 
      IF(ABS(CTHE2).GE.1.002) MKERR=1 
      CTHE2=MAX(-1.,MIN(1.,CTHE2)) 
      STHE2=SQRT(1.-CTHE2**2) 
      CPHI2=((X2*X4-2.*X24)*PECM**2-4.*PA2*CTHE2*PA4*CTHE4)/ 
     &MAX(1E-8*PECM**2,4.*PA2*STHE2*PA4*STHE4) 
      IF(ABS(CPHI2).GE.1.05) MKERR=1 
      CPHI2=MAX(-1.,MIN(1.,CPHI2)) 
      IF(MKERR.EQ.1) CALL LUERRM0(13, 
     &'(LU4ENT0:) unphysical kinematical variable setup') 
 
C...Store partons/particles in P vectors. 
      P(IPA,3)=PA1 
      P(IPA,4)=SQRT(PA1**2+PM1**2) 
      P(IPA,5)=PM1 
      P(IPA+3,1)=PA4*STHE4 
      P(IPA+3,3)=PA4*CTHE4 
      P(IPA+3,4)=SQRT(PA4**2+PM4**2) 
      P(IPA+3,5)=PM4 
      P(IPA+1,1)=PA2*STHE2*CPHI2 
      P(IPA+1,2)=PA2*STHE2*SQRT(1.-CPHI2**2)*(-1.)**INT(RLU0(0)+0.5) 
      P(IPA+1,3)=PA2*CTHE2 
      P(IPA+1,4)=SQRT(PA2**2+PM2**2) 
      P(IPA+1,5)=PM2 
      P(IPA+2,1)=-P(IPA+1,1)-P(IPA+3,1) 
      P(IPA+2,2)=-P(IPA+1,2) 
      P(IPA+2,3)=-P(IPA,3)-P(IPA+1,3)-P(IPA+3,3) 
      P(IPA+2,4)=SQRT(P(IPA+2,1)**2+P(IPA+2,2)**2+P(IPA+2,3)**2+PM3**2) 
      P(IPA+2,5)=PM3 
 
C...Set N. Optionally fragment/decay. 
      N=IPA+3 
      IF(IP.EQ.0) CALL LUEXEC0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUJOIN0(NJOIN,IJOIN) 
 
C...Purpose: to connect a sequence of partons with colour flow indices, 
C...as required for subsequent shower evolution (or other operations). 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION IJOIN(*) 
 
C...Check that partons are of right types to be connected. 
      IF(NJOIN.LT.2) GOTO 120 
      KQSUM=0 
      DO 100 IJN=1,NJOIN 
      I=IJOIN(IJN) 
      IF(I.LE.0.OR.I.GT.N) GOTO 120 
      IF(K(I,1).LT.1.OR.K(I,1).GT.3) GOTO 120 
      KC=LUCOMP0(K(I,2)) 
      IF(KC.EQ.0) GOTO 120 
      KQ=KCHG(KC,2)*ISIGN(1,K(I,2)) 
      IF(KQ.EQ.0) GOTO 120 
      IF(IJN.NE.1.AND.IJN.NE.NJOIN.AND.KQ.NE.2) GOTO 120 
      IF(KQ.NE.2) KQSUM=KQSUM+KQ 
      IF(IJN.EQ.1) KQS=KQ 
  100 CONTINUE 
      IF(KQSUM.NE.0) GOTO 120 
 
C...Connect the partons sequentially (closing for gluon loop). 
      KCS=(9-KQS)/2 
      IF(KQS.EQ.2) KCS=INT(4.5+RLU0(0)) 
      DO 110 IJN=1,NJOIN 
      I=IJOIN(IJN) 
      K(I,1)=3 
      IF(IJN.NE.1) IP=IJOIN(IJN-1) 
      IF(IJN.EQ.1) IP=IJOIN(NJOIN) 
      IF(IJN.NE.NJOIN) IN=IJOIN(IJN+1) 
      IF(IJN.EQ.NJOIN) IN=IJOIN(1) 
      K(I,KCS)=MSTU(5)*IN 
      K(I,9-KCS)=MSTU(5)*IP 
      IF(IJN.EQ.1.AND.KQS.NE.2) K(I,9-KCS)=0 
      IF(IJN.EQ.NJOIN.AND.KQS.NE.2) K(I,KCS)=0 
  110 CONTINUE 
 
C...Error exit: no action taken. 
      RETURN 
  120 CALL LUERRM0(12, 
     &'(LUJOIN0:) given entries can not be joined by one string') 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUGIVE0(CHIN) 
 
C...Purpose: to set values of commonblock variables (also in PYTHIA!). 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      COMMON/LUDAT04/CHAF(500) 
      CHARACTER CHAF*8 
      COMMON/LUDAT0R/MRLU0(6),RRLU0(100) 
      COMMON/PYSUBS/MSEL,MSUB(200),KFIN(2,-40:40),CKIN(200) 
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200) 
      COMMON/PYINT1/MINT(400),VINT(400) 
      COMMON/PYINT2/ISET(200),KFPR(200,2),COEF(200,20),ICOL(40,4,2) 
      COMMON/PYINT3/XSFX(2,-40:40),ISIG(1000,3),SIGH(1000) 
      COMMON/PYINT4/WIDP(21:40,0:40),WIDE(21:40,0:40),WIDS(21:40,3) 
      COMMON/PYINT5/NGEN(0:200,3),XSEC(0:200,3) 
      COMMON/PYINT6/PROC(0:200) 
      COMMON/PYINT7/SIGT(0:6,0:6,0:5) 
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100)
      CHARACTER PROC*28 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/LUDAT03/,/LUDAT04/,/LUDAT0R/ 
      SAVE /PYSUBS/,/PYPARS/,/PYINT1/,/PYINT2/,/PYINT3/,/PYINT4/, 
     &/PYINT5/,/PYINT6/,/PYINT7/ ,/ARLWDAT1/
      CHARACTER CHIN*(*),CHFIX*104,CHBIT*104,CHOLD*8,CHNEW*8,CHOLD2*28, 
     &CHNEW2*28,CHNAM*5,CHVAR(44)*5,CHALP(2)*26,CHIND*8,CHINI*10, 
     &CHINR*16 
      DIMENSION MSVAR(44,8) 
 
C...For each variable to be translated give: name, 
C...integer/real/character, no. of indices, lower&upper index bounds. 
      DATA CHVAR/'N','K','P','V','MSTU','PARU','MSTJ','PARJ','KCHG',    ! 9
     &'PMAS','PARF','VCKM','MDCY','MDME','BRAT','KFDP','CHAF','MRLU0',  !18
     &'RRLU0','MSEL','MSUB','KFIN','CKIN','MSTP','PARP','MSTI','PARI',  !27
     &'MINT','VINT','ISET','KFPR','COEF','ICOL','XSFX','ISIG','SIGH',   !36
     &'WIDP','WIDE','WIDS','NGEN','XSEC','PROC','SIGT','RALPA'/         !44 
      DATA ((MSVAR(I,J),J=1,8),I=1,44)/ 1,7*0,  1,2,1,4000,1,5,2*0, 
     & 2,2,1,4000,1,5,2*0,  2,2,1,4000,1,5,2*0,  1,1,1,200,4*0, 
     & 2,1,1,200,4*0,  1,1,1,200,4*0,  2,1,1,200,4*0, 
     & 1,2,1,500,1,3,2*0,  2,2,1,500,1,4,2*0,  2,1,1,2000,4*0, 
     & 2,2,1,4,1,4,2*0,  1,2,1,500,1,3,2*0,  1,2,1,2000,1,2,2*0, 
     & 2,1,1,2000,4*0,  1,2,1,2000,1,5,2*0,  3,1,1,500,4*0, 
     & 1,1,1,6,4*0,  2,1,1,100,4*0, 
     & 1,7*0,  1,1,1,200,4*0,  1,2,1,2,-40,40,2*0,  2,1,1,200,4*0, 
     & 1,1,1,200,4*0,  2,1,1,200,4*0,  1,1,1,200,4*0,  2,1,1,200,4*0, 
     & 1,1,1,400,4*0,  2,1,1,400,4*0,  1,1,1,200,4*0, 
     & 1,2,1,200,1,2,2*0,  2,2,1,200,1,20,2*0,  1,3,1,40,1,4,1,2, 
     & 2,2,1,2,-40,40,2*0,  1,2,1,1000,1,3,2*0,  2,1,1,1000,4*0, 
     & 2,2,21,40,0,40,2*0,  2,2,21,40,0,40,2*0,  2,2,21,40,1,3,2*0, 
     & 1,2,0,200,1,3,2*0,  2,2,0,200,1,3,2*0,  4,1,0,200,4*0, 
     & 2,3,0,6,0,6,0,5,  2,1,1,200,4*0/
      DATA CHALP/'abcdefghijklmnopqrstuvwxyz', 
     &'ABCDEFGHIJKLMNOPQRSTUVWXYZ'/ 
 
C...Length of character variable. Subdivide it into instructions. 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      CHBIT=CHIN//' '
      LBIT=101 
  100 LBIT=LBIT-1 
      IF(CHBIT(LBIT:LBIT).EQ.' ') GOTO 100 
      LTOT=0 
      DO 110 LCOM=1,LBIT 
      IF(CHBIT(LCOM:LCOM).EQ.' ') GOTO 110 
      LTOT=LTOT+1 
      CHFIX(LTOT:LTOT)=CHBIT(LCOM:LCOM) 
  110 CONTINUE 
      LLOW=0 
  120 LHIG=LLOW+1 
  130 LHIG=LHIG+1 
      IF(LHIG.LE.LTOT.AND.CHFIX(LHIG:LHIG).NE.';') GOTO 130 
      LBIT=LHIG-LLOW-1 
      CHBIT(1:LBIT)=CHFIX(LLOW+1:LHIG-1) 
 
C...Identify commonblock variable. 
      LNAM=1 
  140 LNAM=LNAM+1 
      IF(CHBIT(LNAM:LNAM).NE.'('.AND.CHBIT(LNAM:LNAM).NE.'='.AND. 
     &LNAM.LE.5) GOTO 140 
      CHNAM=CHBIT(1:LNAM-1)//' ' 
      DO 160 LCOM=1,LNAM-1 
      DO 150 LALP=1,26 
      IF(CHNAM(LCOM:LCOM).EQ.CHALP(1)(LALP:LALP)) CHNAM(LCOM:LCOM)= 
     &CHALP(2)(LALP:LALP) 
  150 CONTINUE 
  160 CONTINUE 
      IVAR=0 

      DO 170 IV=1,44 
      IF(CHNAM.EQ.CHVAR(IV)) IVAR=IV 
  170 CONTINUE 
      IF(IVAR.EQ.0) THEN 
        CALL LUERRM0(18,'(LUGIVE0:) do not recognize variable '//CHNAM) 
        LLOW=LHIG 
        IF(LLOW.LT.LTOT) GOTO 120 
        RETURN 
      ENDIF 
      
C...Identify any indices. 
      I1=0 
      I2=0 
      I3=0 
      NINDX=0 
      IF(CHBIT(LNAM:LNAM).EQ.'(') THEN 
        LIND=LNAM 
  180   LIND=LIND+1 
        IF(CHBIT(LIND:LIND).NE.')'.AND.CHBIT(LIND:LIND).NE.',') GOTO 180 
        CHIND=' ' 
        IF((CHBIT(LNAM+1:LNAM+1).EQ.'C'.OR.CHBIT(LNAM+1:LNAM+1).EQ.'c'). 
     &  AND.(IVAR.EQ.9.OR.IVAR.EQ.10.OR.IVAR.EQ.13.OR.IVAR.EQ.17)) THEN 
          CHIND(LNAM-LIND+11:8)=CHBIT(LNAM+2:LIND-1) 
          READ(CHIND,'(I8)') KF 
          I1=LUCOMP0(KF) 
        ELSEIF(CHBIT(LNAM+1:LNAM+1).EQ.'C'.OR.CHBIT(LNAM+1:LNAM+1).EQ. 
     &  'c') THEN 
          CALL LUERRM0(18,'(LUGIVE0:) not allowed to use C index for '// 
     &    CHNAM) 
          LLOW=LHIG 
          IF(LLOW.LT.LTOT) GOTO 120 
          RETURN 
        ELSE 
          CHIND(LNAM-LIND+10:8)=CHBIT(LNAM+1:LIND-1) 
          READ(CHIND,'(I8)') I1 
        ENDIF 
        LNAM=LIND 
        IF(CHBIT(LNAM:LNAM).EQ.')') LNAM=LNAM+1 
        NINDX=1 
      ENDIF 
      IF(CHBIT(LNAM:LNAM).EQ.',') THEN 
        LIND=LNAM 
  190   LIND=LIND+1 
        IF(CHBIT(LIND:LIND).NE.')'.AND.CHBIT(LIND:LIND).NE.',') GOTO 190 
        CHIND=' ' 
        CHIND(LNAM-LIND+10:8)=CHBIT(LNAM+1:LIND-1) 
        READ(CHIND,'(I8)') I2 
        LNAM=LIND 
        IF(CHBIT(LNAM:LNAM).EQ.')') LNAM=LNAM+1 
        NINDX=2 
      ENDIF 
      IF(CHBIT(LNAM:LNAM).EQ.',') THEN 
        LIND=LNAM 
  200   LIND=LIND+1 
        IF(CHBIT(LIND:LIND).NE.')'.AND.CHBIT(LIND:LIND).NE.',') GOTO 200 
        CHIND=' ' 
        CHIND(LNAM-LIND+10:8)=CHBIT(LNAM+1:LIND-1) 
        READ(CHIND,'(I8)') I3 
        LNAM=LIND+1 
        NINDX=3 
      ENDIF 
 
C...Check that indices allowed. 
      IERR=0 
      IF(NINDX.NE.MSVAR(IVAR,2)) IERR=1 
      IF(NINDX.GE.1.AND.(I1.LT.MSVAR(IVAR,3).OR.I1.GT.MSVAR(IVAR,4))) 
     &IERR=2 
      IF(NINDX.GE.2.AND.(I2.LT.MSVAR(IVAR,5).OR.I2.GT.MSVAR(IVAR,6))) 
     &IERR=3 
      IF(NINDX.EQ.3.AND.(I3.LT.MSVAR(IVAR,7).OR.I3.GT.MSVAR(IVAR,8))) 
     &IERR=4 
      IF(CHBIT(LNAM:LNAM).NE.'=') IERR=5 
      IF(IERR.GE.1) THEN 
        CALL LUERRM0(18,'(LUGIVE0:) unallowed indices for '// 
     &  CHBIT(1:LNAM-1)) 
        LLOW=LHIG 
        IF(LLOW.LT.LTOT) GOTO 120 
        RETURN 
      ENDIF 
 
C...Save old value of variable. 
      IF(IVAR.EQ.1) THEN 
        IOLD=N 
      ELSEIF(IVAR.EQ.2) THEN 
        IOLD=K(I1,I2) 
      ELSEIF(IVAR.EQ.3) THEN 
        ROLD=P(I1,I2) 
      ELSEIF(IVAR.EQ.4) THEN 
        ROLD=V(I1,I2) 
      ELSEIF(IVAR.EQ.5) THEN 
        IOLD=MSTU(I1) 
      ELSEIF(IVAR.EQ.6) THEN 
        ROLD=PARU(I1) 
      ELSEIF(IVAR.EQ.7) THEN 
        IOLD=MSTJ(I1) 
      ELSEIF(IVAR.EQ.8) THEN 
        ROLD=PARJ(I1) 
      ELSEIF(IVAR.EQ.9) THEN 
        IOLD=KCHG(I1,I2) 
      ELSEIF(IVAR.EQ.10) THEN 
        ROLD=PMAS(I1,I2) 
      ELSEIF(IVAR.EQ.11) THEN 
        ROLD=PARF(I1) 
      ELSEIF(IVAR.EQ.12) THEN 
        ROLD=VCKM(I1,I2) 
      ELSEIF(IVAR.EQ.13) THEN 
        IOLD=MDCY(I1,I2) 
      ELSEIF(IVAR.EQ.14) THEN 
        IOLD=MDME(I1,I2) 
      ELSEIF(IVAR.EQ.15) THEN 
        ROLD=BRAT(I1) 
      ELSEIF(IVAR.EQ.16) THEN 
        IOLD=KFDP(I1,I2) 
      ELSEIF(IVAR.EQ.17) THEN 
        CHOLD=CHAF(I1) 
      ELSEIF(IVAR.EQ.18) THEN 
        IOLD=MRLU0(I1) 
      ELSEIF(IVAR.EQ.19) THEN 
        ROLD=RRLU0(I1) 
      ELSEIF(IVAR.EQ.20) THEN 
        IOLD=MSEL 
      ELSEIF(IVAR.EQ.21) THEN 
        IOLD=MSUB(I1) 
      ELSEIF(IVAR.EQ.22) THEN 
        IOLD=KFIN(I1,I2) 
      ELSEIF(IVAR.EQ.23) THEN 
        ROLD=CKIN(I1) 
      ELSEIF(IVAR.EQ.24) THEN 
        IOLD=MSTP(I1) 
      ELSEIF(IVAR.EQ.25) THEN 
        ROLD=PARP(I1) 
      ELSEIF(IVAR.EQ.26) THEN 
        IOLD=MSTI(I1) 
      ELSEIF(IVAR.EQ.27) THEN 
        ROLD=PARI(I1) 
      ELSEIF(IVAR.EQ.28) THEN 
        IOLD=MINT(I1) 
      ELSEIF(IVAR.EQ.29) THEN 
        ROLD=VINT(I1) 
      ELSEIF(IVAR.EQ.30) THEN 
        IOLD=ISET(I1) 
      ELSEIF(IVAR.EQ.31) THEN 
        IOLD=KFPR(I1,I2) 
      ELSEIF(IVAR.EQ.32) THEN 
        ROLD=COEF(I1,I2) 
      ELSEIF(IVAR.EQ.33) THEN 
        IOLD=ICOL(I1,I2,I3) 
      ELSEIF(IVAR.EQ.34) THEN 
        ROLD=XSFX(I1,I2) 
      ELSEIF(IVAR.EQ.35) THEN 
        IOLD=ISIG(I1,I2) 
      ELSEIF(IVAR.EQ.36) THEN 
        ROLD=SIGH(I1) 
      ELSEIF(IVAR.EQ.37) THEN 
        ROLD=WIDP(I1,I2) 
      ELSEIF(IVAR.EQ.38) THEN 
        ROLD=WIDE(I1,I2) 
      ELSEIF(IVAR.EQ.39) THEN 
        ROLD=WIDS(I1,I2) 
      ELSEIF(IVAR.EQ.40) THEN 
        IOLD=NGEN(I1,I2) 
      ELSEIF(IVAR.EQ.41) THEN 
        ROLD=XSEC(I1,I2) 
      ELSEIF(IVAR.EQ.42) THEN 
        CHOLD2=PROC(I1) 
      ELSEIF(IVAR.EQ.43) THEN 
         ROLD=SIGT(I1,I2,I3) 
      ELSEIF(IVAR.EQ.44) THEN 
        ROLD=RALPA(I1) 
      ENDIF 
 
C...Print current value of variable. Loop back. 
      IF(LNAM.GE.LBIT) THEN 
        CHBIT(LNAM:14)=' ' 
        CHBIT(15:60)=' has the value                                ' 
        IF(MSVAR(IVAR,1).EQ.1) THEN 
          WRITE(CHBIT(51:60),'(I10)') IOLD 
        ELSEIF(MSVAR(IVAR,1).EQ.2) THEN 
          WRITE(CHBIT(47:60),'(F14.5)') ROLD 
        ELSEIF(MSVAR(IVAR,1).EQ.3) THEN 
          CHBIT(53:60)=CHOLD 
        ELSE 
          CHBIT(33:60)=CHOLD 
        ENDIF 
        IF(MSTU(13).GE.1) WRITE(MSTU(11),5000) CHBIT(1:60) 
        LLOW=LHIG 
        IF(LLOW.LT.LTOT) GOTO 120 
        RETURN 
      ENDIF 
 
C...Read in new variable value. 
      IF(MSVAR(IVAR,1).EQ.1) THEN 
        CHINI=' ' 
        CHINI(LNAM-LBIT+11:10)=CHBIT(LNAM+1:LBIT) 
        READ(CHINI,'(I10)') INEW 
      ELSEIF(MSVAR(IVAR,1).EQ.2) THEN 
        CHINR=' ' 
        CHINR(LNAM-LBIT+17:16)=CHBIT(LNAM+1:LBIT) 
        READ(CHINR,'(F16.2)') RNEW 
      ELSEIF(MSVAR(IVAR,1).EQ.3) THEN 
        CHNEW=CHBIT(LNAM+1:LBIT)//' ' 
      ELSE 
        CHNEW2=CHBIT(LNAM+1:LBIT)//' ' 
      ENDIF 
 
C...Store new variable value. 
      IF(IVAR.EQ.1) THEN 
        N=INEW 
      ELSEIF(IVAR.EQ.2) THEN 
        K(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.3) THEN 
        P(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.4) THEN 
        V(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.5) THEN 
        MSTU(I1)=INEW 
      ELSEIF(IVAR.EQ.6) THEN 
        PARU(I1)=RNEW 
      ELSEIF(IVAR.EQ.7) THEN 
        MSTJ(I1)=INEW 
      ELSEIF(IVAR.EQ.8) THEN 
        PARJ(I1)=RNEW 
      ELSEIF(IVAR.EQ.9) THEN 
        KCHG(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.10) THEN 
        PMAS(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.11) THEN 
        PARF(I1)=RNEW 
      ELSEIF(IVAR.EQ.12) THEN 
        VCKM(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.13) THEN 
        MDCY(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.14) THEN 
        MDME(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.15) THEN 
        BRAT(I1)=RNEW 
      ELSEIF(IVAR.EQ.16) THEN 
        KFDP(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.17) THEN 
        CHAF(I1)=CHNEW 
      ELSEIF(IVAR.EQ.18) THEN 
        MRLU0(I1)=INEW 
      ELSEIF(IVAR.EQ.19) THEN 
        RRLU0(I1)=RNEW 
      ELSEIF(IVAR.EQ.20) THEN 
        MSEL=INEW 
      ELSEIF(IVAR.EQ.21) THEN 
        MSUB(I1)=INEW 
      ELSEIF(IVAR.EQ.22) THEN 
        KFIN(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.23) THEN 
        CKIN(I1)=RNEW 
      ELSEIF(IVAR.EQ.24) THEN 
        MSTP(I1)=INEW 
      ELSEIF(IVAR.EQ.25) THEN 
        PARP(I1)=RNEW 
      ELSEIF(IVAR.EQ.26) THEN 
        MSTI(I1)=INEW 
      ELSEIF(IVAR.EQ.27) THEN 
        PARI(I1)=RNEW 
      ELSEIF(IVAR.EQ.28) THEN 
        MINT(I1)=INEW 
      ELSEIF(IVAR.EQ.29) THEN 
        VINT(I1)=RNEW 
      ELSEIF(IVAR.EQ.30) THEN 
        ISET(I1)=INEW 
      ELSEIF(IVAR.EQ.31) THEN 
        KFPR(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.32) THEN 
        COEF(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.33) THEN 
        ICOL(I1,I2,I3)=INEW 
      ELSEIF(IVAR.EQ.34) THEN 
        XSFX(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.35) THEN 
        ISIG(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.36) THEN 
        SIGH(I1)=RNEW 
      ELSEIF(IVAR.EQ.37) THEN 
        WIDP(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.38) THEN 
        WIDE(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.39) THEN 
        WIDS(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.40) THEN 
        NGEN(I1,I2)=INEW 
      ELSEIF(IVAR.EQ.41) THEN 
        XSEC(I1,I2)=RNEW 
      ELSEIF(IVAR.EQ.42) THEN 
        PROC(I1)=CHNEW2 
      ELSEIF(IVAR.EQ.43) THEN 
        SIGT(I1,I2,I3)=RNEW 
      ELSEIF(IVAR.EQ.44) THEN 
        RALPA(I1)=RNEW 
      ENDIF 
 
c      print*,'RALPA(',I1,')=',RNEW
C...Write old and new value. Loop back. 
      CHBIT(LNAM:14)=' ' 
      CHBIT(15:60)=' changed from                to               ' 
      IF(MSVAR(IVAR,1).EQ.1) THEN 
        WRITE(CHBIT(33:42),'(I10)') IOLD 
        WRITE(CHBIT(51:60),'(I10)') INEW 
        IF(MSTU(13).GE.1) WRITE(MSTU(11),5000) CHBIT(1:60) 
      ELSEIF(MSVAR(IVAR,1).EQ.2) THEN 
        WRITE(CHBIT(29:42),'(F14.5)') ROLD 
        WRITE(CHBIT(47:60),'(F14.5)') RNEW 
        IF(MSTU(13).GE.1) WRITE(MSTU(11),5000) CHBIT(1:60) 
      ELSEIF(MSVAR(IVAR,1).EQ.3) THEN 
        CHBIT(35:42)=CHOLD 
        CHBIT(53:60)=CHNEW 
        IF(MSTU(13).GE.1) WRITE(MSTU(11),5000) CHBIT(1:60) 
      ELSE 
        CHBIT(15:88)=' changed from '//CHOLD2//' to '//CHNEW2 
        IF(MSTU(13).GE.1) WRITE(MSTU(11),5100) CHBIT(1:88) 
      ENDIF 
      LLOW=LHIG 
      IF(LLOW.LT.LTOT) GOTO 120 
 
C...Format statement for output on unit MSTU(11) (by default 6). 
 5000 FORMAT(5X,A60) 
 5100 FORMAT(5X,A88) 
 
c      print*, 'RALPA(18)===',RALPA(18)
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUEXEC0 
 
C...Purpose: to administrate the fragmentation and decay chain. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/LUDAT03/ 
      DIMENSION PS(2,6) 
 
C...Initialize and reset. 
      MSTU(24)=0 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
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
      PS(1,6)=PS(1,6)+LUCHGE0(K(I,2)) 
  130 CONTINUE 
      PARU(21)=PS(1,4) 
 
C...Prepare system for subsequent fragmentation/decay. 
      CALL LUPREP0(0) 
 
C...Loop through jet fragmentation and particle decays. 
      MBE=0 
  140 MBE=MBE+1 
      IP=0 
  150 IP=IP+1 
      KC=0 
      IF(K(IP,1).GT.0.AND.K(IP,1).LE.10) KC=LUCOMP0(K(IP,2)) 
      IF(KC.EQ.0) THEN 
 
C...Particle decay if unstable and allowed. Save long-lived particle 
C...decays until second pass after Bose-Einstein effects. 
      ELSEIF(KCHG(KC,2).EQ.0) THEN 
        IF(MSTJ(21).GE.1.AND.MDCY(KC,1).GE.1.AND.(MSTJ(51).LE.0.OR.MBE 
     &  .EQ.2.OR.PMAS(KC,2).GE.PARJ(91).OR.IABS(K(IP,2)).EQ.311)) 
     &  CALL LUDECY0(IP) 
 
C...Decay products may develop a shower. 
        IF(MSTJ(92).GT.0) THEN 
          IP1=MSTJ(92) 
          QMAX=SQRT(MAX(0.,(P(IP1,4)+P(IP1+1,4))**2-(P(IP1,1)+P(IP1+1, 
     &    1))**2-(P(IP1,2)+P(IP1+1,2))**2-(P(IP1,3)+P(IP1+1,3))**2)) 
          CALL LUSHOW0(IP1,IP1+1,QMAX) 
          CALL LUPREP0(IP1) 
          MSTJ(92)=0 
        ELSEIF(MSTJ(92).LT.0) THEN 
          IP1=-MSTJ(92) 
          CALL LUSHOW0(IP1,-3,P(IP,5)) 
          CALL LUPREP0(IP1) 
          MSTJ(92)=0 
        ENDIF 
 
C...Jet fragmentation: string or independent fragmentation. 
      ELSEIF(K(IP,1).EQ.1.OR.K(IP,1).EQ.2) THEN 
        MFRAG=MSTJ(1) 
        IF(MFRAG.GE.1.AND.K(IP,1).EQ.1) MFRAG=2 
        IF(MSTJ(21).GE.2.AND.K(IP,1).EQ.2.AND.N.GT.IP) THEN 
          IF(K(IP+1,1).EQ.1.AND.K(IP+1,3).EQ.K(IP,3).AND. 
     &    K(IP,3).GT.0.AND.K(IP,3).LT.IP) THEN 
            IF(KCHG(LUCOMP0(K(K(IP,3),2)),2).EQ.0) MFRAG=MIN(1,MFRAG) 
          ENDIF 
        ENDIF 
        IF(MFRAG.EQ.1) CALL LUSTRF0(IP) 
        IF(MFRAG.EQ.2) CALL LUINDF0(IP) 
        IF(MFRAG.EQ.2.AND.K(IP,1).EQ.1) MCONS=0 
        IF(MFRAG.EQ.2.AND.(MSTJ(3).LE.0.OR.MOD(MSTJ(3),5).EQ.0)) MCONS=0 
      ENDIF 
 
C...Loop back if enough space left in LUJETS and no error abort. 
      IF(MSTU(24).NE.0.AND.MSTU(21).GE.2) THEN 
      ELSEIF(IP.LT.N.AND.N.LT.MSTU(4)-20-MSTU(32)) THEN 
        GOTO 150 
      ELSEIF(IP.LT.N) THEN 
        CALL LUERRM0(11,'(LUEXEC0:) no more memory left in LUJETS') 
      ENDIF 
 
C...Include simple Bose-Einstein effect parametrization if desired. 
      IF(MBE.EQ.1.AND.MSTJ(51).GE.1) THEN 
        CALL LUBOEI0(NSAV) 
        GOTO 140 
      ENDIF 
 
C...Check that momentum, energy and charge were conserved. 
      DO 170 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 170 
      DO 160 J=1,4 
      PS(2,J)=PS(2,J)+P(I,J) 
  160 CONTINUE 
      PS(2,6)=PS(2,6)+LUCHGE0(K(I,2)) 
  170 CONTINUE 
      PDEV=(ABS(PS(2,1)-PS(1,1))+ABS(PS(2,2)-PS(1,2))+ABS(PS(2,3)- 
     &PS(1,3))+ABS(PS(2,4)-PS(1,4)))/(1.+ABS(PS(2,4))+ABS(PS(1,4))) 
      IF(MCONS.EQ.1.AND.PDEV.GT.PARU(11)) CALL LUERRM0(15, 
     &'(LUEXEC0:) four-momentum was not conserved') 
      IF(MCONS.EQ.1.AND.ABS(PS(2,6)-PS(1,6)).GT.0.1) CALL LUERRM0(15, 
     &'(LUEXEC0:) charge was not conserved') 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUPREP0(IP) 
 
C...Purpose: to rearrange partons along strings, to allow small systems 
C...to collapse into one or two particles and to check flavours. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/LUDAT03/ 
      DIMENSION DPS(5),DPC(5),UE(3) 
 
C...Rearrange parton shower product listing along strings: begin loop. 
      I1=N 
      DO 130 MQGST=1,2 
      DO 120 I=MAX(1,IP),N 
      IF(K(I,1).NE.3) GOTO 120 
      KC=LUCOMP0(K(I,2)) 
      IF(KC.EQ.0) GOTO 120 
      KQ=KCHG(KC,2) 
      IF(KQ.EQ.0.OR.(MQGST.EQ.1.AND.KQ.EQ.2)) GOTO 120 
 
C...Pick up loose string end. 
      KCS=4 
      IF(KQ*ISIGN(1,K(I,2)).LT.0) KCS=5 
      IA=I 
      NSTP=0 
  100 NSTP=NSTP+1 
      IF(NSTP.GT.4*N) THEN 
        CALL LUERRM0(14,'(LUPREP0:) caught in infinite loop') 
        RETURN 
      ENDIF 
 
C...Copy undecayed parton. 
      IF(K(IA,1).EQ.3) THEN 
        IF(I1.GE.MSTU(4)-MSTU(32)-5) THEN 
          CALL LUERRM0(11,'(LUPREP0:) no more memory left in LUJETS') 
          RETURN 
        ENDIF 
        I1=I1+1 
        K(I1,1)=2 
        IF(NSTP.GE.2.AND.IABS(K(IA,2)).NE.21) K(I1,1)=1 
        K(I1,2)=K(IA,2) 
        K(I1,3)=IA 
        K(I1,4)=0 
        K(I1,5)=0 
        DO 110 J=1,5 
        P(I1,J)=P(IA,J) 
        V(I1,J)=V(IA,J) 
  110   CONTINUE 
        K(IA,1)=K(IA,1)+10 
        IF(K(I1,1).EQ.1) GOTO 120 
      ENDIF 
 
C...Go to next parton in colour space. 
      IB=IA 
      IF(MOD(K(IB,KCS)/MSTU(5)**2,2).EQ.0.AND.MOD(K(IB,KCS),MSTU(5)) 
     &.NE.0) THEN 
        IA=MOD(K(IB,KCS),MSTU(5)) 
        K(IB,KCS)=K(IB,KCS)+MSTU(5)**2 
        MREV=0 
      ELSE 
        IF(K(IB,KCS).GE.2*MSTU(5)**2.OR.MOD(K(IB,KCS)/MSTU(5),MSTU(5)) 
     &  .EQ.0) KCS=9-KCS 
        IA=MOD(K(IB,KCS)/MSTU(5),MSTU(5)) 
        K(IB,KCS)=K(IB,KCS)+2*MSTU(5)**2 
        MREV=1 
      ENDIF 
      IF(IA.LE.0.OR.IA.GT.N) THEN 
        CALL LUERRM0(12,'(LUPREP0:) colour rearrangement failed') 
        RETURN 
      ENDIF 
      IF(MOD(K(IA,4)/MSTU(5),MSTU(5)).EQ.IB.OR.MOD(K(IA,5)/MSTU(5), 
     &MSTU(5)).EQ.IB) THEN 
        IF(MREV.EQ.1) KCS=9-KCS 
        IF(MOD(K(IA,KCS)/MSTU(5),MSTU(5)).NE.IB) KCS=9-KCS 
        K(IA,KCS)=K(IA,KCS)+2*MSTU(5)**2 
      ELSE 
        IF(MREV.EQ.0) KCS=9-KCS 
        IF(MOD(K(IA,KCS),MSTU(5)).NE.IB) KCS=9-KCS 
        K(IA,KCS)=K(IA,KCS)+MSTU(5)**2 
      ENDIF 
      IF(IA.NE.I) GOTO 100 
      K(I1,1)=1 
  120 CONTINUE 
  130 CONTINUE 
      N=I1 
      IF(MSTJ(14).LT.0) RETURN 
 
C...Find lowest-mass colour singlet jet system, OK if above threshold. 
      IF(MSTJ(14).EQ.0) GOTO 320 
      NS=N 
  140 NSIN=N-NS 
      PDM=1.+PARJ(32) 
      IC=0 
      DO 190 I=MAX(1,IP),NS 
      IF(K(I,1).NE.1.AND.K(I,1).NE.2) THEN 
      ELSEIF(K(I,1).EQ.2.AND.IC.EQ.0) THEN 
        NSIN=NSIN+1 
        IC=I 
        DO 150 J=1,4 
        DPS(J)=P(I,J) 
  150   CONTINUE 
        MSTJ(93)=1 
        DPS(5)=ULMASS0(K(I,2)) 
      ELSEIF(K(I,1).EQ.2) THEN 
        DO 160 J=1,4 
        DPS(J)=DPS(J)+P(I,J) 
  160   CONTINUE 
      ELSEIF(IC.NE.0.AND.KCHG(LUCOMP0(K(I,2)),2).NE.0) THEN 
        DO 170 J=1,4 
        DPS(J)=DPS(J)+P(I,J) 
  170   CONTINUE 
        MSTJ(93)=1 
        DPS(5)=DPS(5)+ULMASS0(K(I,2)) 
        PD=SQRT(MAX(0D0,DPS(4)**2-DPS(1)**2-DPS(2)**2-DPS(3)**2))-DPS(5) 
        IF(PD.LT.PDM) THEN 
          PDM=PD 
          DO 180 J=1,5 
          DPC(J)=DPS(J) 
  180     CONTINUE 
          IC1=IC 
          IC2=I 
        ENDIF 
        IC=0 
      ELSE 
        NSIN=NSIN+1 
      ENDIF 
  190 CONTINUE 
      IF(PDM.GE.PARJ(32)) GOTO 320 
 
C...Fill small-mass system as cluster. 
      NSAV=N 
      PECM=SQRT(MAX(0D0,DPC(4)**2-DPC(1)**2-DPC(2)**2-DPC(3)**2)) 
      K(N+1,1)=11 
      K(N+1,2)=91 
      K(N+1,3)=IC1 
      K(N+1,4)=N+2 
      K(N+1,5)=N+3 
      P(N+1,1)=DPC(1) 
      P(N+1,2)=DPC(2) 
      P(N+1,3)=DPC(3) 
      P(N+1,4)=DPC(4) 
      P(N+1,5)=PECM 
 
C...Form two particles from flavours of lowest-mass system, if feasible. 
      K(N+2,1)=1 
      K(N+3,1)=1 
      IF(MSTU(16).NE.2) THEN 
        K(N+2,3)=N+1 
        K(N+3,3)=N+1 
      ELSE 
        K(N+2,3)=IC1 
        K(N+3,3)=IC2 
      ENDIF 
      K(N+2,4)=0 
      K(N+3,4)=0 
      K(N+2,5)=0 
      K(N+3,5)=0 
      IF(IABS(K(IC1,2)).NE.21) THEN 
        KC1=LUCOMP0(K(IC1,2)) 
        KC2=LUCOMP0(K(IC2,2)) 
        IF(KC1.EQ.0.OR.KC2.EQ.0) GOTO 320 
        KQ1=KCHG(KC1,2)*ISIGN(1,K(IC1,2)) 
        KQ2=KCHG(KC2,2)*ISIGN(1,K(IC2,2)) 
        IF(KQ1+KQ2.NE.0) GOTO 320 
  200   CALL LUKFDI0(K(IC1,2),0,KFLN,K(N+2,2)) 
        CALL LUKFDI0(K(IC2,2),-KFLN,KFLDMP,K(N+3,2)) 
        IF(K(N+2,2).EQ.0.OR.K(N+3,2).EQ.0) GOTO 200 
      ELSE 
        IF(IABS(K(IC2,2)).NE.21) GOTO 320 
  210   CALL LUKFDI0(1+INT((2.+PARJ(2))*RLU0(0)),0,KFLN,KFDMP) 
        CALL LUKFDI0(KFLN,0,KFLM,K(N+2,2)) 
        CALL LUKFDI0(-KFLN,-KFLM,KFLDMP,K(N+3,2)) 
        IF(K(N+2,2).EQ.0.OR.K(N+3,2).EQ.0) GOTO 210 
      ENDIF 
      P(N+2,5)=ULMASS0(K(N+2,2)) 
      P(N+3,5)=ULMASS0(K(N+3,2)) 
      IF(P(N+2,5)+P(N+3,5)+PARJ(64).GE.PECM.AND.NSIN.EQ.1) GOTO 320 
      IF(P(N+2,5)+P(N+3,5)+PARJ(64).GE.PECM) GOTO 260 
 
C...Perform two-particle decay of jet system, if possible. 
      IF(PECM.GE.0.02*DPC(4)) THEN 
        PA=SQRT((PECM**2-(P(N+2,5)+P(N+3,5))**2)*(PECM**2- 
     &  (P(N+2,5)-P(N+3,5))**2))/(2.*PECM) 
        UE(3)=2.*RLU0(0)-1. 
        PHI=PARU(2)*RLU0(0) 
        UE(1)=SQRT(1.-UE(3)**2)*COS(PHI) 
        UE(2)=SQRT(1.-UE(3)**2)*SIN(PHI) 
        DO 220 J=1,3 
        P(N+2,J)=PA*UE(J) 
        P(N+3,J)=-PA*UE(J) 
  220   CONTINUE 
        P(N+2,4)=SQRT(PA**2+P(N+2,5)**2) 
        P(N+3,4)=SQRT(PA**2+P(N+3,5)**2) 
        MSTU(33)=1 
        CALL LUDBRB0(N+2,N+3,0.,0.,DPC(1)/DPC(4),DPC(2)/DPC(4), 
     &  DPC(3)/DPC(4)) 
      ELSE 
        NP=0 
        DO 230 I=IC1,IC2 
        IF(K(I,1).EQ.1.OR.K(I,1).EQ.2) NP=NP+1 
  230   CONTINUE 
        HA=P(IC1,4)*P(IC2,4)-P(IC1,1)*P(IC2,1)-P(IC1,2)*P(IC2,2)- 
     &  P(IC1,3)*P(IC2,3) 
        IF(NP.GE.3.OR.HA.LE.1.25*P(IC1,5)*P(IC2,5)) GOTO 260 
        HD1=0.5*(P(N+2,5)**2-P(IC1,5)**2) 
        HD2=0.5*(P(N+3,5)**2-P(IC2,5)**2) 
        HR=SQRT(MAX(0.,((HA-HD1-HD2)**2-(P(N+2,5)*P(N+3,5))**2)/ 
     &  (HA**2-(P(IC1,5)*P(IC2,5))**2)))-1. 
        HC=P(IC1,5)**2+2.*HA+P(IC2,5)**2 
        HK1=((P(IC2,5)**2+HA)*HR+HD1-HD2)/HC 
        HK2=((P(IC1,5)**2+HA)*HR+HD2-HD1)/HC 
        DO 240 J=1,4 
        P(N+2,J)=(1.+HK1)*P(IC1,J)-HK2*P(IC2,J) 
        P(N+3,J)=(1.+HK2)*P(IC2,J)-HK1*P(IC1,J) 
  240   CONTINUE 
      ENDIF 
      DO 250 J=1,4 
      V(N+1,J)=V(IC1,J) 
      V(N+2,J)=V(IC1,J) 
      V(N+3,J)=V(IC2,J) 
  250 CONTINUE 
      V(N+1,5)=0. 
      V(N+2,5)=0. 
      V(N+3,5)=0. 
      N=N+3 
      GOTO 300 
 
C...Else form one particle from the flavours available, if possible. 
  260 K(N+1,5)=N+2 
      IF(IABS(K(IC1,2)).GT.100.AND.IABS(K(IC2,2)).GT.100) THEN 
        GOTO 320 
      ELSEIF(IABS(K(IC1,2)).NE.21) THEN 
        CALL LUKFDI0(K(IC1,2),K(IC2,2),KFLDMP,K(N+2,2)) 
      ELSE 
        KFLN=1+INT((2.+PARJ(2))*RLU0(0)) 
        CALL LUKFDI0(KFLN,-KFLN,KFLDMP,K(N+2,2)) 
      ENDIF 
      IF(K(N+2,2).EQ.0) GOTO 260 
      P(N+2,5)=ULMASS0(K(N+2,2)) 
 
C...Find parton/particle which combines to largest extra mass. 
      IR=0 
      HA=0. 
      HSM=0. 
      DO 280 MCOMB=1,3 
      IF(IR.NE.0) GOTO 280 
      DO 270 I=MAX(1,IP),N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10.OR.(I.GE.IC1.AND.I.LE.IC2 
     &.AND.K(I,1).GE.1.AND.K(I,1).LE.2)) GOTO 270 
      IF(MCOMB.EQ.1) KCI=LUCOMP0(K(I,2)) 
      IF(MCOMB.EQ.1.AND.KCI.EQ.0) GOTO 270 
      IF(MCOMB.EQ.1.AND.KCHG(KCI,2).EQ.0.AND.I.LE.NS) GOTO 270 
      IF(MCOMB.EQ.2.AND.IABS(K(I,2)).GT.10.AND.IABS(K(I,2)).LE.100) 
     &GOTO 270 
      HCR=DPC(4)*P(I,4)-DPC(1)*P(I,1)-DPC(2)*P(I,2)-DPC(3)*P(I,3) 
      HSR=2.*HCR+PECM**2-P(N+2,5)**2-2.*P(N+2,5)*P(I,5) 
      IF(HSR.GT.HSM) THEN 
        IR=I 
        HA=HCR 
        HSM=HSR 
      ENDIF 
  270 CONTINUE 
  280 CONTINUE 
 
C...Shuffle energy and momentum to put new particle on mass shell. 
      IF(IR.NE.0) THEN 
        HB=PECM**2+HA 
        HC=P(N+2,5)**2+HA 
        HD=P(IR,5)**2+HA 
        HK2=0.5*(HB*SQRT(MAX(0.,((HB+HC)**2-4.*(HB+HD)*P(N+2,5)**2)/ 
     &  (HA**2-(PECM*P(IR,5))**2)))-(HB+HC))/(HB+HD) 
        HK1=(0.5*(P(N+2,5)**2-PECM**2)+HD*HK2)/HB 
        DO 290 J=1,4 
        P(N+2,J)=(1.+HK1)*DPC(J)-HK2*P(IR,J) 
        P(IR,J)=(1.+HK2)*P(IR,J)-HK1*DPC(J) 
        V(N+1,J)=V(IC1,J) 
        V(N+2,J)=V(IC1,J) 
  290   CONTINUE 
        V(N+1,5)=0. 
        V(N+2,5)=0. 
        N=N+2 
      ELSE 
        CALL LUERRM0(3,'(LUPREP0:) no match for collapsing cluster') 
        RETURN 
      ENDIF 
 
C...Mark collapsed system and store daughter pointers. Iterate. 
  300 DO 310 I=IC1,IC2 
      IF((K(I,1).EQ.1.OR.K(I,1).EQ.2).AND.KCHG(LUCOMP0(K(I,2)),2).NE.0) 
     &THEN 
        K(I,1)=K(I,1)+10 
        IF(MSTU(16).NE.2) THEN 
          K(I,4)=NSAV+1 
          K(I,5)=NSAV+1 
        ELSE 
          K(I,4)=NSAV+2 
          K(I,5)=N 
        ENDIF 
      ENDIF 
  310 CONTINUE 
      IF(N.LT.MSTU(4)-MSTU(32)-5) GOTO 140 
 
C...Check flavours and invariant masses in parton systems. 
  320 NP=0 
      KFN=0 
      KQS=0 
      NJU=0
      DO 330 J=1,5 
      DPS(J)=0. 
  330 CONTINUE 
      DO 360 I=MAX(1,IP),N 
      IF(K(I,1).EQ.41) NJU=NJU+1
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 360 
      KC=LUCOMP0(K(I,2)) 
      IF(KC.EQ.0) GOTO 360 
      KQ=KCHG(KC,2)*ISIGN(1,K(I,2)) 
      IF(KQ.EQ.0) GOTO 360 
      NP=NP+1 
      IF(KQ.NE.2) THEN 
        KFN=KFN+1 
        KQS=KQS+KQ 
        MSTJ(93)=1 
        DPS(5)=DPS(5)+ULMASS0(K(I,2)) 
      ENDIF 
      DO 340 J=1,4 
      DPS(J)=DPS(J)+P(I,J) 
  340 CONTINUE 
      IF(K(I,1).EQ.1) THEN 
        NFERR=0
        IF(NJU.EQ.0.AND.NP.NE.1) THEN
          IF(KFN.EQ.1.OR.KFN.GE.3.OR.KQS.NE.0) NFERR=1
        ELSEIF(NJU.EQ.1) THEN
          IF(KFN.NE.3.OR.IABS(KQS).NE.3) NFERR=1
        ELSEIF(NJU.EQ.2) THEN
          IF(KFN.NE.4.OR.KQS.NE.0) NFERR=1
        ELSEIF(NJU.GE.3) THEN
          NFERR=1
        ENDIF
        IF(NFERR.EQ.1) CALL 
     &  LUERRM0(2,'(LUPREP0:) unphysical flavour combination') 
        IF(NP.NE.1.AND.DPS(4)**2-DPS(1)**2-DPS(2)**2-DPS(3)**2.LT. 
     &  (0.9*PARJ(32)+DPS(5))**2) CALL LUERRM0(3, 
     &  '(LUPREP0:) too small mass in jet system') 
        NP=0 
        KFN=0 
        KQS=0 
        NJU=0
        DO 350 J=1,5 
        DPS(J)=0. 
  350   CONTINUE 
      ENDIF 
  360 CONTINUE 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUSTRF0(IP) 
C...Purpose: to handle the fragmentation of an arbitrary colour singlet 
C...jet system according to the Lund string fragmentation model. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION DPS(5),KFL(3),PMQ(3),PX(3),PY(3),GAM(3),IE(2),PR(2), 
     &IN(9),DHM(4),DHG(4),DP(5,5),IRANK(2),MJU(4),IJU(3),PJU(5,5), 
     &TJU(5),KFJH(2),NJS(2),KFJS(2),PJS(4,5),MSTU9T(8),PARU9T(8) 
 
C...Function: four-product of two vectors. 
      FOUR(I,J)=P(I,4)*P(J,4)-P(I,1)*P(J,1)-P(I,2)*P(J,2)-P(I,3)*P(J,3) 
      DFOUR(I,J)=DP(I,4)*DP(J,4)-DP(I,1)*DP(J,1)-DP(I,2)*DP(J,2)- 
     &DP(I,3)*DP(J,3) 
 
C...Reset counters. Identify parton system. 
      MSTJ(91)=0 
      NSAV=N 
      MSTU90=MSTU(90) 
      NP=0 
      KQSUM=0 
      DO 100 J=1,5 
      DPS(J)=0D0 
  100 CONTINUE 
      MJU(1)=0 
      MJU(2)=0 
      I=IP-1 
  110 I=I+1 
      IF(I.GT.MIN(N,MSTU(4)-MSTU(32))) THEN 
        CALL LUERRM0(12,'(LUSTRF0:) failed to reconstruct jet system') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(K(I,1).NE.1.AND.K(I,1).NE.2.AND.K(I,1).NE.41) GOTO 110 
      KC=LUCOMP0(K(I,2)) 
      IF(KC.EQ.0) GOTO 110 
      KQ=KCHG(KC,2)*ISIGN(1,K(I,2)) 
      IF(KQ.EQ.0) GOTO 110 
      IF(N+5*NP+11.GT.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUSTRF0:) no more memory left in LUJETS') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Take copy of partons to be considered. Check flavour sum. 
      NP=NP+1 
      DO 120 J=1,5 
      K(N+NP,J)=K(I,J) 
      P(N+NP,J)=P(I,J) 
      IF(J.NE.4) DPS(J)=DPS(J)+P(I,J) 
  120 CONTINUE 
      DPS(4)=DPS(4)+SQRT(DBLE(P(I,1))**2+DBLE(P(I,2))**2+ 
     &DBLE(P(I,3))**2+DBLE(P(I,5))**2) 
      K(N+NP,3)=I 
      IF(KQ.NE.2) KQSUM=KQSUM+KQ 
      IF(K(I,1).EQ.41) THEN 
        KQSUM=KQSUM+2*KQ 
        IF(KQSUM.EQ.KQ) MJU(1)=N+NP 
        IF(KQSUM.NE.KQ) MJU(2)=N+NP 
      ENDIF 
      IF(K(I,1).EQ.2.OR.K(I,1).EQ.41) GOTO 110 
      IF(KQSUM.NE.0) THEN 
        CALL LUERRM0(12,'(LUSTRF0:) unphysical flavour combination') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Boost copied system to CM frame (for better numerical precision). 
      IF(ABS(DPS(3)).LT.0.99D0*DPS(4)) THEN 
        MBST=0 
        MSTU(33)=1 
        CALL LUDBRB0(N+1,N+NP,0.,0.,-DPS(1)/DPS(4),-DPS(2)/DPS(4), 
     &  -DPS(3)/DPS(4)) 
      ELSE 
        MBST=1 
        HHBZ=SQRT(MAX(1D-6,DPS(4)+DPS(3))/MAX(1D-6,DPS(4)-DPS(3))) 
        DO 130 I=N+1,N+NP 
        HHPMT=P(I,1)**2+P(I,2)**2+P(I,5)**2 
        IF(P(I,3).GT.0.) THEN 
          HHPEZ=(P(I,4)+P(I,3))/HHBZ 
          P(I,3)=0.5*(HHPEZ-HHPMT/HHPEZ) 
          P(I,4)=0.5*(HHPEZ+HHPMT/HHPEZ) 
        ELSE 
          HHPEZ=(P(I,4)-P(I,3))*HHBZ 
          P(I,3)=-0.5*(HHPEZ-HHPMT/HHPEZ) 
          P(I,4)=0.5*(HHPEZ+HHPMT/HHPEZ) 
        ENDIF 
  130   CONTINUE 
      ENDIF 
 
C...Search for very nearby partons that may be recombined. 
      NTRYR=0 
      PARU12=PARU(12) 
      PARU13=PARU(13) 
      MJU(3)=MJU(1) 
      MJU(4)=MJU(2) 
      NR=NP 
  140 IF(NR.GE.3) THEN 
        PDRMIN=2.*PARU12 
        DO 150 I=N+1,N+NR 
        IF(I.EQ.N+NR.AND.IABS(K(N+1,2)).NE.21) GOTO 150 
        I1=I+1 
        IF(I.EQ.N+NR) I1=N+1 
        IF(K(I,1).EQ.41.OR.K(I1,1).EQ.41) GOTO 150 
        IF(MJU(1).NE.0.AND.I1.LT.MJU(1).AND.IABS(K(I1,2)).NE.21) 
     &  GOTO 150 
        IF(MJU(2).NE.0.AND.I.GT.MJU(2).AND.IABS(K(I,2)).NE.21) GOTO 150 
        PAP=SQRT((P(I,1)**2+P(I,2)**2+P(I,3)**2)*(P(I1,1)**2+ 
     &  P(I1,2)**2+P(I1,3)**2)) 
        PVP=P(I,1)*P(I1,1)+P(I,2)*P(I1,2)+P(I,3)*P(I1,3) 
        PDR=4.*(PAP-PVP)**2/MAX(1E-6,PARU13**2*PAP+2.*(PAP-PVP)) 
        IF(PDR.LT.PDRMIN) THEN 
          IR=I 
          PDRMIN=PDR 
        ENDIF 
  150   CONTINUE 
 
C...Recombine very nearby partons to avoid machine precision problems. 
        IF(PDRMIN.LT.PARU12.AND.IR.EQ.N+NR) THEN 
          DO 160 J=1,4 
          P(N+1,J)=P(N+1,J)+P(N+NR,J) 
  160     CONTINUE 
          P(N+1,5)=SQRT(MAX(0.,P(N+1,4)**2-P(N+1,1)**2-P(N+1,2)**2- 
     &    P(N+1,3)**2)) 
          NR=NR-1 
          GOTO 140 
        ELSEIF(PDRMIN.LT.PARU12) THEN 
          DO 170 J=1,4 
          P(IR,J)=P(IR,J)+P(IR+1,J) 
  170     CONTINUE 
          P(IR,5)=SQRT(MAX(0.,P(IR,4)**2-P(IR,1)**2-P(IR,2)**2- 
     &    P(IR,3)**2)) 
          DO 190 I=IR+1,N+NR-1 
          K(I,2)=K(I+1,2) 
          DO 180 J=1,5 
          P(I,J)=P(I+1,J) 
  180     CONTINUE 
  190     CONTINUE 
          IF(IR.EQ.N+NR-1) K(IR,2)=K(N+NR,2) 
          NR=NR-1 
          IF(MJU(1).GT.IR) MJU(1)=MJU(1)-1 
          IF(MJU(2).GT.IR) MJU(2)=MJU(2)-1 
          GOTO 140 
        ENDIF 
      ENDIF 
      NTRYR=NTRYR+1 
 
C...Reset particle counter. Skip ahead if no junctions are present; 
C...this is usually the case! 
      NRS=MAX(5*NR+11,NP) 
      NTRY=0 
  200 NTRY=NTRY+1 
      IF(NTRY.GT.100.AND.NTRYR.LE.4) THEN 
        PARU12=4.*PARU12 
        PARU13=2.*PARU13 
        GOTO 140 
      ELSEIF(NTRY.GT.100) THEN 
        CALL LUERRM0(14,'(LUSTRF0:) caught in infinite loop') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      I=N+NRS 
      MSTU(90)=MSTU90 
      IF(MJU(1).EQ.0.AND.MJU(2).EQ.0) GOTO 580 
      DO 570 JT=1,2 
      NJS(JT)=0 
      IF(MJU(JT).EQ.0) GOTO 570 
      JS=3-2*JT 
 
C...Find and sum up momentum on three sides of junction. Check flavours. 
      DO 220 IU=1,3 
      IJU(IU)=0 
      DO 210 J=1,5 
      PJU(IU,J)=0. 
  210 CONTINUE 
  220 CONTINUE 
      IU=0 
      DO 240 I1=N+1+(JT-1)*(NR-1),N+NR+(JT-1)*(1-NR),JS 
      IF(K(I1,2).NE.21.AND.IU.LE.2) THEN 
        IU=IU+1 
        IJU(IU)=I1 
      ENDIF 
      DO 230 J=1,4 
      PJU(IU,J)=PJU(IU,J)+P(I1,J) 
  230 CONTINUE 
  240 CONTINUE 
      DO 250 IU=1,3 
      PJU(IU,5)=SQRT(PJU(IU,1)**2+PJU(IU,2)**2+PJU(IU,3)**2) 
  250 CONTINUE 
      IF(K(IJU(3),2)/100.NE.10*K(IJU(1),2)+K(IJU(2),2).AND. 
     &K(IJU(3),2)/100.NE.10*K(IJU(2),2)+K(IJU(1),2)) THEN 
        CALL LUERRM0(12,'(LUSTRF0:) unphysical flavour combination') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Calculate (approximate) boost to rest frame of junction. 
      T12=(PJU(1,1)*PJU(2,1)+PJU(1,2)*PJU(2,2)+PJU(1,3)*PJU(2,3))/ 
     &(PJU(1,5)*PJU(2,5)) 
      T13=(PJU(1,1)*PJU(3,1)+PJU(1,2)*PJU(3,2)+PJU(1,3)*PJU(3,3))/ 
     &(PJU(1,5)*PJU(3,5)) 
      T23=(PJU(2,1)*PJU(3,1)+PJU(2,2)*PJU(3,2)+PJU(2,3)*PJU(3,3))/ 
     &(PJU(2,5)*PJU(3,5)) 
      T11=SQRT((2./3.)*(1.-T12)*(1.-T13)/(1.-T23)) 
      T22=SQRT((2./3.)*(1.-T12)*(1.-T23)/(1.-T13)) 
      TSQ=SQRT((2.*T11*T22+T12-1.)*(1.+T12)) 
      T1F=(TSQ-T22*(1.+T12))/(1.-T12**2) 
      T2F=(TSQ-T11*(1.+T12))/(1.-T12**2) 
      DO 260 J=1,3 
      TJU(J)=-(T1F*PJU(1,J)/PJU(1,5)+T2F*PJU(2,J)/PJU(2,5)) 
  260 CONTINUE 
      TJU(4)=SQRT(1.+TJU(1)**2+TJU(2)**2+TJU(3)**2) 
      DO 270 IU=1,3 
      PJU(IU,5)=TJU(4)*PJU(IU,4)-TJU(1)*PJU(IU,1)-TJU(2)*PJU(IU,2)- 
     &TJU(3)*PJU(IU,3) 
  270 CONTINUE 
 
C...Put junction at rest if motion could give inconsistencies. 
      IF(PJU(1,5)+PJU(2,5).GT.PJU(1,4)+PJU(2,4)) THEN 
        DO 280 J=1,3 
        TJU(J)=0. 
  280   CONTINUE 
        TJU(4)=1. 
        PJU(1,5)=PJU(1,4) 
        PJU(2,5)=PJU(2,4) 
        PJU(3,5)=PJU(3,4) 
      ENDIF 
 
C...Start preparing for fragmentation of two strings from junction. 
      ISTA=I 
      DO 550 IU=1,2 
      NS=JS*(IJU(IU+1)-IJU(IU)) 
 
C...Junction strings: find longitudinal string directions. 
      DO 310 IS=1,NS 
      IS1=IJU(IU)+IS-1 
      IS2=IJU(IU)+IS 
      DO 290 J=1,5 
      DP(1,J)=0.5*P(IS1,J) 
      IF(IS.EQ.1) DP(1,J)=P(IS1,J) 
      DP(2,J)=0.5*P(IS2,J) 
      IF(IS.EQ.NS) DP(2,J)=-PJU(IU,J) 
  290 CONTINUE 
      IF(IS.EQ.NS) DP(2,4)=SQRT(PJU(IU,1)**2+PJU(IU,2)**2+PJU(IU,3)**2) 
      IF(IS.EQ.NS) DP(2,5)=0. 
      DP(3,5)=DFOUR(1,1) 
      DP(4,5)=DFOUR(2,2) 
      DHKC=DFOUR(1,2) 
      IF(DP(3,5)+2.*DHKC+DP(4,5).LE.0.) THEN 
        DP(1,4)=SQRT(DP(1,1)**2+DP(1,2)**2+DP(1,3)**2) 
        DP(2,4)=SQRT(DP(2,1)**2+DP(2,2)**2+DP(2,3)**2) 
        DP(3,5)=0D0 
        DP(4,5)=0D0 
        DHKC=DFOUR(1,2) 
      ENDIF 
      DHKS=SQRT(DHKC**2-DP(3,5)*DP(4,5)) 
      DHK1=0.5*((DP(4,5)+DHKC)/DHKS-1.) 
      DHK2=0.5*((DP(3,5)+DHKC)/DHKS-1.) 
      IN1=N+NR+4*IS-3 
      P(IN1,5)=SQRT(DP(3,5)+2.*DHKC+DP(4,5)) 
      DO 300 J=1,4 
      P(IN1,J)=(1.+DHK1)*DP(1,J)-DHK2*DP(2,J) 
      P(IN1+1,J)=(1.+DHK2)*DP(2,J)-DHK1*DP(1,J) 
  300 CONTINUE 
  310 CONTINUE 
 
C...Junction strings: initialize flavour, momentum and starting pos. 
      ISAV=I 
      MSTU91=MSTU(90) 
  320 NTRY=NTRY+1 
      IF(NTRY.GT.100.AND.NTRYR.LE.4) THEN 
        PARU12=4.*PARU12 
        PARU13=2.*PARU13 
        GOTO 140 
      ELSEIF(NTRY.GT.100) THEN 
        CALL LUERRM0(14,'(LUSTRF0:) caught in infinite loop') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      I=ISAV 
      MSTU(90)=MSTU91 
      IRANKJ=0 
      IE(1)=K(N+1+(JT/2)*(NP-1),3) 
      IN(4)=N+NR+1 
      IN(5)=IN(4)+1 
      IN(6)=N+NR+4*NS+1 
      DO 340 JQ=1,2 
      DO 330 IN1=N+NR+2+JQ,N+NR+4*NS-2+JQ,4 
      P(IN1,1)=2-JQ 
      P(IN1,2)=JQ-1 
      P(IN1,3)=1. 
  330 CONTINUE 
  340 CONTINUE 
      KFL(1)=K(IJU(IU),2) 
      PX(1)=0. 
      PY(1)=0. 
      GAM(1)=0. 
      DO 350 J=1,5 
      PJU(IU+3,J)=0. 
  350 CONTINUE 
 
C...Junction strings: find initial transverse directions. 
      DO 360 J=1,4 
      DP(1,J)=P(IN(4),J) 
      DP(2,J)=P(IN(4)+1,J) 
      DP(3,J)=0. 
      DP(4,J)=0. 
  360 CONTINUE 
      DP(1,4)=SQRT(DP(1,1)**2+DP(1,2)**2+DP(1,3)**2) 
      DP(2,4)=SQRT(DP(2,1)**2+DP(2,2)**2+DP(2,3)**2) 
      DP(5,1)=DP(1,1)/DP(1,4)-DP(2,1)/DP(2,4) 
      DP(5,2)=DP(1,2)/DP(1,4)-DP(2,2)/DP(2,4) 
      DP(5,3)=DP(1,3)/DP(1,4)-DP(2,3)/DP(2,4) 
      IF(DP(5,1)**2.LE.DP(5,2)**2+DP(5,3)**2) DP(3,1)=1. 
      IF(DP(5,1)**2.GT.DP(5,2)**2+DP(5,3)**2) DP(3,3)=1. 
      IF(DP(5,2)**2.LE.DP(5,1)**2+DP(5,3)**2) DP(4,2)=1. 
      IF(DP(5,2)**2.GT.DP(5,1)**2+DP(5,3)**2) DP(4,3)=1. 
      DHC12=DFOUR(1,2) 
      DHCX1=DFOUR(3,1)/DHC12 
      DHCX2=DFOUR(3,2)/DHC12 
      DHCXX=1D0/SQRT(1D0+2D0*DHCX1*DHCX2*DHC12) 
      DHCY1=DFOUR(4,1)/DHC12 
      DHCY2=DFOUR(4,2)/DHC12 
      DHCYX=DHCXX*(DHCX1*DHCY2+DHCX2*DHCY1)*DHC12 
      DHCYY=1D0/SQRT(1D0+2D0*DHCY1*DHCY2*DHC12-DHCYX**2) 
      DO 370 J=1,4 
      DP(3,J)=DHCXX*(DP(3,J)-DHCX2*DP(1,J)-DHCX1*DP(2,J)) 
      P(IN(6),J)=DP(3,J) 
      P(IN(6)+1,J)=DHCYY*(DP(4,J)-DHCY2*DP(1,J)-DHCY1*DP(2,J)- 
     &DHCYX*DP(3,J)) 
  370 CONTINUE 
 
C...Junction strings: produce new particle, origin. 
  380 I=I+1 
      IF(2*I-NSAV.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUSTRF0:) no more memory left in LUJETS') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IRANKJ=IRANKJ+1 
      K(I,1)=1 
      K(I,3)=IE(1) 
      K(I,4)=0 
      K(I,5)=0 
 
C...Junction strings: generate flavour, hadron, pT, z and Gamma. 
  390 CALL LUKFDI0(KFL(1),0,KFL(3),K(I,2)) 
      IF(K(I,2).EQ.0) GOTO 320 
      IF(MSTJ(12).GE.3.AND.IRANKJ.EQ.1.AND.IABS(KFL(1)).LE.10.AND. 
     &IABS(KFL(3)).GT.10) THEN 
        IF(RLU0(0).GT.PARJ(19)) GOTO 390 
      ENDIF 
      P(I,5)=ULMASS0(K(I,2)) 
      CALL LUPTDI0(KFL(1),PX(3),PY(3)) 
      PR(1)=P(I,5)**2+(PX(1)+PX(3))**2+(PY(1)+PY(3))**2 
      CALL LUZDIS0(KFL(1),KFL(3),PR(1),Z) 
      IF(IABS(KFL(1)).GE.4.AND.IABS(KFL(1)).LE.8.AND. 
     &MSTU(90).LT.8) THEN 
        MSTU(90)=MSTU(90)+1 
        MSTU(90+MSTU(90))=I 
        PARU(90+MSTU(90))=Z 
      ENDIF 
      GAM(3)=(1.-Z)*(GAM(1)+PR(1)/Z) 
      DO 400 J=1,3 
      IN(J)=IN(3+J) 
  400 CONTINUE 
 
C...Junction strings: stepping within or from 'low' string region easy. 
      IF(IN(1)+1.EQ.IN(2).AND.Z*P(IN(1)+2,3)*P(IN(2)+2,3)* 
     &P(IN(1),5)**2.GE.PR(1)) THEN 
        P(IN(1)+2,4)=Z*P(IN(1)+2,3) 
        P(IN(2)+2,4)=PR(1)/(P(IN(1)+2,4)*P(IN(1),5)**2) 
        DO 410 J=1,4 
        P(I,J)=(PX(1)+PX(3))*P(IN(3),J)+(PY(1)+PY(3))*P(IN(3)+1,J) 
  410   CONTINUE 
        GOTO 500 
      ELSEIF(IN(1)+1.EQ.IN(2)) THEN 
        P(IN(2)+2,4)=P(IN(2)+2,3) 
        P(IN(2)+2,1)=1. 
        IN(2)=IN(2)+4 
        IF(IN(2).GT.N+NR+4*NS) GOTO 320 
        IF(FOUR(IN(1),IN(2)).LE.1E-2) THEN 
          P(IN(1)+2,4)=P(IN(1)+2,3) 
          P(IN(1)+2,1)=0. 
          IN(1)=IN(1)+4 
        ENDIF 
      ENDIF 
 
C...Junction strings: find new transverse directions. 
  420 IF(IN(1).GT.N+NR+4*NS.OR.IN(2).GT.N+NR+4*NS.OR. 
     &IN(1).GT.IN(2)) GOTO 320 
      IF(IN(1).NE.IN(4).OR.IN(2).NE.IN(5)) THEN 
        DO 430 J=1,4 
        DP(1,J)=P(IN(1),J) 
        DP(2,J)=P(IN(2),J) 
        DP(3,J)=0. 
        DP(4,J)=0. 
  430   CONTINUE 
        DP(1,4)=SQRT(DP(1,1)**2+DP(1,2)**2+DP(1,3)**2) 
        DP(2,4)=SQRT(DP(2,1)**2+DP(2,2)**2+DP(2,3)**2) 
        DHC12=DFOUR(1,2) 
        IF(DHC12.LE.1E-2) THEN 
          P(IN(1)+2,4)=P(IN(1)+2,3) 
          P(IN(1)+2,1)=0. 
          IN(1)=IN(1)+4 
          GOTO 420 
        ENDIF 
        IN(3)=N+NR+4*NS+5 
        DP(5,1)=DP(1,1)/DP(1,4)-DP(2,1)/DP(2,4) 
        DP(5,2)=DP(1,2)/DP(1,4)-DP(2,2)/DP(2,4) 
        DP(5,3)=DP(1,3)/DP(1,4)-DP(2,3)/DP(2,4) 
        IF(DP(5,1)**2.LE.DP(5,2)**2+DP(5,3)**2) DP(3,1)=1. 
        IF(DP(5,1)**2.GT.DP(5,2)**2+DP(5,3)**2) DP(3,3)=1. 
        IF(DP(5,2)**2.LE.DP(5,1)**2+DP(5,3)**2) DP(4,2)=1. 
        IF(DP(5,2)**2.GT.DP(5,1)**2+DP(5,3)**2) DP(4,3)=1. 
        DHCX1=DFOUR(3,1)/DHC12 
        DHCX2=DFOUR(3,2)/DHC12 
        DHCXX=1D0/SQRT(1D0+2D0*DHCX1*DHCX2*DHC12) 
        DHCY1=DFOUR(4,1)/DHC12 
        DHCY2=DFOUR(4,2)/DHC12 
        DHCYX=DHCXX*(DHCX1*DHCY2+DHCX2*DHCY1)*DHC12 
        DHCYY=1D0/SQRT(1D0+2D0*DHCY1*DHCY2*DHC12-DHCYX**2) 
        DO 440 J=1,4 
        DP(3,J)=DHCXX*(DP(3,J)-DHCX2*DP(1,J)-DHCX1*DP(2,J)) 
        P(IN(3),J)=DP(3,J) 
        P(IN(3)+1,J)=DHCYY*(DP(4,J)-DHCY2*DP(1,J)-DHCY1*DP(2,J)- 
     &  DHCYX*DP(3,J)) 
  440   CONTINUE 
C...Express pT with respect to new axes, if sensible. 
        PXP=-(PX(3)*FOUR(IN(6),IN(3))+PY(3)*FOUR(IN(6)+1,IN(3))) 
        PYP=-(PX(3)*FOUR(IN(6),IN(3)+1)+PY(3)*FOUR(IN(6)+1,IN(3)+1)) 
        IF(ABS(PXP**2+PYP**2-PX(3)**2-PY(3)**2).LT.0.01) THEN 
          PX(3)=PXP 
          PY(3)=PYP 
        ENDIF 
      ENDIF 
 
C...Junction strings: sum up known four-momentum, coefficients for m2. 
      DO 470 J=1,4 
      DHG(J)=0. 
      P(I,J)=PX(1)*P(IN(6),J)+PY(1)*P(IN(6)+1,J)+PX(3)*P(IN(3),J)+ 
     &PY(3)*P(IN(3)+1,J) 
      DO 450 IN1=IN(4),IN(1)-4,4 
      P(I,J)=P(I,J)+P(IN1+2,3)*P(IN1,J) 
  450 CONTINUE 
      DO 460 IN2=IN(5),IN(2)-4,4 
      P(I,J)=P(I,J)+P(IN2+2,3)*P(IN2,J) 
  460 CONTINUE 
  470 CONTINUE 
      DHM(1)=FOUR(I,I) 
      DHM(2)=2.*FOUR(I,IN(1)) 
      DHM(3)=2.*FOUR(I,IN(2)) 
      DHM(4)=2.*FOUR(IN(1),IN(2)) 
 
C...Junction strings: find coefficients for Gamma expression. 
      DO 490 IN2=IN(1)+1,IN(2),4 
      DO 480 IN1=IN(1),IN2-1,4 
      DHC=2.*FOUR(IN1,IN2) 
      DHG(1)=DHG(1)+P(IN1+2,1)*P(IN2+2,1)*DHC 
      IF(IN1.EQ.IN(1)) DHG(2)=DHG(2)-P(IN2+2,1)*DHC 
      IF(IN2.EQ.IN(2)) DHG(3)=DHG(3)+P(IN1+2,1)*DHC 
      IF(IN1.EQ.IN(1).AND.IN2.EQ.IN(2)) DHG(4)=DHG(4)-DHC 
  480 CONTINUE 
  490 CONTINUE 
 
C...Junction strings: solve (m2, Gamma) equation system for energies. 
      DHS1=DHM(3)*DHG(4)-DHM(4)*DHG(3) 
      IF(ABS(DHS1).LT.1E-4) GOTO 320 
      DHS2=DHM(4)*(GAM(3)-DHG(1))-DHM(2)*DHG(3)-DHG(4)* 
     &(P(I,5)**2-DHM(1))+DHG(2)*DHM(3) 
      DHS3=DHM(2)*(GAM(3)-DHG(1))-DHG(2)*(P(I,5)**2-DHM(1)) 
      P(IN(2)+2,4)=0.5*(SQRT(MAX(0D0,DHS2**2-4.*DHS1*DHS3))/ABS(DHS1)- 
     &DHS2/DHS1) 
      IF(DHM(2)+DHM(4)*P(IN(2)+2,4).LE.0.) GOTO 320 
      P(IN(1)+2,4)=(P(I,5)**2-DHM(1)-DHM(3)*P(IN(2)+2,4))/ 
     &(DHM(2)+DHM(4)*P(IN(2)+2,4)) 
 
C...Junction strings: step to new region if necessary. 
      IF(P(IN(2)+2,4).GT.P(IN(2)+2,3)) THEN 
        P(IN(2)+2,4)=P(IN(2)+2,3) 
        P(IN(2)+2,1)=1. 
        IN(2)=IN(2)+4 
        IF(IN(2).GT.N+NR+4*NS) GOTO 320 
        IF(FOUR(IN(1),IN(2)).LE.1E-2) THEN 
          P(IN(1)+2,4)=P(IN(1)+2,3) 
          P(IN(1)+2,1)=0. 
          IN(1)=IN(1)+4 
        ENDIF 
        GOTO 420 
      ELSEIF(P(IN(1)+2,4).GT.P(IN(1)+2,3)) THEN 
        P(IN(1)+2,4)=P(IN(1)+2,3) 
        P(IN(1)+2,1)=0. 
        IN(1)=IN(1)+JS 
        GOTO 820 
      ENDIF 
 
C...Junction strings: particle four-momentum, remainder, loop back. 
  500 DO 510 J=1,4 
      P(I,J)=P(I,J)+P(IN(1)+2,4)*P(IN(1),J)+P(IN(2)+2,4)*P(IN(2),J) 
      PJU(IU+3,J)=PJU(IU+3,J)+P(I,J) 
  510 CONTINUE 
      IF(P(I,4).LT.P(I,5)) GOTO 320 
      PJU(IU+3,5)=TJU(4)*PJU(IU+3,4)-TJU(1)*PJU(IU+3,1)- 
     &TJU(2)*PJU(IU+3,2)-TJU(3)*PJU(IU+3,3) 
      IF(PJU(IU+3,5).LT.PJU(IU,5)) THEN 
        KFL(1)=-KFL(3) 
        PX(1)=-PX(3) 
        PY(1)=-PY(3) 
        GAM(1)=GAM(3) 
        IF(IN(3).NE.IN(6)) THEN 
          DO 520 J=1,4 
          P(IN(6),J)=P(IN(3),J) 
          P(IN(6)+1,J)=P(IN(3)+1,J) 
  520     CONTINUE 
        ENDIF 
        DO 530 JQ=1,2 
        IN(3+JQ)=IN(JQ) 
        P(IN(JQ)+2,3)=P(IN(JQ)+2,3)-P(IN(JQ)+2,4) 
        P(IN(JQ)+2,1)=P(IN(JQ)+2,1)-(3-2*JQ)*P(IN(JQ)+2,4) 
  530   CONTINUE 
        GOTO 380 
      ENDIF 
 
C...Junction strings: save quantities left after each string. 
      IF(IABS(KFL(1)).GT.10) GOTO 320 
      I=I-1 
      KFJH(IU)=KFL(1) 
      DO 540 J=1,4 
      PJU(IU+3,J)=PJU(IU+3,J)-P(I+1,J) 
  540 CONTINUE 
  550 CONTINUE 
 
C...Junction strings: put together to new effective string endpoint. 
      NJS(JT)=I-ISTA 
      KFJS(JT)=K(K(MJU(JT+2),3),2) 
      KFLS=2*INT(RLU0(0)+3.*PARJ(4)/(1.+3.*PARJ(4)))+1 
      IF(KFJH(1).EQ.KFJH(2)) KFLS=3 
      IF(ISTA.NE.I) KFJS(JT)=ISIGN(1000*MAX(IABS(KFJH(1)), 
     &IABS(KFJH(2)))+100*MIN(IABS(KFJH(1)),IABS(KFJH(2)))+ 
     &KFLS,KFJH(1)) 
      DO 560 J=1,4 
      PJS(JT,J)=PJU(1,J)+PJU(2,J)+P(MJU(JT),J) 
      PJS(JT+2,J)=PJU(4,J)+PJU(5,J) 
  560 CONTINUE 
      PJS(JT,5)=SQRT(MAX(0.,PJS(JT,4)**2-PJS(JT,1)**2-PJS(JT,2)**2- 
     &PJS(JT,3)**2)) 
  570 CONTINUE 
 
C...Open versus closed strings. Choose breakup region for latter. 
  580 IF(MJU(1).NE.0.AND.MJU(2).NE.0) THEN 
        NS=MJU(2)-MJU(1) 
        NB=MJU(1)-N 
      ELSEIF(MJU(1).NE.0) THEN 
        NS=N+NR-MJU(1) 
        NB=MJU(1)-N 
      ELSEIF(MJU(2).NE.0) THEN 
        NS=MJU(2)-N 
        NB=1 
      ELSEIF(IABS(K(N+1,2)).NE.21) THEN 
        NS=NR-1 
        NB=1 
      ELSE 
        NS=NR+1 
        W2SUM=0. 
        DO 590 IS=1,NR 
        P(N+NR+IS,1)=0.5*FOUR(N+IS,N+IS+1-NR*(IS/NR)) 
        W2SUM=W2SUM+P(N+NR+IS,1) 
  590   CONTINUE 
        W2RAN=RLU0(0)*W2SUM 
        NB=0 
  600   NB=NB+1 
        W2SUM=W2SUM-P(N+NR+NB,1) 
        IF(W2SUM.GT.W2RAN.AND.NB.LT.NR) GOTO 600 
      ENDIF 
 
C...Find longitudinal string directions (i.e. lightlike four-vectors). 
      DO 630 IS=1,NS 
      IS1=N+IS+NB-1-NR*((IS+NB-2)/NR) 
      IS2=N+IS+NB-NR*((IS+NB-1)/NR) 
      DO 610 J=1,5 
      DP(1,J)=P(IS1,J) 
      IF(IABS(K(IS1,2)).EQ.21) DP(1,J)=0.5*DP(1,J) 
      IF(IS1.EQ.MJU(1)) DP(1,J)=PJS(1,J)-PJS(3,J) 
      DP(2,J)=P(IS2,J) 
      IF(IABS(K(IS2,2)).EQ.21) DP(2,J)=0.5*DP(2,J) 
      IF(IS2.EQ.MJU(2)) DP(2,J)=PJS(2,J)-PJS(4,J) 
  610 CONTINUE 
      DP(3,5)=DFOUR(1,1) 
      DP(4,5)=DFOUR(2,2) 
      DHKC=DFOUR(1,2) 
      IF(DP(3,5)+2.*DHKC+DP(4,5).LE.0.) THEN 
        DP(3,5)=DP(1,5)**2 
        DP(4,5)=DP(2,5)**2 
        DP(1,4)=SQRT(DP(1,1)**2+DP(1,2)**2+DP(1,3)**2+DP(1,5)**2) 
        DP(2,4)=SQRT(DP(2,1)**2+DP(2,2)**2+DP(2,3)**2+DP(2,5)**2) 
        DHKC=DFOUR(1,2) 
      ENDIF 
      DHKS=SQRT(DHKC**2-DP(3,5)*DP(4,5)) 
      DHK1=0.5*((DP(4,5)+DHKC)/DHKS-1.) 
      DHK2=0.5*((DP(3,5)+DHKC)/DHKS-1.) 
      IN1=N+NR+4*IS-3 
      P(IN1,5)=SQRT(DP(3,5)+2.*DHKC+DP(4,5)) 
      DO 620 J=1,4 
      P(IN1,J)=(1.+DHK1)*DP(1,J)-DHK2*DP(2,J) 
      P(IN1+1,J)=(1.+DHK2)*DP(2,J)-DHK1*DP(1,J) 
  620 CONTINUE 
  630 CONTINUE 
 
C...Begin initialization: sum up energy, set starting position. 
      ISAV=I 
      MSTU91=MSTU(90) 
  640 NTRY=NTRY+1 
      IF(NTRY.GT.100.AND.NTRYR.LE.4) THEN 
        PARU12=4.*PARU12 
        PARU13=2.*PARU13 
        GOTO 140 
      ELSEIF(NTRY.GT.100) THEN 
        CALL LUERRM0(14,'(LUSTRF0:) caught in infinite loop') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      I=ISAV 
      MSTU(90)=MSTU91 
      DO 660 J=1,4 
      P(N+NRS,J)=0. 
      DO 650 IS=1,NR 
      P(N+NRS,J)=P(N+NRS,J)+P(N+IS,J) 
  650 CONTINUE 
  660 CONTINUE 
      DO 680 JT=1,2 
      IRANK(JT)=0 
      IF(MJU(JT).NE.0) IRANK(JT)=NJS(JT) 
      IF(NS.GT.NR) IRANK(JT)=1 
      IE(JT)=K(N+1+(JT/2)*(NP-1),3) 
      IN(3*JT+1)=N+NR+1+4*(JT/2)*(NS-1) 
      IN(3*JT+2)=IN(3*JT+1)+1 
      IN(3*JT+3)=N+NR+4*NS+2*JT-1 
      DO 670 IN1=N+NR+2+JT,N+NR+4*NS-2+JT,4 
      P(IN1,1)=2-JT 
      P(IN1,2)=JT-1 
      P(IN1,3)=1. 
  670 CONTINUE 
  680 CONTINUE 
 
C...Initialize flavour and pT variables for open string. 
      IF(NS.LT.NR) THEN 
        PX(1)=0. 
        PY(1)=0. 
        IF(NS.EQ.1.AND.MJU(1)+MJU(2).EQ.0) CALL LUPTDI0(0,PX(1),PY(1)) 
        PX(2)=-PX(1) 
        PY(2)=-PY(1) 
        DO 690 JT=1,2 
        KFL(JT)=K(IE(JT),2) 
        IF(MJU(JT).NE.0) KFL(JT)=KFJS(JT) 
        MSTJ(93)=1 
        PMQ(JT)=ULMASS0(KFL(JT)) 
        GAM(JT)=0. 
  690   CONTINUE 
 
C...Closed string: random initial breakup flavour, pT and vertex. 
      ELSE 
        KFL(3)=INT(1.+(2.+PARJ(2))*RLU0(0))*(-1)**INT(RLU0(0)+0.5) 
        CALL LUKFDI0(KFL(3),0,KFL(1),KDUMP) 
        KFL(2)=-KFL(1) 
        IF(IABS(KFL(1)).GT.10.AND.RLU0(0).GT.0.5) THEN 
          KFL(2)=-(KFL(1)+ISIGN(10000,KFL(1))) 
        ELSEIF(IABS(KFL(1)).GT.10) THEN 
          KFL(1)=-(KFL(2)+ISIGN(10000,KFL(2))) 
        ENDIF 
        CALL LUPTDI0(KFL(1),PX(1),PY(1)) 
        PX(2)=-PX(1) 
        PY(2)=-PY(1) 
        PR3=MIN(25.,0.1*P(N+NR+1,5)**2) 
  700   CALL LUZDIS0(KFL(1),KFL(2),PR3,Z) 
        ZR=PR3/(Z*P(N+NR+1,5)**2) 
        IF(ZR.GE.1.) GOTO 700 
        DO 710 JT=1,2 
        MSTJ(93)=1 
        PMQ(JT)=ULMASS0(KFL(JT)) 
        GAM(JT)=PR3*(1.-Z)/Z 
        IN1=N+NR+3+4*(JT/2)*(NS-1) 
        P(IN1,JT)=1.-Z 
        P(IN1,3-JT)=JT-1 
        P(IN1,3)=(2-JT)*(1.-Z)+(JT-1)*Z 
        P(IN1+1,JT)=ZR 
        P(IN1+1,3-JT)=2-JT 
        P(IN1+1,3)=(2-JT)*(1.-ZR)+(JT-1)*ZR 
  710   CONTINUE 
      ENDIF 
 
C...Find initial transverse directions (i.e. spacelike four-vectors). 
      DO 750 JT=1,2 
      IF(JT.EQ.1.OR.NS.EQ.NR-1) THEN 
        IN1=IN(3*JT+1) 
        IN3=IN(3*JT+3) 
        DO 720 J=1,4 
        DP(1,J)=P(IN1,J) 
        DP(2,J)=P(IN1+1,J) 
        DP(3,J)=0. 
        DP(4,J)=0. 
  720   CONTINUE 
        DP(1,4)=SQRT(DP(1,1)**2+DP(1,2)**2+DP(1,3)**2) 
        DP(2,4)=SQRT(DP(2,1)**2+DP(2,2)**2+DP(2,3)**2) 
        DP(5,1)=DP(1,1)/DP(1,4)-DP(2,1)/DP(2,4) 
        DP(5,2)=DP(1,2)/DP(1,4)-DP(2,2)/DP(2,4) 
        DP(5,3)=DP(1,3)/DP(1,4)-DP(2,3)/DP(2,4) 
        IF(DP(5,1)**2.LE.DP(5,2)**2+DP(5,3)**2) DP(3,1)=1. 
        IF(DP(5,1)**2.GT.DP(5,2)**2+DP(5,3)**2) DP(3,3)=1. 
        IF(DP(5,2)**2.LE.DP(5,1)**2+DP(5,3)**2) DP(4,2)=1. 
        IF(DP(5,2)**2.GT.DP(5,1)**2+DP(5,3)**2) DP(4,3)=1. 
        DHC12=DFOUR(1,2) 
        DHCX1=DFOUR(3,1)/DHC12 
        DHCX2=DFOUR(3,2)/DHC12 
        DHCXX=1D0/SQRT(1D0+2D0*DHCX1*DHCX2*DHC12) 
        DHCY1=DFOUR(4,1)/DHC12 
        DHCY2=DFOUR(4,2)/DHC12 
        DHCYX=DHCXX*(DHCX1*DHCY2+DHCX2*DHCY1)*DHC12 
        DHCYY=1D0/SQRT(1D0+2D0*DHCY1*DHCY2*DHC12-DHCYX**2) 
        DO 730 J=1,4 
        DP(3,J)=DHCXX*(DP(3,J)-DHCX2*DP(1,J)-DHCX1*DP(2,J)) 
        P(IN3,J)=DP(3,J) 
        P(IN3+1,J)=DHCYY*(DP(4,J)-DHCY2*DP(1,J)-DHCY1*DP(2,J)- 
     &  DHCYX*DP(3,J)) 
  730   CONTINUE 
      ELSE 
        DO 740 J=1,4 
        P(IN3+2,J)=P(IN3,J) 
        P(IN3+3,J)=P(IN3+1,J) 
  740   CONTINUE 
      ENDIF 
  750 CONTINUE 
 
C...Remove energy used up in junction string fragmentation. 
      IF(MJU(1)+MJU(2).GT.0) THEN 
        DO 770 JT=1,2 
        IF(NJS(JT).EQ.0) GOTO 770 
        DO 760 J=1,4 
        P(N+NRS,J)=P(N+NRS,J)-PJS(JT+2,J) 
  760   CONTINUE 
  770   CONTINUE 
      ENDIF 
 
C...Produce new particle: side, origin. 
  780 I=I+1 
      IF(2*I-NSAV.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUSTRF0:) no more memory left in LUJETS') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      JT=1.5+RLU0(0) 
      IF(IABS(KFL(3-JT)).GT.10) JT=3-JT 
      IF(IABS(KFL(3-JT)).GE.4.AND.IABS(KFL(3-JT)).LE.8) JT=3-JT 
      JR=3-JT 
      JS=3-2*JT 
      IRANK(JT)=IRANK(JT)+1 
      K(I,1)=1 
      K(I,3)=IE(JT) 
      K(I,4)=0 
      K(I,5)=0 
 
C...Generate flavour, hadron and pT. 
  790 CALL LUKFDI0(KFL(JT),0,KFL(3),K(I,2)) 
      IF(K(I,2).EQ.0) GOTO 640 
      IF(MSTJ(12).GE.3.AND.IRANK(JT).EQ.1.AND.IABS(KFL(JT)).LE.10.AND. 
     &IABS(KFL(3)).GT.10) THEN 
        IF(RLU0(0).GT.PARJ(19)) GOTO 790 
      ENDIF 
      P(I,5)=ULMASS0(K(I,2)) 
      CALL LUPTDI0(KFL(JT),PX(3),PY(3)) 
      PR(JT)=P(I,5)**2+(PX(JT)+PX(3))**2+(PY(JT)+PY(3))**2 
 
C...Final hadrons for small invariant mass. 
      MSTJ(93)=1 
      PMQ(3)=ULMASS0(KFL(3)) 
      PARJST=PARJ(33) 
      IF(MSTJ(11).EQ.2) PARJST=PARJ(34) 
      WMIN=PARJST+PMQ(1)+PMQ(2)+PARJ(36)*PMQ(3) 
      IF(IABS(KFL(JT)).GT.10.AND.IABS(KFL(3)).GT.10) WMIN= 
     &WMIN-0.5*PARJ(36)*PMQ(3) 
      WREM2=FOUR(N+NRS,N+NRS) 
      IF(WREM2.LT.0.10) GOTO 640 
      IF(WREM2.LT.MAX(WMIN*(1.+(2.*RLU0(0)-1.)*PARJ(37)), 
     &PARJ(32)+PMQ(1)+PMQ(2))**2) GOTO 940 
 
C...Choose z, which gives Gamma. Shift z for heavy flavours. 
      CALL LUZDIS0(KFL(JT),KFL(3),PR(JT),Z) 
      IF(IABS(KFL(JT)).GE.4.AND.IABS(KFL(JT)).LE.8.AND. 
     &MSTU(90).LT.8) THEN 
        MSTU(90)=MSTU(90)+1 
        MSTU(90+MSTU(90))=I 
        PARU(90+MSTU(90))=Z 
      ENDIF 
      KFL1A=IABS(KFL(1)) 
      KFL2A=IABS(KFL(2)) 
      IF(MAX(MOD(KFL1A,10),MOD(KFL1A/1000,10),MOD(KFL2A,10), 
     &MOD(KFL2A/1000,10)).GE.4) THEN 
        PR(JR)=(PMQ(JR)+PMQ(3))**2+(PX(JR)-PX(3))**2+(PY(JR)-PY(3))**2 
        PW12=SQRT(MAX(0.,(WREM2-PR(1)-PR(2))**2-4.*PR(1)*PR(2))) 
        Z=(WREM2+PR(JT)-PR(JR)+PW12*(2.*Z-1.))/(2.*WREM2) 
        PR(JR)=(PMQ(JR)+PARJST)**2+(PX(JR)-PX(3))**2+(PY(JR)-PY(3))**2 
        IF((1.-Z)*(WREM2-PR(JT)/Z).LT.PR(JR)) GOTO 940 
      ENDIF 
      GAM(3)=(1.-Z)*(GAM(JT)+PR(JT)/Z) 
      DO 800 J=1,3 
      IN(J)=IN(3*JT+J) 
  800 CONTINUE 
 
C...Stepping within or from 'low' string region easy. 
      IF(IN(1)+1.EQ.IN(2).AND.Z*P(IN(1)+2,3)*P(IN(2)+2,3)* 
     &P(IN(1),5)**2.GE.PR(JT)) THEN 
        P(IN(JT)+2,4)=Z*P(IN(JT)+2,3) 
        P(IN(JR)+2,4)=PR(JT)/(P(IN(JT)+2,4)*P(IN(1),5)**2) 
        DO 810 J=1,4 
        P(I,J)=(PX(JT)+PX(3))*P(IN(3),J)+(PY(JT)+PY(3))*P(IN(3)+1,J) 
  810   CONTINUE 
        GOTO 900 
      ELSEIF(IN(1)+1.EQ.IN(2)) THEN 
        P(IN(JR)+2,4)=P(IN(JR)+2,3) 
        P(IN(JR)+2,JT)=1. 
        IN(JR)=IN(JR)+4*JS 
        IF(JS*IN(JR).GT.JS*IN(4*JR)) GOTO 640 
        IF(FOUR(IN(1),IN(2)).LE.1E-2) THEN 
          P(IN(JT)+2,4)=P(IN(JT)+2,3) 
          P(IN(JT)+2,JT)=0. 
          IN(JT)=IN(JT)+4*JS 
        ENDIF 
      ENDIF 
 
C...Find new transverse directions (i.e. spacelike string vectors). 
  820 IF(JS*IN(1).GT.JS*IN(3*JR+1).OR.JS*IN(2).GT.JS*IN(3*JR+2).OR. 
     &IN(1).GT.IN(2)) GOTO 640 
      IF(IN(1).NE.IN(3*JT+1).OR.IN(2).NE.IN(3*JT+2)) THEN 
        DO 830 J=1,4 
        DP(1,J)=P(IN(1),J) 
        DP(2,J)=P(IN(2),J) 
        DP(3,J)=0. 
        DP(4,J)=0. 
  830   CONTINUE 
        DP(1,4)=SQRT(DP(1,1)**2+DP(1,2)**2+DP(1,3)**2) 
        DP(2,4)=SQRT(DP(2,1)**2+DP(2,2)**2+DP(2,3)**2) 
        DHC12=DFOUR(1,2) 
        IF(DHC12.LE.1E-2) THEN 
          P(IN(JT)+2,4)=P(IN(JT)+2,3) 
          P(IN(JT)+2,JT)=0. 
          IN(JT)=IN(JT)+4*JS 
          GOTO 820 
        ENDIF 
        IN(3)=N+NR+4*NS+5 
        DP(5,1)=DP(1,1)/DP(1,4)-DP(2,1)/DP(2,4) 
        DP(5,2)=DP(1,2)/DP(1,4)-DP(2,2)/DP(2,4) 
        DP(5,3)=DP(1,3)/DP(1,4)-DP(2,3)/DP(2,4) 
        IF(DP(5,1)**2.LE.DP(5,2)**2+DP(5,3)**2) DP(3,1)=1. 
        IF(DP(5,1)**2.GT.DP(5,2)**2+DP(5,3)**2) DP(3,3)=1. 
        IF(DP(5,2)**2.LE.DP(5,1)**2+DP(5,3)**2) DP(4,2)=1. 
        IF(DP(5,2)**2.GT.DP(5,1)**2+DP(5,3)**2) DP(4,3)=1. 
        DHCX1=DFOUR(3,1)/DHC12 
        DHCX2=DFOUR(3,2)/DHC12 
        DHCXX=1D0/SQRT(1D0+2D0*DHCX1*DHCX2*DHC12) 
        DHCY1=DFOUR(4,1)/DHC12 
        DHCY2=DFOUR(4,2)/DHC12 
        DHCYX=DHCXX*(DHCX1*DHCY2+DHCX2*DHCY1)*DHC12 
        DHCYY=1D0/SQRT(1D0+2D0*DHCY1*DHCY2*DHC12-DHCYX**2) 
        DO 840 J=1,4 
        DP(3,J)=DHCXX*(DP(3,J)-DHCX2*DP(1,J)-DHCX1*DP(2,J)) 
        P(IN(3),J)=DP(3,J) 
        P(IN(3)+1,J)=DHCYY*(DP(4,J)-DHCY2*DP(1,J)-DHCY1*DP(2,J)- 
     &  DHCYX*DP(3,J)) 
  840   CONTINUE 
C...Express pT with respect to new axes, if sensible. 
        PXP=-(PX(3)*FOUR(IN(3*JT+3),IN(3))+PY(3)* 
     &  FOUR(IN(3*JT+3)+1,IN(3))) 
        PYP=-(PX(3)*FOUR(IN(3*JT+3),IN(3)+1)+PY(3)* 
     &  FOUR(IN(3*JT+3)+1,IN(3)+1)) 
        IF(ABS(PXP**2+PYP**2-PX(3)**2-PY(3)**2).LT.0.01) THEN 
          PX(3)=PXP 
          PY(3)=PYP 
        ENDIF 
      ENDIF 
 
C...Sum up known four-momentum. Gives coefficients for m2 expression. 
      DO 870 J=1,4 
      DHG(J)=0. 
      P(I,J)=PX(JT)*P(IN(3*JT+3),J)+PY(JT)*P(IN(3*JT+3)+1,J)+ 
     &PX(3)*P(IN(3),J)+PY(3)*P(IN(3)+1,J) 
      DO 850 IN1=IN(3*JT+1),IN(1)-4*JS,4*JS 
      P(I,J)=P(I,J)+P(IN1+2,3)*P(IN1,J) 
  850 CONTINUE 
      DO 860 IN2=IN(3*JT+2),IN(2)-4*JS,4*JS 
      P(I,J)=P(I,J)+P(IN2+2,3)*P(IN2,J) 
  860 CONTINUE 
  870 CONTINUE 
      DHM(1)=FOUR(I,I) 
      DHM(2)=2.*FOUR(I,IN(1)) 
      DHM(3)=2.*FOUR(I,IN(2)) 
      DHM(4)=2.*FOUR(IN(1),IN(2)) 
 
C...Find coefficients for Gamma expression. 
      DO 890 IN2=IN(1)+1,IN(2),4 
      DO 880 IN1=IN(1),IN2-1,4 
      DHC=2.*FOUR(IN1,IN2) 
      DHG(1)=DHG(1)+P(IN1+2,JT)*P(IN2+2,JT)*DHC 
      IF(IN1.EQ.IN(1)) DHG(2)=DHG(2)-JS*P(IN2+2,JT)*DHC 
      IF(IN2.EQ.IN(2)) DHG(3)=DHG(3)+JS*P(IN1+2,JT)*DHC 
      IF(IN1.EQ.IN(1).AND.IN2.EQ.IN(2)) DHG(4)=DHG(4)-DHC 
  880 CONTINUE 
  890 CONTINUE 
 
C...Solve (m2, Gamma) equation system for energies taken. 
      DHS1=DHM(JR+1)*DHG(4)-DHM(4)*DHG(JR+1) 
      IF(ABS(DHS1).LT.1E-4) GOTO 640 
      DHS2=DHM(4)*(GAM(3)-DHG(1))-DHM(JT+1)*DHG(JR+1)-DHG(4)* 
     &(P(I,5)**2-DHM(1))+DHG(JT+1)*DHM(JR+1) 
      DHS3=DHM(JT+1)*(GAM(3)-DHG(1))-DHG(JT+1)*(P(I,5)**2-DHM(1)) 
      P(IN(JR)+2,4)=0.5*(SQRT(MAX(0D0,DHS2**2-4.*DHS1*DHS3))/ABS(DHS1)- 
     &DHS2/DHS1) 
      IF(DHM(JT+1)+DHM(4)*P(IN(JR)+2,4).LE.0.) GOTO 640 
      P(IN(JT)+2,4)=(P(I,5)**2-DHM(1)-DHM(JR+1)*P(IN(JR)+2,4))/ 
     &(DHM(JT+1)+DHM(4)*P(IN(JR)+2,4)) 
 
C...Step to new region if necessary. 
      IF(P(IN(JR)+2,4).GT.P(IN(JR)+2,3)) THEN 
        P(IN(JR)+2,4)=P(IN(JR)+2,3) 
        P(IN(JR)+2,JT)=1. 
        IN(JR)=IN(JR)+4*JS 
        IF(JS*IN(JR).GT.JS*IN(4*JR)) GOTO 640 
        IF(FOUR(IN(1),IN(2)).LE.1E-2) THEN 
          P(IN(JT)+2,4)=P(IN(JT)+2,3) 
          P(IN(JT)+2,JT)=0. 
          IN(JT)=IN(JT)+4*JS 
        ENDIF 
        GOTO 820 
      ELSEIF(P(IN(JT)+2,4).GT.P(IN(JT)+2,3)) THEN 
        P(IN(JT)+2,4)=P(IN(JT)+2,3) 
        P(IN(JT)+2,JT)=0. 
        IN(JT)=IN(JT)+4*JS 
        GOTO 820 
      ENDIF 
 
C...Four-momentum of particle. Remaining quantities. Loop back. 
  900 DO 910 J=1,4 
      P(I,J)=P(I,J)+P(IN(1)+2,4)*P(IN(1),J)+P(IN(2)+2,4)*P(IN(2),J) 
      P(N+NRS,J)=P(N+NRS,J)-P(I,J) 
  910 CONTINUE 
      IF(P(I,4).LT.P(I,5)) GOTO 640 
      KFL(JT)=-KFL(3) 
      PMQ(JT)=PMQ(3) 
      PX(JT)=-PX(3) 
      PY(JT)=-PY(3) 
      GAM(JT)=GAM(3) 
      IF(IN(3).NE.IN(3*JT+3)) THEN 
        DO 920 J=1,4 
        P(IN(3*JT+3),J)=P(IN(3),J) 
        P(IN(3*JT+3)+1,J)=P(IN(3)+1,J) 
  920   CONTINUE 
      ENDIF 
      DO 930 JQ=1,2 
      IN(3*JT+JQ)=IN(JQ) 
      P(IN(JQ)+2,3)=P(IN(JQ)+2,3)-P(IN(JQ)+2,4) 
      P(IN(JQ)+2,JT)=P(IN(JQ)+2,JT)-JS*(3-2*JQ)*P(IN(JQ)+2,4) 
  930 CONTINUE 
      GOTO 780 
 
C...Final hadron: side, flavour, hadron, mass. 
  940 I=I+1 
      K(I,1)=1 
      K(I,3)=IE(JR) 
      K(I,4)=0 
      K(I,5)=0 
      CALL LUKFDI0(KFL(JR),-KFL(3),KFLDMP,K(I,2)) 
      IF(K(I,2).EQ.0) GOTO 640 
      P(I,5)=ULMASS0(K(I,2)) 
      PR(JR)=P(I,5)**2+(PX(JR)-PX(3))**2+(PY(JR)-PY(3))**2 
 
C...Final two hadrons: find common setup of four-vectors. 
      JQ=1 
      IF(P(IN(4)+2,3)*P(IN(5)+2,3)*FOUR(IN(4),IN(5)).LT.P(IN(7),3)* 
     &P(IN(8),3)*FOUR(IN(7),IN(8))) JQ=2 
      DHC12=FOUR(IN(3*JQ+1),IN(3*JQ+2)) 
      DHR1=FOUR(N+NRS,IN(3*JQ+2))/DHC12 
      DHR2=FOUR(N+NRS,IN(3*JQ+1))/DHC12 
      IF(IN(4).NE.IN(7).OR.IN(5).NE.IN(8)) THEN 
        PX(3-JQ)=-FOUR(N+NRS,IN(3*JQ+3))-PX(JQ) 
        PY(3-JQ)=-FOUR(N+NRS,IN(3*JQ+3)+1)-PY(JQ) 
        PR(3-JQ)=P(I+(JT+JQ-3)**2-1,5)**2+(PX(3-JQ)+(2*JQ-3)*JS* 
     &  PX(3))**2+(PY(3-JQ)+(2*JQ-3)*JS*PY(3))**2 
      ENDIF 
 
C...Solve kinematics for final two hadrons, if possible. 
      WREM2=WREM2+(PX(1)+PX(2))**2+(PY(1)+PY(2))**2 
      FD=(SQRT(PR(1))+SQRT(PR(2)))/SQRT(WREM2) 
      IF(MJU(1)+MJU(2).NE.0.AND.I.EQ.ISAV+2.AND.FD.GE.1.) GOTO 200 
      IF(FD.GE.1.) GOTO 640 
      FA=WREM2+PR(JT)-PR(JR) 
      IF(MSTJ(11).NE.2) PREV=0.5*EXP(MAX(-50.,LOG(FD)*PARJ(38)* 
     &(PR(1)+PR(2))**2)) 
      IF(MSTJ(11).EQ.2) PREV=0.5*FD**PARJ(39) 
      FB=SIGN(SQRT(MAX(0.,FA**2-4.*WREM2*PR(JT))),JS*(RLU0(0)-PREV)) 
      KFL1A=IABS(KFL(1)) 
      KFL2A=IABS(KFL(2)) 
      IF(MAX(MOD(KFL1A,10),MOD(KFL1A/1000,10),MOD(KFL2A,10), 
     &MOD(KFL2A/1000,10)).GE.6) FB=SIGN(SQRT(MAX(0.,FA**2- 
     &4.*WREM2*PR(JT))),FLOAT(JS)) 
      DO 950 J=1,4 
      P(I-1,J)=(PX(JT)+PX(3))*P(IN(3*JQ+3),J)+(PY(JT)+PY(3))* 
     &P(IN(3*JQ+3)+1,J)+0.5*(DHR1*(FA+FB)*P(IN(3*JQ+1),J)+ 
     &DHR2*(FA-FB)*P(IN(3*JQ+2),J))/WREM2 
      P(I,J)=P(N+NRS,J)-P(I-1,J) 
  950 CONTINUE 
      IF(P(I-1,4).LT.P(I-1,5).OR.P(I,4).LT.P(I,5)) GOTO 640 
 
C...Mark jets as fragmented and give daughter pointers. 
      N=I-NRS+1 
      DO 960 I=NSAV+1,NSAV+NP 
      IM=K(I,3) 
      K(IM,1)=K(IM,1)+10 
      IF(MSTU(16).NE.2) THEN 
        K(IM,4)=NSAV+1 
        K(IM,5)=NSAV+1 
      ELSE 
        K(IM,4)=NSAV+2 
        K(IM,5)=N 
      ENDIF 
  960 CONTINUE 
 
C...Document string system. Move up particles. 
      NSAV=NSAV+1 
      K(NSAV,1)=11 
      K(NSAV,2)=92 
      K(NSAV,3)=IP 
      K(NSAV,4)=NSAV+1 
      K(NSAV,5)=N 
      DO 970 J=1,4 
      P(NSAV,J)=DPS(J) 
      V(NSAV,J)=V(IP,J) 
  970 CONTINUE 
      P(NSAV,5)=SQRT(MAX(0D0,DPS(4)**2-DPS(1)**2-DPS(2)**2-DPS(3)**2)) 
      V(NSAV,5)=0. 
      DO 990 I=NSAV+1,N 
      DO 980 J=1,5 
      K(I,J)=K(I+NRS-1,J) 
      P(I,J)=P(I+NRS-1,J) 
      V(I,J)=0. 
  980 CONTINUE 
  990 CONTINUE 
      MSTU91=MSTU(90) 
      DO 1000 IZ=MSTU90+1,MSTU91 
      MSTU9T(IZ)=MSTU(90+IZ)-NRS+1-NSAV+N 
      PARU9T(IZ)=PARU(90+IZ) 
 1000 CONTINUE 
      MSTU(90)=MSTU90 
 
C...Order particles in rank along the chain. Update mother pointer. 
      DO 1020 I=NSAV+1,N 
      DO 1010 J=1,5 
      K(I-NSAV+N,J)=K(I,J) 
      P(I-NSAV+N,J)=P(I,J) 
 1010 CONTINUE 
 1020 CONTINUE 
      I1=NSAV 
      DO 1050 I=N+1,2*N-NSAV 
      IF(K(I,3).NE.IE(1)) GOTO 1050 
      I1=I1+1 
      DO 1030 J=1,5 
      K(I1,J)=K(I,J) 
      P(I1,J)=P(I,J) 
 1030 CONTINUE 
      IF(MSTU(16).NE.2) K(I1,3)=NSAV 
      DO 1040 IZ=MSTU90+1,MSTU91 
      IF(MSTU9T(IZ).EQ.I) THEN 
        MSTU(90)=MSTU(90)+1 
        MSTU(90+MSTU(90))=I1 
        PARU(90+MSTU(90))=PARU9T(IZ) 
      ENDIF 
 1040 CONTINUE 
 1050 CONTINUE 
      DO 1080 I=2*N-NSAV,N+1,-1 
      IF(K(I,3).EQ.IE(1)) GOTO 1080 
      I1=I1+1 
      DO 1060 J=1,5 
      K(I1,J)=K(I,J) 
      P(I1,J)=P(I,J) 
 1060 CONTINUE 
      IF(MSTU(16).NE.2) K(I1,3)=NSAV 
      DO 1070 IZ=MSTU90+1,MSTU91 
      IF(MSTU9T(IZ).EQ.I) THEN 
        MSTU(90)=MSTU(90)+1 
        MSTU(90+MSTU(90))=I1 
        PARU(90+MSTU(90))=PARU9T(IZ) 
      ENDIF 
 1070 CONTINUE 
 1080 CONTINUE 
 
C...Boost back particle system. Set production vertices. 
      IF(MBST.EQ.0) THEN 
        MSTU(33)=1 
        CALL LUDBRB0(NSAV+1,N,0.,0.,DPS(1)/DPS(4),DPS(2)/DPS(4), 
     &  DPS(3)/DPS(4)) 
      ELSE 
        DO 1090 I=NSAV+1,N 
        HHPMT=P(I,1)**2+P(I,2)**2+P(I,5)**2 
        IF(P(I,3).GT.0.) THEN 
          HHPEZ=(P(I,4)+P(I,3))*HHBZ 
          P(I,3)=0.5*(HHPEZ-HHPMT/HHPEZ) 
          P(I,4)=0.5*(HHPEZ+HHPMT/HHPEZ) 
        ELSE 
          HHPEZ=(P(I,4)-P(I,3))/HHBZ 
          P(I,3)=-0.5*(HHPEZ-HHPMT/HHPEZ) 
          P(I,4)=0.5*(HHPEZ+HHPMT/HHPEZ) 
        ENDIF 
 1090   CONTINUE 
      ENDIF 
      DO 1110 I=NSAV+1,N 
      DO 1100 J=1,4 
      V(I,J)=V(IP,J) 
 1100 CONTINUE 
 1110 CONTINUE 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUINDF0(IP) 
 
C...Purpose: to handle the fragmentation of a jet system (or a single 
C...jet) according to independent fragmentation models. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION DPS(5),PSI(4),NFI(3),NFL(3),IFET(3),KFLF(3), 
     &KFLO(2),PXO(2),PYO(2),WO(2) 
 
C...Reset counters. Identify parton system and take copy. Check flavour. 
      NSAV=N 
      MSTU90=MSTU(90) 
      NJET=0 
      KQSUM=0 
      DO 100 J=1,5 
      DPS(J)=0. 
  100 CONTINUE 
      I=IP-1 
  110 I=I+1 
      IF(I.GT.MIN(N,MSTU(4)-MSTU(32))) THEN 
        CALL LUERRM0(12,'(LUINDF0:) failed to reconstruct jet system') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(K(I,1).NE.1.AND.K(I,1).NE.2) GOTO 110 
      KC=LUCOMP0(K(I,2)) 
      IF(KC.EQ.0) GOTO 110 
      KQ=KCHG(KC,2)*ISIGN(1,K(I,2)) 
      IF(KQ.EQ.0) GOTO 110 
      NJET=NJET+1 
      IF(KQ.NE.2) KQSUM=KQSUM+KQ 
      DO 120 J=1,5 
      K(NSAV+NJET,J)=K(I,J) 
      P(NSAV+NJET,J)=P(I,J) 
      DPS(J)=DPS(J)+P(I,J) 
  120 CONTINUE 
      K(NSAV+NJET,3)=I 
      IF(K(I,1).EQ.2.OR.(MSTJ(3).LE.5.AND.N.GT.I.AND. 
     &K(I+1,1).EQ.2)) GOTO 110 
      IF(NJET.NE.1.AND.KQSUM.NE.0) THEN 
        CALL LUERRM0(12,'(LUINDF0:) unphysical flavour combination') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Boost copied system to CM frame. Find CM energy and sum flavours. 
      IF(NJET.NE.1) THEN 
        MSTU(33)=1 
        CALL LUDBRB0(NSAV+1,NSAV+NJET,0.,0.,-DPS(1)/DPS(4), 
     &  -DPS(2)/DPS(4),-DPS(3)/DPS(4)) 
      ENDIF 
      PECM=0. 
      DO 130 J=1,3 
      NFI(J)=0 
  130 CONTINUE 
      DO 140 I=NSAV+1,NSAV+NJET 
      PECM=PECM+P(I,4) 
      KFA=IABS(K(I,2)) 
      IF(KFA.LE.3) THEN 
        NFI(KFA)=NFI(KFA)+ISIGN(1,K(I,2)) 
      ELSEIF(KFA.GT.1000) THEN 
        KFLA=MOD(KFA/1000,10) 
        KFLB=MOD(KFA/100,10) 
        IF(KFLA.LE.3) NFI(KFLA)=NFI(KFLA)+ISIGN(1,K(I,2)) 
        IF(KFLB.LE.3) NFI(KFLB)=NFI(KFLB)+ISIGN(1,K(I,2)) 
      ENDIF 
  140 CONTINUE 
 
C...Loop over attempts made. Reset counters. 
      NTRY=0 
  150 NTRY=NTRY+1 
      IF(NTRY.GT.200) THEN 
        CALL LUERRM0(14,'(LUINDF0:) caught in infinite loop') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      N=NSAV+NJET 
      MSTU(90)=MSTU90 
      DO 160 J=1,3 
      NFL(J)=NFI(J) 
      IFET(J)=0 
      KFLF(J)=0 
  160 CONTINUE 
 
C...Loop over jets to be fragmented. 
      DO 230 IP1=NSAV+1,NSAV+NJET 
      MSTJ(91)=0 
      NSAV1=N 
      MSTU91=MSTU(90) 
 
C...Initial flavour and momentum values. Jet along +z axis. 
      KFLH=IABS(K(IP1,2)) 
      IF(KFLH.GT.10) KFLH=MOD(KFLH/1000,10) 
      KFLO(2)=0 
      WF=P(IP1,4)+SQRT(P(IP1,1)**2+P(IP1,2)**2+P(IP1,3)**2) 
 
C...Initial values for quark or diquark jet. 
  170 IF(IABS(K(IP1,2)).NE.21) THEN 
        NSTR=1 
        KFLO(1)=K(IP1,2) 
        CALL LUPTDI0(0,PXO(1),PYO(1)) 
        WO(1)=WF 
 
C...Initial values for gluon treated like random quark jet. 
      ELSEIF(MSTJ(2).LE.2) THEN 
        NSTR=1 
        IF(MSTJ(2).EQ.2) MSTJ(91)=1 
        KFLO(1)=INT(1.+(2.+PARJ(2))*RLU0(0))*(-1)**INT(RLU0(0)+0.5) 
        CALL LUPTDI0(0,PXO(1),PYO(1)) 
        WO(1)=WF 
 
C...Initial values for gluon treated like quark-antiquark jet pair, 
C...sharing energy according to Altarelli-Parisi splitting function. 
      ELSE 
        NSTR=2 
        IF(MSTJ(2).EQ.4) MSTJ(91)=1 
        KFLO(1)=INT(1.+(2.+PARJ(2))*RLU0(0))*(-1)**INT(RLU0(0)+0.5) 
        KFLO(2)=-KFLO(1) 
        CALL LUPTDI0(0,PXO(1),PYO(1)) 
        PXO(2)=-PXO(1) 
        PYO(2)=-PYO(1) 
        WO(1)=WF*RLU0(0)**(1./3.) 
        WO(2)=WF-WO(1) 
      ENDIF 
 
C...Initial values for rank, flavour, pT and W+. 
      DO 220 ISTR=1,NSTR 
  180 I=N 
      MSTU(90)=MSTU91 
      IRANK=0 
      KFL1=KFLO(ISTR) 
      PX1=PXO(ISTR) 
      PY1=PYO(ISTR) 
      W=WO(ISTR) 
 
C...New hadron. Generate flavour and hadron species. 
  190 I=I+1 
      IF(I.GE.MSTU(4)-MSTU(32)-NJET-5) THEN 
        CALL LUERRM0(11,'(LUINDF0:) no more memory left in LUJETS') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IRANK=IRANK+1 
      K(I,1)=1 
      K(I,3)=IP1 
      K(I,4)=0 
      K(I,5)=0 
  200 CALL LUKFDI0(KFL1,0,KFL2,K(I,2)) 
      IF(K(I,2).EQ.0) GOTO 180 
      IF(MSTJ(12).GE.3.AND.IRANK.EQ.1.AND.IABS(KFL1).LE.10.AND. 
     &IABS(KFL2).GT.10) THEN 
        IF(RLU0(0).GT.PARJ(19)) GOTO 200 
      ENDIF 
 
C...Find hadron mass. Generate four-momentum. 
      P(I,5)=ULMASS0(K(I,2)) 
      CALL LUPTDI0(KFL1,PX2,PY2) 
      P(I,1)=PX1+PX2 
      P(I,2)=PY1+PY2 
      PR=P(I,5)**2+P(I,1)**2+P(I,2)**2 
      CALL LUZDIS0(KFL1,KFL2,PR,Z) 
      MZSAV=0 
      IF(IABS(KFL1).GE.4.AND.IABS(KFL1).LE.8.AND.MSTU(90).LT.8) THEN 
        MZSAV=1 
        MSTU(90)=MSTU(90)+1 
        MSTU(90+MSTU(90))=I 
        PARU(90+MSTU(90))=Z 
      ENDIF 
      P(I,3)=0.5*(Z*W-PR/MAX(1E-4,Z*W)) 
      P(I,4)=0.5*(Z*W+PR/MAX(1E-4,Z*W)) 
      IF(MSTJ(3).GE.1.AND.IRANK.EQ.1.AND.KFLH.GE.4.AND. 
     &P(I,3).LE.0.001) THEN 
        IF(W.GE.P(I,5)+0.5*PARJ(32)) GOTO 180 
        P(I,3)=0.0001 
        P(I,4)=SQRT(PR) 
        Z=P(I,4)/W 
      ENDIF 
 
C...Remaining flavour and momentum. 
      KFL1=-KFL2 
      PX1=-PX2 
      PY1=-PY2 
      W=(1.-Z)*W 
      DO 210 J=1,5 
      V(I,J)=0. 
  210 CONTINUE 
 
C...Check if pL acceptable. Go back for new hadron if enough energy. 
      IF(MSTJ(3).GE.0.AND.P(I,3).LT.0.) THEN 
        I=I-1 
        IF(MZSAV.EQ.1) MSTU(90)=MSTU(90)-1 
      ENDIF 
      IF(W.GT.PARJ(31)) GOTO 190 
      N=I 
  220 CONTINUE 
      IF(MOD(MSTJ(3),5).EQ.4.AND.N.EQ.NSAV1) WF=WF+0.1*PARJ(32) 
      IF(MOD(MSTJ(3),5).EQ.4.AND.N.EQ.NSAV1) GOTO 170 
 
C...Rotate jet to new direction. 
      THE=ULANGL0(P(IP1,3),SQRT(P(IP1,1)**2+P(IP1,2)**2)) 
      PHI=ULANGL0(P(IP1,1),P(IP1,2)) 
      MSTU(33)=1 
      CALL LUDBRB0(NSAV1+1,N,THE,PHI,0D0,0D0,0D0) 
      K(K(IP1,3),4)=NSAV1+1 
      K(K(IP1,3),5)=N 
 
C...End of jet generation loop. Skip conservation in some cases. 
  230 CONTINUE 
      IF(NJET.EQ.1.OR.MSTJ(3).LE.0) GOTO 490 
      IF(MOD(MSTJ(3),5).NE.0.AND.N-NSAV-NJET.LT.2) GOTO 150 
 
C...Subtract off produced hadron flavours, finished if zero. 
      DO 240 I=NSAV+NJET+1,N 
      KFA=IABS(K(I,2)) 
      KFLA=MOD(KFA/1000,10) 
      KFLB=MOD(KFA/100,10) 
      KFLC=MOD(KFA/10,10) 
      IF(KFLA.EQ.0) THEN 
        IF(KFLB.LE.3) NFL(KFLB)=NFL(KFLB)-ISIGN(1,K(I,2))*(-1)**KFLB 
        IF(KFLC.LE.3) NFL(KFLC)=NFL(KFLC)+ISIGN(1,K(I,2))*(-1)**KFLB 
      ELSE 
        IF(KFLA.LE.3) NFL(KFLA)=NFL(KFLA)-ISIGN(1,K(I,2)) 
        IF(KFLB.LE.3) NFL(KFLB)=NFL(KFLB)-ISIGN(1,K(I,2)) 
        IF(KFLC.LE.3) NFL(KFLC)=NFL(KFLC)-ISIGN(1,K(I,2)) 
      ENDIF 
  240 CONTINUE 
      NREQ=(IABS(NFL(1))+IABS(NFL(2))+IABS(NFL(3))-IABS(NFL(1)+ 
     &NFL(2)+NFL(3)))/2+IABS(NFL(1)+NFL(2)+NFL(3))/3 
      IF(NREQ.EQ.0) GOTO 320 
 
C...Take away flavour of low-momentum particles until enough freedom. 
      NREM=0 
  250 IREM=0 
      P2MIN=PECM**2 
      DO 260 I=NSAV+NJET+1,N 
      P2=P(I,1)**2+P(I,2)**2+P(I,3)**2 
      IF(K(I,1).EQ.1.AND.P2.LT.P2MIN) IREM=I 
      IF(K(I,1).EQ.1.AND.P2.LT.P2MIN) P2MIN=P2 
  260 CONTINUE 
      IF(IREM.EQ.0) GOTO 150 
      K(IREM,1)=7 
      KFA=IABS(K(IREM,2)) 
      KFLA=MOD(KFA/1000,10) 
      KFLB=MOD(KFA/100,10) 
      KFLC=MOD(KFA/10,10) 
      IF(KFLA.GE.4.OR.KFLB.GE.4) K(IREM,1)=8 
      IF(K(IREM,1).EQ.8) GOTO 250 
      IF(KFLA.EQ.0) THEN 
        ISGN=ISIGN(1,K(IREM,2))*(-1)**KFLB 
        IF(KFLB.LE.3) NFL(KFLB)=NFL(KFLB)+ISGN 
        IF(KFLC.LE.3) NFL(KFLC)=NFL(KFLC)-ISGN 
      ELSE 
        IF(KFLA.LE.3) NFL(KFLA)=NFL(KFLA)+ISIGN(1,K(IREM,2)) 
        IF(KFLB.LE.3) NFL(KFLB)=NFL(KFLB)+ISIGN(1,K(IREM,2)) 
        IF(KFLC.LE.3) NFL(KFLC)=NFL(KFLC)+ISIGN(1,K(IREM,2)) 
      ENDIF 
      NREM=NREM+1 
      NREQ=(IABS(NFL(1))+IABS(NFL(2))+IABS(NFL(3))-IABS(NFL(1)+ 
     &NFL(2)+NFL(3)))/2+IABS(NFL(1)+NFL(2)+NFL(3))/3 
      IF(NREQ.GT.NREM) GOTO 250 
      DO 270 I=NSAV+NJET+1,N 
      IF(K(I,1).EQ.8) K(I,1)=1 
  270 CONTINUE 
 
C...Find combination of existing and new flavours for hadron. 
  280 NFET=2 
      IF(NFL(1)+NFL(2)+NFL(3).NE.0) NFET=3 
      IF(NREQ.LT.NREM) NFET=1 
      IF(IABS(NFL(1))+IABS(NFL(2))+IABS(NFL(3)).EQ.0) NFET=0 
      DO 290 J=1,NFET 
      IFET(J)=1+(IABS(NFL(1))+IABS(NFL(2))+IABS(NFL(3)))*RLU0(0) 
      KFLF(J)=ISIGN(1,NFL(1)) 
      IF(IFET(J).GT.IABS(NFL(1))) KFLF(J)=ISIGN(2,NFL(2)) 
      IF(IFET(J).GT.IABS(NFL(1))+IABS(NFL(2))) KFLF(J)=ISIGN(3,NFL(3)) 
  290 CONTINUE 
      IF(NFET.EQ.2.AND.(IFET(1).EQ.IFET(2).OR.KFLF(1)*KFLF(2).GT.0)) 
     &GOTO 280 
      IF(NFET.EQ.3.AND.(IFET(1).EQ.IFET(2).OR.IFET(1).EQ.IFET(3).OR. 
     &IFET(2).EQ.IFET(3).OR.KFLF(1)*KFLF(2).LT.0.OR.KFLF(1)*KFLF(3) 
     &.LT.0.OR.KFLF(1)*(NFL(1)+NFL(2)+NFL(3)).LT.0)) GOTO 280 
      IF(NFET.EQ.0) KFLF(1)=1+INT((2.+PARJ(2))*RLU0(0)) 
      IF(NFET.EQ.0) KFLF(2)=-KFLF(1) 
      IF(NFET.EQ.1) KFLF(2)=ISIGN(1+INT((2.+PARJ(2))*RLU0(0)),-KFLF(1)) 
      IF(NFET.LE.2) KFLF(3)=0 
      IF(KFLF(3).NE.0) THEN 
        KFLFC=ISIGN(1000*MAX(IABS(KFLF(1)),IABS(KFLF(3)))+ 
     &  100*MIN(IABS(KFLF(1)),IABS(KFLF(3)))+1,KFLF(1)) 
        IF(KFLF(1).EQ.KFLF(3).OR.(1.+3.*PARJ(4))*RLU0(0).GT.1.) 
     &  KFLFC=KFLFC+ISIGN(2,KFLFC) 
      ELSE 
        KFLFC=KFLF(1) 
      ENDIF 
      CALL LUKFDI0(KFLFC,KFLF(2),KFLDMP,KF) 
      IF(KF.EQ.0) GOTO 280 
      DO 300 J=1,MAX(2,NFET) 
      NFL(IABS(KFLF(J)))=NFL(IABS(KFLF(J)))-ISIGN(1,KFLF(J)) 
  300 CONTINUE 
 
C...Store hadron at random among free positions. 
      NPOS=MIN(1+INT(RLU0(0)*NREM),NREM) 
      DO 310 I=NSAV+NJET+1,N 
      IF(K(I,1).EQ.7) NPOS=NPOS-1 
      IF(K(I,1).EQ.1.OR.NPOS.NE.0) GOTO 310 
      K(I,1)=1 
      K(I,2)=KF 
      P(I,5)=ULMASS0(K(I,2)) 
      P(I,4)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2+P(I,5)**2) 
  310 CONTINUE 
      NREM=NREM-1 
      NREQ=(IABS(NFL(1))+IABS(NFL(2))+IABS(NFL(3))-IABS(NFL(1)+ 
     &NFL(2)+NFL(3)))/2+IABS(NFL(1)+NFL(2)+NFL(3))/3 
      IF(NREM.GT.0) GOTO 280 
 
C...Compensate for missing momentum in global scheme (3 options). 
  320 IF(MOD(MSTJ(3),5).NE.0.AND.MOD(MSTJ(3),5).NE.4) THEN 
        DO 340 J=1,3 
        PSI(J)=0. 
        DO 330 I=NSAV+NJET+1,N 
        PSI(J)=PSI(J)+P(I,J) 
  330   CONTINUE 
  340   CONTINUE 
        PSI(4)=PSI(1)**2+PSI(2)**2+PSI(3)**2 
        PWS=0. 
        DO 350 I=NSAV+NJET+1,N 
        IF(MOD(MSTJ(3),5).EQ.1) PWS=PWS+P(I,4) 
        IF(MOD(MSTJ(3),5).EQ.2) PWS=PWS+SQRT(P(I,5)**2+(PSI(1)*P(I,1)+ 
     &  PSI(2)*P(I,2)+PSI(3)*P(I,3))**2/PSI(4)) 
        IF(MOD(MSTJ(3),5).EQ.3) PWS=PWS+1. 
  350   CONTINUE 
        DO 370 I=NSAV+NJET+1,N 
        IF(MOD(MSTJ(3),5).EQ.1) PW=P(I,4) 
        IF(MOD(MSTJ(3),5).EQ.2) PW=SQRT(P(I,5)**2+(PSI(1)*P(I,1)+ 
     &  PSI(2)*P(I,2)+PSI(3)*P(I,3))**2/PSI(4)) 
        IF(MOD(MSTJ(3),5).EQ.3) PW=1. 
        DO 360 J=1,3 
        P(I,J)=P(I,J)-PSI(J)*PW/PWS 
  360   CONTINUE 
        P(I,4)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2+P(I,5)**2) 
  370   CONTINUE 
 
C...Compensate for missing momentum withing each jet separately. 
      ELSEIF(MOD(MSTJ(3),5).EQ.4) THEN 
        DO 390 I=N+1,N+NJET 
        K(I,1)=0 
        DO 380 J=1,5 
        P(I,J)=0. 
  380   CONTINUE 
  390   CONTINUE 
        DO 410 I=NSAV+NJET+1,N 
        IR1=K(I,3) 
        IR2=N+IR1-NSAV 
        K(IR2,1)=K(IR2,1)+1 
        PLS=(P(I,1)*P(IR1,1)+P(I,2)*P(IR1,2)+P(I,3)*P(IR1,3))/ 
     &  (P(IR1,1)**2+P(IR1,2)**2+P(IR1,3)**2) 
        DO 400 J=1,3 
        P(IR2,J)=P(IR2,J)+P(I,J)-PLS*P(IR1,J) 
  400   CONTINUE 
        P(IR2,4)=P(IR2,4)+P(I,4) 
        P(IR2,5)=P(IR2,5)+PLS 
  410   CONTINUE 
        PSS=0. 
        DO 420 I=N+1,N+NJET 
        IF(K(I,1).NE.0) PSS=PSS+P(I,4)/(PECM*(0.8*P(I,5)+0.2)) 
  420   CONTINUE 
        DO 440 I=NSAV+NJET+1,N 
        IR1=K(I,3) 
        IR2=N+IR1-NSAV 
        PLS=(P(I,1)*P(IR1,1)+P(I,2)*P(IR1,2)+P(I,3)*P(IR1,3))/ 
     &  (P(IR1,1)**2+P(IR1,2)**2+P(IR1,3)**2) 
        DO 430 J=1,3 
        P(I,J)=P(I,J)-P(IR2,J)/K(IR2,1)+(1./(P(IR2,5)*PSS)-1.)*PLS* 
     &  P(IR1,J) 
  430   CONTINUE 
        P(I,4)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2+P(I,5)**2) 
  440   CONTINUE 
      ENDIF 
 
C...Scale momenta for energy conservation. 
      IF(MOD(MSTJ(3),5).NE.0) THEN 
        PMS=0. 
        PES=0. 
        PQS=0. 
        DO 450 I=NSAV+NJET+1,N 
        PMS=PMS+P(I,5) 
        PES=PES+P(I,4) 
        PQS=PQS+P(I,5)**2/P(I,4) 
  450   CONTINUE 
        IF(PMS.GE.PECM) GOTO 150 
        NECO=0 
  460   NECO=NECO+1 
        PFAC=(PECM-PQS)/(PES-PQS) 
        PES=0. 
        PQS=0. 
        DO 480 I=NSAV+NJET+1,N 
        DO 470 J=1,3 
        P(I,J)=PFAC*P(I,J) 
  470   CONTINUE 
        P(I,4)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2+P(I,5)**2) 
        PES=PES+P(I,4) 
        PQS=PQS+P(I,5)**2/P(I,4) 
  480   CONTINUE 
        IF(NECO.LT.10.AND.ABS(PECM-PES).GT.2E-6*PECM) GOTO 460 
      ENDIF 
 
C...Origin of produced particles and parton daughter pointers. 
  490 DO 500 I=NSAV+NJET+1,N 
      IF(MSTU(16).NE.2) K(I,3)=NSAV+1 
      IF(MSTU(16).EQ.2) K(I,3)=K(K(I,3),3) 
  500 CONTINUE 
      DO 510 I=NSAV+1,NSAV+NJET 
      I1=K(I,3) 
      K(I1,1)=K(I1,1)+10 
      IF(MSTU(16).NE.2) THEN 
        K(I1,4)=NSAV+1 
        K(I1,5)=NSAV+1 
      ELSE 
        K(I1,4)=K(I1,4)-NJET+1 
        K(I1,5)=K(I1,5)-NJET+1 
        IF(K(I1,5).LT.K(I1,4)) THEN 
          K(I1,4)=0 
          K(I1,5)=0 
        ENDIF 
      ENDIF 
  510 CONTINUE 
 
C...Document independent fragmentation system. Remove copy of jets. 
      NSAV=NSAV+1 
      K(NSAV,1)=11 
      K(NSAV,2)=93 
      K(NSAV,3)=IP 
      K(NSAV,4)=NSAV+1 
      K(NSAV,5)=N-NJET+1 
      DO 520 J=1,4 
      P(NSAV,J)=DPS(J) 
      V(NSAV,J)=V(IP,J) 
  520 CONTINUE 
      P(NSAV,5)=SQRT(MAX(0D0,DPS(4)**2-DPS(1)**2-DPS(2)**2-DPS(3)**2)) 
      V(NSAV,5)=0. 
      DO 540 I=NSAV+NJET,N 
      DO 530 J=1,5 
      K(I-NJET+1,J)=K(I,J) 
      P(I-NJET+1,J)=P(I,J) 
      V(I-NJET+1,J)=V(I,J) 
  530 CONTINUE 
  540 CONTINUE 
      N=N-NJET+1 
      DO 550 IZ=MSTU90+1,MSTU(90) 
      MSTU(90+IZ)=MSTU(90+IZ)-NJET+1 
  550 CONTINUE 
 
C...Boost back particle system. Set production vertices. 
      IF(NJET.NE.1) CALL LUDBRB0(NSAV+1,N,0.,0.,DPS(1)/DPS(4), 
     &DPS(2)/DPS(4),DPS(3)/DPS(4)) 
      DO 570 I=NSAV+1,N 
      DO 560 J=1,4 
      V(I,J)=V(IP,J) 
  560 CONTINUE 
  570 CONTINUE 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUDECY0(IP) 
 
C...Purpose: to handle the decay of unstable particles. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/LUDAT03/ 
      DIMENSION VDCY(4),KFLO(4),KFL1(4),PV(10,5),RORD(10),UE(3),BE(3), 
     &WTCOR(10),PTAU(4),PCMTAU(4) 
      DOUBLE PRECISION DBETAU(3) 
      DATA WTCOR/2.,5.,15.,60.,250.,1500.,1.2E4,1.2E5,150.,16./ 
 
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
      KC=LUCOMP0(KFA) 
      MSTJ(92)=0 
 
C...Choose lifetime and determine decay vertex. 
      IF(K(IP,1).EQ.5) THEN 
        V(IP,5)=0. 
      ELSEIF(K(IP,1).NE.4) THEN 
        V(IP,5)=-PMAS(KC,4)*LOG(RLU0(0)) 
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
        IF(KFORIG.NE.0) CALL LUDBRB0(ITAU,ITAU,0.,0.,-DBETAU(1), 
     &  -DBETAU(2),-DBETAU(3)) 
        PHITAU=ULANGL0(P(ITAU,1),P(ITAU,2)) 
        CALL LUDBRB0(ITAU,ITAU,0.,-PHITAU,0D0,0D0,0D0) 
        THETAU=ULANGL0(P(ITAU,3),P(ITAU,1)) 
        CALL LUDBRB0(ITAU,ITAU,-THETAU,0.,0D0,0D0,0D0) 
 
C...Call tau decay routine (if meaningful) and fill extra info. 
        IF(KFORIG.NE.0.OR.MSTJ(28).EQ.2) THEN 
          CALL LUTAUD0(ITAU,IORIG,KFORIG,NDECAY) 
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
          CALL LUDBRB0(NSAV+1,N,THETAU,PHITAU,0D0,0D0,0D0) 
          IF(KFORIG.NE.0) CALL LUDBRB0(NSAV+1,N,0.,0.,DBETAU(1), 
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
        IF(SIN(0.5*XBBMIX*V(IP,5)/PMAS(KC,4))**2.GT.RLU0(0)) MMIX=1 
        IF(MMIX.EQ.1) KFS=-KFS 
      ENDIF 
 
C...Check existence of decay channels. Particle/antiparticle rules. 
      KCA=KC 
      IF(MDCY(KC,2).GT.0) THEN 
        MDMDCY=MDME(MDCY(KC,2),2) 
        IF(MDMDCY.GT.80.AND.MDMDCY.LE.90) KCA=MDMDCY 
      ENDIF 
      IF(MDCY(KCA,2).LE.0.OR.MDCY(KCA,3).LE.0) THEN 
        CALL LUERRM0(9,'(LUDECY0:) no decay channel defined') 
        RETURN 
      ENDIF 
      IF(MOD(KFA/1000,10).EQ.0.AND.(KCA.EQ.85.OR.KCA.EQ.87)) KFS=-KFS 
      IF(KCHG(KC,3).EQ.0) THEN 
        KFSP=1 
        KFSN=0 
        IF(RLU0(0).GT.0.5) KFS=-KFS 
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
        CALL LUERRM0(2,'(LUDECY0:) all decay channels closed by user') 
        RETURN 
      ENDIF 
 
C...Select decay channel among allowed ones. 
  240 RBR=BRSU*RLU0(0) 
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
 
C...Start readout of decay channel: matrix element, reset counters. 
      MMAT=MDME(IDC,2) 
  260 NTRY=NTRY+1 
      IF(NTRY.GT.1000) THEN 
        CALL LUERRM0(14,'(LUDECY0:) caught in infinite loop') 
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
      DO 280 JT=1,JTMAX 
      IF(JT.LE.5) KP=KFDP(IDC,JT) 
      IF(JT.GE.6) KP=KFDP(IDC+1,JT-5) 
      IF(KP.EQ.0) GOTO 280 
      KPA=IABS(KP) 
      KCP=LUCOMP0(KPA) 
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
        CALL LUKFDI0(-KFS*INT(1.+(2.+PARJ(2))*RLU0(0)),0,KFP,KDUMP) 
        IF(KFP.EQ.0) GOTO 260 
        MSTJ(93)=1 
        IF(PV(1,5).LT.PARJ(32)+2.*ULMASS0(KFP)) GOTO 260 
      ELSEIF(KP.EQ.-82) THEN 
        KFP=-KFP 
        IF(IABS(KFP).GT.10) KFP=KFP+ISIGN(10000,KFP) 
      ENDIF 
      IF(KPA.EQ.81.OR.KPA.EQ.82) KCP=LUCOMP0(KFP) 
 
C...Add decay product to event record or to quark flavour list. 
      KFPA=IABS(KFP) 
      KQP=KCHG(KCP,2) 
      IF(MMAT.GE.11.AND.MMAT.LE.30.AND.KQP.NE.0) THEN 
        NQ=NQ+1 
        KFLO(NQ)=KFP 
        MSTJ(93)=2 
        PSQ=PSQ+ULMASS0(KFLO(NQ)) 
      ELSEIF((MMAT.EQ.42.OR.MMAT.EQ.43.OR.MMAT.EQ.48).AND.NP.EQ.3.AND. 
     &MOD(NQ,2).EQ.1) THEN 
        NQ=NQ-1 
        PS=PS-P(I,5) 
        K(I,1)=1 
        KFI=K(I,2) 
        CALL LUKFDI0(KFP,KFI,KFLDMP,K(I,2)) 
        IF(K(I,2).EQ.0) GOTO 260 
        MSTJ(93)=1 
        P(I,5)=ULMASS0(K(I,2)) 
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
        P(I,5)=ULMASS0(KFP) 
        IF(MMAT.EQ.45.AND.KFPA.EQ.89) P(I,5)=PARJ(32) 
        PS=PS+P(I,5) 
      ENDIF 
  280 CONTINUE 
 
C...Check masses for resonance decays. 
      IF(MHADDY.EQ.0) THEN 
        IF(PS+PARJ(64).GT.PV(1,5)) GOTO 240 
      ENDIF 
 
C...Choose decay multiplicity in phase space model. 
  290 IF(MMAT.GE.11.AND.MMAT.LE.30) THEN 
        PSP=PS 
        CNDE=PARJ(61)*LOG(MAX((PV(1,5)-PS-PSQ)/PARJ(62),1.1)) 
        IF(MMAT.EQ.12) CNDE=CNDE+PARJ(63) 
  300   NTRY=NTRY+1 
        IF(NTRY.GT.1000) THEN 
          CALL LUERRM0(14,'(LUDECY0:) caught in infinite loop') 
          IF(MSTU(21).GE.1) RETURN 
        ENDIF 
        IF(MMAT.LE.20) THEN 
          GAUSS=SQRT(-2.*CNDE*LOG(MAX(1E-10,RLU0(0))))* 
     &    SIN(PARU(2)*RLU0(0)) 
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
        JT=1+INT((NQ-1)*RLU0(0)) 
        CALL LUKFDI0(KFL1(JT),0,KFL2,K(I,2)) 
        IF(K(I,2).EQ.0) GOTO 300 
        KFL1(JT)=-KFL2 
  320   CONTINUE 
  330   JT=2 
        JT2=3 
        JT3=4 
        IF(NQ.EQ.4.AND.RLU0(0).LT.PARJ(66)) JT=4 
        IF(JT.EQ.4.AND.ISIGN(1,KFL1(1)*(10-IABS(KFL1(1))))* 
     &  ISIGN(1,KFL1(JT)*(10-IABS(KFL1(JT)))).GT.0) JT=3 
        IF(JT.EQ.3) JT2=2 
        IF(JT.EQ.4) JT3=2 
        CALL LUKFDI0(KFL1(1),KFL1(JT),KFLDMP,K(N+ND-NQ/2+1,2)) 
        IF(K(N+ND-NQ/2+1,2).EQ.0) GOTO 300 
        IF(NQ.EQ.4) CALL LUKFDI0(KFL1(JT2),KFL1(JT3),KFLDMP,K(N+ND,2)) 
        IF(NQ.EQ.4.AND.K(N+ND,2).EQ.0) GOTO 300 
 
C...Check that sum of decay product masses not too large. 
        PS=PSP 
        DO 340 I=N+NP+1,N+ND 
        K(I,1)=1 
        K(I,3)=IP 
        K(I,4)=0 
        K(I,5)=0 
        P(I,5)=ULMASS0(K(I,2)) 
        PS=PS+P(I,5) 
  340   CONTINUE 
        IF(PS+PARJ(64).GT.PV(1,5)) GOTO 300 
 
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
        PSMC=ULMASS0(K(N+1,2)) 
        MSTJ(93)=1 
        PSMC=PSMC+ULMASS0(K(N+2,2)) 
        IF(MAX(PS,PSMC)+PARJ(32).GT.PV(1,5)) GOTO 240 
        HR1=(P(N+1,5)/PV(1,5))**2 
        HR2=(P(N+2,5)/PV(1,5))**2 
        IF((1.-HR1-HR2)*(2.+HR1+HR2)*SQRT((1.-HR1-HR2)**2-4.*HR1*HR2) 
     &  .LT.2.*RLU0(0)) GOTO 240 
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
  360   HW=HLQ+RLU0(0)*(HUQ-HLQ) 
        IF(HMEPS(HW).LT.RLU0(0)) GOTO 360 
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
  380   HREG=RLU0(0)*(HWT1+HWT2+HWT3) 
        IF(HREG.LE.HWT1) THEN 
          HW=1.+HG*TAN(HATL+RLU0(0)*(HATM-HATL)) 
          HACC=HMEPS(HW/HQW) 
        ELSEIF(HREG.LE.HWT1+HWT2) THEN 
          HW=HM+RLU0(0)*(MIN(1.,HUW)-HM) 
          HACC=HMEPS(HW/HQW)/((HW-1.)**2+HG**2)/HMV 
        ELSE 
          HW=1.+HG*TAN(RLU0(0)*HATU) 
          HACC=HMEPS(HW/HQW)/HMP1 
        ENDIF 
        IF(HACC.LT.RLU0(0)) GOTO 380 
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
  430   PMST=PMES*(P(IP,5)**2/PMES)**RLU0(0) 
        WT=(1+0.5*PMES/PMST)*SQRT(MAX(0.,1.-PMES/PMST))* 
     &  (1.-PMST/P(IP,5)**2)**3*(1.+PGRHO2/PMRHO2)/ 
     &  ((1.-PMST/PMRHO2)**2+PGRHO2/PMRHO2) 
        IF(WT.LT.RLU0(0)) GOTO 430 
        PV(2,5)=MAX(2.00001*PMAS(11,1),SQRT(PMST)) 
 
C...M-generator gives weight. If rejected, try again. 
      ELSE 
  440   RORD(1)=1. 
        DO 470 IL1=2,ND-1 
        RSAV=RLU0(0) 
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
        IF(WT.LT.RLU0(0)*WTMAX) GOTO 440 
      ENDIF 
 
C...Perform two-particle decays in respective CM frame. 
  490 DO 510 IL=1,ND-1 
      PA=PAWT(PV(IL,5),PV(IL+1,5),P(N+IL,5)) 
      UE(3)=2.*RLU0(0)-1. 
      PHI=PARU(2)*RLU0(0) 
      UE(1)=SQRT(1.-UE(3)**2)*COS(PHI) 
      UE(2)=SQRT(1.-UE(3)**2)*SIN(PHI) 
      DO 500 J=1,3 
      P(N+IL,J)=PA*UE(J) 
      PV(IL+1,J)=-PA*UE(J) 
  500 CONTINUE 
      P(N+IL,4)=SQRT(PA**2+P(N+IL,5)**2) 
      PV(IL+1,4)=SQRT(PA**2+PV(IL+1,5)**2) 
  510 CONTINUE 
 
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
        IF(MAX(WT*WTCOR(9)/P(IP,5)**6,0.001).LT.RLU0(0)) GOTO 420 
 
C...Matrix elements for pi0 or eta Dalitz decay to gamma e+ e-. 
      ELSEIF(MMAT.EQ.2) THEN 
        FOUR12=FOUR(N+1,N+2) 
        FOUR13=FOUR(N+1,N+3) 
        WT=(PMST-0.5*PMES)*(FOUR12**2+FOUR13**2)+ 
     &  PMES*(FOUR12*FOUR13+FOUR12**2+FOUR13**2) 
        IF(WT.LT.RLU0(0)*0.25*PMST*(P(IP,5)**2-PMST)**2) GOTO 490 
 
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
        IF(HNUM.LT.RLU0(0)*HDEN) GOTO 490 
 
C...Matrix element for "onium" -> g + g + g or gamma + g + g. 
      ELSEIF(MMAT.EQ.4) THEN 
        HX1=2.*FOUR(IP,N+1)/P(IP,5)**2 
        HX2=2.*FOUR(IP,N+2)/P(IP,5)**2 
        HX3=2.*FOUR(IP,N+3)/P(IP,5)**2 
        WT=((1.-HX1)/(HX2*HX3))**2+((1.-HX2)/(HX1*HX3))**2+ 
     &  ((1.-HX3)/(HX1*HX2))**2 
        IF(WT.LT.2.*RLU0(0)) GOTO 420 
        IF(K(IP+1,2).EQ.22.AND.(1.-HX1)*P(IP,5)**2.LT.4.*PARJ(32)**2) 
     &  GOTO 420 
 
C...Effective matrix element for nu spectrum in tau -> nu + hadrons. 
      ELSEIF(MMAT.EQ.41) THEN 
        HX1=2.*FOUR(IP,N+1)/P(IP,5)**2 
        HXM=MIN(0.75,2.*(1.-PS/P(IP,5))) 
        IF(HX1*(3.-2.*HX1).LT.RLU0(0)*HXM*(3.-2.*HXM)) GOTO 420 
 
C...Matrix elements for weak decays (only semileptonic for c and b) 
      ELSEIF((MMAT.EQ.42.OR.MMAT.EQ.43.OR.MMAT.EQ.44.OR.MMAT.EQ.48) 
     &.AND.ND.EQ.3) THEN 
        IF(MBST.EQ.0) WT=FOUR(IP,N+1)*FOUR(N+2,N+3) 
        IF(MBST.EQ.1) WT=P(IP,5)*P(N+1,4)*FOUR(N+2,N+3) 
        IF(WT.LT.RLU0(0)*P(IP,5)*PV(1,5)**3/WTCOR(10)) GOTO 420 
      ELSEIF(MMAT.EQ.42.OR.MMAT.EQ.43.OR.MMAT.EQ.44.OR.MMAT.EQ.48) THEN 
        DO 580 J=1,4 
        P(N+NP+1,J)=0. 
        DO 570 IS=N+3,N+NP 
        P(N+NP+1,J)=P(N+NP+1,J)+P(IS,J) 
  570   CONTINUE 
  580   CONTINUE 
        IF(MBST.EQ.0) WT=FOUR(IP,N+1)*FOUR(N+2,N+NP+1) 
        IF(MBST.EQ.1) WT=P(IP,5)*P(N+1,4)*FOUR(N+2,N+NP+1) 
        IF(WT.LT.RLU0(0)*P(IP,5)*PV(1,5)**3/WTCOR(10)) GOTO 420 
 
C...Angular distribution in W decay. 
      ELSEIF(MMAT.EQ.46.AND.MSGN.NE.0) THEN 
        IF(MSGN.GT.0) WT=FOUR(IM,N+1)*FOUR(N+2,IP+1) 
        IF(MSGN.LT.0) WT=FOUR(IM,N+2)*FOUR(N+1,IP+1) 
        IF(WT.LT.RLU0(0)*P(IM,5)**4/WTCOR(10)) GOTO 490 
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
        PM2=ULMASS0(K(N+2,2)) 
        MSTJ(93)=1 
        PM3=ULMASS0(K(N+3,2)) 
        IF(P(N+2,5)**2+P(N+3,5)**2+2.*FOUR(N+2,N+3).GE. 
     &  (PARJ(32)+PM2+PM3)**2) GOTO 660 
        K(N+2,1)=1 
        KFTEMP=K(N+2,2) 
        CALL LUKFDI0(KFTEMP,K(N+3,2),KFLDMP,K(N+2,2)) 
        IF(K(N+2,2).EQ.0) GOTO 260 
        P(N+2,5)=ULMASS0(K(N+2,2)) 
        PS=P(N+1,5)+P(N+2,5) 
        PV(2,5)=P(N+2,5) 
        MMAT=0 
        ND=2 
        GOTO 490 
      ELSEIF(MMAT.EQ.44) THEN 
        MSTJ(93)=1 
        PM3=ULMASS0(K(N+3,2)) 
        MSTJ(93)=1 
        PM4=ULMASS0(K(N+4,2)) 
        IF(P(N+3,5)**2+P(N+4,5)**2+2.*FOUR(N+3,N+4).GE. 
     &  (PARJ(32)+PM3+PM4)**2) GOTO 630 
        K(N+3,1)=1 
        KFTEMP=K(N+3,2) 
        CALL LUKFDI0(KFTEMP,K(N+4,2),KFLDMP,K(N+3,2)) 
        IF(K(N+3,2).EQ.0) GOTO 260 
        P(N+3,5)=ULMASS0(K(N+3,2)) 
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
        PM1=ULMASS0(K(N+1,2)) 
        MSTJ(93)=1 
        PM2=ULMASS0(K(N+2,2)) 
        IF(PMR.GT.PARJ(32)+PM1+PM2) GOTO 640 
        KFLDUM=INT(1.5+RLU0(0)) 
        CALL LUKFDI0(K(N+1,2),-ISIGN(KFLDUM,K(N+1,2)),KFLDMP,KF1) 
        CALL LUKFDI0(K(N+2,2),-ISIGN(KFLDUM,K(N+2,2)),KFLDMP,KF2) 
        IF(KF1.EQ.0.OR.KF2.EQ.0) GOTO 260 
        PSM=ULMASS0(KF1)+ULMASS0(KF2) 
        IF((MMAT.EQ.42.OR.MMAT.EQ.48).AND.PMR.GT.PARJ(64)+PSM) GOTO 640 
        IF(MMAT.GE.43.AND.PMR.GT.0.2*PARJ(32)+PSM) GOTO 640 
        IF(MMAT.EQ.48) GOTO 420 
        IF(ND.EQ.4.OR.KFA.EQ.15) GOTO 260 
        K(N+1,1)=1 
        KFTEMP=K(N+1,2) 
        CALL LUKFDI0(KFTEMP,K(N+2,2),KFLDMP,K(N+1,2)) 
        IF(K(N+1,2).EQ.0) GOTO 260 
        P(N+1,5)=ULMASS0(K(N+1,2)) 
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
        PSQ=ULMASS0(KFLO(1)) 
        MSTJ(93)=2 
        PSQ=PSQ+ULMASS0(KFLO(2)) 
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
        KCP=LUCOMP0(K(NSAV+1,2)) 
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
 
C********************************************************************* 
 
      SUBROUTINE LUKFDI0(KFL1,KFL2,KFL3,KF) 
 
C...Purpose: to generate a new flavour pair and combine off a hadron. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUDAT01/,/LUDAT02/ 
 
C...Default flavour values. Input consistency checks. 
      KF1A=IABS(KFL1) 
      KF2A=IABS(KFL2) 
      KFL3=0 
      KF=0 
      IF(KF1A.EQ.0) RETURN 
      IF(KF2A.NE.0) THEN 
        IF(KF1A.LE.10.AND.KF2A.LE.10.AND.KFL1*KFL2.GT.0) RETURN 
        IF(KF1A.GT.10.AND.KF2A.GT.10) RETURN 
        IF((KF1A.GT.10.OR.KF2A.GT.10).AND.KFL1*KFL2.LT.0) RETURN 
      ENDIF 
 
C...Check if tabulated flavour probabilities are to be used. 
      IF(MSTJ(15).EQ.1) THEN 
        KTAB1=-1 
        IF(KF1A.GE.1.AND.KF1A.LE.6) KTAB1=KF1A 
        KFL1A=MOD(KF1A/1000,10) 
        KFL1B=MOD(KF1A/100,10) 
        KFL1S=MOD(KF1A,10) 
        IF(KFL1A.GE.1.AND.KFL1A.LE.4.AND.KFL1B.GE.1.AND.KFL1B.LE.4) 
     &  KTAB1=6+KFL1A*(KFL1A-2)+2*KFL1B+(KFL1S-1)/2 
        IF(KFL1A.GE.1.AND.KFL1A.LE.4.AND.KFL1A.EQ.KFL1B) KTAB1=KTAB1-1 
        IF(KF1A.GE.1.AND.KF1A.LE.6) KFL1A=KF1A 
        KTAB2=0 
        IF(KF2A.NE.0) THEN 
          KTAB2=-1 
          IF(KF2A.GE.1.AND.KF2A.LE.6) KTAB2=KF2A 
          KFL2A=MOD(KF2A/1000,10) 
          KFL2B=MOD(KF2A/100,10) 
          KFL2S=MOD(KF2A,10) 
          IF(KFL2A.GE.1.AND.KFL2A.LE.4.AND.KFL2B.GE.1.AND.KFL2B.LE.4) 
     &    KTAB2=6+KFL2A*(KFL2A-2)+2*KFL2B+(KFL2S-1)/2 
          IF(KFL2A.GE.1.AND.KFL2A.LE.4.AND.KFL2A.EQ.KFL2B) KTAB2=KTAB2-1 
        ENDIF 
        IF(KTAB1.GE.0.AND.KTAB2.GE.0) GOTO 150 
      ENDIF 
 
C...Parameters and breaking diquark parameter combinations. 
  100 PAR2=PARJ(2) 
      PAR3=PARJ(3) 
      PAR4=3.*PARJ(4) 
      IF(MSTJ(12).GE.2) THEN 
        PAR3M=SQRT(PARJ(3)) 
        PAR4M=1./(3.*SQRT(PARJ(4))) 
        PARDM=PARJ(7)/(PARJ(7)+PAR3M*PARJ(6)) 
        PARS0=PARJ(5)*(2.+(1.+PAR2*PAR3M*PARJ(7))*(1.+PAR4M)) 
        PARS1=PARJ(7)*PARS0/(2.*PAR3M)+PARJ(5)*(PARJ(6)*(1.+PAR4M)+ 
     &  PAR2*PAR3M*PARJ(6)*PARJ(7)) 
        PARS2=PARJ(5)*2.*PARJ(6)*PARJ(7)*(PAR2*PARJ(7)+(1.+PAR4M)/PAR3M) 
        PARSM=MAX(PARS0,PARS1,PARS2) 
        PAR4=PAR4*(1.+PARSM)/(1.+PARSM/(3.*PAR4M)) 
      ENDIF 
 
C...Choice of whether to generate meson or baryon. 
  110 MBARY=0 
      KFDA=0 
      IF(KF1A.LE.10) THEN 
        IF(KF2A.EQ.0.AND.MSTJ(12).GE.1.AND.(1.+PARJ(1))*RLU0(0).GT.1.) 
     &  MBARY=1 
        IF(KF2A.GT.10) MBARY=2 
        IF(KF2A.GT.10.AND.KF2A.LE.10000) KFDA=KF2A 
      ELSE 
        MBARY=2 
        IF(KF1A.LE.10000) KFDA=KF1A 
      ENDIF 
 
C...Possibility of process diquark -> meson + new diquark. 
      IF(KFDA.NE.0.AND.MSTJ(12).GE.2) THEN 
        KFLDA=MOD(KFDA/1000,10) 
        KFLDB=MOD(KFDA/100,10) 
        KFLDS=MOD(KFDA,10) 
        WTDQ=PARS0 
        IF(MAX(KFLDA,KFLDB).EQ.3) WTDQ=PARS1 
        IF(MIN(KFLDA,KFLDB).EQ.3) WTDQ=PARS2 
        IF(KFLDS.EQ.1) WTDQ=WTDQ/(3.*PAR4M) 
        IF((1.+WTDQ)*RLU0(0).GT.1.) MBARY=-1 
        IF(MBARY.EQ.-1.AND.KF2A.NE.0) RETURN 
      ENDIF 
 
C...Flavour for meson, possibly with new flavour. 
      IF(MBARY.LE.0) THEN 
        KFS=ISIGN(1,KFL1) 
        IF(MBARY.EQ.0) THEN 
          IF(KF2A.EQ.0) KFL3=ISIGN(1+INT((2.+PAR2)*RLU0(0)),-KFL1) 
          KFLA=MAX(KF1A,KF2A+IABS(KFL3)) 
          KFLB=MIN(KF1A,KF2A+IABS(KFL3)) 
          IF(KFLA.NE.KF1A) KFS=-KFS 
 
C...Splitting of diquark into meson plus new diquark. 
        ELSE 
          KFL1A=MOD(KF1A/1000,10) 
          KFL1B=MOD(KF1A/100,10) 
  120     KFL1D=KFL1A+INT(RLU0(0)+0.5)*(KFL1B-KFL1A) 
          KFL1E=KFL1A+KFL1B-KFL1D 
          IF((KFL1D.EQ.3.AND.RLU0(0).GT.PARDM).OR.(KFL1E.EQ.3.AND. 
     &    RLU0(0).LT.PARDM)) THEN 
            KFL1D=KFL1A+KFL1B-KFL1D 
            KFL1E=KFL1A+KFL1B-KFL1E 
          ENDIF 
          KFL3A=1+INT((2.+PAR2*PAR3M*PARJ(7))*RLU0(0)) 
          IF((KFL1E.NE.KFL3A.AND.RLU0(0).GT.(1.+PAR4M)/MAX(2.,1.+PAR4M)) 
     &    .OR.(KFL1E.EQ.KFL3A.AND.RLU0(0).GT.2./MAX(2.,1.+PAR4M))) 
     &    GOTO 120 
          KFLDS=3 
          IF(KFL1E.NE.KFL3A) KFLDS=2*INT(RLU0(0)+1./(1.+PAR4M))+1 
          KFL3=ISIGN(10000+1000*MAX(KFL1E,KFL3A)+100*MIN(KFL1E,KFL3A)+ 
     &    KFLDS,-KFL1) 
          KFLA=MAX(KFL1D,KFL3A) 
          KFLB=MIN(KFL1D,KFL3A) 
          IF(KFLA.NE.KFL1D) KFS=-KFS 
        ENDIF 
 
C...Form meson, with spin and flavour mixing for diagonal states. 
        IF(KFLA.LE.2) KMUL=INT(PARJ(11)+RLU0(0)) 
        IF(KFLA.EQ.3) KMUL=INT(PARJ(12)+RLU0(0)) 
        IF(KFLA.GE.4) KMUL=INT(PARJ(13)+RLU0(0)) 
        IF(KMUL.EQ.0.AND.PARJ(14).GT.0.) THEN 
          IF(RLU0(0).LT.PARJ(14)) KMUL=2 
        ELSEIF(KMUL.EQ.1.AND.PARJ(15)+PARJ(16)+PARJ(17).GT.0.) THEN 
          RMUL=RLU0(0) 
          IF(RMUL.LT.PARJ(15)) KMUL=3 
          IF(KMUL.EQ.1.AND.RMUL.LT.PARJ(15)+PARJ(16)) KMUL=4 
          IF(KMUL.EQ.1.AND.RMUL.LT.PARJ(15)+PARJ(16)+PARJ(17)) KMUL=5 
        ENDIF 
        KFLS=3 
        IF(KMUL.EQ.0.OR.KMUL.EQ.3) KFLS=1 
        IF(KMUL.EQ.5) KFLS=5 
        IF(KFLA.NE.KFLB) THEN 
          KF=(100*KFLA+10*KFLB+KFLS)*KFS*(-1)**KFLA 
        ELSE 
          RMIX=RLU0(0) 
          IMIX=2*KFLA+10*KMUL 
          IF(KFLA.LE.3) KF=110*(1+INT(RMIX+PARF(IMIX-1))+ 
     &    INT(RMIX+PARF(IMIX)))+KFLS 
          IF(KFLA.GE.4) KF=110*KFLA+KFLS 
        ENDIF 
        IF(KMUL.EQ.2.OR.KMUL.EQ.3) KF=KF+ISIGN(10000,KF) 
        IF(KMUL.EQ.4) KF=KF+ISIGN(20000,KF) 
 
C...Optional extra suppression of eta and eta'. 
        IF(KF.EQ.221) THEN 
          IF(RLU0(0).GT.PARJ(25)) GOTO 110 
        ELSEIF(KF.EQ.331) THEN 
          IF(RLU0(0).GT.PARJ(26)) GOTO 110 
        ENDIF 
 
C...Generate diquark flavour. 
      ELSE 
  130   IF(KF1A.LE.10.AND.KF2A.EQ.0) THEN 
          KFLA=KF1A 
  140     KFLB=1+INT((2.+PAR2*PAR3)*RLU0(0)) 
          KFLC=1+INT((2.+PAR2*PAR3)*RLU0(0)) 
          KFLDS=1 
          IF(KFLB.GE.KFLC) KFLDS=3 
          IF(KFLDS.EQ.1.AND.PAR4*RLU0(0).GT.1.) GOTO 140 
          IF(KFLDS.EQ.3.AND.PAR4.LT.RLU0(0)) GOTO 140 
          KFL3=ISIGN(1000*MAX(KFLB,KFLC)+100*MIN(KFLB,KFLC)+KFLDS,KFL1) 
 
C...Take diquark flavour from input. 
        ELSEIF(KF1A.LE.10) THEN 
          KFLA=KF1A 
          KFLB=MOD(KF2A/1000,10) 
          KFLC=MOD(KF2A/100,10) 
          KFLDS=MOD(KF2A,10) 
 
C...Generate (or take from input) quark to go with diquark. 
        ELSE 
          IF(KF2A.EQ.0) KFL3=ISIGN(1+INT((2.+PAR2)*RLU0(0)),KFL1) 
          KFLA=KF2A+IABS(KFL3) 
          KFLB=MOD(KF1A/1000,10) 
          KFLC=MOD(KF1A/100,10) 
          KFLDS=MOD(KF1A,10) 
        ENDIF 
 
C...SU(6) factors for formation of baryon. Try again if fails. 
        KBARY=KFLDS 
        IF(KFLDS.EQ.3.AND.KFLB.NE.KFLC) KBARY=5 
        IF(KFLA.NE.KFLB.AND.KFLA.NE.KFLC) KBARY=KBARY+1 
        WT=PARF(60+KBARY)+PARJ(18)*PARF(70+KBARY) 
        IF(MBARY.EQ.1.AND.MSTJ(12).GE.2) THEN 
          WTDQ=PARS0 
          IF(MAX(KFLB,KFLC).EQ.3) WTDQ=PARS1 
          IF(MIN(KFLB,KFLC).EQ.3) WTDQ=PARS2 
          IF(KFLDS.EQ.1) WTDQ=WTDQ/(3.*PAR4M) 
          IF(KFLDS.EQ.1) WT=WT*(1.+WTDQ)/(1.+PARSM/(3.*PAR4M)) 
          IF(KFLDS.EQ.3) WT=WT*(1.+WTDQ)/(1.+PARSM) 
        ENDIF 
        IF(KF2A.EQ.0.AND.WT.LT.RLU0(0)) GOTO 130 
 
C...Form baryon. Distinguish Lambda- and Sigmalike baryons. 
        KFLD=MAX(KFLA,KFLB,KFLC) 
        KFLF=MIN(KFLA,KFLB,KFLC) 
        KFLE=KFLA+KFLB+KFLC-KFLD-KFLF 
        KFLS=2 
        IF((PARF(60+KBARY)+PARJ(18)*PARF(70+KBARY))*RLU0(0).GT. 
     &  PARF(60+KBARY)) KFLS=4 
        KFLL=0 
        IF(KFLS.EQ.2.AND.KFLD.GT.KFLE.AND.KFLE.GT.KFLF) THEN 
          IF(KFLDS.EQ.1.AND.KFLA.EQ.KFLD) KFLL=1 
          IF(KFLDS.EQ.1.AND.KFLA.NE.KFLD) KFLL=INT(0.25+RLU0(0)) 
          IF(KFLDS.EQ.3.AND.KFLA.NE.KFLD) KFLL=INT(0.75+RLU0(0)) 
        ENDIF 
        IF(KFLL.EQ.0) KF=ISIGN(1000*KFLD+100*KFLE+10*KFLF+KFLS,KFL1) 
        IF(KFLL.EQ.1) KF=ISIGN(1000*KFLD+100*KFLF+10*KFLE+KFLS,KFL1) 
      ENDIF 
      RETURN 
 
C...Use tabulated probabilities to select new flavour and hadron. 
  150 IF(KTAB2.EQ.0.AND.MSTJ(12).LE.0) THEN 
        KT3L=1 
        KT3U=6 
      ELSEIF(KTAB2.EQ.0.AND.KTAB1.GE.7.AND.MSTJ(12).LE.1) THEN 
        KT3L=1 
        KT3U=6 
      ELSEIF(KTAB2.EQ.0) THEN 
        KT3L=1 
        KT3U=22 
      ELSE 
        KT3L=KTAB2 
        KT3U=KTAB2 
      ENDIF 
      RFL=0. 
      DO 170 KTS=0,2 
      DO 160 KT3=KT3L,KT3U 
      RFL=RFL+PARF(120+80*KTAB1+25*KTS+KT3) 
  160 CONTINUE 
  170 CONTINUE 
      RFL=RLU0(0)*RFL 
      DO 190 KTS=0,2 
      KTABS=KTS 
      DO 180 KT3=KT3L,KT3U 
      KTAB3=KT3 
      RFL=RFL-PARF(120+80*KTAB1+25*KTS+KT3) 
      IF(RFL.LE.0.) GOTO 200 
  180 CONTINUE 
  190 CONTINUE 
  200 CONTINUE 
 
C...Reconstruct flavour of produced quark/diquark. 
      IF(KTAB3.LE.6) THEN 
        KFL3A=KTAB3 
        KFL3B=0 
        KFL3=ISIGN(KFL3A,KFL1*(2*KTAB1-13)) 
      ELSE 
        KFL3A=1 
        IF(KTAB3.GE.8) KFL3A=2 
        IF(KTAB3.GE.11) KFL3A=3 
        IF(KTAB3.GE.16) KFL3A=4 
        KFL3B=(KTAB3-6-KFL3A*(KFL3A-2))/2 
        KFL3=1000*KFL3A+100*KFL3B+1 
        IF(KFL3A.EQ.KFL3B.OR.KTAB3.NE.6+KFL3A*(KFL3A-2)+2*KFL3B) KFL3= 
     &  KFL3+2 
        KFL3=ISIGN(KFL3,KFL1*(13-2*KTAB1)) 
      ENDIF 
 
C...Reconstruct meson code. 
      IF(KFL3A.EQ.KFL1A.AND.KFL3B.EQ.KFL1B.AND.(KFL3A.LE.3.OR. 
     &KFL3B.NE.0)) THEN 
        RFL=RLU0(0)*(PARF(143+80*KTAB1+25*KTABS)+PARF(144+80*KTAB1+ 
     &  25*KTABS)+PARF(145+80*KTAB1+25*KTABS)) 
        KF=110+2*KTABS+1 
        IF(RFL.GT.PARF(143+80*KTAB1+25*KTABS)) KF=220+2*KTABS+1 
        IF(RFL.GT.PARF(143+80*KTAB1+25*KTABS)+PARF(144+80*KTAB1+ 
     &  25*KTABS)) KF=330+2*KTABS+1 
      ELSEIF(KTAB1.LE.6.AND.KTAB3.LE.6) THEN 
        KFLA=MAX(KTAB1,KTAB3) 
        KFLB=MIN(KTAB1,KTAB3) 
        KFS=ISIGN(1,KFL1) 
        IF(KFLA.NE.KF1A) KFS=-KFS 
        KF=(100*KFLA+10*KFLB+2*KTABS+1)*KFS*(-1)**KFLA 
      ELSEIF(KTAB1.GE.7.AND.KTAB3.GE.7) THEN 
        KFS=ISIGN(1,KFL1) 
        IF(KFL1A.EQ.KFL3A) THEN 
          KFLA=MAX(KFL1B,KFL3B) 
          KFLB=MIN(KFL1B,KFL3B) 
          IF(KFLA.NE.KFL1B) KFS=-KFS 
        ELSEIF(KFL1A.EQ.KFL3B) THEN 
          KFLA=KFL3A 
          KFLB=KFL1B 
          KFS=-KFS 
        ELSEIF(KFL1B.EQ.KFL3A) THEN 
          KFLA=KFL1A 
          KFLB=KFL3B 
        ELSEIF(KFL1B.EQ.KFL3B) THEN 
          KFLA=MAX(KFL1A,KFL3A) 
          KFLB=MIN(KFL1A,KFL3A) 
          IF(KFLA.NE.KFL1A) KFS=-KFS 
        ELSE 
          CALL LUERRM0(2,'(LUKFDI0:) no matching flavours for qq -> qq') 
          GOTO 100 
        ENDIF 
        KF=(100*KFLA+10*KFLB+2*KTABS+1)*KFS*(-1)**KFLA 
 
C...Reconstruct baryon code. 
      ELSE 
        IF(KTAB1.GE.7) THEN 
          KFLA=KFL3A 
          KFLB=KFL1A 
          KFLC=KFL1B 
        ELSE 
          KFLA=KFL1A 
          KFLB=KFL3A 
          KFLC=KFL3B 
        ENDIF 
        KFLD=MAX(KFLA,KFLB,KFLC) 
        KFLF=MIN(KFLA,KFLB,KFLC) 
        KFLE=KFLA+KFLB+KFLC-KFLD-KFLF 
        IF(KTABS.EQ.0) KF=ISIGN(1000*KFLD+100*KFLF+10*KFLE+2,KFL1) 
        IF(KTABS.GE.1) KF=ISIGN(1000*KFLD+100*KFLE+10*KFLF+2*KTABS,KFL1) 
      ENDIF 
 
C...Check that constructed flavour code is an allowed one. 
      IF(KFL2.NE.0) KFL3=0 
      KC=LUCOMP0(KF) 
      IF(KC.EQ.0) THEN 
       CALL LUERRM0(2,'(LUKFDI0:) user-defined flavour probabilities '// 
     &  'failed') 
       GOTO 100 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUPTDI0(KFL,PX,PY) 
 
C...Purpose: to generate transverse momentum according to a Gaussian. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT01/ 
 
C...Generate p_T and azimuthal angle, gives p_x and p_y. 
      KFLA=IABS(KFL) 
      PT=PARJ(21)*SQRT(-LOG(MAX(1E-10,RLU0(0)))) 
      IF(PARJ(23).GT.RLU0(0)) PT=PARJ(24)*PT 
      IF(MSTJ(91).EQ.1) PT=PARJ(22)*PT 
      IF(KFLA.EQ.0.AND.MSTJ(13).LE.0) PT=0. 
      PHI=PARU(2)*RLU0(0) 
      PX=PT*COS(PHI) 
      PY=PT*SIN(PHI) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUZDIS0(KFL1,KFL2,PR,Z) 
 
C...Purpose: to generate the longitudinal splitting variable z. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUDAT01/,/LUDAT02/ 
 
C...Check if heavy flavour fragmentation. 
      KFLA=IABS(KFL1) 
      KFLB=IABS(KFL2) 
      KFLH=KFLA 
      IF(KFLA.GE.10) KFLH=MOD(KFLA/1000,10) 
 
C...Lund symmetric scaling function: determine parameters of shape. 
      IF(MSTJ(11).EQ.1.OR.(MSTJ(11).EQ.3.AND.KFLH.LE.3).OR. 
     &MSTJ(11).GE.4) THEN 
        FA=PARJ(41) 
        IF(MSTJ(91).EQ.1) FA=PARJ(43) 
        IF(KFLB.GE.10) FA=FA+PARJ(45) 
        FBB=PARJ(42) 
        IF(MSTJ(91).EQ.1) FBB=PARJ(44) 
        FB=FBB*PR 
        FC=1. 
        IF(KFLA.GE.10) FC=FC-PARJ(45) 
        IF(KFLB.GE.10) FC=FC+PARJ(45) 
        IF(MSTJ(11).GE.4.AND.KFLH.GE.4.AND.KFLH.LE.5) THEN 
          FRED=PARJ(46) 
          IF(MSTJ(11).EQ.5.AND.KFLH.EQ.5) FRED=PARJ(47) 
          FC=FC+FRED*FBB*PARF(100+KFLH)**2 
        ELSEIF(MSTJ(11).GE.4.AND.KFLH.GE.6.AND.KFLH.LE.8) THEN 
          FRED=PARJ(46) 
          IF(MSTJ(11).EQ.5) FRED=PARJ(48) 
          FC=FC+FRED*FBB*PMAS(KFLH,1)**2 
        ENDIF 
        MC=1 
        IF(ABS(FC-1.).GT.0.01) MC=2 
 
C...Determine position of maximum. Special cases for a = 0 or a = c. 
        IF(FA.LT.0.02) THEN 
          MA=1 
          ZMAX=1. 
          IF(FC.GT.FB) ZMAX=FB/FC 
        ELSEIF(ABS(FC-FA).LT.0.01) THEN 
          MA=2 
          ZMAX=FB/(FB+FC) 
        ELSE 
          MA=3 
          ZMAX=0.5*(FB+FC-SQRT((FB-FC)**2+4.*FA*FB))/(FC-FA) 
          IF(ZMAX.GT.0.9999.AND.FB.GT.100.) ZMAX=MIN(ZMAX,1.-FA/FB) 
        ENDIF 
 
C...Subdivide z range if distribution very peaked near endpoint. 
        MMAX=2 
        IF(ZMAX.LT.0.1) THEN 
          MMAX=1 
          ZDIV=2.75*ZMAX 
          IF(MC.EQ.1) THEN 
            FINT=1.-LOG(ZDIV) 
          ELSE 
            ZDIVC=ZDIV**(1.-FC) 
            FINT=1.+(1.-1./ZDIVC)/(FC-1.) 
          ENDIF 
        ELSEIF(ZMAX.GT.0.85.AND.FB.GT.1.) THEN 
          MMAX=3 
          FSCB=SQRT(4.+(FC/FB)**2) 
          ZDIV=FSCB-1./ZMAX-(FC/FB)*LOG(ZMAX*0.5*(FSCB+FC/FB)) 
          IF(MA.GE.2) ZDIV=ZDIV+(FA/FB)*LOG(1.-ZMAX) 
          ZDIV=MIN(ZMAX,MAX(0.,ZDIV)) 
          FINT=1.+FB*(1.-ZDIV) 
        ENDIF 
 
C...Choice of z, preweighted for peaks at low or high z. 
  100   Z=RLU0(0) 
        FPRE=1. 
        IF(MMAX.EQ.1) THEN 
          IF(FINT*RLU0(0).LE.1.) THEN 
            Z=ZDIV*Z 
          ELSEIF(MC.EQ.1) THEN 
            Z=ZDIV**Z 
            FPRE=ZDIV/Z 
          ELSE 
            Z=(ZDIVC+Z*(1.-ZDIVC))**(1./(1.-FC)) 
            FPRE=(ZDIV/Z)**FC 
          ENDIF 
        ELSEIF(MMAX.EQ.3) THEN 
          IF(FINT*RLU0(0).LE.1.) THEN 
            Z=ZDIV+LOG(Z)/FB 
            FPRE=EXP(FB*(Z-ZDIV)) 
          ELSE 
            Z=ZDIV+Z*(1.-ZDIV) 
          ENDIF 
        ENDIF 
 
C...Weighting according to correct formula. 
        IF(Z.LE.0..OR.Z.GE.1.) GOTO 100 
        FEXP=FC*LOG(ZMAX/Z)+FB*(1./ZMAX-1./Z) 
        IF(MA.GE.2) FEXP=FEXP+FA*LOG((1.-Z)/(1.-ZMAX)) 
        FVAL=EXP(MAX(-50.,MIN(50.,FEXP))) 
        IF(FVAL.LT.RLU0(0)*FPRE) GOTO 100 
 
C...Generate z according to Field-Feynman, SLAC, (1-z)**c OR z**c. 
      ELSE 
        FC=PARJ(50+MAX(1,KFLH)) 
        IF(MSTJ(91).EQ.1) FC=PARJ(59) 
  110   Z=RLU0(0) 
        IF(FC.GE.0..AND.FC.LE.1.) THEN 
          IF(FC.GT.RLU0(0)) Z=1.-Z**(1./3.) 
        ELSEIF(FC.GT.-1.AND.FC.LT.0.) THEN 
          IF(-4.*FC*Z*(1.-Z)**2.LT.RLU0(0)*((1.-Z)**2-FC*Z)**2) GOTO 110 
        ELSE 
          IF(FC.GT.0.) Z=1.-Z**(1./FC) 
          IF(FC.LT.0.) Z=Z**(-1./FC) 
        ENDIF 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUSHOW0(IP1,IP2,QMAX) 
 
C...Purpose: to generate timelike parton showers from given partons. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION PMTH(5,50),PS(5),PMA(4),PMSD(4),IEP(4),IPA(4), 
     &KFLA(4),KFLD(4),KFL(4),ITRY(4),ISI(4),ISL(4),DP(4),DPT(5,4), 
     &KSH(0:40),KCII(2),NIIS(2),IIIS(2,2),THEIIS(2,2),PHIIIS(2,2), 
     &ISII(2) 
 
C...Initialization of cutoff masses etc. 
      IF(MSTJ(41).LE.0.OR.(MSTJ(41).EQ.1.AND.QMAX.LE.PARJ(82)).OR. 
     &QMAX.LE.MIN(PARJ(82),PARJ(83))) RETURN 
      DO 100 IFL=0,40 
      KSH(IFL)=0 
  100 CONTINUE 
      KSH(21)=1 
      PMTH(1,21)=ULMASS0(21) 
      PMTH(2,21)=SQRT(PMTH(1,21)**2+0.25*PARJ(82)**2) 
      PMTH(3,21)=2.*PMTH(2,21) 
      PMTH(4,21)=PMTH(3,21) 
      PMTH(5,21)=PMTH(3,21) 
      PMTH(1,22)=ULMASS0(22) 
      PMTH(2,22)=SQRT(PMTH(1,22)**2+0.25*PARJ(83)**2) 
      PMTH(3,22)=2.*PMTH(2,22) 
      PMTH(4,22)=PMTH(3,22) 
      PMTH(5,22)=PMTH(3,22) 
      PMQTH1=PARJ(82) 
      IF(MSTJ(41).GE.2) PMQTH1=MIN(PARJ(82),PARJ(83)) 
      PMQTH2=PMTH(2,21) 
      IF(MSTJ(41).GE.2) PMQTH2=MIN(PMTH(2,21),PMTH(2,22)) 
      DO 110 IFL=1,8 
      KSH(IFL)=1 
      PMTH(1,IFL)=ULMASS0(IFL) 
      PMTH(2,IFL)=SQRT(PMTH(1,IFL)**2+0.25*PMQTH1**2) 
      PMTH(3,IFL)=PMTH(2,IFL)+PMQTH2 
      PMTH(4,IFL)=SQRT(PMTH(1,IFL)**2+0.25*PARJ(82)**2)+PMTH(2,21) 
      PMTH(5,IFL)=SQRT(PMTH(1,IFL)**2+0.25*PARJ(83)**2)+PMTH(2,22) 
  110 CONTINUE 
      DO 120 IFL=11,17,2 
      IF(MSTJ(41).GE.2) KSH(IFL)=1 
      PMTH(1,IFL)=ULMASS0(IFL) 
      PMTH(2,IFL)=SQRT(PMTH(1,IFL)**2+0.25*PARJ(83)**2) 
      PMTH(3,IFL)=PMTH(2,IFL)+PMTH(2,22) 
      PMTH(4,IFL)=PMTH(3,IFL) 
      PMTH(5,IFL)=PMTH(3,IFL) 
  120 CONTINUE 
      PT2MIN=MAX(0.5*PARJ(82),1.1*PARJ(81))**2 
      ALAMS=PARJ(81)**2 
      ALFM=LOG(PT2MIN/ALAMS) 
 
C...Store positions of shower initiating partons. 
      IF(IP1.GT.0.AND.IP1.LE.MIN(N,MSTU(4)-MSTU(32)).AND.IP2.EQ.0) THEN 
        NPA=1 
        IPA(1)=IP1 
      ELSEIF(MIN(IP1,IP2).GT.0.AND.MAX(IP1,IP2).LE.MIN(N,MSTU(4)- 
     &MSTU(32))) THEN 
        NPA=2 
        IPA(1)=IP1 
        IPA(2)=IP2 
      ELSEIF(IP1.GT.0.AND.IP1.LE.MIN(N,MSTU(4)-MSTU(32)).AND.IP2.LT.0 
     &.AND.IP2.GE.-3) THEN 
        NPA=IABS(IP2) 
        DO 130 I=1,NPA 
        IPA(I)=IP1+I-1 
  130   CONTINUE 
      ELSE 
        CALL LUERRM0(12, 
     &  '(LUSHOW0:) failed to reconstruct showering system') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Check on phase space available for emission. 
      IREJ=0 
      DO 140 J=1,5 
      PS(J)=0. 
  140 CONTINUE 
      PM=0. 
      DO 160 I=1,NPA 
      KFLA(I)=IABS(K(IPA(I),2)) 
      PMA(I)=P(IPA(I),5) 
C...Special cutoff masses for t, l, h with variable masses.
      IFLA=KFLA(I)
      IF(KFLA(I).GE.6.AND.KFLA(I).LE.8) THEN
        IFLA=37+KFLA(I)+ISIGN(2,K(IPA(I),2))
        PMTH(1,IFLA)=PMA(I)
        PMTH(2,IFLA)=SQRT(PMTH(1,IFLA)**2+0.25*PMQTH1**2) 
        PMTH(3,IFLA)=PMTH(2,IFLA)+PMQTH2 
        PMTH(4,IFLA)=SQRT(PMTH(1,IFLA)**2+0.25*PARJ(82)**2)+PMTH(2,21) 
        PMTH(5,IFLA)=SQRT(PMTH(1,IFLA)**2+0.25*PARJ(83)**2)+PMTH(2,22) 
      ENDIF 
      IF(KFLA(I).LE.40) THEN 
        IF(KSH(KFLA(I)).EQ.1) PMA(I)=PMTH(3,IFLA)
      ENDIF 
      PM=PM+PMA(I) 
      IF(KFLA(I).GT.40) THEN 
        IREJ=IREJ+1 
      ELSE 
        IF(KSH(KFLA(I)).EQ.0.OR.PMA(I).GT.QMAX) IREJ=IREJ+1 
      ENDIF 
      DO 150 J=1,4 
      PS(J)=PS(J)+P(IPA(I),J) 
  150 CONTINUE 
  160 CONTINUE 
      IF(IREJ.EQ.NPA) RETURN 
      PS(5)=SQRT(MAX(0.,PS(4)**2-PS(1)**2-PS(2)**2-PS(3)**2)) 
      IF(NPA.EQ.1) PS(5)=PS(4) 
      IF(PS(5).LE.PM+PMQTH1) RETURN 
 
C...Check if 3-jet matrix elements to be used. 
      M3JC=0 
      IF(NPA.EQ.2.AND.MSTJ(47).GE.1) THEN 
        IF(KFLA(1).GE.1.AND.KFLA(1).LE.8.AND.KFLA(2).GE.1.AND. 
     &  KFLA(2).LE.8) M3JC=1 
        IF((KFLA(1).EQ.11.OR.KFLA(1).EQ.13.OR.KFLA(1).EQ.15.OR. 
     &  KFLA(1).EQ.17).AND.KFLA(2).EQ.KFLA(1)) M3JC=1 
        IF((KFLA(1).EQ.11.OR.KFLA(1).EQ.13.OR.KFLA(1).EQ.15.OR. 
     &  KFLA(1).EQ.17).AND.KFLA(2).EQ.KFLA(1)+1) M3JC=1 
        IF((KFLA(1).EQ.12.OR.KFLA(1).EQ.14.OR.KFLA(1).EQ.16.OR. 
     &  KFLA(1).EQ.18).AND.KFLA(2).EQ.KFLA(1)-1) M3JC=1 
        IF(MSTJ(47).EQ.2.OR.MSTJ(47).EQ.4) M3JC=1 
        M3JCM=0 
        IF(M3JC.EQ.1.AND.MSTJ(47).GE.3.AND.KFLA(1).EQ.KFLA(2)) THEN 
          M3JCM=1 
          QME=(2.*PMTH(1,KFLA(1))/PS(5))**2 
        ENDIF 
      ENDIF 
 
C...Find if interference with initial state partons. 
      MIIS=0 
      IF(MSTJ(50).GE.1.AND.MSTJ(50).LE.3.AND.NPA.EQ.2) MIIS=MSTJ(50) 
      IF(MIIS.NE.0) THEN 
        DO 180 I=1,2 
        KCII(I)=0 
        KCA=LUCOMP0(KFLA(I)) 
        IF(KCA.NE.0) KCII(I)=KCHG(KCA,2)*ISIGN(1,K(IPA(I),2)) 
        NIIS(I)=0 
        IF(KCII(I).NE.0) THEN 
          DO 170 J=1,2 
          ICSI=MOD(K(IPA(I),3+J)/MSTU(5),MSTU(5)) 
          IF(ICSI.GT.0.AND.ICSI.NE.IPA(1).AND.ICSI.NE.IPA(2).AND. 
     &    (KCII(I).EQ.(-1)**(J+1).OR.KCII(I).EQ.2)) THEN 
            NIIS(I)=NIIS(I)+1 
            IIIS(I,NIIS(I))=ICSI 
          ENDIF 
  170     CONTINUE 
        ENDIF 
  180   CONTINUE 
        IF(NIIS(1)+NIIS(2).EQ.0) MIIS=0 
      ENDIF 
 
C...Boost interfering initial partons to rest frame 
C...and reconstruct their polar and azimuthal angles. 
      IF(MIIS.NE.0) THEN 
        DO 200 I=1,2 
        DO 190 J=1,5 
        K(N+I,J)=K(IPA(I),J) 
        P(N+I,J)=P(IPA(I),J) 
        V(N+I,J)=0. 
  190   CONTINUE 
  200   CONTINUE 
        DO 220 I=3,2+NIIS(1) 
        DO 210 J=1,5 
        K(N+I,J)=K(IIIS(1,I-2),J) 
        P(N+I,J)=P(IIIS(1,I-2),J) 
        V(N+I,J)=0. 
  210   CONTINUE 
  220   CONTINUE 
        DO 240 I=3+NIIS(1),2+NIIS(1)+NIIS(2) 
        DO 230 J=1,5 
        K(N+I,J)=K(IIIS(2,I-2-NIIS(1)),J) 
        P(N+I,J)=P(IIIS(2,I-2-NIIS(1)),J) 
        V(N+I,J)=0. 
  230   CONTINUE 
  240   CONTINUE 
        CALL LUDBRB0(N+1,N+2+NIIS(1)+NIIS(2),0.,0.,-DBLE(PS(1)/PS(4)), 
     &  -DBLE(PS(2)/PS(4)),-DBLE(PS(3)/PS(4))) 
        PHI=ULANGL0(P(N+1,1),P(N+1,2)) 
        CALL LUDBRB0(N+1,N+2+NIIS(1)+NIIS(2),0.,-PHI,0D0,0D0,0D0) 
        THE=ULANGL0(P(N+1,3),P(N+1,1)) 
        CALL LUDBRB0(N+1,N+2+NIIS(1)+NIIS(2),-THE,0.,0D0,0D0,0D0) 
        DO 250 I=3,2+NIIS(1) 
        THEIIS(1,I-2)=ULANGL0(P(N+I,3),SQRT(P(N+I,1)**2+P(N+I,2)**2)) 
        PHIIIS(1,I-2)=ULANGL0(P(N+I,1),P(N+I,2)) 
  250   CONTINUE 
        DO 260 I=3+NIIS(1),2+NIIS(1)+NIIS(2) 
        THEIIS(2,I-2-NIIS(1))=PARU(1)-ULANGL0(P(N+I,3), 
     &  SQRT(P(N+I,1)**2+P(N+I,2)**2)) 
        PHIIIS(2,I-2-NIIS(1))=ULANGL0(P(N+I,1),P(N+I,2)) 
  260   CONTINUE 
      ENDIF 
 
C...Define imagined single initiator of shower for parton system. 
      NS=N 
      IF(N.GT.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUSHOW0:) no more memory left in LUJETS') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(NPA.GE.2) THEN 
        K(N+1,1)=11 
        K(N+1,2)=21 
        K(N+1,3)=0 
        K(N+1,4)=0 
        K(N+1,5)=0 
        P(N+1,1)=0. 
        P(N+1,2)=0. 
        P(N+1,3)=0. 
        P(N+1,4)=PS(5) 
        P(N+1,5)=PS(5) 
        V(N+1,5)=PS(5)**2 
        N=N+1 
      ENDIF 
 
C...Loop over partons that may branch. 
      NEP=NPA 
      IM=NS 
      IF(NPA.EQ.1) IM=NS-1 
  270 IM=IM+1 
      IF(N.GT.NS) THEN 
        IF(IM.GT.N) GOTO 510 
        KFLM=IABS(K(IM,2)) 
        IF(KFLM.GT.40) GOTO 270 
        IF(KSH(KFLM).EQ.0) GOTO 270 
        IFLM=KFLM
        IF(KFLM.GE.6.AND.KFLM.LE.8) IFLM=37+KFLM+ISIGN(2,K(IM,2)) 
        IF(P(IM,5).LT.PMTH(2,IFLM)) GOTO 270 
        IGM=K(IM,3) 
      ELSE 
        IGM=-1 
      ENDIF 
      IF(N+NEP.GT.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUSHOW0:) no more memory left in LUJETS') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Position of aunt (sister to branching parton). 
C...Origin and flavour of daughters. 
      IAU=0 
      IF(IGM.GT.0) THEN 
        IF(K(IM-1,3).EQ.IGM) IAU=IM-1 
        IF(N.GE.IM+1.AND.K(IM+1,3).EQ.IGM) IAU=IM+1 
      ENDIF 
      IF(IGM.GE.0) THEN 
        K(IM,4)=N+1 
        DO 280 I=1,NEP 
        K(N+I,3)=IM 
  280   CONTINUE 
      ELSE 
        K(N+1,3)=IPA(1) 
      ENDIF 
      IF(IGM.LE.0) THEN 
        DO 290 I=1,NEP 
        K(N+I,2)=K(IPA(I),2) 
  290   CONTINUE 
      ELSEIF(KFLM.NE.21) THEN 
        K(N+1,2)=K(IM,2) 
        K(N+2,2)=K(IM,5) 
      ELSEIF(K(IM,5).EQ.21) THEN 
        K(N+1,2)=21 
        K(N+2,2)=21 
      ELSE 
        K(N+1,2)=K(IM,5) 
        K(N+2,2)=-K(IM,5) 
      ENDIF 
 
C...Reset flags on daughers and tries made. 
      DO 300 IP=1,NEP 
      K(N+IP,1)=3 
      K(N+IP,4)=0 
      K(N+IP,5)=0 
      KFLD(IP)=IABS(K(N+IP,2)) 
      IF(KCHG(LUCOMP0(KFLD(IP)),2).EQ.0) K(N+IP,1)=1 
      ITRY(IP)=0 
      ISL(IP)=0 
      ISI(IP)=0 
      IF(KFLD(IP).LE.40) THEN 
        IF(KSH(KFLD(IP)).EQ.1) ISI(IP)=1 
      ENDIF 
  300 CONTINUE 
      ISLM=0 
 
C...Maximum virtuality of daughters. 
      IF(IGM.LE.0) THEN 
        DO 310 I=1,NPA 
        IF(NPA.GE.3) P(N+I,4)=(PS(4)*P(IPA(I),4)-PS(1)*P(IPA(I),1)- 
     &  PS(2)*P(IPA(I),2)-PS(3)*P(IPA(I),3))/PS(5) 
        P(N+I,5)=MIN(QMAX,PS(5)) 
        IF(NPA.GE.3) P(N+I,5)=MIN(P(N+I,5),P(N+I,4)) 
        IF(ISI(I).EQ.0) P(N+I,5)=P(IPA(I),5) 
  310   CONTINUE 
      ELSE 
        IF(MSTJ(43).LE.2) PEM=V(IM,2) 
        IF(MSTJ(43).GE.3) PEM=P(IM,4) 
        P(N+1,5)=MIN(P(IM,5),V(IM,1)*PEM) 
        P(N+2,5)=MIN(P(IM,5),(1.-V(IM,1))*PEM) 
        IF(K(N+2,2).EQ.22) P(N+2,5)=PMTH(1,22) 
      ENDIF 
      DO 320 I=1,NEP 
      PMSD(I)=P(N+I,5) 
      IF(ISI(I).EQ.1) THEN 
        IFLD=KFLD(I)
        IF(KFLD(I).GE.6.AND.KFLD(I).LE.8) IFLD=37+KFLD(I)+
     &  ISIGN(2,K(N+I,2)) 
        IF(P(N+I,5).LE.PMTH(3,IFLD)) P(N+I,5)=PMTH(1,IFLD) 
      ENDIF 
      V(N+I,5)=P(N+I,5)**2 
  320 CONTINUE 
 
C...Choose one of the daughters for evolution. 
  330 INUM=0 
      IF(NEP.EQ.1) INUM=1 
      DO 340 I=1,NEP 
      IF(INUM.EQ.0.AND.ISL(I).EQ.1) INUM=I 
  340 CONTINUE 
      DO 350 I=1,NEP 
      IF(INUM.EQ.0.AND.ITRY(I).EQ.0.AND.ISI(I).EQ.1) THEN 
        IFLD=KFLD(I)
        IF(KFLD(I).GE.6.AND.KFLD(I).LE.8) IFLD=37+KFLD(I)+
     &  ISIGN(2,K(N+I,2)) 
        IF(P(N+I,5).GE.PMTH(2,IFLD)) INUM=I 
      ENDIF 
  350 CONTINUE 
      IF(INUM.EQ.0) THEN 
        RMAX=0. 
        DO 360 I=1,NEP 
        IF(ISI(I).EQ.1.AND.PMSD(I).GE.PMQTH2) THEN 
          RPM=P(N+I,5)/PMSD(I) 
          IFLD=KFLD(I)
          IF(KFLD(I).GE.6.AND.KFLD(I).LE.8) IFLD=37+KFLD(I)+
     &    ISIGN(2,K(N+I,2)) 
          IF(RPM.GT.RMAX.AND.P(N+I,5).GE.PMTH(2,IFLD)) THEN 
            RMAX=RPM 
            INUM=I 
          ENDIF 
        ENDIF 
  360   CONTINUE 
      ENDIF 
 
C...Store information on choice of evolving daughter. 
      INUM=MAX(1,INUM) 
      IEP(1)=N+INUM 
      DO 370 I=2,NEP 
      IEP(I)=IEP(I-1)+1 
      IF(IEP(I).GT.N+NEP) IEP(I)=N+1 
  370 CONTINUE 
      DO 380 I=1,NEP 
      KFL(I)=IABS(K(IEP(I),2)) 
  380 CONTINUE 
      ITRY(INUM)=ITRY(INUM)+1 
      IF(ITRY(INUM).GT.200) THEN 
        CALL LUERRM0(14,'(LUSHOW0:) caught in infinite loop') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      Z=0.5 
      IF(KFL(1).GT.40) GOTO 430 
      IF(KSH(KFL(1)).EQ.0) GOTO 430 
      IFL=KFL(1)
      IF(KFL(1).GE.6.AND.KFL(1).LE.8) IFL=37+KFL(1)+
     &ISIGN(2,K(IEP(1),2)) 
      IF(P(IEP(1),5).LT.PMTH(2,IFL)) GOTO 430 
 
C...Select side for interference with initial state partons. 
      IF(MIIS.GE.1.AND.IEP(1).LE.NS+3) THEN 
        III=IEP(1)-NS-1 
        ISII(III)=0 
        IF(IABS(KCII(III)).EQ.1.AND.NIIS(III).EQ.1) THEN 
          ISII(III)=1 
        ELSEIF(KCII(III).EQ.2.AND.NIIS(III).EQ.1) THEN 
          IF(RLU0(0).GT.0.5) ISII(III)=1 
        ELSEIF(KCII(III).EQ.2.AND.NIIS(III).EQ.2) THEN 
          ISII(III)=1 
          IF(RLU0(0).GT.0.5) ISII(III)=2 
        ENDIF 
      ENDIF 
 
C...Calculate allowed z range. 
      IF(NEP.EQ.1) THEN 
        PMED=PS(4) 
      ELSEIF(IGM.EQ.0.OR.MSTJ(43).LE.2) THEN 
        PMED=P(IM,5) 
      ELSE 
        IF(INUM.EQ.1) PMED=V(IM,1)*PEM 
        IF(INUM.EQ.2) PMED=(1.-V(IM,1))*PEM 
      ENDIF 
      IF(MOD(MSTJ(43),2).EQ.1) THEN 
        ZC=PMTH(2,21)/PMED 
        ZCE=PMTH(2,22)/PMED 
      ELSE 
        ZC=0.5*(1.-SQRT(MAX(0.,1.-(2.*PMTH(2,21)/PMED)**2))) 
        IF(ZC.LT.1E-4) ZC=(PMTH(2,21)/PMED)**2 
        ZCE=0.5*(1.-SQRT(MAX(0.,1.-(2.*PMTH(2,22)/PMED)**2))) 
        IF(ZCE.LT.1E-4) ZCE=(PMTH(2,22)/PMED)**2 
      ENDIF 
      ZC=MIN(ZC,0.491) 
      ZCE=MIN(ZCE,0.491) 
      IF((MSTJ(41).EQ.1.AND.ZC.GT.0.49).OR.(MSTJ(41).GE.2.AND. 
     &MIN(ZC,ZCE).GT.0.49)) THEN 
        P(IEP(1),5)=PMTH(1,IFL) 
        V(IEP(1),5)=P(IEP(1),5)**2 
        GOTO 430 
      ENDIF 
 
C...Integral of Altarelli-Parisi z kernel for QCD. 
      IF(MSTJ(49).EQ.0.AND.KFL(1).EQ.21) THEN 
        FBR=6.*LOG((1.-ZC)/ZC)+MSTJ(45)*(0.5-ZC) 
      ELSEIF(MSTJ(49).EQ.0) THEN 
        FBR=(8./3.)*LOG((1.-ZC)/ZC) 
 
C...Integral of Altarelli-Parisi z kernel for scalar gluon. 
      ELSEIF(MSTJ(49).EQ.1.AND.KFL(1).EQ.21) THEN 
        FBR=(PARJ(87)+MSTJ(45)*PARJ(88))*(1.-2.*ZC) 
      ELSEIF(MSTJ(49).EQ.1) THEN 
        FBR=(1.-2.*ZC)/3. 
        IF(IGM.EQ.0.AND.M3JC.EQ.1) FBR=4.*FBR 
 
C...Integral of Altarelli-Parisi z kernel for Abelian vector gluon. 
      ELSEIF(KFL(1).EQ.21) THEN 
        FBR=6.*MSTJ(45)*(0.5-ZC) 
      ELSE 
        FBR=2.*LOG((1.-ZC)/ZC) 
      ENDIF 
 
C...Reset QCD probability for lepton. 
      IF(KFL(1).GE.11.AND.KFL(1).LE.18) FBR=0. 
 
C...Integral of Altarelli-Parisi kernel for photon emission. 
      IF(MSTJ(41).GE.2.AND.KFL(1).GE.1.AND.KFL(1).LE.18) THEN 
        FBRE=(KCHG(KFL(1),1)/3.)**2*2.*LOG((1.-ZCE)/ZCE) 
        IF(MSTJ(41).EQ.10) FBRE=PARJ(84)*FBRE 
      ENDIF 
 
C...Inner veto algorithm starts. Find maximum mass for evolution. 
  390 PMS=V(IEP(1),5) 
      IF(IGM.GE.0) THEN 
        PM2=0. 
        DO 400 I=2,NEP 
        PM=P(IEP(I),5) 
        IF(KFL(I).LE.40) THEN 
          IFLI=KFL(I)
          IF(KFL(I).GE.6.AND.KFL(I).LE.8) IFLI=37+KFL(I)+
     &    ISIGN(2,K(IEP(I),2)) 
          IF(KSH(KFL(I)).EQ.1) PM=PMTH(2,IFLI) 
        ENDIF 
        PM2=PM2+PM 
  400   CONTINUE 
        PMS=MIN(PMS,(P(IM,5)-PM2)**2) 
      ENDIF 
 
C...Select mass for daughter in QCD evolution. 
      B0=27./6. 
      DO 410 IFF=4,MSTJ(45) 
      IF(PMS.GT.4.*PMTH(2,IFF)**2) B0=(33.-2.*IFF)/6. 
  410 CONTINUE 
      IF(FBR.LT.1E-3) THEN 
        PMSQCD=0. 
      ELSEIF(MSTJ(44).LE.0) THEN 
        PMSQCD=PMS*EXP(MAX(-50.,LOG(RLU0(0))*PARU(2)/(PARU(111)*FBR))) 
      ELSEIF(MSTJ(44).EQ.1) THEN 
        PMSQCD=4.*ALAMS*(0.25*PMS/ALAMS)**(RLU0(0)**(B0/FBR)) 
      ELSE 
        PMSQCD=PMS*EXP(MAX(-50.,ALFM*B0*LOG(RLU0(0))/FBR)) 
      ENDIF 
      IF(ZC.GT.0.49.OR.PMSQCD.LE.PMTH(4,IFL)**2) PMSQCD=PMTH(2,IFL)**2 
      V(IEP(1),5)=PMSQCD 
      MCE=1 
 
C...Select mass for daughter in QED evolution. 
      IF(MSTJ(41).GE.2.AND.KFL(1).GE.1.AND.KFL(1).LE.18) THEN 
        PMSQED=PMS*EXP(MAX(-50.,LOG(RLU0(0))*PARU(2)/(PARU(101)*FBRE))) 
        IF(ZCE.GT.0.49.OR.PMSQED.LE.PMTH(5,IFL)**2) PMSQED= 
     &  PMTH(2,IFL)**2 
        IF(PMSQED.GT.PMSQCD) THEN 
          V(IEP(1),5)=PMSQED 
          MCE=2 
        ENDIF 
      ENDIF 
 
C...Check whether daughter mass below cutoff. 
      P(IEP(1),5)=SQRT(V(IEP(1),5)) 
      IF(P(IEP(1),5).LE.PMTH(3,IFL)) THEN 
        P(IEP(1),5)=PMTH(1,IFL) 
        V(IEP(1),5)=P(IEP(1),5)**2 
        GOTO 430 
      ENDIF 
 
C...Select z value of branching: q -> qgamma. 
      IF(MCE.EQ.2) THEN 
        Z=1.-(1.-ZCE)*(ZCE/(1.-ZCE))**RLU0(0) 
        IF(1.+Z**2.LT.2.*RLU0(0)) GOTO 390 
        K(IEP(1),5)=22 
 
C...Select z value of branching: q -> qg, g -> gg, g -> qqbar. 
      ELSEIF(MSTJ(49).NE.1.AND.KFL(1).NE.21) THEN 
        Z=1.-(1.-ZC)*(ZC/(1.-ZC))**RLU0(0) 
        IF(1.+Z**2.LT.2.*RLU0(0)) GOTO 390 
        K(IEP(1),5)=21 
      ELSEIF(MSTJ(49).EQ.0.AND.MSTJ(45)*(0.5-ZC).LT.RLU0(0)*FBR) THEN 
        Z=(1.-ZC)*(ZC/(1.-ZC))**RLU0(0) 
        IF(RLU0(0).GT.0.5) Z=1.-Z 
        IF((1.-Z*(1.-Z))**2.LT.RLU0(0)) GOTO 390 
        K(IEP(1),5)=21 
      ELSEIF(MSTJ(49).NE.1) THEN 
        Z=ZC+(1.-2.*ZC)*RLU0(0) 
        IF(Z**2+(1.-Z)**2.LT.RLU0(0)) GOTO 390 
        KFLB=1+INT(MSTJ(45)*RLU0(0)) 
        PMQ=4.*PMTH(2,KFLB)**2/V(IEP(1),5) 
        IF(PMQ.GE.1.) GOTO 390 
        PMQ0=4.*PMTH(2,21)**2/V(IEP(1),5) 
        IF(MOD(MSTJ(43),2).EQ.0.AND.(1.+0.5*PMQ)*SQRT(1.-PMQ).LT. 
     &  RLU0(0)*(1.+0.5*PMQ0)*SQRT(1.-PMQ0)) GOTO 390 
        K(IEP(1),5)=KFLB 
 
C...Ditto for scalar gluon model. 
      ELSEIF(KFL(1).NE.21) THEN 
        Z=1.-SQRT(ZC**2+RLU0(0)*(1.-2.*ZC)) 
        K(IEP(1),5)=21 
      ELSEIF(RLU0(0)*(PARJ(87)+MSTJ(45)*PARJ(88)).LE.PARJ(87)) THEN 
        Z=ZC+(1.-2.*ZC)*RLU0(0) 
        K(IEP(1),5)=21 
      ELSE 
        Z=ZC+(1.-2.*ZC)*RLU0(0) 
        KFLB=1+INT(MSTJ(45)*RLU0(0)) 
        PMQ=4.*PMTH(2,KFLB)**2/V(IEP(1),5) 
        IF(PMQ.GE.1.) GOTO 390 
        K(IEP(1),5)=KFLB 
      ENDIF 
      IF(MCE.EQ.1.AND.MSTJ(44).GE.2) THEN 
        IF(Z*(1.-Z)*V(IEP(1),5).LT.PT2MIN) GOTO 390 
        IF(ALFM/LOG(V(IEP(1),5)*Z*(1.-Z)/ALAMS).LT.RLU0(0)) GOTO 390 
      ENDIF 
 
C...Check if z consistent with chosen m. 
      IF(KFL(1).EQ.21) THEN 
        KFLGD1=IABS(K(IEP(1),5)) 
        KFLGD2=KFLGD1 
      ELSE 
        KFLGD1=KFL(1) 
        KFLGD2=IABS(K(IEP(1),5)) 
      ENDIF 
      IF(NEP.EQ.1) THEN 
        PED=PS(4) 
      ELSEIF(NEP.GE.3) THEN 
        PED=P(IEP(1),4) 
      ELSEIF(IGM.EQ.0.OR.MSTJ(43).LE.2) THEN 
        PED=0.5*(V(IM,5)+V(IEP(1),5)-PM2**2)/P(IM,5) 
      ELSE 
        IF(IEP(1).EQ.N+1) PED=V(IM,1)*PEM 
        IF(IEP(1).EQ.N+2) PED=(1.-V(IM,1))*PEM 
      ENDIF 
      IF(MOD(MSTJ(43),2).EQ.1) THEN 
        IFLGD1=KFLGD1
        IF(KFLGD1.GE.6.AND.KFLGD1.LE.8) IFLGD1=IFL
        PMQTH3=0.5*PARJ(82) 
        IF(KFLGD2.EQ.22) PMQTH3=0.5*PARJ(83) 
        PMQ1=(PMTH(1,IFLGD1)**2+PMQTH3**2)/V(IEP(1),5) 
        PMQ2=(PMTH(1,KFLGD2)**2+PMQTH3**2)/V(IEP(1),5) 
        ZD=SQRT(MAX(0.,(1.-V(IEP(1),5)/PED**2)*((1.-PMQ1-PMQ2)**2- 
     &  4.*PMQ1*PMQ2))) 
        ZH=1.+PMQ1-PMQ2 
      ELSE 
        ZD=SQRT(MAX(0.,1.-V(IEP(1),5)/PED**2)) 
        ZH=1. 
      ENDIF 
      ZL=0.5*(ZH-ZD) 
      ZU=0.5*(ZH+ZD) 
      IF(Z.LT.ZL.OR.Z.GT.ZU) GOTO 390 
      IF(KFL(1).EQ.21) V(IEP(1),3)=LOG(ZU*(1.-ZL)/MAX(1E-20,ZL* 
     &(1.-ZU))) 
      IF(KFL(1).NE.21) V(IEP(1),3)=LOG((1.-ZL)/MAX(1E-10,1.-ZU)) 
 
C...Width suppression for q -> q + g.
      IF(MSTJ(40).NE.0.AND.KFL(1).NE.21) THEN
        IF(IGM.EQ.0) THEN
          EGLU=0.5*PS(5)*(1.-Z)*(1.+V(IEP(1),5)/V(NS+1,5))
        ELSE
          EGLU=PMED*(1.-Z)
        ENDIF
        CHI=PARJ(89)**2/(PARJ(89)**2+EGLU**2)
        IF(MSTJ(40).EQ.1) THEN
          IF(CHI.LT.RLU0(0)) GOTO 390  
        ELSEIF(MSTJ(40).EQ.2) THEN
          IF(1.-CHI.LT.RLU0(0)) GOTO 390
        ENDIF
      ENDIF
 
C...Three-jet matrix element correction. 
      IF(IGM.EQ.0.AND.M3JC.EQ.1) THEN 
        X1=Z*(1.+V(IEP(1),5)/V(NS+1,5)) 
        X2=1.-V(IEP(1),5)/V(NS+1,5) 
        X3=(1.-X1)+(1.-X2) 
        IF(MCE.EQ.2) THEN 
          KI1=K(IPA(INUM),2) 
          KI2=K(IPA(3-INUM),2) 
          QF1=KCHG(IABS(KI1),1)*ISIGN(1,KI1)/3. 
          QF2=KCHG(IABS(KI2),1)*ISIGN(1,KI2)/3. 
          WSHOW=QF1**2*(1.-X1)/X3*(1.+(X1/(2.-X2))**2)+ 
     &    QF2**2*(1.-X2)/X3*(1.+(X2/(2.-X1))**2) 
          WME=(QF1*(1.-X1)/X3-QF2*(1.-X2)/X3)**2*(X1**2+X2**2) 
        ELSEIF(MSTJ(49).NE.1) THEN 
          WSHOW=1.+(1.-X1)/X3*(X1/(2.-X2))**2+ 
     &    (1.-X2)/X3*(X2/(2.-X1))**2 
          WME=X1**2+X2**2 
          IF(M3JCM.EQ.1) WME=WME-QME*X3-0.5*QME**2- 
     &    (0.5*QME+0.25*QME**2)*((1.-X2)/MAX(1E-7,1.-X1)+
     &    (1.-X1)/MAX(1E-7,1.-X2)) 
        ELSE 
          WSHOW=4.*X3*((1.-X1)/(2.-X2)**2+(1.-X2)/(2.-X1)**2) 
          WME=X3**2 
          IF(MSTJ(102).GE.2) WME=X3**2-2.*(1.+X3)*(1.-X1)*(1.-X2)* 
     &    PARJ(171) 
        ENDIF 
        IF(WME.LT.RLU0(0)*WSHOW) GOTO 390 
 
C...Impose angular ordering by rejection of nonordered emission. 
      ELSEIF(MCE.EQ.1.AND.IGM.GT.0.AND.MSTJ(42).GE.2) THEN 
        MAOM=1 
        ZM=V(IM,1) 
        IF(IEP(1).EQ.N+2) ZM=1.-V(IM,1) 
        THE2ID=Z*(1.-Z)*(ZM*P(IM,4))**2/V(IEP(1),5) 
        IAOM=IM 
  420   IF(K(IAOM,5).EQ.22) THEN 
          IAOM=K(IAOM,3) 
          IF(K(IAOM,3).LE.NS) MAOM=0 
          IF(MAOM.EQ.1) GOTO 420 
        ENDIF 
        IF(MAOM.EQ.1) THEN 
          THE2IM=V(IAOM,1)*(1.-V(IAOM,1))*P(IAOM,4)**2/V(IAOM,5) 
          IF(THE2ID.LT.THE2IM) GOTO 390 
        ENDIF 
      ENDIF 
 
C...Impose user-defined maximum angle at first branching. 
      IF(MSTJ(48).EQ.1) THEN 
        IF(NEP.EQ.1.AND.IM.EQ.NS) THEN 
          THE2ID=Z*(1.-Z)*PS(4)**2/V(IEP(1),5) 
          IF(THE2ID.LT.1./PARJ(85)**2) GOTO 390 
        ELSEIF(NEP.EQ.2.AND.IEP(1).EQ.NS+2) THEN 
          THE2ID=Z*(1.-Z)*(0.5*P(IM,4))**2/V(IEP(1),5) 
          IF(THE2ID.LT.1./PARJ(85)**2) GOTO 390 
        ELSEIF(NEP.EQ.2.AND.IEP(1).EQ.NS+3) THEN 
          THE2ID=Z*(1.-Z)*(0.5*P(IM,4))**2/V(IEP(1),5) 
          IF(THE2ID.LT.1./PARJ(86)**2) GOTO 390 
        ENDIF 
      ENDIF 
 
C...Impose angular constraint in first branching from interference 
C...with initial state partons. 
      IF(MIIS.GE.2.AND.IEP(1).LE.NS+3) THEN 
        THE2D=MAX((1.-Z)/Z,Z/(1.-Z))*V(IEP(1),5)/(0.5*P(IM,4))**2 
        IF(IEP(1).EQ.NS+2.AND.ISII(1).GE.1) THEN 
          IF(THE2D.GT.THEIIS(1,ISII(1))**2) GOTO 390 
        ELSEIF(IEP(1).EQ.NS+3.AND.ISII(2).GE.1) THEN 
          IF(THE2D.GT.THEIIS(2,ISII(2))**2) GOTO 390 
        ENDIF 
      ENDIF 
 
C...End of inner veto algorithm. Check if only one leg evolved so far. 
  430 V(IEP(1),1)=Z 
      ISL(1)=0 
      ISL(2)=0 
      IF(NEP.EQ.1) GOTO 460 
      IF(NEP.EQ.2.AND.P(IEP(1),5)+P(IEP(2),5).GE.P(IM,5)) GOTO 330 
      DO 440 I=1,NEP 
      IF(ITRY(I).EQ.0.AND.KFLD(I).LE.40) THEN 
        IF(KSH(KFLD(I)).EQ.1) THEN 
          IFLD=KFLD(I)
          IF(KFLD(I).GE.6.AND.KFLD(I).LE.8) IFLD=37+KFLD(I)+
     &    ISIGN(2,K(N+I,2)) 
          IF(P(N+I,5).GE.PMTH(2,IFLD)) GOTO 330 
        ENDIF 
      ENDIF 
  440 CONTINUE 
 
C...Check if chosen multiplet m1,m2,z1,z2 is physical. 
      IF(NEP.EQ.3) THEN 
        PA1S=(P(N+1,4)+P(N+1,5))*(P(N+1,4)-P(N+1,5)) 
        PA2S=(P(N+2,4)+P(N+2,5))*(P(N+2,4)-P(N+2,5)) 
        PA3S=(P(N+3,4)+P(N+3,5))*(P(N+3,4)-P(N+3,5)) 
        PTS=0.25*(2.*PA1S*PA2S+2.*PA1S*PA3S+2.*PA2S*PA3S- 
     &  PA1S**2-PA2S**2-PA3S**2)/PA1S 
        IF(PTS.LE.0.) GOTO 330 
      ELSEIF(IGM.EQ.0.OR.MSTJ(43).LE.2.OR.MOD(MSTJ(43),2).EQ.0) THEN 
        DO 450 I1=N+1,N+2 
        KFLDA=IABS(K(I1,2)) 
        IF(KFLDA.GT.40) GOTO 450 
        IF(KSH(KFLDA).EQ.0) GOTO 450 
        IFLDA=KFLDA 
        IF(KFLDA.GE.6.AND.KFLDA.LE.8) IFLDA=37+KFLDA+
     &  ISIGN(2,K(I1,2)) 
        IF(P(I1,5).LT.PMTH(2,IFLDA)) GOTO 450 
        IF(KFLDA.EQ.21) THEN 
          KFLGD1=IABS(K(I1,5)) 
          KFLGD2=KFLGD1 
        ELSE 
          KFLGD1=KFLDA 
          KFLGD2=IABS(K(I1,5)) 
        ENDIF 
        I2=2*N+3-I1 
        IF(IGM.EQ.0.OR.MSTJ(43).LE.2) THEN 
          PED=0.5*(V(IM,5)+V(I1,5)-V(I2,5))/P(IM,5) 
        ELSE 
          IF(I1.EQ.N+1) ZM=V(IM,1) 
          IF(I1.EQ.N+2) ZM=1.-V(IM,1) 
          PML=SQRT((V(IM,5)-V(N+1,5)-V(N+2,5))**2- 
     &    4.*V(N+1,5)*V(N+2,5)) 
          PED=PEM*(0.5*(V(IM,5)-PML+V(I1,5)-V(I2,5))+PML*ZM)/V(IM,5) 
        ENDIF 
        IF(MOD(MSTJ(43),2).EQ.1) THEN 
          PMQTH3=0.5*PARJ(82) 
          IF(KFLGD2.EQ.22) PMQTH3=0.5*PARJ(83) 
          IFLGD1=KFLGD1
          IF(KFLGD1.GE.6.AND.KFLGD1.LE.8) IFLGD1=IFLDA
          PMQ1=(PMTH(1,IFLGD1)**2+PMQTH3**2)/V(I1,5) 
          PMQ2=(PMTH(1,KFLGD2)**2+PMQTH3**2)/V(I1,5) 
          ZD=SQRT(MAX(0.,(1.-V(I1,5)/PED**2)*((1.-PMQ1-PMQ2)**2- 
     &    4.*PMQ1*PMQ2))) 
          ZH=1.+PMQ1-PMQ2 
        ELSE 
          ZD=SQRT(MAX(0.,1.-V(I1,5)/PED**2)) 
          ZH=1. 
        ENDIF 
        ZL=0.5*(ZH-ZD) 
        ZU=0.5*(ZH+ZD) 
        IF(I1.EQ.N+1.AND.(V(I1,1).LT.ZL.OR.V(I1,1).GT.ZU)) ISL(1)=1 
        IF(I1.EQ.N+2.AND.(V(I1,1).LT.ZL.OR.V(I1,1).GT.ZU)) ISL(2)=1 
        IF(KFLDA.EQ.21) V(I1,4)=LOG(ZU*(1.-ZL)/MAX(1E-20,ZL*(1.-ZU))) 
        IF(KFLDA.NE.21) V(I1,4)=LOG((1.-ZL)/MAX(1E-10,1.-ZU)) 
  450   CONTINUE 
        IF(ISL(1).EQ.1.AND.ISL(2).EQ.1.AND.ISLM.NE.0) THEN 
          ISL(3-ISLM)=0 
          ISLM=3-ISLM 
        ELSEIF(ISL(1).EQ.1.AND.ISL(2).EQ.1) THEN 
          ZDR1=MAX(0.,V(N+1,3)/MAX(1E-6,V(N+1,4))-1.) 
          ZDR2=MAX(0.,V(N+2,3)/MAX(1E-6,V(N+2,4))-1.) 
          IF(ZDR2.GT.RLU0(0)*(ZDR1+ZDR2)) ISL(1)=0 
          IF(ISL(1).EQ.1) ISL(2)=0 
          IF(ISL(1).EQ.0) ISLM=1 
          IF(ISL(2).EQ.0) ISLM=2 
        ENDIF 
        IF(ISL(1).EQ.1.OR.ISL(2).EQ.1) GOTO 330 
      ENDIF 
      IFLD1=KFLD(1)
      IF(KFLD(1).GE.6.AND.KFLD(1).LE.8) IFLD1=37+KFLD(1)+
     &ISIGN(2,K(N+1,2)) 
      IFLD2=KFLD(2)
      IF(KFLD(2).GE.6.AND.KFLD(2).LE.8) IFLD2=37+KFLD(2)+
     &ISIGN(2,K(N+2,2)) 
      IF(IGM.GT.0.AND.MOD(MSTJ(43),2).EQ.1.AND.(P(N+1,5).GE. 
     &PMTH(2,IFLD1).OR.P(N+2,5).GE.PMTH(2,IFLD2))) THEN 
        PMQ1=V(N+1,5)/V(IM,5) 
        PMQ2=V(N+2,5)/V(IM,5) 
        ZD=SQRT(MAX(0.,(1.-V(IM,5)/PEM**2)*((1.-PMQ1-PMQ2)**2- 
     &  4.*PMQ1*PMQ2))) 
        ZH=1.+PMQ1-PMQ2 
        ZL=0.5*(ZH-ZD) 
        ZU=0.5*(ZH+ZD) 
        IF(V(IM,1).LT.ZL.OR.V(IM,1).GT.ZU) GOTO 330 
      ENDIF 
 
C...Accepted branch. Construct four-momentum for initial partons. 
  460 MAZIP=0 
      MAZIC=0 
      IF(NEP.EQ.1) THEN 
        P(N+1,1)=0. 
        P(N+1,2)=0. 
        P(N+1,3)=SQRT(MAX(0.,(P(IPA(1),4)+P(N+1,5))*(P(IPA(1),4)- 
     &  P(N+1,5)))) 
        P(N+1,4)=P(IPA(1),4) 
        V(N+1,2)=P(N+1,4) 
      ELSEIF(IGM.EQ.0.AND.NEP.EQ.2) THEN 
        PED1=0.5*(V(IM,5)+V(N+1,5)-V(N+2,5))/P(IM,5) 
        P(N+1,1)=0. 
        P(N+1,2)=0. 
        P(N+1,3)=SQRT(MAX(0.,(PED1+P(N+1,5))*(PED1-P(N+1,5)))) 
        P(N+1,4)=PED1 
        P(N+2,1)=0. 
        P(N+2,2)=0. 
        P(N+2,3)=-P(N+1,3) 
        P(N+2,4)=P(IM,5)-PED1 
        V(N+1,2)=P(N+1,4) 
        V(N+2,2)=P(N+2,4) 
      ELSEIF(NEP.EQ.3) THEN 
        P(N+1,1)=0. 
        P(N+1,2)=0. 
        P(N+1,3)=SQRT(MAX(0.,PA1S)) 
        P(N+2,1)=SQRT(PTS) 
        P(N+2,2)=0. 
        P(N+2,3)=0.5*(PA3S-PA2S-PA1S)/P(N+1,3) 
        P(N+3,1)=-P(N+2,1) 
        P(N+3,2)=0. 
        P(N+3,3)=-(P(N+1,3)+P(N+2,3)) 
        V(N+1,2)=P(N+1,4) 
        V(N+2,2)=P(N+2,4) 
        V(N+3,2)=P(N+3,4) 
 
C...Construct transverse momentum for ordinary branching in shower. 
      ELSE 
        ZM=V(IM,1) 
        PZM=SQRT(MAX(0.,(PEM+P(IM,5))*(PEM-P(IM,5)))) 
        PMLS=(V(IM,5)-V(N+1,5)-V(N+2,5))**2-4.*V(N+1,5)*V(N+2,5) 
        IF(PZM.LE.0.) THEN 
          PTS=0. 
        ELSEIF(MOD(MSTJ(43),2).EQ.1) THEN 
          PTS=(PEM**2*(ZM*(1.-ZM)*V(IM,5)-(1.-ZM)*V(N+1,5)- 
     &    ZM*V(N+2,5))-0.25*PMLS)/PZM**2 
        ELSE 
          PTS=PMLS*(ZM*(1.-ZM)*PEM**2/V(IM,5)-0.25)/PZM**2 
        ENDIF 
        PT=SQRT(MAX(0.,PTS)) 
 
C...Find coefficient of azimuthal asymmetry due to gluon polarization. 
        HAZIP=0. 
        IF(MSTJ(49).NE.1.AND.MOD(MSTJ(46),2).EQ.1.AND.K(IM,2).EQ.21. 
     &  AND.IAU.NE.0) THEN 
          IF(K(IGM,3).NE.0) MAZIP=1 
          ZAU=V(IGM,1) 
          IF(IAU.EQ.IM+1) ZAU=1.-V(IGM,1) 
          IF(MAZIP.EQ.0) ZAU=0. 
          IF(K(IGM,2).NE.21) THEN 
            HAZIP=2.*ZAU/(1.+ZAU**2) 
          ELSE 
            HAZIP=(ZAU/(1.-ZAU*(1.-ZAU)))**2 
          ENDIF 
          IF(K(N+1,2).NE.21) THEN 
            HAZIP=HAZIP*(-2.*ZM*(1.-ZM))/(1.-2.*ZM*(1.-ZM)) 
          ELSE 
            HAZIP=HAZIP*(ZM*(1.-ZM)/(1.-ZM*(1.-ZM)))**2 
          ENDIF 
        ENDIF 
 
C...Find coefficient of azimuthal asymmetry due to soft gluon 
C...interference. 
        HAZIC=0. 
        IF(MSTJ(49).NE.2.AND.MSTJ(46).GE.2.AND.(K(N+1,2).EQ.21.OR. 
     &  K(N+2,2).EQ.21).AND.IAU.NE.0) THEN 
          IF(K(IGM,3).NE.0) MAZIC=N+1 
          IF(K(IGM,3).NE.0.AND.K(N+1,2).NE.21) MAZIC=N+2 
          IF(K(IGM,3).NE.0.AND.K(N+1,2).EQ.21.AND.K(N+2,2).EQ.21.AND. 
     &    ZM.GT.0.5) MAZIC=N+2 
          IF(K(IAU,2).EQ.22) MAZIC=0 
          ZS=ZM 
          IF(MAZIC.EQ.N+2) ZS=1.-ZM 
          ZGM=V(IGM,1) 
          IF(IAU.EQ.IM-1) ZGM=1.-V(IGM,1) 
          IF(MAZIC.EQ.0) ZGM=1. 
          IF(MAZIC.NE.0) HAZIC=(P(IM,5)/P(IGM,5))*
     &    SQRT((1.-ZS)*(1.-ZGM)/(ZS*ZGM)) 
          HAZIC=MIN(0.95,HAZIC) 
        ENDIF 
      ENDIF 
 
C...Construct kinematics for ordinary branching in shower. 
  470 IF(NEP.EQ.2.AND.IGM.GT.0) THEN 
        IF(MOD(MSTJ(43),2).EQ.1) THEN 
          P(N+1,4)=PEM*V(IM,1) 
        ELSE 
          P(N+1,4)=PEM*(0.5*(V(IM,5)-SQRT(PMLS)+V(N+1,5)-V(N+2,5))+ 
     &    SQRT(PMLS)*ZM)/V(IM,5) 
        ENDIF 
        PHI=PARU(2)*RLU0(0) 
        P(N+1,1)=PT*COS(PHI) 
        P(N+1,2)=PT*SIN(PHI) 
        IF(PZM.GT.0.) THEN 
          P(N+1,3)=0.5*(V(N+2,5)-V(N+1,5)-V(IM,5)+2.*PEM*P(N+1,4))/PZM 
        ELSE 
          P(N+1,3)=0. 
        ENDIF 
        P(N+2,1)=-P(N+1,1) 
        P(N+2,2)=-P(N+1,2) 
        P(N+2,3)=PZM-P(N+1,3) 
        P(N+2,4)=PEM-P(N+1,4) 
        IF(MSTJ(43).LE.2) THEN 
          V(N+1,2)=(PEM*P(N+1,4)-PZM*P(N+1,3))/P(IM,5) 
          V(N+2,2)=(PEM*P(N+2,4)-PZM*P(N+2,3))/P(IM,5) 
        ENDIF 
      ENDIF 
 
C...Rotate and boost daughters. 
      IF(IGM.GT.0) THEN 
        IF(MSTJ(43).LE.2) THEN 
          BEX=P(IGM,1)/P(IGM,4) 
          BEY=P(IGM,2)/P(IGM,4) 
          BEZ=P(IGM,3)/P(IGM,4) 
          GA=P(IGM,4)/P(IGM,5) 
          GABEP=GA*(GA*(BEX*P(IM,1)+BEY*P(IM,2)+BEZ*P(IM,3))/(1.+GA)- 
     &    P(IM,4)) 
        ELSE 
          BEX=0. 
          BEY=0. 
          BEZ=0. 
          GA=1. 
          GABEP=0. 
        ENDIF 
        THE=ULANGL0(P(IM,3)+GABEP*BEZ,SQRT((P(IM,1)+GABEP*BEX)**2+ 
     &  (P(IM,2)+GABEP*BEY)**2)) 
        PHI=ULANGL0(P(IM,1)+GABEP*BEX,P(IM,2)+GABEP*BEY) 
        DO 480 I=N+1,N+2 
        DP(1)=COS(THE)*COS(PHI)*P(I,1)-SIN(PHI)*P(I,2)+ 
     &  SIN(THE)*COS(PHI)*P(I,3) 
        DP(2)=COS(THE)*SIN(PHI)*P(I,1)+COS(PHI)*P(I,2)+ 
     &  SIN(THE)*SIN(PHI)*P(I,3) 
        DP(3)=-SIN(THE)*P(I,1)+COS(THE)*P(I,3) 
        DP(4)=P(I,4) 
        DBP=BEX*DP(1)+BEY*DP(2)+BEZ*DP(3) 
        DGABP=GA*(GA*DBP/(1D0+GA)+DP(4)) 
        P(I,1)=DP(1)+DGABP*BEX 
        P(I,2)=DP(2)+DGABP*BEY 
        P(I,3)=DP(3)+DGABP*BEZ 
        P(I,4)=GA*(DP(4)+DBP) 
  480   CONTINUE 
      ENDIF 
 
C...Weight with azimuthal distribution, if required. 
      IF(MAZIP.NE.0.OR.MAZIC.NE.0) THEN 
        DO 490 J=1,3 
        DPT(1,J)=P(IM,J) 
        DPT(2,J)=P(IAU,J) 
        DPT(3,J)=P(N+1,J) 
  490   CONTINUE 
        DPMA=DPT(1,1)*DPT(2,1)+DPT(1,2)*DPT(2,2)+DPT(1,3)*DPT(2,3) 
        DPMD=DPT(1,1)*DPT(3,1)+DPT(1,2)*DPT(3,2)+DPT(1,3)*DPT(3,3) 
        DPMM=DPT(1,1)**2+DPT(1,2)**2+DPT(1,3)**2 
        DO 500 J=1,3 
        DPT(4,J)=DPT(2,J)-DPMA*DPT(1,J)/DPMM 
        DPT(5,J)=DPT(3,J)-DPMD*DPT(1,J)/DPMM 
  500   CONTINUE 
        DPT(4,4)=SQRT(DPT(4,1)**2+DPT(4,2)**2+DPT(4,3)**2) 
        DPT(5,4)=SQRT(DPT(5,1)**2+DPT(5,2)**2+DPT(5,3)**2) 
        IF(MIN(DPT(4,4),DPT(5,4)).GT.0.1*PARJ(82)) THEN 
          CAD=(DPT(4,1)*DPT(5,1)+DPT(4,2)*DPT(5,2)+ 
     &    DPT(4,3)*DPT(5,3))/(DPT(4,4)*DPT(5,4)) 
          IF(MAZIP.NE.0) THEN 
            IF(1.+HAZIP*(2.*CAD**2-1.).LT.RLU0(0)*(1.+ABS(HAZIP))) 
     &      GOTO 470 
          ENDIF 
          IF(MAZIC.NE.0) THEN 
            IF(MAZIC.EQ.N+2) CAD=-CAD 
            IF((1.-HAZIC)*(1.-HAZIC*CAD)/(1.+HAZIC**2-2.*HAZIC*CAD) 
     &      .LT.RLU0(0)) GOTO 470 
          ENDIF 
        ENDIF 
      ENDIF 
 
C...Azimuthal anisotropy due to interference with initial state partons. 
      IF(MOD(MIIS,2).EQ.1.AND.IGM.EQ.NS+1.AND.(K(N+1,2).EQ.21.OR. 
     &K(N+2,2).EQ.21)) THEN 
        III=IM-NS-1 
        IF(ISII(III).GE.1) THEN 
          IAZIID=N+1 
          IF(K(N+1,2).NE.21) IAZIID=N+2 
          IF(K(N+1,2).EQ.21.AND.K(N+2,2).EQ.21.AND. 
     &    P(N+1,4).GT.P(N+2,4)) IAZIID=N+2 
          THEIID=
     &    ULANGL0(P(IAZIID,3),SQRT(P(IAZIID,1)**2+P(IAZIID,2)**2)) 
          IF(III.EQ.2) THEIID=PARU(1)-THEIID 
          PHIIID=ULANGL0(P(IAZIID,1),P(IAZIID,2)) 
          HAZII=MIN(0.95,THEIID/THEIIS(III,ISII(III))) 
          CAD=COS(PHIIID-PHIIIS(III,ISII(III))) 
          PHIREL=ABS(PHIIID-PHIIIS(III,ISII(III))) 
          IF(PHIREL.GT.PARU(1)) PHIREL=PARU(2)-PHIREL 
          IF((1.-HAZII)*(1.-HAZII*CAD)/(1.+HAZII**2-2.*HAZII*CAD) 
     &    .LT.RLU0(0)) GOTO 470 
        ENDIF 
      ENDIF 
 
C...Continue loop over partons that may branch, until none left. 
      IF(IGM.GE.0) K(IM,1)=14 
      N=N+NEP 
      NEP=2 
      IF(N.GT.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUSHOW0:) no more memory left in LUJETS') 
        IF(MSTU(21).GE.1) N=NS 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      GOTO 270 
 
C...Set information on imagined shower initiator. 
  510 IF(NPA.GE.2) THEN 
        K(NS+1,1)=11 
        K(NS+1,2)=94 
        K(NS+1,3)=IP1 
        IF(IP2.GT.0.AND.IP2.LT.IP1) K(NS+1,3)=IP2 
        K(NS+1,4)=NS+2 
        K(NS+1,5)=NS+1+NPA 
        IIM=1 
      ELSE 
        IIM=0 
      ENDIF 
 
C...Reconstruct string drawing information. 
      DO 520 I=NS+1+IIM,N 
      IF(K(I,1).LE.10.AND.K(I,2).EQ.22) THEN 
        K(I,1)=1 
      ELSEIF(K(I,1).LE.10.AND.IABS(K(I,2)).GE.11.AND. 
     &IABS(K(I,2)).LE.18) THEN 
        K(I,1)=1 
      ELSEIF(K(I,1).LE.10) THEN 
        K(I,4)=MSTU(5)*(K(I,4)/MSTU(5)) 
        K(I,5)=MSTU(5)*(K(I,5)/MSTU(5)) 
      ELSEIF(K(MOD(K(I,4),MSTU(5))+1,2).NE.22) THEN 
        ID1=MOD(K(I,4),MSTU(5)) 
        IF(K(I,2).GE.1.AND.K(I,2).LE.8) ID1=MOD(K(I,4),MSTU(5))+1 
        ID2=2*MOD(K(I,4),MSTU(5))+1-ID1 
        K(I,4)=MSTU(5)*(K(I,4)/MSTU(5))+ID1 
        K(I,5)=MSTU(5)*(K(I,5)/MSTU(5))+ID2 
        K(ID1,4)=K(ID1,4)+MSTU(5)*I 
        K(ID1,5)=K(ID1,5)+MSTU(5)*ID2 
        K(ID2,4)=K(ID2,4)+MSTU(5)*ID1 
        K(ID2,5)=K(ID2,5)+MSTU(5)*I 
      ELSE 
        ID1=MOD(K(I,4),MSTU(5)) 
        ID2=ID1+1 
        K(I,4)=MSTU(5)*(K(I,4)/MSTU(5))+ID1 
        K(I,5)=MSTU(5)*(K(I,5)/MSTU(5))+ID1 
        IF(IABS(K(I,2)).LE.10.OR.K(ID1,1).GE.11) THEN 
          K(ID1,4)=K(ID1,4)+MSTU(5)*I 
          K(ID1,5)=K(ID1,5)+MSTU(5)*I 
        ELSE 
          K(ID1,4)=0 
          K(ID1,5)=0 
        ENDIF 
        K(ID2,4)=0 
        K(ID2,5)=0 
      ENDIF 
  520 CONTINUE 
 
C...Transformation from CM frame. 
      IF(NPA.GE.2) THEN 
        BEX=PS(1)/PS(4) 
        BEY=PS(2)/PS(4) 
        BEZ=PS(3)/PS(4) 
        GA=PS(4)/PS(5) 
        GABEP=GA*(GA*(BEX*P(IPA(1),1)+BEY*P(IPA(1),2)+BEZ*P(IPA(1),3)) 
     &  /(1.+GA)-P(IPA(1),4)) 
      ELSE 
        BEX=0. 
        BEY=0. 
        BEZ=0. 
        GABEP=0. 
      ENDIF 
      THE=ULANGL0(P(IPA(1),3)+GABEP*BEZ,SQRT((P(IPA(1),1) 
     &+GABEP*BEX)**2+(P(IPA(1),2)+GABEP*BEY)**2)) 
      PHI=ULANGL0(P(IPA(1),1)+GABEP*BEX,P(IPA(1),2)+GABEP*BEY) 
      IF(NPA.EQ.3) THEN 
        CHI=ULANGL0(COS(THE)*COS(PHI)*(P(IPA(2),1)+GABEP*BEX)+COS(THE)* 
     &  SIN(PHI)*(P(IPA(2),2)+GABEP*BEY)-SIN(THE)*(P(IPA(2),3)+GABEP* 
     &  BEZ),-SIN(PHI)*(P(IPA(2),1)+GABEP*BEX)+COS(PHI)*(P(IPA(2),2)+ 
     &  GABEP*BEY)) 
        MSTU(33)=1 
        CALL LUDBRB0(NS+1,N,0.,CHI,0D0,0D0,0D0) 
      ENDIF 
      DBEX=DBLE(BEX) 
      DBEY=DBLE(BEY) 
      DBEZ=DBLE(BEZ) 
      MSTU(33)=1 
      CALL LUDBRB0(NS+1,N,THE,PHI,DBEX,DBEY,DBEZ) 
 
C...Decay vertex of shower. 
      DO 540 I=NS+1,N 
      DO 530 J=1,5 
      V(I,J)=V(IP1,J) 
  530 CONTINUE 
  540 CONTINUE 
 
C...Delete trivial shower, else connect initiators. 
      IF(N.EQ.NS+NPA+IIM) THEN 
        N=NS 
      ELSE 
        DO 550 IP=1,NPA 
        K(IPA(IP),1)=14 
        K(IPA(IP),4)=K(IPA(IP),4)+NS+IIM+IP 
        K(IPA(IP),5)=K(IPA(IP),5)+NS+IIM+IP 
        K(NS+IIM+IP,3)=IPA(IP) 
        IF(IIM.EQ.1.AND.MSTU(16).NE.2) K(NS+IIM+IP,3)=NS+1 
        IF(K(NS+IIM+IP,1).NE.1) THEN 
          K(NS+IIM+IP,4)=MSTU(5)*IPA(IP)+K(NS+IIM+IP,4) 
          K(NS+IIM+IP,5)=MSTU(5)*IPA(IP)+K(NS+IIM+IP,5) 
        ENDIF 
  550   CONTINUE 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUBOEI0(NSAV) 
 
C...Purpose: to modify event so as to approximately take into account 
C...Bose-Einstein effects according to a simple phenomenological 
C...parametrization. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUJETS/,/LUDAT01/ 
      DIMENSION DPS(4),KFBE(9),NBE(0:9),BEI(100) 
      DATA KFBE/211,-211,111,321,-321,130,310,221,331/ 
 
C...Boost event to overall CM frame. Calculate CM energy. 
      IF((MSTJ(51).NE.1.AND.MSTJ(51).NE.2).OR.N-NSAV.LE.1) RETURN 
      DO 100 J=1,4 
      DPS(J)=0. 
  100 CONTINUE 
      DO 120 I=1,N 
      KFA=IABS(K(I,2))
      IF(K(I,1).LE.10.AND.((KFA.GT.10.AND.KFA.LE.20).OR.KFA.EQ.22).AND.
     &K(I,3).GT.0) THEN
        KFMA=IABS(K(K(I,3),2))
        IF(KFMA.GT.10.AND.KFMA.LE.80) K(I,1)=-K(I,1)
      ELSEIF(KFA.EQ.22.AND.K(I,3).EQ.0) THEN
        K(I,1)=-K(I,1)
      ENDIF
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 120 
      DO 110 J=1,4 
      DPS(J)=DPS(J)+P(I,J) 
  110 CONTINUE 
  120 CONTINUE 
      CALL LUDBRB0(0,0,0.,0.,-DPS(1)/DPS(4),-DPS(2)/DPS(4), 
     &-DPS(3)/DPS(4)) 
      PECM=0. 
      DO 130 I=1,N 
      IF(K(I,1).GE.1.AND.K(I,1).LE.10) PECM=PECM+P(I,4) 
  130 CONTINUE 
 
C...Reserve copy of particles by species at end of record. 
      NBE(0)=N+MSTU(3) 
      DO 160 IBE=1,MIN(9,MSTJ(52)) 
      NBE(IBE)=NBE(IBE-1) 
      DO 150 I=NSAV+1,N 
      IF(K(I,2).NE.KFBE(IBE)) GOTO 150 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 150 
      IF(NBE(IBE).GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUBOEI0:) no more memory left in LUJETS') 
        RETURN 
      ENDIF 
      NBE(IBE)=NBE(IBE)+1 
      K(NBE(IBE),1)=I 
      DO 140 J=1,3 
      P(NBE(IBE),J)=0. 
  140 CONTINUE 
  150 CONTINUE 
  160 CONTINUE 
      IF(NBE(MIN(9,MSTJ(52)))-NBE(0).LE.1) GOTO 280
 
C...Tabulate integral for subsequent momentum shift. 
      DO 220 IBE=1,MIN(9,MSTJ(52)) 
      IF(IBE.NE.1.AND.IBE.NE.4.AND.IBE.LE.7) GOTO 180 
      IF(IBE.EQ.1.AND.MAX(NBE(1)-NBE(0),NBE(2)-NBE(1),NBE(3)-NBE(2)) 
     &.LE.1) GOTO 180 
      IF(IBE.EQ.4.AND.MAX(NBE(4)-NBE(3),NBE(5)-NBE(4),NBE(6)-NBE(5), 
     &NBE(7)-NBE(6)).LE.1) GOTO 180 
      IF(IBE.GE.8.AND.NBE(IBE)-NBE(IBE-1).LE.1) GOTO 180 
      IF(IBE.EQ.1) PMHQ=2.*ULMASS0(211) 
      IF(IBE.EQ.4) PMHQ=2.*ULMASS0(321) 
      IF(IBE.EQ.8) PMHQ=2.*ULMASS0(221) 
      IF(IBE.EQ.9) PMHQ=2.*ULMASS0(331) 
      QDEL=0.1*MIN(PMHQ,PARJ(93)) 
      IF(MSTJ(51).EQ.1) THEN 
        NBIN=MIN(100,NINT(9.*PARJ(93)/QDEL)) 
        BEEX=EXP(0.5*QDEL/PARJ(93)) 
        BERT=EXP(-QDEL/PARJ(93)) 
      ELSE 
        NBIN=MIN(100,NINT(3.*PARJ(93)/QDEL)) 
      ENDIF 
      DO 170 IBIN=1,NBIN 
      QBIN=QDEL*(IBIN-0.5) 
      BEI(IBIN)=QDEL*(QBIN**2+QDEL**2/12.)/SQRT(QBIN**2+PMHQ**2) 
      IF(MSTJ(51).EQ.1) THEN 
        BEEX=BEEX*BERT 
        BEI(IBIN)=BEI(IBIN)*BEEX 
      ELSE 
        BEI(IBIN)=BEI(IBIN)*EXP(-(QBIN/PARJ(93))**2) 
      ENDIF 
      IF(IBIN.GE.2) BEI(IBIN)=BEI(IBIN)+BEI(IBIN-1) 
  170 CONTINUE 
 
C...Loop through particle pairs and find old relative momentum. 
  180 DO 210 I1M=NBE(IBE-1)+1,NBE(IBE)-1 
      I1=K(I1M,1) 
      DO 200 I2M=I1M+1,NBE(IBE) 
      I2=K(I2M,1) 
      Q2OLD=MAX(0.,(P(I1,4)+P(I2,4))**2-(P(I1,1)+P(I2,1))**2-(P(I1,2)+ 
     &P(I2,2))**2-(P(I1,3)+P(I2,3))**2-(P(I1,5)+P(I2,5))**2) 
      QOLD=SQRT(Q2OLD) 
 
C...Calculate new relative momentum. 
      IF(QOLD.LT.1E-3*QDEL) THEN 
        GOTO 200 
      ELSEIF(QOLD.LE.QDEL) THEN 
        QMOV=QOLD/3. 
      ELSEIF(QOLD.LT.(NBIN-0.1)*QDEL) THEN 
        RBIN=QOLD/QDEL 
        IBIN=RBIN 
        RINP=(RBIN**3-IBIN**3)/(3*IBIN*(IBIN+1)+1) 
        QMOV=(BEI(IBIN)+RINP*(BEI(IBIN+1)-BEI(IBIN)))* 
     &  SQRT(Q2OLD+PMHQ**2)/Q2OLD 
      ELSE 
        QMOV=BEI(NBIN)*SQRT(Q2OLD+PMHQ**2)/Q2OLD 
      ENDIF 
      Q2NEW=Q2OLD*(QOLD/(QOLD+3.*PARJ(92)*QMOV))**(2./3.) 
 
C...Calculate and save shift to be performed on three-momenta. 
      HC1=(P(I1,4)+P(I2,4))**2-(Q2OLD-Q2NEW) 
      HC2=(Q2OLD-Q2NEW)*(P(I1,4)-P(I2,4))**2 
      HA=0.5*(1.-SQRT(HC1*Q2NEW/(HC1*Q2OLD-HC2))) 
      DO 190 J=1,3 
      PD=HA*(P(I2,J)-P(I1,J)) 
      P(I1M,J)=P(I1M,J)+PD 
      P(I2M,J)=P(I2M,J)-PD 
  190 CONTINUE 
  200 CONTINUE 
  210 CONTINUE 
  220 CONTINUE 
 
C...Shift momenta and recalculate energies. 
      DO 240 IM=NBE(0)+1,NBE(MIN(9,MSTJ(52))) 
      I=K(IM,1) 
      DO 230 J=1,3 
      P(I,J)=P(I,J)+P(IM,J) 
  230 CONTINUE 
      P(I,4)=SQRT(P(I,5)**2+P(I,1)**2+P(I,2)**2+P(I,3)**2) 
  240 CONTINUE 
 
C...Rescale all momenta for energy conservation. 
      PES=0. 
      PQS=0. 
      DO 250 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 250 
      PES=PES+P(I,4) 
      PQS=PQS+P(I,5)**2/P(I,4) 
  250 CONTINUE 
      FAC=(PECM-PQS)/(PES-PQS) 
      DO 270 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 270 
      DO 260 J=1,3 
      P(I,J)=FAC*P(I,J) 
  260 CONTINUE 
      P(I,4)=SQRT(P(I,5)**2+P(I,1)**2+P(I,2)**2+P(I,3)**2) 
  270 CONTINUE 
 
C...Boost back to correct reference frame. 
  280 CALL LUDBRB0(0,0,0.,0.,DPS(1)/DPS(4),DPS(2)/DPS(4),DPS(3)/DPS(4)) 
      DO 290 I=1,N
      IF(K(I,1).LT.0) K(I,1)=-K(I,1)
  290 CONTINUE
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION ULMASS0(KF) 
 
C...Purpose: to give the mass of a particle/parton. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUDAT01/,/LUDAT02/ 
 
C...Reset variables. Compressed code. 
      ULMASS0=0. 
      KFA=IABS(KF) 
      KC=LUCOMP0(KF) 
      IF(KC.EQ.0) RETURN 
      PARF(106)=PMAS(6,1) 
      PARF(107)=PMAS(7,1) 
      PARF(108)=PMAS(8,1) 
 
C...Guarantee use of constituent masses for internal checks. 
      IF((MSTJ(93).EQ.1.OR.MSTJ(93).EQ.2).AND.KFA.LE.10) THEN 
        ULMASS0=PARF(100+KFA) 
        IF(MSTJ(93).EQ.2) ULMASS0=MAX(0.,ULMASS0-PARF(121)) 
 
C...Masses that can be read directly off table. 
      ELSEIF(KFA.LE.100.OR.KC.LE.80.OR.KC.GT.100) THEN 
        ULMASS0=PMAS(KC,1) 
 
C...Find constituent partons and their masses. 
      ELSE 
        KFLA=MOD(KFA/1000,10) 
        KFLB=MOD(KFA/100,10) 
        KFLC=MOD(KFA/10,10) 
        KFLS=MOD(KFA,10) 
        KFLR=MOD(KFA/10000,10) 
        PMA=PARF(100+KFLA) 
        PMB=PARF(100+KFLB) 
        PMC=PARF(100+KFLC) 
 
C...Construct masses for various meson, diquark and baryon cases. 
        IF(KFLA.EQ.0.AND.KFLR.EQ.0.AND.KFLS.LE.3) THEN 
          IF(KFLS.EQ.1) PMSPL=-3./(PMB*PMC) 
          IF(KFLS.GE.3) PMSPL=1./(PMB*PMC) 
          ULMASS0=PARF(111)+PMB+PMC+PARF(113)*PARF(101)**2*PMSPL 
        ELSEIF(KFLA.EQ.0) THEN 
          KMUL=2 
          IF(KFLS.EQ.1) KMUL=3 
          IF(KFLR.EQ.2) KMUL=4 
          IF(KFLS.EQ.5) KMUL=5 
          ULMASS0=PARF(113+KMUL)+PMB+PMC 
        ELSEIF(KFLC.EQ.0) THEN 
          IF(KFLS.EQ.1) PMSPL=-3./(PMA*PMB) 
          IF(KFLS.EQ.3) PMSPL=1./(PMA*PMB) 
          ULMASS0=2.*PARF(112)/3.+PMA+PMB+PARF(114)*PARF(101)**2*PMSPL 
          IF(MSTJ(93).EQ.1) ULMASS0=PMA+PMB 
          IF(MSTJ(93).EQ.2) ULMASS0=MAX(0.,ULMASS0-PARF(122)- 
     &    2.*PARF(112)/3.) 
        ELSE 
          IF(KFLS.EQ.2.AND.KFLA.EQ.KFLB) THEN 
            PMSPL=1./(PMA*PMB)-2./(PMA*PMC)-2./(PMB*PMC) 
          ELSEIF(KFLS.EQ.2.AND.KFLB.GE.KFLC) THEN 
            PMSPL=-2./(PMA*PMB)-2./(PMA*PMC)+1./(PMB*PMC) 
          ELSEIF(KFLS.EQ.2) THEN 
            PMSPL=-3./(PMB*PMC) 
          ELSE 
            PMSPL=1./(PMA*PMB)+1./(PMA*PMC)+1./(PMB*PMC) 
          ENDIF 
          ULMASS0=PARF(112)+PMA+PMB+PMC+PARF(114)*PARF(101)**2*PMSPL 
        ENDIF 
      ENDIF 
 
C...Optional mass broadening according to truncated Breit-Wigner 
C...(either in m or in m^2). 
      IF(MSTJ(24).GE.1.AND.PMAS(KC,2).GT.1E-4) THEN 
        IF(MSTJ(24).EQ.1.OR.(MSTJ(24).EQ.2.AND.KFA.GT.100)) THEN 
          ULMASS0=ULMASS0+0.5*PMAS(KC,2)*TAN((2.*RLU0(0)-1.)* 
     &    ATAN(2.*PMAS(KC,3)/PMAS(KC,2))) 
        ELSE 
          PM0=ULMASS0 
          PMLOW=ATAN((MAX(0.,PM0-PMAS(KC,3))**2-PM0**2)/ 
     &    (PM0*PMAS(KC,2))) 
          PMUPP=ATAN(((PM0+PMAS(KC,3))**2-PM0**2)/(PM0*PMAS(KC,2))) 
          ULMASS0=SQRT(MAX(0.,PM0**2+PM0*PMAS(KC,2)*TAN(PMLOW+ 
     &    (PMUPP-PMLOW)*RLU0(0)))) 
        ENDIF 
      ENDIF 
      MSTJ(93)=0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUNAME0(KF,CHAU) 
 
C...Purpose: to give the particle/parton name as a character string. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT04/CHAF(500) 
      CHARACTER CHAF*8 
      SAVE /LUDAT01/,/LUDAT02/,/LUDAT04/ 
      CHARACTER CHAU*16 
 
C...Initial values. Charge. Subdivide code. 
      CHAU=' ' 
      KFA=IABS(KF) 
      KC=LUCOMP0(KF) 
      IF(KC.EQ.0) RETURN 
      KQ=LUCHGE0(KF) 
      KFLA=MOD(KFA/1000,10) 
      KFLB=MOD(KFA/100,10) 
      KFLC=MOD(KFA/10,10) 
      KFLS=MOD(KFA,10) 
      KFLR=MOD(KFA/10000,10) 
 
C...Read out root name and spin for simple particle. 
      IF(KFA.LE.100.OR.(KFA.GT.100.AND.KC.GT.100)) THEN 
        CHAU=CHAF(KC) 
        LEN=0 
        DO 100 LEM=1,8 
        IF(CHAU(LEM:LEM).NE.' ') LEN=LEM 
  100   CONTINUE 
 
C...Construct root name for diquark. Add on spin. 
      ELSEIF(KFLC.EQ.0) THEN 
        CHAU(1:2)=CHAF(KFLA)(1:1)//CHAF(KFLB)(1:1) 
        IF(KFLS.EQ.1) CHAU(3:4)='_0' 
        IF(KFLS.EQ.3) CHAU(3:4)='_1' 
        LEN=4 
 
C...Construct root name for heavy meson. Add on spin and heavy flavour. 
      ELSEIF(KFLA.EQ.0) THEN 
        IF(KFLB.EQ.5) CHAU(1:1)='B' 
        IF(KFLB.EQ.6) CHAU(1:1)='T' 
        IF(KFLB.EQ.7) CHAU(1:1)='L' 
        IF(KFLB.EQ.8) CHAU(1:1)='H' 
        LEN=1 
        IF(KFLR.EQ.0.AND.KFLS.EQ.1) THEN 
        ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.3) THEN 
          CHAU(2:2)='*' 
          LEN=2 
        ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.3) THEN 
          CHAU(2:3)='_1' 
          LEN=3 
        ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.1) THEN 
          CHAU(2:4)='*_0' 
          LEN=4 
        ELSEIF(KFLR.EQ.2) THEN 
          CHAU(2:4)='*_1' 
          LEN=4 
        ELSEIF(KFLS.EQ.5) THEN 
          CHAU(2:4)='*_2' 
          LEN=4 
        ENDIF 
        IF(KFLC.GE.3.AND.KFLR.EQ.0.AND.KFLS.LE.3) THEN 
          CHAU(LEN+1:LEN+2)='_'//CHAF(KFLC)(1:1) 
          LEN=LEN+2 
        ELSEIF(KFLC.GE.3) THEN 
          CHAU(LEN+1:LEN+1)=CHAF(KFLC)(1:1) 
          LEN=LEN+1 
        ENDIF 
 
C...Construct root name and spin for heavy baryon. 
      ELSE 
        IF(KFLB.LE.2.AND.KFLC.LE.2) THEN 
          CHAU='Sigma ' 
          IF(KFLC.GT.KFLB) CHAU='Lambda' 
          IF(KFLS.EQ.4) CHAU='Sigma*' 
          LEN=5 
          IF(CHAU(6:6).NE.' ') LEN=6 
        ELSEIF(KFLB.LE.2.OR.KFLC.LE.2) THEN 
          CHAU='Xi ' 
          IF(KFLA.GT.KFLB.AND.KFLB.GT.KFLC) CHAU='Xi''' 
          IF(KFLS.EQ.4) CHAU='Xi*' 
          LEN=2 
          IF(CHAU(3:3).NE.' ') LEN=3 
        ELSE 
          CHAU='Omega ' 
          IF(KFLA.GT.KFLB.AND.KFLB.GT.KFLC) CHAU='Omega''' 
          IF(KFLS.EQ.4) CHAU='Omega*' 
          LEN=5 
          IF(CHAU(6:6).NE.' ') LEN=6 
        ENDIF 
 
C...Add on heavy flavour content for heavy baryon. 
        CHAU(LEN+1:LEN+2)='_'//CHAF(KFLA)(1:1) 
        LEN=LEN+2 
        IF(KFLB.GE.KFLC.AND.KFLC.GE.4) THEN 
          CHAU(LEN+1:LEN+2)=CHAF(KFLB)(1:1)//CHAF(KFLC)(1:1) 
          LEN=LEN+2 
        ELSEIF(KFLB.GE.KFLC.AND.KFLB.GE.4) THEN 
          CHAU(LEN+1:LEN+1)=CHAF(KFLB)(1:1) 
          LEN=LEN+1 
        ELSEIF(KFLC.GT.KFLB.AND.KFLB.GE.4) THEN 
          CHAU(LEN+1:LEN+2)=CHAF(KFLC)(1:1)//CHAF(KFLB)(1:1) 
          LEN=LEN+2 
        ELSEIF(KFLC.GT.KFLB.AND.KFLC.GE.4) THEN 
          CHAU(LEN+1:LEN+1)=CHAF(KFLC)(1:1) 
          LEN=LEN+1 
        ENDIF 
      ENDIF 
 
C...Add on bar sign for antiparticle (where necessary). 
      IF(KF.GT.0.OR.LEN.EQ.0) THEN 
      ELSEIF(KFA.GT.10.AND.KFA.LE.40.AND.KQ.NE.0.AND.MOD(KQ,3).EQ.0) 
     &THEN 
      ELSEIF(KFA.EQ.89.OR.(KFA.GE.91.AND.KFA.LE.99)) THEN 
      ELSEIF(KFA.GT.100.AND.KFLA.EQ.0.AND.KQ.NE.0) THEN 
      ELSEIF(MSTU(15).LE.1) THEN 
        CHAU(LEN+1:LEN+1)='~' 
        LEN=LEN+1 
      ELSE 
        CHAU(LEN+1:LEN+3)='bar' 
        LEN=LEN+3 
      ENDIF 
 
C...Add on charge where applicable (conventional cases skipped). 
      IF(KQ.EQ.6) CHAU(LEN+1:LEN+2)='++' 
      IF(KQ.EQ.-6) CHAU(LEN+1:LEN+2)='--' 
      IF(KQ.EQ.3) CHAU(LEN+1:LEN+1)='+' 
      IF(KQ.EQ.-3) CHAU(LEN+1:LEN+1)='-' 
      IF(KQ.EQ.0.AND.(KFA.LE.22.OR.LEN.EQ.0)) THEN 
      ELSEIF(KQ.EQ.0.AND.(KFA.GE.81.AND.KFA.LE.100)) THEN 
      ELSEIF(KFA.EQ.28.OR.KFA.EQ.29) THEN 
      ELSEIF(KFA.GT.100.AND.KFLA.EQ.0.AND.KFLB.EQ.KFLC.AND. 
     &KFLB.NE.1) THEN 
      ELSEIF(KQ.EQ.0) THEN 
        CHAU(LEN+1:LEN+1)='0' 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION LUCHGE0(KF) 
 
C...Purpose: to give three times the charge for a particle/parton. 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUDAT02/ 
 
C...Initial values. Simple case of direct readout. 
      LUCHGE0=0 
      KFA=IABS(KF) 
      KC=LUCOMP0(KFA) 
      IF(KC.EQ.0) THEN 
      ELSEIF(KFA.LE.100.OR.KC.LE.80.OR.KC.GT.100) THEN 
        LUCHGE0=KCHG(KC,1) 
 
C...Construction from quark content for heavy meson, diquark, baryon. 
      ELSEIF(MOD(KFA/1000,10).EQ.0) THEN 
        LUCHGE0=(KCHG(MOD(KFA/100,10),1)-KCHG(MOD(KFA/10,10),1))* 
     &  (-1)**MOD(KFA/100,10) 
      ELSEIF(MOD(KFA/10,10).EQ.0) THEN 
        LUCHGE0=KCHG(MOD(KFA/1000,10),1)+KCHG(MOD(KFA/100,10),1) 
      ELSE 
        LUCHGE0=KCHG(MOD(KFA/1000,10),1)+KCHG(MOD(KFA/100,10),1)+ 
     &  KCHG(MOD(KFA/10,10),1) 
      ENDIF 
 
C...Add on correct sign. 
      LUCHGE0=LUCHGE0*ISIGN(1,KF) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION LUCOMP0(KF) 
 
C...Purpose: to compress the standard KF codes for use in mass and decay 
C...arrays; also to check whether a given code actually is defined. 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUDAT02/ 
      DIMENSION KFTAB(25),KCTAB(25) 
      DATA KFTAB/211,111,221,311,321,130,310,213,113,223, 
     &313,323,2112,2212,210,2110,2210,110,220,330,440,30443,30553,0,0/ 
      DATA KCTAB/101,111,112,102,103,221,222,121,131,132, 
     &122,123,332,333,281,282,283,284,285,286,287,231,235,0,0/ 
 
C...Starting values. 
      LUCOMP0=0 
      KFA=IABS(KF) 
 
C...Simple cases: direct translation or table. 
      IF(KFA.EQ.0.OR.KFA.GE.100000) THEN 
        RETURN 
      ELSEIF(KFA.LE.100) THEN 
        LUCOMP0=KFA 
        IF(KF.LT.0.AND.KCHG(KFA,3).EQ.0) LUCOMP0=0 
        RETURN 
      ELSE 
        DO 100 IKF=1,23 
        IF(KFA.EQ.KFTAB(IKF)) THEN 
          LUCOMP0=KCTAB(IKF) 
          IF(KF.LT.0.AND.KCHG(LUCOMP0,3).EQ.0) LUCOMP0=0 
          RETURN 
        ENDIF 
  100   CONTINUE 
      ENDIF 
 
C...Subdivide KF code into constituent pieces. 
      KFLA=MOD(KFA/1000,10) 
      KFLB=MOD(KFA/100,10) 
      KFLC=MOD(KFA/10,10) 
      KFLS=MOD(KFA,10) 
      KFLR=MOD(KFA/10000,10) 
 
C...Mesons. 
      IF(KFA-10000*KFLR.LT.1000) THEN 
        IF(KFLB.EQ.0.OR.KFLB.EQ.9.OR.KFLC.EQ.0.OR.KFLC.EQ.9) THEN 
        ELSEIF(KFLB.LT.KFLC) THEN 
        ELSEIF(KF.LT.0.AND.KFLB.EQ.KFLC) THEN 
        ELSEIF(KFLB.EQ.KFLC) THEN 
          IF(KFLR.EQ.0.AND.KFLS.EQ.1) THEN 
            LUCOMP0=110+KFLB 
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.3) THEN 
            LUCOMP0=130+KFLB 
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.3) THEN 
            LUCOMP0=150+KFLB 
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.1) THEN 
            LUCOMP0=170+KFLB 
          ELSEIF(KFLR.EQ.2.AND.KFLS.EQ.3) THEN 
            LUCOMP0=190+KFLB 
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.5) THEN 
            LUCOMP0=210+KFLB 
          ENDIF 
        ELSEIF(KFLB.LE.5) THEN 
          IF(KFLR.EQ.0.AND.KFLS.EQ.1) THEN 
            LUCOMP0=100+((KFLB-1)*(KFLB-2))/2+KFLC 
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.3) THEN 
            LUCOMP0=120+((KFLB-1)*(KFLB-2))/2+KFLC 
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.3) THEN 
            LUCOMP0=140+((KFLB-1)*(KFLB-2))/2+KFLC 
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.1) THEN 
            LUCOMP0=160+((KFLB-1)*(KFLB-2))/2+KFLC 
          ELSEIF(KFLR.EQ.2.AND.KFLS.EQ.3) THEN 
            LUCOMP0=180+((KFLB-1)*(KFLB-2))/2+KFLC 
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.5) THEN 
            LUCOMP0=200+((KFLB-1)*(KFLB-2))/2+KFLC 
          ENDIF 
        ELSEIF((KFLS.EQ.1.AND.KFLR.LE.1).OR.(KFLS.EQ.3.AND.KFLR.LE.2) 
     &  .OR.(KFLS.EQ.5.AND.KFLR.EQ.0)) THEN 
          LUCOMP0=80+KFLB 
        ENDIF 
 
C...Diquarks. 
      ELSEIF((KFLR.EQ.0.OR.KFLR.EQ.1).AND.KFLC.EQ.0) THEN 
        IF(KFLS.NE.1.AND.KFLS.NE.3) THEN 
        ELSEIF(KFLA.EQ.9.OR.KFLB.EQ.0.OR.KFLB.EQ.9) THEN 
        ELSEIF(KFLA.LT.KFLB) THEN 
        ELSEIF(KFLS.EQ.1.AND.KFLA.EQ.KFLB) THEN 
        ELSE 
          LUCOMP0=90 
        ENDIF 
 
C...Spin 1/2 baryons. 
      ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.2) THEN 
        IF(KFLA.EQ.9.OR.KFLB.EQ.0.OR.KFLB.EQ.9.OR.KFLC.EQ.9) THEN 
        ELSEIF(KFLA.LE.KFLC.OR.KFLA.LT.KFLB) THEN 
        ELSEIF(KFLA.GE.6.OR.KFLB.GE.4.OR.KFLC.GE.4) THEN 
          LUCOMP0=80+KFLA 
        ELSEIF(KFLB.LT.KFLC) THEN 
          LUCOMP0=300+((KFLA+1)*KFLA*(KFLA-1))/6+(KFLC*(KFLC-1))/2+KFLB 
        ELSE 
          LUCOMP0=330+((KFLA+1)*KFLA*(KFLA-1))/6+(KFLB*(KFLB-1))/2+KFLC 
        ENDIF 
 
C...Spin 3/2 baryons. 
      ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.4) THEN 
        IF(KFLA.EQ.9.OR.KFLB.EQ.0.OR.KFLB.EQ.9.OR.KFLC.EQ.9) THEN 
        ELSEIF(KFLA.LT.KFLB.OR.KFLB.LT.KFLC) THEN 
        ELSEIF(KFLA.GE.6.OR.KFLB.GE.4) THEN 
          LUCOMP0=80+KFLA 
        ELSE 
          LUCOMP0=360+((KFLA+1)*KFLA*(KFLA-1))/6+(KFLB*(KFLB-1))/2+KFLC 
        ENDIF 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUERRM0(MERR,CHMESS) 
 
C...Purpose: to inform user of errors in program execution. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/CHECKTAG/DECAYTAG !CHECK THE DECAY 
      real*8 DECAYTAG
      SAVE /LUJETS/,/LUDAT01/ 
      CHARACTER CHMESS*(*) 
      DECAYTAG=0
C...Write first few warnings, then be silent. 
      IF(MERR.LE.10) THEN 
        MSTU(27)=MSTU(27)+1 
        MSTU(28)=MERR 
        IF(MSTU(25).EQ.1.AND.MSTU(27).LE.MSTU(26)) WRITE(MSTU(11),5000) 
     &  MERR,MSTU(31),CHMESS 
 
C...Write first few errors, then be silent or stop program. 
      ELSEIF(MERR.LE.20) THEN 
        MSTU(23)=MSTU(23)+1 
        MSTU(24)=MERR-10 
        IF(MSTU(21).GE.1.AND.MSTU(23).LE.MSTU(22)) WRITE(MSTU(11),5100) 
     &  MERR-10,MSTU(31),CHMESS 
        IF(MSTU(21).GE.2.AND.MSTU(23).GT.MSTU(22)) THEN 
          WRITE(MSTU(11),5100) MERR-10,MSTU(31),CHMESS 
          WRITE(MSTU(11),5200) 
          IF(MERR.NE.17) CALL LULIST0(2) 
          DECAYTAG=1
c          STOP 
          print*,'This event will be thrown away'
          return !pingrgn
        ENDIF 
 
C...Stop program in case of irreparable error. 
      ELSE 
        WRITE(MSTU(11),5300) MERR-20,MSTU(31),CHMESS 
        DECAYTAG=1
c        STOP 
        return
      ENDIF 
 
C...Formats for output. 
 5000 FORMAT(/5X,'Advisory warning type',I2,' given after',I6, 
     &' LUEXEC0 calls:'/5X,A) 
 5100 FORMAT(/5X,'Error type',I2,' has occured after',I6, 
     &' LUEXEC0 calls:'/5X,A) 
 5200 FORMAT(5X,'Execution will be stopped after listing of last ', 
     &'event!') 
 5300 FORMAT(/5X,'Fatal error type',I2,' has occured after',I6, 
     &' LUEXEC0 calls:'/5X,A/5X,'Execution will now be stopped!') 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION ULALEM0(Q2) 
 
C...Purpose: to calculate the running alpha_electromagnetic. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT01/ 
 
C...Calculate real part of photon vacuum polarization. 
C...For leptons simplify by using asymptotic (Q^2 >> m^2) expressions. 
C...For hadrons use parametrization of H. Burkhardt et al. 
C...See R. Kleiss et al, CERN 89-08, vol. 3, pp. 129-131. 
      AEMPI=PARU(101)/(3.*PARU(1)) 
      IF(MSTU(101).LE.0.OR.Q2.LT.2E-6) THEN 
        RPIGG=0. 
      ELSEIF(MSTU(101).EQ.2.AND.Q2.LT.PARU(104)) THEN
        RPIGG=0.
      ELSEIF(MSTU(101).EQ.2) THEN
        RPIGG=1.-PARU(101)/PARU(103) 
      ELSEIF(Q2.LT.0.09) THEN 
        RPIGG=AEMPI*(13.4916+LOG(Q2))+0.00835*LOG(1.+Q2) 
      ELSEIF(Q2.LT.9.) THEN 
        RPIGG=AEMPI*(16.3200+2.*LOG(Q2))+0.00238*LOG(1.+3.927*Q2) 
      ELSEIF(Q2.LT.1E4) THEN 
        RPIGG=AEMPI*(13.4955+3.*LOG(Q2))+0.00165+0.00299*LOG(1.+Q2) 
      ELSE 
        RPIGG=AEMPI*(13.4955+3.*LOG(Q2))+0.00221+0.00293*LOG(1.+Q2) 
      ENDIF 
 
C...Calculate running alpha_em. 
      ULALEM0=PARU(101)/(1.-RPIGG) 
      PARU(108)=ULALEM0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION ULALPS0(Q2) 
 
C...Purpose: to give the value of alpha_strong. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUDAT01/,/LUDAT02/ 
 
C...Constant alpha_strong trivial. 
      IF(MSTU(111).LE.0) THEN 
        ULALPS0=PARU(111) 
        MSTU(118)=MSTU(112) 
        PARU(117)=0. 
        PARU(118)=PARU(111) 
        RETURN 
      ENDIF 
 
C...Find effective Q2, number of flavours and Lambda. 
      Q2EFF=Q2 
      IF(MSTU(115).GE.2) Q2EFF=MAX(Q2,PARU(114)) 
      NF=MSTU(112) 
      ALAM2=PARU(112)**2 
  100 IF(NF.GT.MAX(2,MSTU(113))) THEN 
        Q2THR=PARU(113)*PMAS(NF,1)**2 
        IF(Q2EFF.LT.Q2THR) THEN 
          NF=NF-1 
          ALAM2=ALAM2*(Q2THR/ALAM2)**(2./(33.-2.*NF)) 
          GOTO 100 
        ENDIF 
      ENDIF 
  110 IF(NF.LT.MIN(8,MSTU(114))) THEN 
        Q2THR=PARU(113)*PMAS(NF+1,1)**2 
        IF(Q2EFF.GT.Q2THR) THEN 
          NF=NF+1 
          ALAM2=ALAM2*(ALAM2/Q2THR)**(2./(33.-2.*NF)) 
          GOTO 110 
        ENDIF 
      ENDIF 
      IF(MSTU(115).EQ.1) Q2EFF=Q2EFF+ALAM2 
      PARU(117)=SQRT(ALAM2) 
 
C...Evaluate first or second order alpha_strong. 
      B0=(33.-2.*NF)/6. 
      ALGQ=LOG(MAX(1.0001,Q2EFF/ALAM2)) 
      IF(MSTU(111).EQ.1) THEN 
        ULALPS0=MIN(PARU(115),PARU(2)/(B0*ALGQ)) 
      ELSE 
        B1=(153.-19.*NF)/6. 
        ULALPS0=MIN(PARU(115),PARU(2)/(B0*ALGQ)*(1.-B1*LOG(ALGQ)/ 
     &  (B0**2*ALGQ))) 
      ENDIF 
      MSTU(118)=NF 
      PARU(118)=ULALPS0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION ULANGL0(X,Y) 
 
C...Purpose: to reconstruct an angle from given x and y coordinates. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT01/ 
 
      ULANGL0=0. 
      R=SQRT(X**2+Y**2) 
      IF(R.LT.1E-20) RETURN 
      IF(ABS(X)/R.LT.0.8) THEN 
        ULANGL0=SIGN(ACOS(X/R),Y) 
      ELSE 
        ULANGL0=ASIN(Y/R) 
        IF(X.LT.0..AND.ULANGL0.GE.0.) THEN 
          ULANGL0=PARU(1)-ULANGL0 
        ELSEIF(X.LT.0.) THEN 
          ULANGL0=-PARU(1)-ULANGL0 
        ENDIF 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION RLU0(IDUMMY) 
cc-- unified the random engine with BesEvtGen, pingrg-2010-1-26
      external  RLU   
      RLU0=RLU(0)    
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE RLUGET0(LFN,MOVE) 
 
C...Purpose: to dump the state of the random number generator on a file 
C...for subsequent startup from this state onwards. 
      COMMON/LUDAT0R/MRLU0(6),RRLU0(100) 
      SAVE /LUDAT0R/ 
      CHARACTER CHERR*8 
 
C...Backspace required number of records (or as many as there are). 
      IF(MOVE.LT.0) THEN 
        NBCK=MIN(MRLU0(6),-MOVE) 
        DO 100 IBCK=1,NBCK 
        BACKSPACE(LFN,ERR=110,IOSTAT=IERR) 
  100   CONTINUE 
        MRLU0(6)=MRLU0(6)-NBCK 
      ENDIF 
 
C...Unformatted write on unit LFN. 
      WRITE(LFN,ERR=110,IOSTAT=IERR) (MRLU0(I1),I1=1,5), 
     &(RRLU0(I2),I2=1,100) 
      MRLU0(6)=MRLU0(6)+1 
      RETURN 
 
C...Write error. 
  110 WRITE(CHERR,'(I8)') IERR 
      CALL LUERRM0(18,'(RLUGET0:) error when accessing file, IOSTAT ='// 
     &CHERR) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE RLUSET0(LFN,MOVE) 
 
C...Purpose: to read a state of the random number generator from a file 
C...for subsequent generation from this state onwards. 
      COMMON/LUDAT0R/MRLU0(6),RRLU0(100) 
      SAVE /LUDAT0R/ 
      CHARACTER CHERR*8 
 
C...Backspace required number of records (or as many as there are). 
      IF(MOVE.LT.0) THEN 
        NBCK=MIN(MRLU0(6),-MOVE) 
        DO 100 IBCK=1,NBCK 
        BACKSPACE(LFN,ERR=120,IOSTAT=IERR) 
  100   CONTINUE 
        MRLU0(6)=MRLU0(6)-NBCK 
      ENDIF 
 
C...Unformatted read from unit LFN. 
      NFOR=1+MAX(0,MOVE) 
      DO 110 IFOR=1,NFOR 
      READ(LFN,ERR=120,IOSTAT=IERR) (MRLU0(I1),I1=1,5), 
     &(RRLU0(I2),I2=1,100) 
  110 CONTINUE 
      MRLU0(6)=MRLU0(6)+NFOR 
      RETURN 
 
C...Write error. 
  120 WRITE(CHERR,'(I8)') IERR 
      CALL LUERRM0(18,'(RLUSET0:) error when accessing file, IOSTAT ='// 
     &CHERR) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUROBO0(THE,PHI,BEX,BEY,BEZ) 
 
C...Purpose: to perform rotations and boosts. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/CHECKTAG/DECAYTAG !CHECK THE DECAY 
      real*8 DECAYTAG
      SAVE /LUJETS/,/LUDAT01/ 
      DIMENSION ROT(3,3),PR(3),VR(3),DP(4),DV(4) 
 
C...Find range of rotation/boost. Convert boost to double precision.
      DECAYTAG=0
      IMIN=1 
      IF(MSTU(1).GT.0) IMIN=MSTU(1) 
      IMAX=N 
      IF(MSTU(2).GT.0) IMAX=MSTU(2) 
      DBX=BEX 
      DBY=BEY 
      DBZ=BEZ 
      GOTO 120 
 
C...Entry for specific range and double precision boost. 
      ENTRY LUDBRB0(IMI,IMA,THE,PHI,DBEX,DBEY,DBEZ) 
      IMIN=IMI 
      IF(IMIN.LE.0) IMIN=1 
      IMAX=IMA 
      IF(IMAX.LE.0) IMAX=N 
      DBX=DBEX 
      DBY=DBEY 
      DBZ=DBEZ 
 
C...Optional resetting of V (when not set before.) 
      IF(MSTU(33).NE.0) THEN 
        DO 110 I=MIN(IMIN,MSTU(4)),MIN(IMAX,MSTU(4)) 
        DO 100 J=1,5 
        V(I,J)=0. 
  100   CONTINUE 
  110 CONTINUE 
        MSTU(33)=0 
      ENDIF 
 
C...Check range of rotation/boost. 
  120 IF(IMIN.GT.MSTU(4).OR.IMAX.GT.MSTU(4)) THEN 
        CALL LUERRM0(11,'(LUROBO0:) range outside LUJETS memory') 
        DECAYTAG=1
        RETURN 
      ENDIF 
 
C...Rotate, typically from z axis to direction (theta,phi). 
      IF(THE**2+PHI**2.GT.1E-20) THEN 
        ROT(1,1)=COS(THE)*COS(PHI) 
        ROT(1,2)=-SIN(PHI) 
        ROT(1,3)=SIN(THE)*COS(PHI) 
        ROT(2,1)=COS(THE)*SIN(PHI) 
        ROT(2,2)=COS(PHI) 
        ROT(2,3)=SIN(THE)*SIN(PHI) 
        ROT(3,1)=-SIN(THE) 
        ROT(3,2)=0. 
        ROT(3,3)=COS(THE) 
        DO 150 I=IMIN,IMAX 
        IF(K(I,1).LE.0) GOTO 150 
        DO 130 J=1,3 
        PR(J)=P(I,J) 
        VR(J)=V(I,J) 
  130   CONTINUE 
        DO 140 J=1,3 
        P(I,J)=ROT(J,1)*PR(1)+ROT(J,2)*PR(2)+ROT(J,3)*PR(3) 
        V(I,J)=ROT(J,1)*VR(1)+ROT(J,2)*VR(2)+ROT(J,3)*VR(3) 
  140   CONTINUE 
  150   CONTINUE 
      ENDIF 
 
C...Boost, typically from rest to momentum/energy=beta. 
      IF(DBX**2+DBY**2+DBZ**2.GT.1E-20) THEN 
        DB=SQRT(DBX**2+DBY**2+DBZ**2) 
        IF(DB.GT.0.99999999D0) THEN 
C...Rescale boost vector if too close to unity. 
          CALL LUERRM0(3,'(LUROBO0:) boost vector too large') 
          DBX=DBX*(0.99999999D0/DB) 
          DBY=DBY*(0.99999999D0/DB) 
          DBZ=DBZ*(0.99999999D0/DB) 
          DB=0.99999999D0 
        ENDIF 
        DGA=1D0/SQRT(1D0-DB**2) 
        DO 170 I=IMIN,IMAX 
        IF(K(I,1).LE.0) GOTO 170 
        DO 160 J=1,4 
        DP(J)=P(I,J) 
        DV(J)=V(I,J) 
  160   CONTINUE 
        DBP=DBX*DP(1)+DBY*DP(2)+DBZ*DP(3) 
        DGABP=DGA*(DGA*DBP/(1D0+DGA)+DP(4)) 
        P(I,1)=DP(1)+DGABP*DBX 
        P(I,2)=DP(2)+DGABP*DBY 
        P(I,3)=DP(3)+DGABP*DBZ 
        P(I,4)=DGA*(DP(4)+DBP) 
        DBV=DBX*DV(1)+DBY*DV(2)+DBZ*DV(3) 
        DGABV=DGA*(DGA*DBV/(1D0+DGA)+DV(4)) 
        V(I,1)=DV(1)+DGABV*DBX 
        V(I,2)=DV(2)+DGABV*DBY 
        V(I,3)=DV(3)+DGABV*DBZ 
        V(I,4)=DGA*(DV(4)+DBV) 
  170   CONTINUE 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUEDIT0(MEDIT) 
 
C...Purpose: to perform global manipulations on the event record, 
C...in particular to exclude unstable or undetectable partons/particles. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION NS(2),PTS(2),PLS(2) 
 
C...Remove unwanted partons/particles. 
      IF((MEDIT.GE.0.AND.MEDIT.LE.3).OR.MEDIT.EQ.5) THEN 
        IMAX=N 
        IF(MSTU(2).GT.0) IMAX=MSTU(2) 
        I1=MAX(1,MSTU(1))-1 
        DO 110 I=MAX(1,MSTU(1)),IMAX 
        IF(K(I,1).EQ.0.OR.K(I,1).GT.20) GOTO 110 
        IF(MEDIT.EQ.1) THEN 
          IF(K(I,1).GT.10) GOTO 110 
        ELSEIF(MEDIT.EQ.2) THEN 
          IF(K(I,1).GT.10) GOTO 110 
          KC=LUCOMP0(K(I,2)) 
          IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR.KC.EQ.18) 
     &    GOTO 110 
        ELSEIF(MEDIT.EQ.3) THEN 
          IF(K(I,1).GT.10) GOTO 110 
          KC=LUCOMP0(K(I,2)) 
          IF(KC.EQ.0) GOTO 110 
          IF(KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) GOTO 110 
        ELSEIF(MEDIT.EQ.5) THEN 
          IF(K(I,1).EQ.13.OR.K(I,1).EQ.14) GOTO 110 
          KC=LUCOMP0(K(I,2)) 
          IF(KC.EQ.0) GOTO 110 
          IF(K(I,1).GE.11.AND.KCHG(KC,2).EQ.0) GOTO 110 
        ENDIF 
 
C...Pack remaining partons/particles. Origin no longer known. 
        I1=I1+1 
        DO 100 J=1,5 
        K(I1,J)=K(I,J) 
        P(I1,J)=P(I,J) 
        V(I1,J)=V(I,J) 
  100   CONTINUE 
        K(I1,3)=0 
  110   CONTINUE 
        IF(I1.LT.N) MSTU(3)=0 
        IF(I1.LT.N) MSTU(70)=0 
        N=I1 
 
C...Selective removal of class of entries. New position of retained. 
      ELSEIF(MEDIT.GE.11.AND.MEDIT.LE.15) THEN 
        I1=0 
        DO 120 I=1,N 
        K(I,3)=MOD(K(I,3),MSTU(5)) 
        IF(MEDIT.EQ.11.AND.K(I,1).LT.0) GOTO 120 
        IF(MEDIT.EQ.12.AND.K(I,1).EQ.0) GOTO 120 
        IF(MEDIT.EQ.13.AND.(K(I,1).EQ.11.OR.K(I,1).EQ.12.OR. 
     &  K(I,1).EQ.15).AND.K(I,2).NE.94) GOTO 120 
        IF(MEDIT.EQ.14.AND.(K(I,1).EQ.13.OR.K(I,1).EQ.14.OR. 
     &  K(I,2).EQ.94)) GOTO 120 
        IF(MEDIT.EQ.15.AND.K(I,1).GE.21) GOTO 120 
        I1=I1+1 
        K(I,3)=K(I,3)+MSTU(5)*I1 
  120   CONTINUE 
 
C...Find new event history information and replace old. 
        DO 140 I=1,N 
        IF(K(I,1).LE.0.OR.K(I,1).GT.20.OR.K(I,3)/MSTU(5).EQ.0) GOTO 140 
        ID=I 
  130   IM=MOD(K(ID,3),MSTU(5)) 
        IF(MEDIT.EQ.13.AND.IM.GT.0.AND.IM.LE.N) THEN 
          IF((K(IM,1).EQ.11.OR.K(IM,1).EQ.12.OR.K(IM,1).EQ.15).AND. 
     &    K(IM,2).NE.94) THEN 
            ID=IM 
            GOTO 130 
          ENDIF 
        ELSEIF(MEDIT.EQ.14.AND.IM.GT.0.AND.IM.LE.N) THEN 
          IF(K(IM,1).EQ.13.OR.K(IM,1).EQ.14.OR.K(IM,2).EQ.94) THEN 
            ID=IM 
            GOTO 130 
          ENDIF 
        ENDIF 
        K(I,3)=MSTU(5)*(K(I,3)/MSTU(5)) 
        IF(IM.NE.0) K(I,3)=K(I,3)+K(IM,3)/MSTU(5) 
        IF(K(I,1).NE.3.AND.K(I,1).NE.13.AND.K(I,1).NE.14) THEN 
          IF(K(I,4).GT.0.AND.K(I,4).LE.MSTU(4)) K(I,4)= 
     &    K(K(I,4),3)/MSTU(5) 
          IF(K(I,5).GT.0.AND.K(I,5).LE.MSTU(4)) K(I,5)= 
     &    K(K(I,5),3)/MSTU(5) 
        ELSE 
          KCM=MOD(K(I,4)/MSTU(5),MSTU(5)) 
          IF(KCM.GT.0.AND.KCM.LE.MSTU(4)) KCM=K(KCM,3)/MSTU(5) 
          KCD=MOD(K(I,4),MSTU(5)) 
          IF(KCD.GT.0.AND.KCD.LE.MSTU(4)) KCD=K(KCD,3)/MSTU(5) 
          K(I,4)=MSTU(5)**2*(K(I,4)/MSTU(5)**2)+MSTU(5)*KCM+KCD 
          KCM=MOD(K(I,5)/MSTU(5),MSTU(5)) 
          IF(KCM.GT.0.AND.KCM.LE.MSTU(4)) KCM=K(KCM,3)/MSTU(5) 
          KCD=MOD(K(I,5),MSTU(5)) 
          IF(KCD.GT.0.AND.KCD.LE.MSTU(4)) KCD=K(KCD,3)/MSTU(5) 
          K(I,5)=MSTU(5)**2*(K(I,5)/MSTU(5)**2)+MSTU(5)*KCM+KCD 
        ENDIF 
  140   CONTINUE 
 
C...Pack remaining entries. 
        I1=0 
        MSTU90=MSTU(90) 
        MSTU(90)=0 
        DO 170 I=1,N 
        IF(K(I,3)/MSTU(5).EQ.0) GOTO 170 
        I1=I1+1 
        DO 150 J=1,5 
        K(I1,J)=K(I,J) 
        P(I1,J)=P(I,J) 
        V(I1,J)=V(I,J) 
  150   CONTINUE 
        K(I1,3)=MOD(K(I1,3),MSTU(5)) 
        DO 160 IZ=1,MSTU90 
        IF(I.EQ.MSTU(90+IZ)) THEN 
          MSTU(90)=MSTU(90)+1 
          MSTU(90+MSTU(90))=I1 
          PARU(90+MSTU(90))=PARU(90+IZ) 
        ENDIF 
  160   CONTINUE 
  170   CONTINUE 
        IF(I1.LT.N) MSTU(3)=0 
        IF(I1.LT.N) MSTU(70)=0 
        N=I1 
 
C...Fill in some missing daughter pointers (lost in colour flow). 
      ELSEIF(MEDIT.EQ.16) THEN 
        DO 190 I=1,N 
        IF(K(I,1).LE.10.OR.K(I,1).GT.20) GOTO 190 
        IF(K(I,4).NE.0.OR.K(I,5).NE.0) GOTO 190 
C...Find daughters who point to mother.
        DO 180 I1=I+1,N 
        IF(K(I1,3).NE.I) THEN 
        ELSEIF(K(I,4).EQ.0) THEN 
          K(I,4)=I1 
        ELSE 
          K(I,5)=I1 
        ENDIF 
  180   CONTINUE 
        IF(K(I,5).EQ.0) K(I,5)=K(I,4)
        IF(K(I,4).NE.0) GOTO 190
C...Find daughters who point to documentation version of mother.      
        IM=K(I,3)
        IF(IM.LE.0.OR.IM.GE.I) GOTO 190
        IF(K(IM,1).LE.20.OR.K(IM,1).GT.30) GOTO 190  
        IF(K(IM,2).NE.K(I,2).OR.ABS(P(IM,5)-P(I,5)).GT.1E-2) GOTO 190
        DO 182 I1=I+1,N 
        IF(K(I1,3).NE.IM) THEN 
        ELSEIF(K(I,4).EQ.0) THEN 
          K(I,4)=I1 
        ELSE 
          K(I,5)=I1 
        ENDIF 
  182   CONTINUE 
        IF(K(I,5).EQ.0) K(I,5)=K(I,4)
        IF(K(I,4).NE.0) GOTO 190
C...Find daughters who point to documentation daughters who,
C...in their turn, point to documentation mother.
        ID1=IM
        ID2=IM
        DO 184 I1=IM+1,I-1
        IF(K(I1,3).EQ.IM.AND.K(I1,1).GT.20.AND.K(I1,1).LE.30) THEN
          ID2=I1
          IF(ID1.EQ.IM) ID1=I1
        ENDIF
  184   CONTINUE 
        DO 186 I1=I+1,N 
        IF(K(I1,3).NE.ID1.AND.K(I1,3).NE.ID2) THEN 
        ELSEIF(K(I,4).EQ.0) THEN 
          K(I,4)=I1 
        ELSE 
          K(I,5)=I1 
        ENDIF 
  186   CONTINUE 
        IF(K(I,5).EQ.0) K(I,5)=K(I,4)
  190   CONTINUE 
 
C...Save top entries at bottom of LUJETS commonblock. 
      ELSEIF(MEDIT.EQ.21) THEN 
        IF(2*N.GE.MSTU(4)) THEN 
          CALL LUERRM0(11,'(LUEDIT0:) no more memory left in LUJETS') 
          RETURN 
        ENDIF 
        DO 210 I=1,N 
        DO 200 J=1,5 
        K(MSTU(4)-I,J)=K(I,J) 
        P(MSTU(4)-I,J)=P(I,J) 
        V(MSTU(4)-I,J)=V(I,J) 
  200   CONTINUE 
  210   CONTINUE 
        MSTU(32)=N 
 
C...Restore bottom entries of commonblock LUJETS to top. 
      ELSEIF(MEDIT.EQ.22) THEN 
        DO 230 I=1,MSTU(32) 
        DO 220 J=1,5 
        K(I,J)=K(MSTU(4)-I,J) 
        P(I,J)=P(MSTU(4)-I,J) 
        V(I,J)=V(MSTU(4)-I,J) 
  220   CONTINUE 
  230   CONTINUE 
        N=MSTU(32) 
 
C...Mark primary entries at top of commonblock LUJETS as untreated. 
      ELSEIF(MEDIT.EQ.23) THEN 
        I1=0 
        DO 240 I=1,N 
        KH=K(I,3) 
        IF(KH.GE.1) THEN 
          IF(K(KH,1).GT.20) KH=0 
        ENDIF 
        IF(KH.NE.0) GOTO 250 
        I1=I1+1 
        IF(K(I,1).GT.10.AND.K(I,1).LE.20) K(I,1)=K(I,1)-10 
  240   CONTINUE 
  250   N=I1 
 
C...Place largest axis along z axis and second largest in xy plane. 
      ELSEIF(MEDIT.EQ.31.OR.MEDIT.EQ.32) THEN 
        CALL LUDBRB0(1,N+MSTU(3),0.,-ULANGL0(P(MSTU(61),1), 
     &  P(MSTU(61),2)),0D0,0D0,0D0) 
        CALL LUDBRB0(1,N+MSTU(3),-ULANGL0(P(MSTU(61),3), 
     &  P(MSTU(61),1)),0.,0D0,0D0,0D0) 
        CALL LUDBRB0(1,N+MSTU(3),0.,-ULANGL0(P(MSTU(61)+1,1), 
     &  P(MSTU(61)+1,2)),0D0,0D0,0D0) 
        IF(MEDIT.EQ.31) RETURN 
 
C...Rotate to put slim jet along +z axis. 
        DO 260 IS=1,2 
        NS(IS)=0 
        PTS(IS)=0. 
        PLS(IS)=0. 
  260   CONTINUE 
        DO 270 I=1,N 
        IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 270 
        IF(MSTU(41).GE.2) THEN 
          KC=LUCOMP0(K(I,2)) 
          IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &    KC.EQ.18) GOTO 270 
          IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &    GOTO 270 
        ENDIF 
        IS=2.-SIGN(0.5,P(I,3)) 
        NS(IS)=NS(IS)+1 
        PTS(IS)=PTS(IS)+SQRT(P(I,1)**2+P(I,2)**2) 
  270   CONTINUE 
        IF(NS(1)*PTS(2)**2.LT.NS(2)*PTS(1)**2) 
     &  CALL LUDBRB0(1,N+MSTU(3),PARU(1),0.,0D0,0D0,0D0) 
 
C...Rotate to put second largest jet into -z,+x quadrant. 
        DO 280 I=1,N 
        IF(P(I,3).GE.0.) GOTO 280 
        IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 280 
        IF(MSTU(41).GE.2) THEN 
          KC=LUCOMP0(K(I,2)) 
          IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &    KC.EQ.18) GOTO 280 
          IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &    GOTO 280 
        ENDIF 
        IS=2.-SIGN(0.5,P(I,1)) 
        PLS(IS)=PLS(IS)-P(I,3) 
  280   CONTINUE 
        IF(PLS(2).GT.PLS(1)) CALL LUDBRB0(1,N+MSTU(3),0.,PARU(1), 
     &  0D0,0D0,0D0) 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LULIST0(MLIST) 
 
C...Purpose: to give program heading, or list an event, or particle 
C...data, or current parameter values. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/LUDAT03/ 
      CHARACTER CHAP*16,CHAC*16,CHAN*16,CHAD(5)*16,CHDL(7)*4 
      DIMENSION PS(6) 
      DATA CHDL/'(())',' ','()','!!','<>','==','(==)'/ 
 
C...Initialization printout: version number and date of last change. 
      IF(MLIST.EQ.0.OR.MSTU(12).EQ.1) THEN 
        CALL LULOGO0 
        MSTU(12)=0 
        IF(MLIST.EQ.0) RETURN 
      ENDIF 
 
C...List event data, including additional lines after N. 
      IF(MLIST.GE.1.AND.MLIST.LE.3) THEN 
        IF(MLIST.EQ.1) WRITE(MSTU(11),5100) 
        IF(MLIST.EQ.2) WRITE(MSTU(11),5200) 
        IF(MLIST.EQ.3) WRITE(MSTU(11),5300) 
        LMX=12 
        IF(MLIST.GE.2) LMX=16 
        ISTR=0 
        IMAX=N 
        IF(MSTU(2).GT.0) IMAX=MSTU(2) 
        DO 120 I=MAX(1,MSTU(1)),MAX(IMAX,N+MAX(0,MSTU(3))) 
        IF((I.GT.IMAX.AND.I.LE.N).OR.K(I,1).LT.0) GOTO 120 
 
C...Get particle name, pad it and check it is not too long. 
        CALL LUNAME0(K(I,2),CHAP) 
        LEN=0 
        DO 100 LEM=1,16 
        IF(CHAP(LEM:LEM).NE.' ') LEN=LEM 
  100   CONTINUE 
        MDL=(K(I,1)+19)/10 
        LDL=0 
        IF(MDL.EQ.2.OR.MDL.GE.8) THEN 
          CHAC=CHAP 
          IF(LEN.GT.LMX) CHAC(LMX:LMX)='?' 
        ELSE 
          LDL=1 
          IF(MDL.EQ.1.OR.MDL.EQ.7) LDL=2 
          IF(LEN.EQ.0) THEN 
            CHAC=CHDL(MDL)(1:2*LDL)//' ' 
          ELSE 
            CHAC=CHDL(MDL)(1:LDL)//CHAP(1:MIN(LEN,LMX-2*LDL))// 
     &      CHDL(MDL)(LDL+1:2*LDL)//' ' 
            IF(LEN+2*LDL.GT.LMX) CHAC(LMX:LMX)='?' 
          ENDIF 
        ENDIF 
 
C...Add information on string connection. 
        IF(K(I,1).EQ.1.OR.K(I,1).EQ.2.OR.K(I,1).EQ.11.OR.K(I,1).EQ.12) 
     &  THEN 
          KC=LUCOMP0(K(I,2)) 
          KCC=0 
          IF(KC.NE.0) KCC=KCHG(KC,2) 
          IF(IABS(K(I,2)).EQ.39) THEN 
            IF(LEN+2*LDL+3.LE.LMX) CHAC(LMX-1:LMX-1)='X' 
          ELSEIF(KCC.NE.0.AND.ISTR.EQ.0) THEN 
            ISTR=1 
            IF(LEN+2*LDL+3.LE.LMX) CHAC(LMX-1:LMX-1)='A' 
          ELSEIF(KCC.NE.0.AND.(K(I,1).EQ.2.OR.K(I,1).EQ.12)) THEN 
            IF(LEN+2*LDL+3.LE.LMX) CHAC(LMX-1:LMX-1)='I' 
          ELSEIF(KCC.NE.0) THEN 
            ISTR=0 
            IF(LEN+2*LDL+3.LE.LMX) CHAC(LMX-1:LMX-1)='V' 
          ENDIF 
        ENDIF 
 
C...Write data for particle/jet. 
        IF(MLIST.EQ.1.AND.ABS(P(I,4)).LT.9999.) THEN 
          WRITE(MSTU(11),5400) I,CHAC(1:12),(K(I,J1),J1=1,3), 
     &    (P(I,J2),J2=1,5) 
        ELSEIF(MLIST.EQ.1.AND.ABS(P(I,4)).LT.99999.) THEN 
          WRITE(MSTU(11),5500) I,CHAC(1:12),(K(I,J1),J1=1,3), 
     &    (P(I,J2),J2=1,5) 
        ELSEIF(MLIST.EQ.1) THEN 
          WRITE(MSTU(11),5600) I,CHAC(1:12),(K(I,J1),J1=1,3), 
     &    (P(I,J2),J2=1,5) 
        ELSEIF(MSTU(5).EQ.10000.AND.(K(I,1).EQ.3.OR.K(I,1).EQ.13.OR. 
     &  K(I,1).EQ.14)) THEN 
          WRITE(MSTU(11),5700) I,CHAC,(K(I,J1),J1=1,3), 
     &    K(I,4)/100000000,MOD(K(I,4)/10000,10000),MOD(K(I,4),10000), 
     &    K(I,5)/100000000,MOD(K(I,5)/10000,10000),MOD(K(I,5),10000), 
     &    (P(I,J2),J2=1,5) 
        ELSE 
          WRITE(MSTU(11),5800) I,CHAC,(K(I,J1),J1=1,5),(P(I,J2),J2=1,5) 
        ENDIF 
        IF(MLIST.EQ.3) WRITE(MSTU(11),5900) (V(I,J),J=1,5) 
 
C...Insert extra separator lines specified by user. 
        IF(MSTU(70).GE.1) THEN 
          ISEP=0 
          DO 110 J=1,MIN(10,MSTU(70)) 
          IF(I.EQ.MSTU(70+J)) ISEP=1 
  110     CONTINUE 
          IF(ISEP.EQ.1.AND.MLIST.EQ.1) WRITE(MSTU(11),6000) 
          IF(ISEP.EQ.1.AND.MLIST.GE.2) WRITE(MSTU(11),6100) 
        ENDIF 
  120   CONTINUE 
 
C...Sum of charges and momenta. 
        DO 130 J=1,6 
        PS(J)=PLU0(0,J) 
  130   CONTINUE 
        IF(MLIST.EQ.1.AND.ABS(PS(4)).LT.9999.) THEN 
          WRITE(MSTU(11),6200) PS(6),(PS(J),J=1,5) 
        ELSEIF(MLIST.EQ.1.AND.ABS(PS(4)).LT.99999.) THEN 
          WRITE(MSTU(11),6300) PS(6),(PS(J),J=1,5) 
        ELSEIF(MLIST.EQ.1) THEN 
          WRITE(MSTU(11),6400) PS(6),(PS(J),J=1,5) 
        ELSE 
          WRITE(MSTU(11),6500) PS(6),(PS(J),J=1,5) 
        ENDIF 
 
C...Give simple list of KF codes defined in program. 
      ELSEIF(MLIST.EQ.11) THEN 
        WRITE(MSTU(11),6600) 
        DO 140 KF=1,40 
        CALL LUNAME0(KF,CHAP) 
        CALL LUNAME0(-KF,CHAN) 
        IF(CHAP.NE.' '.AND.CHAN.EQ.' ') WRITE(MSTU(11),6700) KF,CHAP 
        IF(CHAN.NE.' ') WRITE(MSTU(11),6700) KF,CHAP,-KF,CHAN 
  140   CONTINUE 
        DO 170 KFLS=1,3,2 
        DO 160 KFLA=1,8 
        DO 150 KFLB=1,KFLA-(3-KFLS)/2 
        KF=1000*KFLA+100*KFLB+KFLS 
        CALL LUNAME0(KF,CHAP) 
        CALL LUNAME0(-KF,CHAN) 
        WRITE(MSTU(11),6700) KF,CHAP,-KF,CHAN 
  150   CONTINUE 
  160   CONTINUE 
  170   CONTINUE 
        KF=130 
        CALL LUNAME0(KF,CHAP) 
        WRITE(MSTU(11),6700) KF,CHAP 
        KF=310 
        CALL LUNAME0(KF,CHAP) 
        WRITE(MSTU(11),6700) KF,CHAP 
        DO 200 KMUL=0,5 
        KFLS=3 
        IF(KMUL.EQ.0.OR.KMUL.EQ.3) KFLS=1 
        IF(KMUL.EQ.5) KFLS=5 
        KFLR=0 
        IF(KMUL.EQ.2.OR.KMUL.EQ.3) KFLR=1 
        IF(KMUL.EQ.4) KFLR=2 
        DO 190 KFLB=1,8 
        DO 180 KFLC=1,KFLB-1 
        KF=10000*KFLR+100*KFLB+10*KFLC+KFLS 
        CALL LUNAME0(KF,CHAP) 
        CALL LUNAME0(-KF,CHAN) 
        WRITE(MSTU(11),6700) KF,CHAP,-KF,CHAN 
  180   CONTINUE 
        KF=10000*KFLR+110*KFLB+KFLS 
        CALL LUNAME0(KF,CHAP) 
        WRITE(MSTU(11),6700) KF,CHAP 
  190   CONTINUE 
  200 CONTINUE 
        KF=30443 
        CALL LUNAME0(KF,CHAP) 
        WRITE(MSTU(11),6700) KF,CHAP 
        KF=30553 
        CALL LUNAME0(KF,CHAP) 
        WRITE(MSTU(11),6700) KF,CHAP 
        DO 240 KFLSP=1,3 
        KFLS=2+2*(KFLSP/3) 
        DO 230 KFLA=1,8 
        DO 220 KFLB=1,KFLA 
        DO 210 KFLC=1,KFLB 
        IF(KFLSP.EQ.1.AND.(KFLA.EQ.KFLB.OR.KFLB.EQ.KFLC)) GOTO 210 
        IF(KFLSP.EQ.2.AND.KFLA.EQ.KFLC) GOTO 210 
        IF(KFLSP.EQ.1) KF=1000*KFLA+100*KFLC+10*KFLB+KFLS 
        IF(KFLSP.GE.2) KF=1000*KFLA+100*KFLB+10*KFLC+KFLS 
        CALL LUNAME0(KF,CHAP) 
        CALL LUNAME0(-KF,CHAN) 
        WRITE(MSTU(11),6700) KF,CHAP,-KF,CHAN 
  210   CONTINUE 
  220   CONTINUE 
  230   CONTINUE 
  240   CONTINUE 
 
C...List parton/particle data table. Check whether to be listed. 
      ELSEIF(MLIST.EQ.12) THEN 
        WRITE(MSTU(11),6800) 
        MSTJ24=MSTJ(24) 
        MSTJ(24)=0 
        KFMAX=30553 
        IF(MSTU(2).NE.0) KFMAX=MSTU(2) 
        DO 270 KF=MAX(1,MSTU(1)),KFMAX 
        KC=LUCOMP0(KF) 
        IF(KC.EQ.0) GOTO 270 
        IF(MSTU(14).EQ.0.AND.KF.GT.100.AND.KC.LE.100) GOTO 270 
        IF(MSTU(14).GT.0.AND.KF.GT.100.AND.MAX(MOD(KF/1000,10), 
     &  MOD(KF/100,10)).GT.MSTU(14)) GOTO 270 
        IF(MSTU(14).GT.0.AND.KF.GT.100.AND.KC.EQ.90) GOTO 270 
 
C...Find particle name and mass. Print information. 
        CALL LUNAME0(KF,CHAP) 
        IF(KF.LE.100.AND.CHAP.EQ.' '.AND.MDCY(KC,2).EQ.0) GOTO 270 
        CALL LUNAME0(-KF,CHAN) 
        PM=ULMASS0(KF) 
        WRITE(MSTU(11),6900) KF,KC,CHAP,CHAN,KCHG(KC,1),KCHG(KC,2), 
     &  KCHG(KC,3),PM,PMAS(KC,2),PMAS(KC,3),PMAS(KC,4),MDCY(KC,1) 
 
C...Particle decay: channel number, branching ration, matrix element, 
C...decay products. 
        IF(KF.GT.100.AND.KC.LE.100) GOTO 270 
        DO 260 IDC=MDCY(KC,2),MDCY(KC,2)+MDCY(KC,3)-1 
        DO 250 J=1,5 
        CALL LUNAME0(KFDP(IDC,J),CHAD(J)) 
  250   CONTINUE 
        WRITE(MSTU(11),7000) IDC,MDME(IDC,1),MDME(IDC,2),BRAT(IDC), 
     &  (CHAD(J),J=1,5) 
  260   CONTINUE 
  270   CONTINUE 
        MSTJ(24)=MSTJ24 
 
C...List parameter value table. 
      ELSEIF(MLIST.EQ.13) THEN 
        WRITE(MSTU(11),7100) 
        DO 280 I=1,200 
        WRITE(MSTU(11),7200) I,MSTU(I),PARU(I),MSTJ(I),PARJ(I),PARF(I) 
  280   CONTINUE 
      ENDIF 
 
C...Format statements for output on unit MSTU(11) (by default 6). 
 5100 FORMAT(///28X,'Event listing (summary)'//4X,'I  particle/jet KS', 
     &5X,'KF orig    p_x      p_y      p_z       E        m'/) 
 5200 FORMAT(///28X,'Event listing (standard)'//4X,'I  particle/jet', 
     &'  K(I,1)   K(I,2) K(I,3)     K(I,4)      K(I,5)       P(I,1)', 
     &'       P(I,2)       P(I,3)       P(I,4)       P(I,5)'/) 
 5300 FORMAT(///28X,'Event listing (with vertices)'//4X,'I  particle/j', 
     &'et  K(I,1)   K(I,2) K(I,3)     K(I,4)      K(I,5)       P(I,1)', 
     &'       P(I,2)       P(I,3)       P(I,4)       P(I,5)'/73X, 
     &'V(I,1)       V(I,2)       V(I,3)       V(I,4)       V(I,5)'/) 
 5400 FORMAT(1X,I4,2X,A12,1X,I2,1X,I6,1X,I4,5F9.3) 
 5500 FORMAT(1X,I4,2X,A12,1X,I2,1X,I6,1X,I4,5F9.2) 
 5600 FORMAT(1X,I4,2X,A12,1X,I2,1X,I6,1X,I4,5F9.1) 
 5700 FORMAT(1X,I4,2X,A16,1X,I3,1X,I8,2X,I4,2(3X,I1,2I4),5F13.5) 
 5800 FORMAT(1X,I4,2X,A16,1X,I3,1X,I8,2X,I4,2(3X,I9),5F13.5) 
 5900 FORMAT(66X,5(1X,F12.3)) 
 6000 FORMAT(1X,78('=')) 
 6100 FORMAT(1X,130('=')) 
 6200 FORMAT(19X,'sum:',F6.2,5X,5F9.3) 
 6300 FORMAT(19X,'sum:',F6.2,5X,5F9.2) 
 6400 FORMAT(19X,'sum:',F6.2,5X,5F9.1) 
 6500 FORMAT(19X,'sum charge:',F6.2,3X,'sum momentum and inv. mass:', 
     &5F13.5) 
 6600 FORMAT(///20X,'List of KF codes in program'/) 
 6700 FORMAT(4X,I6,4X,A16,6X,I6,4X,A16) 
 6800 FORMAT(///30X,'Particle/parton data table'//5X,'KF',5X,'KC',4X, 
     &'particle',8X,'antiparticle',6X,'chg  col  anti',8X,'mass',7X, 
     &'width',7X,'w-cut',5X,'lifetime',1X,'decay'/11X,'IDC',1X,'on/off', 
     &1X,'ME',3X,'Br.rat.',4X,'decay products') 
 6900 FORMAT(/1X,I6,3X,I4,4X,A16,A16,3I5,1X,F12.5,2(1X,F11.5), 
     &2X,F12.5,3X,I2) 
 7000 FORMAT(10X,I4,2X,I3,2X,I3,2X,F8.5,4X,5A16) 
 7100 FORMAT(///20X,'Parameter value table'//4X,'I',3X,'MSTU(I)', 
     &8X,'PARU(I)',3X,'MSTJ(I)',8X,'PARJ(I)',8X,'PARF(I)') 
 7200 FORMAT(1X,I4,1X,I9,1X,F14.5,1X,I9,1X,F14.5,1X,F14.5) 
 
      RETURN 
      END 
   
C*********************************************************************

      SUBROUTINE LULOGO0

C...Purpose: to write logo for JETSET and PYTHIA programs.
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      SAVE /LUDAT01/
      SAVE /PYPARS/
      CHARACTER MONTH(12)*3, LOGO(48)*32, REFER(16)*36, LINE*79,
     &VERS*1, SUBV*3, DATE*2, YEAR*4

C...Data on months, logo, titles, and references.
      DATA MONTH/'Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep',
     &'Oct','Nov','Dec'/
      DATA (LOGO(J),J=1,10)/
     &'L    U  U   A   RRRRR L  W    W ',
     &'L    U  U  A A  R   R L  W    W ',
     &'L    U  U AAAAA RRRRR L   W W W ',
     &'L    U  U A   A R  R  L    W W  ',
     &'LLLL UUUU A   A R   R LLLL  W   ',
     &'JJJJ EEEE TTTTT  SSS  EEEE TTTTT',
     &'   J E      T   S     E      T  ',
     &'   J EEE    T    SSS  EEE    T  ',
     &'J  J E      T       S E      T  ',
     &' JJ  EEEE   T    SSS  EEEE   T  '/
      DATA (LOGO(J),J=11,29)/
     &'             p         p~  pi0. ',
     &'pi+  pi-     .         .    .:: ',
     &':.    :.    :::      .::. .:::  ',
     &':::  :::.  :::::.  .:::::*:::   ',
     &' :::*::::*:::::::*::::::::::    ',
     &'  :::::::::::::::::::::::::     ',
     &'   :::::::::::::::::::::::      ',
     &'    :::::::::::::::::::::       ',
     &'     :::::::::::::::::::        ',
     &'      ::::::: A :::::::         ',
     &'       :::::::::::::::          ',
     &'        :::::::::::::           ',
     &'         :::::::::::            ',
     &'          :::::::::             ',
     &'           :::::::              ',
     &'            :::::               ',
     &'             :::                ',
     &'           q0 * q0~             ',
     &'                                '/
      DATA (LOGO(J),J=30,48)/
     &'Welcome to the Lund Monte Carlo!',
     &'                                ',
     &'  This is PYTHIA version x.xxx  ',
     &'Last date of change: xx xxx 199x',
     &'        LUARLW must be          ',
     &'used with JETSET version x.xxx  ',
     &'Last date of change: xx xxx 199x',
     &'                                ',
     &'                                ',
     &'          Main authors:         ',
     &' JETSET: Torbjorn Sjostrand     ',
     &'   Theory Division, CERN,       ',
     &'       CH-1211 Geneva 23,       ',
     &'          Switzerland           ',
     &'  E-mail TORSJO@CERNVM.CERN.CH  ',
     &'         -------------          ',
     &'  LUARLW:    Haiming Hu         ',
     &'  Beijing, IHEP, P.O.Box 918-(1)',
     &'  E-mail HUHM@PONY2.IHEP.AC.CN  '/
      DATA REFER/
     &'                      ----- COMMENT ',
     &'-----                               ',
     &'The well-known Monte Carlo JETSET is',
     &' not built in order to describe few ',
     &'body states (in particular at few   ',
     &'GeV level in e+e- annihilation as at',
     &'BEPC). A new formalism which use the',
     &'basic Lund area law for Monte Carlo ',
     &'simulation was developed. LUARLW can',
     &' be used to generate e+e-=>2,3,4,5,6',
     &'hadrons in string fragmentation. In ',
     &'LUARLW, the gluon effect was ignored',
     &'for it is not important at BEPC/BES ',
     &'energy region. The treatments to the',
     &'baryon production will be improved  ',
     &'in new LUARLW version. (by H. Hu)   '/
     

C...Check if PYTHIA linked.
      IF(MSTP(183)/10.NE.199) THEN
        LOGO(32)=' This is LUARLW version 1.0     '
        LOGO(33)='Last change data: 30 Nov 1999   '
      ELSE
        WRITE(VERS,'(I1)') MSTP(181)
        LOGO(32)(26:26)=VERS
        WRITE(SUBV,'(I3)') MSTP(182)
        LOGO(32)(28:30)=SUBV
        WRITE(DATE,'(I2)') MSTP(185)
        LOGO(33)(22:23)=DATE
        LOGO(33)(25:27)=MONTH(MSTP(184))
        WRITE(YEAR,'(I4)') MSTP(183)
        LOGO(33)(29:32)=YEAR
      ENDIF

C...Check if JETSET linked.
      IF(MSTU(183)/10.NE.199) THEN
        LOGO(35)='  Error: JETSET is not loaded!  '
        LOGO(36)='Did you remember to link LUDATA0?'
      ELSE
        WRITE(VERS,'(I1)') MSTU(181)
        LOGO(35)(26:26)=VERS
        WRITE(SUBV,'(I3)') MSTU(182)
        LOGO(35)(28:30)=SUBV
        WRITE(DATE,'(I2)') MSTU(185)
        LOGO(36)(22:23)=DATE
        LOGO(36)(25:27)=MONTH(MSTU(184))
        WRITE(YEAR,'(I4)') MSTU(183)
        LOGO(36)(29:32)=YEAR
      ENDIF

C...Loop over lines in header. Define page feed and side borders.
      DO 100 ILIN=1,45
      LINE=' '
      IF(ILIN.EQ.1) THEN
        LINE(1:1)='1'
      ELSE
        LINE(2:3)='**'
        LINE(78:79)='**'
      ENDIF

C...Separator lines and logos.
      IF(ILIN.EQ.2.OR.ILIN.EQ.3.OR.ILIN.EQ.44.OR.ILIN.EQ.45) THEN
        LINE(4:77)='***********************************************'//
     &  '***************************'
      ELSEIF(ILIN.GE.6.AND.ILIN.LE.10) THEN
        LINE(6:37)=LOGO(ILIN-5)
        LINE(44:75)=LOGO(ILIN)
      ELSEIF(ILIN.GE.13.AND.ILIN.LE.31) THEN
        LINE(6:37)=LOGO(ILIN-2)
        LINE(44:75)=LOGO(ILIN+17)
      ELSEIF(ILIN.GE.34.AND.ILIN.LE.41) THEN
        LINE(5:40)=REFER(2*ILIN-67)
        LINE(41:76)=REFER(2*ILIN-66)
      ENDIF

C...Write lines to appropriate unit.
      IF(MSTU(183)/10.EQ.199) THEN
        WRITE(MSTU(11),'(A79)') LINE
      ELSE
        WRITE(*,'(A79)') LINE
      ENDIF
  100 CONTINUE

C...Check that matching subversions are linked.
      IF(MSTU(183)/10.EQ.199.AND.MSTP(183)/10.EQ.199) THEN
        IF(MSTU(182).LT.MSTP(186)) WRITE(MSTU(11),
     &  '(/'' Warning: JETSET subversion too old for PYTHIA''/)')
        IF(MSTP(182).LT.MSTU(186)) WRITE(MSTU(11),
     &  '(/'' Warning: PYTHIA subversion too old for JETSET''/)')
      ENDIF

      RETURN
      END

 
 
C********************************************************************* 
 
      SUBROUTINE LUUPDA0(MUPDA,LFN) 
 
C...Purpose: to facilitate the updating of particle and decay data. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      COMMON/LUDAT04/CHAF(500) 
      CHARACTER CHAF*8 
      SAVE /LUDAT01/,/LUDAT02/,/LUDAT03/,/LUDAT04/ 
      CHARACTER CHINL*80,CHKC*4,CHVAR(19)*9,CHLIN*72, 
     &CHBLK(20)*72,CHOLD*12,CHTMP*12,CHNEW*12,CHCOM*12 
      DATA CHVAR/ 'KCHG(I,1)','KCHG(I,2)','KCHG(I,3)','PMAS(I,1)', 
     &'PMAS(I,2)','PMAS(I,3)','PMAS(I,4)','MDCY(I,1)','MDCY(I,2)', 
     &'MDCY(I,3)','MDME(I,1)','MDME(I,2)','BRAT(I)  ','KFDP(I,1)', 
     &'KFDP(I,2)','KFDP(I,3)','KFDP(I,4)','KFDP(I,5)','CHAF(I)  '/ 
 
C...Write information on file for editing. 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      IF(MUPDA.EQ.1) THEN 
        DO 110 KC=1,MSTU(6) 
        WRITE(LFN,5000) KC,CHAF(KC),(KCHG(KC,J1),J1=1,3), 
     &  (PMAS(KC,J2),J2=1,4),MDCY(KC,1) 
        DO 100 IDC=MDCY(KC,2),MDCY(KC,2)+MDCY(KC,3)-1 
        WRITE(LFN,5100) MDME(IDC,1),MDME(IDC,2),BRAT(IDC), 
     &  (KFDP(IDC,J),J=1,5) 
  100   CONTINUE 
  110   CONTINUE 
 
C...Reset variables and read information from edited file. 
      ELSEIF(MUPDA.EQ.2) THEN 
        DO 130 I=1,MSTU(7) 
        MDME(I,1)=1 
        MDME(I,2)=0 
        BRAT(I)=0. 
        DO 120 J=1,5 
        KFDP(I,J)=0 
  120   CONTINUE 
  130   CONTINUE 
        KC=0 
        IDC=0 
        NDC=0 
  140   READ(LFN,5200,END=150) CHINL 
        IF(CHINL(2:5).NE.'    ') THEN 
          CHKC=CHINL(2:5) 
          IF(KC.NE.0) THEN 
            MDCY(KC,2)=0 
            IF(NDC.NE.0) MDCY(KC,2)=IDC+1-NDC 
            MDCY(KC,3)=NDC 
          ENDIF 
          READ(CHKC,5300) KC 
          IF(KC.LE.0.OR.KC.GT.MSTU(6)) CALL LUERRM0(27, 
     &    '(LUUPDA0:) Read KC code illegal, KC ='//CHKC) 
          READ(CHINL,5000) KCR,CHAF(KC),(KCHG(KC,J1),J1=1,3), 
     &    (PMAS(KC,J2),J2=1,4),MDCY(KC,1) 
          NDC=0 
        ELSE 
          IDC=IDC+1 
          NDC=NDC+1 
          IF(IDC.GE.MSTU(7)) CALL LUERRM0(27, 
     &    '(LUUPDA0:) Decay data arrays full by KC ='//CHKC) 
          READ(CHINL,5100) MDME(IDC,1),MDME(IDC,2),BRAT(IDC), 
     &    (KFDP(IDC,J),J=1,5) 
        ENDIF 
        GOTO 140 
  150   MDCY(KC,2)=0 
        IF(NDC.NE.0) MDCY(KC,2)=IDC+1-NDC 
        MDCY(KC,3)=NDC 
 
C...Perform possible tests that new information is consistent. 
        MSTJ24=MSTJ(24) 
        MSTJ(24)=0 
        DO 180 KC=1,MSTU(6) 
        WRITE(CHKC,5300) KC 
        IF(MIN(PMAS(KC,1),PMAS(KC,2),PMAS(KC,3),PMAS(KC,1)-PMAS(KC,3), 
     &  PMAS(KC,4)).LT.0..OR.MDCY(KC,3).LT.0) CALL LUERRM0(17, 
     &  '(LUUPDA0:) Mass/width/life/(# channels) wrong for KC ='//CHKC) 
        BRSUM=0. 
        DO 170 IDC=MDCY(KC,2),MDCY(KC,2)+MDCY(KC,3)-1 
        IF(MDME(IDC,2).GT.80) GOTO 170 
        KQ=KCHG(KC,1) 
        PMS=PMAS(KC,1)-PMAS(KC,3)-PARJ(64) 
        MERR=0 
        DO 160 J=1,5 
        KP=KFDP(IDC,J) 
        IF(KP.EQ.0.OR.KP.EQ.81.OR.IABS(KP).EQ.82) THEN 
        ELSEIF(LUCOMP0(KP).EQ.0) THEN 
          MERR=3 
        ELSE 
          KQ=KQ-LUCHGE0(KP) 
          PMS=PMS-ULMASS0(KP) 
        ENDIF 
  160   CONTINUE 
        IF(KQ.NE.0) MERR=MAX(2,MERR) 
        IF(KFDP(IDC,2).NE.0.AND.(KC.LE.20.OR.KC.GT.40).AND. 
     &  (KC.LE.80.OR.KC.GT.100).AND.MDME(IDC,2).NE.34.AND. 
     &  MDME(IDC,2).NE.61.AND.PMS.LT.0.) MERR=MAX(1,MERR) 
        IF(MERR.EQ.3) CALL LUERRM0(17, 
     &  '(LUUPDA0:) Unknown particle code in decay of KC ='//CHKC) 
        IF(MERR.EQ.2) CALL LUERRM0(17, 
     &  '(LUUPDA0:) Charge not conserved in decay of KC ='//CHKC) 
        IF(MERR.EQ.1) CALL LUERRM0(7, 
     &  '(LUUPDA0:) Kinematically unallowed decay of KC ='//CHKC) 
        BRSUM=BRSUM+BRAT(IDC) 
  170   CONTINUE 
        WRITE(CHTMP,5500) BRSUM 
        IF(ABS(BRSUM).GT.0.0005.AND.ABS(BRSUM-1.).GT.0.0005) CALL 
     &  LUERRM0(7,'(LUUPDA0:) Sum of branching ratios is 
     &         '//CHTMP(5:12)// ' for KC ='//CHKC) 
  180   CONTINUE 
        MSTJ(24)=MSTJ24 
 
C...Initialize writing of DATA statements for inclusion in program. 
      ELSEIF(MUPDA.EQ.3) THEN 
        DO 250 IVAR=1,19 
        NDIM=MSTU(6) 
        IF(IVAR.GE.11.AND.IVAR.LE.18) NDIM=MSTU(7) 
        NLIN=1 
        CHLIN=' ' 
        CHLIN(7:35)='DATA ('//CHVAR(IVAR)//',I=   1,    )/' 
        LLIN=35 
        CHOLD='START' 
 
C...Loop through variables for conversion to characters. 
        DO 230 IDIM=1,NDIM 
        IF(IVAR.EQ.1) WRITE(CHTMP,5400) KCHG(IDIM,1) 
        IF(IVAR.EQ.2) WRITE(CHTMP,5400) KCHG(IDIM,2) 
        IF(IVAR.EQ.3) WRITE(CHTMP,5400) KCHG(IDIM,3) 
        IF(IVAR.EQ.4) WRITE(CHTMP,5500) PMAS(IDIM,1) 
        IF(IVAR.EQ.5) WRITE(CHTMP,5500) PMAS(IDIM,2) 
        IF(IVAR.EQ.6) WRITE(CHTMP,5500) PMAS(IDIM,3) 
        IF(IVAR.EQ.7) WRITE(CHTMP,5500) PMAS(IDIM,4) 
        IF(IVAR.EQ.8) WRITE(CHTMP,5400) MDCY(IDIM,1) 
        IF(IVAR.EQ.9) WRITE(CHTMP,5400) MDCY(IDIM,2) 
        IF(IVAR.EQ.10) WRITE(CHTMP,5400) MDCY(IDIM,3) 
        IF(IVAR.EQ.11) WRITE(CHTMP,5400) MDME(IDIM,1) 
        IF(IVAR.EQ.12) WRITE(CHTMP,5400) MDME(IDIM,2) 
        IF(IVAR.EQ.13) WRITE(CHTMP,5500) BRAT(IDIM) 
        IF(IVAR.EQ.14) WRITE(CHTMP,5400) KFDP(IDIM,1) 
        IF(IVAR.EQ.15) WRITE(CHTMP,5400) KFDP(IDIM,2) 
        IF(IVAR.EQ.16) WRITE(CHTMP,5400) KFDP(IDIM,3) 
        IF(IVAR.EQ.17) WRITE(CHTMP,5400) KFDP(IDIM,4) 
        IF(IVAR.EQ.18) WRITE(CHTMP,5400) KFDP(IDIM,5) 
        IF(IVAR.EQ.19) CHTMP=CHAF(IDIM) 
 
C...Length of variable, trailing decimal zeros, quotation marks. 
        LLOW=1 
        LHIG=1 
        DO 190 LL=1,12 
        IF(CHTMP(13-LL:13-LL).NE.' ') LLOW=13-LL 
        IF(CHTMP(LL:LL).NE.' ') LHIG=LL 
  190   CONTINUE 
        CHNEW=CHTMP(LLOW:LHIG)//' ' 
        LNEW=1+LHIG-LLOW 
        IF((IVAR.GE.4.AND.IVAR.LE.7).OR.IVAR.EQ.13) THEN 
          LNEW=LNEW+1 
  200     LNEW=LNEW-1 
          IF(CHNEW(LNEW:LNEW).EQ.'0') GOTO 200 
          IF(LNEW.EQ.1) CHNEW(1:2)='0.' 
          IF(LNEW.EQ.1) LNEW=2 
        ELSEIF(IVAR.EQ.19) THEN 
          DO 210 LL=LNEW,1,-1 
          IF(CHNEW(LL:LL).EQ.'''') THEN 
            CHTMP=CHNEW 
            CHNEW=CHTMP(1:LL)//''''//CHTMP(LL+1:11) 
            LNEW=LNEW+1 
          ENDIF 
  210     CONTINUE 
          CHTMP=CHNEW 
          CHNEW(1:LNEW+2)=''''//CHTMP(1:LNEW)//'''' 
          LNEW=LNEW+2 
        ENDIF 
 
C...Form composite character string, often including repetition counter. 
        IF(CHNEW.NE.CHOLD) THEN 
          NRPT=1 
          CHOLD=CHNEW 
          CHCOM=CHNEW 
          LCOM=LNEW 
        ELSE 
          LRPT=LNEW+1 
          IF(NRPT.GE.2) LRPT=LNEW+3 
          IF(NRPT.GE.10) LRPT=LNEW+4 
          IF(NRPT.GE.100) LRPT=LNEW+5 
          IF(NRPT.GE.1000) LRPT=LNEW+6 
          LLIN=LLIN-LRPT 
          NRPT=NRPT+1 
          WRITE(CHTMP,5400) NRPT 
          LRPT=1 
          IF(NRPT.GE.10) LRPT=2 
          IF(NRPT.GE.100) LRPT=3 
          IF(NRPT.GE.1000) LRPT=4 
          CHCOM(1:LRPT+1+LNEW)=CHTMP(13-LRPT:12)//'*'//CHNEW(1:LNEW) 
          LCOM=LRPT+1+LNEW 
        ENDIF 
 
C...Add characters to end of line, to new line (after storing old line), 
C...or to new block of lines (after writing old block). 
        IF(LLIN+LCOM.LE.70) THEN 
          CHLIN(LLIN+1:LLIN+LCOM+1)=CHCOM(1:LCOM)//',' 
          LLIN=LLIN+LCOM+1 
        ELSEIF(NLIN.LE.19) THEN 
          CHLIN(LLIN+1:72)=' ' 
          CHBLK(NLIN)=CHLIN 
          NLIN=NLIN+1 
          CHLIN(6:6+LCOM+1)='&'//CHCOM(1:LCOM)//',' 
          LLIN=6+LCOM+1 
        ELSE 
          CHLIN(LLIN:72)='/'//' ' 
          CHBLK(NLIN)=CHLIN 
          WRITE(CHTMP,5400) IDIM-NRPT 
          CHBLK(1)(30:33)=CHTMP(9:12) 
          DO 220 ILIN=1,NLIN 
          WRITE(LFN,5600) CHBLK(ILIN) 
  220     CONTINUE 
          NLIN=1 
          CHLIN=' ' 
          CHLIN(7:35+LCOM+1)='DATA ('//CHVAR(IVAR)//',I=    ,    )/'// 
     &    CHCOM(1:LCOM)//',' 
          WRITE(CHTMP,5400) IDIM-NRPT+1 
          CHLIN(25:28)=CHTMP(9:12) 
          LLIN=35+LCOM+1 
        ENDIF 
  230   CONTINUE 
 
C...Write final block of lines. 
        CHLIN(LLIN:72)='/'//' ' 
        CHBLK(NLIN)=CHLIN 
        WRITE(CHTMP,5400) NDIM 
        CHBLK(1)(30:33)=CHTMP(9:12) 
        DO 240 ILIN=1,NLIN 
        WRITE(LFN,5600) CHBLK(ILIN) 
  240   CONTINUE 
  250   CONTINUE 
      ENDIF 
 
C...Formats for reading and writing particle data. 
 5000 FORMAT(1X,I4,2X,A8,3I3,3F12.5,2X,F12.5,I3) 
 5100 FORMAT(5X,2I5,F12.5,5I8) 
 5200 FORMAT(A80) 
 5300 FORMAT(I4) 
 5400 FORMAT(I12) 
 5500 FORMAT(F12.5) 
 5600 FORMAT(A72) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION KLU0(I,J) 
 
C...Purpose: to provide various integer-valued event related data. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Default value. For I=0 number of entries, number of stable entries 
C...or 3 times total charge. 
      KLU0=0 
      IF(I.LT.0.OR.I.GT.MSTU(4).OR.J.LE.0) THEN 
      ELSEIF(I.EQ.0.AND.J.EQ.1) THEN 
        KLU0=N 
      ELSEIF(I.EQ.0.AND.(J.EQ.2.OR.J.EQ.6)) THEN 
        DO 100 I1=1,N 
        IF(J.EQ.2.AND.K(I1,1).GE.1.AND.K(I1,1).LE.10) KLU0=KLU0+1 
        IF(J.EQ.6.AND.K(I1,1).GE.1.AND.K(I1,1).LE.10) KLU0=KLU0+ 
     &  LUCHGE0(K(I1,2)) 
  100   CONTINUE 
      ELSEIF(I.EQ.0) THEN 
 
C...For I > 0 direct readout of K matrix or charge. 
      ELSEIF(J.LE.5) THEN 
        KLU0=K(I,J) 
      ELSEIF(J.EQ.6) THEN 
        KLU0=LUCHGE0(K(I,2)) 
 
C...Status (existing/fragmented/decayed), parton/hadron separation. 
      ELSEIF(J.LE.8) THEN 
        IF(K(I,1).GE.1.AND.K(I,1).LE.10) KLU0=1 
        IF(J.EQ.8) KLU0=KLU0*K(I,2) 
      ELSEIF(J.LE.12) THEN 
        KFA=IABS(K(I,2)) 
        KC=LUCOMP0(KFA) 
        KQ=0 
        IF(KC.NE.0) KQ=KCHG(KC,2) 
        IF(J.EQ.9.AND.KC.NE.0.AND.KQ.NE.0) KLU0=K(I,2) 
        IF(J.EQ.10.AND.KC.NE.0.AND.KQ.EQ.0) KLU0=K(I,2) 
        IF(J.EQ.11) KLU0=KC 
        IF(J.EQ.12) KLU0=KQ*ISIGN(1,K(I,2)) 
 
C...Heaviest flavour in hadron/diquark. 
      ELSEIF(J.EQ.13) THEN 
        KFA=IABS(K(I,2)) 
        KLU0=MOD(KFA/100,10)*(-1)**MOD(KFA/100,10) 
        IF(KFA.LT.10) KLU0=KFA 
        IF(MOD(KFA/1000,10).NE.0) KLU0=MOD(KFA/1000,10) 
        KLU0=KLU0*ISIGN(1,K(I,2)) 
 
C...Particle history: generation, ancestor, rank. 
      ELSEIF(J.LE.15) THEN 
        I2=I 
        I1=I 
  110   KLU0=KLU0+1 
        I2=I1 
        I1=K(I1,3) 
        IF(I1.GT.0.AND.K(I1,1).GT.0.AND.K(I1,1).LE.20) GOTO 110 
        IF(J.EQ.15) KLU0=I2 
      ELSEIF(J.EQ.16) THEN 
        KFA=IABS(K(I,2))
        IF(K(I,1).LE.20.AND.((KFA.GE.11.AND.KFA.LE.20).OR.KFA.EQ.22.OR.        
     &  (KFA.GT.100.AND.MOD(KFA/10,10).NE.0))) THEN  
          I1=I
  120     I2=I1 
          I1=K(I1,3)
          IF(I1.GT.0) THEN
            KFAM=IABS(K(I1,2))
            ILP=1
            IF(KFAM.NE.0.AND.KFAM.LE.10) ILP=0
            IF(KFAM.EQ.21.OR.KFAM.EQ.91.OR.KFAM.EQ.92.OR.KFAM.EQ.93) 
     &      ILP=0
            IF(KFAM.GT.100.AND.MOD(KFAM/10,10).EQ.0) ILP=0
            IF(ILP.EQ.1) GOTO 120
          ENDIF
          IF(K(I1,1).EQ.12) THEN
            DO 130 I3=I1+1,I2 
            IF(K(I3,3).EQ.K(I2,3).AND.K(I3,2).NE.91.AND.K(I3,2).NE.92
     &      .AND.K(I3,2).NE.93) KLU0=KLU0+1
  130       CONTINUE
          ELSE
            I3=I2
  140       KLU0=KLU0+1
            I3=I3+1
            IF(I3.LT.N.AND.K(I3,3).EQ.K(I2,3)) GOTO 140           
          ENDIF 
        ENDIF 
 
C...Particle coming from collapsing jet system or not. 
      ELSEIF(J.EQ.17) THEN 
        I1=I 
  150   KLU0=KLU0+1 
        I3=I1 
        I1=K(I1,3) 
        I0=MAX(1,I1) 
        KC=LUCOMP0(K(I0,2)) 
        IF(I1.EQ.0.OR.K(I0,1).LE.0.OR.K(I0,1).GT.20.OR.KC.EQ.0) THEN 
          IF(KLU0.EQ.1) KLU0=-1 
          IF(KLU0.GT.1) KLU0=0 
          RETURN 
        ENDIF 
        IF(KCHG(KC,2).EQ.0) GOTO 150 
        IF(K(I1,1).NE.12) KLU0=0 
        IF(K(I1,1).NE.12) RETURN 
        I2=I1 
  160   I2=I2+1 
        IF(I2.LT.N.AND.K(I2,1).NE.11) GOTO 160 
        K3M=K(I3-1,3) 
        IF(K3M.GE.I1.AND.K3M.LE.I2) KLU0=0 
        K3P=K(I3+1,3) 
        IF(I3.LT.N.AND.K3P.GE.I1.AND.K3P.LE.I2) KLU0=0 
 
C...Number of decay products. Colour flow. 
      ELSEIF(J.EQ.18) THEN 
        IF(K(I,1).EQ.11.OR.K(I,1).EQ.12) KLU0=MAX(0,K(I,5)-K(I,4)+1) 
        IF(K(I,4).EQ.0.OR.K(I,5).EQ.0) KLU0=0 
      ELSEIF(J.LE.22) THEN 
        IF(K(I,1).NE.3.AND.K(I,1).NE.13.AND.K(I,1).NE.14) RETURN 
        IF(J.EQ.19) KLU0=MOD(K(I,4)/MSTU(5),MSTU(5)) 
        IF(J.EQ.20) KLU0=MOD(K(I,5)/MSTU(5),MSTU(5)) 
        IF(J.EQ.21) KLU0=MOD(K(I,4),MSTU(5)) 
        IF(J.EQ.22) KLU0=MOD(K(I,5),MSTU(5)) 
      ELSE 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      FUNCTION PLU0(I,J) 
 
C...Purpose: to provide various real-valued event related data. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION PSUM(4) 
 
C...Set default value. For I = 0 sum of momenta or charges, 
C...or invariant mass of system. 
      PLU0=0. 
      IF(I.LT.0.OR.I.GT.MSTU(4).OR.J.LE.0) THEN 
      ELSEIF(I.EQ.0.AND.J.LE.4) THEN 
        DO 100 I1=1,N 
        IF(K(I1,1).GT.0.AND.K(I1,1).LE.10) PLU0=PLU0+P(I1,J) 
  100   CONTINUE 
      ELSEIF(I.EQ.0.AND.J.EQ.5) THEN 
        DO 120 J1=1,4 
        PSUM(J1)=0. 
        DO 110 I1=1,N 
        IF(K(I1,1).GT.0.AND.K(I1,1).LE.10) PSUM(J1)=PSUM(J1)+P(I1,J1) 
  110   CONTINUE 
  120 CONTINUE 
        PLU0=SQRT(MAX(0.,PSUM(4)**2-PSUM(1)**2-PSUM(2)**2-PSUM(3)**2)) 
      ELSEIF(I.EQ.0.AND.J.EQ.6) THEN 
        DO 130 I1=1,N 
        IF(K(I1,1).GT.0.AND.K(I1,1).LE.10) PLU0=PLU0+LUCHGE0(K(I1,2))/3. 
  130   CONTINUE 
      ELSEIF(I.EQ.0) THEN 
 
C...Direct readout of P matrix. 
      ELSEIF(J.LE.5) THEN 
        PLU0=P(I,J) 
 
C...Charge, total momentum, transverse momentum, transverse mass. 
      ELSEIF(J.LE.12) THEN 
        IF(J.EQ.6) PLU0=LUCHGE0(K(I,2))/3. 
        IF(J.EQ.7.OR.J.EQ.8) PLU0=P(I,1)**2+P(I,2)**2+P(I,3)**2 
        IF(J.EQ.9.OR.J.EQ.10) PLU0=P(I,1)**2+P(I,2)**2 
        IF(J.EQ.11.OR.J.EQ.12) PLU0=P(I,5)**2+P(I,1)**2+P(I,2)**2 
        IF(J.EQ.8.OR.J.EQ.10.OR.J.EQ.12) PLU0=SQRT(PLU0) 
 
C...Theta and phi angle in radians or degrees. 
      ELSEIF(J.LE.16) THEN 
        IF(J.LE.14) PLU0=ULANGL0(P(I,3),SQRT(P(I,1)**2+P(I,2)**2)) 
        IF(J.GE.15) PLU0=ULANGL0(P(I,1),P(I,2)) 
        IF(J.EQ.14.OR.J.EQ.16) PLU0=PLU0*180./PARU(1) 
 
C...True rapidity, rapidity with pion mass, pseudorapidity. 
      ELSEIF(J.LE.19) THEN 
        PMR=0. 
        IF(J.EQ.17) PMR=P(I,5) 
        IF(J.EQ.18) PMR=ULMASS0(211) 
        PR=MAX(1E-20,PMR**2+P(I,1)**2+P(I,2)**2) 
        PLU0=SIGN(LOG(MIN((SQRT(PR+P(I,3)**2)+ABS(P(I,3)))/SQRT(PR), 
     &  1E20)),P(I,3)) 
 
C...Energy and momentum fractions (only to be used in CM frame). 
      ELSEIF(J.LE.25) THEN 
        IF(J.EQ.20) PLU0=2.*SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2)/PARU(21) 
        IF(J.EQ.21) PLU0=2.*P(I,3)/PARU(21) 
        IF(J.EQ.22) PLU0=2.*SQRT(P(I,1)**2+P(I,2)**2)/PARU(21) 
        IF(J.EQ.23) PLU0=2.*P(I,4)/PARU(21) 
        IF(J.EQ.24) PLU0=(P(I,4)+P(I,3))/PARU(21) 
        IF(J.EQ.25) PLU0=(P(I,4)-P(I,3))/PARU(21) 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUSPHE0(SPH,APL) 
 
C...Purpose: to perform sphericity tensor analysis to give sphericity, 
C...aplanarity and the related event axes. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION SM(3,3),SV(3,3) 
 
C...Calculate matrix to be diagonalized. 
      NP=0 
      DO 110 J1=1,3 
      DO 100 J2=J1,3 
      SM(J1,J2)=0. 
  100 CONTINUE 
  110 CONTINUE 
      PS=0. 
      DO 140 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 140 
      IF(MSTU(41).GE.2) THEN 
        KC=LUCOMP0(K(I,2)) 
        IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &  KC.EQ.18) GOTO 140 
        IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &  GOTO 140 
      ENDIF 
      NP=NP+1 
      PA=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2) 
      PWT=1. 
      IF(ABS(PARU(41)-2.).GT.0.001) PWT=MAX(1E-10,PA)**(PARU(41)-2.) 
      DO 130 J1=1,3 
      DO 120 J2=J1,3 
      SM(J1,J2)=SM(J1,J2)+PWT*P(I,J1)*P(I,J2) 
  120 CONTINUE 
  130 CONTINUE 
      PS=PS+PWT*PA**2 
  140 CONTINUE 
 
C...Very low multiplicities (0 or 1) not considered. 
      IF(NP.LE.1) THEN 
        CALL LUERRM0(8,'(LUSPHE0:) too few particles for analysis') 
        SPH=-1. 
        APL=-1. 
        RETURN 
      ENDIF 
      DO 160 J1=1,3 
      DO 150 J2=J1,3 
      SM(J1,J2)=SM(J1,J2)/PS 
  150 CONTINUE 
  160 CONTINUE 
 
C...Find eigenvalues to matrix (third degree equation). 
      SQ=(SM(1,1)*SM(2,2)+SM(1,1)*SM(3,3)+SM(2,2)*SM(3,3)-SM(1,2)**2- 
     &SM(1,3)**2-SM(2,3)**2)/3.-1./9. 
      SR=-0.5*(SQ+1./9.+SM(1,1)*SM(2,3)**2+SM(2,2)*SM(1,3)**2+SM(3,3)* 
     &SM(1,2)**2-SM(1,1)*SM(2,2)*SM(3,3))+SM(1,2)*SM(1,3)*SM(2,3)+1./27. 
      SP=COS(ACOS(MAX(MIN(SR/SQRT(-SQ**3),1.),-1.))/3.) 
      P(N+1,4)=1./3.+SQRT(-SQ)*MAX(2.*SP,SQRT(3.*(1.-SP**2))-SP) 
      P(N+3,4)=1./3.+SQRT(-SQ)*MIN(2.*SP,-SQRT(3.*(1.-SP**2))-SP) 
      P(N+2,4)=1.-P(N+1,4)-P(N+3,4) 
      IF(P(N+2,4).LT.1E-5) THEN 
        CALL LUERRM0(8,'(LUSPHE0:) all particles back-to-back') 
        SPH=-1. 
        APL=-1. 
        RETURN 
      ENDIF 
 
C...Find first and last eigenvector by solving equation system. 
      DO 240 I=1,3,2 
      DO 180 J1=1,3 
      SV(J1,J1)=SM(J1,J1)-P(N+I,4) 
      DO 170 J2=J1+1,3 
      SV(J1,J2)=SM(J1,J2) 
      SV(J2,J1)=SM(J1,J2) 
  170 CONTINUE 
  180 CONTINUE 
      SMAX=0. 
      DO 200 J1=1,3 
      DO 190 J2=1,3 
      IF(ABS(SV(J1,J2)).LE.SMAX) GOTO 190 
      JA=J1 
      JB=J2 
      SMAX=ABS(SV(J1,J2)) 
  190 CONTINUE 
  200 CONTINUE 
      SMAX=0. 
      DO 220 J3=JA+1,JA+2 
      J1=J3-3*((J3-1)/3) 
      RL=SV(J1,JB)/SV(JA,JB) 
      DO 210 J2=1,3 
      SV(J1,J2)=SV(J1,J2)-RL*SV(JA,J2) 
      IF(ABS(SV(J1,J2)).LE.SMAX) GOTO 210 
      JC=J1 
      SMAX=ABS(SV(J1,J2)) 
  210 CONTINUE 
  220 CONTINUE 
      JB1=JB+1-3*(JB/3) 
      JB2=JB+2-3*((JB+1)/3) 
      P(N+I,JB1)=-SV(JC,JB2) 
      P(N+I,JB2)=SV(JC,JB1) 
      P(N+I,JB)=-(SV(JA,JB1)*P(N+I,JB1)+SV(JA,JB2)*P(N+I,JB2))/ 
     &SV(JA,JB) 
      PA=SQRT(P(N+I,1)**2+P(N+I,2)**2+P(N+I,3)**2) 
      SGN=(-1.)**INT(RLU0(0)+0.5) 
      DO 230 J=1,3 
      P(N+I,J)=SGN*P(N+I,J)/PA 
  230 CONTINUE 
  240 CONTINUE 
 
C...Middle axis orthogonal to other two. Fill other codes. 
      SGN=(-1.)**INT(RLU0(0)+0.5) 
      P(N+2,1)=SGN*(P(N+1,2)*P(N+3,3)-P(N+1,3)*P(N+3,2)) 
      P(N+2,2)=SGN*(P(N+1,3)*P(N+3,1)-P(N+1,1)*P(N+3,3)) 
      P(N+2,3)=SGN*(P(N+1,1)*P(N+3,2)-P(N+1,2)*P(N+3,1)) 
      DO 260 I=1,3 
      K(N+I,1)=31 
      K(N+I,2)=95 
      K(N+I,3)=I 
      K(N+I,4)=0 
      K(N+I,5)=0 
      P(N+I,5)=0. 
      DO 250 J=1,5 
      V(I,J)=0. 
  250 CONTINUE 
  260 CONTINUE 
 
C...Calculate sphericity and aplanarity. Select storing option. 
      SPH=1.5*(P(N+2,4)+P(N+3,4)) 
      APL=1.5*P(N+3,4) 
      MSTU(61)=N+1 
      MSTU(62)=NP 
      IF(MSTU(43).LE.1) MSTU(3)=3 
      IF(MSTU(43).GE.2) N=N+3 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUTHRU0(THR,OBL) 
 
C...Purpose: to perform thrust analysis to give thrust, oblateness 
C...and the related event axes. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION TDI(3),TPR(3) 
 
C...Take copy of particles that are to be considered in thrust analysis. 
      NP=0 
      PS=0. 
      DO 100 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 100 
      IF(MSTU(41).GE.2) THEN 
        KC=LUCOMP0(K(I,2)) 
        IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &  KC.EQ.18) GOTO 100 
        IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &  GOTO 100 
      ENDIF 
      IF(N+NP+MSTU(44)+15.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUTHRU0:) no more memory left in LUJETS') 
        THR=-2. 
        OBL=-2. 
        RETURN 
      ENDIF 
      NP=NP+1 
      K(N+NP,1)=23 
      P(N+NP,1)=P(I,1) 
      P(N+NP,2)=P(I,2) 
      P(N+NP,3)=P(I,3) 
      P(N+NP,4)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2) 
      P(N+NP,5)=1. 
      IF(ABS(PARU(42)-1.).GT.0.001) P(N+NP,5)=P(N+NP,4)**(PARU(42)-1.) 
      PS=PS+P(N+NP,4)*P(N+NP,5) 
  100 CONTINUE 
 
C...Very low multiplicities (0 or 1) not considered. 
      IF(NP.LE.1) THEN 
        CALL LUERRM0(8,'(LUTHRU0:) too few particles for analysis') 
        THR=-1. 
        OBL=-1. 
        RETURN 
      ENDIF 
 
C...Loop over thrust and major. T axis along z direction in latter case. 
      DO 320 ILD=1,2 
      IF(ILD.EQ.2) THEN 
        K(N+NP+1,1)=31 
        PHI=ULANGL0(P(N+NP+1,1),P(N+NP+1,2)) 
        MSTU(33)=1 
        CALL LUDBRB0(N+1,N+NP+1,0.,-PHI,0D0,0D0,0D0) 
        THE=ULANGL0(P(N+NP+1,3),P(N+NP+1,1)) 
        CALL LUDBRB0(N+1,N+NP+1,-THE,0.,0D0,0D0,0D0) 
      ENDIF 
 
C...Find and order particles with highest p (pT for major). 
      DO 110 ILF=N+NP+4,N+NP+MSTU(44)+4 
      P(ILF,4)=0. 
  110 CONTINUE 
      DO 160 I=N+1,N+NP 
      IF(ILD.EQ.2) P(I,4)=SQRT(P(I,1)**2+P(I,2)**2) 
      DO 130 ILF=N+NP+MSTU(44)+3,N+NP+4,-1 
      IF(P(I,4).LE.P(ILF,4)) GOTO 140 
      DO 120 J=1,5 
      P(ILF+1,J)=P(ILF,J) 
  120 CONTINUE 
  130 CONTINUE 
      ILF=N+NP+3 
  140 DO 150 J=1,5 
      P(ILF+1,J)=P(I,J) 
  150 CONTINUE 
  160 CONTINUE 
 
C...Find and order initial axes with highest thrust (major). 
      DO 170 ILG=N+NP+MSTU(44)+5,N+NP+MSTU(44)+15 
      P(ILG,4)=0. 
  170 CONTINUE 
      NC=2**(MIN(MSTU(44),NP)-1) 
      DO 250 ILC=1,NC 
      DO 180 J=1,3 
      TDI(J)=0. 
  180 CONTINUE 
      DO 200 ILF=1,MIN(MSTU(44),NP) 
      SGN=P(N+NP+ILF+3,5) 
      IF(2**ILF*((ILC+2**(ILF-1)-1)/2**ILF).GE.ILC) SGN=-SGN 
      DO 190 J=1,4-ILD 
      TDI(J)=TDI(J)+SGN*P(N+NP+ILF+3,J) 
  190 CONTINUE 
  200 CONTINUE 
      TDS=TDI(1)**2+TDI(2)**2+TDI(3)**2 
      DO 220 ILG=N+NP+MSTU(44)+MIN(ILC,10)+4,N+NP+MSTU(44)+5,-1 
      IF(TDS.LE.P(ILG,4)) GOTO 230 
      DO 210 J=1,4 
      P(ILG+1,J)=P(ILG,J) 
  210 CONTINUE 
  220 CONTINUE 
      ILG=N+NP+MSTU(44)+4 
  230 DO 240 J=1,3 
      P(ILG+1,J)=TDI(J) 
  240 CONTINUE 
      P(ILG+1,4)=TDS 
  250 CONTINUE 
 
C...Iterate direction of axis until stable maximum. 
      P(N+NP+ILD,4)=0. 
      ILG=0 
  260 ILG=ILG+1 
      THP=0. 
  270 THPS=THP 
      DO 280 J=1,3 
      IF(THP.LE.1E-10) TDI(J)=P(N+NP+MSTU(44)+4+ILG,J) 
      IF(THP.GT.1E-10) TDI(J)=TPR(J) 
      TPR(J)=0. 
  280 CONTINUE 
      DO 300 I=N+1,N+NP 
      SGN=SIGN(P(I,5),TDI(1)*P(I,1)+TDI(2)*P(I,2)+TDI(3)*P(I,3)) 
      DO 290 J=1,4-ILD 
      TPR(J)=TPR(J)+SGN*P(I,J) 
  290 CONTINUE 
  300 CONTINUE 
      THP=SQRT(TPR(1)**2+TPR(2)**2+TPR(3)**2)/PS 
      IF(THP.GE.THPS+PARU(48)) GOTO 270 
 
C...Save good axis. Try new initial axis until a number of tries agree. 
      IF(THP.LT.P(N+NP+ILD,4)-PARU(48).AND.ILG.LT.MIN(10,NC)) GOTO 260 
      IF(THP.GT.P(N+NP+ILD,4)+PARU(48)) THEN 
        IAGR=0 
        SGN=(-1.)**INT(RLU0(0)+0.5) 
        DO 310 J=1,3 
        P(N+NP+ILD,J)=SGN*TPR(J)/(PS*THP) 
  310   CONTINUE 
        P(N+NP+ILD,4)=THP 
        P(N+NP+ILD,5)=0. 
      ENDIF 
      IAGR=IAGR+1 
      IF(IAGR.LT.MSTU(45).AND.ILG.LT.MIN(10,NC)) GOTO 260 
  320 CONTINUE 
 
C...Find minor axis and value by orthogonality. 
      SGN=(-1.)**INT(RLU0(0)+0.5) 
      P(N+NP+3,1)=-SGN*P(N+NP+2,2) 
      P(N+NP+3,2)=SGN*P(N+NP+2,1) 
      P(N+NP+3,3)=0. 
      THP=0. 
      DO 330 I=N+1,N+NP 
      THP=THP+P(I,5)*ABS(P(N+NP+3,1)*P(I,1)+P(N+NP+3,2)*P(I,2)) 
  330 CONTINUE 
      P(N+NP+3,4)=THP/PS 
      P(N+NP+3,5)=0. 
 
C...Fill axis information. Rotate back to original coordinate system. 
      DO 350 ILD=1,3 
      K(N+ILD,1)=31 
      K(N+ILD,2)=96 
      K(N+ILD,3)=ILD 
      K(N+ILD,4)=0 
      K(N+ILD,5)=0 
      DO 340 J=1,5 
      P(N+ILD,J)=P(N+NP+ILD,J) 
      V(N+ILD,J)=0. 
  340 CONTINUE 
  350 CONTINUE 
      CALL LUDBRB0(N+1,N+3,THE,PHI,0D0,0D0,0D0) 
 
C...Calculate thrust and oblateness. Select storing option. 
      THR=P(N+1,4) 
      OBL=P(N+2,4)-P(N+3,4) 
      MSTU(61)=N+1 
      MSTU(62)=NP 
      IF(MSTU(43).LE.1) MSTU(3)=3 
      IF(MSTU(43).GE.2) N=N+3 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUCLUS0(NJET) 
 
C...Purpose: to subdivide the particle content of an event into 
C...jets/clusters. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION PS(5) 
      SAVE NSAV,NP,PS,PSS,RINIT,NPRE,NREM 
 
C...Functions: distance measure in pT, (pseudo)mass or Durham pT. 
      R2T(I1,I2)=(P(I1,5)*P(I2,5)-P(I1,1)*P(I2,1)-P(I1,2)*P(I2,2)- 
     &P(I1,3)*P(I2,3))*2.*P(I1,5)*P(I2,5)/(0.0001+P(I1,5)+P(I2,5))**2 
      R2M(I1,I2)=2.*P(I1,4)*P(I2,4)*(1.-(P(I1,1)*P(I2,1)+P(I1,2)* 
     &P(I2,2)+P(I1,3)*P(I2,3))/(P(I1,5)*P(I2,5))) 
      R2D(I1,I2)=2.*MIN(P(I1,4),P(I2,4))**2*(1.-(P(I1,1)*P(I2,1)+
     &P(I1,2)*P(I2,2)+P(I1,3)*P(I2,3))/(P(I1,5)*P(I2,5))) 
 
C...If first time, reset. If reentering, skip preliminaries. 
      IF(MSTU(48).LE.0) THEN 
        NP=0 
        DO 100 J=1,5 
        PS(J)=0. 
  100   CONTINUE 
        PSS=0. 
      ELSE 
        NJET=NSAV 
        IF(MSTU(43).GE.2) N=N-NJET 
        DO 110 I=N+1,N+NJET 
        P(I,5)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2) 
  110   CONTINUE 
        IF(MSTU(46).LE.3.OR.MSTU(46).EQ.5) THEN 
          R2ACC=PARU(44)**2 
        ELSE 
          R2ACC=PARU(45)*PS(5)**2
        ENDIF 
        NLOOP=0 
        GOTO 300 
      ENDIF 
 
C...Find which particles are to be considered in cluster search. 
      DO 140 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 140 
      IF(MSTU(41).GE.2) THEN 
        KC=LUCOMP0(K(I,2)) 
        IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &  KC.EQ.18) GOTO 140 
        IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &  GOTO 140 
      ENDIF 
      IF(N+2*NP.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUCLUS0:) no more memory left in LUJETS') 
        NJET=-1 
        RETURN 
      ENDIF 
 
C...Take copy of these particles, with space left for jets later on. 
      NP=NP+1 
      K(N+NP,3)=I 
      DO 120 J=1,5 
      P(N+NP,J)=P(I,J) 
  120 CONTINUE 
      IF(MSTU(42).EQ.0) P(N+NP,5)=0. 
      IF(MSTU(42).EQ.1.AND.K(I,2).NE.22) P(N+NP,5)=PMAS(101,1) 
      P(N+NP,4)=SQRT(P(N+NP,5)**2+P(I,1)**2+P(I,2)**2+P(I,3)**2) 
      P(N+NP,5)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2) 
      DO 130 J=1,4 
      PS(J)=PS(J)+P(N+NP,J) 
  130 CONTINUE 
      PSS=PSS+P(N+NP,5) 
  140 CONTINUE 
      DO 160 I=N+1,N+NP 
      K(I+NP,3)=K(I,3) 
      DO 150 J=1,5 
      P(I+NP,J)=P(I,J) 
  150 CONTINUE 
  160 CONTINUE 
      PS(5)=SQRT(MAX(0.,PS(4)**2-PS(1)**2-PS(2)**2-PS(3)**2)) 
 
C...Very low multiplicities not considered. 
      IF(NP.LT.MSTU(47)) THEN 
        CALL LUERRM0(8,'(LUCLUS0:) too few particles for analysis') 
        NJET=-1 
        RETURN 
      ENDIF 
 
C...Find precluster configuration. If too few jets, make harder cuts. 
      NLOOP=0 
      IF(MSTU(46).LE.3.OR.MSTU(46).EQ.5) THEN 
        R2ACC=PARU(44)**2 
      ELSE 
        R2ACC=PARU(45)*PS(5)**2 
      ENDIF 
      RINIT=1.25*PARU(43) 
      IF(NP.LE.MSTU(47)+2) RINIT=0. 
  170 RINIT=0.8*RINIT 
      NPRE=0 
      NREM=NP 
      DO 180 I=N+NP+1,N+2*NP 
      K(I,4)=0 
  180 CONTINUE 
 
C...Sum up small momentum region. Jet if enough absolute momentum. 
      IF(MSTU(46).LE.2) THEN 
        DO 190 J=1,4 
        P(N+1,J)=0. 
  190   CONTINUE 
        DO 210 I=N+NP+1,N+2*NP 
        IF(P(I,5).GT.2.*RINIT) GOTO 210 
        NREM=NREM-1 
        K(I,4)=1 
        DO 200 J=1,4 
        P(N+1,J)=P(N+1,J)+P(I,J) 
  200   CONTINUE 
  210   CONTINUE 
        P(N+1,5)=SQRT(P(N+1,1)**2+P(N+1,2)**2+P(N+1,3)**2) 
        IF(P(N+1,5).GT.2.*RINIT) NPRE=1 
        IF(RINIT.GE.0.2*PARU(43).AND.NPRE+NREM.LT.MSTU(47)) GOTO 170 
        IF(NREM.EQ.0) GOTO 170 
      ENDIF 
 
C...Find fastest remaining particle. 
  220 NPRE=NPRE+1 
      PMAX=0. 
      DO 230 I=N+NP+1,N+2*NP 
      IF(K(I,4).NE.0.OR.P(I,5).LE.PMAX) GOTO 230 
      IMAX=I 
      PMAX=P(I,5) 
  230 CONTINUE 
      DO 240 J=1,5 
      P(N+NPRE,J)=P(IMAX,J) 
  240 CONTINUE 
      NREM=NREM-1 
      K(IMAX,4)=NPRE 
 
C...Sum up precluster around it according to pT separation. 
      IF(MSTU(46).LE.2) THEN 
        DO 260 I=N+NP+1,N+2*NP 
        IF(K(I,4).NE.0) GOTO 260 
        R2=R2T(I,IMAX) 
        IF(R2.GT.RINIT**2) GOTO 260 
        NREM=NREM-1 
        K(I,4)=NPRE 
        DO 250 J=1,4 
        P(N+NPRE,J)=P(N+NPRE,J)+P(I,J) 
  250   CONTINUE 
  260   CONTINUE 
        P(N+NPRE,5)=SQRT(P(N+NPRE,1)**2+P(N+NPRE,2)**2+P(N+NPRE,3)**2) 
 
C...Sum up precluster around it according to mass or 
C...Durham pT separation. 
      ELSE 
  270   IMIN=0 
        R2MIN=RINIT**2 
        DO 280 I=N+NP+1,N+2*NP 
        IF(K(I,4).NE.0) GOTO 280
        IF(MSTU(46).LE.4) THEN 
          R2=R2M(I,N+NPRE) 
        ELSE
          R2=R2D(I,N+NPRE) 
        ENDIF
        IF(R2.GE.R2MIN) GOTO 280 
        IMIN=I 
        R2MIN=R2 
  280   CONTINUE 
        IF(IMIN.NE.0) THEN 
          DO 290 J=1,4 
          P(N+NPRE,J)=P(N+NPRE,J)+P(IMIN,J) 
  290     CONTINUE 
          P(N+NPRE,5)=SQRT(P(N+NPRE,1)**2+P(N+NPRE,2)**2+P(N+NPRE,3)**2) 
          NREM=NREM-1 
          K(IMIN,4)=NPRE 
          GOTO 270 
        ENDIF 
      ENDIF 
 
C...Check if more preclusters to be found. Start over if too few. 
      IF(RINIT.GE.0.2*PARU(43).AND.NPRE+NREM.LT.MSTU(47)) GOTO 170 
      IF(NREM.GT.0) GOTO 220 
      NJET=NPRE 
 
C...Reassign all particles to nearest jet. Sum up new jet momenta. 
  300 TSAV=0. 
      PSJT=0. 
  310 IF(MSTU(46).LE.1) THEN 
        DO 330 I=N+1,N+NJET 
        DO 320 J=1,4 
        V(I,J)=0. 
  320   CONTINUE 
  330 CONTINUE 
        DO 360 I=N+NP+1,N+2*NP 
        R2MIN=PSS**2 
        DO 340 IJET=N+1,N+NJET 
        IF(P(IJET,5).LT.RINIT) GOTO 340 
        R2=R2T(I,IJET) 
        IF(R2.GE.R2MIN) GOTO 340 
        IMIN=IJET 
        R2MIN=R2 
  340   CONTINUE 
        K(I,4)=IMIN-N 
        DO 350 J=1,4 
        V(IMIN,J)=V(IMIN,J)+P(I,J) 
  350   CONTINUE 
  360   CONTINUE 
        PSJT=0. 
        DO 380 I=N+1,N+NJET 
        DO 370 J=1,4 
        P(I,J)=V(I,J) 
  370   CONTINUE 
        P(I,5)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2) 
        PSJT=PSJT+P(I,5) 
  380   CONTINUE 
      ENDIF 
 
C...Find two closest jets. 
      R2MIN=2.*MAX(R2ACC,PS(5)**2) 
      DO 400 ITRY1=N+1,N+NJET-1 
      DO 390 ITRY2=ITRY1+1,N+NJET 
      IF(MSTU(46).LE.2) THEN 
        R2=R2T(ITRY1,ITRY2) 
      ELSEIF(MSTU(46).LE.4) THEN
        R2=R2M(ITRY1,ITRY2)
      ELSE
        R2=R2D(ITRY1,ITRY2)
      ENDIF 
      IF(R2.GE.R2MIN) GOTO 390 
      IMIN1=ITRY1 
      IMIN2=ITRY2 
      R2MIN=R2 
  390 CONTINUE 
  400 CONTINUE 
 
C...If allowed, join two closest jets and start over. 
      IF(NJET.GT.MSTU(47).AND.R2MIN.LT.R2ACC) THEN 
        IREC=MIN(IMIN1,IMIN2) 
        IDEL=MAX(IMIN1,IMIN2) 
        DO 410 J=1,4 
        P(IREC,J)=P(IMIN1,J)+P(IMIN2,J) 
  410   CONTINUE 
        P(IREC,5)=SQRT(P(IREC,1)**2+P(IREC,2)**2+P(IREC,3)**2) 
        DO 430 I=IDEL+1,N+NJET 
        DO 420 J=1,5 
        P(I-1,J)=P(I,J) 
  420   CONTINUE 
  430 CONTINUE 
        IF(MSTU(46).GE.2) THEN 
          DO 440 I=N+NP+1,N+2*NP 
          IORI=N+K(I,4) 
          IF(IORI.EQ.IDEL) K(I,4)=IREC-N 
          IF(IORI.GT.IDEL) K(I,4)=K(I,4)-1 
  440     CONTINUE 
        ENDIF 
        NJET=NJET-1 
        GOTO 300 
 
C...Divide up broad jet if empty cluster in list of final ones. 
      ELSEIF(NJET.EQ.MSTU(47).AND.MSTU(46).LE.1.AND.NLOOP.LE.2) THEN 
        DO 450 I=N+1,N+NJET 
        K(I,5)=0 
  450   CONTINUE 
        DO 460 I=N+NP+1,N+2*NP 
        K(N+K(I,4),5)=K(N+K(I,4),5)+1 
  460   CONTINUE 
        IEMP=0 
        DO 470 I=N+1,N+NJET 
        IF(K(I,5).EQ.0) IEMP=I 
  470   CONTINUE 
        IF(IEMP.NE.0) THEN 
          NLOOP=NLOOP+1 
          ISPL=0 
          R2MAX=0. 
          DO 480 I=N+NP+1,N+2*NP 
          IF(K(N+K(I,4),5).LE.1.OR.P(I,5).LT.RINIT) GOTO 480 
          IJET=N+K(I,4) 
          R2=R2T(I,IJET) 
          IF(R2.LE.R2MAX) GOTO 480 
          ISPL=I 
          R2MAX=R2 
  480     CONTINUE 
          IF(ISPL.NE.0) THEN 
            IJET=N+K(ISPL,4) 
            DO 490 J=1,4 
            P(IEMP,J)=P(ISPL,J) 
            P(IJET,J)=P(IJET,J)-P(ISPL,J) 
  490       CONTINUE 
            P(IEMP,5)=P(ISPL,5) 
            P(IJET,5)=SQRT(P(IJET,1)**2+P(IJET,2)**2+P(IJET,3)**2) 
            IF(NLOOP.LE.2) GOTO 300 
          ENDIF 
        ENDIF 
      ENDIF 
 
C...If generalized thrust has not yet converged, continue iteration. 
      IF(MSTU(46).LE.1.AND.NLOOP.LE.2.AND.PSJT/PSS.GT.TSAV+PARU(48)) 
     &THEN 
        TSAV=PSJT/PSS 
        GOTO 310 
      ENDIF 
 
C...Reorder jets according to energy. 
      DO 510 I=N+1,N+NJET 
      DO 500 J=1,5 
      V(I,J)=P(I,J) 
  500 CONTINUE 
  510 CONTINUE 
      DO 540 INEW=N+1,N+NJET 
      PEMAX=0. 
      DO 520 ITRY=N+1,N+NJET 
      IF(V(ITRY,4).LE.PEMAX) GOTO 520 
      IMAX=ITRY 
      PEMAX=V(ITRY,4) 
  520 CONTINUE 
      K(INEW,1)=31 
      K(INEW,2)=97 
      K(INEW,3)=INEW-N 
      K(INEW,4)=0 
      DO 530 J=1,5 
      P(INEW,J)=V(IMAX,J) 
  530 CONTINUE 
      V(IMAX,4)=-1. 
      K(IMAX,5)=INEW 
  540 CONTINUE 
 
C...Clean up particle-jet assignments and jet information. 
      DO 550 I=N+NP+1,N+2*NP 
      IORI=K(N+K(I,4),5) 
      K(I,4)=IORI-N 
      IF(K(K(I,3),1).NE.3) K(K(I,3),4)=IORI-N 
      K(IORI,4)=K(IORI,4)+1 
  550 CONTINUE 
      IEMP=0 
      PSJT=0. 
      DO 570 I=N+1,N+NJET 
      K(I,5)=0 
      PSJT=PSJT+P(I,5) 
      P(I,5)=SQRT(MAX(P(I,4)**2-P(I,5)**2,0.)) 
      DO 560 J=1,5 
      V(I,J)=0. 
  560 CONTINUE 
      IF(K(I,4).EQ.0) IEMP=I 
  570 CONTINUE 
 
C...Select storing option. Output variables. Check for failure. 
      MSTU(61)=N+1 
      MSTU(62)=NP 
      MSTU(63)=NPRE 
      PARU(61)=PS(5) 
      PARU(62)=PSJT/PSS 
      PARU(63)=SQRT(R2MIN) 
      IF(NJET.LE.1) PARU(63)=0. 
      IF(IEMP.NE.0) THEN 
        CALL LUERRM0(8,'(LUCLUS0:) failed to reconstruct as requested') 
        NJET=-1 
      ENDIF 
      IF(MSTU(43).LE.1) MSTU(3)=NJET 
      IF(MSTU(43).GE.2) N=N+NJET 
      NSAV=NJET 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUCELL0(NJET) 
 
C...Purpose: to provide a simple way of jet finding in an eta-phi-ET 
C...coordinate frame, as used for calorimeters at hadron colliders. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Loop over all particles. Find cell that was hit by given particle. 
      PTLRAT=1./SINH(PARU(51))**2 
      NP=0 
      NC=N 
      DO 110 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 110 
      IF(P(I,1)**2+P(I,2)**2.LE.PTLRAT*P(I,3)**2) GOTO 110 
      IF(MSTU(41).GE.2) THEN 
        KC=LUCOMP0(K(I,2)) 
        IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &  KC.EQ.18) GOTO 110 
        IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &  GOTO 110 
      ENDIF 
      NP=NP+1 
      PT=SQRT(P(I,1)**2+P(I,2)**2) 
      ETA=SIGN(LOG((SQRT(PT**2+P(I,3)**2)+ABS(P(I,3)))/PT),P(I,3)) 
      IETA=MAX(1,MIN(MSTU(51),1+INT(MSTU(51)*0.5*(ETA/PARU(51)+1.)))) 
      PHI=ULANGL0(P(I,1),P(I,2)) 
      IPHI=MAX(1,MIN(MSTU(52),1+INT(MSTU(52)*0.5*(PHI/PARU(1)+1.)))) 
      IETPH=MSTU(52)*IETA+IPHI 
 
C...Add to cell already hit, or book new cell. 
      DO 100 IC=N+1,NC 
      IF(IETPH.EQ.K(IC,3)) THEN 
        K(IC,4)=K(IC,4)+1 
        P(IC,5)=P(IC,5)+PT 
        GOTO 110 
      ENDIF 
  100 CONTINUE 
      IF(NC.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUCELL0:) no more memory left in LUJETS') 
        NJET=-2 
        RETURN 
      ENDIF 
      NC=NC+1 
      K(NC,3)=IETPH 
      K(NC,4)=1 
      K(NC,5)=2 
      P(NC,1)=(PARU(51)/MSTU(51))*(2*IETA-1-MSTU(51)) 
      P(NC,2)=(PARU(1)/MSTU(52))*(2*IPHI-1-MSTU(52)) 
      P(NC,5)=PT 
  110 CONTINUE 
 
C...Smear true bin content by calorimeter resolution. 
      IF(MSTU(53).GE.1) THEN 
        DO 130 IC=N+1,NC 
        PEI=P(IC,5) 
        IF(MSTU(53).EQ.2) PEI=P(IC,5)*COSH(P(IC,1)) 
  120   PEF=PEI+PARU(55)*SQRT(-2.*LOG(MAX(1E-10,RLU0(0)))*PEI)* 
     &  COS(PARU(2)*RLU0(0)) 
        IF(PEF.LT.0..OR.PEF.GT.PARU(56)*PEI) GOTO 120 
        P(IC,5)=PEF 
        IF(MSTU(53).EQ.2) P(IC,5)=PEF/COSH(P(IC,1)) 
  130   CONTINUE 
      ENDIF 
 
C...Remove cells below threshold. 
      IF(PARU(58).GT.0.) THEN 
        NCC=NC 
        NC=N 
        DO 140 IC=N+1,NCC 
        IF(P(IC,5).GT.PARU(58)) THEN 
          NC=NC+1 
          K(NC,3)=K(IC,3) 
          K(NC,4)=K(IC,4) 
          K(NC,5)=K(IC,5) 
          P(NC,1)=P(IC,1) 
          P(NC,2)=P(IC,2) 
          P(NC,5)=P(IC,5) 
        ENDIF 
  140   CONTINUE 
      ENDIF 
 
C...Find initiator cell: the one with highest pT of not yet used ones. 
      NJ=NC 
  150 ETMAX=0. 
      DO 160 IC=N+1,NC 
      IF(K(IC,5).NE.2) GOTO 160 
      IF(P(IC,5).LE.ETMAX) GOTO 160 
      ICMAX=IC 
      ETA=P(IC,1) 
      PHI=P(IC,2) 
      ETMAX=P(IC,5) 
  160 CONTINUE 
      IF(ETMAX.LT.PARU(52)) GOTO 220 
      IF(NJ.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUCELL0:) no more memory left in LUJETS') 
        NJET=-2 
        RETURN 
      ENDIF 
      K(ICMAX,5)=1 
      NJ=NJ+1 
      K(NJ,4)=0 
      K(NJ,5)=1 
      P(NJ,1)=ETA 
      P(NJ,2)=PHI 
      P(NJ,3)=0. 
      P(NJ,4)=0. 
      P(NJ,5)=0. 
 
C...Sum up unused cells within required distance of initiator. 
      DO 170 IC=N+1,NC 
      IF(K(IC,5).EQ.0) GOTO 170 
      IF(ABS(P(IC,1)-ETA).GT.PARU(54)) GOTO 170 
      DPHIA=ABS(P(IC,2)-PHI) 
      IF(DPHIA.GT.PARU(54).AND.DPHIA.LT.PARU(2)-PARU(54)) GOTO 170 
      PHIC=P(IC,2) 
      IF(DPHIA.GT.PARU(1)) PHIC=PHIC+SIGN(PARU(2),PHI) 
      IF((P(IC,1)-ETA)**2+(PHIC-PHI)**2.GT.PARU(54)**2) GOTO 170 
      K(IC,5)=-K(IC,5) 
      K(NJ,4)=K(NJ,4)+K(IC,4) 
      P(NJ,3)=P(NJ,3)+P(IC,5)*P(IC,1) 
      P(NJ,4)=P(NJ,4)+P(IC,5)*PHIC 
      P(NJ,5)=P(NJ,5)+P(IC,5) 
  170 CONTINUE 
 
C...Reject cluster below minimum ET, else accept. 
      IF(P(NJ,5).LT.PARU(53)) THEN 
        NJ=NJ-1 
        DO 180 IC=N+1,NC 
        IF(K(IC,5).LT.0) K(IC,5)=-K(IC,5) 
  180   CONTINUE 
      ELSEIF(MSTU(54).LE.2) THEN 
        P(NJ,3)=P(NJ,3)/P(NJ,5) 
        P(NJ,4)=P(NJ,4)/P(NJ,5) 
        IF(ABS(P(NJ,4)).GT.PARU(1)) P(NJ,4)=P(NJ,4)-SIGN(PARU(2), 
     &  P(NJ,4)) 
        DO 190 IC=N+1,NC 
        IF(K(IC,5).LT.0) K(IC,5)=0 
  190   CONTINUE 
      ELSE 
        DO 200 J=1,4 
        P(NJ,J)=0. 
  200   CONTINUE 
        DO 210 IC=N+1,NC 
        IF(K(IC,5).GE.0) GOTO 210 
        P(NJ,1)=P(NJ,1)+P(IC,5)*COS(P(IC,2)) 
        P(NJ,2)=P(NJ,2)+P(IC,5)*SIN(P(IC,2)) 
        P(NJ,3)=P(NJ,3)+P(IC,5)*SINH(P(IC,1)) 
        P(NJ,4)=P(NJ,4)+P(IC,5)*COSH(P(IC,1)) 
        K(IC,5)=0 
  210   CONTINUE 
      ENDIF 
      GOTO 150 
 
C...Arrange clusters in falling ET sequence. 
  220 DO 250 I=1,NJ-NC 
      ETMAX=0. 
      DO 230 IJ=NC+1,NJ 
      IF(K(IJ,5).EQ.0) GOTO 230 
      IF(P(IJ,5).LT.ETMAX) GOTO 230 
      IJMAX=IJ 
      ETMAX=P(IJ,5) 
  230 CONTINUE 
      K(IJMAX,5)=0 
      K(N+I,1)=31 
      K(N+I,2)=98 
      K(N+I,3)=I 
      K(N+I,4)=K(IJMAX,4) 
      K(N+I,5)=0 
      DO 240 J=1,5 
      P(N+I,J)=P(IJMAX,J) 
      V(N+I,J)=0. 
  240 CONTINUE 
  250 CONTINUE 
      NJET=NJ-NC 
 
C...Convert to massless or massive four-vectors. 
      IF(MSTU(54).EQ.2) THEN 
        DO 260 I=N+1,N+NJET 
        ETA=P(I,3) 
        P(I,1)=P(I,5)*COS(P(I,4)) 
        P(I,2)=P(I,5)*SIN(P(I,4)) 
        P(I,3)=P(I,5)*SINH(ETA) 
        P(I,4)=P(I,5)*COSH(ETA) 
        P(I,5)=0. 
  260   CONTINUE 
      ELSEIF(MSTU(54).GE.3) THEN 
        DO 270 I=N+1,N+NJET 
        P(I,5)=SQRT(MAX(0.,P(I,4)**2-P(I,1)**2-P(I,2)**2-P(I,3)**2)) 
  270   CONTINUE 
      ENDIF 
 
C...Information about storage. 
      MSTU(61)=N+1 
      MSTU(62)=NP 
      MSTU(63)=NC-N 
      IF(MSTU(43).LE.1) MSTU(3)=NJET 
      IF(MSTU(43).GE.2) N=N+NJET 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUJMAS0(PMH,PML) 
 
C...Purpose: to determine, approximately, the two jet masses that 
C...minimize the sum m_H^2 + m_L^2, a la Clavelli and Wyler. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
      DIMENSION SM(3,3),SAX(3),PS(3,5) 
 
C...Reset. 
      NP=0 
      DO 120 J1=1,3 
      DO 100 J2=J1,3 
      SM(J1,J2)=0. 
  100 CONTINUE 
      DO 110 J2=1,4 
      PS(J1,J2)=0. 
  110 CONTINUE 
  120 CONTINUE 
      PSS=0. 
 
C...Take copy of particles that are to be considered in mass analysis. 
      DO 170 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 170 
      IF(MSTU(41).GE.2) THEN 
        KC=LUCOMP0(K(I,2)) 
        IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &  KC.EQ.18) GOTO 170 
        IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &  GOTO 170 
      ENDIF 
      IF(N+NP+1.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUJMAS0:) no more memory left in LUJETS') 
        PMH=-2. 
        PML=-2. 
        RETURN 
      ENDIF 
      NP=NP+1 
      DO 130 J=1,5 
      P(N+NP,J)=P(I,J) 
  130 CONTINUE 
      IF(MSTU(42).EQ.0) P(N+NP,5)=0. 
      IF(MSTU(42).EQ.1.AND.K(I,2).NE.22) P(N+NP,5)=PMAS(101,1) 
      P(N+NP,4)=SQRT(P(N+NP,5)**2+P(I,1)**2+P(I,2)**2+P(I,3)**2) 
 
C...Fill information in sphericity tensor and total momentum vector. 
      DO 150 J1=1,3 
      DO 140 J2=J1,3 
      SM(J1,J2)=SM(J1,J2)+P(I,J1)*P(I,J2) 
  140 CONTINUE 
  150 CONTINUE 
      PSS=PSS+(P(I,1)**2+P(I,2)**2+P(I,3)**2) 
      DO 160 J=1,4 
      PS(3,J)=PS(3,J)+P(N+NP,J) 
  160 CONTINUE 
  170 CONTINUE 
 
C...Very low multiplicities (0 or 1) not considered. 
      IF(NP.LE.1) THEN 
        CALL LUERRM0(8,'(LUJMAS0:) too few particles for analysis') 
        PMH=-1. 
        PML=-1. 
        RETURN 
      ENDIF 
      PARU(61)=SQRT(MAX(0.,PS(3,4)**2-PS(3,1)**2-PS(3,2)**2-PS(3,3)**2)) 
 
C...Find largest eigenvalue to matrix (third degree equation). 
      DO 190 J1=1,3 
      DO 180 J2=J1,3 
      SM(J1,J2)=SM(J1,J2)/PSS 
  180 CONTINUE 
  190 CONTINUE 
      SQ=(SM(1,1)*SM(2,2)+SM(1,1)*SM(3,3)+SM(2,2)*SM(3,3)-SM(1,2)**2- 
     &SM(1,3)**2-SM(2,3)**2)/3.-1./9. 
      SR=-0.5*(SQ+1./9.+SM(1,1)*SM(2,3)**2+SM(2,2)*SM(1,3)**2+SM(3,3)* 
     &SM(1,2)**2-SM(1,1)*SM(2,2)*SM(3,3))+SM(1,2)*SM(1,3)*SM(2,3)+1./27. 
      SP=COS(ACOS(MAX(MIN(SR/SQRT(-SQ**3),1.),-1.))/3.) 
      SMA=1./3.+SQRT(-SQ)*MAX(2.*SP,SQRT(3.*(1.-SP**2))-SP) 
 
C...Find largest eigenvector by solving equation system. 
      DO 210 J1=1,3 
      SM(J1,J1)=SM(J1,J1)-SMA 
      DO 200 J2=J1+1,3 
      SM(J2,J1)=SM(J1,J2) 
  200 CONTINUE 
  210 CONTINUE 
      SMAX=0. 
      DO 230 J1=1,3 
      DO 220 J2=1,3 
      IF(ABS(SM(J1,J2)).LE.SMAX) GOTO 220 
      JA=J1 
      JB=J2 
      SMAX=ABS(SM(J1,J2)) 
  220 CONTINUE 
  230 CONTINUE 
      SMAX=0. 
      DO 250 J3=JA+1,JA+2 
      J1=J3-3*((J3-1)/3) 
      RL=SM(J1,JB)/SM(JA,JB) 
      DO 240 J2=1,3 
      SM(J1,J2)=SM(J1,J2)-RL*SM(JA,J2) 
      IF(ABS(SM(J1,J2)).LE.SMAX) GOTO 240 
      JC=J1 
      SMAX=ABS(SM(J1,J2)) 
  240 CONTINUE 
  250 CONTINUE 
      JB1=JB+1-3*(JB/3) 
      JB2=JB+2-3*((JB+1)/3) 
      SAX(JB1)=-SM(JC,JB2) 
      SAX(JB2)=SM(JC,JB1) 
      SAX(JB)=-(SM(JA,JB1)*SAX(JB1)+SM(JA,JB2)*SAX(JB2))/SM(JA,JB) 
 
C...Divide particles into two initial clusters by hemisphere. 
      DO 270 I=N+1,N+NP 
      PSAX=P(I,1)*SAX(1)+P(I,2)*SAX(2)+P(I,3)*SAX(3) 
      IS=1 
      IF(PSAX.LT.0.) IS=2 
      K(I,3)=IS 
      DO 260 J=1,4 
      PS(IS,J)=PS(IS,J)+P(I,J) 
  260 CONTINUE 
  270 CONTINUE 
      PMS=MAX(1E-10,PS(1,4)**2-PS(1,1)**2-PS(1,2)**2-PS(1,3)**2)+ 
     &MAX(1E-10,PS(2,4)**2-PS(2,1)**2-PS(2,2)**2-PS(2,3)**2) 
 
C...Reassign one particle at a time; find maximum decrease of m^2 sum. 
  280 PMD=0. 
      IM=0 
      DO 290 J=1,4 
      PS(3,J)=PS(1,J)-PS(2,J) 
  290 CONTINUE 
      DO 300 I=N+1,N+NP 
      PPS=P(I,4)*PS(3,4)-P(I,1)*PS(3,1)-P(I,2)*PS(3,2)-P(I,3)*PS(3,3) 
      IF(K(I,3).EQ.1) PMDI=2.*(P(I,5)**2-PPS) 
      IF(K(I,3).EQ.2) PMDI=2.*(P(I,5)**2+PPS) 
      IF(PMDI.LT.PMD) THEN 
        PMD=PMDI 
        IM=I 
      ENDIF 
  300 CONTINUE 
 
C...Loop back if significant reduction in sum of m^2. 
      IF(PMD.LT.-PARU(48)*PMS) THEN 
        PMS=PMS+PMD 
        IS=K(IM,3) 
        DO 310 J=1,4 
        PS(IS,J)=PS(IS,J)-P(IM,J) 
        PS(3-IS,J)=PS(3-IS,J)+P(IM,J) 
  310   CONTINUE 
        K(IM,3)=3-IS 
        GOTO 280 
      ENDIF 
 
C...Final masses and output. 
      MSTU(61)=N+1 
      MSTU(62)=NP 
      PS(1,5)=SQRT(MAX(0.,PS(1,4)**2-PS(1,1)**2-PS(1,2)**2-PS(1,3)**2)) 
      PS(2,5)=SQRT(MAX(0.,PS(2,4)**2-PS(2,1)**2-PS(2,2)**2-PS(2,3)**2)) 
      PMH=MAX(PS(1,5),PS(2,5)) 
      PML=MIN(PS(1,5),PS(2,5)) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUFOWO0(H10,H20,H30,H40) 
 
C...Purpose: to calculate the first few Fox-Wolfram moments. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Copy momenta for particles and calculate H0. 
      NP=0 
      H0=0. 
      HD=0. 
      DO 110 I=1,N 
      IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 110 
      IF(MSTU(41).GE.2) THEN 
        KC=LUCOMP0(K(I,2)) 
        IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &  KC.EQ.18) GOTO 110 
        IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &  GOTO 110 
      ENDIF 
      IF(N+NP.GE.MSTU(4)-MSTU(32)-5) THEN 
        CALL LUERRM0(11,'(LUFOWO0:) no more memory left in LUJETS') 
        H10=-1. 
        H20=-1. 
        H30=-1. 
        H40=-1. 
        RETURN 
      ENDIF 
      NP=NP+1 
      DO 100 J=1,3 
      P(N+NP,J)=P(I,J) 
  100 CONTINUE 
      P(N+NP,4)=SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2) 
      H0=H0+P(N+NP,4) 
      HD=HD+P(N+NP,4)**2 
  110 CONTINUE 
      H0=H0**2 
 
C...Very low multiplicities (0 or 1) not considered. 
      IF(NP.LE.1) THEN 
        CALL LUERRM0(8,'(LUFOWO0:) too few particles for analysis') 
        H10=-1. 
        H20=-1. 
        H30=-1. 
        H40=-1. 
        RETURN 
      ENDIF 
 
C...Calculate H1 - H4. 
      H10=0. 
      H20=0. 
      H30=0. 
      H40=0. 
      DO 130 I1=N+1,N+NP 
      DO 120 I2=I1+1,N+NP 
      CTHE=(P(I1,1)*P(I2,1)+P(I1,2)*P(I2,2)+P(I1,3)*P(I2,3))/ 
     &(P(I1,4)*P(I2,4)) 
      H10=H10+P(I1,4)*P(I2,4)*CTHE 
      H20=H20+P(I1,4)*P(I2,4)*(1.5*CTHE**2-0.5) 
      H30=H30+P(I1,4)*P(I2,4)*(2.5*CTHE**3-1.5*CTHE) 
      H40=H40+P(I1,4)*P(I2,4)*(4.375*CTHE**4-3.75*CTHE**2+0.375) 
  120 CONTINUE 
  130 CONTINUE 
 
C...Calculate H1/H0 - H4/H0. Output. 
      MSTU(61)=N+1 
      MSTU(62)=NP 
      H10=(HD+2.*H10)/H0 
      H20=(HD+2.*H20)/H0 
      H30=(HD+2.*H30)/H0 
      H40=(HD+2.*H40)/H0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUTABU0(MTABU) 
 
C...Purpose: to evaluate various properties of an event, with 
C...statistics accumulated during the course of the run and 
C...printed at the end. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/,/LUDAT03/ 
      DIMENSION KFIS(100,2),NPIS(100,0:10),KFFS(400),NPFS(400,4), 
     &FEVFM(10,4),FM1FM(3,10,4),FM2FM(3,10,4),FMOMA(4),FMOMS(4), 
     &FEVEE(50),FE1EC(50),FE2EC(50),FE1EA(25),FE2EA(25), 
     &KFDM(8),KFDC(200,0:8),NPDC(200) 
      SAVE NEVIS,NKFIS,KFIS,NPIS,NEVFS,NPRFS,NFIFS,NCHFS,NKFFS, 
     &KFFS,NPFS,NEVFM,NMUFM,FM1FM,FM2FM,NEVEE,FE1EC,FE2EC,FE1EA, 
     &FE2EA,NEVDC,NKFDC,NREDC,KFDC,NPDC 
      CHARACTER CHAU*16,CHIS(2)*12,CHDC(8)*12 
      DATA NEVIS/0/,NKFIS/0/,NEVFS/0/,NPRFS/0/,NFIFS/0/,NCHFS/0/, 
     &NKFFS/0/,NEVFM/0/,NMUFM/0/,FM1FM/120*0./,FM2FM/120*0./, 
     &NEVEE/0/,FE1EC/50*0./,FE2EC/50*0./,FE1EA/25*0./,FE2EA/25*0./, 
     &NEVDC/0/,NKFDC/0/,NREDC/0/ 
 
C...Reset statistics on initial parton state. 
      IF(MTABU.EQ.10) THEN 
        NEVIS=0 
        NKFIS=0 
 
C...Identify and order flavour content of initial state. 
      ELSEIF(MTABU.EQ.11) THEN 
        NEVIS=NEVIS+1 
        KFM1=2*IABS(MSTU(161)) 
        IF(MSTU(161).GT.0) KFM1=KFM1-1 
        KFM2=2*IABS(MSTU(162)) 
        IF(MSTU(162).GT.0) KFM2=KFM2-1 
        KFMN=MIN(KFM1,KFM2) 
        KFMX=MAX(KFM1,KFM2) 
        DO 100 I=1,NKFIS 
        IF(KFMN.EQ.KFIS(I,1).AND.KFMX.EQ.KFIS(I,2)) THEN 
          IKFIS=-I 
          GOTO 110 
        ELSEIF(KFMN.LT.KFIS(I,1).OR.(KFMN.EQ.KFIS(I,1).AND. 
     &  KFMX.LT.KFIS(I,2))) THEN 
          IKFIS=I 
          GOTO 110 
        ENDIF 
  100   CONTINUE 
        IKFIS=NKFIS+1 
  110   IF(IKFIS.LT.0) THEN 
          IKFIS=-IKFIS 
        ELSE 
          IF(NKFIS.GE.100) RETURN 
          DO 130 I=NKFIS,IKFIS,-1 
          KFIS(I+1,1)=KFIS(I,1) 
          KFIS(I+1,2)=KFIS(I,2) 
          DO 120 J=0,10 
          NPIS(I+1,J)=NPIS(I,J) 
  120     CONTINUE 
  130   CONTINUE 
          NKFIS=NKFIS+1 
          KFIS(IKFIS,1)=KFMN 
          KFIS(IKFIS,2)=KFMX 
          DO 140 J=0,10 
          NPIS(IKFIS,J)=0 
  140     CONTINUE 
        ENDIF 
        NPIS(IKFIS,0)=NPIS(IKFIS,0)+1 
 
C...Count number of partons in initial state. 
        NP=0 
        DO 160 I=1,N 
        IF(K(I,1).LE.0.OR.K(I,1).GT.12) THEN 
        ELSEIF(IABS(K(I,2)).GT.80.AND.IABS(K(I,2)).LE.100) THEN 
        ELSEIF(IABS(K(I,2)).GT.100.AND.MOD(IABS(K(I,2))/10,10).NE.0) 
     &  THEN 
        ELSE 
          IM=I 
  150     IM=K(IM,3) 
          IF(IM.LE.0.OR.IM.GT.N) THEN 
            NP=NP+1 
          ELSEIF(K(IM,1).LE.0.OR.K(IM,1).GT.20) THEN 
            NP=NP+1 
          ELSEIF(IABS(K(IM,2)).GT.80.AND.IABS(K(IM,2)).LE.100) THEN 
          ELSEIF(IABS(K(IM,2)).GT.100.AND.MOD(IABS(K(IM,2))/10,10).NE.0) 
     &    THEN 
          ELSE 
            GOTO 150 
          ENDIF 
        ENDIF 
  160   CONTINUE 
        NPCO=MAX(NP,1) 
        IF(NP.GE.6) NPCO=6 
        IF(NP.GE.8) NPCO=7 
        IF(NP.GE.11) NPCO=8 
        IF(NP.GE.16) NPCO=9 
        IF(NP.GE.26) NPCO=10 
        NPIS(IKFIS,NPCO)=NPIS(IKFIS,NPCO)+1 
        MSTU(62)=NP 
 
C...Write statistics on initial parton state. 
      ELSEIF(MTABU.EQ.12) THEN 
        FAC=1./MAX(1,NEVIS) 
        WRITE(MSTU(11),5000) NEVIS 
        DO 170 I=1,NKFIS 
        KFMN=KFIS(I,1) 
        IF(KFMN.EQ.0) KFMN=KFIS(I,2) 
        KFM1=(KFMN+1)/2 
        IF(2*KFM1.EQ.KFMN) KFM1=-KFM1 
        CALL LUNAME0(KFM1,CHAU) 
        CHIS(1)=CHAU(1:12) 
        IF(CHAU(13:13).NE.' ') CHIS(1)(12:12)='?' 
        KFMX=KFIS(I,2) 
        IF(KFIS(I,1).EQ.0) KFMX=0 
        KFM2=(KFMX+1)/2 
        IF(2*KFM2.EQ.KFMX) KFM2=-KFM2 
        CALL LUNAME0(KFM2,CHAU) 
        CHIS(2)=CHAU(1:12) 
        IF(CHAU(13:13).NE.' ') CHIS(2)(12:12)='?' 
        WRITE(MSTU(11),5100) CHIS(1),CHIS(2),FAC*NPIS(I,0), 
     &  (NPIS(I,J)/FLOAT(NPIS(I,0)),J=1,10) 
  170   CONTINUE 
 
C...Copy statistics on initial parton state into /LUJETS/. 
      ELSEIF(MTABU.EQ.13) THEN 
        FAC=1./MAX(1,NEVIS) 
        DO 190 I=1,NKFIS 
        KFMN=KFIS(I,1) 
        IF(KFMN.EQ.0) KFMN=KFIS(I,2) 
        KFM1=(KFMN+1)/2 
        IF(2*KFM1.EQ.KFMN) KFM1=-KFM1 
        KFMX=KFIS(I,2) 
        IF(KFIS(I,1).EQ.0) KFMX=0 
        KFM2=(KFMX+1)/2 
        IF(2*KFM2.EQ.KFMX) KFM2=-KFM2 
        K(I,1)=32 
        K(I,2)=99 
        K(I,3)=KFM1 
        K(I,4)=KFM2 
        K(I,5)=NPIS(I,0) 
        DO 180 J=1,5 
        P(I,J)=FAC*NPIS(I,J) 
        V(I,J)=FAC*NPIS(I,J+5) 
  180   CONTINUE 
  190   CONTINUE 
        N=NKFIS 
        DO 200 J=1,5 
        K(N+1,J)=0 
        P(N+1,J)=0. 
        V(N+1,J)=0. 
  200   CONTINUE 
        K(N+1,1)=32 
        K(N+1,2)=99 
        K(N+1,5)=NEVIS 
        MSTU(3)=1 
 
C...Reset statistics on number of particles/partons. 
      ELSEIF(MTABU.EQ.20) THEN 
        NEVFS=0 
        NPRFS=0 
        NFIFS=0 
        NCHFS=0 
        NKFFS=0 
 
C...Identify whether particle/parton is primary or not. 
      ELSEIF(MTABU.EQ.21) THEN 
        NEVFS=NEVFS+1 
        MSTU(62)=0 
        DO 260 I=1,N 
        IF(K(I,1).LE.0.OR.K(I,1).GT.20.OR.K(I,1).EQ.13) GOTO 260 
        MSTU(62)=MSTU(62)+1 
        KC=LUCOMP0(K(I,2)) 
        MPRI=0 
        IF(K(I,3).LE.0.OR.K(I,3).GT.N) THEN 
          MPRI=1 
        ELSEIF(K(K(I,3),1).LE.0.OR.K(K(I,3),1).GT.20) THEN 
          MPRI=1 
        ELSEIF(K(K(I,3),2).GE.91.AND.K(K(I,3),2).LE.93) THEN 
          MPRI=1 
        ELSEIF(KC.EQ.0) THEN 
        ELSEIF(K(K(I,3),1).EQ.13) THEN 
          IM=K(K(I,3),3) 
          IF(IM.LE.0.OR.IM.GT.N) THEN 
            MPRI=1 
          ELSEIF(K(IM,1).LE.0.OR.K(IM,1).GT.20) THEN 
            MPRI=1 
          ENDIF 
        ELSEIF(KCHG(KC,2).EQ.0) THEN 
          KCM=LUCOMP0(K(K(I,3),2)) 
          IF(KCM.NE.0) THEN 
            IF(KCHG(KCM,2).NE.0) MPRI=1 
          ENDIF 
        ENDIF 
        IF(KC.NE.0.AND.MPRI.EQ.1) THEN 
          IF(KCHG(KC,2).EQ.0) NPRFS=NPRFS+1 
        ENDIF 
        IF(K(I,1).LE.10) THEN 
          NFIFS=NFIFS+1 
          IF(LUCHGE0(K(I,2)).NE.0) NCHFS=NCHFS+1 
        ENDIF 
 
C...Fill statistics on number of particles/partons in event. 
        KFA=IABS(K(I,2)) 
        KFS=3-ISIGN(1,K(I,2))-MPRI 
        DO 210 IP=1,NKFFS 
        IF(KFA.EQ.KFFS(IP)) THEN 
          IKFFS=-IP 
          GOTO 220 
        ELSEIF(KFA.LT.KFFS(IP)) THEN 
          IKFFS=IP 
          GOTO 220 
        ENDIF 
  210   CONTINUE 
        IKFFS=NKFFS+1 
  220   IF(IKFFS.LT.0) THEN 
          IKFFS=-IKFFS 
        ELSE 
          IF(NKFFS.GE.400) RETURN 
          DO 240 IP=NKFFS,IKFFS,-1 
          KFFS(IP+1)=KFFS(IP) 
          DO 230 J=1,4 
          NPFS(IP+1,J)=NPFS(IP,J) 
  230     CONTINUE 
  240   CONTINUE 
          NKFFS=NKFFS+1 
          KFFS(IKFFS)=KFA 
          DO 250 J=1,4 
          NPFS(IKFFS,J)=0 
  250     CONTINUE 
        ENDIF 
        NPFS(IKFFS,KFS)=NPFS(IKFFS,KFS)+1 
  260   CONTINUE 
 
C...Write statistics on particle/parton composition of events. 
      ELSEIF(MTABU.EQ.22) THEN 
        FAC=1./MAX(1,NEVFS) 
        WRITE(MSTU(11),5200) NEVFS,FAC*NPRFS,FAC*NFIFS,FAC*NCHFS 
        DO 270 I=1,NKFFS 
        CALL LUNAME0(KFFS(I),CHAU) 
        KC=LUCOMP0(KFFS(I)) 
        MDCYF=0 
        IF(KC.NE.0) MDCYF=MDCY(KC,1) 
        WRITE(MSTU(11),5300) KFFS(I),CHAU,MDCYF,(FAC*NPFS(I,J),J=1,4), 
     &  FAC*(NPFS(I,1)+NPFS(I,2)+NPFS(I,3)+NPFS(I,4)) 
  270   CONTINUE 
 
C...Copy particle/parton composition information into /LUJETS/. 
      ELSEIF(MTABU.EQ.23) THEN 
        FAC=1./MAX(1,NEVFS) 
        DO 290 I=1,NKFFS 
        K(I,1)=32 
        K(I,2)=99 
        K(I,3)=KFFS(I) 
        K(I,4)=0 
        K(I,5)=NPFS(I,1)+NPFS(I,2)+NPFS(I,3)+NPFS(I,4) 
        DO 280 J=1,4 
        P(I,J)=FAC*NPFS(I,J) 
        V(I,J)=0. 
  280   CONTINUE 
        P(I,5)=FAC*K(I,5) 
        V(I,5)=0. 
  290   CONTINUE 
        N=NKFFS 
        DO 300 J=1,5 
        K(N+1,J)=0 
        P(N+1,J)=0. 
        V(N+1,J)=0. 
  300   CONTINUE 
        K(N+1,1)=32 
        K(N+1,2)=99 
        K(N+1,5)=NEVFS 
        P(N+1,1)=FAC*NPRFS 
        P(N+1,2)=FAC*NFIFS 
        P(N+1,3)=FAC*NCHFS 
        MSTU(3)=1 
 
C...Reset factorial moments statistics. 
      ELSEIF(MTABU.EQ.30) THEN 
        NEVFM=0 
        NMUFM=0 
        DO 330 IM=1,3 
        DO 320 IB=1,10 
        DO 310 IP=1,4 
        FM1FM(IM,IB,IP)=0. 
        FM2FM(IM,IB,IP)=0. 
  310   CONTINUE 
  320   CONTINUE 
  330   CONTINUE 
 
C...Find particles to include, with (pion,pseudo)rapidity and azimuth. 
      ELSEIF(MTABU.EQ.31) THEN 
        NEVFM=NEVFM+1 
        NLOW=N+MSTU(3) 
        NUPP=NLOW 
        DO 410 I=1,N 
        IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 410 
        IF(MSTU(41).GE.2) THEN 
          KC=LUCOMP0(K(I,2)) 
          IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &    KC.EQ.18) GOTO 410 
          IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &    GOTO 410 
        ENDIF 
        PMR=0. 
        IF(MSTU(42).EQ.1.AND.K(I,2).NE.22) PMR=ULMASS0(211) 
        IF(MSTU(42).GE.2) PMR=P(I,5) 
        PR=MAX(1E-20,PMR**2+P(I,1)**2+P(I,2)**2) 
        YETA=SIGN(LOG(MIN((SQRT(PR+P(I,3)**2)+ABS(P(I,3)))/SQRT(PR), 
     &  1E20)),P(I,3)) 
        IF(ABS(YETA).GT.PARU(57)) GOTO 410 
        PHI=ULANGL0(P(I,1),P(I,2)) 
        IYETA=512.*(YETA+PARU(57))/(2.*PARU(57)) 
        IYETA=MAX(0,MIN(511,IYETA)) 
        IPHI=512.*(PHI+PARU(1))/PARU(2) 
        IPHI=MAX(0,MIN(511,IPHI)) 
        IYEP=0 
        DO 340 IB=0,9 
        IYEP=IYEP+4**IB*(2*MOD(IYETA/2**IB,2)+MOD(IPHI/2**IB,2)) 
  340   CONTINUE 
 
C...Order particles in (pseudo)rapidity and/or azimuth. 
        IF(NUPP.GT.MSTU(4)-5-MSTU(32)) THEN 
          CALL LUERRM0(11,'(LUTABU0:) no more memory left in LUJETS') 
          RETURN 
        ENDIF 
        NUPP=NUPP+1 
        IF(NUPP.EQ.NLOW+1) THEN 
          K(NUPP,1)=IYETA 
          K(NUPP,2)=IPHI 
          K(NUPP,3)=IYEP 
        ELSE 
          DO 350 I1=NUPP-1,NLOW+1,-1 
          IF(IYETA.GE.K(I1,1)) GOTO 360 
          K(I1+1,1)=K(I1,1) 
  350     CONTINUE 
  360     K(I1+1,1)=IYETA 
          DO 370 I1=NUPP-1,NLOW+1,-1 
          IF(IPHI.GE.K(I1,2)) GOTO 380 
          K(I1+1,2)=K(I1,2) 
  370     CONTINUE 
  380     K(I1+1,2)=IPHI 
          DO 390 I1=NUPP-1,NLOW+1,-1 
          IF(IYEP.GE.K(I1,3)) GOTO 400 
          K(I1+1,3)=K(I1,3) 
  390     CONTINUE 
  400     K(I1+1,3)=IYEP 
        ENDIF 
  410   CONTINUE 
        K(NUPP+1,1)=2**10 
        K(NUPP+1,2)=2**10 
        K(NUPP+1,3)=4**10 
 
C...Calculate sum of factorial moments in event. 
        DO 480 IM=1,3 
        DO 430 IB=1,10 
        DO 420 IP=1,4 
        FEVFM(IB,IP)=0. 
  420   CONTINUE 
  430   CONTINUE 
        DO 450 IB=1,10 
        IF(IM.LE.2) IBIN=2**(10-IB) 
        IF(IM.EQ.3) IBIN=4**(10-IB) 
        IAGR=K(NLOW+1,IM)/IBIN 
        NAGR=1 
        DO 440 I=NLOW+2,NUPP+1 
        ICUT=K(I,IM)/IBIN 
        IF(ICUT.EQ.IAGR) THEN 
          NAGR=NAGR+1 
        ELSE 
          IF(NAGR.EQ.1) THEN 
          ELSEIF(NAGR.EQ.2) THEN 
            FEVFM(IB,1)=FEVFM(IB,1)+2. 
          ELSEIF(NAGR.EQ.3) THEN 
            FEVFM(IB,1)=FEVFM(IB,1)+6. 
            FEVFM(IB,2)=FEVFM(IB,2)+6. 
          ELSEIF(NAGR.EQ.4) THEN 
            FEVFM(IB,1)=FEVFM(IB,1)+12. 
            FEVFM(IB,2)=FEVFM(IB,2)+24. 
            FEVFM(IB,3)=FEVFM(IB,3)+24. 
          ELSE 
            FEVFM(IB,1)=FEVFM(IB,1)+NAGR*(NAGR-1.) 
            FEVFM(IB,2)=FEVFM(IB,2)+NAGR*(NAGR-1.)*(NAGR-2.) 
            FEVFM(IB,3)=FEVFM(IB,3)+NAGR*(NAGR-1.)*(NAGR-2.)*(NAGR-3.) 
            FEVFM(IB,4)=FEVFM(IB,4)+NAGR*(NAGR-1.)*(NAGR-2.)*(NAGR-3.)* 
     &      (NAGR-4.) 
          ENDIF 
          IAGR=ICUT 
          NAGR=1 
        ENDIF 
  440   CONTINUE 
  450   CONTINUE 
 
C...Add results to total statistics. 
        DO 470 IB=10,1,-1 
        DO 460 IP=1,4 
        IF(FEVFM(1,IP).LT.0.5) THEN 
          FEVFM(IB,IP)=0. 
        ELSEIF(IM.LE.2) THEN 
          FEVFM(IB,IP)=2.**((IB-1)*IP)*FEVFM(IB,IP)/FEVFM(1,IP) 
        ELSE 
          FEVFM(IB,IP)=4.**((IB-1)*IP)*FEVFM(IB,IP)/FEVFM(1,IP) 
        ENDIF 
        FM1FM(IM,IB,IP)=FM1FM(IM,IB,IP)+FEVFM(IB,IP) 
        FM2FM(IM,IB,IP)=FM2FM(IM,IB,IP)+FEVFM(IB,IP)**2 
  460   CONTINUE 
  470   CONTINUE 
  480   CONTINUE 
        NMUFM=NMUFM+(NUPP-NLOW) 
        MSTU(62)=NUPP-NLOW 
 
C...Write accumulated statistics on factorial moments. 
      ELSEIF(MTABU.EQ.32) THEN 
        FAC=1./MAX(1,NEVFM) 
        IF(MSTU(42).LE.0) WRITE(MSTU(11),5400) NEVFM,'eta' 
        IF(MSTU(42).EQ.1) WRITE(MSTU(11),5400) NEVFM,'ypi' 
        IF(MSTU(42).GE.2) WRITE(MSTU(11),5400) NEVFM,'y  ' 
        DO 510 IM=1,3 
        WRITE(MSTU(11),5500) 
        DO 500 IB=1,10 
        BYETA=2.*PARU(57) 
        IF(IM.NE.2) BYETA=BYETA/2**(IB-1) 
        BPHI=PARU(2) 
        IF(IM.NE.1) BPHI=BPHI/2**(IB-1) 
        IF(IM.LE.2) BNAVE=FAC*NMUFM/FLOAT(2**(IB-1)) 
        IF(IM.EQ.3) BNAVE=FAC*NMUFM/FLOAT(4**(IB-1)) 
        DO 490 IP=1,4 
        FMOMA(IP)=FAC*FM1FM(IM,IB,IP) 
        FMOMS(IP)=SQRT(MAX(0.,FAC*(FAC*FM2FM(IM,IB,IP)-FMOMA(IP)**2))) 
  490   CONTINUE 
        WRITE(MSTU(11),5600) BYETA,BPHI,BNAVE,(FMOMA(IP),FMOMS(IP), 
     &  IP=1,4) 
  500   CONTINUE 
  510   CONTINUE 
 
C...Copy statistics on factorial moments into /LUJETS/. 
      ELSEIF(MTABU.EQ.33) THEN 
        FAC=1./MAX(1,NEVFM) 
        DO 540 IM=1,3 
        DO 530 IB=1,10 
        I=10*(IM-1)+IB 
        K(I,1)=32 
        K(I,2)=99 
        K(I,3)=1 
        IF(IM.NE.2) K(I,3)=2**(IB-1) 
        K(I,4)=1 
        IF(IM.NE.1) K(I,4)=2**(IB-1) 
        K(I,5)=0 
        P(I,1)=2.*PARU(57)/K(I,3) 
        V(I,1)=PARU(2)/K(I,4) 
        DO 520 IP=1,4 
        P(I,IP+1)=FAC*FM1FM(IM,IB,IP) 
        V(I,IP+1)=SQRT(MAX(0.,FAC*(FAC*FM2FM(IM,IB,IP)-P(I,IP+1)**2))) 
  520   CONTINUE 
  530   CONTINUE 
  540   CONTINUE 
        N=30 
        DO 550 J=1,5 
        K(N+1,J)=0 
        P(N+1,J)=0. 
        V(N+1,J)=0. 
  550   CONTINUE 
        K(N+1,1)=32 
        K(N+1,2)=99 
        K(N+1,5)=NEVFM 
        MSTU(3)=1 
 
C...Reset statistics on Energy-Energy Correlation. 
      ELSEIF(MTABU.EQ.40) THEN 
        NEVEE=0 
        DO 560 J=1,25 
        FE1EC(J)=0. 
        FE2EC(J)=0. 
        FE1EC(51-J)=0. 
        FE2EC(51-J)=0. 
        FE1EA(J)=0. 
        FE2EA(J)=0. 
  560   CONTINUE 
 
C...Find particles to include, with proper assumed mass. 
      ELSEIF(MTABU.EQ.41) THEN 
        NEVEE=NEVEE+1 
        NLOW=N+MSTU(3) 
        NUPP=NLOW 
        ECM=0. 
        DO 570 I=1,N 
        IF(K(I,1).LE.0.OR.K(I,1).GT.10) GOTO 570 
        IF(MSTU(41).GE.2) THEN 
          KC=LUCOMP0(K(I,2)) 
          IF(KC.EQ.0.OR.KC.EQ.12.OR.KC.EQ.14.OR.KC.EQ.16.OR. 
     &    KC.EQ.18) GOTO 570 
          IF(MSTU(41).GE.3.AND.KCHG(KC,2).EQ.0.AND.LUCHGE0(K(I,2)).EQ.0) 
     &    GOTO 570 
        ENDIF 
        PMR=0. 
        IF(MSTU(42).EQ.1.AND.K(I,2).NE.22) PMR=ULMASS0(211) 
        IF(MSTU(42).GE.2) PMR=P(I,5) 
        IF(NUPP.GT.MSTU(4)-5-MSTU(32)) THEN 
          CALL LUERRM0(11,'(LUTABU0:) no more memory left in LUJETS') 
          RETURN 
        ENDIF 
        NUPP=NUPP+1 
        P(NUPP,1)=P(I,1) 
        P(NUPP,2)=P(I,2) 
        P(NUPP,3)=P(I,3) 
        P(NUPP,4)=SQRT(PMR**2+P(I,1)**2+P(I,2)**2+P(I,3)**2) 
        P(NUPP,5)=MAX(1E-10,SQRT(P(I,1)**2+P(I,2)**2+P(I,3)**2)) 
        ECM=ECM+P(NUPP,4) 
  570   CONTINUE 
        IF(NUPP.EQ.NLOW) RETURN 
 
C...Analyze Energy-Energy Correlation in event. 
        FAC=(2./ECM**2)*50./PARU(1) 
        DO 580 J=1,50 
        FEVEE(J)=0. 
  580   CONTINUE 
        DO 600 I1=NLOW+2,NUPP 
        DO 590 I2=NLOW+1,I1-1 
        CTHE=(P(I1,1)*P(I2,1)+P(I1,2)*P(I2,2)+P(I1,3)*P(I2,3))/ 
     &  (P(I1,5)*P(I2,5)) 
        THE=ACOS(MAX(-1.,MIN(1.,CTHE))) 
        ITHE=MAX(1,MIN(50,1+INT(50.*THE/PARU(1)))) 
        FEVEE(ITHE)=FEVEE(ITHE)+FAC*P(I1,4)*P(I2,4) 
  590   CONTINUE 
  600   CONTINUE 
        DO 610 J=1,25 
        FE1EC(J)=FE1EC(J)+FEVEE(J) 
        FE2EC(J)=FE2EC(J)+FEVEE(J)**2 
        FE1EC(51-J)=FE1EC(51-J)+FEVEE(51-J) 
        FE2EC(51-J)=FE2EC(51-J)+FEVEE(51-J)**2 
        FE1EA(J)=FE1EA(J)+(FEVEE(51-J)-FEVEE(J)) 
        FE2EA(J)=FE2EA(J)+(FEVEE(51-J)-FEVEE(J))**2 
  610   CONTINUE 
        MSTU(62)=NUPP-NLOW 
 
C...Write statistics on Energy-Energy Correlation. 
      ELSEIF(MTABU.EQ.42) THEN 
        FAC=1./MAX(1,NEVEE) 
        WRITE(MSTU(11),5700) NEVEE 
        DO 620 J=1,25 
        FEEC1=FAC*FE1EC(J) 
        FEES1=SQRT(MAX(0.,FAC*(FAC*FE2EC(J)-FEEC1**2))) 
        FEEC2=FAC*FE1EC(51-J) 
        FEES2=SQRT(MAX(0.,FAC*(FAC*FE2EC(51-J)-FEEC2**2))) 
        FEECA=FAC*FE1EA(J) 
        FEESA=SQRT(MAX(0.,FAC*(FAC*FE2EA(J)-FEECA**2))) 
        WRITE(MSTU(11),5800) 3.6*(J-1),3.6*J,FEEC1,FEES1,FEEC2,FEES2, 
     &  FEECA,FEESA 
  620   CONTINUE 
 
C...Copy statistics on Energy-Energy Correlation into /LUJETS/. 
      ELSEIF(MTABU.EQ.43) THEN 
        FAC=1./MAX(1,NEVEE) 
        DO 630 I=1,25 
        K(I,1)=32 
        K(I,2)=99 
        K(I,3)=0 
        K(I,4)=0 
        K(I,5)=0 
        P(I,1)=FAC*FE1EC(I) 
        V(I,1)=SQRT(MAX(0.,FAC*(FAC*FE2EC(I)-P(I,1)**2))) 
        P(I,2)=FAC*FE1EC(51-I) 
        V(I,2)=SQRT(MAX(0.,FAC*(FAC*FE2EC(51-I)-P(I,2)**2))) 
        P(I,3)=FAC*FE1EA(I) 
        V(I,3)=SQRT(MAX(0.,FAC*(FAC*FE2EA(I)-P(I,3)**2))) 
        P(I,4)=PARU(1)*(I-1)/50. 
        P(I,5)=PARU(1)*I/50. 
        V(I,4)=3.6*(I-1) 
        V(I,5)=3.6*I 
  630   CONTINUE 
        N=25 
        DO 640 J=1,5 
        K(N+1,J)=0 
        P(N+1,J)=0. 
        V(N+1,J)=0. 
  640   CONTINUE 
        K(N+1,1)=32 
        K(N+1,2)=99 
        K(N+1,5)=NEVEE 
        MSTU(3)=1 
 
C...Reset statistics on decay channels. 
      ELSEIF(MTABU.EQ.50) THEN 
        NEVDC=0 
        NKFDC=0 
        NREDC=0 
 
C...Identify and order flavour content of final state. 
      ELSEIF(MTABU.EQ.51) THEN 
        NEVDC=NEVDC+1 
        NDS=0 
        DO 670 I=1,N 
        IF(K(I,1).LE.0.OR.K(I,1).GE.6) GOTO 670 
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
  670   CONTINUE 
 
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
        ELSEIF(NKFDC.GE.200) THEN 
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
      ELSEIF(MTABU.EQ.52) THEN 
        FAC=1./MAX(1,NEVDC) 
        WRITE(MSTU(11),5900) NEVDC 
        DO 750 IDC=1,NKFDC 
        DO 740 I=1,KFDC(IDC,0) 
        KFM=KFDC(IDC,I) 
        KF=(KFM+1)/2 
        IF(2*KF.NE.KFM) KF=-KF 
        CALL LUNAME0(KF,CHAU) 
        CHDC(I)=CHAU(1:12) 
        IF(CHAU(13:13).NE.' ') CHDC(I)(12:12)='?' 
  740   CONTINUE 
        WRITE(MSTU(11),6000) FAC*NPDC(IDC),(CHDC(I),I=1,KFDC(IDC,0)) 
  750   CONTINUE 
        IF(NREDC.NE.0) WRITE(MSTU(11),6100) FAC*NREDC 
 
C...Copy statistics on decay channels into /LUJETS/. 
      ELSEIF(MTABU.EQ.53) THEN 
        FAC=1./MAX(1,NEVDC) 
        DO 780 IDC=1,NKFDC 
        K(IDC,1)=32 
        K(IDC,2)=99 
        K(IDC,3)=0 
        K(IDC,4)=0 
        K(IDC,5)=KFDC(IDC,0) 
        DO 760 J=1,5 
        P(IDC,J)=0. 
        V(IDC,J)=0. 
  760   CONTINUE 
        DO 770 I=1,KFDC(IDC,0) 
        KFM=KFDC(IDC,I) 
        KF=(KFM+1)/2 
        IF(2*KF.NE.KFM) KF=-KF 
        IF(I.LE.5) P(IDC,I)=KF 
        IF(I.GE.6) V(IDC,I-5)=KF 
  770   CONTINUE 
        V(IDC,5)=FAC*NPDC(IDC) 
  780   CONTINUE 
        N=NKFDC 
        DO 790 J=1,5 
        K(N+1,J)=0 
        P(N+1,J)=0. 
        V(N+1,J)=0. 
  790   CONTINUE 
        K(N+1,1)=32 
        K(N+1,2)=99 
        K(N+1,5)=NEVDC 
        V(N+1,5)=FAC*NREDC 
        MSTU(3)=1 
      ENDIF 
 
C...Format statements for output on unit MSTU(11) (default 6). 
 5000 FORMAT(///20X,'Event statistics - initial state'/ 
     &20X,'based on an analysis of ',I6,' events'// 
     &3X,'Main flavours after',8X,'Fraction',4X,'Subfractions ', 
     &'according to fragmenting system multiplicity'/ 
     &4X,'hard interaction',24X,'1',7X,'2',7X,'3',7X,'4',7X,'5', 
     &6X,'6-7',5X,'8-10',3X,'11-15',3X,'16-25',4X,'>25'/) 
 5100 FORMAT(3X,A12,1X,A12,F10.5,1X,10F8.4) 
 5200 FORMAT(///20X,'Event statistics - final state'/ 
     &20X,'based on an analysis of ',I7,' events'// 
     &5X,'Mean primary multiplicity =',F10.4/ 
     &5X,'Mean final   multiplicity =',F10.4/ 
     &5X,'Mean charged multiplicity =',F10.4// 
     &5X,'Number of particles produced per event (directly and via ', 
     &'decays/branchings)'/ 
     &5X,'KF    Particle/jet  MDCY',10X,'Particles',13X,'Antiparticles', 
     &8X,'Total'/35X,'prim        seco        prim        seco'/) 
 5300 FORMAT(1X,I6,4X,A16,I2,5(1X,F11.6)) 
 5400 FORMAT(///20X,'Factorial moments analysis of multiplicity'/ 
     &20X,'based on an analysis of ',I6,' events'// 
     &3X,'delta-',A3,' delta-phi     <n>/bin',10X,'<F2>',18X,'<F3>', 
     &18X,'<F4>',18X,'<F5>'/35X,4('     value     error  ')) 
 5500 FORMAT(10X) 
 5600 FORMAT(2X,2F10.4,F12.4,4(F12.4,F10.4)) 
 5700 FORMAT(///20X,'Energy-Energy Correlation and Asymmetry'/ 
     &20X,'based on an analysis of ',I6,' events'// 
     &2X,'theta range',8X,'EEC(theta)',8X,'EEC(180-theta)',7X, 
     &'EECA(theta)'/2X,'in degrees ',3('      value    error')/) 
 5800 FORMAT(2X,F4.1,' - ',F4.1,3(F11.4,F9.4)) 
 5900 FORMAT(///20X,'Decay channel analysis - final state'/ 
     &20X,'based on an analysis of ',I6,' events'// 
     &2X,'Probability',10X,'Complete final state'/) 
 6000 FORMAT(2X,F9.5,5X,8(A12,1X)) 
 6100 FORMAT(2X,F9.5,5X,'into other channels (more than 8 particles ', 
     &'or table overflow)') 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUEEVT0(KFL,ECM) 
 
C...Purpose: to handle the generation of an e+e- annihilation jet event. 
      IMPLICIT DOUBLE PRECISION(D) 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Check input parameters. 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      IF(KFL.LT.0.OR.KFL.GT.8) THEN 
        CALL LUERRM0(16,'(LUEEVT0:) called with unknown flavour code') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(KFL.LE.5) ECMMIN=PARJ(127)+2.02*PARF(100+MAX(1,KFL)) 
      IF(KFL.GE.6) ECMMIN=PARJ(127)+2.02*PMAS(KFL,1) 
      IF(ECM.LT.ECMMIN) THEN 
        CALL LUERRM0(16,'(LUEEVT0:) called with too small CM energy') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Check consistency of MSTJ options set. 
      IF(MSTJ(109).EQ.2.AND.MSTJ(110).NE.1) THEN 
        CALL LUERRM0(6, 
     &  '(LUEEVT0:) MSTJ(109) value requires MSTJ(110) = 1') 
        MSTJ(110)=1 
      ENDIF 
      IF(MSTJ(109).EQ.2.AND.MSTJ(111).NE.0) THEN 
        CALL LUERRM0(6, 
     &  '(LUEEVT0:) MSTJ(109) value requires MSTJ(111) = 0') 
        MSTJ(111)=0 
      ENDIF 
 
C...Initialize alpha_strong and total cross-section. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      IF(MSTJ(116).GT.0.AND.(MSTJ(116).GE.2.OR.ABS(ECM-PARJ(151)).GE. 
     &PARJ(139).OR.10*MSTJ(102)+KFL.NE.MSTJ(119))) CALL LUXTOT0(KFL,ECM, 
     &XTOT) 
      IF(MSTJ(116).GE.3) MSTJ(116)=1 
      PARJ(171)=0. 
 
C...Add initial e+e- to event record (documentation only). 
      NTRY=0 
  100 NTRY=NTRY+1 
      IF(NTRY.GT.100) THEN 
        CALL LUERRM0(14,'(LUEEVT0:) caught in an infinite loop') 
        RETURN 
      ENDIF 
      MSTU(24)=0 
      NC=0 
      IF(MSTJ(115).GE.2) THEN 
        NC=NC+2 
        CALL LU1ENT0(NC-1,11,0.5*ECM,0.,0.) 
        K(NC-1,1)=21 
        CALL LU1ENT0(NC,-11,0.5*ECM,PARU(1),0.) 
        K(NC,1)=21 
      ENDIF 
 
C...Radiative photon (in initial state). 
      MK=0 
      ECMC=ECM 
      IF(MSTJ(107).GE.1.AND.MSTJ(116).GE.1) CALL LURADK0(ECM,MK,PAK, 
     &THEK,PHIK,ALPK) 
      IF(MK.EQ.1) ECMC=SQRT(ECM*(ECM-2.*PAK)) 
      IF(MSTJ(115).GE.1.AND.MK.EQ.1) THEN 
        NC=NC+1 
        CALL LU1ENT0(NC,22,PAK,THEK,PHIK) 
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
        CALL LU1ENT0(NC,KF,ECMC,0.,0.) 
        K(NC,1)=21 
        K(NC,3)=1 
        MSTU(10)=MSTU10 
      ENDIF 
 
C...Choice of flavour and jet configuration. 
      CALL LUXKFL0(KFL,ECM,ECMC,KFLC) 
      IF(KFLC.EQ.0) GOTO 100 
      CALL LUXJET0(ECMC,NJET,CUT) 
      KFLN=21 
      IF(NJET.EQ.4) CALL LUX4JT0(NJET,CUT,KFLC,ECMC,KFLN,X1,X2,X4, 
     &X12,X14) 
      IF(NJET.EQ.3) CALL LUX3JT0(NJET,CUT,KFLC,ECMC,X1,X3) 
      IF(NJET.EQ.2) MSTJ(120)=1 
 
C...Fill jet configuration and origin. 
      IF(NJET.EQ.2.AND.MSTJ(101).NE.5)
     &CALL LU2ENT0(NC+1,KFLC,-KFLC,ECMC) 
      IF(NJET.EQ.2.AND.MSTJ(101).EQ.5)
     &CALL LU2ENT0(-(NC+1),KFLC,-KFLC,ECMC) 
      IF(NJET.EQ.3) CALL LU3ENT0(NC+1,KFLC,21,-KFLC,ECMC,X1,X3) 
      IF(NJET.EQ.4.AND.KFLN.EQ.21) CALL LU4ENT0(NC+1,KFLC,KFLN,KFLN, 
     &-KFLC,ECMC,X1,X2,X4,X12,X14) 
      IF(NJET.EQ.4.AND.KFLN.NE.21) CALL LU4ENT0(NC+1,KFLC,-KFLN,KFLN, 
     &-KFLC,ECMC,X1,X2,X4,X12,X14) 
      IF(MSTU(24).NE.0) GOTO 100 
      DO 110 IP=NC+1,N 
      K(IP,3)=K(IP,3)+MIN(MSTJ(115)/2,1)+(MSTJ(115)/3)*(NC-1) 
  110 CONTINUE 
 
C...Angular orientation according to matrix element. 
      IF(MSTJ(106).EQ.1) THEN 
        CALL LUXDIF0(NC,NJET,KFLC,ECMC,CHI,THE,PHI) 
        CALL LUDBRB0(NC+1,N,0.,CHI,0D0,0D0,0D0) 
        CALL LUDBRB0(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 
 
C...Rotation and boost from radiative photon. 
      IF(MK.EQ.1) THEN 
        DBEK=-PAK/(ECM-PAK) 
        NMIN=NC+1-MSTJ(115)/3 
        CALL LUDBRB0(NMIN,N,0.,-PHIK,0D0,0D0,0D0) 
        CALL LUDBRB0(NMIN,N,ALPK,0.,DBEK*SIN(THEK),0D0,DBEK*COS(THEK)) 
        CALL LUDBRB0(NMIN,N,0.,PHIK,0D0,0D0,0D0) 
      ENDIF 
 
C...Generate parton shower. Rearrange along strings and check. 
      IF(MSTJ(101).EQ.5) THEN 
        CALL LUSHOW0(N-1,N,ECMC) 
        MSTJ14=MSTJ(14) 
        IF(MSTJ(105).EQ.-1) MSTJ(14)=-1 
        IF(MSTJ(105).GE.0) MSTU(28)=0 
        CALL LUPREP0(0) 
        MSTJ(14)=MSTJ14 
        IF(MSTJ(105).GE.0.AND.MSTU(28).NE.0) GOTO 100 
      ENDIF 
 
C...Fragmentation/decay generation. Information for LUTABU0. 
      IF(MSTJ(105).EQ.1) CALL LUEXEC0 
      MSTU(161)=KFLC 
      MSTU(162)=-KFLC 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUXTOT0(KFL,ECM,XTOT) 
 
C...Purpose: to calculate total cross-section, including initial 
C...state radiation effects. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUDAT01/,/LUDAT02/ 
 
C...Status, (optimized) Q^2 scale, alpha_strong. 
      PARJ(151)=ECM 
      MSTJ(119)=10*MSTJ(102)+KFL 
      IF(MSTJ(111).EQ.0) THEN 
        Q2R=ECM**2 
      ELSEIF(MSTU(111).EQ.0) THEN 
        PARJ(168)=MIN(1.,MAX(PARJ(128),EXP(-12.*PARU(1)/ 
     &  ((33.-2.*MSTU(112))*PARU(111))))) 
        Q2R=PARJ(168)*ECM**2 
      ELSE 
        PARJ(168)=MIN(1.,MAX(PARJ(128),PARU(112)/ECM, 
     &  (2.*PARU(112)/ECM)**2)) 
        Q2R=PARJ(168)*ECM**2 
      ENDIF 
      ALSPI=ULALPS0(Q2R)/PARU(1) 
 
C...QCD corrections factor in R. 
      IF(MSTJ(101).EQ.0.OR.MSTJ(109).EQ.1) THEN 
        RQCD=1. 
      ELSEIF(IABS(MSTJ(101)).EQ.1.AND.MSTJ(109).EQ.0) THEN 
        RQCD=1.+ALSPI 
      ELSEIF(MSTJ(109).EQ.0) THEN 
        RQCD=1.+ALSPI+(1.986-0.115*MSTU(118))*ALSPI**2 
        IF(MSTJ(111).EQ.1) RQCD=MAX(1.,RQCD+(33.-2.*MSTU(112))/12.* 
     &  LOG(PARJ(168))*ALSPI**2) 
      ELSEIF(IABS(MSTJ(101)).EQ.1) THEN 
        RQCD=1.+(3./4.)*ALSPI 
      ELSE 
        RQCD=1.+(3./4.)*ALSPI-(3./32.+0.519*MSTU(118))*ALSPI**2 
      ENDIF 
 
C...Calculate Z0 width if default value not acceptable. 
      IF(MSTJ(102).GE.3) THEN 
        RVA=3.*(3.+(4.*PARU(102)-1.)**2)+6.*RQCD*(2.+(1.-8.*PARU(102)/ 
     &  3.)**2+(4.*PARU(102)/3.-1.)**2) 
        DO 100 KFLC=5,6 
        VQ=1. 
        IF(MOD(MSTJ(103),2).EQ.1) VQ=SQRT(MAX(0.,1.-(2.*ULMASS0(KFLC)/ 
     &  ECM)**2)) 
        IF(KFLC.EQ.5) VF=4.*PARU(102)/3.-1. 
        IF(KFLC.EQ.6) VF=1.-8.*PARU(102)/3. 
        RVA=RVA+3.*RQCD*(0.5*VQ*(3.-VQ**2)*VF**2+VQ**3) 
  100   CONTINUE 
        PARJ(124)=PARU(101)*PARJ(123)*RVA/(48.*PARU(102)*(1.-PARU(102))) 
      ENDIF 
 
C...Calculate propagator and related constants for QFD case. 
      POLL=1.-PARJ(131)*PARJ(132) 
      IF(MSTJ(102).GE.2) THEN 
        SFF=1./(16.*PARU(102)*(1.-PARU(102))) 
        SFW=ECM**4/((ECM**2-PARJ(123)**2)**2+(PARJ(123)*PARJ(124))**2) 
        SFI=SFW*(1.-(PARJ(123)/ECM)**2) 
        VE=4.*PARU(102)-1. 
        SF1I=SFF*(VE*POLL+PARJ(132)-PARJ(131)) 
        SF1W=SFF**2*((VE**2+1.)*POLL+2.*VE*(PARJ(132)-PARJ(131))) 
        HF1I=SFI*SF1I 
        HF1W=SFW*SF1W 
      ENDIF 
 
C...Loop over different flavours: charge, velocity. 
      RTOT=0. 
      RQQ=0. 
      RQV=0. 
      RVA=0. 
      DO 110 KFLC=1,MAX(MSTJ(104),KFL) 
      IF(KFL.GT.0.AND.KFLC.NE.KFL) GOTO 110 
      MSTJ(93)=1 
      PMQ=ULMASS0(KFLC) 
      IF(ECM.LT.2.*PMQ+PARJ(127)) GOTO 110 
      QF=KCHG(KFLC,1)/3. 
      VQ=1. 
      IF(MOD(MSTJ(103),2).EQ.1) VQ=SQRT(1.-(2.*PMQ/ECM)**2) 
 
C...Calculate R and sum of charges for QED or QFD case. 
      RQQ=RQQ+3.*QF**2*POLL 
      IF(MSTJ(102).LE.1) THEN 
        RTOT=RTOT+3.*0.5*VQ*(3.-VQ**2)*QF**2*POLL 
      ELSE 
        VF=SIGN(1.,QF)-4.*QF*PARU(102) 
        RQV=RQV-6.*QF*VF*SF1I 
        RVA=RVA+3.*(VF**2+1.)*SF1W 
        RTOT=RTOT+3.*(0.5*VQ*(3.-VQ**2)*(QF**2*POLL-2.*QF*VF*HF1I+ 
     &  VF**2*HF1W)+VQ**3*HF1W) 
      ENDIF 
  110 CONTINUE 
      RSUM=RQQ 
      IF(MSTJ(102).GE.2) RSUM=RQQ+SFI*RQV+SFW*RVA 
 
C...Calculate cross-section, including QCD corrections. 
      PARJ(141)=RQQ 
      PARJ(142)=RTOT 
      PARJ(143)=RTOT*RQCD 
      PARJ(144)=PARJ(143) 
      PARJ(145)=PARJ(141)*86.8/ECM**2 
      PARJ(146)=PARJ(142)*86.8/ECM**2 
      PARJ(147)=PARJ(143)*86.8/ECM**2 
      PARJ(148)=PARJ(147) 
      PARJ(157)=RSUM*RQCD 
      PARJ(158)=0. 
      PARJ(159)=0. 
      XTOT=PARJ(147) 
      IF(MSTJ(107).LE.0) RETURN 
 
C...Virtual cross-section. 
      XKL=PARJ(135) 
      XKU=MIN(PARJ(136),1.-(2.*PARJ(127)/ECM)**2) 
      ALE=2.*LOG(ECM/ULMASS0(11))-1. 
      SIGV=ALE/3.+2.*LOG(ECM**2/(ULMASS0(13)*ULMASS0(15)))/3.-4./3.+ 
     &1.526*LOG(ECM**2/0.932) 
 
C...Soft and hard radiative cross-section in QED case. 
      IF(MSTJ(102).LE.1) THEN 
        SIGV=1.5*ALE-0.5+PARU(1)**2/3.+2.*SIGV 
        SIGS=ALE*(2.*LOG(XKL)-LOG(1.-XKL)-XKL) 
        SIGH=ALE*(2.*LOG(XKU/XKL)-LOG((1.-XKU)/(1.-XKL))-(XKU-XKL)) 
 
C...Soft and hard radiative cross-section in QFD case. 
      ELSE 
        SZM=1.-(PARJ(123)/ECM)**2 
        SZW=PARJ(123)*PARJ(124)/ECM**2 
        PARJ(161)=-RQQ/RSUM 
        PARJ(162)=-(RQQ+RQV+RVA)/RSUM 
        PARJ(163)=(RQV*(1.-0.5*SZM-SFI)+RVA*(1.5-SZM-SFW))/RSUM 
        PARJ(164)=(RQV*SZW**2*(1.-2.*SFW)+RVA*(2.*SFI+SZW**2-4.+3.*SZM- 
     &  SZM**2))/(SZW*RSUM) 
        SIGV=1.5*ALE-0.5+PARU(1)**2/3.+((2.*RQQ+SFI*RQV)/RSUM)*SIGV+ 
     &  (SZW*SFW*RQV/RSUM)*PARU(1)*20./9. 
        SIGS=ALE*(2.*LOG(XKL)+PARJ(161)*LOG(1.-XKL)+PARJ(162)*XKL+ 
     &  PARJ(163)*LOG(((XKL-SZM)**2+SZW**2)/(SZM**2+SZW**2))+ 
     &  PARJ(164)*(ATAN((XKL-SZM)/SZW)-ATAN(-SZM/SZW))) 
        SIGH=ALE*(2.*LOG(XKU/XKL)+PARJ(161)*LOG((1.-XKU)/(1.-XKL))+ 
     &  PARJ(162)*(XKU-XKL)+PARJ(163)*LOG(((XKU-SZM)**2+SZW**2)/ 
     &  ((XKL-SZM)**2+SZW**2))+PARJ(164)*(ATAN((XKU-SZM)/SZW)- 
     &  ATAN((XKL-SZM)/SZW))) 
      ENDIF 
 
C...Total cross-section and fraction of hard photon events. 
      PARJ(160)=SIGH/(PARU(1)/PARU(101)+SIGV+SIGS+SIGH) 
      PARJ(157)=RSUM*(1.+(PARU(101)/PARU(1))*(SIGV+SIGS+SIGH))*RQCD 
      PARJ(144)=PARJ(157) 
      PARJ(148)=PARJ(144)*86.8/ECM**2 
      XTOT=PARJ(148) 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LURADK0(ECM,MK,PAK,THEK,PHIK,ALPK) 
 
C...Purpose: to generate initial state photon radiation. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT01/ 
 
C...Function: cumulative hard photon spectrum in QFD case. 
      FXK(XX)=2.*LOG(XX)+PARJ(161)*LOG(1.-XX)+PARJ(162)*XX+ 
     &PARJ(163)*LOG((XX-SZM)**2+SZW**2)+PARJ(164)*ATAN((XX-SZM)/SZW) 
 
C...Determine whether radiative photon or not. 
      MK=0 
      PAK=0.
      RLU00=RLU0(0) 
      IF(PARJ(160).LT.RLU00) RETURN 
      MK=1 
 
C...Photon energy range. Find photon momentum in QED case. 
      XKL=PARJ(135) 
      XKU=MIN(PARJ(136),1.-(2.*PARJ(127)/ECM)**2) 
      IF(MSTJ(102).LE.1) THEN 
  100   XK=1./(1.+(1./XKL-1.)*((1./XKU-1.)/(1./XKL-1.))**RLU0(0)) 
        IF(1.+(1.-XK)**2.LT.2.*RLU0(0)) GOTO 100 
 
C...Ditto in QFD case, by numerical inversion of integrated spectrum. 
      ELSE 
        SZM=1.-(PARJ(123)/ECM)**2 
        SZW=PARJ(123)*PARJ(124)/ECM**2 
        FXKL=FXK(XKL) 
        FXKU=FXK(XKU) 
        FXKD=1E-4*(FXKU-FXKL) 
        FXKR=FXKL+RLU0(0)*(FXKU-FXKL) 
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
        IF(NXK.LT.15.AND.FXKU-FXKL.GT.FXKD) GOTO 110 
        XK=XKL+(XKU-XKL)*(FXKR-FXKL)/(FXKU-FXKL) 
      ENDIF 
      PAK=0.5*ECM*XK 
 
C...Photon polar and azimuthal angle. 
      PME=2.*(ULMASS0(11)/ECM)**2 
  120 CTHM=PME*(2./PME)**RLU0(0) 
      IF(1.-(XK**2*CTHM*(1.-0.5*CTHM)+2.*(1.-XK)*PME/MAX(PME, 
     &CTHM*(1.-0.5*CTHM)))/(1.+(1.-XK)**2).LT.RLU0(0)) GOTO 120 
      CTHE=1.-CTHM 
      IF(RLU0(0).GT.0.5) CTHE=-CTHE 
      STHE=SQRT(MAX(0.,(CTHM-PME)*(2.-CTHM))) 
      THEK=ULANGL0(CTHE,STHE) 
      PHIK=PARU(2)*RLU0(0) 
   
C...Rotation angle for hadronic system. 
      SGN=1. 
      IF(0.5*(2.-XK*(1.-CTHE))**2/((2.-XK)**2+(XK*CTHE)**2).GT. 
     &RLU0(0)) SGN=-1. 
      ALPK=ASIN(SGN*STHE*(XK-SGN*(2.*SQRT(1.-XK)-2.+XK)*CTHE)/ 
     &(2.-XK*(1.-SGN*CTHE))) 
      
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUXKFL0(KFL,ECM,ECMC,KFLC) 
 
C...Purpose: to select flavour for produced qqbar pair. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/ARLWDAT1/IALFU(100),RALFU(100),IALPA(100),RALPA(100) 
      SAVE /LUDAT01/,/LUDAT02/,/ARLWDAT1/
      PARJ127=PARJ(127) 
 
C...Calculate maximum weight in QED or QFD case. 
      IF(MSTJ(102).LE.1) THEN 
        RFMAX=4./9. 
      ELSE 
        POLL=1.-PARJ(131)*PARJ(132) 
        SFF=1./(16.*PARU(102)*(1.-PARU(102))) 
        SFW=ECMC**4/((ECMC**2-PARJ(123)**2)**2+(PARJ(123)*PARJ(124))**2) 
        SFI=SFW*(1.-(PARJ(123)/ECMC)**2) 
        VE=4.*PARU(102)-1. 
        HF1I=SFI*SFF*(VE*POLL+PARJ(132)-PARJ(131)) 
        HF1W=SFW*SFF**2*((VE**2+1.)*POLL+2.*VE*(PARJ(132)-PARJ(131))) 
        RFMAX=MAX(4./9.*POLL-4./3.*(1.-8.*PARU(102)/3.)*HF1I+ 
     &  ((1.-8.*PARU(102)/3.)**2+1.)*HF1W,1./9.*POLL+2./3.* 
     &  (-1.+4.*PARU(102)/3.)*HF1I+((-1.+4.*PARU(102)/3.)**2+1.)*HF1W) 
      ENDIF 
 
C...Choose flavour. Gives charge and velocity. 
      NTRY=0 
  100 NTRY=NTRY+1 
      IF(NTRY.GT.1000) THEN 
        CALL LUERRM0(1,'(LUXKFL0:) caught in an infinite loop')
        KFLC=0 
        RETURN 
      ENDIF 
      KFLC=KFL 
      IF(KFL.LE.0) KFLC=1+INT(MSTJ(104)*RLU0(0)) 
      MSTJ(93)=1 
      PMQ=ULMASS0(KFLC)
      IF(1.LE.KFLC.AND.KFLC.LE.4)PARJ(127)=RALPA(86+KFLC) 
      IF(ECMC.LT.2.*PMQ+PARJ(127)) GOTO 100 
      QF=KCHG(KFLC,1)/3. 
      VQ=1. 
      IF(MOD(MSTJ(103),2).EQ.1) VQ=SQRT(MAX(0.,1.-(2.*PMQ/ECMC)**2)) 
 
C...Calculate weight in QED or QFD case. 
      IF(MSTJ(102).LE.1) THEN 
        RF=QF**2 
        RFV=0.5*VQ*(3.-VQ**2)*QF**2 
      ELSE 
        VF=SIGN(1.,QF)-4.*QF*PARU(102) 
        RF=QF**2*POLL-2.*QF*VF*HF1I+(VF**2+1.)*HF1W 
        RFV=0.5*VQ*(3.-VQ**2)*(QF**2*POLL-2.*QF*VF*HF1I+VF**2*HF1W)+ 
     &  VQ**3*HF1W 
        IF(RFV.GT.0.) PARJ(171)=MIN(1.,VQ**3*HF1W/RFV) 
      ENDIF 
 
C...Weighting or new event (radiative photon). Cross-section update. 
      IF(KFL.LE.0.AND.RF.LT.RLU0(0)*RFMAX) GOTO 100 
      PARJ(158)=PARJ(158)+1. 
      IF(ECMC.LT.2.*PMQ+PARJ(127).OR.RFV.LT.RLU0(0)*RF) KFLC=0 
      IF(MSTJ(107).LE.0.AND.KFLC.EQ.0) GOTO 100 
      IF(KFLC.NE.0) PARJ(159)=PARJ(159)+1. 
      PARJ(144)=PARJ(157)*PARJ(159)/PARJ(158) 
      PARJ(148)=PARJ(144)*86.8/ECM**2 
      PARJ(127)=PARJ127
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUXJET0(ECM,NJET,CUT) 
 
C...Purpose: to select number of jets in matrix element approach. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT01/ 
      DIMENSION ZHUT(5) 
 
C...Relative three-jet rate in Zhu second order parametrization. 
      DATA ZHUT/3.0922, 6.2291, 7.4782, 7.8440, 8.2560/ 
 
C...Trivial result for two-jets only, including parton shower. 
      IF(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.5) THEN 
        CUT=0. 
 
C...QCD and Abelian vector gluon theory: Q^2 for jet rate and R. 
      ELSEIF(MSTJ(109).EQ.0.OR.MSTJ(109).EQ.2) THEN 
        CF=4./3. 
        IF(MSTJ(109).EQ.2) CF=1. 
        IF(MSTJ(111).EQ.0) THEN 
          Q2=ECM**2 
          Q2R=ECM**2 
        ELSEIF(MSTU(111).EQ.0) THEN 
          PARJ(169)=MIN(1.,PARJ(129)) 
          Q2=PARJ(169)*ECM**2 
          PARJ(168)=MIN(1.,MAX(PARJ(128),EXP(-12.*PARU(1)/ 
     &    ((33.-2.*MSTU(112))*PARU(111))))) 
          Q2R=PARJ(168)*ECM**2 
        ELSE 
          PARJ(169)=MIN(1.,MAX(PARJ(129),(2.*PARU(112)/ECM)**2)) 
          Q2=PARJ(169)*ECM**2 
          PARJ(168)=MIN(1.,MAX(PARJ(128),PARU(112)/ECM, 
     &    (2.*PARU(112)/ECM)**2)) 
          Q2R=PARJ(168)*ECM**2 
        ENDIF 
 
C...alpha_strong for R and R itself. 
        ALSPI=(3./4.)*CF*ULALPS0(Q2R)/PARU(1) 
        IF(IABS(MSTJ(101)).EQ.1) THEN 
          RQCD=1.+ALSPI 
        ELSEIF(MSTJ(109).EQ.0) THEN 
          RQCD=1.+ALSPI+(1.986-0.115*MSTU(118))*ALSPI**2 
          IF(MSTJ(111).EQ.1) RQCD=MAX(1.,RQCD+(33.-2.*MSTU(112))/12.* 
     &    LOG(PARJ(168))*ALSPI**2) 
        ELSE 
          RQCD=1.+ALSPI-(3./32.+0.519*MSTU(118))*(4.*ALSPI/3.)**2 
        ENDIF 
 
C...alpha_strong for jet rate. Initial value for y cut. 
        ALSPI=(3./4.)*CF*ULALPS0(Q2)/PARU(1) 
        CUT=MAX(0.001,PARJ(125),(PARJ(126)/ECM)**2) 
        IF(IABS(MSTJ(101)).LE.1.OR.(MSTJ(109).EQ.0.AND.MSTJ(111).EQ.0)) 
     &  CUT=MAX(CUT,EXP(-SQRT(0.75/ALSPI))/2.) 
        IF(MSTJ(110).EQ.2) CUT=MAX(0.01,MIN(0.05,CUT)) 
 
C...Parametrization of first order three-jet cross-section. 
  100   IF(MSTJ(101).EQ.0.OR.CUT.GE.0.25) THEN 
          PARJ(152)=0. 
        ELSE 
          PARJ(152)=(2.*ALSPI/3.)*((3.-6.*CUT+2.*LOG(CUT))* 
     &    LOG(CUT/(1.-2.*CUT))+(2.5+1.5*CUT-6.571)*(1.-3.*CUT)+ 
     &    5.833*(1.-3.*CUT)**2-3.894*(1.-3.*CUT)**3+ 
     &    1.342*(1.-3.*CUT)**4)/RQCD 
          IF(MSTJ(109).EQ.2.AND.(MSTJ(101).EQ.2.OR.MSTJ(101).LE.-2)) 
     &    PARJ(152)=0. 
        ENDIF 
 
C...Parametrization of second order three-jet cross-section. 
        IF(IABS(MSTJ(101)).LE.1.OR.MSTJ(101).EQ.3.OR.MSTJ(109).EQ.2.OR. 
     &  CUT.GE.0.25) THEN 
          PARJ(153)=0. 
        ELSEIF(MSTJ(110).LE.1) THEN 
          CT=LOG(1./CUT-2.) 
          PARJ(153)=ALSPI**2*CT**2*(2.419+0.5989*CT+0.6782*CT**2- 
     &    0.2661*CT**3+0.01159*CT**4)/RQCD 
 
C...Interpolation in second/first order ratio for Zhu parametrization. 
        ELSEIF(MSTJ(110).EQ.2) THEN 
          IZA=0 
          DO 110 IY=1,5 
          IF(ABS(CUT-0.01*IY).LT.0.0001) IZA=IY 
  110     CONTINUE 
          IF(IZA.NE.0) THEN 
            ZHURAT=ZHUT(IZA) 
          ELSE 
            IZ=100.*CUT 
            ZHURAT=ZHUT(IZ)+(100.*CUT-IZ)*(ZHUT(IZ+1)-ZHUT(IZ)) 
          ENDIF 
          PARJ(153)=ALSPI*PARJ(152)*ZHURAT 
        ENDIF 
 
C...Shift in second order three-jet cross-section with optimized Q^2. 
        IF(MSTJ(111).EQ.1.AND.IABS(MSTJ(101)).GE.2.AND.MSTJ(101).NE.3. 
     &  AND.CUT.LT.0.25) PARJ(153)=PARJ(153)+(33.-2.*MSTU(112))/12.* 
     &  LOG(PARJ(169))*ALSPI*PARJ(152) 
 
C...Parametrization of second order four-jet cross-section. 
        IF(IABS(MSTJ(101)).LE.1.OR.CUT.GE.0.125) THEN 
          PARJ(154)=0. 
        ELSE 
          CT=LOG(1./CUT-5.) 
          IF(CUT.LE.0.018) THEN 
            XQQGG=6.349-4.330*CT+0.8304*CT**2 
            IF(MSTJ(109).EQ.2) XQQGG=(4./3.)**2*(3.035-2.091*CT+ 
     &      0.4059*CT**2) 
            XQQQQ=1.25*(-0.1080+0.01486*CT+0.009364*CT**2) 
            IF(MSTJ(109).EQ.2) XQQQQ=8.*XQQQQ 
          ELSE 
            XQQGG=-0.09773+0.2959*CT-0.2764*CT**2+0.08832*CT**3 
            IF(MSTJ(109).EQ.2) XQQGG=(4./3.)**2*(-0.04079+0.1340*CT- 
     &      0.1326*CT**2+0.04365*CT**3) 
            XQQQQ=1.25*(0.003661-0.004888*CT-0.001081*CT**2+0.002093* 
     &      CT**3) 
            IF(MSTJ(109).EQ.2) XQQQQ=8.*XQQQQ 
          ENDIF 
          PARJ(154)=ALSPI**2*CT**2*(XQQGG+XQQQQ)/RQCD 
          PARJ(155)=XQQQQ/(XQQGG+XQQQQ) 
        ENDIF 
 
C...If negative three-jet rate, change y' optimization parameter. 
        IF(MSTJ(111).EQ.1.AND.PARJ(152)+PARJ(153).LT.0..AND. 
     &  PARJ(169).LT.0.99) THEN 
          PARJ(169)=MIN(1.,1.2*PARJ(169)) 
          Q2=PARJ(169)*ECM**2 
          ALSPI=(3./4.)*CF*ULALPS0(Q2)/PARU(1) 
          GOTO 100 
        ENDIF 
 
C...If too high cross-section, use harder cuts, or fail. 
        IF(PARJ(152)+PARJ(153)+PARJ(154).GE.1) THEN 
          IF(MSTJ(110).EQ.2.AND.CUT.GT.0.0499.AND.MSTJ(111).EQ.1.AND. 
     &    PARJ(169).LT.0.99) THEN 
            PARJ(169)=MIN(1.,1.2*PARJ(169)) 
            Q2=PARJ(169)*ECM**2 
            ALSPI=(3./4.)*CF*ULALPS0(Q2)/PARU(1) 
            GOTO 100 
          ELSEIF(MSTJ(110).EQ.2.AND.CUT.GT.0.0499) THEN 
            CALL LUERRM0(26, 
     &      '(LUXJET0:) no allowed y cut value for Zhu parametrization') 
          ENDIF 
          CUT=0.26*(4.*CUT)**(PARJ(152)+PARJ(153)+PARJ(154))**(-1./3.) 
          IF(MSTJ(110).EQ.2) CUT=MAX(0.01,MIN(0.05,CUT)) 
          GOTO 100 
        ENDIF 
 
C...Scalar gluon (first order only). 
      ELSE 
        ALSPI=ULALPS0(ECM**2)/PARU(1) 
        CUT=MAX(0.001,PARJ(125),(PARJ(126)/ECM)**2,EXP(-3./ALSPI)) 
        PARJ(152)=0. 
        IF(CUT.LT.0.25) PARJ(152)=(ALSPI/3.)*((1.-2.*CUT)* 
     &  LOG((1.-2.*CUT)/CUT)+0.5*(9.*CUT**2-1.)) 
        PARJ(153)=0. 
        PARJ(154)=0. 
      ENDIF 
 
C...Select number of jets. 
      PARJ(150)=CUT 
      IF(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.5) THEN 
        NJET=2 
      ELSEIF(MSTJ(101).LE.0) THEN 
        NJET=MIN(4,2-MSTJ(101)) 
      ELSE 
        RNJ=RLU0(0) 
        NJET=2 
        IF(PARJ(152)+PARJ(153)+PARJ(154).GT.RNJ) NJET=3 
        IF(PARJ(154).GT.RNJ) NJET=4 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUX3JT0(NJET,CUT,KFL,ECM,X1,X2) 
 
C...Purpose: to select the kinematical variables of three-jet events. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT01/ 
      DIMENSION ZHUP(5,12) 
 
C...Coefficients of Zhu second order parametrization. 
      DATA ((ZHUP(IC1,IC2),IC2=1,12),IC1=1,5)/ 
     &    18.29,    89.56,    4.541,   -52.09,   -109.8,    24.90, 
     &    11.63,    3.683,    17.50, 0.002440,   -1.362,  -0.3537, 
     &    11.42,    6.299,   -22.55,   -8.915,    59.25,   -5.855, 
     &   -32.85,   -1.054,   -16.90, 0.006489,  -0.8156,  0.01095, 
     &    7.847,   -3.964,   -35.83,    1.178,    29.39,   0.2806, 
     &    47.82,   -12.36,   -56.72,  0.04054,  -0.4365,   0.6062, 
     &    5.441,   -56.89,   -50.27,    15.13,    114.3,   -18.19, 
     &    97.05,   -1.890,   -139.9,  0.08153,  -0.4984,   0.9439, 
     &   -17.65,    51.44,   -58.32,    70.95,   -255.7,   -78.99, 
     &    476.9,    29.65,   -239.3,   0.4745,   -1.174,    6.081/ 
 
C...Dilogarithm of x for x<0.5 (x>0.5 obtained by analytic trick). 
      DILOG(X)=X+X**2/4.+X**3/9.+X**4/16.+X**5/25.+X**6/36.+X**7/49. 
 
C...Event type. Mass effect factors and other common constants. 
      MSTJ(120)=2 
      MSTJ(121)=0 
      PMQ=ULMASS0(KFL) 
      QME=(2.*PMQ/ECM)**2 
      IF(MSTJ(109).NE.1) THEN 
        CUTL=LOG(CUT) 
        CUTD=LOG(1./CUT-2.) 
        IF(MSTJ(109).EQ.0) THEN 
          CF=4./3. 
          CN=3. 
          TR=2. 
          WTMX=MIN(20.,37.-6.*CUTD) 
          IF(MSTJ(110).EQ.2) WTMX=2.*(7.5+80.*CUT) 
        ELSE 
          CF=1. 
          CN=0. 
          TR=12. 
          WTMX=0. 
        ENDIF 
 
C...Alpha_strong and effects of optimized Q^2 scale. Maximum weight. 
        ALS2PI=PARU(118)/PARU(2) 
        WTOPT=0. 
        IF(MSTJ(111).EQ.1) WTOPT=(33.-2.*MSTU(112))/6.*LOG(PARJ(169))* 
     &  ALS2PI 
        WTMAX=MAX(0.,1.+WTOPT+ALS2PI*WTMX) 
 
C...Choose three-jet events in allowed region. 
  100   NJET=3 
  110   Y13L=CUTL+CUTD*RLU0(0) 
        Y23L=CUTL+CUTD*RLU0(0) 
        Y13=EXP(Y13L) 
        Y23=EXP(Y23L) 
        Y12=1.-Y13-Y23 
        IF(Y12.LE.CUT) GOTO 110 
        IF(Y13**2+Y23**2+2.*Y12.LE.2.*RLU0(0)) GOTO 110 
 
C...Second order corrections. 
        IF(MSTJ(101).EQ.2.AND.MSTJ(110).LE.1) THEN 
          Y12L=LOG(Y12) 
          Y13M=LOG(1.-Y13) 
          Y23M=LOG(1.-Y23) 
          Y12M=LOG(1.-Y12) 
          IF(Y13.LE.0.5) Y13I=DILOG(Y13) 
          IF(Y13.GE.0.5) Y13I=1.644934-Y13L*Y13M-DILOG(1.-Y13) 
          IF(Y23.LE.0.5) Y23I=DILOG(Y23) 
          IF(Y23.GE.0.5) Y23I=1.644934-Y23L*Y23M-DILOG(1.-Y23) 
          IF(Y12.LE.0.5) Y12I=DILOG(Y12) 
          IF(Y12.GE.0.5) Y12I=1.644934-Y12L*Y12M-DILOG(1.-Y12) 
          WT1=(Y13**2+Y23**2+2.*Y12)/(Y13*Y23) 
          WT2=CF*(-2.*(CUTL-Y12L)**2-3.*CUTL-1.+3.289868+ 
     &    2.*(2.*CUTL-Y12L)*CUT/Y12)+ 
     &    CN*((CUTL-Y12L)**2-(CUTL-Y13L)**2-(CUTL-Y23L)**2-11.*CUTL/6.+ 
     &    67./18.+1.644934-(2.*CUTL-Y12L)*CUT/Y12+(2.*CUTL-Y13L)* 
     &    CUT/Y13+(2.*CUTL-Y23L)*CUT/Y23)+ 
     &    TR*(2.*CUTL/3.-10./9.)+ 
     &    CF*(Y12/(Y12+Y13)+Y12/(Y12+Y23)+(Y12+Y23)/Y13+(Y12+Y13)/Y23+ 
     &    Y13L*(4.*Y12**2+2.*Y12*Y13+4.*Y12*Y23+Y13*Y23)/(Y12+Y23)**2+ 
     &    Y23L*(4.*Y12**2+2.*Y12*Y23+4.*Y12*Y13+Y13*Y23)/(Y12+Y13)**2)/ 
     &    WT1+ 
     &    CN*(Y13L*Y13/(Y12+Y23)+Y23L*Y23/(Y12+Y13))/WT1+ 
     &    (CN-2.*CF)*((Y12**2+(Y12+Y13)**2)*(Y12L*Y23L-Y12L*Y12M-Y23L* 
     &    Y23M+1.644934-Y12I-Y23I)/(Y13*Y23)+(Y12**2+(Y12+Y23)**2)* 
     &    (Y12L*Y13L-Y12L*Y12M-Y13L*Y13M+1.644934-Y12I-Y13I)/ 
     &    (Y13*Y23)+(Y13**2+Y23**2)/(Y13*Y23*(Y13+Y23))- 
     &    2.*Y12L*Y12**2/(Y13+Y23)**2-4.*Y12L*Y12/(Y13+Y23))/WT1- 
     &    CN*(Y13L*Y23L-Y13L*Y13M-Y23L*Y23M+1.644934-Y13I-Y23I) 
          IF(1.+WTOPT+ALS2PI*WT2.LE.0.) MSTJ(121)=1 
          IF(1.+WTOPT+ALS2PI*WT2.LE.WTMAX*RLU0(0)) GOTO 110 
          PARJ(156)=(WTOPT+ALS2PI*WT2)/(1.+WTOPT+ALS2PI*WT2) 
 
        ELSEIF(MSTJ(101).EQ.2.AND.MSTJ(110).EQ.2) THEN 
C...Second order corrections; Zhu parametrization of ERT. 
          ZX=(Y23-Y13)**2 
          ZY=1.-Y12 
          IZA=0 
          DO 120 IY=1,5 
          IF(ABS(CUT-0.01*IY).LT.0.0001) IZA=IY 
  120     CONTINUE 
          IF(IZA.NE.0) THEN 
            IZ=IZA 
            WT2=ZHUP(IZ,1)+ZHUP(IZ,2)*ZX+ZHUP(IZ,3)*ZX**2+(ZHUP(IZ,4)+ 
     &      ZHUP(IZ,5)*ZX)*ZY+(ZHUP(IZ,6)+ZHUP(IZ,7)*ZX)*ZY**2+ 
     &      (ZHUP(IZ,8)+ZHUP(IZ,9)*ZX)*ZY**3+ZHUP(IZ,10)/(ZX-ZY**2)+ 
     &      ZHUP(IZ,11)/(1.-ZY)+ZHUP(IZ,12)/ZY 
          ELSE 
            IZ=100.*CUT 
            WTL=ZHUP(IZ,1)+ZHUP(IZ,2)*ZX+ZHUP(IZ,3)*ZX**2+(ZHUP(IZ,4)+ 
     &      ZHUP(IZ,5)*ZX)*ZY+(ZHUP(IZ,6)+ZHUP(IZ,7)*ZX)*ZY**2+ 
     &      (ZHUP(IZ,8)+ZHUP(IZ,9)*ZX)*ZY**3+ZHUP(IZ,10)/(ZX-ZY**2)+ 
     &      ZHUP(IZ,11)/(1.-ZY)+ZHUP(IZ,12)/ZY 
            IZ=IZ+1 
            WTU=ZHUP(IZ,1)+ZHUP(IZ,2)*ZX+ZHUP(IZ,3)*ZX**2+(ZHUP(IZ,4)+ 
     &      ZHUP(IZ,5)*ZX)*ZY+(ZHUP(IZ,6)+ZHUP(IZ,7)*ZX)*ZY**2+ 
     &      (ZHUP(IZ,8)+ZHUP(IZ,9)*ZX)*ZY**3+ZHUP(IZ,10)/(ZX-ZY**2)+ 
     &      ZHUP(IZ,11)/(1.-ZY)+ZHUP(IZ,12)/ZY 
            WT2=WTL+(WTU-WTL)*(100.*CUT+1.-IZ) 
          ENDIF 
          IF(1.+WTOPT+2.*ALS2PI*WT2.LE.0.) MSTJ(121)=1 
          IF(1.+WTOPT+2.*ALS2PI*WT2.LE.WTMAX*RLU0(0)) GOTO 110 
          PARJ(156)=(WTOPT+2.*ALS2PI*WT2)/(1.+WTOPT+2.*ALS2PI*WT2) 
        ENDIF 
 
C...Impose mass cuts (gives two jets). For fixed jet number new try. 
        X1=1.-Y23 
        X2=1.-Y13 
        X3=1.-Y12 
        IF(4.*Y23*Y13*Y12/X3**2.LE.QME) NJET=2 
        IF(MOD(MSTJ(103),4).GE.2.AND.IABS(MSTJ(101)).LE.1.AND.QME*X3+ 
     &  0.5*QME**2+(0.5*QME+0.25*QME**2)*((1.-X2)/(1.-X1)+ 
     &  (1.-X1)/(1.-X2)).GT.(X1**2+X2**2)*RLU0(0)) NJET=2 
        IF(MSTJ(101).EQ.-1.AND.NJET.EQ.2) GOTO 100 
 
C...Scalar gluon model (first order only, no mass effects). 
      ELSE 
  130   NJET=3 
  140   X3=SQRT(4.*CUT**2+RLU0(0)*((1.-CUT)**2-4.*CUT**2)) 
        IF(LOG((X3-CUT)/CUT).LE.RLU0(0)*LOG((1.-2.*CUT)/CUT)) GOTO 140 
        YD=SIGN(2.*CUT*((X3-CUT)/CUT)**RLU0(0)-X3,RLU0(0)-0.5) 
        X1=1.-0.5*(X3+YD) 
        X2=1.-0.5*(X3-YD) 
        IF(4.*(1.-X1)*(1.-X2)*(1.-X3)/X3**2.LE.QME) NJET=2 
        IF(MSTJ(102).GE.2) THEN 
          IF(X3**2-2.*(1.+X3)*(1.-X1)*(1.-X2)*PARJ(171).LT. 
     &    X3**2*RLU0(0)) NJET=2 
        ENDIF 
        IF(MSTJ(101).EQ.-1.AND.NJET.EQ.2) GOTO 130 
      ENDIF 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUX4JT0(NJET,CUT,KFL,ECM,KFLN,X1,X2,X4,X12,X14) 
 
C...Purpose: to select the kinematical variables of four-jet events. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUDAT01/ 
      DIMENSION WTA(4),WTB(4),WTC(4),WTD(4),WTE(4) 
 
C...Common constants. Colour factors for QCD and Abelian gluon theory. 
      PMQ=ULMASS0(KFL) 
      QME=(2.*PMQ/ECM)**2 
      CT=LOG(1./CUT-5.) 
      IF(MSTJ(109).EQ.0) THEN 
        CF=4./3. 
        CN=3. 
        TR=2.5 
      ELSE 
        CF=1. 
        CN=0. 
        TR=15. 
      ENDIF 
 
C...Choice of process (qqbargg or qqbarqqbar). 
  100 NJET=4 
      IT=1 
      IF(PARJ(155).GT.RLU0(0)) IT=2 
      IF(MSTJ(101).LE.-3) IT=-MSTJ(101)-2 
      IF(IT.EQ.1) WTMX=0.7/CUT**2 
      IF(IT.EQ.1.AND.MSTJ(109).EQ.2) WTMX=0.6/CUT**2 
      IF(IT.EQ.2) WTMX=0.1125*CF*TR/CUT**2 
      ID=1 
 
C...Sample the five kinematical variables (for qqgg preweighted in y34). 
  110 Y134=3.*CUT+(1.-6.*CUT)*RLU0(0) 
      Y234=3.*CUT+(1.-6.*CUT)*RLU0(0) 
      IF(IT.EQ.1) Y34=(1.-5.*CUT)*EXP(-CT*RLU0(0)) 
      IF(IT.EQ.2) Y34=CUT+(1.-6.*CUT)*RLU0(0) 
      IF(Y34.LE.Y134+Y234-1..OR.Y34.GE.Y134*Y234) GOTO 110 
      VT=RLU0(0) 
      CP=COS(PARU(1)*RLU0(0)) 
      Y14=(Y134-Y34)*VT 
      Y13=Y134-Y14-Y34 
      VB=Y34*(1.-Y134-Y234+Y34)/((Y134-Y34)*(Y234-Y34)) 
      Y24=0.5*(Y234-Y34)*(1.-4.*SQRT(MAX(0.,VT*(1.-VT)*VB*(1.-VB)))* 
     &CP-(1.-2.*VT)*(1.-2.*VB)) 
      Y23=Y234-Y34-Y24 
      Y12=1.-Y134-Y23-Y24 
      IF(MIN(Y12,Y13,Y14,Y23,Y24).LE.CUT) GOTO 110 
      Y123=Y12+Y13+Y23 
      Y124=Y12+Y14+Y24 
 
C...Calculate matrix elements for qqgg or qqqq process. 
      IC=0 
      WTTOT=0. 
  120 IC=IC+1 
      IF(IT.EQ.1) THEN 
        WTA(IC)=(Y12*Y34**2-Y13*Y24*Y34+Y14*Y23*Y34+3.*Y12*Y23*Y34+ 
     &  3.*Y12*Y14*Y34+4.*Y12**2*Y34-Y13*Y23*Y24+2.*Y12*Y23*Y24- 
     &  Y13*Y14*Y24-2.*Y12*Y13*Y24+2.*Y12**2*Y24+Y14*Y23**2+2.*Y12* 
     &  Y23**2+Y14**2*Y23+4.*Y12*Y14*Y23+4.*Y12**2*Y23+2.*Y12*Y14**2+ 
     &  2.*Y12*Y13*Y14+4.*Y12**2*Y14+2.*Y12**2*Y13+2.*Y12**3)/(2.*Y13* 
     &  Y134*Y234*Y24)+(Y24*Y34+Y12*Y34+Y13*Y24-Y14*Y23+Y12*Y13)/(Y13* 
     &  Y134**2)+2.*Y23*(1.-Y13)/(Y13*Y134*Y24)+Y34/(2.*Y13*Y24) 
        WTB(IC)=(Y12*Y24*Y34+Y12*Y14*Y34-Y13*Y24**2+Y13*Y14*Y24+2.*Y12* 
     &  Y14*Y24)/(Y13*Y134*Y23*Y14)+Y12*(1.+Y34)*Y124/(Y134*Y234*Y14* 
     &  Y24)-(2.*Y13*Y24+Y14**2+Y13*Y23+2.*Y12*Y13)/(Y13*Y134*Y14)+ 
     &  Y12*Y123*Y124/(2.*Y13*Y14*Y23*Y24) 
        WTC(IC)=-(5.*Y12*Y34**2+2.*Y12*Y24*Y34+2.*Y12*Y23*Y34+2.*Y12* 
     &  Y14*Y34+2.*Y12*Y13*Y34+4.*Y12**2*Y34-Y13*Y24**2+Y14*Y23*Y24+ 
     &  Y13*Y23*Y24+Y13*Y14*Y24-Y12*Y14*Y24-Y13**2*Y24-3.*Y12*Y13*Y24- 
     &  Y14*Y23**2-Y14**2*Y23+Y13*Y14*Y23-3.*Y12*Y14*Y23-Y12*Y13*Y23)/ 
     &  (4.*Y134*Y234*Y34**2)+(3.*Y12*Y34**2-3.*Y13*Y24*Y34+3.*Y12*Y24* 
     &  Y34+3.*Y14*Y23*Y34-Y13*Y24**2-Y12*Y23*Y34+6.*Y12*Y14*Y34+2.*Y12* 
     &  Y13*Y34-2.*Y12**2*Y34+Y14*Y23*Y24-3.*Y13*Y23*Y24-2.*Y13*Y14* 
     &  Y24+4.*Y12*Y14*Y24+2.*Y12*Y13*Y24+3.*Y14*Y23**2+2.*Y14**2*Y23+ 
     &  2.*Y14**2*Y12+2.*Y12**2*Y14+6.*Y12*Y14*Y23-2.*Y12*Y13**2- 
     &  2.*Y12**2*Y13)/(4.*Y13*Y134*Y234*Y34) 
        WTC(IC)=WTC(IC)+(2.*Y12*Y34**2-2.*Y13*Y24*Y34+Y12*Y24*Y34+ 
     &  4.*Y13*Y23*Y34+4.*Y12*Y14*Y34+2.*Y12*Y13*Y34+2.*Y12**2*Y34- 
     &  Y13*Y24**2+3.*Y14*Y23*Y24+4.*Y13*Y23*Y24-2.*Y13*Y14*Y24+ 
     &  4.*Y12*Y14*Y24+2.*Y12*Y13*Y24+2.*Y14*Y23**2+4.*Y13*Y23**2+ 
     &  2.*Y13*Y14*Y23+2.*Y12*Y14*Y23+4.*Y12*Y13*Y23+2.*Y12*Y14**2+4.* 
     &  Y12**2*Y13+4.*Y12*Y13*Y14+2.*Y12**2*Y14)/(4.*Y13*Y134*Y24*Y34)- 
     &  (Y12*Y34**2-2.*Y14*Y24*Y34-2.*Y13*Y24*Y34-Y14*Y23*Y34+Y13*Y23* 
     &  Y34+Y12*Y14*Y34+2.*Y12*Y13*Y34-2.*Y14**2*Y24-4.*Y13*Y14*Y24- 
     &  4.*Y13**2*Y24-Y14**2*Y23-Y13**2*Y23+Y12*Y13*Y14-Y12*Y13**2)/ 
     &  (2.*Y13*Y34*Y134**2)+(Y12*Y34**2-4.*Y14*Y24*Y34-2.*Y13*Y24*Y34- 
     &  2.*Y14*Y23*Y34-4.*Y13*Y23*Y34-4.*Y12*Y14*Y34-4.*Y12*Y13*Y34- 
     &  2.*Y13*Y14*Y24+2.*Y13**2*Y24+2.*Y14**2*Y23-2.*Y13*Y14*Y23- 
     &  Y12*Y14**2-6.*Y12*Y13*Y14-Y12*Y13**2)/(4.*Y34**2*Y134**2) 
        WTTOT=WTTOT+Y34*CF*(CF*WTA(IC)+(CF-0.5*CN)*WTB(IC)+CN*WTC(IC))/ 
     &  8. 
      ELSE 
        WTD(IC)=(Y13*Y23*Y34+Y12*Y23*Y34-Y12**2*Y34+Y13*Y23*Y24+2.*Y12* 
     &  Y23*Y24-Y14*Y23**2+Y12*Y13*Y24+Y12*Y14*Y23+Y12*Y13*Y14)/(Y13**2* 
     &  Y123**2)-(Y12*Y34**2-Y13*Y24*Y34+Y12*Y24*Y34-Y14*Y23*Y34-Y12* 
     &  Y23*Y34-Y13*Y24**2+Y14*Y23*Y24-Y13*Y23*Y24-Y13**2*Y24+Y14* 
     &  Y23**2)/(Y13**2*Y123*Y134)+(Y13*Y14*Y12+Y34*Y14*Y12-Y34**2*Y12+ 
     &  Y13*Y14*Y24+2.*Y34*Y14*Y24-Y23*Y14**2+Y34*Y13*Y24+Y34*Y23*Y14+ 
     &  Y34*Y13*Y23)/(Y13**2*Y134**2)-(Y34*Y12**2-Y13*Y24*Y12+Y34*Y24* 
     &  Y12-Y23*Y14*Y12-Y34*Y14*Y12-Y13*Y24**2+Y23*Y14*Y24-Y13*Y14*Y24- 
     &  Y13**2*Y24+Y23*Y14**2)/(Y13**2*Y134*Y123) 
        WTE(IC)=(Y12*Y34*(Y23-Y24+Y14+Y13)+Y13*Y24**2-Y14*Y23*Y24+Y13* 
     &  Y23*Y24+Y13*Y14*Y24+Y13**2*Y24-Y14*Y23*(Y14+Y23+Y13))/(Y13*Y23* 
     &  Y123*Y134)-Y12*(Y12*Y34-Y23*Y24-Y13*Y24-Y14*Y23-Y14*Y13)/(Y13* 
     &  Y23*Y123**2)-(Y14+Y13)*(Y24+Y23)*Y34/(Y13*Y23*Y134*Y234)+ 
     &  (Y12*Y34*(Y14-Y24+Y23+Y13)+Y13*Y24**2-Y23*Y14*Y24+Y13*Y14*Y24+ 
     &  Y13*Y23*Y24+Y13**2*Y24-Y23*Y14*(Y14+Y23+Y13))/(Y13*Y14*Y134* 
     &  Y123)-Y34*(Y34*Y12-Y14*Y24-Y13*Y24-Y23*Y14-Y23*Y13)/(Y13*Y14* 
     &  Y134**2)-(Y23+Y13)*(Y24+Y14)*Y12/(Y13*Y14*Y123*Y124) 
        WTTOT=WTTOT+CF*(TR*WTD(IC)+(CF-0.5*CN)*WTE(IC))/16. 
      ENDIF 
 
C...Permutations of momenta in matrix element. Weighting. 
  130 IF(IC.EQ.1.OR.IC.EQ.3.OR.ID.EQ.2.OR.ID.EQ.3) THEN 
        YSAV=Y13 
        Y13=Y14 
        Y14=YSAV 
        YSAV=Y23 
        Y23=Y24 
        Y24=YSAV 
        YSAV=Y123 
        Y123=Y124 
        Y124=YSAV 
      ENDIF 
      IF(IC.EQ.2.OR.IC.EQ.4.OR.ID.EQ.3.OR.ID.EQ.4) THEN 
        YSAV=Y13 
        Y13=Y23 
        Y23=YSAV 
        YSAV=Y14 
        Y14=Y24 
        Y24=YSAV 
        YSAV=Y134 
        Y134=Y234 
        Y234=YSAV 
      ENDIF 
      IF(IC.LE.3) GOTO 120 
      IF(ID.EQ.1.AND.WTTOT.LT.RLU0(0)*WTMX) GOTO 110 
      IC=5 
 
C...qqgg events: string configuration and event type. 
      IF(IT.EQ.1) THEN 
        IF(MSTJ(109).EQ.0.AND.ID.EQ.1) THEN 
          PARJ(156)=Y34*(2.*(WTA(1)+WTA(2)+WTA(3)+WTA(4))+4.*(WTC(1)+ 
     &    WTC(2)+WTC(3)+WTC(4)))/(9.*WTTOT) 
          IF(WTA(2)+WTA(4)+2.*(WTC(2)+WTC(4)).GT.RLU0(0)*(WTA(1)+WTA(2)+ 
     &    WTA(3)+WTA(4)+2.*(WTC(1)+WTC(2)+WTC(3)+WTC(4)))) ID=2 
          IF(ID.EQ.2) GOTO 130 
        ELSEIF(MSTJ(109).EQ.2.AND.ID.EQ.1) THEN 
          PARJ(156)=Y34*(WTA(1)+WTA(2)+WTA(3)+WTA(4))/(8.*WTTOT) 
          IF(WTA(2)+WTA(4).GT.RLU0(0)*(WTA(1)+WTA(2)+WTA(3)+WTA(4)))ID=2 
          IF(ID.EQ.2) GOTO 130 
        ENDIF 
        MSTJ(120)=3 
        IF(MSTJ(109).EQ.0.AND.0.5*Y34*(WTC(1)+WTC(2)+WTC(3)+WTC(4)).GT. 
     &  RLU0(0)*WTTOT) MSTJ(120)=4 
        KFLN=21 
 
C...Mass cuts. Kinematical variables out. 
        IF(Y12.LE.CUT+QME) NJET=2 
        IF(NJET.EQ.2) GOTO 150 
        Q12=0.5*(1.-SQRT(1.-QME/Y12)) 
        X1=1.-(1.-Q12)*Y234-Q12*Y134 
        X4=1.-(1.-Q12)*Y134-Q12*Y234 
        X2=1.-Y124 
        X12=(1.-Q12)*Y13+Q12*Y23 
        X14=Y12-0.5*QME 
        IF(Y134*Y234/((1.-X1)*(1.-X4)).LE.RLU0(0)) NJET=2 
 
C...qqbarqqbar events: string configuration, choose new flavour. 
      ELSE 
        IF(ID.EQ.1) THEN 
          WTR=RLU0(0)*(WTD(1)+WTD(2)+WTD(3)+WTD(4)) 
          IF(WTR.LT.WTD(2)+WTD(3)+WTD(4)) ID=2 
          IF(WTR.LT.WTD(3)+WTD(4)) ID=3 
          IF(WTR.LT.WTD(4)) ID=4 
          IF(ID.GE.2) GOTO 130 
        ENDIF 
        MSTJ(120)=5 
        PARJ(156)=CF*TR*(WTD(1)+WTD(2)+WTD(3)+WTD(4))/(16.*WTTOT) 
  140   KFLN=1+INT(5.*RLU0(0)) 
        IF(KFLN.NE.KFL.AND.0.2*PARJ(156).LE.RLU0(0)) GOTO 140 
        IF(KFLN.EQ.KFL.AND.1.-0.8*PARJ(156).LE.RLU0(0)) GOTO 140 
        IF(KFLN.GT.MSTJ(104)) NJET=2 
        PMQN=ULMASS0(KFLN) 
        QMEN=(2.*PMQN/ECM)**2 
 
C...Mass cuts. Kinematical variables out. 
        IF(Y24.LE.CUT+QME.OR.Y13.LE.1.1*QMEN) NJET=2 
        IF(NJET.EQ.2) GOTO 150 
        Q24=0.5*(1.-SQRT(1.-QME/Y24)) 
        Q13=0.5*(1.-SQRT(1.-QMEN/Y13)) 
        X1=1.-(1.-Q24)*Y123-Q24*Y134 
        X4=1.-(1.-Q24)*Y134-Q24*Y123 
        X2=1.-(1.-Q13)*Y234-Q13*Y124 
        X12=(1.-Q24)*((1.-Q13)*Y14+Q13*Y34)+Q24*((1.-Q13)*Y12+Q13*Y23) 
        X14=Y24-0.5*QME 
        X34=(1.-Q24)*((1.-Q13)*Y23+Q13*Y12)+Q24*((1.-Q13)*Y34+Q13*Y14) 
        IF(PMQ**2+PMQN**2+MIN(X12,X34)*ECM**2.LE. 
     &  (PARJ(127)+PMQ+PMQN)**2) NJET=2 
        IF(Y123*Y134/((1.-X1)*(1.-X4)).LE.RLU0(0)) NJET=2 
      ENDIF 
  150 IF(MSTJ(101).LE.-2.AND.NJET.EQ.2) GOTO 100 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUXDIF0(NC,NJET,KFL,ECM,CHI,THE,PHI) 
 
C...Purpose: to give the angular orientation of events. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Charge. Factors depending on polarization for QED case. 
      QF=KCHG(KFL,1)/3. 
      POLL=1.-PARJ(131)*PARJ(132) 
      POLD=PARJ(132)-PARJ(131) 
      IF(MSTJ(102).LE.1.OR.MSTJ(109).EQ.1) THEN 
        HF1=POLL 
        HF2=0. 
        HF3=PARJ(133)**2 
        HF4=0. 
 
C...Factors depending on flavour, energy and polarization for QFD case. 
      ELSE 
        SFF=1./(16.*PARU(102)*(1.-PARU(102))) 
        SFW=ECM**4/((ECM**2-PARJ(123)**2)**2+(PARJ(123)*PARJ(124))**2) 
        SFI=SFW*(1.-(PARJ(123)/ECM)**2) 
        AE=-1. 
        VE=4.*PARU(102)-1. 
        AF=SIGN(1.,QF) 
        VF=AF-4.*QF*PARU(102) 
        HF1=QF**2*POLL-2.*QF*VF*SFI*SFF*(VE*POLL-AE*POLD)+ 
     &  (VF**2+AF**2)*SFW*SFF**2*((VE**2+AE**2)*POLL-2.*VE*AE*POLD) 
        HF2=-2.*QF*AF*SFI*SFF*(AE*POLL-VE*POLD)+2.*VF*AF*SFW*SFF**2* 
     &  (2.*VE*AE*POLL-(VE**2+AE**2)*POLD) 
        HF3=PARJ(133)**2*(QF**2-2.*QF*VF*SFI*SFF*VE+(VF**2+AF**2)* 
     &  SFW*SFF**2*(VE**2-AE**2)) 
        HF4=-PARJ(133)**2*2.*QF*VF*SFW*(PARJ(123)*PARJ(124)/ECM**2)* 
     &  SFF*AE 
      ENDIF 
 
C...Mass factor. Differential cross-sections for two-jet events. 
      SQ2=SQRT(2.) 
      QME=0. 
      IF(MSTJ(103).GE.4.AND.IABS(MSTJ(101)).LE.1.AND.MSTJ(102).LE.1.AND. 
     &MSTJ(109).NE.1) QME=(2.*ULMASS0(KFL)/ECM)**2 
      IF(NJET.EQ.2) THEN 
        SIGU=4.*SQRT(1.-QME) 
        SIGL=2.*QME*SQRT(1.-QME) 
        SIGT=0. 
        SIGI=0. 
        SIGA=0. 
        SIGP=4. 
 
C...Kinematical variables. Reduce four-jet event to three-jet one. 
      ELSE 
        IF(NJET.EQ.3) THEN 
          X1=2.*P(NC+1,4)/ECM 
          X2=2.*P(NC+3,4)/ECM 
        ELSE 
          ECMR=P(NC+1,4)+P(NC+4,4)+SQRT((P(NC+2,1)+P(NC+3,1))**2+ 
     &    (P(NC+2,2)+P(NC+3,2))**2+(P(NC+2,3)+P(NC+3,3))**2) 
          X1=2.*P(NC+1,4)/ECMR 
          X2=2.*P(NC+4,4)/ECMR 
        ENDIF 
 
C...Differential cross-sections for three-jet (or reduced four-jet). 
        XQ=(1.-X1)/(1.-X2) 
        CT12=(X1*X2-2.*X1-2.*X2+2.+QME)/SQRT((X1**2-QME)*(X2**2-QME)) 
        ST12=SQRT(1.-CT12**2) 
        IF(MSTJ(109).NE.1) THEN 
          SIGU=2.*X1**2+X2**2*(1.+CT12**2)-QME*(3.+CT12**2-X1-X2)- 
     &    QME*X1/XQ+0.5*QME*((X2**2-QME)*ST12**2-2.*X2)*XQ 
          SIGL=(X2*ST12)**2-QME*(3.-CT12**2-2.5*(X1+X2)+X1*X2+QME)+ 
     &    0.5*QME*(X1**2-X1-QME)/XQ+0.5*QME*((X2**2-QME)*CT12**2-X2)*XQ 
          SIGT=0.5*(X2**2-QME-0.5*QME*(X2**2-QME)/XQ)*ST12**2 
          SIGI=((1.-0.5*QME*XQ)*(X2**2-QME)*ST12*CT12+QME*(1.-X1-X2+ 
     &    0.5*X1*X2+0.5*QME)*ST12/CT12)/SQ2 
          SIGA=X2**2*ST12/SQ2 
          SIGP=2.*(X1**2-X2**2*CT12) 
 
C...Differential cross-sect for scalar gluons (no mass effects). 
        ELSE 
          X3=2.-X1-X2 
          XT=X2*ST12 
          CT13=SQRT(MAX(0.,1.-(XT/X3)**2)) 
          SIGU=(1.-PARJ(171))*(X3**2-0.5*XT**2)+ 
     &    PARJ(171)*(X3**2-0.5*XT**2-4.*(1.-X1)*(1.-X2)**2/X1) 
          SIGL=(1.-PARJ(171))*0.5*XT**2+ 
     &    PARJ(171)*0.5*(1.-X1)**2*XT**2 
          SIGT=(1.-PARJ(171))*0.25*XT**2+ 
     &    PARJ(171)*0.25*XT**2*(1.-2.*X1) 
          SIGI=-(0.5/SQ2)*((1.-PARJ(171))*XT*X3*CT13+ 
     &    PARJ(171)*XT*((1.-2.*X1)*X3*CT13-X1*(X1-X2))) 
          SIGA=(0.25/SQ2)*XT*(2.*(1.-X1)-X1*X3) 
          SIGP=X3**2-2.*(1.-X1)*(1.-X2)/X1 
        ENDIF 
      ENDIF 
 
C...Upper bounds for differential cross-section. 
      HF1A=ABS(HF1) 
      HF2A=ABS(HF2) 
      HF3A=ABS(HF3) 
      HF4A=ABS(HF4) 
      SIGMAX=(2.*HF1A+HF3A+HF4A)*ABS(SIGU)+2.*(HF1A+HF3A+HF4A)* 
     &ABS(SIGL)+2.*(HF1A+2.*HF3A+2.*HF4A)*ABS(SIGT)+2.*SQ2* 
     &(HF1A+2.*HF3A+2.*HF4A)*ABS(SIGI)+4.*SQ2*HF2A*ABS(SIGA)+ 
     &2.*HF2A*ABS(SIGP) 
 
C...Generate angular orientation according to differential cross-sect. 
  100 CHI=PARU(2)*RLU0(0) 
      CTHE=2.*RLU0(0)-1. 
      PHI=PARU(2)*RLU0(0) 
      CCHI=COS(CHI) 
      SCHI=SIN(CHI) 
      C2CHI=COS(2.*CHI) 
      S2CHI=SIN(2.*CHI) 
      THE=ACOS(CTHE) 
      STHE=SIN(THE) 
      C2PHI=COS(2.*(PHI-PARJ(134))) 
      S2PHI=SIN(2.*(PHI-PARJ(134))) 
      SIG=((1.+CTHE**2)*HF1+STHE**2*(C2PHI*HF3-S2PHI*HF4))*SIGU+ 
     &2.*(STHE**2*HF1-STHE**2*(C2PHI*HF3-S2PHI*HF4))*SIGL+ 
     &2.*(STHE**2*C2CHI*HF1+((1.+CTHE**2)*C2CHI*C2PHI-2.*CTHE*S2CHI* 
     &S2PHI)*HF3-((1.+CTHE**2)*C2CHI*S2PHI+2.*CTHE*S2CHI*C2PHI)*HF4)* 
     &SIGT-2.*SQ2*(2.*STHE*CTHE*CCHI*HF1-2.*STHE*(CTHE*CCHI*C2PHI- 
     &SCHI*S2PHI)*HF3+2.*STHE*(CTHE*CCHI*S2PHI+SCHI*C2PHI)*HF4)*SIGI+ 
     &4.*SQ2*STHE*CCHI*HF2*SIGA+2.*CTHE*HF2*SIGP 
      IF(SIG.LT.SIGMAX*RLU0(0)) GOTO 100 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUONIA0(KFL,ECM) 
 
C...Purpose: to generate Upsilon and toponium decays into three 
C...gluons or two gluons and a photon. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Printout. Check input parameters. 
      IF(MSTU(12).GE.1) CALL LULIST0(0) 
      IF(KFL.LT.0.OR.KFL.GT.8) THEN 
        CALL LUERRM0(16,'(LUONIA0:) called with unknown flavour code') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
      IF(ECM.LT.PARJ(127)+2.02*PARF(101)) THEN 
        CALL LUERRM0(16,'(LUONIA0:) called with too small CM energy') 
        IF(MSTU(21).GE.1) RETURN 
      ENDIF 
 
C...Initial e+e- and onium state (optional). 
      NC=0 
      IF(MSTJ(115).GE.2) THEN 
        NC=NC+2 
        CALL LU1ENT0(NC-1,11,0.5*ECM,0.,0.) 
        K(NC-1,1)=21 
        CALL LU1ENT0(NC,-11,0.5*ECM,PARU(1),0.) 
        K(NC,1)=21 
      ENDIF 
      KFLC=IABS(KFL) 
      IF(MSTJ(115).GE.3.AND.KFLC.GE.5) THEN 
        NC=NC+1 
        KF=110*KFLC+3 
        MSTU10=MSTU(10) 
        MSTU(10)=1 
        P(NC,5)=ECM 
        CALL LU1ENT0(NC,KF,ECM,0.,0.) 
        K(NC,1)=21 
        K(NC,3)=1 
        MSTU(10)=MSTU10 
      ENDIF 
 
C...Choose x1 and x2 according to matrix element. 
      NTRY=0 
  100 X1=RLU0(0) 
      X2=RLU0(0) 
      X3=2.-X1-X2 
      IF(X3.GE.1..OR.((1.-X1)/(X2*X3))**2+((1.-X2)/(X1*X3))**2+ 
     &((1.-X3)/(X1*X2))**2.LE.2.*RLU0(0)) GOTO 100 
      NTRY=NTRY+1 
      NJET=3 
      IF(MSTJ(101).LE.4) CALL LU3ENT0(NC+1,21,21,21,ECM,X1,X3) 
      IF(MSTJ(101).GE.5) CALL LU3ENT0(-(NC+1),21,21,21,ECM,X1,X3) 
 
C...Photon-gluon-gluon events. Small system modifications. Jet origin. 
      MSTU(111)=MSTJ(108) 
      IF(MSTJ(108).EQ.2.AND.(MSTJ(101).EQ.0.OR.MSTJ(101).EQ.1)) 
     &MSTU(111)=1 
      PARU(112)=PARJ(121) 
      IF(MSTU(111).EQ.2) PARU(112)=PARJ(122) 
      QF=0. 
      IF(KFLC.NE.0) QF=KCHG(KFLC,1)/3. 
      RGAM=7.2*QF**2*PARU(101)/ULALPS0(ECM**2) 
      MK=0 
      ECMC=ECM 
      IF(RLU0(0).GT.RGAM/(1.+RGAM)) THEN 
        IF(1.-MAX(X1,X2,X3).LE.MAX((PARJ(126)/ECM)**2,PARJ(125))) 
     &  NJET=2 
        IF(NJET.EQ.2.AND.MSTJ(101).LE.4) CALL LU2ENT0(NC+1,21,21,ECM) 
        IF(NJET.EQ.2.AND.MSTJ(101).GE.5) CALL LU2ENT0(-(NC+1),21,21,ECM) 
      ELSE 
        MK=1 
        ECMC=SQRT(1.-X1)*ECM 
        IF(ECMC.LT.2.*PARJ(127)) GOTO 100 
        K(NC+1,1)=1 
        K(NC+1,2)=22 
        K(NC+1,4)=0 
        K(NC+1,5)=0 
        IF(MSTJ(101).GE.5) K(NC+2,4)=MSTU(5)*(NC+3) 
        IF(MSTJ(101).GE.5) K(NC+2,5)=MSTU(5)*(NC+3) 
        IF(MSTJ(101).GE.5) K(NC+3,4)=MSTU(5)*(NC+2) 
        IF(MSTJ(101).GE.5) K(NC+3,5)=MSTU(5)*(NC+2) 
        NJET=2 
        IF(ECMC.LT.4.*PARJ(127)) THEN 
          MSTU10=MSTU(10) 
          MSTU(10)=1 
          P(NC+2,5)=ECMC 
          CALL LU1ENT0(NC+2,83,0.5*(X2+X3)*ECM,PARU(1),0.) 
          MSTU(10)=MSTU10 
          NJET=0 
        ENDIF 
      ENDIF 
      DO 110 IP=NC+1,N 
      K(IP,3)=K(IP,3)+(MSTJ(115)/2)+(KFLC/5)*(MSTJ(115)/3)*(NC-1) 
  110 CONTINUE 
 
C...Differential cross-sections. Upper limit for cross-section. 
      IF(MSTJ(106).EQ.1) THEN 
        SQ2=SQRT(2.) 
        HF1=1.-PARJ(131)*PARJ(132) 
        HF3=PARJ(133)**2 
        CT13=(X1*X3-2.*X1-2.*X3+2.)/(X1*X3) 
        ST13=SQRT(1.-CT13**2) 
        SIGL=0.5*X3**2*((1.-X2)**2+(1.-X3)**2)*ST13**2 
        SIGU=(X1*(1.-X1))**2+(X2*(1.-X2))**2+(X3*(1.-X3))**2-SIGL 
        SIGT=0.5*SIGL 
        SIGI=(SIGL*CT13/ST13+0.5*X1*X3*(1.-X2)**2*ST13)/SQ2 
        SIGMAX=(2.*HF1+HF3)*ABS(SIGU)+2.*(HF1+HF3)*ABS(SIGL)+2.*(HF1+ 
     &  2.*HF3)*ABS(SIGT)+2.*SQ2*(HF1+2.*HF3)*ABS(SIGI) 
 
C...Angular orientation of event. 
  120   CHI=PARU(2)*RLU0(0) 
        CTHE=2.*RLU0(0)-1. 
        PHI=PARU(2)*RLU0(0) 
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
        IF(SIG.LT.SIGMAX*RLU0(0)) GOTO 120 
        CALL LUDBRB0(NC+1,N,0.,CHI,0D0,0D0,0D0) 
        CALL LUDBRB0(NC+1,N,THE,PHI,0D0,0D0,0D0) 
      ENDIF 
 
C...Generate parton shower. Rearrange along strings and check. 
      IF(MSTJ(101).GE.5.AND.NJET.GE.2) THEN 
        CALL LUSHOW0(NC+MK+1,-NJET,ECMC) 
        MSTJ14=MSTJ(14) 
        IF(MSTJ(105).EQ.-1) MSTJ(14)=-1 
        IF(MSTJ(105).GE.0) MSTU(28)=0 
        CALL LUPREP0(0) 
        MSTJ(14)=MSTJ14 
        IF(MSTJ(105).GE.0.AND.MSTU(28).NE.0) GOTO 100 
      ENDIF 
 
C...Generate fragmentation. Information for LUTABU0: 
      IF(MSTJ(105).EQ.1) CALL LUEXEC0 
      MSTU(161)=110*KFLC+3 
      MSTU(162)=0 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUHEPC0(MCONV) 
 
C...Purpose: to convert JETSET event record contents to or from 
C...the standard event record commonblock. 
C...Note that HEPEVT is in double precision according to LEP 2 standard.
      PARAMETER (NMXHEP=2000) 
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP), 
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP) 
      DOUBLE PRECISION PHEP,VHEP
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      SAVE /HEPEVT/ 
      SAVE /LUJETS/,/LUDAT01/,/LUDAT02/ 
 
C...Conversion from JETSET to standard, the easy part. 
      IF(MCONV.EQ.1) THEN 
        NEVHEP=0 
        IF(N.GT.NMXHEP) CALL LUERRM0(8, 
     &  '(LUHEPC0:) no more space in /HEPEVT/') 
        NHEP=MIN(N,NMXHEP) 
        DO 140 I=1,NHEP 
        ISTHEP(I)=0 
        IF(K(I,1).GE.1.AND.K(I,1).LE.10) ISTHEP(I)=1 
        IF(K(I,1).GE.11.AND.K(I,1).LE.20) ISTHEP(I)=2 
        IF(K(I,1).GE.21.AND.K(I,1).LE.30) ISTHEP(I)=3 
        IF(K(I,1).GE.31.AND.K(I,1).LE.100) ISTHEP(I)=K(I,1) 
        IDHEP(I)=K(I,2) 
        JMOHEP(1,I)=K(I,3) 
        JMOHEP(2,I)=0 
        IF(K(I,1).NE.3.AND.K(I,1).NE.13.AND.K(I,1).NE.14) THEN 
          JDAHEP(1,I)=K(I,4) 
          JDAHEP(2,I)=K(I,5) 
        ELSE 
          JDAHEP(1,I)=0 
          JDAHEP(2,I)=0 
        ENDIF 
        DO 100 J=1,5 
        PHEP(J,I)=P(I,J) 
  100   CONTINUE 
        DO 110 J=1,4 
        VHEP(J,I)=V(I,J) 
  110   CONTINUE 
 
C...Check if new event (from pileup). 
        IF(I.EQ.1) THEN 
          INEW=1 
        ELSE 
          IF(K(I,1).EQ.21.AND.K(I-1,1).NE.21) INEW=I 
        ENDIF 
 
C...Fill in missing mother information. 
        IF(I.GE.INEW+2.AND.K(I,1).EQ.21.AND.K(I,3).EQ.0) THEN 
          IMO1=I-2 
          IF(I.GE.INEW+3.AND.K(I-1,1).EQ.21.AND.K(I-1,3).EQ.0) 
     &    IMO1=IMO1-1 
          JMOHEP(1,I)=IMO1 
          JMOHEP(2,I)=IMO1+1 
        ELSEIF(K(I,2).GE.91.AND.K(I,2).LE.93) THEN 
          I1=K(I,3)-1 
  120     I1=I1+1 
          IF(I1.GE.I) CALL LUERRM0(8, 
     &    '(LUHEPC0:) translation of inconsistent event history') 
          IF(I1.LT.I.AND.K(I1,1).NE.1.AND.K(I1,1).NE.11) GOTO 120 
          KC=LUCOMP0(K(I1,2)) 
          IF(I1.LT.I.AND.KC.EQ.0) GOTO 120 
          IF(I1.LT.I.AND.KCHG(KC,2).EQ.0) GOTO 120 
          JMOHEP(2,I)=I1 
        ELSEIF(K(I,2).EQ.94) THEN 
          NJET=2 
          IF(NHEP.GE.I+3.AND.K(I+3,3).LE.I) NJET=3 
          IF(NHEP.GE.I+4.AND.K(I+4,3).LE.I) NJET=4 
          JMOHEP(2,I)=MOD(K(I+NJET,4)/MSTU(5),MSTU(5)) 
          IF(JMOHEP(2,I).EQ.JMOHEP(1,I)) JMOHEP(2,I)= 
     &    MOD(K(I+1,4)/MSTU(5),MSTU(5)) 
        ENDIF 
 
C...Fill in missing daughter information. 
        IF(K(I,2).EQ.94.AND.MSTU(16).NE.2) THEN 
          DO 130 I1=JDAHEP(1,I),JDAHEP(2,I) 
          I2=MOD(K(I1,4)/MSTU(5),MSTU(5)) 
          JDAHEP(1,I2)=I 
  130     CONTINUE 
        ENDIF 
        IF(K(I,2).GE.91.AND.K(I,2).LE.94) GOTO 140 
        I1=JMOHEP(1,I) 
        IF(I1.LE.0.OR.I1.GT.NHEP) GOTO 140 
        IF(K(I1,1).NE.13.AND.K(I1,1).NE.14) GOTO 140 
        IF(JDAHEP(1,I1).EQ.0) THEN 
          JDAHEP(1,I1)=I 
        ELSE 
          JDAHEP(2,I1)=I 
        ENDIF 
  140   CONTINUE 
        DO 150 I=1,NHEP 
        IF(K(I,1).NE.13.AND.K(I,1).NE.14) GOTO 150 
        IF(JDAHEP(2,I).EQ.0) JDAHEP(2,I)=JDAHEP(1,I) 
  150   CONTINUE 
 
C...Conversion from standard to JETSET, the easy part. 
      ELSE 
        IF(NHEP.GT.MSTU(4)) CALL LUERRM0(8, 
     &  '(LUHEPC0:) no more space in /LUJETS/') 
        N=MIN(NHEP,MSTU(4)) 
        NKQ=0 
        KQSUM=0 
        DO 180 I=1,N 
        K(I,1)=0 
        IF(ISTHEP(I).EQ.1) K(I,1)=1 
        IF(ISTHEP(I).EQ.2) K(I,1)=11 
        IF(ISTHEP(I).EQ.3) K(I,1)=21 
        K(I,2)=IDHEP(I) 
        K(I,3)=JMOHEP(1,I) 
        K(I,4)=JDAHEP(1,I) 
        K(I,5)=JDAHEP(2,I) 
        DO 160 J=1,5 
        P(I,J)=PHEP(J,I) 
  160   CONTINUE 
        DO 170 J=1,4 
        V(I,J)=VHEP(J,I) 
  170   CONTINUE 
        V(I,5)=0. 
        IF(ISTHEP(I).EQ.2.AND.PHEP(4,I).GT.PHEP(5,I)) THEN 
          I1=JDAHEP(1,I) 
          IF(I1.GT.0.AND.I1.LE.NHEP) V(I,5)=(VHEP(4,I1)-VHEP(4,I))* 
     &    PHEP(5,I)/PHEP(4,I) 
        ENDIF 
 
C...Fill in missing information on colour connection in jet systems. 
        IF(ISTHEP(I).EQ.1) THEN 
          KC=LUCOMP0(K(I,2)) 
          KQ=0 
          IF(KC.NE.0) KQ=KCHG(KC,2)*ISIGN(1,K(I,2)) 
          IF(KQ.NE.0) NKQ=NKQ+1 
          IF(KQ.NE.2) KQSUM=KQSUM+KQ 
          IF(KQ.NE.0.AND.KQSUM.NE.0) THEN 
            K(I,1)=2 
          ELSEIF(KQ.EQ.2.AND.I.LT.N) THEN 
            IF(K(I+1,2).EQ.21) K(I,1)=2 
          ENDIF 
        ENDIF 
  180   CONTINUE 
        IF(NKQ.EQ.1.OR.KQSUM.NE.0) CALL LUERRM0(8, 
     &  '(LUHEPC0:) input parton configuration not colour singlet') 
      ENDIF 
 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUTEST0(MTEST) 
 
C...Purpose: to provide a simple program (disguised as subroutine) to 
C...run at installation as a check that the program works as intended. 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUJETS/,/LUDAT01/ 
      DIMENSION PSUM(5),PINI(6),PFIN(6) 
 
C...Loop over events to be generated. 
      IF(MTEST.GE.1) CALL LUTABU0(20) 
      NERR=0 
      DO 180 IEV=1,600 
 
C...Reset parameter values. Switch on some nonstandard features. 
      MSTJ(1)=1 
      MSTJ(3)=0 
      MSTJ(11)=1 
      MSTJ(42)=2 
      MSTJ(43)=4 
      MSTJ(44)=2 
      PARJ(17)=0.1 
      PARJ(22)=1.5 
      PARJ(43)=1. 
      PARJ(54)=-0.05 
      MSTJ(101)=5 
      MSTJ(104)=5 
      MSTJ(105)=0 
      MSTJ(107)=1 
      IF(IEV.EQ.301.OR.IEV.EQ.351.OR.IEV.EQ.401) MSTJ(116)=3 
 
C...Ten events each for some single jets configurations. 
      IF(IEV.LE.50) THEN 
        ITY=(IEV+9)/10 
        MSTJ(3)=-1 
        IF(ITY.EQ.3.OR.ITY.EQ.4) MSTJ(11)=2 
        IF(ITY.EQ.1) CALL LU1ENT0(1,1,15.,0.,0.) 
        IF(ITY.EQ.2) CALL LU1ENT0(1,3101,15.,0.,0.) 
        IF(ITY.EQ.3) CALL LU1ENT0(1,-2203,15.,0.,0.) 
        IF(ITY.EQ.4) CALL LU1ENT0(1,-4,30.,0.,0.) 
        IF(ITY.EQ.5) CALL LU1ENT0(1,21,15.,0.,0.) 
 
C...Ten events each for some simple jet systems; string fragmentation. 
      ELSEIF(IEV.LE.130) THEN 
        ITY=(IEV-41)/10 
        IF(ITY.EQ.1)CALL LU2ENT0(1,1,-1,40.) 
        IF(ITY.EQ.2)CALL LU2ENT0(1,4,-4,30.) 
        IF(ITY.EQ.3)CALL LU2ENT0(1,2,2103,100.) 
        IF(ITY.EQ.4)CALL LU2ENT0(1,21,21,40.) 
        IF(ITY.EQ.5)CALL LU3ENT0(1,2101,21,-3203,30.,0.6,0.8) 
        IF(ITY.EQ.6)CALL LU3ENT0(1,5,21,-5,40.,0.9,0.8) 
        IF(ITY.EQ.7)CALL LU3ENT0(1,21,21,21,60.,0.7,0.5) 
        IF(ITY.EQ.8)CALL LU4ENT0(1,2,21,21,-2,40.,0.4,0.64,0.6,0.12,0.2) 
 
C...Seventy events with independent fragmentation and momentum cons. 
      ELSEIF(IEV.LE.200) THEN 
        ITY=1+(IEV-131)/16 
        MSTJ(2)=1+MOD(IEV-131,4) 
        MSTJ(3)=1+MOD((IEV-131)/4,4) 
        IF(ITY.EQ.1)CALL LU2ENT0(1,4,-5,40.) 
        IF(ITY.EQ.2)CALL LU3ENT0(1,3,21,-3,40.,0.9,0.4) 
        IF(ITY.EQ.3)CALL LU4ENT0(1,2,21,21,-2,40.,0.4,0.64,0.6,0.12,0.2) 
        IF(ITY.GE.4)CALL LU4ENT0(1,2,-3,3,-2,40.,0.4,0.64,0.6,0.12,0.2) 
 
C...A hundred events with random jets (check invariant mass). 
      ELSEIF(IEV.LE.300) THEN 
  100   DO 110 J=1,5 
        PSUM(J)=0. 
  110   CONTINUE 
        NJET=2.+6.*RLU0(0) 
        DO 130 I=1,NJET 
        KFL=21 
        IF(I.EQ.1) KFL=INT(1.+4.*RLU0(0)) 
        IF(I.EQ.NJET) KFL=-INT(1.+4.*RLU0(0)) 
        EJET=5.+20.*RLU0(0) 
        THETA=ACOS(2.*RLU0(0)-1.) 
        PHI=6.2832*RLU0(0) 
        IF(I.LT.NJET) CALL LU1ENT0(-I,KFL,EJET,THETA,PHI) 
        IF(I.EQ.NJET) CALL LU1ENT0(I,KFL,EJET,THETA,PHI) 
        IF(I.EQ.1.OR.I.EQ.NJET) MSTJ(93)=1 
        IF(I.EQ.1.OR.I.EQ.NJET) PSUM(5)=PSUM(5)+ULMASS0(KFL) 
        DO 120 J=1,4 
        PSUM(J)=PSUM(J)+P(I,J) 
  120   CONTINUE 
  130   CONTINUE 
        IF(PSUM(4)**2-PSUM(1)**2-PSUM(2)**2-PSUM(3)**2.LT. 
     &  (PSUM(5)+PARJ(32))**2) GOTO 100 
 
C...Fifty e+e- continuum events with matrix elements. 
      ELSEIF(IEV.LE.350) THEN 
        MSTJ(101)=2 
        CALL LUEEVT0(0,40.) 
 
C...Fifty e+e- continuum event with varying shower options. 
      ELSEIF(IEV.LE.400) THEN 
        MSTJ(42)=1+MOD(IEV,2) 
        MSTJ(43)=1+MOD(IEV/2,4) 
        MSTJ(44)=MOD(IEV/8,3) 
        CALL LUEEVT0(0,90.) 
 
C...Fifty e+e- continuum events with coherent shower, including top. 
      ELSEIF(IEV.LE.450) THEN 
        MSTJ(104)=6 
        CALL LUEEVT0(0,500.) 
 
C...Fifty Upsilon decays to ggg or gammagg with coherent shower. 
      ELSEIF(IEV.LE.500) THEN 
        CALL LUONIA0(5,9.46) 
 
C...One decay each for some heavy mesons. 
      ELSEIF(IEV.LE.560) THEN 
        ITY=IEV-501 
        KFLS=2*(ITY/20)+1 
        KFLB=8-MOD(ITY/5,4) 
        KFLC=KFLB-MOD(ITY,5) 
        CALL LU1ENT0(1,100*KFLB+10*KFLC+KFLS,0.,0.,0.) 
 
C...One decay each for some heavy baryons. 
      ELSEIF(IEV.LE.600) THEN 
        ITY=IEV-561 
        KFLS=2*(ITY/20)+2 
        KFLA=8-MOD(ITY/5,4) 
        KFLB=KFLA-MOD(ITY,5) 
        KFLC=MAX(1,KFLB-1) 
        CALL LU1ENT0(1,1000*KFLA+100*KFLB+10*KFLC+KFLS,0.,0.,0.) 
      ENDIF 
 
C...Generate event. Find total momentum, energy and charge. 
      DO 140 J=1,4 
      PINI(J)=PLU0(0,J) 
  140 CONTINUE 
      PINI(6)=PLU0(0,6) 
      CALL LUEXEC0 
      DO 150 J=1,4 
      PFIN(J)=PLU0(0,J) 
  150 CONTINUE 
      PFIN(6)=PLU0(0,6) 
 
C...Check conservation of energy, momentum and charge; 
C...usually exact, but only approximate for single jets. 
      MERR=0 
      IF(IEV.LE.50) THEN 
        IF((PFIN(1)-PINI(1))**2+(PFIN(2)-PINI(2))**2.GE.4.) MERR=MERR+1 
        EPZREM=PINI(4)+PINI(3)-PFIN(4)-PFIN(3) 
        IF(EPZREM.LT.0..OR.EPZREM.GT.2.*PARJ(31)) MERR=MERR+1 
        IF(ABS(PFIN(6)-PINI(6)).GT.2.1) MERR=MERR+1 
      ELSE 
        DO 160 J=1,4 
        IF(ABS(PFIN(J)-PINI(J)).GT.0.0001*PINI(4)) MERR=MERR+1 
  160   CONTINUE 
        IF(ABS(PFIN(6)-PINI(6)).GT.0.1) MERR=MERR+1 
      ENDIF 
      IF(MERR.NE.0) WRITE(MSTU(11),5000) (PINI(J),J=1,4),PINI(6), 
     &(PFIN(J),J=1,4),PFIN(6) 
 
C...Check that all KF codes are known ones, and that partons/particles 
C...satisfy energy-momentum-mass relation. Store particle statistics. 
      DO 170 I=1,N 
      IF(K(I,1).GT.20) GOTO 170 
      IF(LUCOMP0(K(I,2)).EQ.0) THEN 
        WRITE(MSTU(11),5100) I 
        MERR=MERR+1 
      ENDIF 
      PD=P(I,4)**2-P(I,1)**2-P(I,2)**2-P(I,3)**2-P(I,5)**2 
      IF(ABS(PD).GT.MAX(0.1,0.001*P(I,4)**2).OR.P(I,4).LT.0.) THEN 
        WRITE(MSTU(11),5200) I 
        MERR=MERR+1 
      ENDIF 
  170 CONTINUE 
      IF(MTEST.GE.1) CALL LUTABU0(21) 
 
C...List all erroneous events and some normal ones. 
      IF(MERR.NE.0.OR.MSTU(24).NE.0.OR.MSTU(28).NE.0) THEN 
        CALL LULIST0(2) 
      ELSEIF(MTEST.GE.1.AND.MOD(IEV-5,100).EQ.0) THEN 
        CALL LULIST0(1) 
      ENDIF 
 
C...Stop execution if too many errors. 
      IF(MERR.NE.0) NERR=NERR+1 
      IF(NERR.GE.10) THEN 
        WRITE(MSTU(11),5300) IEV 
        STOP 
      ENDIF 
  180 CONTINUE 
 
C...Summarize result of run. 
      IF(MTEST.GE.1) CALL LUTABU0(22) 
      IF(NERR.EQ.0) WRITE(MSTU(11),5400) 
      IF(NERR.GT.0) WRITE(MSTU(11),5500) NERR 
 
C...Reset commonblock variables changed during run. 
      MSTJ(2)=3 
      PARJ(17)=0. 
      PARJ(22)=1. 
      PARJ(43)=0.5 
      PARJ(54)=0. 
      MSTJ(105)=1 
      MSTJ(107)=0 
 
C...Format statements for output. 
 5000 FORMAT(/' Momentum, energy and/or charge were not conserved ', 
     &'in following event'/' sum of',9X,'px',11X,'py',11X,'pz',11X, 
     &'E',8X,'charge'/' before',2X,4(1X,F12.5),1X,F8.2/' after',3X, 
     &4(1X,F12.5),1X,F8.2) 
 5100 FORMAT(/5X,'Entry no.',I4,' in following event not known code') 
 5200 FORMAT(/5X,'Entry no.',I4,' in following event has faulty ', 
     &'kinematics') 
 5300 FORMAT(/5X,'Ten errors experienced by event ',I3/ 
     &5X,'Something is seriously wrong! Execution stopped now!') 
 5400 FORMAT(//5X,'End result of LUTEST0: no errors detected.') 
 5500 FORMAT(//5X,'End result of LUTEST0:',I2,' errors detected.'/ 
     &5X,'This should not have happened!') 
 
      RETURN 
      END 
 
C********************************************************************* 
 
      BLOCK DATA LUDATA0 
 
C...Purpose: to give default values to parameters and particle and 
C...decay data. 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      COMMON/LUDAT02/KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4) 
      COMMON/LUDAT03/MDCY(500,3),MDME(2000,2),BRAT(2000),KFDP(2000,5) 
      COMMON/LUDAT04/CHAF(500) 
      CHARACTER CHAF*8 
      COMMON/LUDAT0R/MRLU0(6),RRLU0(100) 
      SAVE /LUDAT01/,/LUDAT02/,/LUDAT03/,/LUDAT04/,/LUDAT0R/ 
 
C...LUDAT1, containing status codes and most parameters. 
      DATA MSTU/ 
     &    0,    0,    0, 4000,10000,  500, 2000,    0,    0,    2, 
     1    6,    1,    1,    0,    1,    1,    0,    0,    0,    0, 
     2    2,   10,    0,    0,    1,   10,    0,    0,    0,    0, 
     3    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
     4    2,    2,    1,    4,    2,    1,    1,    0,    0,    0, 
     5   25,   24,    0,    1,    0,    0,    0,    0,    0,    0, 
     6    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
     7  30*0, 
     &    1,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
     1    1,    5,    3,    5,    0,    0,    0,    0,    0,    0, 
     2  60*0, 
     8    7,  410, 1997,   01,   20,  700,    0,    0,    0,    0, 
     9    0,    0,    0,    0,    0,    0,    0,    0,    0,    0/ 
      DATA PARU/ 
     & 3.1415927, 6.2831854, 0.1973, 5.068, 0.3894, 2.568,   4*0., 
     1 0.001, 0.09, 0.01,  0.,   0.,   0.,   0.,   0.,   0.,   0., 
     2   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     3   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     4  2.0,  1.0, 0.25,  2.5, 0.05,   0.,   0., 0.0001, 0.,   0., 
     5  2.5,  1.5,  7.0,  1.0,  0.5,  2.0,  3.2,   0.,   0.,   0., 
     6  40*0., 
     & 0.00729735, 0.232, 0.007764, 1.0, 1.16639E-5, 0., 0., 0., 
     &   0.,   0., 
     1 0.20, 0.25,  1.0,  4.0,  10.,   0.,   0.,   0.,   0.,   0., 
     2 -0.693, -1.0, 0.387, 1.0, -0.08, -1.0, 1.0, 1.0, 1.0,   0., 
     3  1.0, -1.0,  1.0, -1.0,  1.0,   0.,   0.,   0.,   0.,   0., 
     4  5.0,  1.0,  1.0,   0.,  1.0,  1.0,   0.,   0.,   0.,   0., 
     5  1.0,   0.,   0.,   0., 1000., 1.0,  1.0,  1.0,  1.0,   0., 
     6  1.0,  1.0,  1.0,  1.0,  1.0,   0.,   0.,   0.,   0.,   0., 
     7  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,   0.,   0.,   0., 
     8  1.0,  1.0,  1.0,  0.0,  0.0,  1.0,  1.0,  0.0,  0.0,   0., 
     9   0.,   0.,   0.,   0.,  1.0,   0.,   0.,   0.,   0.,   0./ 
      DATA MSTJ/ 
     &    1,    3,    0,    0,    0,    0,    0,    0,    0,    0, 
     1    4,    2,    0,    1,    0,    0,    0,    0,    0,    0, 
     2    2,    1,    1,    2,    1,    2,    2,    0,    0,    0, 
     3    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
     4    2,    2,    4,    2,    5,    3,    3,    0,    0,    3, 
     5    0,    3,    0,    0,    0,    0,    0,    0,    0,    0, 
     6  40*0, 
     &    5,    2,    7,    5,    1,    1,    0,    2,    0,    2, 
     1    0,    0,    0,    0,    1,    1,    0,    0,    0,    0, 
     2  80*0/ 
      DATA PARJ/ 
     & 0.10, 0.30, 0.40, 0.05, 0.50, 0.50, 0.50,   0.,   0.,   0., 
     1 0.50, 0.60, 0.75,   0.,   0.,   0.,   0.,  1.0,  1.0,   0., 
     2 0.36,  1.0, 0.01,  2.0,  1.0,  0.4,   0.,   0.,   0.,   0., 
     3 0.10,  1.0,  0.8,  1.5,   0.,  2.0,  0.2,  2.5,  0.6,   0., 
     4  0.3, 0.58,  0.5,  0.9,  0.5,  1.0,  1.0,  1.0,   0.,   0., 
     5 0.77,0.77,0.77,-0.05,-0.005,-0.00001,-0.00001,-0.00001,1.0,0., 
     6  4.5,  0.7,  0., 0.003,  0.5,  0.5,   0.,   0.,   0.,   0., 
     7  10., 1000., 100., 1000., 0.,  0.7,  10.,   0.,   0.,   0., 
     8 0.29,  1.0,  1.0,   0.,  10.,  10.,   0.,   0.,   0.,   0., 
     9 0.02,  1.0,  0.2,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     &   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     1   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     2  1.0, 0.25,91.187,2.489, 0.01, 2.0,  1.0, 0.25,0.002,   0., 
     3   0.,   0.,   0.,   0., 0.01, 0.99,   0.,   0.,  0.2,   0., 
     4  60*0./ 
 
C...LUDAT2, with particle data and flavour treatment parameters. 
      DATA (KCHG(I,1),I=   1, 500)/-1,2,-1,2,-1,2,-1,2,2*0,-3,0,-3,0, 
     &-3,0,-3,6*0,3,9*0,3,2*0,3,0,-1,44*0,2,-1,2,-1,2,3,11*0,3,0,2*3,0, 
     &3,0,3,0,3,10*0,3,0,2*3,0,3,0,3,0,3,10*0,3,0,2*3,0,3,0,3,0,3,10*0, 
     &3,0,2*3,0,3,0,3,0,3,10*0,3,0,2*3,0,3,0,3,0,3,10*0,3,0,2*3,0,3,0, 
     &3,0,3,70*0,3,0,3,28*0,3,2*0,3,8*0,-3,8*0,3,0,-3,0,3,-3,3*0,3,6,0, 
     &3,5*0,-3,0,3,-3,0,-3,4*0,-3,0,3,6,-3,0,3,-3,0,-3,0,3,6,0,3,5*0, 
     &-3,0,3,-3,0,-3,114*0/ 
      DATA (KCHG(I,2),I=   1, 500)/8*1,12*0,2,16*0,2,1,50*0,-1,410*0/ 
      DATA (KCHG(I,3),I=   1, 500)/8*1,2*0,8*1,5*0,1,9*0,1,2*0,1,0,2*1, 
     &41*0,1,0,7*1,10*0,10*1,10*0,10*1,10*0,10*1,10*0,10*1,10*0,10*1, 
     &10*0,10*1,70*0,3*1,22*0,1,5*0,1,0,2*1,6*0,1,0,2*1,6*0,2*1,0,5*1, 
     &0,6*1,4*0,6*1,4*0,16*1,4*0,6*1,114*0/ 
      DATA (PMAS(I,1),I=   1, 500)/0.0099,0.0056,0.199,1.35,5.,160., 
     &2*250.,2*0.,0.00051,0.,0.1057,0.,1.777,0.,250.,5*0.,91.187,80.25, 
     &80.,6*0.,500.,900.,500.,3*300.,350.,200.,5000.,60*0.,0.1396, 
     &0.4977,0.4936,1.8693,1.8645,1.9688,5.2787,5.2786,5.47972,6.594, 
     &0.135,0.5475,0.9578,2.9788,9.4,320.,2*500.,2*0.,0.7669,0.8961, 
     &0.8916,2.0101,2.0071,2.11,2*5.325,5.5068,6.602,0.7683,0.782, 
     &1.0194,3.0969,9.4603,320.,2*500.,2*0.,1.232,2*1.29,2*2.424,2.536, 
     &2*5.73,5.97,7.3,1.232,1.17,1.4,3.46,9.875,320.,2*500.,2*0.,0.983, 
     &2*1.429,2*2.272,2.5,2*5.68,5.92,7.25,0.9827,1.,1.4,3.4151,9.8598, 
     &320.,2*500.,2*0.,1.26,2*1.402,2*2.372,2.56,2*5.78,6.02,7.3,1.26, 
     &1.282,1.42,3.5106,9.8919,320.,2*500.,2*0.,1.318,1.432,1.425, 
     &2*2.46,2.61,2*5.83,6.07,7.35,1.318,1.275,1.525,3.5562,9.9132, 
     &320.,2*500.,2*0.,2*0.4977,8*0.,3.686,3*0.,10.0233,70*0.,1.1156, 
     &5*0.,2.2849,0.,2.473,2.466,6*0.,5.641,0.,2*5.84,6*0.,0.9396, 
     &0.9383,0.,1.1974,1.1926,1.1894,1.3213,1.3149,0.,2.4525,2.4529, 
     &2.4527,2*2.55,2.73,4*0.,3*5.8,2*5.96,6.12,4*0.,1.234,1.233,1.232, 
     &1.231,1.3872,1.3837,1.3828,1.535,1.5318,1.6724,3*2.5,2*2.63,2.8, 
     &4*0.,3*5.81,2*5.97,6.13,114*0./ 
      DATA (PMAS(I,2),I=   1, 500)/22*0.,2.489,2.066,88*0.,0.0002, 
     &0.001,6*0.,0.149,0.0505,0.0498,7*0.,0.151,0.00843,0.0044,7*0., 
     &0.155,2*0.09,2*0.02,0.,4*0.05,0.155,0.36,0.08,2*0.01,5*0.,0.057, 
     &2*0.287,7*0.05,0.057,0.,0.25,0.014,6*0.,0.4,2*0.174,7*0.05,0.4, 
     &0.024,0.06,0.0009,6*0.,0.11,0.109,0.098,2*0.019,5*0.02,0.11, 
     &0.185,0.076,0.002,146*0.,4*0.12,0.0394,0.036,0.0358,0.0099, 
     &0.0091,131*0./ 
      DATA (PMAS(I,3),I=   1, 500)/22*0.,2*20.,88*0.,0.002,0.005,6*0., 
     &0.4,2*0.2,7*0.,0.4,0.1,0.015,7*0.,0.25,0.005,0.01,2*0.08,0., 
     &4*0.1,0.25,0.2,0.001,2*0.02,5*0.,0.05,2*0.4,6*0.1,2*0.05,0.,0.35, 
     &0.05,6*0.,3*0.3,2*0.1,0.03,4*0.1,0.3,0.05,0.02,0.001,6*0.,0.25, 
     &4*0.12,5*0.05,0.25,0.17,0.2,0.01,146*0.,4*0.14,0.04,2*0.035, 
     &2*0.05,131*0./ 
      DATA (PMAS(I,4),I=   1, 500)/12*0.,658650.,0.,0.0914,68*0.,0.1, 
     &0.387,15*0.,7804.,0.,3709.,0.32,0.1259,0.135,3*0.387,0.15,110*0., 
     &15500.,26.75,83*0.,78.88,5*0.,0.057,0.,0.025,0.09,6*0.,0.387,0., 
     &2*0.387,9*0.,44.3,0.,23.95,49.1,86.9,6*0.,0.13,9*0.,0.387,13*0., 
     &24.60001,130*0./ 
      DATA PARF/ 
     &  0.5, 0.25,  0.5, 0.25,   1.,  0.5,   0.,   0.,   0.,   0., 
     1  0.5,   0.,  0.5,   0.,   1.,   1.,   0.,   0.,   0.,   0., 
     2  0.5,   0.,  0.5,   0.,   1.,   1.,   0.,   0.,   0.,   0., 
     3  0.5,   0.,  0.5,   0.,   1.,   1.,   0.,   0.,   0.,   0., 
     4  0.5,   0.,  0.5,   0.,   1.,   1.,   0.,   0.,   0.,   0., 
     5  0.5,   0.,  0.5,   0.,   1.,   1.,   0.,   0.,   0.,   0., 
     6 0.75,  0.5,   0., 0.1667, 0.0833, 0.1667, 0., 0., 0.,   0., 
     7   0.,   0.,   1., 0.3333, 0.6667, 0.3333, 0., 0., 0.,   0., 
     8   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     9   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     & 0.325, 0.325, 0.5, 1.6,  5.0,   0.,   0.,   0.,   0.,   0., 
     1   0., 0.11, 0.16, 0.048, 0.50, 0.45, 0.55, 0.60,  0.,   0., 
     2  0.2,  0.1,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0., 
     3  1870*0./ 
      DATA ((VCKM(I,J),J=1,4),I=1,4)/ 
     1  0.95113,  0.04884,  0.00003,  0.00000, 
     2  0.04884,  0.94940,  0.00176,  0.00000, 
     3  0.00003,  0.00176,  0.99821,  0.00000, 
     4  0.00000,  0.00000,  0.00000,  1.00000/ 
 
C...LUDAT3, with particle decay parameters and data. 
      DATA (MDCY(I,1),I=   1, 500)/5*0,3*1,6*0,1,0,1,5*0,3*1,6*0,1,0,1, 
     &2*0,4*1,42*0,7*1,12*0,1,0,15*1,2*0,18*1,2*0,18*1,2*0,18*1,2*0, 
     &18*1,2*0,18*1,3*0,1,8*0,1,3*0,1,70*0,1,5*0,1,0,2*1,6*0,1,0,2*1, 
     &9*0,5*1,0,6*1,4*0,6*1,4*0,16*1,4*0,6*1,114*0/ 
      DATA (MDCY(I,2),I=   1, 500)/1,9,17,25,33,41,50,60,2*0,70,74,76, 
     &81,83,124,126,132,2*0,135,144,156,172,192,6*0,209,0,231,254,274, 
     &292,301,304,305,42*0,314,315,319,328,331,336,338,11*0,358,359, 
     &361,367,430,491,524,560,596,635,666,668,675,681,682,683,684,685, 
     &2*0,686,688,691,694,697,699,700,701,702,703,704,708,713,721,724, 
     &733,734,735,2*0,736,737,742,747,749,751,753,755,757,759,761,762, 
     &765,769,770,771,772,773,2*0,774,775,777,779,781,783,785,787,789, 
     &791,793,794,799,804,806,808,809,810,2*0,811,813,815,817,819,821, 
     &823,825,827,829,831,833,846,850,852,854,855,856,2*0,857,863,873, 
     &884,892,900,904,912,920,924,928,936,945,951,953,955,956,957,2*0, 
     &958,966,8*0,968,3*0,979,70*0,993,5*0,997,0,1073,1074,6*0,1075,0, 
     &1092,1093,9*0,1094,1096,1097,1100,1101,0,1103,1104,1105,1106, 
     &1107,1108,4*0,1109,1110,1111,1112,1113,1114,4*0,1115,1116,1119, 
     &1122,1123,1126,1129,1132,1134,1136,1140,1141,1142,1143,1145,1147, 
     &4*0,1148,1149,1150,1151,1152,1153,114*0/ 
      DATA (MDCY(I,3),I=   1, 500)/5*8,9,2*10,2*0,4,2,5,2,41,2,6,3,2*0, 
     &9,12,16,20,17,6*0,22,0,23,20,18,9,3,1,9,42*0,1,4,9,3,5,2,20,11*0, 
     &1,2,6,63,61,33,2*36,39,31,2,7,6,5*1,2*0,2,3*3,2,5*1,4,5,8,3,9, 
     &3*1,2*0,1,2*5,7*2,1,3,4,5*1,2*0,1,9*2,1,2*5,2*2,3*1,2*0,11*2,13, 
     &4,2*2,3*1,2*0,6,10,11,2*8,4,2*8,2*4,8,9,6,2*2,3*1,2*0,8,2,8*0,11, 
     &3*0,14,70*0,4,5*0,76,0,2*1,6*0,17,0,2*1,9*0,2,1,3,1,2,0,6*1,4*0, 
     &6*1,4*0,1,2*3,1,3*3,2*2,4,3*1,2*2,1,4*0,6*1,114*0/ 
      DATA (MDME(I,1),I=   1,2000)/6*1,-1,7*1,-1,7*1,-1,7*1,-1,7*1,-1, 
     &7*1,-1,1,-1,8*1,2*-1,8*1,2*-1,61*1,-1,2*1,-1,6*1,2*-1,7*1,2*-1, 
     &3*1,-1,6*1,2*-1,6*1,2*-1,3*1,-1,3*1,-1,3*1,5*-1,3*1,-1,6*1,2*-1, 
     &3*1,-1,11*1,2*-1,6*1,8*-1,3*1,-1,3*1,-1,3*1,5*-1,3*1,4*-1,6*1, 
     &2*-1,3*1,-1,5*1,-1,8*1,2*-1,3*1,-1,9*1,-1,3*1,-1,9*1,2*-1,2*1,-1, 
     &16*1,-1,2*1,3*-1,1665*1/ 
      DATA (MDME(I,2),I=   1,2000)/75*102,42,6*102,2*42,2*0,7*41,2*0, 
     &24*41,6*102,45,29*102,8*32,8*0,16*32,4*0,8*32,4*0,32,4*0,8*32, 
     &14*0,16*32,7*0,8*32,4*0,32,7*0,8*32,4*0,32,5*0,4*32,5*0,3*32,0, 
     &6*32,3*0,12,2*42,2*11,9*42,2*45,31,2*45,2*33,31,2*45,20*46,7*0, 
     &24*42,41*0,16*42,46*0,10*42,20*0,2*13,14*42,16*0,48,3*13,16*42, 
     &16*0,48,3*13,16*42,19*0,48,3*13,2*42,0,2*11,28*42,0,2,4*0,2,8*0, 
     &12,32,86,87,88,3,0,2*3,0,2*3,0,2*3,0,3,6*0,3,3*0,1,0,3,2*0,2*3, 
     &3*0,1,4*0,12,3*0,4*32,2*4,86,87,88,33*0,12,32,86,87,88,31*0,12,0, 
     &32,86,87,88,40*0,12,0,32,86,87,88,95*0,12,0,32,86,87,88,2*0,4*42, 
     &6*0,12,11*0,4*32,2*4,9*0,14*42,52*0,10*13,2*84,3*42,8*0,48,3*13, 
     &2*42,2*85,14*0,84,5*0,85,886*0/ 
      DATA (BRAT(I)  ,I=   1, 439)/75*0.,1.,6*0.,0.179,0.178,0.116, 
     &0.235,0.005,0.056,0.018,0.023,0.011,2*0.004,0.0067,0.014,2*0.002, 
     &2*0.001,0.0022,0.054,0.002,0.016,0.005,0.011,0.0101,5*0.006, 
     &0.002,2*0.001,5*0.002,6*0.,1.,29*0.,0.15394,0.11936,0.15394, 
     &0.11926,0.15254,3*0.,0.03368,0.06664,0.03368,0.06664,0.03368, 
     &0.06664,2*0.,0.3214,0.0165,2*0.,0.0165,0.3207,2*0.,0.00001, 
     &0.00059,6*0.,3*0.1081,3*0.,0.0003,0.048,0.8705,4*0.,0.0002, 
     &0.0603,0.,0.0199,0.0008,3*0.,0.143,0.111,0.143,0.111,0.143,0.085, 
     &2*0.,0.03,0.058,0.03,0.058,0.03,0.058,8*0.,0.25,0.01,2*0.,0.01, 
     &0.25,4*0.,0.24,5*0.,3*0.08,6*0.,0.01,0.08,0.82,5*0.,0.09,11*0., 
     &0.01,0.08,0.82,5*0.,0.09,9*0.,1.,6*0.,0.01,0.98,0.01,1.,4*0.215, 
     &2*0.,2*0.07,0.,1.,2*0.08,0.76,0.08,2*0.105,0.04,0.5,0.08,0.14, 
     &0.01,0.015,0.005,1.,3*0.,1.,4*0.,1.,0.25,0.01,2*0.,0.01,0.25, 
     &4*0.,0.24,5*0.,3*0.08,0.,1.,2*0.5,0.635,0.212,0.056,0.017,0.048, 
     &0.032,0.07,0.065,2*0.005,2*0.011,5*0.001,0.07,0.065,2*0.005, 
     &2*0.011,5*0.001,0.026,0.019,0.066,0.041,0.045,0.076,0.0073, 
     &2*0.0047,0.026,0.001,0.0006,0.0066,0.005,2*0.003,2*0.0006, 
     &2*0.001,0.006,0.005,0.012,0.0057,0.067,0.008,0.0022,0.027,0.004, 
     &0.019,0.012,0.002,0.009,0.0218,0.001,0.022,0.087,0.001,0.0019, 
     &0.0015,0.0028,0.034,0.027,2*0.002,2*0.004,2*0.002,0.034,0.027/ 
      DATA (BRAT(I)  ,I= 440, 655)/2*0.002,2*0.004,2*0.002,0.0365, 
     &0.045,0.073,0.062,3*0.021,0.0061,0.015,0.025,0.0088,0.074,0.0109, 
     &0.0041,0.002,0.0035,0.0011,0.001,0.0027,2*0.0016,0.0018,0.011, 
     &0.0063,0.0052,0.018,0.016,0.0034,0.0036,0.0009,0.0006,0.015, 
     &0.0923,0.018,0.022,0.0077,0.009,0.0075,0.024,0.0085,0.067,0.0511, 
     &0.017,0.0004,0.0028,0.01,2*0.02,0.03,2*0.005,2*0.02,0.03,2*0.005, 
     &0.015,0.037,0.028,0.079,0.095,0.052,0.0078,4*0.001,0.028,0.033, 
     &0.026,0.05,0.01,4*0.005,0.25,0.0952,0.02,0.055,2*0.005,0.008, 
     &0.012,0.02,0.055,2*0.005,0.008,0.012,0.01,0.03,0.0035,0.011, 
     &0.0055,0.0042,0.009,0.018,0.015,0.0185,0.0135,0.025,0.0004, 
     &0.0007,0.0008,0.0014,0.0019,0.0025,0.4291,0.08,0.07,0.02,0.015, 
     &0.005,0.02,0.055,2*0.005,0.008,0.012,0.02,0.055,2*0.005,0.008, 
     &0.012,0.01,0.03,0.0035,0.011,0.0055,0.0042,0.009,0.018,0.015, 
     &0.0185,0.0135,0.025,0.0004,0.0007,0.0008,0.0014,0.0019,0.0025, 
     &0.4291,0.08,0.07,0.02,0.015,0.005,0.02,0.055,2*0.005,0.008,0.012, 
     &0.02,0.055,2*0.005,0.008,0.012,0.01,0.03,0.0035,0.011,0.0055, 
     &0.0042,0.009,0.018,0.015,0.0185,0.0135,0.025,2*0.0002,0.0007, 
     &2*0.0004,0.0014,0.001,0.0009,0.0025,0.4291,0.08,0.07,0.02,0.015, 
     &0.005,0.047,0.122,0.006,0.012,0.035,0.012,0.035,0.003,0.007,0.15, 
     &0.037,0.008,0.002,0.05,0.015,0.003,0.001,0.014,0.042,0.014,0.042/ 
      DATA (BRAT(I)  ,I= 656, 931)/0.24,0.065,0.012,0.003,0.001,0.002, 
     &0.001,0.002,0.014,0.003,0.988,0.012,0.389,0.319,0.2367,0.049, 
     &0.005,0.001,0.0003,0.441,0.206,0.3,0.03,0.022,0.001,5*1.,0.99955, 
     &0.00045,0.665,0.333,0.002,0.666,0.333,0.001,0.65,0.3,0.05,0.56, 
     &0.44,5*1.,0.99912,0.00079,0.00005,0.00004,0.888,0.085,0.021, 
     &2*0.003,0.49,0.344,3*0.043,0.023,0.013,0.001,0.0627,0.0597, 
     &0.8776,3*0.027,0.015,0.045,0.015,0.045,0.77,0.029,4*1.,0.28,0.14, 
     &0.313,0.157,0.11,0.28,0.14,0.313,0.157,0.11,0.667,0.333,0.667, 
     &0.333,2*0.5,0.667,0.333,0.667,0.333,4*0.5,1.,0.333,0.334,0.333, 
     &4*0.25,6*1.,0.667,0.333,0.667,0.333,0.667,0.333,0.667,0.333, 
     &2*0.5,0.667,0.333,0.667,0.333,4*0.5,1.,0.52,0.26,0.11,2*0.055, 
     &0.62,0.31,0.035,2*0.0175,0.007,0.993,0.02,0.98,3*1.,2*0.5,0.667, 
     &0.333,0.667,0.333,0.667,0.333,0.667,0.333,2*0.5,0.667,0.333, 
     &0.667,0.333,6*0.5,3*0.12,0.097,0.043,4*0.095,4*0.03,4*0.25,0.273, 
     &0.727,0.35,0.65,3*1.,2*0.35,0.144,0.105,0.048,0.003,0.333,0.166, 
     &0.168,0.084,0.087,0.043,0.059,2*0.029,0.002,0.332,0.166,0.168, 
     &0.084,0.086,0.043,0.059,2*0.029,2*0.002,0.3,0.15,0.16,0.08,0.13, 
     &0.06,0.08,0.04,0.3,0.15,0.16,0.08,0.13,0.06,0.08,0.04,2*0.3, 
     &2*0.2,0.3,0.15,0.16,0.08,0.13,0.06,0.08,0.04,0.3,0.15,0.16,0.08, 
     &0.13,0.06,0.08,0.04,2*0.3,2*0.2,2*0.3,2*0.2,2*0.35,0.144,0.105/ 
      DATA (BRAT(I)  ,I= 932,2000)/0.024,2*0.012,0.003,0.566,0.283, 
     &0.069,0.028,0.023,2*0.0115,0.005,0.003,0.356,2*0.178,0.28, 
     &2*0.004,0.135,0.865,0.22,0.78,3*1.,0.217,0.124,2*0.193,2*0.135, 
     &0.002,0.001,0.686,0.314,2*0.0083,0.1866,0.324,0.184,0.027,0.001, 
     &0.093,0.087,0.078,0.0028,3*0.014,0.008,0.024,0.008,0.024,0.425, 
     &0.02,0.185,0.088,0.043,0.067,0.066,0.641,0.357,2*0.001,0.018, 
     &2*0.005,0.003,0.002,2*0.006,0.018,2*0.005,0.003,0.002,2*0.006, 
     &0.0066,0.025,0.016,0.0088,2*0.005,0.0058,0.005,0.0055,4*0.004, 
     &2*0.002,2*0.004,0.003,0.002,2*0.003,3*0.002,2*0.001,0.002, 
     &2*0.001,2*0.002,0.0013,0.0018,5*0.001,4*0.003,2*0.005,2*0.002, 
     &2*0.001,2*0.002,2*0.001,0.2432,0.057,2*0.035,0.15,2*0.075,0.03, 
     &2*0.015,2*1.,2*0.105,0.04,0.0077,0.02,0.0235,0.0285,0.0435, 
     &0.0011,0.0022,0.0044,0.4291,0.08,0.07,0.02,0.015,0.005,2*1., 
     &0.999,0.001,1.,0.516,0.483,0.001,1.,0.995,0.005,13*1.,0.331, 
     &0.663,0.006,0.663,0.331,0.006,1.,0.88,2*0.06,0.88,2*0.06,0.88, 
     &2*0.06,0.667,2*0.333,0.667,0.676,0.234,0.085,0.005,3*1.,4*0.5, 
     &7*1.,847*0./ 
      DATA (KFDP(I,1),I=   1, 507)/21,22,23,4*-24,25,21,22,23,4*24,25, 
     &21,22,23,4*-24,25,21,22,23,4*24,25,21,22,23,4*-24,25,21,22,23, 
     &4*24,25,37,21,22,23,4*-24,25,2*-37,21,22,23,4*24,25,2*37,22,23, 
     &-24,25,23,24,-12,22,23,-24,25,23,24,-12,-14,35*16,22,23,-24,25, 
     &23,24,-89,22,23,-24,25,-37,23,24,37,1,2,3,4,5,6,7,8,21,1,2,3,4,5, 
     &6,7,8,11,13,15,17,1,2,3,4,5,6,7,8,11,12,13,14,15,16,17,18,4*-1, 
     &4*-3,4*-5,4*-7,-11,-13,-15,-17,1,2,3,4,5,6,7,8,11,13,15,17,21, 
     &2*22,23,24,1,2,3,4,5,6,7,8,11,12,13,14,15,16,17,18,24,37,2*23,25, 
     &35,4*-1,4*-3,4*-5,4*-7,-11,-13,-15,-17,3*24,1,2,3,4,5,6,7,8,11, 
     &13,15,17,21,2*22,23,24,23,25,36,1,2,3,4,5,6,7,8,11,13,15,17,21, 
     &2*22,23,24,23,-1,-3,-5,-7,-11,-13,-15,-17,24,5,6,21,2,1,2,3,4,5, 
     &6,11,13,15,82,-11,-13,2*2,-12,-14,-16,2*-2,2*-4,-2,-4,2*89,37, 
     &2*-89,2*5,-37,2*89,4*-1,4*-3,4*-5,4*-7,-11,-13,-15,-17,-13,130, 
     &310,-13,3*211,12,14,11*-11,11*-13,-311,-313,-311,-313,-20313, 
     &2*-311,-313,-311,-313,2*111,2*221,2*331,2*113,2*223,2*333,-311, 
     &-313,2*-321,211,-311,-321,333,-311,-313,-321,211,2*-321,2*-311, 
     &-321,211,113,8*-11,8*-13,-321,-323,-321,-323,-311,2*-313,-311, 
     &-313,2*-311,-321,-10323,-321,-323,-321,-311,2*-313,211,111,333, 
     &3*-321,-311,-313,-321,-313,310,333,211,2*-321,-311,-313,-311,211, 
     &-321,3*-311,211,113,321,-15,5*-11,5*-13,221,331,333,221,331,333/ 
      DATA (KFDP(I,1),I= 508, 924)/10221,211,213,211,213,321,323,321, 
     &323,2212,221,331,333,221,2*2,6*12,6*14,2*16,3*-411,3*-413,2*-411, 
     &2*-413,2*441,2*443,2*20443,2*2,2*4,2,4,6*12,6*14,2*16,3*-421, 
     &3*-423,2*-421,2*-423,2*441,2*443,2*20443,2*2,2*4,2,4,6*12,6*14, 
     &2*16,3*-431,3*-433,2*-431,2*-433,3*441,3*443,3*20443,2*2,2*4,2,4, 
     &16,2*4,2*12,2*14,2*16,4*2,4*4,2*-11,2*-13,2*-1,2*-3,2*-11,2*-13, 
     &2*-1,3*22,111,211,2*22,211,22,211,111,3*22,111,82,21,3*0,2*211, 
     &321,3*311,2*321,421,2*411,2*421,431,511,521,531,541,211,111,13, 
     &11,211,22,211,2*111,321,130,-213,113,213,211,22,111,11,13,82,11, 
     &13,15,1,2,3,4,21,22,3*0,223,321,311,323,313,2*311,321,313,323, 
     &321,423,2*413,2*423,413,523,2*513,2*523,2*513,523,223,213,113, 
     &-213,313,-313,323,-323,82,21,3*0,221,321,2*311,321,421,2*411,421, 
     &411,421,521,2*511,2*521,2*511,521,221,211,111,321,130,310,211, 
     &111,321,130,310,443,82,553,21,3*0,113,213,323,2*313,323,423, 
     &2*413,2*423,413,523,2*513,2*523,2*513,523,213,-213,10211,10111, 
     &-10211,2*221,213,2*113,-213,2*321,2*311,313,-313,323,-323,443,82, 
     &553,21,3*0,213,113,221,223,321,211,321,311,323,313,323,313,321, 
     &4*311,321,313,323,313,323,311,4*321,421,411,423,413,423,413,421, 
     &2*411,421,413,423,413,423,411,2*421,411,423,413,521,511,523,513, 
     &523,513,521,2*511,521,513,523,513,523,511,2*521,511,523,513,511/ 
      DATA (KFDP(I,1),I= 925,2000)/521,513,523,213,-213,221,223,321, 
     &130,310,111,211,111,2*211,321,130,310,221,111,321,130,310,221, 
     &211,111,443,82,553,21,3*0,111,211,-12,12,-14,14,211,111,211,111, 
     &11,13,82,4*443,10441,20443,445,441,11,13,15,1,2,3,4,21,22,2*553, 
     &10551,20553,555,2212,2*2112,-12,7*-11,7*-13,2*2224,2*2212,2*2214, 
     &2*3122,2*3212,2*3214,5*3222,4*3224,2*3322,3324,2*2224,7*2212, 
     &5*2214,2*2112,2*2114,2*3122,2*3212,2*3214,2*3222,2*3224,4*2,3, 
     &2*2,1,2*2,2*0,-12,-14,-16,5*4122,441,443,20443,2*-2,2*-4,-2,-4, 
     &2*0,2112,-12,3122,2212,2112,2212,3*3122,3*4122,4132,4232,0, 
     &3*5122,5132,5232,0,2112,2212,2*2112,2212,2112,2*2212,3122,3212, 
     &3112,3122,3222,3112,3122,3222,3212,3322,3312,3322,3312,3122,3322, 
     &3312,-12,3*4122,2*4132,2*4232,4332,3*5122,5132,5232,5332,847*0/ 
      DATA (KFDP(I,2),I=   1, 476)/3*1,2,4,6,8,1,3*2,1,3,5,7,2,3*3,2,4, 
     &6,8,3,3*4,1,3,5,7,4,3*5,2,4,6,8,5,3*6,1,3,5,7,6,5,3*7,2,4,6,8,7, 
     &4,6,3*8,1,3,5,7,8,5,7,2*11,12,11,12,2*11,2*13,14,13,14,13,11,13, 
     &-211,-213,-211,-213,-211,-213,3*-211,-321,-323,-321,-323,3*-321, 
     &4*-211,-213,-211,-213,-211,-213,-211,-213,-211,-213,6*-211,2*15, 
     &16,15,16,15,18,2*17,18,17,2*18,2*17,-1,-2,-3,-4,-5,-6,-7,-8,21, 
     &-1,-2,-3,-4,-5,-6,-7,-8,-11,-13,-15,-17,-1,-2,-3,-4,-5,-6,-7,-8, 
     &-11,-12,-13,-14,-15,-16,-17,-18,2,4,6,8,2,4,6,8,2,4,6,8,2,4,6,8, 
     &12,14,16,18,-1,-2,-3,-4,-5,-6,-7,-8,-11,-13,-15,-17,21,22,2*23, 
     &-24,-1,-2,-3,-4,-5,-6,-7,-8,-11,-12,-13,-14,-15,-16,-17,-18,-24, 
     &-37,22,25,2*36,2,4,6,8,2,4,6,8,2,4,6,8,2,4,6,8,12,14,16,18,23,22, 
     &25,-1,-2,-3,-4,-5,-6,-7,-8,-11,-13,-15,-17,21,22,2*23,-24,2*25, 
     &36,-1,-2,-3,-4,-5,-6,-7,-8,-11,-13,-15,-17,21,22,2*23,-24,25,2,4, 
     &6,8,12,14,16,18,25,-5,-6,21,11,-3,-4,-5,-6,-7,-8,-13,-15,-17,-82, 
     &12,14,-1,-3,11,13,15,1,4,3,4,1,3,5,3,5,6,4,21,22,4,7,5,2,4,6,8,2, 
     &4,6,8,2,4,6,8,2,4,6,8,12,14,16,18,14,2*0,14,111,211,111,-11,-13, 
     &11*12,11*14,2*211,2*213,211,20213,2*321,2*323,211,213,211,213, 
     &211,213,211,213,211,213,211,213,3*211,213,211,2*321,8*211,2*113, 
     &2*211,8*12,8*14,2*211,2*213,2*111,221,2*113,223,333,20213,211, 
     &2*321,323,2*311,313,-211,111,113,2*211,321,2*211,311,321,310,211/ 
      DATA (KFDP(I,2),I= 477, 857)/-211,4*211,321,4*211,113,2*211,-321, 
     &16,5*12,5*14,3*211,3*213,211,2*111,2*113,2*-311,2*-313,-2112, 
     &3*321,323,2*-1,6*-11,6*-13,2*-15,211,213,20213,211,213,20213,431, 
     &433,431,433,311,313,311,313,311,313,-1,-4,-3,-4,-1,-3,6*-11, 
     &6*-13,2*-15,211,213,20213,211,213,20213,431,433,431,433,321,323, 
     &321,323,321,323,-1,-4,-3,-4,-1,-3,6*-11,6*-13,2*-15,211,213, 
     &20213,211,213,20213,431,433,431,433,221,331,333,221,331,333,221, 
     &331,333,-1,-4,-3,-4,-1,-3,-15,-3,-1,2*-11,2*-13,2*-15,-1,-4,-3, 
     &-4,-3,-4,-1,-4,2*12,2*14,2,3,2,3,2*12,2*14,2,1,22,11,22,111,-211, 
     &211,11,-211,13,-211,111,113,223,22,111,-82,21,3*0,111,22,-211, 
     &111,22,211,111,22,211,111,22,111,6*22,-211,22,-13,-11,-211,111, 
     &-211,2*111,-321,310,211,111,2*-211,221,22,-11,-13,-82,-11,-13, 
     &-15,-1,-2,-3,-4,2*21,3*0,211,-213,113,-211,111,223,213,113,211, 
     &111,223,211,111,-211,111,321,311,-211,111,211,111,-321,-311,411, 
     &421,111,-211,111,211,-311,311,-321,321,-82,21,3*0,211,-211,111, 
     &211,111,211,111,-211,111,311,321,-211,111,211,111,-321,-311,411, 
     &421,111,-211,111,-321,130,310,-211,111,-321,130,310,22,-82,22,21, 
     &3*0,211,111,-211,111,211,111,211,111,-211,111,321,311,-211,111, 
     &211,111,-321,-311,411,421,-211,211,-211,111,2*211,111,-211,211, 
     &111,211,-321,2*-311,-321,-311,311,-321,321,22,-82,22,21,3*0,111/ 
      DATA (KFDP(I,2),I= 858,2000)/3*211,-311,22,-211,111,-211,111, 
     &-211,211,-213,113,223,221,211,111,211,111,2*211,213,113,223,221, 
     &22,211,111,211,111,4*211,-211,111,-211,111,-211,211,-211,211,321, 
     &311,321,311,-211,111,-211,111,-211,211,-211,2*211,111,211,111, 
     &4*211,-321,-311,-321,-311,411,421,411,421,-211,211,111,211,-321, 
     &130,310,22,-211,111,2*-211,-321,130,310,221,111,-321,130,310,221, 
     &-211,111,22,-82,22,21,3*0,111,-211,11,-11,13,-13,-211,111,-211, 
     &111,-11,-13,-82,211,111,221,111,4*22,-11,-13,-15,-1,-2,-3,-4, 
     &2*21,211,111,3*22,-211,111,22,11,7*12,7*14,-321,-323,-311,-313, 
     &-311,-313,211,213,211,213,211,213,111,221,331,113,223,111,221, 
     &113,223,321,323,321,-211,-213,111,221,331,113,223,333,10221,111, 
     &221,331,113,223,211,213,211,213,321,323,321,323,321,323,311,313, 
     &311,313,2*-1,-3,-1,2203,3201,3203,2203,2101,2103,2*0,11,13,15, 
     &-211,-213,-20213,-431,-433,3*3122,1,4,3,4,1,3,2*0,-211,11,22,111, 
     &211,22,-211,111,22,-211,111,211,2*22,0,-211,111,211,2*22,0, 
     &2*-211,111,22,111,211,22,211,2*-211,2*111,-211,2*211,111,211, 
     &-211,2*111,211,-321,-211,111,11,-211,111,211,111,22,111,2*22, 
     &-211,111,211,3*22,847*0/ 
      DATA (KFDP(I,3),I=   1, 944)/75*0,14,6*0,2*16,2*0,5*111,310,130, 
     &2*0,2*111,310,130,321,113,211,223,221,2*113,2*211,2*223,2*221, 
     &2*113,221,113,2*213,-213,195*0,4*3,4*4,1,4,3,2*2,10*81,25*0,-211, 
     &3*111,-311,-313,-311,-321,-313,-323,111,221,331,113,223,-311, 
     &-313,-311,-321,-313,-323,111,221,331,113,223,22*0,111,113,2*211, 
     &-211,-311,211,111,3*211,-211,7*211,-321,-323,-311,-321,-313,-323, 
     &-211,-213,-321,-323,-311,-321,-313,-323,-211,-213,22*0,111,113, 
     &-311,2*-211,211,-211,310,-211,2*111,211,2*-211,-321,-211,2*211, 
     &-211,111,-211,2*211,0,221,331,333,321,311,221,331,333,321,311, 
     &20*0,3,0,-411,-413,-10413,-10411,-20413,-415,-411,-413,-10413, 
     &-10411,-20413,-415,-411,-413,16*0,-4,-1,-4,-3,2*-2,-421,-423, 
     &-10423,-10421,-20423,-425,-421,-423,-10423,-10421,-20423,-425, 
     &-421,-423,16*0,-4,-1,-4,-3,2*-2,-431,-433,-10433,-10431,-20433, 
     &-435,-431,-433,-10433,-10431,-20433,-435,-431,-433,19*0,-4,-1,-4, 
     &-3,2*-2,3*0,441,443,441,443,441,443,-4,-1,-4,-3,-4,-3,-4,-1,531, 
     &533,531,533,3,2,3,2,511,513,511,513,1,2,0,-11,0,2*111,-211,-11, 
     &11,-13,2*221,3*0,111,27*0,111,2*0,22,111,5*0,111,12*0,2*21,103*0, 
     &-211,2*111,-211,3*111,-211,111,211,14*0,111,6*0,111,-211,8*0,111, 
     &-211,9*0,111,-211,111,-211,4*0,111,-211,111,-211,8*0,111,-211, 
     &111,-211,4*0,111,-211,111,-211,11*0,-211,6*0,111,211,4*0,111/ 
      DATA (KFDP(I,3),I= 945,2000)/13*0,2*111,211,-211,211,-211,7*0, 
     &-211,111,13*0,2*21,-211,111,6*0,2212,3122,3212,3214,2112,2114, 
     &2212,2112,3122,3212,3214,2112,2114,2212,2112,52*0,3*3,1,8*0, 
     &3*4122,8*0,4,1,4,3,2*2,3*0,2112,43*0,3322,861*0/ 
      DATA (KFDP(I,4),I=   1,2000)/88*0,3*111,8*0,-211,0,-211,3*0,111, 
     &2*-211,0,111,0,2*111,113,221,111,-213,-211,211,195*0,13*81,41*0, 
     &111,211,111,211,7*0,111,211,111,211,35*0,2*-211,2*111,211,111, 
     &-211,2*211,2*-211,2*0,-211,111,-211,111,4*0,-211,111,-211,111, 
     &34*0,111,-211,3*111,3*-211,2*111,3*-211,4*0,-321,-311,3*0,-321, 
     &-311,20*0,-3,31*0,6*1,30*0,6*2,33*0,6*3,9*0,8*4,4*0,4*-5,4*0, 
     &2*-5,7*0,-11,264*0,111,-211,4*0,111,57*0,-211,111,5*0,-211,111, 
     &52*0,2101,2103,2*2101,19*0,6*2101,909*0/ 
      DATA (KFDP(I,5),I=   1,2000)/90*0,111,16*0,111,7*0,111,0,2*111, 
     &303*0,-211,2*111,-211,111,-211,111,54*0,111,-211,3*111,-211,111, 
     &1510*0/ 
 
C...LUDAT4, with character strings. 
      DATA (CHAF(I)  ,I=   1, 281)/'d','u','s','c','b','t','l','h', 
     &2*' ','e','nu_e','mu','nu_mu','tau','nu_tau','chi','nu_chi', 
     &2*' ','g','gamma','Z','W','H',2*' ','reggeon','pomeron',2*' ', 
     &'Z''','Z"','W''','H''','A','H','eta_tech','LQ_ue','R',40*' ', 
     &'specflav','rndmflav','phasespa','c-hadron','b-hadron', 
     &'t-hadron','l-hadron','h-hadron','Wvirt','diquark','cluster', 
     &'string','indep.','CMshower','SPHEaxis','THRUaxis','CLUSjet', 
     &'CELLjet','table',' ','pi',2*'K',2*'D','D_s',2*'B','B_s','B_c', 
     &'pi','eta','eta''','eta_c','eta_b','eta_t','eta_l','eta_h',2*' ', 
     &'rho',2*'K*',2*'D*','D*_s',2*'B*','B*_s','B*_c','rho','omega', 
     &'phi','J/psi','Upsilon','Theta','Theta_l','Theta_h',2*' ','b_1', 
     &2*'K_1',2*'D_1','D_1s',2*'B_1','B_1s','B_1c','b_1','h_1','h''_1', 
     &'h_1c','h_1b','h_1t','h_1l','h_1h',2*' ','a_0',2*'K*_0',2*'D*_0', 
     &'D*_0s',2*'B*_0','B*_0s','B*_0c','a_0','f_0','f''_0','chi_0c', 
     &'chi_0b','chi_0t','chi_0l','chi_0h',2*' ','a_1',2*'K*_1', 
     &2*'D*_1','D*_1s',2*'B*_1','B*_1s','B*_1c','a_1','f_1','f''_1', 
     &'chi_1c','chi_1b','chi_1t','chi_1l','chi_1h',2*' ','a_2', 
     &2*'K*_2',2*'D*_2','D*_2s',2*'B*_2','B*_2s','B*_2c','a_2','f_2', 
     &'f''_2','chi_2c','chi_2b','chi_2t','chi_2l','chi_2h',2*' ','K_L', 
     &'K_S',8*' ','psi''',3*' ','Upsilon''',45*' ','pi_diffr'/ 
      DATA (CHAF(I)  ,I= 282, 500)/'n_diffr','p_diffr','rho_diff', 
     &'omega_di','phi_diff','J/psi_di',18*' ','Lambda',5*' ', 
     &'Lambda_c',' ',2*'Xi_c',6*' ','Lambda_b',' ',2*'Xi_b',6*' ','n', 
     &'p',' ',3*'Sigma',2*'Xi',' ',3*'Sigma_c',2*'Xi''_c','Omega_c', 
     &4*' ',3*'Sigma_b',2*'Xi''_b','Omega_b',4*' ',4*'Delta', 
     &3*'Sigma*',2*'Xi*','Omega',3*'Sigma*_c',2*'Xi*_c','Omega*_c', 
     &4*' ',3*'Sigma*_b',2*'Xi*_b','Omega*_b',114*' '/ 
 
C...LUDATR, with initial values for the random number generator. 
      DATA MRLU0/19780503,0,0,97,33,0/ 
 
      END 
 
C********************************************************************* 
 
      SUBROUTINE LUTAUD0(ITAU,IORIG,KFORIG,NDECAY) 
 
C...Dummy routine, to be replaced by user, to handle the decay of a 
C...polarized tau lepton. 
C...Input: 
C...ITAU is the position where the decaying tau is stored in /LUJETS/. 
C...IORIG is the position where the mother of the tau is stored; 
C...     is 0 when the mother is not stored. 
C...KFORIG is the flavour of the mother of the tau; 
C...     is 0 when the mother is not known. 
C...Note that IORIG=0 does not necessarily imply KFORIG=0; 
C...     e.g. in B hadron semileptonic decays the W  propagator 
C...     is not explicitly stored but the W code is still unambiguous. 
C...Output: 
C...NDECAY is the number of decay products in the current tau decay. 
C...These decay products should be added to the /LUJETS/ common block, 
C...in positions N+1 through N+NDECAY. For each product I you must 
C...give the flavour codes K(I,2) and the five-momenta P(I,1), P(I,2), 
C...P(I,3), P(I,4) and P(I,5). The rest will be stored automatically. 
 
      COMMON/LUJETS/N,K(4000,5),P(4000,5),V(4000,5) 
      COMMON/LUDAT01/MSTU(200),PARU(200),MSTJ(200),PARJ(200) 
      SAVE /LUJETS/,/LUDAT01/ 
 
C...Stop program if this routine is ever called. 
C...You should not copy these lines to your own routine. 
      NDECAY=ITAU+IORIG+KFORIG      
      WRITE(MSTU(11),5000) 
      IF(RLU0(0).LT.10.) STOP 
 
C...Format for error printout. 
 5000 FORMAT(1X,'Error: you did not link your LUTAUD0 routine ', 
     &'correctly.'/1X,'Dummy routine in JETSET file called instead.'/ 
     &1X,'Execution stopped!') 
 
 
      RETURN 
      END 

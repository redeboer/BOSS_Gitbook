

      SUBROUTINE SET_PYPARAM
C...Commonblocks.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      DOUBLE PRECISION P,V,PARU,PARJ,PMAS,PARF,VCKM
      INTEGER N, NPAD, K, MSTU, MSTJ, KCHG
      SAVE /PYJETS/,/PYDAT1/,/PYDAT2/
C..

C      PARJ(21)=0.5     ! xgm(D=0.36), width in the Px,Py Gaussian dist.
C      PARJ(33)=0.6     ! mass-2q(D=0.8),below which 2 -hadrons prodced,but no m$ 
       PARJ(126)=1.     ! (D=2.)GeV Mass_min(gg) (merge g-g to g)

C...baryon production parameters
      PARJ(1)=0.09       !(D=0.1)p(qq)/p(q)
      PARJ(2)=0.4    
      PARJ(11)=0.60     !*(D=0.5) V/p ratio of u&d
      PARJ(12)=0.66    !(D=0.6)  V/p ratio of strange(s)
      
      PARJ(14)=0.60    !*(D=0.)  spin=0,L=1,J=1   axial Vector meson
      PARJ(15)=0.12    !(D=0.)   spin=1,L=1,J=0   scalar meson
      PARJ(16)=0.12    !(D=0.)   spin=1,L=1,J=1   anothe axial Vector meson
      PARJ(17)=0.10    !(D=0.)   spin=1,L=1,J=2   tensor meson

      PARJ(25)=0.5     !(D=1.) extra supression for eta production
      MSTJ(12) = 5
      MSTJ(23) = 0
      PARJ(1) = 0.18
      PARJ(18) = 0.19
C
C  following parameters are copied from PHD thesis of Huang GS
C      
CHEKL      PARJ(21) = 0.45    ! 0.36->0.45
CHEKL      PARJ(33) = 0.50    ! 0.80->0.50
      PARJ(21) = 0.50    ! 0.36->0.45
      PARJ(33) = 0.28    ! 0.80->0.50
      PARJ(41) = 1.5     ! 0.30->1.50
      PARJ(42) = 0.78    ! 0.58->0.78

      END

      SUBROUTINE SET_PYPARAM_OLD
C...Commonblocks.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      DOUBLE PRECISION P,V,PARU,PARJ,PMAS,PARF,VCKM
      INTEGER N, NPAD, K, MSTU, MSTJ, KCHG
      SAVE /PYJETS/,/PYDAT1/,/PYDAT2/
C..

C      PARJ(21)=0.5     ! xgm(D=0.36), width in the Px,Py Gaussian dist.
C      PARJ(33)=0.6     ! mass-2q(D=0.8),below which 2 -hadrons prodced,but no m$ 
       PARJ(126)=1.     ! (D=2.)GeV Mass_min(gg) (merge g-g to g)

C...baryon production parameters
      PARJ(1)=0.09       !(D=0.1)p(qq)/p(q)
      PARJ(2)=0.4    
      PARJ(11)=0.60     !*(D=0.5) V/p ratio of u&d
      PARJ(12)=0.66    !(D=0.6)  V/p ratio of strange(s)
      
      PARJ(14)=0.60    !*(D=0.)  spin=0,L=1,J=1   axial Vector meson
      PARJ(15)=0.12    !(D=0.)   spin=1,L=1,J=0   scalar meson
      PARJ(16)=0.12    !(D=0.)   spin=1,L=1,J=1   anothe axial Vector meson
      PARJ(17)=0.10    !(D=0.)   spin=1,L=1,J=2   tensor meson

      PARJ(25)=0.5     !(D=1.) extra supression for eta production
      MSTJ(23) = 0

C
C  following parameters are copied from PHD thesis of Huang GS
C      
CHEKL      PARJ(21) = 0.45    ! 0.36->0.45
CHEKL      PARJ(33) = 0.50    ! 0.80->0.50
      PARJ(21) = 0.50    ! 0.36->0.45
      PARJ(33) = 0.28    ! 0.80->0.50
      PARJ(41) = 1.5     ! 0.30->1.50
      PARJ(42) = 0.78    ! 0.58->0.78

      END

      SUBROUTINE MY_PYUPD
      
C      CALL CHARMONIA_PYUPD
C      CALL CHARM_MESON_PYUPD
C      CALL OTHER_MESON_PYUPD
      CALL SET_DECAY_SWITCH
      CALL SET_PYPARAM
      
      END


      SUBROUTINE SET_DECAY_SWITCH
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER PYK,PYCHGE,PYCOMP
C...Commonblocks.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYDAT4/CHAF(500,2)
      CHARACTER CHAF*16
      COMMON/PYDATR/MRPY(6),RRPY(100)
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT1/MINT(400),VINT(400)
      COMMON/PYINT2/ISET(500),KFPR(500,2),COEF(500,20),ICOL(40,4,2)
      COMMON/PYINT3/XSFX(2,-40:40),ISIG(1000,3),SIGH(1000)
      COMMON/PYINT4/MWID(500),WIDS(500,5)
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
      COMMON/PYINT6/PROC(0:500)
      CHARACTER PROC*28
      COMMON/PYINT7/SIGT(0:6,0:6,0:5)
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
      COMMON/PYSSMT/ZMIX(4,4),UMIX(2,2),VMIX(2,2),SMZ(4),SMW(2),
     &SFMIX(16,4),ZMIXI(4,4),UMIXI(2,2),VMIXI(2,2)
      COMMON/PYMSRV/RVLAM(3,3,3), RVLAMP(3,3,3), RVLAMB(3,3,3)
      COMMON/PYBINS/IHIST(4),INDX(1000),BIN(20000)
      SAVE /PYDAT1/,/PYDAT2/,/PYDAT3/,/PYDAT4/,/PYDATR/,/PYSUBS/,
     &/PYPARS/,/PYINT1/,/PYINT2/,/PYINT3/,/PYINT4/,/PYINT5/,
     &/PYINT6/,/PYINT7/,/PYMSSM/,/PYSSMT/,/PYMSRV/,/PYBINS/

C
C  SWITCH OFF pi0 pi+/- decay
C
      KC = PYCOMP(111)
      MDCY(KC, 1) = 0

      KC = PYCOMP(211)
      MDCY(KC, 1) = 0
C
C  SWITCH OFF eta decay
C     
      KC = PYCOMP(221)
      MDCY(KC, 1) = 0
C
C  SWITCH OFF K0, Ks, KL, K+/- decay
C     
      KC = PYCOMP(311)
      MDCY(KC, 1) = 0

      KC = PYCOMP(130)
      MDCY(KC, 1) = 0

      KC = PYCOMP(310)
      MDCY(KC, 1) = 0

      KC = PYCOMP(321)
      MDCY(KC, 1) = 0
C
C  SWITCH OFF MU DECAY
C
      KC = PYCOMP(13)
      MDCY(KC, 1) = 0
C
C  SWITCH OFF LAMBDA DECAY
C
      KC = PYCOMP(3122)
      MDCY(KC, 1) = 0

      END

      
      SUBROUTINE CHARMONIA_PYUPD
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER PYK,PYCHGE,PYCOMP
C...Commonblocks.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYDAT4/CHAF(500,2)
      CHARACTER CHAF*16
      COMMON/PYDATR/MRPY(6),RRPY(100)
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT1/MINT(400),VINT(400)
      COMMON/PYINT2/ISET(500),KFPR(500,2),COEF(500,20),ICOL(40,4,2)
      COMMON/PYINT3/XSFX(2,-40:40),ISIG(1000,3),SIGH(1000)
      COMMON/PYINT4/MWID(500),WIDS(500,5)
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
      COMMON/PYINT6/PROC(0:500)
      CHARACTER PROC*28
      COMMON/PYINT7/SIGT(0:6,0:6,0:5)
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
      COMMON/PYSSMT/ZMIX(4,4),UMIX(2,2),VMIX(2,2),SMZ(4),SMW(2),
     &SFMIX(16,4),ZMIXI(4,4),UMIXI(2,2),VMIXI(2,2)
      COMMON/PYMSRV/RVLAM(3,3,3), RVLAMP(3,3,3), RVLAMB(3,3,3)
      COMMON/PYBINS/IHIST(4),INDX(1000),BIN(20000)
      SAVE /PYDAT1/,/PYDAT2/,/PYDAT3/,/PYDAT4/,/PYDATR/,/PYSUBS/,
     &/PYPARS/,/PYINT1/,/PYINT2/,/PYINT3/,/PYINT4/,/PYINT5/,
     &/PYINT6/,/PYINT7/,/PYMSSM/,/PYSSMT/,/PYMSRV/,/PYBINS/
 
      PARAMETER(IPSI1DKT=4001)
      PARAMETER(IPSI2DKT=4301)
      PARAMETER(IPSI3DKT=4401)
      PARAMETER(IPSI0DKT=4501)
      PARAMETER(IXIC0DKT=4601)
      PARAMETER(IXIC1DKT=4701)
      PARAMETER(IXIC2DKT=4801)
      PARAMETER(IPSI7DKT=4901)
      DOUBLE PRECISION BRAT_PSI1(300)
      DOUBLE PRECISION BRAT_PSI2(100)
      DOUBLE PRECISION BRAT_PSI3(100)
      DOUBLE PRECISION BRAT_PSI0(100)
      DOUBLE PRECISION BRAT_PSI7(100)
      DOUBLE PRECISION BRAT_XIC0(100)
      DOUBLE PRECISION BRAT_XIC1(100)
      DOUBLE PRECISION BRAT_XIC2(100)

      INTEGER MDME_PSI1(2, 300), KFDP_PSI1(5, 300)
      INTEGER MDME_PSI2(2, 100), KFDP_PSI2(5, 100)
      INTEGER MDME_PSI3(2, 100), KFDP_PSI3(5, 100)
      INTEGER MDME_PSI7(2, 100), KFDP_PSI7(5, 100)
      INTEGER MDME_PSI0(2, 100), KFDP_PSI0(5, 100)
      INTEGER MDME_XIC0(2, 100), KFDP_XIC0(5, 100)
      INTEGER MDME_XIC1(2, 100), KFDP_XIC1(5, 100)
      INTEGER MDME_XIC2(2, 100), KFDP_XIC2(5, 100)

      DATA (BRAT_PSI1(J), MDME_PSI1(1, J), MDME_PSI1(2, J),
     &     KFDP_PSI1(1, J), KFDP_PSI1(2, J), KFDP_PSI1(3, J),
     &     KFDP_PSI1(4, J), KFDP_PSI1(5, J), J=1, 85)/
C------------------------------------------------------------------------
     &     5.93D-2, 1, 64, 11, -11, 0, 0, 0,          !  e+ e-
     &     5.88D-2, 1, 64, 13, -13, 0, 0, 0,          !  mu+ mu-
CC
C   gamma + P
CC
     &  1.30D-2, 1, 64, 22, 441, 0, 0, 0,          ! gamma + eta_c
     &  0.430D-2, 1, 64, 22, 331, 0, 0, 0,         ! gamma eta'
     &  0.086D-2, 1, 64, 22, 221, 0, 0, 0,         ! gamma eta #5
C------------------------------------------------------------------------
     &  0.004D-2, 1, 64, 22, 111, 0, 0, 0,         ! gamma pi0
     &  0.D0, 0, 0, 111, 2212, -2212, 0, 0,        ! pi0 p pbar
     &  0.D0, 0, 0, 221, 2212, -2212, 0, 0,        ! eta p pbar
     &  0.D0, 0, 0, 331, 2212, -2212, 0, 0,        ! eta' p pbar
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! # 10
C-----------------------------------------------------------------------
CC
CC    VP modes
CC
C--------------------------------------------------------------------
     &  0.42D-2, 1, 64, 113, 111, 0, 0, 0,         ! rho0 pi0
     &  0.42D-2, 1, 64, 213, -211, 0, 0, 0,        ! rho+ pi-
     &  0.42D-2, 1, 64, -213, 211, 0, 0, 0,        ! rho- pi+
     &  0.25D-2, 1, 64, 321, -323, 0, 0, 0,        ! K+ K*-
     &  0.25D-2, 1, 64, -321, 323, 0, 0, 0,        ! K- K*+   #15
C-----------------------------------------------------------
     &  0.21D-2, 1, 64, 311, -313, 0, 0, 0,        ! K0 K*b
     &  0.21D-2, 1, 64, -311, 313, 0, 0, 0,        ! K0b K*0
     &  0.158D-2, 1, 64, 223, 221, 0, 0, 0,        ! omega eta
     &  0.065D-2, 1, 64, 333, 221, 0, 0, 0,        ! phi eta
     &  0.019D-2, 1, 64, 113, 221, 0, 0, 0,        ! rho0 eta  #20
C-------------------------------------------------------------------
     &  0.017D-2, 1, 64, 223, 331, 0, 0, 0,        ! omega eta'
     &  0.033D-2, 1, 64, 333, 331, 0, 0, 0,        ! phi eta'
     &  0.010D-2, 1, 64, 113, 331, 0, 0, 0,        ! rho eta'
     &  0.042D-2, 1, 64, 223, 111, 0, 0, 0,        ! omega pi0
     &  0.000D-2, 0, 64, 333, 111, 0, 0, 0,        ! phi pi0     #25
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! # 30
C-------------------------------------------------------------------
CC
C   PP modes
CC
     &     0.024D-2, 1, 63, 321, -321, 0, 0, 0,       ! K+ K-
     &     0.015D-2, 1, 63, 211, -211, 0, 0, 0,       ! pi+ pi-
     &     0.015D-2, 1, 63, 310, 130, 0, 0, 0,        ! Ks KL
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! # 35
C-----------------------------------------------------------------
     &     13.40D-2, 1, 12, 82, -82, 0, 0, 0,        ! qq~
     &     65.83D-2, 1, 4, 21, 21, 21, 0, 0,         ! g+g+g 
     &     4.56D-2, 1, 4, 22, 21, 21, 0, 0,          ! gamma+g+g
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! # 40
C------------------------------------------------------------------
CC
C  Rare decays, semileptonic modes
CC  
C------------------------------------------------------------------ 
     &     0.00D0, 0, 42, 431, 11, -12, 0, 0,        ! Ds+ e- nue
     &     0.00D0, 0, 42, -431, -11, 12, 0, 0,       ! Ds- e+ nue
     &     0.00D0, 0, 42, 431, 13, -14, 0, 0,        ! Ds+ mu- nue
     &     0.00D0, 0, 42, -431, -13, 14, 0, 0,       ! Ds- mu+ nue
     &     0.00D0, 0, 42, 411, 11, -12, 0, 0,        ! D+ e- nue
     &     0.00D0, 0, 42, -411, -11, 12, 0, 0,       ! D- e+ nue
     &     0.00D0, 0, 42, 411, 13, -14, 0, 0,        ! D+ mu- nue
     &     0.00D0, 0, 42, -411, -13, 14, 0, 0,       ! D- mu+ nue
     &     0.00D0, 0, 0,  421, 11, -11, 0, 0,        ! D0 e+ e-
     &     0.00D0, 0, 0,  -421, 11, -11, 0, 0,       ! D0b e+ e-
     &     0.00D0, 0, 0,  421, 13, -13, 0, 0,        ! D0 mu+ mu-
     &     0.00D0, 0, 0,  -421, 13, -13, 0, 0,       ! D0b mu+ mu-
     &     0.00D0, 0, 0, 333, 11, -11, 0, 0,         ! phi e+ e-
     &     0.00D0, 0, 0, 333, 13, -13, 0, 0,         ! phi mu+ mu-
     &     0.00D0, 0, 0, 223, 11, -11, 0, 0,         ! omega e+ e-
     &     0.00D0, 0, 0, 223, 13, -13, 0, 0,         ! omega mu+ mu-
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! +20
C--------------------------------------------------------------------
     &     0.00D0, 0, 42, 433, 11, -12, 0, 0,        ! Ds*+ e- nue
     &     0.00D0, 0, 42, -433, -11, 12, 0, 0,       ! Ds*- e+ nue
     &     0.00D0, 0, 42, 433, 13, -14, 0, 0,        ! Ds*+ mu- nue
     &     0.00D0, 0, 42, -433, -13, 14, 0, 0,       ! Ds*- mu+ nue
     &     0.00D0, 0, 42, 413, 11, -12, 0, 0,        ! D*+ e- nue
     &     0.00D0, 0, 42, -413, -11, 12, 0, 0,       ! D*- e+ nue
     &     0.00D0, 0, 42, 413, 13, -14, 0, 0,        ! D*+ mu- nue
     &     0.00D0, 0, 42, -413, -13, 14, 0, 0,       ! D*- mu+ nue
     &     0.00D0, 0, 0,  423, 11, -11, 0, 0,        ! D*0 e+ e-
     &     0.00D0, 0, 0,  -423, 11, -11, 0, 0,       ! D*0b e+ e-
     &     0.00D0, 0, 0,  423, 13, -13, 0, 0,        ! D*0 mu+ mu-
     &     0.00D0, 0, 0,  -423, 13, -13, 0, 0,       ! D*0b mu+ mu-
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !  +15
C---------------------------------------------------------------
     &  0.D0, 0, 63, 431, -211, 0, 0, 0,           ! Ds+ pi-
     &  0.D0, 0, 63, -431, 211, 0, 0, 0,           ! Ds+ pi-
     &  0.D0, 0, 63, 411, -211, 0, 0, 0,           ! D+ pi-
     &  0.D0, 0, 63, -411, 211, 0, 0, 0,           ! D- pi+
     &  0.D0, 0, 63, 421, -311, 0, 0, 0,           ! D0 K0b
     &  0.D0, 0, 63, -421, 311, 0, 0, 0,           ! D0b K0
     &  0.D0, 0, 64, 421, 113, 0, 0, 0,            ! D0 rho0
     &  0.D0, 0, 64, -421, 113, 0, 0, 0,           ! D0b rho0
     &  0.D0, 0, 63, 411, -321, 0, 0, 0,           ! D+ K-
     &  0.D0, 0, 63, -411, 321, 0, 0, 0/           ! D- K+     +10
C----------------------------------------------------------------- 

      DATA (BRAT_PSI2(J), MDME_PSI2(1, J), MDME_PSI2(2, J),
     &     KFDP_PSI2(1, J), KFDP_PSI2(2, J), KFDP_PSI2(3, J),
     &  KFDP_PSI2(4, J), KFDP_PSI2(5, J), J=1,20)/
     &  0.755D-2, 1, 64, 11, -11, 0, 0, 0,    !  e+ e-
     &  0.730D-2, 1, 64, 13, -13, 0, 0, 0,    ! mu+ mu-
     &  0.028D-2, 1, 64, 15, -15, 0, 0, 0,    ! tau+ tau-
     &  33.54D-2,  1, 71, 211, -211, 443, 0, 0, ! pi+ pi- J/psi
     &  16.52D-2, 1, 72, 111, 111, 443, 0, 0,  ! pi0 pi0 J/psi
     &  3.25D-2, 1, 64, 221, 443, 0, 0, 0,    ! eta J/psi
     &  0.13D-2, 1, 64, 111, 443, 0, 0, 0,    ! pi0 J/psi
     &  0.32D-2, 1, 64, 22, 441, 0, 0, 0,     ! gamma + eta_c
     &  9.22D-2, 1, 64, 22, 10441, 0, 0, 0,   ! gamma + chi_c(0)
     &  9.07D-2, 1, 73, 22, 20443, 0, 0, 0,   ! gamma + chi_c(1)
     &  9.33D-2, 1, 0, 22, 445, 0, 0, 0,     ! gamma + chi_c(2)
     &  1.66D-2, 1, 12, 82, -82, 0, 0, 0,    ! qq~
     &  14.49D-2, 1, 4, 21, 21, 21, 0, 0,    ! g+g+g 
     &  0.96D-2, 1, 4, 22, 21, 21, 0, 0,     ! gamma + g + g 
     &  0.D0, 0, 0, 0, 0, 0, 0, 0,           ! #15
C-------------------------------------------------------
     &  0.D0, 0, 0, 113, 2212, -2212, 0, 0,  ! rho0 p pb
     &  0.D0, 0, 0, 223, 2212, -2212, 0, 0,  ! omega p pb
     &  0.D0, 0, 0, 111, 2212, -2212, 0, 0,
     &  0.D0, 0, 0, 221, 2212, -2212, 0, 0,
     &  0.D0, 0, 0, 331, 2212, -2212, 0, 0/
      DATA (BRAT_PSI3(J), MDME_PSI3(1, J), MDME_PSI3(2, J),
     &     KFDP_PSI3(1, J), KFDP_PSI3(2, J), KFDP_PSI3(3, J),
     &  KFDP_PSI3(4, J), KFDP_PSI3(5, J), J=1, 20)/
     &  0.755D-2, 1, 64, 11, -11, 0, 0, 0,    !  e+ e-
     &  0.730D-2, 1, 64, 13, -13, 0, 0, 0,    ! mu+ mu-
     &  0.028D-2, 1, 64, 15, -15, 0, 0, 0,    ! tau+ tau-
     &  33.54D-2,  1, 71, 211, -211, 443, 0, 0, ! pi+ pi- J/psi
     &  16.52D-2, 1, 72, 111, 111, 443, 0, 0,  ! pi0 pi0 J/psi
     &  3.25D-2, 1, 64, 221, 443, 0, 0, 0,    ! eta J/psi
     &  0.13D-2, 1, 64, 111, 443, 0, 0, 0,    ! pi0 J/psi
     &  0.32D-2, 1, 64, 22, 441, 0, 0, 0,     ! gamma + eta_c
     &  9.22D-2, 1, 64, 22, 10441, 0, 0, 0,   ! gamma + chi_c(0)
     &  9.07D-2, 1, 73, 22, 20443, 0, 0, 0,   ! gamma + chi_c(1)
     &  9.33D-2, 1, 0, 22, 445, 0, 0, 0,     ! gamma + chi_c(2)
     &  1.66D-2, 1, 12, 82, -82, 0, 0, 0,    ! qq~
     &  14.49D-2, 1, 4, 21, 21, 21, 0, 0,    ! g+g+g 
     &  0.96D-2, 1, 4, 22, 21, 21, 0, 0,     ! gamma + g + g 
     &  0.D0, 0, 0, 0, 0, 0, 0, 0,           ! #15
C-------------------------------------------------------
     &  0.D0, 0, 0, 113, 2212, -2212, 0, 0,  ! rho0 p pb
     &  0.D0, 0, 0, 223, 2212, -2212, 0, 0,  ! omega p pb
     &  0.D0, 0, 0, 111, 2212, -2212, 0, 0,
     &  0.D0, 0, 0, 221, 2212, -2212, 0, 0,
     &  0.D0, 0, 0, 331, 2212, -2212, 0, 0/

      DATA (BRAT_PSI7(J), MDME_PSI7(1, J), MDME_PSI7(2, J),
     &  KFDP_PSI7(1, J), KFDP_PSI7(2, J), KFDP_PSI7(3, J),
     &  KFDP_PSI7(4, J), KFDP_PSI7(5, J), J=1, 2)/
     &  100D-2,  1, 71, 211, -211, 443, 0, 0, ! pi+ pi- J/psi
     &  0.0D-2,  1, 0, 211, -211, 30443, 0, 0/ ! pi+ pi- psi(3770)

      DATA (BRAT_PSI0(J), MDME_PSI0(1, J), MDME_PSI0(2, J),
     &     KFDP_PSI0(1, J), KFDP_PSI0(2, J), KFDP_PSI0(3, J),
     &     KFDP_PSI0(4, J), KFDP_PSI0(5, J), J=1, 25)/
C--------------------------------------------------------------------
     &     1.20D-2, 1, 0, 211, -211, 211, -211, 0,  ! pi+ pi- pi+ pi-
     &     1.30D-2, 1, 0, 113, 113, 0, 0, 0,        ! rho0 rho0
     &     1.30D-2, 1, 0, 213, -213, 0, 0, 0,       ! rho+ rho- 
C--------------------------------------------------------------------
     &     0.14D-2, 1, 0, 321, -321, 321, -321, 0,  ! K+ K- K+ K-
     &     0.26D-2, 1, 0, 333, 333, 0, 0, 0,        ! phi phi
     &     0.29D-2, 1, 0, 333, 321, -321, 0, 0,     ! phi K+ K-
C-------------------------------------------------------------------
     &     2.45D-2, 1, 0, 221, 211, -211, 0, 0,     ! eta pi+ pi-
     &     2.45D-2, 1, 0, 221, 111, 111, 0, 0,      ! eta pi0 pi0
     &     2.05D-2, 1, 0, 331, 211, -211, 0, 0,     ! eta' pi+ pi-
     &     2.05D-2, 1, 0, 331, 111, 111, 0, 0,      ! eta' pi0 pi0
C-------------------------------------------------------------------
     &     1.90D-2, 1, 0, 321, -321, 111, 0, 0,     ! K+ K- pi0
     &     1.90D-2, 1, 0, 321, -311, -211, 0, 0,    ! K+ K0b pi-
     &     1.90D-2, 1, 0, -321, 311, 211, 0, 0,     ! K- K0 pi+
C--------------------------------------------------------------------
     &     1.50D-2, 1, 0, 321, -321, 211, -211, 0,  ! K+ K- pi+ pi-
     &     1.00D-2, 1, 0, 313, -321, 211, 0, 0,     ! K*0 K- pi+
     &     1.00D-2, 1, 0, -313, 321, -211, 0, 0,    ! K*b K+ pi-
     &     0.43D-2, 1, 0, 313, -313, 0, 0, 0,       ! K*0 K*b
     &     0.42D-2, 1, 0, 323, -323, 0, 0, 0,       ! K*+ K*-
C----------------------------------------------------------------------
     &     0.13D-2, 1, 0, 2212, -2212, 0, 0, 0,     ! p p~
     &     0.00D0,  0, 0, 211, -211, 0, 0, 0,       ! pi+ pi- parity violating
     &     0.00D0,  0, 0, 321, -321, 0, 0, 0,       ! K+ K- parity violating
     &     50.89D-2, 1, 32, 2, -2, 0, 0, 0,         ! *UU*
     &     12.72D-2, 1, 32, 1, -1, 0, 0, 0,         ! *DD*
     &     12.72D-2, 1, 32, 3, -3, 0, 0, 0,         ! *SS*
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/               ! 

C---------------------------------------------------------------------
      DATA (BRAT_XIC0(J), MDME_XIC0(1, J), MDME_XIC0(2, J),
     &     KFDP_XIC0(1, J), KFDP_XIC0(2, J), KFDP_XIC0(3, J),
     &     KFDP_XIC0(4, J), KFDP_XIC0(5, J), J=1, 25)/
     &     1.18D-2, 1, 0, 22, 443, 0, 0, 0,         ! gamma J/psi
     &     2.58D-2, 1, 0, 211, -211, 211, -211, 0,  ! pi+ pi- pi+ pi-
     &     2.10D-2, 1, 0, 211, -211, 321, -321, 0,  ! pi+ pi- K+ K-
     &     1.60D-2, 1, 0, 113, 211, -211, 0, 0,     ! rho0 pi+ pi-
     &     1.27D-2, 1, 0, 211, -211, 211, -211, 211, ! 6 pi   #5
C------------------------------------------------------------------
     &     0.00D+0, 0, 101, -211, 0, 0, 0, 0,    
     &     0.60D-2, 1, 0, 321, -313, -211, 0, 0,    ! K+ K*b pi-
     &     0.60D-2, 1, 0, -321, 313, 211, 0, 0,     ! K- K* pi+
     &     0.60D-2, 1, 0, 321, -321, 0, 0, 0,       ! K+ K-
     &     0.37D-2, 1, 0, 211, -211, 0, 0, 0,        ! pi+ pi-  #10
C---------------------------------------------------------
     &     0.37D-2, 1, 0, 111, 111, 0, 0, 0,        ! pi0 pi0
     &     0.21D-2, 1, 0, 221, 221, 0, 0, 0,        ! eta eta
     &     0.23D-2, 1, 0, 321, -321, 321, -321, 0,  ! K+ K- K+ K-
     &     0.21D-2, 1, 0, 310, 310, 0, 0, 0,        ! Ks Ks
     &     0.22D-2, 1, 0, 211, -211, 2212, -2212, 0, ! pi+ pi- p p~ #15
C--------------------------------------------------------------------
     &     0.10D-2, 1, 0, 333, 333, 0, 0, 0,        ! phi phi
     &     0.02D-2, 1, 0, 2212, -2212, 0, 0, 0,     ! p p~
     &     0.05D-2, 1, 0, 3122, -3122, 0, 0, 0,     ! lambda lambda~
     &     87.69D-2, 1, 12, 82, -82, 0, 0, 0,       ! qq~
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
C---------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/
C---------------------------------------------------------------------

C---------------------------------------------------------------------
      DATA (BRAT_XIC1(J), MDME_XIC1(1, J), MDME_XIC1(2, J),
     &     KFDP_XIC1(1, J), KFDP_XIC1(2, J), KFDP_XIC1(3, J),
     &     KFDP_XIC1(4, J), KFDP_XIC1(5, J), J=1, 20)/
C--------------------------------------------------------------------
     &     31.6D-2, 1, 0, 22, 443, 0, 0, 0,         ! gamma + J/psi
     &     0.62D-2, 1, 0, 211, -211, 211, -211, 211, ! 6 pi
     &     0.00D+0, 0, 101, -211, 0, 0, 0, 0,         
     &     0.82D-2, 1, 0, 211, -211, 211, -211, 0,  ! pi+ pi- pi+ pi-
     &     0.49D-2, 1, 0, 211, -211, 321, -321, 0,  ! pi+ pi- K+ K-
C---------------------------------------------------------------------
     &     0.39D-2, 1, 0, 113, 211, -211, 0, 0,     ! rho0 pi+ pi-
     &     0.16D-2, 1, 0, 321, -313, -211, 0, 0,    ! K+ K*b pi-
     &     0.16D-2, 1, 0, -321, 313, 211, 0, 0,     ! K- K* pi+
     &     0.125D-2, 1, 0, 310, 321, -211, 0, 0,    ! Ks K+ pi-
     &     0.125D-2, 1, 0, 310, -321, 211, 0, 0,    ! Ks K- pi+
C----------------------------------------------------------------------
     &     0.05D-2, 1, 0, 211, -211, 2212, -2212, 0,! pi+ pi- p p~
     &     0.04D-2, 1, 0, 321, -321, 321, -321, 0,  ! K+ K- K+ K-
     &     0.026D-2, 1, 0, 3122, -3122, 0, 0, 0,    ! lambda lambda~
     &     0.007D-2, 1, 0, 2212, -2212, 0, 0, 0,    ! p p~
     &     65.324D-2, 1, 12, 82, -82, 0, 0, 0,      ! qq~
C----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/
C---------------------------------------------------------------------
      DATA (BRAT_XIC2(J), MDME_XIC2(1, J), MDME_XIC2(2, J),
     &     KFDP_XIC2(1, J), KFDP_XIC2(2, J), KFDP_XIC2(3, J),
     &     KFDP_XIC2(4, J), KFDP_XIC2(5, J), J=1, 20)/
C------------------------------------------------------------------
     &     20.20D-2, 1, 0, 22, 443, 0, 0, 0,       ! gamma J/psi
     &     1.48D-2, 1, 0, 211, -211, 211, -211, 0, ! pi+ pi- pi+ pi-
     &     1.24D-2, 1, 0, 211, -211, 321, -321, 0, ! pi+ pi- K+ K-
     &     1.07D-2, 1, 0, 211, -211, 211, -211, 211, ! 6pi
     &     0.00D+0, 0, 101, -211, 0, 0, 0, 0,        
C---------------------------------------------------------------------
     &     0.70D-2, 1, 0, 113, 211, -211, 0, 0,    ! rho0 pi+ pi-
     &     0.24D-2, 1, 0, 321, -313, -211, 0, 0,   ! K+ K*b pi-
     &     0.24D-2, 1, 0, -321, 313, 211, 0, 0,    ! K- K*0 pi+
     &     0.24D-2, 1, 0, 333, 333, 0, 0, 0,       ! phi phi
     &     0.18D-2, 1, 0, 211, -211, 0, 0, 0,      ! pi + pi-
C--------------------------------------------------------------------
     &     0.11D-2, 1, 0, 111, 111, 0, 0, 0,       ! pi0 pi0
     &     0.18D-2, 1, 0, 321, -321, 321, -321, 0, ! K+ K- K+ K-
     &     0.17D-2, 1, 0, 211, -211, 2212, -2212, 0, ! pi+ pi- p p~
     &     0.09D-2, 1, 0, 321, -321, 0, 0, 0,      ! K+ K-
     &     0.07D-2, 1, 0, 310, 310, 0, 0, 0,       ! Ks Ks
C--------------------------------------------------------------------
     &     0.007D-2, 1, 0, 2212, -2212, 0, 0, 0,   ! p p~
     &     0.033D-2, 1, 0, 3122, -3122, 0, 0, 0,   ! lambda lambda~
     &     73.75D-2, 1, 12, 82, -82, 0, 0, 0,      ! qq~
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/
C----------------------------------------------------------------   

      IPPSI0 = PYCOMP(441)
      IPPSI1 = PYCOMP(443)
      IPPSI2 = PYCOMP(100443)
      IPPSI3 = PYCOMP(30443)
      IPPSI7 = PYCOMP(9030443)
      IPXIC0 = PYCOMP(10441)
      IPXIC1 = PYCOMP(20443)
      IPXIC2 = PYCOMP(445)


cc     
cc  J/psi decay fractions
cc
      MDCY(IPPSI1, 1) = 1
      MDCY(IPPSI1, 2) = IPSI1DKT
      MDCY(IPPSI1, 3) = 300


      DO I = 1, 300
         BRAT(IPSI1DKT+I-1) = BRAT_PSI1(I)
         DO J = 1, 2
            MDME(IPSI1DKT+I-1, J) = MDME_PSI1(J, I)
         END DO
         DO J = 1, 5
            KFDP(IPSI1DKT+I-1, J) = KFDP_PSI1(J, I)
         END DO
      END DO

      TOTBRPSI1 = 0.D0
      DO I = MDCY(IPPSI1, 2), MDCY(IPPSI1, 2) + MDCY(IPPSI1, 3) - 1
         TOTBRPSI1 = TOTBRPSI1 + BRAT(I)
      END DO

      DO I = MDCY(IPPSI1, 2), MDCY(IPPSI1, 2) + MDCY(IPPSI1, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRPSI1
      END DO
      
      
CC
CC  psi(2S) decay branching ratios
CC
      MDCY(IPPSI2, 1) = 1
      MDCY(IPPSI2, 2) = IPSI2DKT
      MDCY(IPPSI2, 3) = 100

      DO I = 1, 100
         BRAT(IPSI2DKT+I-1) = BRAT_PSI2(I)
         DO J = 1, 2
            MDME(IPSI2DKT+I-1, J) = MDME_PSI2(J, I)
         END DO
         DO J = 1, 5
            KFDP(IPSI2DKT+I-1, J) = KFDP_PSI2(J, I)
         END DO
      END DO

      TOTBRPSI2 = 0.D0
      DO I = MDCY(IPPSI2, 2), MDCY(IPPSI2, 2) + MDCY(IPPSI2, 3) - 1
         TOTBRPSI2 = TOTBRPSI2 + BRAT(I)
      END DO

      DO I = MDCY(IPPSI2, 2), MDCY(IPPSI2, 2) + MDCY(IPPSI2, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRPSI2
      END DO


cc
cc  psi(3770) non-DD~ decay fraction
cc       

      MDCY(IPPSI3, 1) = 1
      MDCY(IPPSI3, 2) = IPSI3DKT
      MDCY(IPPSI3, 3) = 100

      

      DO I = 1, 100
         BRAT(IPSI3DKT+I-1) = BRAT_PSI3(I)
         DO J = 1, 2
            MDME(IPSI3DKT+I-1, J) = MDME_PSI3(J, I)
         END DO
         DO J = 1, 5
            KFDP(IPSI3DKT+I-1, J) = KFDP_PSI3(J, I)
         END DO
      END DO

      TOTBRPSI3 = 0.D0
      DO I = MDCY(IPPSI3, 2), MDCY(IPPSI3, 2) + MDCY(IPPSI3, 3) - 1
         TOTBRPSI3 = TOTBRPSI3 + BRAT(I)
      END DO

      DO I = MDCY(IPPSI3, 2), MDCY(IPPSI3, 2) + MDCY(IPPSI3, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRPSI3
      END DO

cc
cc  Y(4260) decays 
cc       

      MDCY(IPPSI7, 1) = 1
      MDCY(IPPSI7, 2) = IPSI7DKT
      MDCY(IPPSI7, 3) = 100

      

      DO I = 1, 100
        BRAT(IPSI7DKT+I-1) = BRAT_PSI7(I)
         DO J = 1, 2
           MDME(IPSI7DKT+I-1, J) = MDME_PSI7(J, I)
         END DO
         DO J = 1, 5
           KFDP(IPSI7DKT+I-1, J) = KFDP_PSI7(J, I)
         END DO
      END DO

      TOTBRPSI7 = 0.D0
      DO I = MDCY(IPPSI7, 2), MDCY(IPPSI7, 2) + MDCY(IPPSI7, 3) - 1
        TOTBRPSI7 = TOTBRPSI7 + BRAT(I)
      END DO

      DO I = MDCY(IPPSI7, 2), MDCY(IPPSI7, 2) + MDCY(IPPSI7, 3) - 1
        BRAT(I) = BRAT(I) / TOTBRPSI7
      END DO


cc     
cc  eta_c decay fractions
cc
      MDCY(IPPSI0, 1) = 1
      MDCY(IPPSI0, 2) = IPSI0DKT
      MDCY(IPPSI0, 3) = 100

      DO I = 1, 100
         BRAT(IPSI0DKT+I-1) = BRAT_PSI0(I)
         DO J = 1, 2
            MDME(IPSI0DKT+I-1, J) = MDME_PSI0(J, I)
         END DO
         DO J = 1, 5
            KFDP(IPSI0DKT+I-1, J) = KFDP_PSI0(J, I)
         END DO
      END DO

      TOTBRPSI0 = 0.D0
      DO I = MDCY(IPPSI0, 2), MDCY(IPPSI0, 2) + MDCY(IPPSI0, 3) - 1
         TOTBRPSI0 = TOTBRPSI0 + BRAT(I)
      END DO

      DO I = MDCY(IPPSI0, 2), MDCY(IPPSI0, 2) + MDCY(IPPSI0, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRPSI0
      END DO

cc     
cc  chi_0c decay fractions
cc
      MDCY(IPXIC0, 1) = 1
      MDCY(IPXIC0, 2) = IXIC0DKT
      MDCY(IPXIC0, 3) = 100

      DO I = 1, 100
         BRAT(IXIC0DKT+I-1) = BRAT_XIC0(I)
         DO J = 1, 2
            MDME(IXIC0DKT+I-1, J) = MDME_XIC0(J, I)
         END DO
         DO J = 1, 5
            KFDP(IXIC0DKT+I-1, J) = KFDP_XIC0(J, I)
         END DO
      END DO

      TOTBRXIC0 = 0.D0
      DO I = MDCY(IPXIC0, 2), MDCY(IPXIC0, 2) + MDCY(IPXIC0, 3) - 1
         TOTBRXIC0 = TOTBRXIC0 + BRAT(I)
      END DO

      DO I = MDCY(IPXIC0, 2), MDCY(IPXIC0, 2) + MDCY(IPXIC0, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRXIC0
      END DO

cc     
cc  chi_1c decay fractions
cc
      MDCY(IPXIC1, 1) = 1
      MDCY(IPXIC1, 2) = IXIC1DKT
      MDCY(IPXIC1, 3) = 100

      DO I = 1, 100
         BRAT(IXIC1DKT+I-1) = BRAT_XIC1(I)
         DO J = 1, 2
            MDME(IXIC1DKT+I-1, J) = MDME_XIC1(J, I)
         END DO
         DO J = 1, 5
            KFDP(IXIC1DKT+I-1, J) = KFDP_XIC1(J, I)
         END DO
      END DO

      TOTBRXIC1 = 0.D0
      DO I = MDCY(IPXIC1, 2), MDCY(IPXIC1, 2) + MDCY(IPXIC1, 3) - 1
         TOTBRXIC1 = TOTBRXIC1 + BRAT(I)
      END DO

      DO I = MDCY(IPXIC1, 2), MDCY(IPXIC1, 2) + MDCY(IPXIC1, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRXIC1
      END DO

cc     
cc  chi_2c decay fractions
cc
      MDCY(IPXIC2, 1) = 1
      MDCY(IPXIC2, 2) = IXIC2DKT
      MDCY(IPXIC2, 3) = 100

      DO I = 1, 100
         BRAT(IXIC2DKT+I-1) = BRAT_XIC2(I)
         DO J = 1, 2
            MDME(IXIC2DKT+I-1, J) = MDME_XIC2(J, I)
         END DO
         DO J = 1, 5
            KFDP(IXIC2DKT+I-1, J) = KFDP_XIC2(J, I)
         END DO
      END DO

      TOTBRXIC2 = 0.D0
      DO I = MDCY(IPXIC2, 2), MDCY(IPXIC2, 2) + MDCY(IPXIC2, 3) - 1
         TOTBRXIC2 = TOTBRXIC2 + BRAT(I)
      END DO

      DO I = MDCY(IPXIC2, 2), MDCY(IPXIC2, 2) + MDCY(IPXIC2, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRXIC2
      END DO



      END 


      SUBROUTINE CHARM_MESON_PYUPD
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER PYK,PYCHGE,PYCOMP
C...Commonblocks.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYDAT4/CHAF(500,2)
      CHARACTER CHAF*16
      COMMON/PYDATR/MRPY(6),RRPY(100)
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT1/MINT(400),VINT(400)
      COMMON/PYINT2/ISET(500),KFPR(500,2),COEF(500,20),ICOL(40,4,2)
      COMMON/PYINT3/XSFX(2,-40:40),ISIG(1000,3),SIGH(1000)
      COMMON/PYINT4/MWID(500),WIDS(500,5)
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
      COMMON/PYINT6/PROC(0:500)
      CHARACTER PROC*28
      COMMON/PYINT7/SIGT(0:6,0:6,0:5)
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
      COMMON/PYSSMT/ZMIX(4,4),UMIX(2,2),VMIX(2,2),SMZ(4),SMW(2),
     &SFMIX(16,4),ZMIXI(4,4),UMIXI(2,2),VMIXI(2,2)
      COMMON/PYMSRV/RVLAM(3,3,3), RVLAMP(3,3,3), RVLAMB(3,3,3)
      COMMON/PYBINS/IHIST(4),INDX(1000),BIN(20000)
      SAVE /PYDAT1/,/PYDAT2/,/PYDAT3/,/PYDAT4/,/PYDATR/,/PYSUBS/,
     &/PYPARS/,/PYINT1/,/PYINT2/,/PYINT3/,/PYINT4/,/PYINT5/,
     &/PYINT6/,/PYINT7/,/PYMSSM/,/PYSSMT/,/PYMSRV/,/PYBINS/
 
      PARAMETER(IPD0DKT = 5001)
      PARAMETER(IPDPDKT = 5251)
      PARAMETER(IPDSDKT = 5501)

      DOUBLE PRECISION BRAT_D0(250)
      INTEGER MDME_D0(2, 250), KFDP_D0(5, 250)
      DOUBLE PRECISION BRAT_DP(250)
      INTEGER MDME_DP(2, 250), KFDP_DP(5, 250)
      DOUBLE PRECISION BRAT_DS(250)
      INTEGER MDME_DS(2, 250), KFDP_DS(5, 250)
       
      DATA (BRAT_D0(J), MDME_D0(1, J), MDME_D0(2, J),
     &     KFDP_D0(1, J), KFDP_D0(2, J), KFDP_D0(3, J),
     &     KFDP_D0(4, J), KFDP_D0(5, J), J=1,150)/
C
C  X e+ nu
C   
     &     1.98D-2, 1, 42, -323, -11, 12, 0, 0,  ! K*- e+ nue
     &     3.74D-2, 1, 42, -321, -11, 12, 0, 0,  ! K- e+ nue
     &     0.12D-2, 1, 42, -325, -11, 12, 0, 0,  ! K2*- e+ nue
     &     0.26D-2, 1, 42, -321, 111, -11, 12, 0, ! K- pi0 e+ nue
     &     0.26D-2, 1, 42, -311, -211, -11, 12, 0, ! K0b pi- e+ nue #5
C-------------------------------------------------------------------------
     &     0.20D-2, 1, 42, -211, -11, 12, 0, 0,   ! pi- e+ nue
     &     0.11D-2, 1, 42, -213, -11, 12, 0, 0,   ! rho- e+ nue
     &     0.03D-2, 1, 42, -10213, -11, 12, 0, 0, ! b1- e+ nue
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,             ! #10
C------------------------------------------------------------------------
C
C  X mu+ nu
C   
     &     1.98D-2, 1, 42, -323, -13, 14, 0, 0,  ! K*- mu+ numu
     &     3.74D-2, 1, 42, -321, -13, 14, 0, 0,  ! K- mu+ numu
     &     0.12D-2, 1, 42, -325, -13, 14, 0, 0,  ! K2*- mu+ numu
     &     0.26D-2, 1, 42, -321, 111, -13, 14, 0, ! K- pi0 mu+ numu
     &     0.26D-2, 1, 42, -311, -211, -13, 14, 0, ! K0b pi- mu+ numu #15
C-------------------------------------------------------------------------
     &     0.20D-2, 1, 42, -211, -13, 14, 0, 0,   ! pi- mu+ numu
     &     0.11D-2, 1, 42, -213, -13, 14, 0, 0,   ! rho- mu+ numu
     &     0.03D-2, 1, 42, -10213, -13, 14, 0, 0, ! b1- mu+ numu
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,             ! #20
C-------------------------------------------------------------------------
C
C  2-body Hadronic channels 
C    
     &     3.82D-2, 1, 0, -321, 211, 0, 0, 0,     ! K- pi+
     &     2.11D-2, 1, 0, -311, 111, 0, 0, 0,     ! K0b pi0
     &     0.70D-2, 1, 0, -311, 221, 0, 0, 0,     ! K0b eta
     &     0.84D-2, 1, 3, -311, 113, 0, 0, 0,     ! K0b rho0
     &     9.94D-2, 1, 3, -321, 213, 0, 0, 0,     ! K- rho+    #25
C-----------------------------------------------------------------------
     &     1.47D-2, 1, 3, -311, 223, 0, 0, 0,     ! K0b omega
     &     1.71D-2, 1, 0, -311, 331, 0, 0, 0,     ! K0b eta'
C     &     0.40D-2, 1, 0, -311, 9010221, 0, 0, 0, ! K0b f0
     &     0.00D-2, 0, 0, -311, 9010221, 0, 0, 0, ! K0b f0
     &     6.69D-2, 1, 0, -321, 20213, 0, 0, 0,   ! K- a1+
     &     0.29D-2, 1, 0, -311, 225, 0, 0, 0,     ! K0b f2   #30
C-----------------------------------------------------------------------
     &     0.48D-2, 1, 0, -311, 10221, 0, 0, 0,   ! K0b f0'
     &     3.50D-2, 1, 3, -323, 211, 0, 0, 0,     ! K*- pi+
     &     2.88D-2, 1, 3, -313, 111, 0, 0, 0,     ! K*0b pi0
     &     1.35D-2, 1, 0, -313, 113, 0, 0, 0,     ! K*0b rho0
     &     6.10D-2, 1, 0, -323, 213, 0, 0, 0,     ! K*- rho+   #35
C-----------------------------------------------------------------------
     &     0.97D-2, 1, 0, -10323, 211, 0, 0, 0,   ! K1- pi+
     &     0.71D-2, 1, 0, -10313, 111, 0, 0, 0,   ! K1b pi0
     &     0.73D-2, 1, 3, -323, 211, 0, 0, 0,     ! K*- pi+
     &     1.90D-2, 1, 3, -313, 221, 0, 0, 0,     ! K*b eta
     &     1.01D-2, 1, 0, -313, 223, 0, 0, 0,     ! K*b omega  # 40
C-----------------------------------------------------------------------
C
C  CKM suppressed modes
C
     &     0.15D-2, 1, 0, 211, -211, 0, 0, 0,     ! pi+ pi-
     &     0.08D-2, 1, 0, 111, 111, 0, 0, 0,      ! pi0 pi0
     &     0.10D-2, 1, 0, 221, 111, 0, 0, 0,      ! eta pi0
     &     0.10D-2, 1, 0, 331, 111, 0, 0, 0,      ! eta' pi0
     &     0.10D-2, 1, 0, 221, 221, 0, 0, 0,      ! eta eta   #45
C----------------------------------------------------------------
     &     0.94D-2, 1, 3, 213, -211, 0, 0, 0,     ! rho- pi+
     &     0.33D-2, 1, 3, -213, 211, 0, 0, 0,     ! rho+ pi-
     &     0.27D-2, 1, 3, 113, 111, 0, 0, 0,      ! rho0 pi0
C      
     &     0.434D-2, 1, 0, 321, -321, 0, 0, 0,    ! K+ K-
     &     0.0325D-2, 1, 0, 310, 310, 0, 0, 0,    ! Ks Ks   #50
C------------------------------------------------------------------
     &     0.0325D-2, 1, 0, 130, 130, 0, 0, 0,    ! KL KL
C     &     0.08D-2, 0, 3, -313, 311, 0, 0, 0,     ! K*0b K0
     &     0.00D-2, 0, 0, -313, 311, 0, 0, 0,     ! K*0b K0
     &     0.35D-2, 1, 3, 323, -321, 0, 0, 0,     ! K*+ K-
C     &     0.04D-2, 0, 3, 313, -311, 0, 0, 0,     ! K*0 K0b
     &     0.00D-2, 0, 0, 313, -311, 0, 0, 0,     ! K*0 K0b
     &     0.18D-2, 1, 3, -323, 321, 0, 0, 0,     ! K*- K+  #55
C----------------------------------------------------------------
C     &     0.07D-2, 0, 3, 333, 111, 0, 0, 0,      ! phi pi0
C     &     0.14D-2, 0, 3, 333, 221, 0, 0, 0,      ! phi eta
C     &     0.10D-2, 0, 0, 333, 223, 0, 0, 0,      ! phi omega
     &     0.00D-2, 0, 0, 333, 111, 0, 0, 0,      ! phi pi0
     &     0.00D-2, 0, 0, 333, 221, 0, 0, 0,      ! phi eta
     &     0.00D-2, 0, 0, 333, 223, 0, 0, 0,      ! phi omega
     &     0.069D-2, 1, 0, 333, 113, 0, 0, 0,     ! phi rho0
     &     0.087D-2, 1, 0, -313, 313, 0, 0, 0,    ! K*b K*  # 60
C------------------------------------------------------------------
     &     0.86D-2, 1, 3, -311, 333, 0, 0, 0,     ! K0b phi
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #65
C----------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #70
C----------------------------------------------------------------
C
C  3-body Hadronic channels 
C    
     &     1.30D-2, 1, 0, -313, 211, -211, 0, 0,     ! K*b pi+ pi-
     &     1.16D-2, 1, 0, -313, 111, 111, 0, 0,      ! K*b pi0 pi0
     &     1.00D-2, 1, 0, -323, 211, 111, 0, 0,      ! K*- pi+ pi0
     &     0.68D-2, 1, 0, -321, 213, 111, 0, 0,      ! K- rho+ pi0
     &     0.43D-2, 1, 0, -321, 211, 113, 0, 0,      ! K- pi+ rho0  #75
C-----------------------------------------------------------------------
     &     2.08D-2, 1, 0, -321, 211, 223, 0, 0,      ! K- pi+ omega
     &     1.00D-2, 1, 0, -321, 211, 221, 0, 0,      ! K- pi+ eta
     &     0.70D-2, 1, 0, -321, 211, 331, 0, 0,      ! K- pi+ eta'
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #80
C----------------------------------------------------------------------
C     &     1.03D-2, 1, 0, -311, 211, -211, 0, 0,     ! K0b pi+ pi-
     &     1.43D-2, 1, 0, -311, 211, -211, 0, 0,     ! K0b pi+ pi-
     &     0.63D-2, 1, 0, -321, 211, 111, 0, 0,      ! K- pi+ pi0
     &     0.78D-2, 1, 0, -311, 111, 111, 0, 0,      ! K0b pi0 pi0
     &     0.44D-2, 1, 0, -311, 321, -321, 0, 0,     ! K0b K+ K-
     &     0.066D-2, 1, 0, 310, 310, 310, 0, 0,      ! Ks Ks Ks    # 85
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #90
C-----------------------------------------------------------------------
C
C  CKM suppressed modes
C     
     &     0.06D-2, 1, 0, 211, -211, 111, 0, 0,      ! pi+ pi- pi0
     &     0.10D-2, 1, 0, 111, 111, 111, 0, 0,       ! pi0 pi0 pi0
     &     0.41D-2, 1, 0, 311, -321, 211, 0, 0,      ! K0 K- pi+
     &     0.38D-2, 1, 0, -311, 321, -211, 0, 0,     ! K0b K+ pi-
C     &     0.024D-2, 0, 0, 321, -321, 111, 0, 0,     ! K+ K- pi0
     &     0.000D-2, 0, 0, 321, -321, 111, 0, 0,     ! K+ K- pi0  #95
C----------------------------------------------------------------------
     &     0.056D-2, 1, 0, 321, -321, 113, 0, 0,     ! K+ K- rho0
     &     0.14D-2, 1, 0, 313, -321, 211, 0, 0,      ! K*0 K- pi+
     &     0.056D-2, 1, 0, -313, 321, -211, 0, 0,    ! K*0b K+ pi-
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #100
C----------------------------------------------------------------------
C
C  Multiple body hadronic channels
C
     &     1.60D-2, 1, 0, -321, 211, 211, -211, 0,  ! K- pi+ pi+ pi-
     &     2.49D-2, 1, 0, -311, 211, -211, 111, 0,  ! K0b pi+ pi- pi0
     &     2.08D-2, 1, 0, -321, 211, 111, 111, 0,   ! K- pi+ pi0 pi0
     &     1.65D-2, 1, 0, -311, 111, 111, 111, 0,   ! K0b pi0 pi0 pi0
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,               ! #105
C--------------------------------------------------------------------
     &     0.02D-2, 1, 0, 321, -321, -321, 211, 0,  ! K+ K- K- pi+
     &     0.72D-2, 1, 0, 321, -321, -311, 111, 0,  ! K+ K- K0b pi0
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #110
C---------------------------------------------------------------------
     &     0.74D-2, 1, 0, 211, 211, -211, -211, 0,   ! pi+ pi+ pi- pi-
     &     0.50D-2, 1, 0, 211, -211, 111, 111, 0,    ! pi+ pi- pi0 pi0
c     &     0.04D-2, 0, 0, 321, -321, 211, -211, 0,   ! K+ K- pi+ pi-
     &     0.00D-2, 0, 0, 321, -321, 211, -211, 0,   ! K+ K- pi+ pi-
     &     0.30D-2, 1, 0, 321, -321, 111, 111, 0,    ! K+ K- pi0 pi0
     &     0.64D-2, 1, 0, 311, -311, 211, -211, 0,   ! K0b K0 pi+ pi-  # 115
C-------------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! # 120
C--------------------------------------------------------------------------
     &     0.57D-2, 1, 0, -311, 211, -211, 211, -211, ! K0b pi+ pi- pi+ pi-
     &     0.93D-2, 1, 0, -321, 211, 211, -211, 111,  ! K- pi+ pi+ pi- pi0
     &     0.57D-2, 1, 0, -321, 211, 111, 111, 111,   ! K- pi+ pi0 pi0 pi0
     &     3.37D-2, 1, 0, -311, 211, -211, 111, 111,  ! K0b pi+ pi- pi0 pi0
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                 ! #125
C----------------------------------------------------------------------------
     &     1.89D-2, 1, 0, 211, -211, 211, -211, 111,  ! pi+ pi- pi+ pi- pi0
     &     0.60D-2, 1, 0, 211, -211, 111, 111, 111,   ! Pi+ pi- pi0 pi0 pi0
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0, 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                 ! #130
C---------------------------------------------------------------------------
     &     0.31D-2, 1, 0, 321, -321, 211, -211, 111,  ! K- K+ pi+ pi- pi0
     &     0.04D-2, 1, 0, 211, -211, 211, -211, 211,
     &     0.00D0, 0, 101, -211, 0, 0, 0, 0,
     &     3.37D-2, 1, 0, -311, 211, -211, 111, 111,  ! K0b pI+ pi- pi0 pi0 pi0
     &     0.00D0, 0, 101, 111, 0, 0, 0, 0,           ! #135
C------------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                 ! #140
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                 ! #145
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/                 ! #150
C------------------------------------------------------------------------

      DATA (BRAT_DP(J), MDME_DP(1, J), MDME_DP(2, J),
     &     KFDP_DP(1, J), KFDP_DP(2, J), KFDP_DP(3, J),
     &     KFDP_DP(4, J), KFDP_DP(5, J), J=1,150)/
C     
C  X e+ nue
C
     &     5.04D-2, 1, 42, -313, -11, 12, 0, 0,      ! K*b e+ nue
     &     9.29D-2, 1, 42, -311, -11, 12, 0, 0,      ! K0b e+ nue
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #5
C-----------------------------------------------------------------------
     &     0.30D-2, 1, 42, -315, -11, 12, 0, 0,      ! K2*b e+ nue
     &     0.43D-2, 1, 42, -321, 211, -11, 12, 0,    ! K- pi+ e+ nue
     &     0.43D-2, 1, 42, -311, 111, -11, 12, 0,    ! K0b pi0 e+ nue  
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! # 10
C------------------------------------------------------------------------
     &     0.43D-2, 1, 42, 111, -11, 12, 0, 0,       ! pi0 e+ nue
     &     0.24D-2, 1, 42, 113, -11, 12, 0, 0,       ! rho0 e+ nue
     &     0.26D-2, 1, 42, 221, -11, 12, 0, 0,       ! eta e+ nue
     &     0.05D-2, 1, 42, 331, -11, 12, 0, 0,       ! eta' e+ nue
     &     0.22D-2, 1, 42, 223, -11, 12, 0, 0,       ! omega e+ nue  # 15
C-------------------------------------------------------------------------
     &     0.02D-2, 1, 42, 10113, -11, 12, 0, 0,     ! b10 e+ nue
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #20
C------------------------------------------------------------------------
C     
C  X mu+ numu
C
     &     5.04D-2, 1, 42, -313, -13, 14, 0, 0,      ! K*b mu+ numu
     &     9.29D-2, 1, 42, -311, -13, 14, 0, 0,      ! K0b mu+ numu
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #25
C-------------------------------------------------------------------------
     &     0.30D-2, 1, 42, -315, -13, 14, 0, 0,      ! K2*b mu+ numu
     &     0.43D-2, 1, 42, -321, 211, -13, 14, 0,    ! K- pi+ mu+ numu
     &     0.43D-2, 1, 42, -311, 111, -13, 14, 0,    ! K0b pi0 mu+ numu  
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! # 30
C------------------------------------------------------------------------
     &     0.43D-2, 1, 42, 111, -13, 14, 0, 0,       ! pi0 mu+ numu
     &     0.24D-2, 1, 42, 113, -13, 14, 0, 0,       ! rho0 mu+ numu
     &     0.26D-2, 1, 42, 221, -13, 14, 0, 0,       ! eta mu+ numu
     &     0.05D-2, 1, 42, 331, -13, 14, 0, 0,       ! eta' mu+ numu
     &     0.22D-2, 1, 42, 223, -13, 14, 0, 0,       ! omega mu+ numu  # 35
C--------------------------------------------------------------------------
     &     0.02D-2, 1, 42, 10113, -13, 14, 0, 0,     ! b10 mu+ numu
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #40
C-------------------------------------------------------------------------
     &     0.09D-2, 1, 42, -13, 14, 0, 0, 0,         ! mu+ numu
     &     0.24D-2, 1, 42, -15, 16, 0, 0, 0,         ! tau+ nutau 
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #45
C--------------------------------------------------------------------------
C
C 2-body hadronic channels
C
     &     2.89D-2, 1, 0, -311, 211, 0, 0, 0,        ! K0b pi+
     &     6.60D-2, 1, 3, -311, 213, 0, 0, 0,        ! K0b rho+
     &     6.99D-2, 1, 0, -311, 20213, 0, 0, 0,      ! K0b a1+
     &     1.40D-2, 1, 3, -313, 211, 0, 0, 0,        ! K*b pi+
     &     3.37D-2, 1, 3, -30313, 211, 0, 0, 0,      ! K1*b pi+  # 50
C----------------------------------------------------------------------
     &     2.72D-2, 1, 3, -10311, 211, 0, 0, 0,      ! K0*b  pi+
     &     1.45d-2, 1, 0, -313, 213, 0, 0, 0,        ! K*b   rho+
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #55
C--------------------------------------------------------------------------
C
C 3-body hadronic channels
C
     &     6.25D-2, 1, 0, -321, 211, 211, 0, 0,      ! K- pi+ pi+
     &     1.73D-2, 1, 0, -311, 111, 211, 0, 0,      ! K0b pi0 pi+
     &     1.00D-2, 1, 0, -311, 221, 211, 0, 0,      ! K0b eta pi+
     &     0.44D-2, 1, 0, -311, 113, 211, 0, 0,      ! K0b rho0 pi+
     &     0.22D-2, 1, 0, -311, 223, 211, 0, 0,      ! K0b omega pi+  #60
C---------------------------------------------------------------------
     &     0.76D-2, 1, 0, -321, 213, 211, 0, 0,      ! K- rho+ pi+
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #65
C----------------------------------------------------------------------
     &     1.38D-2, 1, 0, -323, 211, 211, 0, 0,      ! K*- pi+ pi+
     &     2.89D-2, 1, 0, -313, 111, 211, 0, 0,      ! K*b pi0 pi+
     &     0.44D-2, 1, 0, -313, 221, 211, 0, 0,      ! K*b eta pi+
     &     0.29D-2, 1, 0, -313, 113, 211, 0, 0,      ! K*b rho0 pi+
     &     0.22D-2, 1, 0, -313, 223, 211, 0, 0,      ! K*b omega pi+  #70
C---------------------------------------------------------------------   
     &     1.00D-2, 1, 0, -323, 213, 211, 0, 0,      ! K*- rho+ pi+
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #75
C-----------------------------------------------------------------------
     &     0.83D-2, 1, 0, -321, 211, 211, 111, 0,    ! K- pi+ pi+ pi0
     &     0.70D-2, 1, 0, -311, 211, 211, -211, 0,   ! K0b pi+ pi+ pi-
     &     0.75D-2, 1, 0, -311, 211, 111, 111, 0,    ! K0b pi+ pi0 pi0
     &     0.02D-2, 1, 0, -321, 113, 211, 211, 0,    ! K- rho0 pi+ pi+
     &     0.43D-2, 1, 0, -313, 211, 211, -211, 0,   ! K*b pi+ pi+ pi-  #80
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #85
C-----------------------------------------------------------------------
     &     0.22D-2, 1, 0, -321, 211, 211, 211, -211, ! K- pi+ pi+ pi+ pi-
     &     2.65D-2, 1, 0, -311, 211, 211, -211, 111, ! K0b pi+ pi+ pi- pi0
     &     1.87D-2, 1, 0, -321, 211, 211, 111, 111,  ! K- pi+ pi+ pi0 pi0
     &     0.35D-2, 1, 0, -311, 211, 111, 111, 111,  ! K0b pi+ pi0 pi0 pi0
     &     0.08D-2, 1, 0, -311, 211, 211, 211, -211, ! K0b pi+ pi+ pi+ pi- pi- #90
C-------------------------------------------------------------------------
     &     0.00D0, 0, 101, -211, 0, 0, 0, 0,         
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #95
C---------------------------------------------------------------------------
C
C CKM suppressed modes
C
     &     1.80D-2, 1, 0, -311, -311, 321, 0, 0,     ! K0b K0b K+
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #100
C---------------------------------------------------------------------------
     &     0.52D-2, 1, 3, 333, 211, 0, 0, 0,         ! phi pi+
     &     1.88D-2, 1, 0, 333, 211, 111, 0, 0,       ! phi pi+ pi0
     &     0.74D-2, 1, 0, -311, 321, 0, 0, 0,        ! K0b K+
     &     0.36D-2, 1, 3, -313, 321, 0, 0, 0,        ! K*b K+
     &     3.20D-2, 1, 3, -311, 323, 0, 0, 0,        ! K0b K*+    #105
C---------------------------------------------------------------------
     &     1.95D-2, 1, 3, -313, 323, 0, 0, 0,        ! K*B K*+
     &     0.38D-2, 1, 0, 321, -321, 211, 0, 0,      ! K+ K- pi+
     &     0.10D-2, 1, 0, 321, -311, 111, 0, 0,      ! K+ K0b pi0
     &     0.10D-2, 1, 0, -311, 311, 211, 0, 0,      ! K0b K0 pi+
     &     0.10D-2, 1, 0, 323, -321, 211, 0, 0,       ! K*+ K- pi+  #110
C-------------------------------------------------------------------
     &     0.10D-2, 1, 0, 321, -323, 211, 0, 0,      ! K+ K*- pi+
     &     0.10D-2, 1, 0, 323, -311, 111, 0, 0,      ! K*+ K0b pi0
     &     0.10D-2, 1, 0, 321, -313, 111, 0, 0,      ! K+ K*b pi0
     &     0.10D-2, 1, 0, -313, 311, 211, 0, 0,      ! K*b K0 pi+
     &     0.10D-2, 1, 0, -311, 313, 211, 0, 0,      ! K0b K*0 pi+ #115
C--------------------------------------------------------------------
     &     0.25D-2, 1, 0, 111, 211, 0, 0, 0,         ! pi0 pi+
     &     0.07D-2, 1, 3, 113, 211, 0, 0, 0,         ! rho0 pi+
     &     0.24D-2, 1, 0, 211, -211, 211, 0, 0,      ! pi+ pi- pi+
     &     0.30D-2, 1, 0, 211, 111, 111, 0, 0,       ! pi+ pi0 pi0
     &     1.64D-2, 1, 0, 211, 211, -211, 111, 0,    ! pi+ pi+ pi- pi0 #120
C---------------------------------------------------------------------
     &     0.20D-2, 1, 0, 211, 111, 111, 111, 0,     ! pi+ pi0 pi0 pi0
     &     0.21D-2, 1, 0, 211, 211, 211, -211, -211, ! pi+ pi+ pi+ pi- pi-
     &     0.22D-2, 1, 0, 211, 211, 211, -211, -211, ! pi+ pi+ pi+ pi- pi- pi0
     &     0D0, 0, 0, 101, 0, 0, 0, 0,                
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                ! #125
C---------------------------------------------------------------------------
     &     0.75D-2, 1, 0, 221, 211, 0, 0, 0,         ! eta pi+
     &     0.30D-2, 1, 0, 221, 211, 111, 0, 0,       ! eta pi+ pi0
     &     0.12D-2, 1, 0, 221, 211, 211, -211, 0,    ! eta pi+ pi+ pi-
     &     0.08D-2, 1, 0, 221, 211, 111, 111, 0,     ! eta pi+ pi0 pi0
     &     0.30D-2, 1, 0, 331, 211, 0, 0, 0,         ! eta' pi+       #130
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                 ! #135
C------------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                 ! #140
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                 ! #145
C-----------------------------------------------------------------------
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/                 ! #150
C------------------------------------------------------------------------
      DATA (BRAT_DS(J), MDME_DS(1, J), MDME_DS(2, J),
     &     KFDP_DS(1, J), KFDP_DS(2, J), KFDP_DS(3, J),
     &     KFDP_DS(4, J), KFDP_DS(5, J), J=1,80)/
C---------------------------------------------------------------------------
C
C  X e+ enu
C
     &     1.90D-2, 1, 42, 333, -11, 12, 0, 0,       ! phi e+ nue
     &     2.50D-2, 1, 42, 221, -11, 12, 0, 0,       ! eta e+ nue
     &     0.90D-2, 1, 42, 331, -11, 12, 0, 0,       ! eta' e+ nue
     &     0.27D-2, 1, 42, -311, -11, 12, 0, 0,      ! K0b e+ nue
     &     0.10D-2, 1, 42, -313, -11, 12, 0, 0,      ! K*b e+ nue   #5
C-----------------------------------------------------------------   
     &     1.115D-2, 1, 42, 321, -321, -11, 12, 0,   ! K+ K- e+ nue
     &     1.115D-2, 1, 42, 311, -311, -11, 12, 0,   ! K0 K0b e+ nue
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,       
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !              #10
C------------------------------------------------------------------          
C
C  X mu+ munu
C
     &     1.90D-2, 1, 42, 333, -13, 14, 0, 0,       ! phi e+ nue
     &     2.50D-2, 1, 42, 221, -13, 14, 0, 0,       ! eta e+ nue
     &     0.90D-2, 1, 42, 331, -13, 14, 0, 0,       ! eta' e+ nue
     &     0.27D-2, 1, 42, -311, -13, 14, 0, 0,      ! K0b e+ nue
     &     0.10D-2, 1, 42, -313, -13, 14, 0, 0,      ! K*b e+ nue   #15
C-----------------------------------------------------------------   
     &     1.115D-2, 1, 42, 321, -321, -13, 14, 0,   ! K+ K- e+ nue
     &     1.115D-2, 1, 42, 311, -311, -13, 14, 0,   ! K0 K0b e+ nue
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !              
     &     0.60D-2, 1, 42, -13, 14, 0, 0, 0,         ! mu+ numu
     &     6.00D-2, 1, 42, -15, 16, 0, 0, 0,         ! tau+ nu_tau  #20
C------------------------------------------------------------------          
C     
C Hadronic decays
C
     &     3.60D-2, 1, 3, 333, 211, 0, 0, 0,         ! phi pi+
     &     1.73D-2, 1, 0, 221, 211, 0, 0, 0,         ! eta pi+
     &     3.71D-2, 1, 0, 331, 211, 0, 0, 0,         ! eta' pi+
     &     0.27D-2, 1, 3, 223, 211, 0, 0, 0,         ! omega pi+
     &     0.10D-2, 1, 3, 113, 211, 0, 0, 0,         ! rho0 pi+     #25
C----------------------------------------------------------------------
     &     0.20D-2, 1, 3, 213, 111, 0, 0, 0,         ! rho+ pi0
     &     0.10D-2, 1, 0, 211, 111, 0, 0, 0,         ! pi+ pi0
C     &     1.20D-2, 1, 0, 9010221, 211, 0, 0, 0,     ! f0 pi+
     &     1.20D-2, 1, 0, 10221, 211, 0, 0, 0,     ! f0 pi+
     &     6.70D-2, 1, 0, 333, 213, 0, 0, 0,         ! phi rho+
     &     10.73D-2, 1, 3, 221, 213, 0, 0, 0,        ! eta rho+      #30
C-----------------------------------------------------------------------
     &     10.00D-2, 1, 3, 331, 213, 0, 0, 0,        ! eta' rho+
     &     1.00D-2, 1, 0, 211, 211, -211, 0, 0,      ! pi+ pi- pi+
     &     0.50D-2, 1, 0, 211, 111, 111, 0, 0,       ! pi+ pi0 pi0
     &     2.30D-2, 1, 0, 333, 211, 111, 0, 0,       ! phi pi+ pi0
     &     1.00D-2, 1, 0, 221, 211, 111, 0, 0,       ! eta pi+ pi0   #35
C--------------------------------------------------------------------
     &     1.00D-2, 1, 0, 331, 211, 111, 0, 0,       ! eta' pi+ pi0
     &     1.18D-2, 1, 0, 333, 211, -211, 211, 0,    ! phi pi+ pi- pi+
     &     0.50D-2, 1, 0, 333, 211, 111, 111, 0,     ! phi pi+ pi0 pi0
     &     0.50D-2, 1, 0, 221, 211, -211, 211, 0,    ! eta pi+ pi- pi+
     &     0.50D-2, 1, 0, 221, 211, 111, 111, 0,     ! eta pi+ pi0 pi0 #40
C--------------------------------------------------------------------------
     &     0.72D-2, 1, 0, 211, 211, 211, -211, -211, ! pi+ pi+ pi+ pi- pi-
     &     4.06D-2, 1, 0, 211, 211, 211, -211, -211, ! pi+ pi+ pi+ pi- pi- pi0
     &     0.00D-2, 0, 101, 111, 0, 0, 0, 0,         
     &     3.60D-2, 1, 0, -311, 321, 0, 0, 0,        ! K0b K+
     &     3.40D-2, 1, 3, -313, 321, 0, 0, 0,        ! K*b K+          #45
C--------------------------------------------------------------------------
     &     4.30D-2, 1, 3, -311, 323, 0, 0, 0,        ! K0b  K*+
     &     5.80D-2, 1, 0, -313, 323, 0, 0, 0,        ! K*b K*+
     &     0.70D-2, 1, 0, -10311, 321, 0, 0, 0,      ! K0*b K+
     &     0.90D-2, 1, 0, 321, -321, 211, 0, 0,      ! K+ K- pi+
     &     0.50D-2, 1, 0, -311, 321, 111, 0, 0,      ! K0b K+ pi0      #50
C---------------------------------------------------------------------
     &     0.20D-2, 1, 0, -311, 321, 211, -211, 0,   ! K0b K+ pi+ pi-
     &     1.70D-2, 1, 0, 311, -321, 211, 211, 0,    ! K0 K- pi+ pi+
     &     0.20D-2, 1, 0, -311, 321, 111, 111, 0,    ! K0b K+ pi0 pi0
     &     0.30D-2, 1, 0, 321, -321, 211, 211, -211, ! K+ K- pi+ pi+ pi-
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !                  #55
C-----------------------------------------------------------------------------
     &     0.30D-2, 1, 3, 333, 321, 0, 0, 0,         ! phi K+
     &     0.15D-2, 1, 0, 221, 321, 0, 0, 0,         ! eta K+
     &     0.35D-2, 1, 0, 331, 321, 0, 0, 0,         ! eta' K+
     &     0.30D-2, 1, 0, 221, 321, 111, 0, 0,       ! eta K+ pi0
     &     0.20D-2, 1, 0, 221, 321, 211, -211, 0,    ! eta K+ pi+ pi-   #60
C----------------------------------------------------------------------------
     &     0.20D-2, 1, 0, 331, 321, 111, 0, 0,       ! eta' K+ pi0
     &     0.10D-2, 1, 0, 331, 321, 211, -211, 0,    ! eta' K+ pi+ pi-
     &     0.30D-2, 1, 0, 321, -321, 321, 0, 0,      ! K+ K- K+
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !                  
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !                  #65
C----------------------------------------------------------------------------
     &     0.25D-2, 1, 0, 311, 211, 0, 0, 0,         ! K0 pi+
     &     0.25D-2, 1, 3, 311, 213, 0, 0, 0,         ! K0 rho+
     &     0.25D-2, 1, 3, 321, 113, 0, 0, 0,         ! K+ rho0
     &     0.10D-2, 1, 0, 311, 211, 111, 0, 0,       ! K0 pi+ pi0
     &     0.32D-2, 1, 0, 321, 211, -211, 0, 0,      ! K+ pi+ pi-       #70
C---------------------------------------------------------------------------
     &     0.20D-2, 1, 3, 311, 20213, 0, 0, 0,       ! K0 a1+
     &     0.65D-2, 1, 3, 313, 211, 0, 0, 0,         ! K*0 pi+
     &     0.25D-2, 1, 0, 313, 213, 0, 0, 0,         ! K*0 rho+
     &     0.23D-2, 1, 0, 313, 211, 111, 0, 0,       ! K*0 pi+ pi0
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !                  #75
C---------------------------------------------------------------------------
     &     0.030D0, 1, 0, 2212, -2112, 0, 0, 0,      ! p nbar                  
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !                  
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !                  
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,                !                  
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/                !                  #80
C---------------------------------------------------------------------------


      IPD0 = PYCOMP(421)
      IPDP = PYCOMP(411)
      IPDS = PYCOMP(431)

CC
CC   D0 decay fractions
CC
      MDCY(IPD0, 1) = 1
      MDCY(IPD0, 2) = IPD0DKT
      MDCY(IPD0, 3) = 250

      DO I = 1, 250
         BRAT(I+IPD0DKT-1) = BRAT_D0(I)
         DO J = 1, 2
            MDME(I+IPD0DKT-1, J) = MDME_D0(J, I)
         END DO
         DO J = 1, 5
            KFDP(I+IPD0DKT-1, J) = KFDP_D0(J, I)
         END DO
      END DO

      TOTBRD0 = 0.D0
      DO I = MDCY(IPD0, 2), MDCY(IPD0, 2) + MDCY(IPD0, 3) - 1
         TOTBRD0 = TOTBRD0 + BRAT(I)
      END DO
      DO I = MDCY(IPD0, 2), MDCY(IPD0, 2) + MDCY(IPD0, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRD0
      END DO
CC
CC   D+ decay fractions
CC

      MDCY(IPDP, 1) = 1
      MDCY(IPDP, 2) = IPDPDKT
      MDCY(IPDP, 3) = 250

      DO I = 1, 250
         BRAT(I+IPDPDKT-1) = BRAT_DP(I)
         DO J = 1, 2
            MDME(I+IPDPDKT-1, J) = MDME_DP(J, I)
         END DO
         DO J = 1, 5
            KFDP(I+IPDPDKT-1, J) = KFDP_DP(J, I)
         END DO
      END DO

      TOTBRDP = 0.D0
      DO I = MDCY(IPDP, 2), MDCY(IPDP, 2) + MDCY(IPDP, 3) - 1
         TOTBRDP = TOTBRDP + BRAT(I)
      END DO
      DO I = MDCY(IPDP, 2), MDCY(IPDP, 2) + MDCY(IPDP, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRDP
      END DO

CC
CC   DS+ decay fractions
CC

      MDCY(IPDS, 1) = 1
      MDCY(IPDS, 2) = IPDSDKT
      MDCY(IPDS, 3) = 250

      DO I = 1, 250
         BRAT(I+IPDSDKT-1) = BRAT_DS(I)
         DO J = 1, 2
            MDME(I+IPDSDKT-1, J) = MDME_DS(J, I)
         END DO
         DO J = 1, 5
            KFDP(I+IPDSDKT-1, J) = KFDP_DS(J, I)
         END DO
      END DO

      TOTBRDS = 0.D0
      DO I = MDCY(IPDS, 2), MDCY(IPDS, 2) + MDCY(IPDS, 3) - 1
         TOTBRDS = TOTBRDS + BRAT(I)
      END DO
      DO I = MDCY(IPDS, 2), MDCY(IPDS, 2) + MDCY(IPDS, 3) - 1
         BRAT(I) = BRAT(I) / TOTBRDS
      END DO



      END 


      SUBROUTINE OTHER_MESON_PYUPD
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER PYK,PYCHGE,PYCOMP
C...Commonblocks.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYDAT4/CHAF(500,2)
      CHARACTER CHAF*16
      COMMON/PYDATR/MRPY(6),RRPY(100)
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT1/MINT(400),VINT(400)
      COMMON/PYINT2/ISET(500),KFPR(500,2),COEF(500,20),ICOL(40,4,2)
      COMMON/PYINT3/XSFX(2,-40:40),ISIG(1000,3),SIGH(1000)
      COMMON/PYINT4/MWID(500),WIDS(500,5)
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
      COMMON/PYINT6/PROC(0:500)
      CHARACTER PROC*28
      COMMON/PYINT7/SIGT(0:6,0:6,0:5)
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
      COMMON/PYSSMT/ZMIX(4,4),UMIX(2,2),VMIX(2,2),SMZ(4),SMW(2),
     &SFMIX(16,4),ZMIXI(4,4),UMIXI(2,2),VMIXI(2,2)
      COMMON/PYMSRV/RVLAM(3,3,3), RVLAMP(3,3,3), RVLAMB(3,3,3)
      COMMON/PYBINS/IHIST(4),INDX(1000),BIN(20000)
      SAVE /PYDAT1/,/PYDAT2/,/PYDAT3/,/PYDAT4/,/PYDATR/,/PYSUBS/,
     &/PYPARS/,/PYINT1/,/PYINT2/,/PYINT3/,/PYINT4/,/PYINT5/,
     &/PYINT6/,/PYINT7/,/PYMSSM/,/PYSSMT/,/PYMSRV/,/PYBINS/
 
      PARAMETER(IKST10DKT = 6001)
      PARAMETER(IKST1CDKT = 6011)

      DOUBLE PRECISION BRAT_KST10(10)
      INTEGER MDME_KST10(2, 10), KFDP_KST10(5, 10)
      DOUBLE PRECISION BRAT_KST1C(10)
      INTEGER MDME_KST1C(2, 10), KFDP_KST1C(5, 10)
      
      DATA (BRAT_KST10(J), MDME_KST10(1, J), MDME_KST10(2, J),
     &     KFDP_KST10(1, J), KFDP_KST10(2, J), KFDP_KST10(3, J),
     &     KFDP_KST10(4, J), KFDP_KST10(5, J), J = 1, 10)/
     &     25.8D-2, 1, 3, 321, -211, 0, 0, 0,  ! K+ pi-
     &     12.9D-2, 1, 3, 311, 111, 0, 0, 0,   ! K0 pi0
     &     20.9D-2, 1, 0, 321, -213, 0, 0, 0,  ! K+ rho-
     &     13.5D-2, 1, 0, 311, 113, 0, 0, 0,   ! K0 rho0
     &     19.9D-2, 1, 0, 323, -211, 0, 0, 0,  ! K*+ pi-
     &     10.0D-2, 1, 0, 313, 111, 0, 0, 0,   ! K*0 pi0
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/
      DATA (BRAT_KST1C(J), MDME_KST1C(1, J), MDME_KST1C(2, J),
     &     KFDP_KST1C(1, J), KFDP_KST1C(2, J), KFDP_KST1C(3, J),
     &     KFDP_KST1C(4, J), KFDP_KST1C(5, J), J = 1, 10)/
     &     25.8D-2, 1, 3, 311, 211, 0, 0, 0,   ! K0 pi+
     &     12.9D-2, 1, 3, 321, 111, 0, 0, 0,   ! K+ pi0
     &     20.9D-2, 1, 0, 311, 213, 0, 0, 0,   ! K0 rho+
     &     13.5D-2, 1, 0, 321, 113, 0, 0, 0,   ! K+ rho0
     &     19.9D-2, 1, 0, 313, 211, 0, 0, 0,   ! K*0 pi+
     &     10.0D-2, 1, 0, 323, 111, 0, 0, 0,   ! K*+ pi0
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,
     &     0.D0, 0, 0, 0, 0, 0, 0, 0,
     &     0.D0, 0, 0, 0, 0, 0, 0, 0/

      IKST10 = PYCOMP(30313)
      IKST1C = PYCOMP(30323)
C
C  K1*(1680)      
C
      
C      PMAS(IKST10, 1) = 1.717D0
C      PMAS(IKST10, 2) = 0.322D0
C      PMAS(IKST10, 3) = 0.8D0
C      PMAS(IKST10, 4) = 0D0
      MDCY(IKST10, 1) = 1
      MDCY(IKST10, 2) = IKST10DKT
      MDCY(IKST10, 3) = 10


C      PMAS(IKST1C, 1) = 1.717D0
C      PMAS(IKST1C, 2) = 0.322D0
C      PMAS(IKST1C, 3) = 0.8D0
C      PMAS(IKST1C, 4) = 0D0
      MDCY(IKST1C, 1) = 1
      MDCY(IKST1C, 2) = IKST1CDKT
      MDCY(IKST1C, 3) = 10

      DO I = 1, 10
         BRAT(I+IKST10DKT-1) = BRAT_KST10(I)
         BRAT(I+IKST1CDKT-1) = BRAT_KST1C(I)
         DO J = 1, 2
            MDME(I+IKST10DKT-1, J) = MDME_KST10(J, I)
            MDME(I+IKST1CDKT-1, J) = MDME_KST1C(J, I)
         END DO
         DO J = 1, 5
            KFDP(I+IKST10DKT-1, J) = KFDP_KST10(J, I)
            KFDP(I+IKST1CDKT-1, J) = KFDP_KST1C(J, I)
         END DO
      END DO
      END











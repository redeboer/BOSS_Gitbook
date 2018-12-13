C---------------------------------------------------------------------
C  FIST.INC            Matrix element and mixing parameters
C
C  950720  Michael Kelsey -- Extracted from FCALC.F
C  950724  Move OMEGA, HEX matrices, other parameters out of arguments
C---------------------------------------------------------------------

C  Mixing Matrix
      INTEGER FIST_MIX, FIST_NFS
      PARAMETER (FIST_MIX = 6)
      PARAMETER (FIST_NFS = 16)

      DOUBLE PRECISION MSD(FIST_MIX,FIST_MIX)
      COMPLEX OMEGA(FIST_MIX,FIST_MIX)
      COMPLEX HEX(FIST_MIX,FIST_MIX,FIST_NFS)
      COMPLEX B(FIST_MIX),DPW1(FIST_MIX),DPW2(FIST_MIX)
      INTEGER NS1

      COMMON /DFMIX/ MSD,OMEGA,HEX,B,DPW1,DPW2,NS1

C  Momentum steps and matrix elements 
      INTEGER FIST_NP
      PARAMETER (FIST_NP = 103)

      DOUBLE PRECISION H, DEM
      DOUBLE PRECISION P(FIST_NP), D(22,FIST_NP), F(8,FIST_NP)
      DOUBLE PRECISION D1(22*fist_np),F1(8*FIST_NP)
      EQUIVALENCE (D(1,1),D1(1)),(f(1,1),f1(1))
      INTEGER IPHFL

      COMMON /FIST/ H,DEM,P,D,F,IPHFL

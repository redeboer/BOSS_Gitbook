C---------------------------------------------------------------------
C  GEN.INC            Masses and center-fo-mass reference points
C
C  950720  Michael Kelsey -- Extracted from FCALC.F
C  950722  Added parameters, CM and DCM arrays
C---------------------------------------------------------------------
      INTEGER GEN_NCM, GEN_ND
      PARAMETER (GEN_NCM=9, GEN_ND=2)

      DOUBLE PRECISION A, ALPS, ALAM
      DOUBLE PRECISION CM(GEN_NCM), DCM(GEN_ND)
      DOUBLE PRECISION MASS(4,2), MP, MS, MC
      COMMON /GEN/ MASS,MP,MS,MC,A,ALPS,ALAM,CM,DCM

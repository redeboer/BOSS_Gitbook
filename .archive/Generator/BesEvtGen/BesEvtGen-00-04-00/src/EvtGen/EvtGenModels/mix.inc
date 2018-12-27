C  Local Paramters
      INTEGER N_SDM
      PARAMETER (N_SDM = FIST_MIX*(FIST_MIX+1)/2)
	    
C  Local variables
      DOUBLE PRECISION SDM(N_SDM)      !Lower diagonal matrix data
      common /mix/sdm		  

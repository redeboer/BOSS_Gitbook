// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Vector/LorentzVector.h"
	#include "TrackSelector/KKFitResult.h"
	using CLHEP::HepLorentzVector;



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/**
	 * @brief
	 */
	KKFitResult::KKFitResult()
	{
		ResetCompareMeasures();
	}

	/**
	 * @brief
	 */
	KKFitResult::KKFitResult(KalmanKinematicFit* kkmfit) :
		fFit(kkmfit)
	{
		if(fFit) SetValues(fFit);
		else     ResetCompareMeasures();
	}



// * =================================== * //
// * -------- PROTECTED METHODS -------- * //
// * =================================== * //

	/**
	 * @brief Constructof for the `KKFitResult` base class.
	 */
	void KKFitResult::SetValues(KalmanKinematicFit* kkmfit)
	{
		if(!kkmfit) return;
		fChiSquared = fFit->chisq();
		SetValues_rest();
	}


	/**
	 * @brief Comparison method.
	 */
	bool KKFitResult::IsBetter(const double &value, double &bestvalue)
	{
		/// -# Returns `false` if its own `fCompareValue` worse than `fBestCompareValue`.
		if(value > bestvalue) return false;
		/// -# If not, update `fBestCompareValue`
		bestvalue = value;
		return true;
	}